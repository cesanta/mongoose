// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static char s_name[] = "Mongoose";
static char s_service[] = "_http._tcp";

static void mdns_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_MDNS_RESP) {
    struct mg_mdns_resp *resp = (struct mg_mdns_resp *) ev_data;
    if (resp->rr->atype == MG_DNS_RTYPE_PTR &&
        mg_strcasecmp(mg_str(s_service), resp->sd.srvcproto) == 0) {
      char addr[40];
      MG_INFO(("Got a response"));
      printf("\tSERVICE -> %.*s\n", (int) resp->sd.srvcproto.len,
             resp->sd.srvcproto.buf);
      printf("\tNAME -> %.*s\n", (int) resp->name.len, resp->name.buf);
      if (resp->sd.port != 0)
        printf("\tPORT -> %u\n", resp->sd.port);
      if (resp->addr.addr.ip4 != 0) {
        mg_snprintf(addr, sizeof(addr), "%M", mg_print_ip, &resp->addr);
        printf("\tADDRESS -> %s\n", addr);
      }
      if (resp->sd.txt.buf != NULL)
        printf("\tTXT -> %.*s\n", (int) resp->sd.txt.len, resp->sd.txt.buf);
      if (resp->addr.addr.ip4 != 0 && resp->addr.port != 0) {
        mg_snprintf(addr, sizeof(addr), "http://%M:%u", mg_print_ip, &resp->addr,
                    mg_ntohs(resp->addr.port));
        printf("\tURL -> %s\n", addr);
      } else if (resp->sd.port != 0 && resp->name.buf != NULL) {
        printf("\tURL -> http://%.*s:%u\n", (int) resp->name.len,
               resp->name.buf, resp->sd.port);
      }
      printf("\n");
    }
  }
  (void) c;
}

int main(void) {
  struct mg_mgr mgr;
  char query[256];
  static struct mg_connection *c;
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);        // Initialise event manager

  c = mg_mdns_listen(&mgr, mdns_ev_handler, s_name);  // Start mDNS server
  if (c == NULL) return 1;
  mg_snprintf(query, sizeof(query), "%s.local", s_service);
   // Request service providers
  if (!mg_mdns_query(c, query, MG_DNS_RTYPE_PTR)) return 1;

  for (;;) mg_mgr_poll(&mgr, 1000);  // Event loop

  mg_mgr_free(&mgr);
  return 0;
}
