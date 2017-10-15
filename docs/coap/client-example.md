---
title: CoAP client example
---

To create a CoAP client, follow this pattern:

1. Create an outbound connection by calling `mg_connect`
2. Call `mg_set_protocol_coap` for created connection
3. Create an event handler function that handles the following events:
- `MG_EV_COAP_CON`
- `MG_EV_COAP_NOC`
- `MG_EV_COAP_ACK`
- `MG_EV_COAP_RST`

Here's an example of the simplest CoAP client.
Error checking is omitted for the sake of clarity:

```c
#include "mongoose.h"

static int s_time_to_exit = 0;
static char *s_default_address = "udp://coap.me:5683";

static void coap_handler(struct mg_connection *nc, int ev, void *p) {
  switch (ev) {
    case MG_EV_CONNECT: {
      struct mg_coap_message cm;

      memset(&cm, 0, sizeof(cm));
      cm.msg_id = 1;
      cm.msg_type = MG_COAP_MSG_CON;
      mg_coap_send_message(nc, &cm);
      break;
    }
    case MG_EV_COAP_ACK:
    case MG_EV_COAP_RST: {
      struct mg_coap_message *cm = (struct mg_coap_message *) p;
      printf("ACK/RST for message with msg_id = %d received\n", cm->msg_id);
      s_time_to_exit = 1;
      break;
    }
    case MG_EV_CLOSE: {
      if (s_time_to_exit == 0) {
        printf("Server closed connection\n");
        s_time_to_exit = 1;
      }
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, 0);

  nc = mg_connect(&mgr, s_default_address, coap_handler);

  mg_set_protocol_coap(nc);

  while (!s_time_to_exit) {
    mg_mgr_poll(&mgr, 1000000);
  }

  mg_mgr_free(&mgr);

  return 0;
}
```

See full source code at [CoAP client example](https://github.com/cesanta/mongoose/tree/master/examples/coap_client).
