/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_DNS

#include "mg_internal.h"
#include "mg_dns.h"

static int mg_dns_tid = 0xa0;

struct mg_dns_header {
  uint16_t transaction_id;
  uint16_t flags;
  uint16_t num_questions;
  uint16_t num_answers;
  uint16_t num_authority_prs;
  uint16_t num_other_prs;
};

struct mg_dns_resource_record *mg_dns_next_record(
    struct mg_dns_message *msg, int query,
    struct mg_dns_resource_record *prev) {
  struct mg_dns_resource_record *rr;

  for (rr = (prev == NULL ? msg->answers : prev + 1);
       rr - msg->answers < msg->num_answers; rr++) {
    if (rr->rtype == query) {
      return rr;
    }
  }
  return NULL;
}

int mg_dns_parse_record_data(struct mg_dns_message *msg,
                             struct mg_dns_resource_record *rr, void *data,
                             size_t data_len) {
  switch (rr->rtype) {
    case MG_DNS_A_RECORD:
      if (data_len < sizeof(struct in_addr)) {
        return -1;
      }
      if (rr->rdata.p + data_len > msg->pkt.p + msg->pkt.len) {
        return -1;
      }
      memcpy(data, rr->rdata.p, data_len);
      return 0;
#if MG_ENABLE_IPV6
    case MG_DNS_AAAA_RECORD:
      if (data_len < sizeof(struct in6_addr)) {
        return -1; /* LCOV_EXCL_LINE */
      }
      memcpy(data, rr->rdata.p, data_len);
      return 0;
#endif
    case MG_DNS_CNAME_RECORD:
      mg_dns_uncompress_name(msg, &rr->rdata, (char *) data, data_len);
      return 0;
  }

  return -1;
}

int mg_dns_insert_header(struct mbuf *io, size_t pos,
                         struct mg_dns_message *msg) {
  struct mg_dns_header header;

  memset(&header, 0, sizeof(header));
  header.transaction_id = msg->transaction_id;
  header.flags = htons(msg->flags);
  header.num_questions = htons(msg->num_questions);
  header.num_answers = htons(msg->num_answers);

  return mbuf_insert(io, pos, &header, sizeof(header));
}

int mg_dns_copy_questions(struct mbuf *io, struct mg_dns_message *msg) {
  unsigned char *begin, *end;
  struct mg_dns_resource_record *last_q;
  if (msg->num_questions <= 0) return 0;
  begin = (unsigned char *) msg->pkt.p + sizeof(struct mg_dns_header);
  last_q = &msg->questions[msg->num_questions - 1];
  end = (unsigned char *) last_q->name.p + last_q->name.len + 4;
  return mbuf_append(io, begin, end - begin);
}

int mg_dns_encode_name(struct mbuf *io, const char *name, size_t len) {
  const char *s;
  unsigned char n;
  size_t pos = io->len;

  do {
    if ((s = strchr(name, '.')) == NULL) {
      s = name + len;
    }

    if (s - name > 127) {
      return -1; /* TODO(mkm) cover */
    }
    n = s - name;           /* chunk length */
    mbuf_append(io, &n, 1); /* send length */
    mbuf_append(io, name, n);

    if (*s == '.') {
      n++;
    }

    name += n;
    len -= n;
  } while (*s != '\0');
  mbuf_append(io, "\0", 1); /* Mark end of host name */

  return io->len - pos;
}

