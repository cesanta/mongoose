// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

char s_theone[] = "Server.local";

static void mdns_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_MDNS_RESP) {  // is this the one ?
    struct mg_mdns_resp *resp = (struct mg_mdns_resp *) ev_data;
    if (mg_strcasecmp(mg_str(s_theone), resp->name) == 0)
      MG_INFO(("%.*s is: %M", resp->name.len, resp->name.buf, mg_print_ip, &resp->addr));
  }
  (void) c;
}

int main(void) {
  struct mg_mgr mgr;
  static struct mg_connection *c;
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);        // Initialise event manager

  // Desired name must NOT have any dots in it, nor a domain
  c = mg_mdns_listen(&mgr, mdns_ev_handler, "Mongoose");  // Start mDNS server
  if (c == NULL) return 1;
  if (!mg_mdns_query(c, s_theone, MG_DNS_RTYPE_A)) return 1;

  for (;;) mg_mgr_poll(&mgr, 1000);  // Event loop

  mg_mgr_free(&mgr);
  return 0;
}
