---
title: "mg_dns_create_reply()"
decl_name: "mg_dns_create_reply"
symbol_kind: "func"
signature: |
  struct mg_dns_reply mg_dns_create_reply(struct mbuf *io,
                                          struct mg_dns_message *msg);
---

Creates a DNS reply.

The reply will be based on an existing query message `msg`.
The query body will be appended to the output buffer.
"reply + recursion allowed" will be added to the message flags and the
message's num_answers will be set to 0.

Answer records can be appended with `mg_dns_send_reply` or by lower
level function defined in the DNS API.

In order to send a reply use `mg_dns_send_reply`.
It's possible to use a connection's send buffer as reply buffer,
and it will work for both UDP and TCP connections.

Example:

```c
reply = mg_dns_create_reply(&nc->send_mbuf, msg);
for (i = 0; i < msg->num_questions; i++) {
  rr = &msg->questions[i];
  if (rr->rtype == MG_DNS_A_RECORD) {
    mg_dns_reply_record(&reply, rr, 3600, &dummy_ip_addr, 4);
  }
}
mg_dns_send_reply(nc, &reply);
``` 

