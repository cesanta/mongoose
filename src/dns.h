#pragma once

#include "net.h"
#include "str.h"

// Mongoose sends DNS queries that contain only one question:
// either A (IPv4) or AAAA (IPv6) address lookup.
// Therefore, we expect zero or one answer.
// If `resolved` is true, then `addr` contains resolved IPv4 or IPV6 address.
struct mg_dns_message {
  uint16_t txnid;       // Transaction ID
  bool resolved;        // Resolve successful, addr is set
  struct mg_addr addr;  // Resolved address
  char name[256];       // Host name
};

struct mg_dns_header {
  uint16_t txnid;  // Transaction ID
  uint16_t flags;
  uint16_t num_questions;
  uint16_t num_answers;
  uint16_t num_authority_prs;
  uint16_t num_other_prs;
};

// DNS resource record
struct mg_dns_rr {
  uint16_t nlen;    // Name or pointer length
  uint16_t atype;   // Address type
  uint16_t aclass;  // Address class
  uint16_t alen;    // Address length
};

// DNS-SD response record
struct mg_dnssd_record {
  struct mg_str srvcproto;  // service.proto, service name
  struct mg_str txt;        // TXT record contents
  uint16_t port;            // SRV record port
};

// mDNS request
struct mg_mdns_req {
  struct mg_dns_rr *rr;
  struct mg_dnssd_record *r;
  struct mg_str reqname;   // requested name in RR
  struct mg_str respname;  // actual name in response
  struct mg_addr addr;
  bool is_listing;
  bool is_resp;
  bool is_unicast;
};

void mg_resolve(struct mg_connection *, const char *url);
void mg_resolve_cancel(struct mg_connection *);
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *);
size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *);

struct mg_connection *mg_mdns_listen(struct mg_mgr *mgr, mg_event_handler_t fn,
                                     void *fn_data);
