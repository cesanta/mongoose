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
 * === MQTT API reference
 */

#ifndef CS_MONGOOSE_SRC_MQTT_H_
#define CS_MONGOOSE_SRC_MQTT_H_

#include "mg_net.h"

struct mg_mqtt_message {
  int cmd;
  int qos;
  int len; /* message length in the IO buffer */
  struct mg_str topic;
  struct mg_str payload;

  uint8_t connack_ret_code; /* connack */
  uint16_t message_id;      /* puback */

  /* connect */
  uint8_t protocol_version;
  uint8_t connect_flags;
  uint16_t keep_alive_timer;
  struct mg_str protocol_name;
  struct mg_str client_id;
  struct mg_str will_topic;
  struct mg_str will_message;
  struct mg_str user_name;
  struct mg_str password;
};

struct mg_mqtt_topic_expression {
  const char *topic;
  uint8_t qos;
};

struct mg_send_mqtt_handshake_opts {
  unsigned char flags; /* connection flags */
  uint16_t keep_alive;
  const char *will_topic;
  const char *will_message;
  const char *user_name;
  const char *password;
};

/* mg_mqtt_proto_data should be in header to allow external access to it */
struct mg_mqtt_proto_data {
  uint16_t keep_alive;
  double last_control_time;
};

/* Message types */
#define MG_MQTT_CMD_CONNECT 1
#define MG_MQTT_CMD_CONNACK 2
#define MG_MQTT_CMD_PUBLISH 3
#define MG_MQTT_CMD_PUBACK 4
#define MG_MQTT_CMD_PUBREC 5
#define MG_MQTT_CMD_PUBREL 6
#define MG_MQTT_CMD_PUBCOMP 7
#define MG_MQTT_CMD_SUBSCRIBE 8
#define MG_MQTT_CMD_SUBACK 9
#define MG_MQTT_CMD_UNSUBSCRIBE 10
#define MG_MQTT_CMD_UNSUBACK 11
#define MG_MQTT_CMD_PINGREQ 12
#define MG_MQTT_CMD_PINGRESP 13
#define MG_MQTT_CMD_DISCONNECT 14

/* MQTT event types */
#define MG_MQTT_EVENT_BASE 200
#define MG_EV_MQTT_CONNECT (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_CONNECT)
#define MG_EV_MQTT_CONNACK (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_CONNACK)
#define MG_EV_MQTT_PUBLISH (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBLISH)
#define MG_EV_MQTT_PUBACK (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBACK)
#define MG_EV_MQTT_PUBREC (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBREC)
#define MG_EV_MQTT_PUBREL (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBREL)
#define MG_EV_MQTT_PUBCOMP (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBCOMP)
#define MG_EV_MQTT_SUBSCRIBE (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_SUBSCRIBE)
#define MG_EV_MQTT_SUBACK (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_SUBACK)
#define MG_EV_MQTT_UNSUBSCRIBE (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_UNSUBSCRIBE)
#define MG_EV_MQTT_UNSUBACK (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_UNSUBACK)
#define MG_EV_MQTT_PINGREQ (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PINGREQ)
#define MG_EV_MQTT_PINGRESP (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PINGRESP)
#define MG_EV_MQTT_DISCONNECT (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_DISCONNECT)

/* Message flags */
#define MG_MQTT_RETAIN 0x1
#define MG_MQTT_DUP 0x4
#define MG_MQTT_QOS(qos) ((qos) << 1)
#define MG_MQTT_GET_QOS(flags) (((flags) &0x6) >> 1)
#define MG_MQTT_SET_QOS(flags, qos) (flags) = ((flags) & ~0x6) | ((qos) << 1)

/* Connection flags */
#define MG_MQTT_CLEAN_SESSION 0x02
#define MG_MQTT_HAS_WILL 0x04
#define MG_MQTT_WILL_RETAIN 0x20
#define MG_MQTT_HAS_PASSWORD 0x40
#define MG_MQTT_HAS_USER_NAME 0x80
#define MG_MQTT_GET_WILL_QOS(flags) (((flags) &0x18) >> 3)
#define MG_MQTT_SET_WILL_QOS(flags, qos) \
  (flags) = ((flags) & ~0x18) | ((qos) << 3)

