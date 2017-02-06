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

int main(void) {
  struct mg_mgr mgr;
  const char *address = "0.0.0.0:1883";
  struct mg_connection *nc;
  struct mg_mqtt_broker brk;

  mg_mgr_init(&mgr, NULL);
  mg_mqtt_broker_init(&brk, NULL);

  if ((nc = mg_bind(&mgr, address, mg_mqtt_broker)) == NULL) {
    fprintf(stderr, "mg_bind(%s) failed\n", address);
    exit(EXIT_FAILURE);
  }
  nc->user_data = &brk;

  printf("MQTT broker started on %s\n", address);

  /*
   * TODO: Add a HTTP status page that shows current sessions
   * and subscriptions
   */

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
}
