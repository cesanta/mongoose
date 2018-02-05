/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === API reference
 */

#ifndef CS_MONGOOSE_SRC_RESOLV_H_
#define CS_MONGOOSE_SRC_RESOLV_H_

#include "mg_dns.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum mg_resolve_err {
  MG_RESOLVE_OK = 0,
  MG_RESOLVE_NO_ANSWERS = 1,
  MG_RESOLVE_EXCEEDED_RETRY_COUNT = 2,
  MG_RESOLVE_TIMEOUT = 3
};

typedef void (*mg_resolve_callback_t)(struct mg_dns_message *dns_message,
                                      void *user_data, enum mg_resolve_err);

/* Options for `mg_resolve_async_opt`. */
struct mg_resolve_async_opts {
  const char *nameserver;
  int max_retries;    /* defaults to 2 if zero */
  int timeout;        /* in seconds; defaults to 5 if zero */
  int accept_literal; /* pseudo-resolve literal ipv4 and ipv6 addrs */
  int only_literal;   /* only resolves literal addrs; sync cb invocation */
  struct mg_connection **dns_conn; /* return DNS connection */
};

/* See `mg_resolve_async_opt()` */
int mg_resolve_async(struct mg_mgr *mgr, const char *name, int query,
                     mg_resolve_callback_t cb, void *data);

/* Set default DNS server */
void mg_set_nameserver(struct mg_mgr *mgr, const char *nameserver);

/*
 * Resolved a DNS name asynchronously.
 *
 * Upon successful resolution, the user callback will be invoked
 * with the full DNS response message and a pointer to the user's
 * context `data`.
 *
 * In case of timeout while performing the resolution the callback
 * will receive a NULL `msg`.
 *
 * The DNS answers can be extracted with `mg_next_record` and
 * `mg_dns_parse_record_data`:
 *
 * [source,c]
 * ----
 * struct in_addr ina;
 * struct mg_dns_resource_record *rr = mg_next_record(msg, MG_DNS_A_RECORD,
 *   NULL);
 * mg_dns_parse_record_data(msg, rr, &ina, sizeof(ina));
 * ----
 */
int mg_resolve_async_opt(struct mg_mgr *mgr, const char *name, int query,
                         mg_resolve_callback_t cb, void *data,
                         struct mg_resolve_async_opts opts);

/*
 * Resolve a name from `/etc/hosts`.
 *
 * Returns 0 on success, -1 on failure.
 */
int mg_resolve_from_hosts_file(const char *host, union socket_address *usa);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_RESOLV_H_ */
