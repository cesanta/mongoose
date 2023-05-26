// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "net.h"

// Zephyr: Define a semaphore and network management callback to be able to wait
// until our IP address is ready. The main function will start and block on this
// semaphore until this event handler releases it when the network is ready
K_SEM_DEFINE(run, 0, 1);

static void zeh(struct net_mgmt_event_callback *cb, uint32_t mgmt_event,
                struct net_if *iface) {
  if (mgmt_event == NET_EVENT_L4_CONNECTED) k_sem_give(&run);
}

int main(int argc, char *argv[]) {
  // Zephyr: Register the network management callback and block on the semaphore
  struct net_mgmt_event_callback ncb;
  net_mgmt_init_event_callback(&ncb, zeh, NET_EVENT_L4_CONNECTED);
  net_mgmt_add_event_callback(&ncb);
  k_sem_take(&run, K_FOREVER);

  struct mg_mgr mgr;

  mg_log_set(MG_LL_INFO);

  mg_mgr_init(&mgr);

  web_init(&mgr);

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
