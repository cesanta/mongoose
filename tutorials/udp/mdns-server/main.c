// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#ifdef ADVANCED_EXAMPLE
static void mdns_ev_handler(struct mg_connection *c, int ev, void *ev_data);
#endif

int main(void) {
  struct mg_mgr mgr;
  static struct mg_connection *c;
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);        // Initialise event manager

  // Desired name must NOT have any dots in it, nor a domain
#ifndef ADVANCED_EXAMPLE
  c = mg_mdns_listen(&mgr, NULL, "Mongoose");  // Start mDNS server
#else
  c = mg_mdns_listen(&mgr, mdns_ev_handler, "Mongoose");  // Start mDNS server
#endif
  if (c == NULL) return 1;

  for (;;) mg_mgr_poll(&mgr, 1000);  // Event loop

  mg_mgr_free(&mgr);
  return 0;
}

// When not using our built-in TCP/IP stack, Mongoose responds with the address
// of the interface in which the request came in. If, for some reason, you want
// to use some other address, catch the MG_EV_MDNS_REQ event for MG_DNS_RTYPE_A
// requests, and fill your desired address
#ifdef ADVANCED_EXAMPLE
static void mdns_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_MDNS_REQ) {
    struct mg_mdns_req *req = (struct mg_mdns_req *) ev_data;
    if (req->rr->atype == MG_DNS_RTYPE_A) {
      struct mg_addr myaddr;
      memset(&myaddr, 0, sizeof(myaddr));
      myaddr.addr.ip4 = MG_IPV4(1, 2, 3, 4);
      req->addr = &myaddr;
    }
  }
  (void) c;
}
#endif
