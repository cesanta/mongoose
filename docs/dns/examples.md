# Examples


## DNS server example

To create a DNS server, follow this pattern:

1. Create a listening UDP connection by calling `mg_bind()` or `mg_bind_opt()`
2. Call `mg_set_protocol_dns()` for that listening connection.
  That attaches a built-in DNS event handler which parses incoming
  data and triggers DNS-specific events.
3. Create an event handler function.

Here is an example of a simpe DNS server. It is a captive DNS server, meaning
that it replies with the same IP address on all queries, regardless of what
exactly host name is being resolved. Error checking is omitted for
the sake of clarity:

```c
#include "mongoose.h"
#include <stdio.h>

static const char *s_listening_addr = "udp://:5353";

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct mg_dns_message *msg;
  struct mg_dns_resource_record *rr;
  struct mg_dns_reply reply;
  int i;

  switch (ev) {
    case MG_DNS_MESSAGE: {
      struct mbuf reply_buf;
      mbuf_init(&reply_buf, 512);
      msg = (struct mg_dns_message *) ev_data;
      reply = mg_dns_create_reply(&reply_buf, msg);

      for (i = 0; i < msg->num_questions; i++) {
        char rname[256];
        rr = &msg->questions[i];
        mg_dns_uncompress_name(msg, &rr->name, rname, sizeof(rname) - 1);
        fprintf(stdout, "Q type %d name %s\n", rr->rtype, rname);
        if (rr->rtype == MG_DNS_A_RECORD) {
          in_addr_t addr = inet_addr("127.0.0.1");
          mg_dns_reply_record(&reply, rr, NULL, rr->rtype, 10, &addr, 4);
        }
      }

      mg_dns_send_reply(nc, &reply);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      mbuf_free(&reply_buf);
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *c;

  mg_mgr_init(&mgr, NULL);
  c = mg_bind(&mgr, s_listening_addr, ev_handler);
  mg_set_protocol_dns(c);

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
```

See full [Captive DNS server example](https://github.com/cesanta/mongoose/tree/master/examples/captive_dns_server).


## DNS client example

See https://github.com/cesanta/mongoose/blob/master/mongoose.c and search
for the `mg_resolve_async_eh()` function - that is the core of
built-in Mongoose async DNS resolver.
