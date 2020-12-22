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

void mg_resolve(struct mg_connection *, struct mg_str *, int);
void mg_resolve_cancel(struct mg_connection *);
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *);
