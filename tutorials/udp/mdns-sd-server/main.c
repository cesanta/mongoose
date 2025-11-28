// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

// A TXT record data containing JSON, if actually needed.
// The whole record must not exceed 256 bytes, so keep this small
char txt[] = "\x15{\"property\": \"value\"}";
// string length followed by string data, data length = 21 + 1 = 22 bytes

// DNS-SD service records
static const struct mg_dnssd_record srvcs[] = {
    {{"_myservice._tcp", 15}, {"", 0}, 9876},
    {{"_myservice._udp", 15}, {txt, 22}, 9876}};

// DNS-SD service records database
struct dnssd_db {
  struct mg_dnssd_record *srvcs;  // service record data
  uint8_t num;                    // number of records in db
};

static const struct dnssd_db db_ = {
    (struct mg_dnssd_record *) srvcs,
    sizeof(srvcs) / sizeof(struct mg_dnssd_record)};

static struct mg_dnssd_record *dnssd_lookup(const struct dnssd_db *db,
                                            struct mg_str *name) {
  unsigned int i;
  for (i = 0; i < db->num; i++) {
    if (mg_strcasecmp(db->srvcs[i].srvcproto, *name) == 0) break;
  }
  if (i == db->num) return NULL;
  return &db->srvcs[i];
}

// we can do more complex stuff, this is a simple implementation
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_mdns_req *req = (struct mg_mdns_req *) ev_data;
  if (ev == MG_EV_MDNS_PTR) {  // do we serve this service ?
    struct mg_dnssd_record *r;
    if (req->is_listing) return;  // TODO(): handle services listing
    r = dnssd_lookup(&db_, &req->reqname);
    if (r == NULL) return;
    req->r = r;
    req->is_resp = true;
  } else if (ev == MG_EV_MDNS_SRV || ev == MG_EV_MDNS_TXT) {
    // Mongoose already checked the host name for us, otherwise we need to do
    // that ourselves first, then check for service name in db
    struct mg_dnssd_record *r = dnssd_lookup(&db_, &req->reqname);
    if (r == NULL) return;
    req->r = r;
    req->is_resp = true;
  } else if (ev == MG_EV_MDNS_A) {  // is this us ?
    // Mongoose already checked the host name for us, otherwise we need to do
    // that ourselves
  }
  (void) c;
}

int main(void) {
  uint32_t response_ip = inet_addr("192.168.69.11");
  struct mg_mgr mgr;
  static struct mg_connection *c;
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);        // Initialise event manager

  // Desired name must NOT have any dots in it, nor a domain
  c = mg_mdns_listen(&mgr, fn, "Mongoose");  // Start mDNS server
  if (c == NULL) return 1;
  // if not using our built-in TCP/IP stack, pass the IP address you want to
  // use as a response, this depends on your underlying TCP/IP stack and number
  // of interfaces available
  memcpy(c->data, &response_ip, sizeof(response_ip));

  for (;;) mg_mgr_poll(&mgr, 1000);  // Event loop

  mg_mgr_free(&mgr);
  return 0;
}
