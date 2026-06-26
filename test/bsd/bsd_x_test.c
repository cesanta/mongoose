#define BSDTEST_USING_DHCP 1

#include <spawn.h>
#include <sys/wait.h>

static int s_num_tests = 0;
static bool s_error = false;
static volatile bool s_done = false, s_wait;

#ifdef NO_ABORT
static int s_abort = 0;
#define ABORT() ++s_abort, s_error = true
#else
#ifdef NO_SLEEP_ABORT
#define ABORT() abort()
#else
#define ABORT()                       \
  sleep(2); /* 2s, GH print reason */ \
  abort();
#endif
#endif

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      fflush(stdout);                                           \
      ABORT();                                                  \
    }                                                           \
  } while (0)

struct client_data {
  char hostname[64];
  uint32_t ip;
  bool done;
};

// client_task: connects to hostname:80, fetches "/", logs response length.
static void client_task(void *args) {
  int fd, c;
  struct client_data *d = (struct client_data *) args;
  char req[256];
  fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sa = {.sin_family = AF_INET, .sin_port = htons(80)};
  memcpy(&sa.sin_addr, &d->ip, 4);
  c = connect(fd, (struct sockaddr *) &sa, sizeof(sa));
  ASSERT(c >= 0);
  if (c < 0) {
    MG_ERROR(("connect failed"));
    close(fd);
    vTaskDelete(NULL);
    return;
  }
  snprintf(req, sizeof(req),"GET / HTTP/1.0\r\nHost: %s\r\n\r\n", d->hostname);
  send(fd, req, strlen(req), 0);
  size_t total = 0;
  char buf[512];
  ssize_t n;
  while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) total += (size_t) n;
  MG_INFO(("%s response: %lu bytes", d->hostname, (unsigned long) total));
  ASSERT(total > 0);
  close(fd);
  d->done = true;
  vTaskDelete(NULL);
  (void) args;
}

// echo_task: one per accepted connection.  Echoes data back.
static void echo_task(void *args) {
  int fd = (int) (uintptr_t) args;
  char buf[512];
  ssize_t n;
  while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) send(fd, buf, (size_t) n, 0);
  close(fd);
  vTaskDelete(NULL);
}

// accept loop.  Waits for incoming connections on port 1234 and spawns
// an echo_task for each one, allowing concurrent clients.
static void atask(void *args) {
  mg_bsd_init();
  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sa = {.sin_family = AF_INET, .sin_port = htons(1234), .sin_addr = {INADDR_ANY}};
  bind(lfd, (struct sockaddr *) &sa, sizeof(sa));
  listen(lfd, 5);
  fcntl(lfd, F_SETFL, O_NONBLOCK);
  MG_INFO(("BSD echo server on :1234"));
  while (!s_wait) {
    int fd = accept(lfd, NULL, NULL);
    if (fd < 0) { vTaskDelay(pdMS_TO_TICKS(10)); continue; }
    // echo_task at higher priority than task1
    xTaskCreate(echo_task, "echo", 512, (void *) (uintptr_t) fd, configMAX_PRIORITIES - 1, NULL);
  }
  close(lfd);
  s_wait = false;
  vTaskDelete(NULL);
  (void) args;
}