int mg_dns_encode_record(struct mbuf *io, struct mg_dns_resource_record *rr,
                         const char *name, size_t nlen, const void *rdata,
                         size_t rlen) {
  size_t pos = io->len;
  uint16_t u16;
  uint32_t u32;

  if (rr->kind == MG_DNS_INVALID_RECORD) {
    return -1; /* LCOV_EXCL_LINE */
  }

  if (mg_dns_encode_name(io, name, nlen) == -1) {
    return -1;
  }

  u16 = htons(rr->rtype);
  mbuf_append(io, &u16, 2);
  u16 = htons(rr->rclass);
  mbuf_append(io, &u16, 2);

  if (rr->kind == MG_DNS_ANSWER) {
    u32 = htonl(rr->ttl);
    mbuf_append(io, &u32, 4);

    if (rr->rtype == MG_DNS_CNAME_RECORD) {
      int clen;
      /* fill size after encoding */
      size_t off = io->len;
      mbuf_append(io, &u16, 2);
      if ((clen = mg_dns_encode_name(io, (const char *) rdata, rlen)) == -1) {
        return -1;
      }
      u16 = clen;
      io->buf[off] = u16 >> 8;
      io->buf[off + 1] = u16 & 0xff;
    } else {
      u16 = htons((uint16_t) rlen);
      mbuf_append(io, &u16, 2);
      mbuf_append(io, rdata, rlen);
    }
  }

  return io->len - pos;
}

void mg_send_dns_query(struct mg_connection *nc, const char *name,
                       int query_type) {
  struct mg_dns_message *msg =
      (struct mg_dns_message *) MG_CALLOC(1, sizeof(*msg));
  struct mbuf pkt;
  struct mg_dns_resource_record *rr = &msg->questions[0];

  DBG(("%s %d", name, query_type));

  mbuf_init(&pkt, 64 /* Start small, it'll grow as needed. */);

  msg->transaction_id = ++mg_dns_tid;
  msg->flags = 0x100;
  msg->num_questions = 1;

  mg_dns_insert_header(&pkt, 0, msg);

  rr->rtype = query_type;
  rr->rclass = 1; /* Class: inet */
  rr->kind = MG_DNS_QUESTION;

  if (mg_dns_encode_record(&pkt, rr, name, strlen(name), NULL, 0) == -1) {
    /* TODO(mkm): return an error code */
    goto cleanup; /* LCOV_EXCL_LINE */
  }

  /* TCP DNS requires messages to be prefixed with len */
  if (!(nc->flags & MG_F_UDP)) {
    uint16_t len = htons((uint16_t) pkt.len);
    mbuf_insert(&pkt, 0, &len, 2);
  }

  mg_send(nc, pkt.buf, pkt.len);
  mbuf_free(&pkt);

cleanup:
  MG_FREE(msg);
}

static unsigned char *mg_parse_dns_resource_record(
    unsigned char *data, unsigned char *end, struct mg_dns_resource_record *rr,
    int reply) {
  unsigned char *name = data;
  int chunk_len, data_len;

  while (data < end && (chunk_len = *data)) {
    if (((unsigned char *) data)[0] & 0xc0) {
      data += 1;
      break;
    }
    data += chunk_len + 1;
  }

  if (data > end - 5) {
    return NULL;
  }

  rr->name.p = (char *) name;
  rr->name.len = data - name + 1;
  data++;

  rr->rtype = data[0] << 8 | data[1];
  data += 2;

  rr->rclass = data[0] << 8 | data[1];
  data += 2;

  rr->kind = reply ? MG_DNS_ANSWER : MG_DNS_QUESTION;
  if (reply) {
    if (data >= end - 6) {
      return NULL;
    }

    rr->ttl = (uint32_t) data[0] << 24 | (uint32_t) data[1] << 16 |
              data[2] << 8 | data[3];
    data += 4;

    data_len = *data << 8 | *(data + 1);
    data += 2;

    rr->rdata.p = (char *) data;
    rr->rdata.len = data_len;
    data += data_len;
  }
  return data;
}

