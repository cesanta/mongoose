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

// DNS-SD service record
struct mg_dnssd_record {
  char *srvcproto;  // service.proto, service name
  char *txt;        // TXT record contents
  uint16_t port;    // SRV record port
};

// DNS-SD service database
struct mg_dnssd_db {
  struct mg_dnssd_record *srvcs;  // service record data
  uint8_t num;                    // number of records in db
};

void mg_resolve(struct mg_connection *, const char *url);
void mg_resolve_cancel(struct mg_connection *);
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *);
size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *);
                       
struct mg_connection *mg_mdns_listen(struct mg_mgr *mgr, char *name);