// task2: tests owner.
static void task2(void *args) {
  struct mg_mgr *mgr = (struct mg_mgr *)args;  // Event manager
  char *ip, *cmd[3];
  pid_t pid;
  int status;
  extern char **environ;

#define DASHBOARD(x)  printf("HEALTH_DASHBOARD\t\"%s\": %s,\n", x, s_error ? "false":"true");

  vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms
  s_error = false;
  s_wait = false;
  // atask at higher priority than task1
  xTaskCreate(atask, "atask", 256,  NULL, configMAX_PRIORITIES - 1, NULL);
  ip = mg_mprintf("%M", mg_print_ip4, &mgr->ifp->ip);
  cmd[0] = "./bsd_client";
  cmd[1] = ip;
  cmd[2] = NULL;
  ASSERT(posix_spawn(&pid, cmd[0], NULL, NULL, cmd, environ) == 0);
  for (;;) {
    pid_t r = waitpid(pid, &status, WNOHANG);
    ASSERT(r >= 0);
    if (r == pid) break;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  ASSERT(WIFEXITED(status));
  ASSERT(WEXITSTATUS(status) == 0);
  mg_free(ip);
  s_wait = true;
  while (s_wait) vTaskDelay(pdMS_TO_TICKS(10));
  DASHBOARD("accept");

  vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms
  s_error = false;
  {
    struct hostent *h = gethostbyname("invalidhostname");
    ASSERT(h == NULL);
    h = gethostbyname("mongoose.ws");
    ASSERT(h != NULL);
    if (h == NULL) {
      MG_ERROR(("DNS failed"));
       vTaskDelete(NULL);
      return;
    }
  }
  DASHBOARD("gethostbyname");

  vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms
  s_error = false;
  s_wait = true;
  {
    struct client_data cd[3];
    struct hostent *h = gethostbyname("mongoose.ws");
    strcpy(cd[0].hostname, "mongoose.ws");
    memcpy(&cd[0].ip, h->h_addr, 4);
    cd[0].done = false;
    // client_task at higher priority than task1
    xTaskCreate(client_task, "client1", 1024, &cd[0], configMAX_PRIORITIES - 1, NULL);
    h = gethostbyname("cesanta.com");
    strcpy(cd[1].hostname, "cesanta.com");
    memcpy(&cd[1].ip, h->h_addr, 4);
    cd[1].done = false;
    // client_task at higher priority than task1
    xTaskCreate(client_task, "client2", 1024, &cd[1], configMAX_PRIORITIES - 1, NULL);
    h = gethostbyname("google.com");
    strcpy(cd[2].hostname, "google.com");
    memcpy(&cd[2].ip, h->h_addr, 4);
    cd[2].done = false;
    // client_task at higher priority than task1
    xTaskCreate(client_task, "client3", 1024, &cd[2], configMAX_PRIORITIES - 1, NULL);
    while (!cd[0].done || !cd[1].done || !cd[2].done) vTaskDelay(pdMS_TO_TICKS(10));
  }
  DASHBOARD("connect");
  // End task 1
  s_done = true;
  while (s_done) vTaskDelay(pdMS_TO_TICKS(10));

  // Clear
  s_error = false;
  mg_mgr_free(mgr);
  ASSERT(mgr->conns == NULL);  // Deconstruction OK
  printf("HEALTH_DASHBOARD\t\"cleanup\": %s\n", s_error ? "false":"true");
 // last entry with no comma
  vTaskEndScheduler();
}

// task1: network owner.  Runs the Mongoose event loop and the BSD command queue.
// The interface needs to be started here, as FreeRTOS needs to be running (time base)
static void task1(void *args) {
  struct mg_mgr *mgr = (struct mg_mgr *)args;  // Event manager

  // Stack initialization, Network configuration (DHCP lease, ...)
#if BSDTEST_USING_DHCP == 0
  MG_INFO(("MIF configuration: Static IP"));
  ASSERT(mif.ip != 0);     // Check we have a satic IP assigned
  mg_mgr_poll(&mgr, 0);  // For initialisation
#else
  MG_INFO(("MIF configuration: DHCP"));
  ASSERT(mgr->ifp->ip == 0);  // Check we are set for DHCP
  while (mgr->ifp->ip == 0 && mg_millis() < 5000) {
    mg_mgr_poll(mgr, 0);
  }
  if (mgr->ifp->ip == 0) MG_ERROR(("No ip assigned (DHCP lease may have failed).\n"));
  ASSERT(mgr->ifp->ip != 0);  // We have an IP (lease or static)
#endif
  while (mgr->ifp->state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(mgr, 0);
  }
  MG_INFO(("Interface started, starting main loop"));
  xTaskCreate(task2, "task2", 256,  mgr, configMAX_PRIORITIES - 2, NULL);

  while (!s_done) {
    mg_mgr_poll(mgr, 0);
    mg_bsd_poll(mgr);
  }
  s_done = false;
  vTaskDelete(NULL);
}

static bool bsd_x_test(struct mg_tcpip_driver *driver, void *driver_data, const char *mac, const char *debug_level) {
  if (debug_level == NULL) debug_level = "3";
  mg_log_set(atoi(debug_level));

  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager

  struct mg_tcpip_if mif;
  memset(&mif, 0, sizeof(mif));

  mif.driver = driver;
  mif.driver_data = driver_data;

#if BSDTEST_USING_DHCP == 1
#else
  mif.ip = mg_htonl(MG_U32(192, 168, 32, 2));  // Triggering a network failure
  mif.mask = mg_htonl(MG_U32(255, 255, 255, 0));
  mif.gw = mg_htonl(MG_U32(192, 168, 32, 1));
#endif

  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mif.mac[0], &mif.mac[1],
         &mif.mac[2], &mif.mac[3], &mif.mac[4], &mif.mac[5]);

  mg_tcpip_init(&mgr, &mif);
  ASSERT(mgr.ifp != NULL);  // driver init succeeded
  MG_INFO(("Init done, starting interface"));
  usleep(200000);  // 200 ms

  xTaskCreate(task1, "task1", 2048, &mgr, configMAX_PRIORITIES - 2, NULL);
  vTaskStartScheduler();

#ifdef NO_ABORT
  if (s_abort != 0) return false;
#endif

  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return true;
}
