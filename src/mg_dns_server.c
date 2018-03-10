/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_DNS_SERVER

#include "mg_internal.h"
#include "dns-server.h"

struct mg_dns_reply mg_dns_create_reply(struct mbuf *io,
                                        struct mg_dns_message *msg) {
  struct mg_dns_reply rep;
  rep.msg = msg;
  rep.io = io;
  rep.start = io->len;

  /* reply + recursion allowed */
  msg->flags |= 0x8080;
  mg_dns_copy_questions(io, msg);

  msg->num_answers = 0;
  return rep;
}

void mg_dns_send_reply(struct mg_connection *nc, struct mg_dns_reply *r) {
  size_t sent = r->io->len - r->start;
  mg_dns_insert_header(r->io, r->start, r->msg);
  if (!(nc->flags & MG_F_UDP)) {
    uint16_t len = htons((uint16_t) sent);
    mbuf_insert(r->io, r->start, &len, 2);
  }

  if (&nc->send_mbuf != r->io) {
    mg_send(nc, r->io->buf + r->start, r->io->len - r->start);
    r->io->len = r->start;
  }
}

int mg_dns_reply_record(struct mg_dns_reply *reply,
                        struct mg_dns_resource_record *question,
                        const char *name, int rtype, int ttl, const void *rdata,
                        size_t rdata_len) {
  struct mg_dns_message *msg = (struct mg_dns_message *) reply->msg;
  char rname[512];
  struct mg_dns_resource_record *ans = &msg->answers[msg->num_answers];
  if (msg->num_answers >= MG_MAX_DNS_ANSWERS) {
    return -1; /* LCOV_EXCL_LINE */
  }

  if (name == NULL) {
    name = rname;
    rname[511] = 0;
    mg_dns_uncompress_name(msg, &question->name, rname, sizeof(rname) - 1);
  }

  *ans = *question;
  ans->kind = MG_DNS_ANSWER;
  ans->rtype = rtype;
  ans->ttl = ttl;

  if (mg_dns_encode_record(reply->io, ans, name, strlen(name), rdata,
                           rdata_len) == -1) {
    return -1; /* LCOV_EXCL_LINE */
  };

  msg->num_answers++;
  return 0;
}

#endif /* MG_ENABLE_DNS_SERVER */
