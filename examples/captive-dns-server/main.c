// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example captive DNS server (captive portal).
//    1. Run `make`. This builds and starts a server on port 5533
//    2. Run `dig -t A www.google.com -4 @localhost -p 5533`

#include "mongoose.h"

static const char *s_listen_url = "udp://0.0.0.0:5533";

// DNS answer section. We response with IP 1.2.3.4 - you can change it
//  in the last 4 bytes of this array
uint8_t answer[] = {
    0xc0, 0x0c,          // Point to the name in the DNS question
    0,    1,             // 2 bytes - record type, A
    0,    1,             // 2 bytes - address class, INET
    0,    0,    0, 120,  // 4 bytes - TTL
    0,    4,             // 2 bytes - address length
    1,    2,    3, 4     // 4 bytes - IP address
};

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    c->is_hexdumping = 1;
  } else if (ev == MG_EV_READ) {
    struct mg_dns_rr rr;  // Parse first question, offset 12 is header size
    size_t n = mg_dns_parse_rr(c->recv.buf, c->recv.len, 12, true, &rr);
    MG_INFO(("DNS request parsed, result=%d", (int) n));
    if (n > 0) {
      char buf[512];
      struct mg_dns_header *h = (struct mg_dns_header *) buf;
      memset(buf, 0, sizeof(buf));  // Clear the whole datagram
      h->txnid = ((struct mg_dns_header *) c->recv.buf)->txnid;  // Copy tnxid
      h->num_questions = mg_htons(1);  // We use only the 1st question
      h->num_answers = mg_htons(1);    // And only one answer
      h->flags = mg_htons(0x8400);     // Authoritative response
      memcpy(buf + sizeof(*h), c->recv.buf + sizeof(*h), n);  // Copy question
      memcpy(buf + sizeof(*h) + n, answer, sizeof(answer));   // And answer
      mg_send(c, buf, 12 + n + sizeof(answer));               // And send it!
    }
    mg_iobuf_del(&c->recv, 0, c->recv.len);
  }
  (void) fn_data;
  (void) ev_data;
}

int main(void) {
  struct mg_mgr mgr;

  mg_log_set(MG_LL_DEBUG);                  // Set log level
  mg_mgr_init(&mgr);                        // Initialise event manager
  mg_listen(&mgr, s_listen_url, fn, NULL);  // Start DNS server
  for (;;) mg_mgr_poll(&mgr, 1000);         // Event loop
  mg_mgr_free(&mgr);

  return 0;
}