/* CONNACK return codes */
#define MG_EV_MQTT_CONNACK_ACCEPTED 0
#define MG_EV_MQTT_CONNACK_UNACCEPTABLE_VERSION 1
#define MG_EV_MQTT_CONNACK_IDENTIFIER_REJECTED 2
#define MG_EV_MQTT_CONNACK_SERVER_UNAVAILABLE 3
#define MG_EV_MQTT_CONNACK_BAD_AUTH 4
#define MG_EV_MQTT_CONNACK_NOT_AUTHORIZED 5

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Attaches a built-in MQTT event handler to the given connection.
 *
 * The user-defined event handler will receive following extra events:
 *
 * - MG_EV_MQTT_CONNACK
 * - MG_EV_MQTT_PUBLISH
 * - MG_EV_MQTT_PUBACK
 * - MG_EV_MQTT_PUBREC
 * - MG_EV_MQTT_PUBREL
 * - MG_EV_MQTT_PUBCOMP
 * - MG_EV_MQTT_SUBACK
 */
void mg_set_protocol_mqtt(struct mg_connection *nc);

/* Sends an MQTT handshake. */
void mg_send_mqtt_handshake(struct mg_connection *nc, const char *client_id);

/* Sends an MQTT handshake with optional parameters. */
void mg_send_mqtt_handshake_opt(struct mg_connection *nc, const char *client_id,
                                struct mg_send_mqtt_handshake_opts);

/* Publishes a message to a given topic. */
void mg_mqtt_publish(struct mg_connection *nc, const char *topic,
                     uint16_t message_id, int flags, const void *data,
                     size_t len);

/* Subscribes to a bunch of topics. */
void mg_mqtt_subscribe(struct mg_connection *nc,
                       const struct mg_mqtt_topic_expression *topics,
                       size_t topics_len, uint16_t message_id);

/* Unsubscribes from a bunch of topics. */
void mg_mqtt_unsubscribe(struct mg_connection *nc, char **topics,
                         size_t topics_len, uint16_t message_id);

/* Sends a DISCONNECT command. */
void mg_mqtt_disconnect(struct mg_connection *nc);

/* Sends a CONNACK command with a given `return_code`. */
void mg_mqtt_connack(struct mg_connection *nc, uint8_t return_code);

/* Sends a PUBACK command with a given `message_id`. */
void mg_mqtt_puback(struct mg_connection *nc, uint16_t message_id);

/* Sends a PUBREC command with a given `message_id`. */
void mg_mqtt_pubrec(struct mg_connection *nc, uint16_t message_id);

/* Sends a PUBREL command with a given `message_id`. */
void mg_mqtt_pubrel(struct mg_connection *nc, uint16_t message_id);

/* Sends a PUBCOMP command with a given `message_id`. */
void mg_mqtt_pubcomp(struct mg_connection *nc, uint16_t message_id);

/*
 * Sends a SUBACK command with a given `message_id`
 * and a sequence of granted QoSs.
 */
void mg_mqtt_suback(struct mg_connection *nc, uint8_t *qoss, size_t qoss_len,
                    uint16_t message_id);

/* Sends a UNSUBACK command with a given `message_id`. */
void mg_mqtt_unsuback(struct mg_connection *nc, uint16_t message_id);

/* Sends a PINGREQ command. */
void mg_mqtt_ping(struct mg_connection *nc);

/* Sends a PINGRESP command. */
void mg_mqtt_pong(struct mg_connection *nc);

/*
 * Extracts the next topic expression from a SUBSCRIBE command payload.
 *
 * The topic expression name will point to a string in the payload buffer.
 * Returns the pos of the next topic expression or -1 when the list
 * of topics is exhausted.
 */
int mg_mqtt_next_subscribe_topic(struct mg_mqtt_message *msg,
                                 struct mg_str *topic, uint8_t *qos, int pos);

/*
 * Matches a topic against a topic expression
 *
 * Returns 1 if it matches; 0 otherwise.
 */
int mg_mqtt_match_topic_expression(struct mg_str exp, struct mg_str topic);

/*
 * Same as `mg_mqtt_match_topic_expression()`, but takes `exp` as a
 * NULL-terminated string.
 */
int mg_mqtt_vmatch_topic_expression(const char *exp, struct mg_str topic);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MONGOOSE_SRC_MQTT_H_ */
