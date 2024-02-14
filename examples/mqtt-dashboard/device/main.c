// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "net.h"
#include "hal.h"

// Handle interrupts, like Ctrl-C
static int s_signo;
static void signal_handler(int signo) {
  s_signo = signo;
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  int i;

  // Parse command-line flags
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-u") == 0 && argv[i + 1] != NULL) {
      g_mqtt_server_url = argv[++i];
    } else if (strcmp(argv[i], "-i") == 0 && argv[i + 1] != NULL) {
      g_device_id = strdup(argv[++i]);
    } else if (strcmp(argv[i], "-t") == 0 && argv[i + 1] != NULL) {
      g_root_topic = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0 && argv[i + 1] != NULL) {
      mg_log_set(atoi(argv[++i]));
    } else {
      MG_ERROR(("Unknown option: %s. Usage:", argv[i]));
      MG_ERROR(
          ("%s [-u mqtt://SERVER:PORT] [-i DEVICE_ID] [-t TOPIC_NAME] [-v "
           "DEBUG_LEVEL]",
           argv[0], argv[i]));
      return 1;
    }
  }

  signal(SIGINT, signal_handler);   // Setup signal handlers - exist event
  signal(SIGTERM, signal_handler);  // manager loop on SIGINT and SIGTERM

  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_mgr_init(&mgr);

  web_init(&mgr);
  while (s_signo == 0) {
    mg_mgr_poll(&mgr, 50);
  }

  web_free();
  mg_mgr_free(&mgr);
  MG_INFO(("Exiting on signal %d", s_signo));

  return 0;
}
