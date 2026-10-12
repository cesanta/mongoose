#define BSDTEST_USING_DHCP 1

#include <errno.h>
#include <spawn.h>
#include <sys/wait.h>

static int s_num_tests = 0;
static bool s_error = false;

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

#define DASHBOARD(x) \
  printf("HEALTH_DASHBOARD\t\"%s\": %s,\n", x, s_error ? "false" : "true")

static pid_t start_client(const char *ip, const char *connections) {
  char *cmd[4];
  pid_t pid;
  extern char **environ;

  cmd[0] = (char *) "./bsd_client";
  cmd[1] = (char *) ip;
  cmd[2] = (char *) connections;
  cmd[3] = NULL;
  ASSERT(posix_spawn(&pid, cmd[0], NULL, NULL, cmd, environ) == 0);
  return pid;
}

static bool client_done(pid_t pid, int *status) {
  pid_t r = waitpid(pid, status, WNOHANG);
  ASSERT(r >= 0);
  return r == pid;
}

static void check_client(int status) {
  ASSERT(WIFEXITED(status));
  ASSERT(WEXITSTATUS(status) == 0);
}

static int open_listener(int backlog, bool nonblock) {
  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sa;

  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(1234);
  sa.sin_addr.s_addr = INADDR_ANY;
  ASSERT(lfd >= 0);
  ASSERT(bind(lfd, (struct sockaddr *) &sa, sizeof(sa)) == 0);
  ASSERT(listen(lfd, backlog) == 0);
  if (nonblock) ASSERT(fcntl(lfd, F_SETFL, O_NONBLOCK) == 0);
  else ASSERT((fcntl(lfd, F_GETFL, 0) & O_NONBLOCK) == 0);
  MG_INFO(("BSD echo server on :1234"));
  return lfd;
}

static void wait_ready(struct mg_mgr *mgr) {
  // Stack initialization, Network configuration (DHCP lease, ...)
#if BSDTEST_USING_DHCP == 0
  MG_INFO(("MIF configuration: Static IP"));
  ASSERT(mgr->ifp->ip != 0);  // Check we have a satic IP assigned
  mg_mgr_poll(mgr, 0);  // For initialisation
#else
  MG_INFO(("MIF configuration: DHCP"));
  ASSERT(mgr->ifp->ip == 0);  // Check we are set for DHCP
  while (mgr->ifp->ip == 0 && mg_millis() < 5000) mg_mgr_poll(mgr, 0);
  if (mgr->ifp->ip == 0) MG_ERROR(("No ip assigned (DHCP lease may have failed).\n"));
  ASSERT(mgr->ifp->ip != 0);  // We have an IP (lease or static)
#endif
  while (mgr->ifp->state != MG_TCPIP_STATE_READY) mg_mgr_poll(mgr, 0);
  MG_INFO(("Interface started, starting main loop"));
}

#if !MG_ENABLE_FREERTOS
static bool s_fail_alloc;

void *mg_calloc(size_t count, size_t size) {
  return s_fail_alloc ? NULL : calloc(count, size);
}

void mg_free(void *ptr) {
  free(ptr);
}

static uint64_t s_bootmillis = 0;

uint64_t mg_millis(void) {
  struct timespec ts = {0, 0};
  // See #1615 - prefer monotonic clock
#if defined(CLOCK_MONOTONIC_RAW)
  // Raw hardware-based time that is not subject to NTP adjustment
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
#elif defined(CLOCK_MONOTONIC)
  // Affected by the incremental adjustments performed by adjtime and NTP
  clock_gettime(CLOCK_MONOTONIC, &ts);
#else
  // Affected by discontinuous jumps in the system time and by the incremental
  // adjustments performed by adjtime and NTP
  clock_gettime(CLOCK_REALTIME, &ts);
#endif
  return ((uint64_t) ts.tv_sec * 1000 + (uint64_t) ts.tv_nsec / 1000000) - s_bootmillis;
}

#endif

#if MG_ENABLE_FREERTOS
static volatile bool s_done = false, s_wait, s_accept_waiting;
static volatile bool s_accept_done, s_dontwait_done, s_nonblock_done;
static volatile bool s_select_waiting, s_select_done;
static volatile bool s_poll_waiting, s_poll_done;
static volatile bool s_ready_waiting, s_ready_done;
static volatile bool s_backlog_ready, s_backlog_full, s_backlog_accept;
static volatile bool s_tx_held, s_tx_waits, s_tx_fail, s_tx_error;
static volatile bool s_send_waiting;
static volatile bool s_send_done, s_send_expect_error;
static volatile bool s_recv_start, s_recv_waiting, s_recv_done;
static volatile bool s_recv_expect_error;
static size_t (*s_tx)(const void *, size_t, struct mg_tcpip_if *);

