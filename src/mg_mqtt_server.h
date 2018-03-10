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

/*
 * === MQTT Server API reference
 */

#ifndef CS_MONGOOSE_SRC_MQTT_BROKER_H_
#define CS_MONGOOSE_SRC_MQTT_BROKER_H_

#if MG_ENABLE_MQTT_BROKER

#include "common/queue.h"
#include "mg_mqtt.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MG_MQTT_MAX_SESSION_SUBSCRIPTIONS
#define MG_MQTT_MAX_SESSION_SUBSCRIPTIONS 512
#endif

struct mg_mqtt_broker;

/* MQTT session (Broker side). */
struct mg_mqtt_session {
  struct mg_mqtt_broker *brk;       /* Broker */
  LIST_ENTRY(mg_mqtt_session) link; /* mg_mqtt_broker::sessions linkage */
  struct mg_connection *nc;         /* Connection with the client */
  size_t num_subscriptions;         /* Size of `subscriptions` array */
  void *user_data;                  /* User data */
  struct mg_mqtt_topic_expression *subscriptions;
};

/* MQTT broker. */
struct mg_mqtt_broker {
  LIST_HEAD(_mg_sesshead, mg_mqtt_session) sessions; /* Session list */
  void *user_data;                                   /* User data */
};

/* Initialises a MQTT broker. */
void mg_mqtt_broker_init(struct mg_mqtt_broker *brk, void *user_data);

/*
 * Processes a MQTT broker message.
 *
 * The listening connection expects a pointer to an initialised
 * `mg_mqtt_broker` structure in the `user_data` field.
 *
 * Basic usage:
 *
 * ```c
 * mg_mqtt_broker_init(&brk, NULL);
 *
 * if ((nc = mg_bind(&mgr, address, mg_mqtt_broker)) == NULL) {
 *   // fail;
 * }
 * nc->user_data = &brk;
 * ```
 *
 * New incoming connections will receive a `mg_mqtt_session` structure
 * in the connection `user_data`. The original `user_data` will be stored
 * in the `user_data` field of the session structure. This allows the user
 * handler to store user data before `mg_mqtt_broker` creates the session.
 *
 * Since only the MG_EV_ACCEPT message is processed by the listening socket,
 * for most events the `user_data` will thus point to a `mg_mqtt_session`.
 */
void mg_mqtt_broker(struct mg_connection *brk, int ev, void *data);

/*
 * Iterates over all MQTT session connections. Example:
 *
 * ```c
 * struct mg_mqtt_session *s;
 * for (s = mg_mqtt_next(brk, NULL); s != NULL; s = mg_mqtt_next(brk, s)) {
 *   // Do something
 * }
 * ```
 */
struct mg_mqtt_session *mg_mqtt_next(struct mg_mqtt_broker *brk,
                                     struct mg_mqtt_session *s);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_MQTT_BROKER */
#endif /* CS_MONGOOSE_SRC_MQTT_BROKER_H_ */
