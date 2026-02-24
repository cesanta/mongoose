// Copyright (c) 2022-2026 Cesanta Software Limited
// All rights reserved
//
// This example runs an MDNS server, auto-discoverable as MDNS_NAME.
// Also, it runs a web server; we can find it by discovering MDNS_NAME.

// Finally, under ADVANCED_EXAMPLE there is an example of how to
// implement a database that can hold those services we serve, and how to
// announce them responding mDNS-SD requests, like this web server
// - make CFLAGS_EXTRA="-DADVANCED_EXAMPLE"
// - avahi -r _http._tcp

#include "mongoose.h"

// Desired name must NOT have any dots in it, nor a domain
#define MDNS_NAME "mongoose-device"
#define WEB_SERVER_ADDR "http://0.0.0.0:8000"  // port is hardcoded below

// When not using our built-in TCP/IP stack, Mongoose responds with the address
// of the interface in which the request came in. If, for some reason, you want
// to use some other address, fill req->addr. See: mdns-client advanced example

#ifdef ADVANCED_EXAMPLE
// A TXT record data containing JSON, if actually needed.
// The whole record must not exceed 256 bytes, so keep this small
char txt[] = "\x15{\"property\": \"value\"}";
// string length followed by string data, data length = 21 + 1 = 22 bytes

// DNS-SD service records
static const struct mg_dnssd_record s_records[] = {
    {{"_http._tcp", 10}, {"", 0}, 8000},  // port is hardcoded
    {{"_myservice._tcp", 15}, {"", 0}, 9876},
    {{"_myservice._udp", 15}, {txt, 22}, 9876}};

#if 0
// DNS-SD service records database
struct dnssd_db {
  struct mg_dnssd_record *srvcs;  // service record data
  uint8_t num;                    // number of records in db
} db_ = {
    (struct mg_dnssd_record *) srvcs,
    sizeof(srvcs) / sizeof(struct mg_dnssd_record),
};
#endif

static const struct mg_dnssd_record *dnssd_lookup(struct mg_str name) {
  size_t i;
  for (i = 0; i < sizeof(s_records) / sizeof(s_records[0]); i++) {
    if (mg_strcasecmp(s_records[i].srvcproto, name) == 0) return &s_records[i];
  }
  return NULL;
}
#endif

// we can do more complex stuff, this is a simple implementation
static void mdns_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
#ifndef ADVANCED_EXAMPLE
  if (ev == MG_EV_MDNS_REQ) {
    struct mg_mdns_req *req = (struct mg_mdns_req *) ev_data;
    if (req->rr->atype == MG_DNS_RTYPE_PTR) {
      // do we serve this service ?
      MG_INFO(("PTR %.*s", req->reqname.len, req->reqname.buf));
    } else if (req->rr->atype == MG_DNS_RTYPE_SRV) {
      MG_INFO(("SRV"));
    } else if (req->rr->atype == MG_DNS_RTYPE_TXT) {
      MG_INFO(("TXT"));
    } else if (req->rr->atype == MG_DNS_RTYPE_A) {
      MG_INFO(("A %.*s", req->reqname.len, req->reqname.buf));
    }
  }

#else
  if (ev == MG_EV_MDNS_REQ) {
    struct mg_mdns_req *req = (struct mg_mdns_req *) ev_data;
    if (req->rr->atype == MG_DNS_RTYPE_PTR) {  // do we serve this service ?
      struct mg_dnssd_record *r;
      if (req->is_listing) return;  // TODO(): handle services listing
      r = (struct mg_dnssd_record *) dnssd_lookup(req->reqname);
      if (r == NULL) return;
      req->r = r;
      req->is_resp = true;
    } else if (req->rr->atype == MG_DNS_RTYPE_SRV ||
               req->rr->atype == MG_DNS_RTYPE_TXT) {
      // Mongoose already checked the host name for us, otherwise we need to do
      // that ourselves first, then check for service name in db
      struct mg_dnssd_record *r =
          (struct mg_dnssd_record *) dnssd_lookup(req->reqname);
      if (r == NULL) return;
      req->r = r;
      req->is_resp = true;
    } else if (req->rr->atype == MG_DNS_RTYPE_A) {  // is this us ?
      // Mongoose already checked the host name for us, otherwise we need to do
      // that ourselves
    }
  }
#endif
  (void) c;
}

// Simple web server, responds with a JSON like this
// {"uri":"/","addr":"192.168.0.2:1234"}
static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_http_reply(c, 200, "", "{%m:%m,%m:%m}\n",  // Print JSON response
                  MG_ESC("uri"), mg_print_esc, hm->uri.len, hm->uri.buf,  // URI
                  MG_ESC("addr"), mg_print_ip, &c->rem);  // And remote addr
  }
}

int main(void) {
  struct mg_mgr mgr;
  static struct mg_connection *c;

  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);        // Initialise event manager

  c = mg_mdns_listen(&mgr, mdns_ev_handler, MDNS_NAME);
  if (c == NULL) return 1;

  mg_http_listen(&mgr, WEB_SERVER_ADDR, http_ev_handler, NULL);

  for (;;) mg_mgr_poll(&mgr, 1000);  // Event loop

  mg_mgr_free(&mgr);
  return 0;
}