struct client_data {
  char hostname[64];
  uint32_t ip;
  bool done;
};

static void echo_task(void *args);
static void echo_dontwait_task(void *args);
static void echo_nonblock_task(void *args);
static void send_task(void *args);
static void recv_wait_task(void *args);
static void select_task(void *args);
static void readiness_task(void *args);
static void backlog_task(void *args);
static void tx_gate_task(void *args);

struct atask_args {
  bool listener_nonblock;
  bool worker_nonblock;
  bool once;
  bool receiver;
  TaskFunction_t task;
  TaskFunction_t gate;
};

static const struct atask_args s_echo_server = {true,  false,     false,
                                                false, echo_task, NULL};
static const struct atask_args s_accept_server = {false, false,     true,
                                                  false, echo_task, NULL};
static const struct atask_args s_nonblock_accept_server = {
    true, false, true, false, echo_task, NULL};
static const struct atask_args s_dontwait_server = {
    true, false, false, false, echo_dontwait_task, NULL};
static const struct atask_args s_nonblock_server = {
    true, true, false, false, echo_nonblock_task, NULL};
static const struct atask_args s_send_server = {true, false,     false,
                                                true, send_task, tx_gate_task};
static const struct atask_args s_send_error_server = {
    true, false, false, false, send_task, tx_gate_task};

static size_t tx_gate(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  if (s_tx_fail) return (size_t) -1;  // Force MG_IO_ERR after send() waits
  if (s_tx_held) {
    s_tx_waits = true;
    return 0;  // Gate TX: MG_IO_WAIT leaves c->send queued
  }
  return s_tx(buf, len, ifp);
}

static void run_client(const char *ip, const char *connections) {
  int status;
  pid_t pid = start_client(ip, connections);

  while (!client_done(pid, &status)) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  check_client(status);
}

