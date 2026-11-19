#include <errno.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "FreeRTOS.h"
#include "task.h"

volatile bool s_cpq_done;
volatile bool s_cpq_ready;
bool s_cpq_remote;
static bool s_cpq_ok;

#include "bsd_udp_cpq_fail.c"

static void cpq_log(char ch, void *param) {
  (void) param;
  putchar(ch);
}

static void cpq_task1(void *args) {
  struct mg_mgr *mgr = (struct mg_mgr *) args;
  const char *remote_peer = getenv("CPQ_REMOTE_PEER");
  char *ip = NULL;
  char *argv[5];
  pid_t pid;
  pid_t r;
  int status;
  extern char **environ;

  while (mgr->ifp->ip == 0 && mg_millis() < 5000) {
    mg_mgr_poll(mgr, 0);
    vTaskDelay(1);
  }
  if (mgr->ifp->ip == 0) {
    printf("FAIL setup: DHCP lease timed out\n");
    s_cpq_ok = false;
    goto done;
  }
  while (mgr->ifp->state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(mgr, 0);
    vTaskDelay(1);
  }

  s_cpq_remote = remote_peer != NULL && remote_peer[0] != '\0';
  s_cpq_done = false;
  s_cpq_ready = false;
  if (xTaskCreate(bsd_udp_cpq_fail_task, "cpq", 1024, NULL,
                  configMAX_PRIORITIES - 1, NULL) != pdPASS) {
    printf("FAIL setup: cannot create CPQ task\n");
    s_cpq_ok = false;
    goto done;
  }
  while (!s_cpq_ready && !s_cpq_done) {
    mg_mgr_poll(mgr, 0);
    vTaskDelay(1);
  }
  if (!s_cpq_ready) {
    printf("FAIL setup: CPQ task did not bind sockets\n");
    s_cpq_ok = false;
    goto done;
  }
  printf("CPQ: sockets ready\n");

  ip = mg_mprintf("%M", mg_print_ip4, &mgr->ifp->ip);
  printf("CPQ: Mongoose IP %s\n", ip);
  argv[0] = (char *) "./bsd_udp_cpq_peer";
  argv[1] = s_cpq_remote ? (char *) "--helper" : ip;
  argv[2] = s_cpq_remote ? (char *) remote_peer : NULL;
  argv[3] = s_cpq_remote ? ip : NULL;
  argv[4] = NULL;
  if (posix_spawn(&pid, argv[0], NULL, NULL, argv, environ) != 0) {
    printf("FAIL setup: cannot start CPQ helper\n");
    s_cpq_ok = false;
    goto done;
  }
  printf("CPQ: %s started\n", s_cpq_remote ? "helper" : "peer");

  while (!s_cpq_done) {
    mg_mgr_poll(mgr, 0);
    vTaskDelay(1);
  }
  while ((r = waitpid(pid, &status, WNOHANG)) == 0) {
    mg_mgr_poll(mgr, 0);
    vTaskDelay(1);
  }
  if (r != pid || !WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    printf("FAIL setup: CPQ peer failed\n");
    s_cpq_ok = false;
  }

done:
  mg_free(ip);
  mg_mgr_free(mgr);
  vTaskEndScheduler();
}

static bool bsd_cpq_test(struct mg_tcpip_driver *driver, void *driver_data,
                         const char *mac, const char *debug_level) {
  struct mg_mgr mgr;
  struct mg_tcpip_if mif;

  mg_log_set(atoi(debug_level == NULL ? "3" : debug_level));
  mg_log_set_fn(cpq_log, NULL);
  mg_mgr_init(&mgr);
  memset(&mif, 0, sizeof(mif));
  mif.driver = driver;
  mif.driver_data = driver_data;
  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mif.mac[0], &mif.mac[1],
         &mif.mac[2], &mif.mac[3], &mif.mac[4], &mif.mac[5]);
  mg_tcpip_init(&mgr, &mif);
  if (mgr.ifp == NULL ||
      xTaskCreate(cpq_task1, "cpqnet", 2048, &mgr,
                  configMAX_PRIORITIES - 2, NULL) != pdPASS)
    return false;
  s_cpq_ok = true;
  vTaskStartScheduler();
  return s_cpq_ok;
}
