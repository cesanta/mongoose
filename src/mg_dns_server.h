/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === DNS server API reference
 *
 * Disabled by default; enable with `-DMG_ENABLE_DNS_SERVER`.
 */

#ifndef CS_MONGOOSE_SRC_DNS_SERVER_H_
#define CS_MONGOOSE_SRC_DNS_SERVER_H_

#if MG_ENABLE_DNS_SERVER

#include "mg_dns.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MG_DNS_SERVER_DEFAULT_TTL 3600

struct mg_dns_reply {
  struct mg_dns_message *msg;
  struct mbuf *io;
  size_t start;
};

/*
 * Creates a DNS reply.
 *
 * The reply will be based on an existing query message `msg`.
 * The query body will be appended to the output buffer.
 * "reply + recursion allowed" will be added to the message flags and the
 * message's num_answers will be set to 0.
 *
 * Answer records can be appended with `mg_dns_send_reply` or by lower
 * level function defined in the DNS API.
 *
 * In order to send a reply use `mg_dns_send_reply`.
 * It's possible to use a connection's send buffer as reply buffer,
 * and it will work for both UDP and TCP connections.
 *
 * Example:
 *
 * ```c
 * reply = mg_dns_create_reply(&nc->send_mbuf, msg);
 * for (i = 0; i < msg->num_questions; i++) {
 *   rr = &msg->questions[i];
 *   if (rr->rtype == MG_DNS_A_RECORD) {
 *     mg_dns_reply_record(&reply, rr, 3600, &dummy_ip_addr, 4);
 *   }
 * }
 * mg_dns_send_reply(nc, &reply);
 * ```
 */
struct mg_dns_reply mg_dns_create_reply(struct mbuf *io,
                                        struct mg_dns_message *msg);

/*
 * Appends a DNS reply record to the IO buffer and to the DNS message.
 *
 * The message's num_answers field will be incremented. It's the caller's duty
 * to ensure num_answers is properly initialised.
 *
 * Returns -1 on error.
 */
int mg_dns_reply_record(struct mg_dns_reply *reply,
                        struct mg_dns_resource_record *question,
                        const char *name, int rtype, int ttl, const void *rdata,
                        size_t rdata_len);

/*
 * Sends a DNS reply through a connection.
 *
 * The DNS data is stored in an IO buffer pointed by reply structure in `r`.
 * This function mutates the content of that buffer in order to ensure that
 * the DNS header reflects the size and flags of the message, that might have
 * been updated either with `mg_dns_reply_record` or by direct manipulation of
 * `r->message`.
 *
 * Once sent, the IO buffer will be trimmed unless the reply IO buffer
 * is the connection's send buffer and the connection is not in UDP mode.
 */
void mg_dns_send_reply(struct mg_connection *nc, struct mg_dns_reply *r);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_DNS_SERVER */
#endif /* CS_MONGOOSE_SRC_DNS_SERVER_H_ */
