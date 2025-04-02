// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

int main(void) {
  uint32_t response_ip = inet_addr("192.168.69.11");
  struct mg_mgr mgr;
  static struct mg_connection *c;
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);        // Initialise event manager

  // Desired name must NOT have any dots in it, nor a domain
  c = mg_mdns_listen(&mgr, "Mongoose");  // Start mDNS server
  // if not using our built-in TCP/IP stack, pass the IP address you want to
  // use as a response, this depends on your underlying TCP/IP stack and number
  // of interfaces available
  memcpy(c->data, &response_ip, sizeof(response_ip));

  for (;;) mg_mgr_poll(&mgr, 1000);  // Event loop

  mg_mgr_free(&mgr);
  return 0;
}
