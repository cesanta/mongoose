/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

#include "../../mongoose.h"

static const char *s_listening_address = "0.0.0.0:1883";

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev != MG_EV_POLL) printf("USER HANDLER GOT EVENT %d\n", ev);
  /* Do your custom event processing here */
  mg_mqtt_broker(c, ev, ev_data);
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;
  struct mg_mqtt_broker brk;

  mg_mgr_init(&mgr, NULL);

  if ((c = mg_bind(&mgr, s_listening_address, ev_handler)) == NULL) {
    fprintf(stderr, "mg_bind(%s) failed\n", s_listening_address);
    exit(EXIT_FAILURE);
  }
  mg_mqtt_broker_init(&brk, NULL);
  c->priv_2 = &brk;
  mg_set_protocol_mqtt(c);

  printf("MQTT broker started on %s\n", s_listening_address);

  /*
   * TODO: Add a HTTP status page that shows current sessions
   * and subscriptions
   */

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
}