// client_task: connects to hostname:80, fetches "/", logs response length.
static void client_task(void *args) {
  fd_set w, e;
  struct pollfd p;
  int fd;
  struct client_data *d = (struct client_data *) args;
  char req[256];
  fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sa;
  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(80);
  memcpy(&sa.sin_addr, &d->ip, 4);
  ASSERT(fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
  ASSERT(connect(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0 &&
         errno == EINPROGRESS);
  FD_ZERO(&w);
  FD_ZERO(&e);
  FD_SET(fd, &w);
  FD_SET(fd, &e);
  ASSERT(select(fd + 1, NULL, &w, &e, NULL) == 1);
  ASSERT(FD_ISSET(fd, &w));
  ASSERT(!FD_ISSET(fd, &e));
  p.fd = fd, p.events = POLLOUT, p.revents = 0;
  ASSERT(poll(&p, 1, 0) == 1);
  ASSERT(p.revents == POLLOUT);
  ASSERT(fcntl(fd, F_SETFL, 0) == 0);
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

// Echo data back from one accepted connection.
static void echo_task_common(int fd, int flags, volatile bool *done) {
  bool nonblock = (fcntl(fd, F_GETFL, 0) & O_NONBLOCK) != 0;
  bool dontwait = (flags & MSG_DONTWAIT) != 0;
  bool echoed = false, eagain = false;
  static const size_t sizes[] = {1, 7, 17, 255, 256, 257, 511, 512};
  char buf[512];
  size_t i = 0;
  ssize_t n;

  MG_INFO(("Echo task started: %d", fd));
  for (;;) {
    n = recv(fd, buf, sizes[i++ % (sizeof(sizes) / sizeof(sizes[0]))], flags);
    if (n > 0) {
      if (!nonblock) {
        ssize_t w = send(fd, buf, (size_t) n, 0);
        if (done != NULL) ASSERT(w == n);
        echoed = w == n;
      } else {
        size_t sent = 0;
        while (sent < (size_t) n) {
          ssize_t w = send(fd, buf + sent, (size_t) n - sent, 0);
          if (w > 0) sent += (size_t) w;
          else if (w < 0 && errno == EAGAIN) vTaskDelay(1);
          else {
            ASSERT(w > 0);
            break;
          }
        }
        if (sent != (size_t) n) break;
        echoed = true;
      }
    } else if ((nonblock || dontwait) && n < 0 && errno == EAGAIN) {
      if (echoed) eagain = true;
      vTaskDelay(1);
    } else {
      break;
    }
  }
  if (done != NULL) {
    ASSERT(eagain);
    *done = true;
  }
  close(fd);
  vTaskDelete(NULL);
}

static void echo_task(void *args) {
  echo_task_common((int) (uintptr_t) args, 0, NULL);
}

static void echo_dontwait_task(void *args) {
  echo_task_common((int) (uintptr_t) args, MSG_DONTWAIT, &s_dontwait_done);
}

static void echo_nonblock_task(void *args) {
  echo_task_common((int) (uintptr_t) args, 0, &s_nonblock_done);
}

static void tx_gate_task(void *args) {
  // receiver/sender > network task > gate: final send is waiting
  while (!s_send_waiting) vTaskDelay(1);
  ASSERT(!s_send_done);
  if (s_send_expect_error) s_tx_error = s_tx_fail = true;
  else s_tx_held = false;
  vTaskDelete(NULL);
  (void) args;
}

static void recv_wait_task(void *args) {
  char buf;
  int fd = (int) (uintptr_t) args;
  ssize_t n;

  while (!s_recv_start) vTaskDelay(1);
  s_recv_waiting = true;  // send_task starts after recv() is waiting
  n = recv(fd, &buf, sizeof(buf), 0);
  if (s_recv_expect_error) {
    ASSERT(n < 0 && errno == EIO);  // bsd_client aborts with unread data
  } else {
    ASSERT(n == 0);  // bsd_client drains, then closes
  }
  s_recv_done = true;
  close(fd);  // Receiver owns the shared socket close
  vTaskDelete(NULL);
}

static void send_task(void *args) {
  static const char buf[512] = {0};
  static const char marker = '\1';
  int fd = (int) (uintptr_t) args;
  ssize_t n;

  ASSERT(fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
  s_tx_held = true;  // Gate TX until c->send fills
  do {
    n = send(fd, buf, sizeof(buf), 0);
  } while (n == (ssize_t) sizeof(buf));
  ASSERT(n < 0 && errno == EAGAIN);  // Full c->send: O_NONBLOCK
  while (!s_tx_waits) vTaskDelay(1);  // Next write_conn hits the gate

  ASSERT(fcntl(fd, F_SETFL, 0) == 0);
  n = send(fd, buf, sizeof(buf), MSG_DONTWAIT);
  ASSERT(n < 0 && errno == EAGAIN);  // Same full c->send: MSG_DONTWAIT

  if (!s_send_expect_error) {
    s_recv_start = true;  // O_NONBLOCK is per socket; now start the receiver
    while (!s_recv_waiting) vTaskDelay(1);
  }
  s_send_waiting = true;  // Gate task releases TX after this send blocks
  n = send(fd, &marker, sizeof(marker), 0);
  if (s_send_expect_error) ASSERT(n < 0 && errno == EPIPE);
  else ASSERT(n == (ssize_t) sizeof(marker));
  s_send_done = true;
  vTaskDelete(NULL);
}

// accept loop.  Waits for incoming connections on port 1234 and spawns
// an echo_task for each one, allowing concurrent clients.
static void atask(void *args) {
  const struct atask_args *cfg = (const struct atask_args *) args;
  BaseType_t result;
  int lfd = open_listener(5, cfg->listener_nonblock);
  if (cfg->once && cfg->listener_nonblock) {
    ASSERT(accept(lfd, NULL, NULL) < 0 && errno == EAGAIN);
    s_accept_waiting = true;
  }
  while (!s_wait) {
    int fd;

    if (cfg->once && !cfg->listener_nonblock) s_accept_waiting = true;
    fd = accept(lfd, NULL, NULL);
    if (fd < 0) {
      if (!cfg->listener_nonblock) {
        ASSERT(fd >= 0);
        break;
      }
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }
    // Accepted sockets start blocking, regardless of listener flags
    ASSERT((fcntl(fd, F_GETFL, 0) & O_NONBLOCK) == 0);
    if (cfg->worker_nonblock) ASSERT(fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
    // Worker tasks run at higher priority than task1
    if (cfg->receiver) {
      result = xTaskCreate(recv_wait_task, "recv", 512,
                           (void *) (uintptr_t) fd,
                           configMAX_PRIORITIES - 1, NULL);
      ASSERT(result == pdPASS);
    }
    if (cfg->gate != NULL) {
      result = xTaskCreate(cfg->gate, "gate", 256, NULL,
                           configMAX_PRIORITIES - 3, NULL);
      ASSERT(result == pdPASS);
    }
    result = xTaskCreate(cfg->task, "echo", 512, (void *) (uintptr_t) fd,
                         configMAX_PRIORITIES - 1, NULL);
    MG_INFO(("Echo task %d: %s", fd, result == pdPASS ? "created" : "FAILED"));
    ASSERT(result == pdPASS);
    if (cfg->once) break;
  }
  close(lfd);
  if (cfg->once) s_accept_done = true;
  else s_wait = false;
  vTaskDelete(NULL);
}

static void select_task(void *args) {
  struct timeval zero = {0, 0};
  fd_set r, w, e;
  int fd, lfd, n;

  (void) args;
  lfd = open_listener(5, true);
  FD_ZERO(&r);
  FD_SET(lfd, &r);
  ASSERT(select(lfd, &r, NULL, NULL, &zero) == 0);
  ASSERT(!FD_ISSET(lfd, &r));
  FD_ZERO(&r);
  FD_SET(lfd + 1, &r);
  ASSERT(select(lfd + 2, &r, NULL, NULL, &zero) < 0 && errno == EBADF);
  FD_ZERO(&r);
  FD_ZERO(&w);
  FD_ZERO(&e);
  FD_SET(lfd, &r);
  FD_SET(lfd, &w);
  FD_SET(lfd, &e);
  s_select_waiting = true;
  n = select(lfd + 1, &r, &w, &e, NULL);
  ASSERT(n == 1);
  ASSERT(FD_ISSET(lfd, &r));
  ASSERT(!FD_ISSET(lfd, &w));
  ASSERT(!FD_ISSET(lfd, &e));
  fd = accept(lfd, NULL, NULL);
  ASSERT(fd >= 0);
  ASSERT(close(fd) == 0);
  ASSERT(close(lfd) == 0);
  s_select_done = true;
  vTaskDelete(NULL);
}

static void poll_task(void *args) {
  struct pollfd p;
  int fd, lfd;

  (void) args;
  lfd = open_listener(5, true);
  p.fd = lfd, p.events = POLLIN, p.revents = 0;
  ASSERT(poll(&p, 1, 0) == 0);
  ASSERT(p.revents == 0);
  p.fd = lfd + 1, p.revents = 0;
  ASSERT(poll(&p, 1, 0) == 1);
  ASSERT(p.revents == POLLNVAL);
  p.fd = lfd, p.revents = 0;
  s_poll_waiting = true;
  ASSERT(poll(&p, 1, -1) == 1);
  ASSERT(p.revents == POLLIN);
  fd = accept(lfd, NULL, NULL);
  ASSERT(fd >= 0);
  ASSERT(close(fd) == 0);
  ASSERT(close(lfd) == 0);
  s_poll_done = true;
  vTaskDelete(NULL);
}

static void readiness_task(void *args) {
  struct pollfd p;
  fd_set r, w;
  int fd, lfd;

  (void) args;
  lfd = open_listener(5, true);
  s_ready_waiting = true;
  while ((fd = accept(lfd, NULL, NULL)) < 0) {
    ASSERT(errno == EAGAIN);
    vTaskDelay(1);
  }
  ASSERT(fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
  FD_ZERO(&r);
  FD_SET(fd, &r);
  ASSERT(select(fd + 1, &r, NULL, NULL, NULL) == 1);
  ASSERT(FD_ISSET(fd, &r));
  p.fd = fd, p.events = POLLIN, p.revents = 0;
  ASSERT(poll(&p, 1, 0) == 1);
  ASSERT(p.revents == POLLIN);
  FD_ZERO(&w);
  FD_SET(fd, &w);
  ASSERT(select(fd + 1, NULL, &w, NULL, NULL) == 1);
  ASSERT(FD_ISSET(fd, &w));
  p.events = POLLOUT, p.revents = 0;
  ASSERT(poll(&p, 1, 0) == 1);
  ASSERT(p.revents == POLLOUT);
  ASSERT(close(lfd) == 0);
  echo_task_common(fd, 0, &s_ready_done);
}

static void backlog_task(void *args) {
  struct pollfd p;
  BaseType_t result;
  int fd, lfd;

  (void) args;
  lfd = open_listener(1, true);
  p.fd = lfd, p.events = POLLIN, p.revents = 0;
  s_backlog_ready = true;
  while (poll(&p, 1, 0) == 0) vTaskDelay(1);
  ASSERT(p.revents == POLLIN);
  s_backlog_full = true;  // One orphan occupies the BSD backlog
  while (!s_backlog_accept) vTaskDelay(1);
  ASSERT((fd = accept(lfd, NULL, NULL)) >= 0);
  result = xTaskCreate(echo_task, "echo", 512, (void *) (uintptr_t) fd,
                       configMAX_PRIORITIES - 1, NULL);
  ASSERT(result == pdPASS);
  ASSERT(close(lfd) == 0);
  vTaskDelete(NULL);
}

// task2: tests owner.
static void task2(void *args) {
  struct mg_mgr *mgr = (struct mg_mgr *)args;  // Event manager
  char *ip;
  int status;
  pid_t first, second;

  vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms
  s_error = false;
  s_wait = false;
  ip = mg_mprintf("%M", mg_print_ip4, &mgr->ifp->ip);
  s_poll_waiting = s_poll_done = false;
  ASSERT(xTaskCreate(poll_task, "poll", 512, NULL,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  while (!s_poll_waiting) vTaskDelay(1);
  ASSERT(!s_poll_done);
  run_client(ip, "hold");
  while (!s_poll_done) vTaskDelay(1);
  DASHBOARD("poll_read");

  s_select_waiting = s_select_done = false;
  ASSERT(xTaskCreate(select_task, "select", 512, NULL,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  while (!s_select_waiting) vTaskDelay(1);
  ASSERT(!s_select_done);
  run_client(ip, "hold");
  while (!s_select_done) vTaskDelay(1);
  DASHBOARD("select_read");

  s_error = false;
  s_ready_waiting = s_ready_done = false;
  ASSERT(xTaskCreate(readiness_task, "ready", 512, NULL,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  while (!s_ready_waiting) vTaskDelay(1);
  run_client(ip, "1");
  while (!s_ready_done) vTaskDelay(1);
  DASHBOARD("select_child_read");
  DASHBOARD("poll_child_read");
  DASHBOARD("select_child_write");
  DASHBOARD("poll_child_write");

  #if 0
  s_error = false;
  s_backlog_ready = s_backlog_full = s_backlog_accept = false;
  ASSERT(xTaskCreate(backlog_task, "backlog", 512, NULL,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  while (!s_backlog_ready) vTaskDelay(1);
  first = start_client(ip, "1");  // Fills listen(fd, 1)'s pending accept
  while (!s_backlog_full) vTaskDelay(1);
  second = start_client(ip, "expectclose");  // Rejected over the backlog
  while (!client_done(second, &status)) vTaskDelay(1);
  check_client(status);
  s_backlog_accept = true;
  while (!client_done(first, &status)) vTaskDelay(1);
  check_client(status);
  DASHBOARD("listen_backlog");
#else
    (void) first;
    (void) second;
    (void) status;
    (void) backlog_task;
#endif

  s_error = false;
  // atask at higher priority than task1
  s_accept_waiting = s_accept_done = false;
  ASSERT(xTaskCreate(atask, "atask", 256, (void *) &s_accept_server,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  while (!s_accept_waiting) vTaskDelay(pdMS_TO_TICKS(10));
  ASSERT(!s_accept_done);
  run_client(ip, "1");
  while (!s_accept_done) vTaskDelay(pdMS_TO_TICKS(10));
  DASHBOARD("blocking_accept");

  s_error = false;
  s_wait = false;
  s_accept_waiting = s_accept_done = false;
  ASSERT(xTaskCreate(atask, "atask", 256, (void *) &s_nonblock_accept_server,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  while (!s_accept_waiting) vTaskDelay(pdMS_TO_TICKS(10));
  run_client(ip, "1");
  while (!s_accept_done) vTaskDelay(pdMS_TO_TICKS(10));
  DASHBOARD("nonblocking_accept");

  s_error = false;
  s_wait = false;
  ASSERT(xTaskCreate(atask, "atask", 256, (void *) &s_echo_server,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  run_client(ip, NULL);
  s_wait = true;
  while (s_wait) vTaskDelay(pdMS_TO_TICKS(10));
  DASHBOARD("accept");

  s_error = false;
  s_wait = false;
  s_dontwait_done = false;
  ASSERT(xTaskCreate(atask, "atask", 256, (void *) &s_dontwait_server,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  run_client(ip, "1");
  while (!s_dontwait_done) vTaskDelay(pdMS_TO_TICKS(10));
  s_wait = true;
  while (s_wait) vTaskDelay(pdMS_TO_TICKS(10));
  DASHBOARD("dontwait_recv");

  s_error = false;
  s_wait = false;
  s_nonblock_done = false;
  ASSERT(xTaskCreate(atask, "atask", 256, (void *) &s_nonblock_server,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  run_client(ip, "1");
  while (!s_nonblock_done) vTaskDelay(pdMS_TO_TICKS(10));
  s_wait = true;
  while (s_wait) vTaskDelay(pdMS_TO_TICKS(10));
  DASHBOARD("nonblocking_recv");

  s_error = false;
  s_wait = s_tx_held = s_tx_waits = s_tx_fail = s_tx_error = false;
  s_send_waiting = false;
  s_send_done = s_send_expect_error = false;
  s_recv_start = s_recv_waiting = s_recv_done = s_recv_expect_error = false;
  ASSERT(xTaskCreate(atask, "atask", 256, (void *) &s_send_server,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  run_client(ip, "waitanddrain");  // Peer drains through the final marker
  ASSERT(s_send_done);
  DASHBOARD("nonblocking_send");
  DASHBOARD("blocking_send");
  while (!s_recv_done) vTaskDelay(pdMS_TO_TICKS(10));
  DASHBOARD("blocking_recv");
  DASHBOARD("shared_socket");
  s_wait = true;
  while (s_wait) vTaskDelay(pdMS_TO_TICKS(10));

  s_error = false;
  s_wait = s_tx_held = s_tx_waits = s_tx_fail = s_tx_error = false;
  s_send_waiting = false;
  s_send_done = s_send_expect_error = false;
  s_recv_start = s_recv_waiting = s_recv_done = false;
  s_recv_expect_error = true;
  ASSERT(xTaskCreate(atask, "atask", 256, (void *) &s_send_server,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  run_client(ip, "waitandabort");  // Peer leaves data unread and sends RST
  ASSERT(s_send_done);
  while (!s_recv_done) vTaskDelay(pdMS_TO_TICKS(10));
  DASHBOARD("error_wakeup");
  s_wait = true;
  while (s_wait) vTaskDelay(pdMS_TO_TICKS(10));

  s_error = false;
  s_wait = s_tx_held = s_tx_waits = s_tx_fail = s_tx_error = false;
  s_send_waiting = false;
  s_send_done = false;
  s_send_expect_error = true;
  ASSERT(xTaskCreate(atask, "atask", 256, (void *) &s_send_error_server,
                     configMAX_PRIORITIES - 1, NULL) == pdPASS);
  run_client(ip, "hold");  // Peer stays connected while the driver fails TX
  while (!s_tx_error) vTaskDelay(pdMS_TO_TICKS(10));
  ASSERT(s_send_done);
  DASHBOARD("send_error_wakeup");
  s_wait = true;
  while (s_wait) vTaskDelay(pdMS_TO_TICKS(10));
  s_tx_held = s_tx_fail = s_tx_error = s_send_expect_error = false;

  mg_free(ip);

  s_error = false;
  {
    fd_set e;
    struct pollfd p;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(1234);
    ASSERT(fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
    ASSERT(connect(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0 &&
           errno == EINPROGRESS);
    FD_ZERO(&e);
    FD_SET(fd, &e);
    ASSERT(select(fd + 1, NULL, NULL, &e, NULL) == 1);
    ASSERT(FD_ISSET(fd, &e));
    p.fd = fd, p.events = 0, p.revents = 0;
    ASSERT(poll(&p, 1, -1) == 1);
    ASSERT((p.revents & POLLERR) != 0);
    p.events = POLLOUT, p.revents = 0;
    ASSERT(poll(&p, 1, 0) == 1);
    ASSERT((p.revents & POLLERR) != 0);
    ASSERT((p.revents & POLLOUT) == 0);
    ASSERT(close(fd) == 0);
  }
  DASHBOARD("select_error");
  DASHBOARD("poll_error");

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
  DASHBOARD("select_write");
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

  wait_ready(mgr);
  xTaskCreate(task2, "task2", 256,  mgr, configMAX_PRIORITIES - 2, NULL);

  while (!s_done) {
    mg_mgr_poll(mgr, 0);
    vTaskDelay(1);  // Let idle reclaim deleted echo tasks
  }
  s_done = false;
  vTaskDelete(NULL);
}

#else

#define NORTOS_ECHO_CLIENTS 3

struct nortos_echo_client {
  int fd;
  size_t len, sent;
  bool eof;
  char buf[512];
};

static int nortos_accept(struct mg_mgr *mgr, int lfd) {
  int fd;

  while ((fd = accept(lfd, NULL, NULL)) < 0) {
    ASSERT(errno == EAGAIN);
    mg_mgr_poll(mgr, 0);
  }
  return fd;
}

static void nortos_wait_client(struct mg_mgr *mgr, pid_t pid) {
  int status;

  while (!client_done(pid, &status)) mg_mgr_poll(mgr, 0);
  check_client(status);
}

static void nortos_echo(struct mg_mgr *mgr, struct nortos_echo_client *clients,
                        size_t count, pid_t pid) {
  struct pollfd p[NORTOS_ECHO_CLIENTS];
  fd_set r;
  size_t i, live = count;
  int maxfd, result;
  ssize_t n;

  while (live > 0) {
    for (i = 0; i < count; i++) {
      struct nortos_echo_client *c = &clients[i];
      if (!c->eof && c->sent < c->len) {
        n = send(c->fd, c->buf + c->sent, c->len - c->sent, 0);
        if (n > 0) c->sent += (size_t) n;
        else ASSERT(n < 0 && errno == EAGAIN);
      }
    }

    FD_ZERO(&r);
    maxfd = -1;
    for (i = 0; i < count; i++) {
      struct nortos_echo_client *c = &clients[i];
      p[i].fd = -1, p[i].events = POLLIN, p[i].revents = 0;
      if (!c->eof && c->sent == c->len) {
        p[i].fd = c->fd;
        FD_SET(c->fd, &r);
        if (c->fd > maxfd) maxfd = c->fd;
      }
    }
    if (maxfd >= 0) {
      result = poll(p, (nfds_t) count, 0);
      ASSERT(result >= 0);
      if (result > 0) {
        ASSERT(select(maxfd + 1, &r, NULL, NULL, NULL) == result);
        for (i = 0; i < count; i++) {
          struct nortos_echo_client *c = &clients[i];
          bool readable = (p[i].revents & POLLIN) != 0;

          ASSERT(p[i].revents == (readable ? POLLIN : 0));
          ASSERT(FD_ISSET(c->fd, &r) == readable);
          if (readable) {
            n = recv(c->fd, c->buf, sizeof(c->buf), 0);
            ASSERT(n >= 0);
            if (n > 0) c->len = (size_t) n, c->sent = 0;
            else c->eof = true, live--;
          }
        }
      }
    }
    mg_mgr_poll(mgr, 0);
  }
  nortos_wait_client(mgr, pid);
}

static void nortos_test(struct mg_mgr *mgr) {
  static const char bootstrap = '\0';
  static const char grow[512] = {0};
  static const char marker = '\1';
  struct sockaddr_in sa;
  struct timeval zero = {0, 0};
  struct pollfd p;
  fd_set r, w, e;
  char buf;
  char *ip;
  pid_t pid;
  size_t i, j;
  int fd, lfd, n;
  struct nortos_echo_client clients[NORTOS_ECHO_CLIENTS];

  ip = mg_mprintf("%M", mg_print_ip4, &mgr->ifp->ip);
  ASSERT(ip != NULL);
  lfd = open_listener(5, true);
  p.fd = lfd, p.events = POLLIN, p.revents = 0;
  ASSERT(poll(&p, 1, 10) == 0);
  ASSERT(p.revents == 0);
  p.fd = lfd + 1, p.revents = 0;
  ASSERT(poll(&p, 1, 10) == 1);
  ASSERT(p.revents == POLLNVAL);
  p.fd = lfd, p.revents = 0;
  FD_ZERO(&r);
  FD_SET(lfd, &r);
  ASSERT(select(lfd, &r, NULL, NULL, &zero) == 0);
  ASSERT(!FD_ISSET(lfd, &r));
  FD_ZERO(&r);
  FD_SET(lfd + 1, &r);
  ASSERT(select(lfd + 2, &r, NULL, NULL, &zero) < 0 && errno == EBADF);
  ASSERT(accept(lfd, NULL, NULL) < 0 && errno == EAGAIN);
  DASHBOARD("nortos_nonblocking_accept");

  pid = start_client(ip, "waitanddrain");
  do {
    p.revents = 0;
    n = poll(&p, 1, 10);
    if (n == 0) mg_mgr_poll(mgr, 0);
  } while (n == 0);
  ASSERT(n == 1);
  ASSERT(p.revents == POLLIN);
  DASHBOARD("nortos_poll_read");
  do {
    FD_ZERO(&r);
    FD_ZERO(&w);
    FD_ZERO(&e);
    FD_SET(lfd, &r);
    FD_SET(lfd, &w);
    FD_SET(lfd, &e);
    n = select(lfd + 1, &r, &w, &e, NULL);
    if (n == 0) mg_mgr_poll(mgr, 0);
  } while (n == 0);
  ASSERT(n == 1);
  ASSERT(FD_ISSET(lfd, &r));
  ASSERT(!FD_ISSET(lfd, &w));
  ASSERT(!FD_ISSET(lfd, &e));
  DASHBOARD("nortos_select_read");
  fd = nortos_accept(mgr, lfd);
  ASSERT(recv(fd, &buf, sizeof(buf), MSG_DONTWAIT) < 0 && errno == EAGAIN);
  DASHBOARD("nortos_dontwait_recv");
  ASSERT(fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
  ASSERT(recv(fd, &buf, sizeof(buf), 0) < 0 && errno == EAGAIN);
  DASHBOARD("nortos_nonblocking_recv");

  ASSERT(send(fd, &bootstrap, sizeof(bootstrap), 0) ==
         (ssize_t) sizeof(bootstrap));
  s_fail_alloc = true;  // Fail growth after c->send has been bootstrapped
  ASSERT(send(fd, grow, sizeof(grow), 0) < 0 && errno == EAGAIN);
  s_fail_alloc = false;
  ASSERT(send(fd, &marker, sizeof(marker), 0) == (ssize_t) sizeof(marker));
  nortos_wait_client(mgr, pid);
  ASSERT(close(fd) == 0);
  DASHBOARD("nortos_nonblocking_send");

  memset(clients, 0, sizeof(clients));
  pid = start_client(ip, "3");
  for (i = 0; i < NORTOS_ECHO_CLIENTS; i++) {
    clients[i].fd = nortos_accept(mgr, lfd);
    ASSERT(fcntl(clients[i].fd, F_SETFL, O_NONBLOCK) == 0);
    for (j = 0; j < i; j++) ASSERT(clients[i].fd != clients[j].fd);
  }
  nortos_echo(mgr, clients, NORTOS_ECHO_CLIENTS, pid);
  for (i = 0; i < NORTOS_ECHO_CLIENTS; i++)
    ASSERT(close(clients[i].fd) == 0);
  ASSERT(close(lfd) == 0);
  DASHBOARD("nortos_select_children");
  DASHBOARD("nortos_poll_children");
  DASHBOARD("nortos_echo");

  fd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(1234);
  ASSERT(fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
  ASSERT(connect(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0 &&
         errno == EINPROGRESS);
  ASSERT(close(fd) == 0);
  mg_mgr_poll(mgr, 0);
  DASHBOARD("nortos_nonblocking_connect");
  mg_free(ip);
}

#endif  // MG_ENABLE_FREERTOS

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

#if MG_ENABLE_FREERTOS
  s_tx = driver->tx;
  driver->tx = tx_gate;
  xTaskCreate(task1, "task1", 2048, &mgr, configMAX_PRIORITIES - 2, NULL);
  vTaskStartScheduler();
  driver->tx = s_tx;
#else
  s_bootmillis = mg_millis();
  wait_ready(&mgr);
  nortos_test(&mgr);
  s_error = false;
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
  printf("HEALTH_DASHBOARD\t\"cleanup\": %s\n", s_error ? "false" : "true");
#endif

#ifdef NO_ABORT
  if (s_abort != 0) return false;
#endif

  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return true;
}
