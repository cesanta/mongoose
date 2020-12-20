#pragma once

#include "net.h"
#include "str.h"

struct mg_dns_message {
  uint16_t txnid;   // Transaction ID
  bool resolved;    // Resolve successful, ipaddr is set
  uint32_t ipaddr;  // Resolved IPv4 address
  char name[256];   // Host name
};

void mg_resolve(struct mg_mgr *, struct mg_connection *, struct mg_str *, int);
void mg_resolve_cancel(struct mg_mgr *, struct mg_connection *);
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *);