int mg_parse_dns(const char *buf, int len, struct mg_dns_message *msg) {
  struct mg_dns_header *header = (struct mg_dns_header *) buf;
  unsigned char *data = (unsigned char *) buf + sizeof(*header);
  unsigned char *end = (unsigned char *) buf + len;
  int i;

  memset(msg, 0, sizeof(*msg));
  msg->pkt.p = buf;
  msg->pkt.len = len;

  if (len < (int) sizeof(*header)) return -1;

  msg->transaction_id = header->transaction_id;
  msg->flags = ntohs(header->flags);
  msg->num_questions = ntohs(header->num_questions);
  if (msg->num_questions > (int) ARRAY_SIZE(msg->questions)) {
    msg->num_questions = (int) ARRAY_SIZE(msg->questions);
  }
  msg->num_answers = ntohs(header->num_answers);
  if (msg->num_answers > (int) ARRAY_SIZE(msg->answers)) {
    msg->num_answers = (int) ARRAY_SIZE(msg->answers);
  }

  for (i = 0; i < msg->num_questions; i++) {
    data = mg_parse_dns_resource_record(data, end, &msg->questions[i], 0);
    if (data == NULL) return -1;
  }

  for (i = 0; i < msg->num_answers; i++) {
    data = mg_parse_dns_resource_record(data, end, &msg->answers[i], 1);
    if (data == NULL) return -1;
  }

  return 0;
}

size_t mg_dns_uncompress_name(struct mg_dns_message *msg, struct mg_str *name,
                              char *dst, int dst_len) {
  int chunk_len, num_ptrs = 0;
  char *old_dst = dst;
  const unsigned char *data = (unsigned char *) name->p;
  const unsigned char *end = (unsigned char *) msg->pkt.p + msg->pkt.len;

  if (data >= end) {
    return 0;
  }

  while ((chunk_len = *data++)) {
    int leeway = dst_len - (dst - old_dst);
    if (data >= end) {
      return 0;
    }

    if ((chunk_len & 0xc0) == 0xc0) {
      uint16_t off = (data[-1] & (~0xc0)) << 8 | data[0];
      if (off >= msg->pkt.len) {
        return 0;
      }
      /* Basic circular loop avoidance: allow up to 16 pointer hops. */
      if (++num_ptrs > 15) {
        return 0;
      }
      data = (unsigned char *) msg->pkt.p + off;
      continue;
    }
    if (chunk_len > 63) {
      return 0;
    }
    if (chunk_len > leeway) {
      chunk_len = leeway;
    }

    if (data + chunk_len >= end) {
      return 0;
    }

    memcpy(dst, data, chunk_len);
    data += chunk_len;
    dst += chunk_len;
    leeway -= chunk_len;
    if (leeway == 0) {
      return dst - old_dst;
    }
    *dst++ = '.';
  }

  if (dst != old_dst) {
    *--dst = 0;
  }
  return dst - old_dst;
}

static void dns_handler(struct mg_connection *nc, int ev,
                        void *ev_data MG_UD_ARG(void *user_data)) {
  struct mbuf *io = &nc->recv_mbuf;
  struct mg_dns_message msg;

  /* Pass low-level events to the user handler */
  nc->handler(nc, ev, ev_data MG_UD_ARG(user_data));

  switch (ev) {
    case MG_EV_RECV:
      if (!(nc->flags & MG_F_UDP)) {
        mbuf_remove(&nc->recv_mbuf, 2);
      }
      if (mg_parse_dns(nc->recv_mbuf.buf, nc->recv_mbuf.len, &msg) == -1) {
        /* reply + recursion allowed + format error */
        memset(&msg, 0, sizeof(msg));
        msg.flags = 0x8081;
        mg_dns_insert_header(io, 0, &msg);
        if (!(nc->flags & MG_F_UDP)) {
          uint16_t len = htons((uint16_t) io->len);
          mbuf_insert(io, 0, &len, 2);
        }
        mg_send(nc, io->buf, io->len);
      } else {
        /* Call user handler with parsed message */
        nc->handler(nc, MG_DNS_MESSAGE, &msg MG_UD_ARG(user_data));
      }
      mbuf_remove(io, io->len);
      break;
  }
}

void mg_set_protocol_dns(struct mg_connection *nc) {
  nc->proto_handler = dns_handler;
}

#endif /* MG_ENABLE_DNS */
