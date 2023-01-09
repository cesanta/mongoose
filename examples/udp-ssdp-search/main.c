// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const char *s_ssdp_url = "udp://239.255.255.250:1900";

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  MG_DEBUG(("%p got event: %d %p %p", c, ev, ev_data, fn_data));
  if (ev == MG_EV_OPEN) {
    //    c->is_hexdumping = 1;
  } else if (ev == MG_EV_RESOLVE) {
    // c->rem gets populated with multicast address. Store it in c->data
    memcpy(c->data, &c->rem, sizeof(c->rem));
  } else if (ev == MG_EV_READ) {
    MG_INFO(("Got a response"));
    struct mg_http_message hm;
    if (mg_http_parse((const char *) c->recv.buf, c->recv.len, &hm) > 0) {
      size_t i, max = sizeof(hm.headers) / sizeof(hm.headers[0]);
      // Iterate over request headers
      for (i = 0; i < max && hm.headers[i].name.len > 0; i++) {
        struct mg_str *k = &hm.headers[i].name, *v = &hm.headers[i].value;
        if ((mg_vcasecmp(k, "SERVER") == 0) ||
            (mg_vcasecmp(k, "LOCATION") == 0)) {
          printf("\t%.*s -> %.*s\n", (int) k->len, k->ptr, (int) v->len,
                 v->ptr);
        }
      }
      printf("\n");
    }
    // Each response to the SSDP socket will change c->rem.
    // We can now do mg_printf(c, "haha"); to respond back to the remote side.
    // But in our case, we should restore the multicast address in order
    // to have next search to go to the multicast address
    memcpy(&c->rem, c->data, sizeof(c->rem));
    // Discard the content of this response as we expect each SSDP response
    // to generate at most one MG_EV_READ event.
    c->recv.len = 0UL;
  }
}

static void tfn(void *param) {
  struct mg_connection *c = param;
  if (c == NULL) return;
  MG_INFO(("Sending M-SEARCH"));
  mg_printf(c, "%s",
            "M-SEARCH * HTTP/1.1\r\n"
            "HOST: 239.255.255.250:1900\r\n"
            "MAN: \"ssdp:discover\"\r\n"
            "ST: urn:schemas-upnp-org:device:MediaRenderer:1\r\n"
            "MX: 5\r\n"
            "\r\n");
}

int main(void) {
  struct mg_mgr mgr;
  static struct mg_connection *c;
  mg_mgr_init(&mgr);
  c = mg_connect(&mgr, s_ssdp_url, fn, NULL);
  mg_timer_add(&mgr, 2000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, tfn, c);
  while (true) mg_mgr_poll(&mgr, 200);
  mg_mgr_free(&mgr);
  return 0;
}
