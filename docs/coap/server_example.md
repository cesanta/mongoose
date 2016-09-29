---
title: CoAP server example
---

To create a CoAP server, follow this pattern:
1. Create a listening connection by calling `mg_bind()` or `mg_bind_opt()`
2. 2. Call `mg_set_protocol_coap()` for that listening connection.
3. Create an event handler function that handles the following events:
- `MG_EV_COAP_CON`
- `MG_EV_COAP_NOC`
- `MG_EV_COAP_ACK`
- `MG_EV_COAP_RST`

Here's an example of the simplest CoAP server. Error checking is omitted for the sake of clarity:

```c
#include "mongoose.h"

static char *s_default_address = "udp://:5683";

static void coap_handler(struct mg_connection *nc, int ev, void *p) {
  switch (ev) {
    case MG_EV_COAP_CON: {
      uint32_t res;
      struct mg_coap_message *cm = (struct mg_coap_message *) p;
      printf("CON with msg_id = %d received\n", cm->msg_id);
      res = mg_coap_send_ack(nc, cm->msg_id);
      if (res == 0) {
        printf("Successfully sent ACK for message with msg_id = %d\n",
               cm->msg_id);
      } else {
        printf("Error: %d\n", res);
      }
      break;
    }
    case MG_EV_COAP_NOC:
    case MG_EV_COAP_ACK:
    case MG_EV_COAP_RST: {
      struct mg_coap_message *cm = (struct mg_coap_message *) p;
      printf("ACK/RST/NOC with msg_id = %d received\n", cm->msg_id);
      break;
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, 0);

  nc = mg_bind(&mgr, s_default_address, coap_handler);
  mg_set_protocol_coap(nc);

  while (1) {
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);
  return 0;
}
```

See full source code at [CoAP server example](https://github.com/cesanta/mongoose/tree/master/examples/coap_server).
