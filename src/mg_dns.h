/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === DNS API reference
 */

#ifndef CS_MONGOOSE_SRC_DNS_H_
#define CS_MONGOOSE_SRC_DNS_H_

#include "mg_net.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MG_DNS_A_RECORD 0x01     /* Lookup IP address */
#define MG_DNS_CNAME_RECORD 0x05 /* Lookup CNAME */
#define MG_DNS_PTR_RECORD 0x0c   /* Lookup PTR */
#define MG_DNS_TXT_RECORD 0x10   /* Lookup TXT */
#define MG_DNS_AAAA_RECORD 0x1c  /* Lookup IPv6 address */
#define MG_DNS_SRV_RECORD 0x21   /* Lookup SRV */
#define MG_DNS_MX_RECORD 0x0f    /* Lookup mail server for domain */
#define MG_DNS_ANY_RECORD 0xff
#define MG_DNS_NSEC_RECORD 0x2f

#define MG_MAX_DNS_QUESTIONS 32
#define MG_MAX_DNS_ANSWERS 32

#define MG_DNS_MESSAGE 100 /* High-level DNS message event */

enum mg_dns_resource_record_kind {
  MG_DNS_INVALID_RECORD = 0,
  MG_DNS_QUESTION,
  MG_DNS_ANSWER
};

/* DNS resource record. */
struct mg_dns_resource_record {
  struct mg_str name; /* buffer with compressed name */
  int rtype;
  int rclass;
  int ttl;
  enum mg_dns_resource_record_kind kind;
  struct mg_str rdata; /* protocol data (can be a compressed name) */
};

/* DNS message (request and response). */
struct mg_dns_message {
  struct mg_str pkt; /* packet body */
  uint16_t flags;
  uint16_t transaction_id;
  int num_questions;
  int num_answers;
  struct mg_dns_resource_record questions[MG_MAX_DNS_QUESTIONS];
  struct mg_dns_resource_record answers[MG_MAX_DNS_ANSWERS];
};

struct mg_dns_resource_record *mg_dns_next_record(
    struct mg_dns_message *msg, int query, struct mg_dns_resource_record *prev);

/*
 * Parses the record data from a DNS resource record.
 *
 *  - A:     struct in_addr *ina
 *  - AAAA:  struct in6_addr *ina
 *  - CNAME: char buffer
 *
 * Returns -1 on error.
 *
 * TODO(mkm): MX
 */
int mg_dns_parse_record_data(struct mg_dns_message *msg,
                             struct mg_dns_resource_record *rr, void *data,
                             size_t data_len);

/*
 * Sends a DNS query to the remote end.
 */
void mg_send_dns_query(struct mg_connection *nc, const char *name,
                       int query_type);

/*
 * Inserts a DNS header to an IO buffer.
 *
 * Returns the number of bytes inserted.
 */
int mg_dns_insert_header(struct mbuf *io, size_t pos,
                         struct mg_dns_message *msg);

/*
 * Appends already encoded questions from an existing message.
 *
 * This is useful when generating a DNS reply message which includes
 * all question records.
 *
 * Returns the number of appended bytes.
 */
int mg_dns_copy_questions(struct mbuf *io, struct mg_dns_message *msg);

/*
 * Encodes and appends a DNS resource record to an IO buffer.
 *
 * The record metadata is taken from the `rr` parameter, while the name and data
 * are taken from the parameters, encoded in the appropriate format depending on
 * record type and stored in the IO buffer. The encoded values might contain
 * offsets within the IO buffer. It's thus important that the IO buffer doesn't
 * get trimmed while a sequence of records are encoded while preparing a DNS
 * reply.
 *
 * This function doesn't update the `name` and `rdata` pointers in the `rr`
 * struct because they might be invalidated as soon as the IO buffer grows
 * again.
 *
 * Returns the number of bytes appended or -1 in case of error.
 */
int mg_dns_encode_record(struct mbuf *io, struct mg_dns_resource_record *rr,
                         const char *name, size_t nlen, const void *rdata,
                         size_t rlen);

/*
 * Encodes a DNS name.
 */
int mg_dns_encode_name(struct mbuf *io, const char *name, size_t len);

/* Low-level: parses a DNS response. */
int mg_parse_dns(const char *buf, int len, struct mg_dns_message *msg);

/*
 * Uncompresses a DNS compressed name.
 *
 * The containing DNS message is required because of the compressed encoding
 * and reference suffixes present elsewhere in the packet.
 *
 * If the name is less than `dst_len` characters long, the remainder
 * of `dst` is terminated with `\0` characters. Otherwise, `dst` is not
 * terminated.
 *
 * If `dst_len` is 0 `dst` can be NULL.
 * Returns the uncompressed name length.
 */
size_t mg_dns_uncompress_name(struct mg_dns_message *msg, struct mg_str *name,
                              char *dst, int dst_len);

/*
 * Attaches a built-in DNS event handler to the given listening connection.
 *
 * The DNS event handler parses the incoming UDP packets, treating them as DNS
 * requests. If an incoming packet gets successfully parsed by the DNS event
 * handler, a user event handler will receive an `MG_DNS_REQUEST` event, with
 * `ev_data` pointing to the parsed `struct mg_dns_message`.
 *
 * See
 * [captive_dns_server](https://github.com/cesanta/mongoose/tree/master/examples/captive_dns_server)
 * example on how to handle DNS request and send DNS reply.
 */
void mg_set_protocol_dns(struct mg_connection *nc);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_DNS_H_ */
