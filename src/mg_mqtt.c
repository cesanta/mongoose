/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_MQTT

#include <string.h>

#include "mg_internal.h"
#include "mg_mqtt.h"

static uint16_t getu16(const char *p) {
  const uint8_t *up = (const uint8_t *) p;
  return (up[0] << 8) + up[1];
}

static const char *scanto(const char *p, struct mg_str *s) {
  s->len = getu16(p);
  s->p = p + 2;
  return s->p + s->len;
}

MG_INTERNAL int parse_mqtt(struct mbuf *io, struct mg_mqtt_message *mm) {
  uint8_t header;
  size_t len = 0, len_len = 0;
  const char *p, *end, *eop = &io->buf[io->len];
  unsigned char lc = 0;
  int cmd;

  if (io->len < 2) return MG_MQTT_ERROR_INCOMPLETE_MSG;
  header = io->buf[0];
  cmd = header >> 4;

  /* decode mqtt variable length */
  len = len_len = 0;
  p = io->buf + 1;
  while (p < eop) {
    lc = *((const unsigned char *) p++);
    len += (lc & 0x7f) << 7 * len_len;
    len_len++;
    if (!(lc & 0x80)) break;
    if (len_len > 4) return MG_MQTT_ERROR_MALFORMED_MSG;
  }

  end = p + len;
  if (lc & 0x80 || end > eop) return MG_MQTT_ERROR_INCOMPLETE_MSG;

  mm->cmd = cmd;
  mm->qos = MG_MQTT_GET_QOS(header);

  switch (cmd) {
    case MG_MQTT_CMD_CONNECT: {
      p = scanto(p, &mm->protocol_name);
      if (p > end - 4) return MG_MQTT_ERROR_MALFORMED_MSG;
      mm->protocol_version = *(uint8_t *) p++;
      mm->connect_flags = *(uint8_t *) p++;
      mm->keep_alive_timer = getu16(p);
      p += 2;
      if (p >= end) return MG_MQTT_ERROR_MALFORMED_MSG;
      p = scanto(p, &mm->client_id);
      if (p > end) return MG_MQTT_ERROR_MALFORMED_MSG;
      if (mm->connect_flags & MG_MQTT_HAS_WILL) {
        if (p >= end) return MG_MQTT_ERROR_MALFORMED_MSG;
        p = scanto(p, &mm->will_topic);
      }
      if (mm->connect_flags & MG_MQTT_HAS_WILL) {
        if (p >= end) return MG_MQTT_ERROR_MALFORMED_MSG;
        p = scanto(p, &mm->will_message);
      }
      if (mm->connect_flags & MG_MQTT_HAS_USER_NAME) {
        if (p >= end) return MG_MQTT_ERROR_MALFORMED_MSG;
        p = scanto(p, &mm->user_name);
      }
      if (mm->connect_flags & MG_MQTT_HAS_PASSWORD) {
        if (p >= end) return MG_MQTT_ERROR_MALFORMED_MSG;
        p = scanto(p, &mm->password);
      }
      if (p != end) return MG_MQTT_ERROR_MALFORMED_MSG;

      LOG(LL_DEBUG,
          ("%d %2x %d proto [%.*s] client_id [%.*s] will_topic [%.*s] "
           "will_msg [%.*s] user_name [%.*s] password [%.*s]",
           (int) len, (int) mm->connect_flags, (int) mm->keep_alive_timer,
           (int) mm->protocol_name.len, mm->protocol_name.p,
           (int) mm->client_id.len, mm->client_id.p, (int) mm->will_topic.len,
           mm->will_topic.p, (int) mm->will_message.len, mm->will_message.p,
           (int) mm->user_name.len, mm->user_name.p, (int) mm->password.len,
           mm->password.p));
      break;
    }
    case MG_MQTT_CMD_CONNACK:
      if (end - p < 2) return MG_MQTT_ERROR_MALFORMED_MSG;
      mm->connack_ret_code = p[1];
      break;
    case MG_MQTT_CMD_PUBACK:
    case MG_MQTT_CMD_PUBREC:
    case MG_MQTT_CMD_PUBREL:
    case MG_MQTT_CMD_PUBCOMP:
    case MG_MQTT_CMD_SUBACK:
      if (end - p < 2) return MG_MQTT_ERROR_MALFORMED_MSG;
      mm->message_id = getu16(p);
      p += 2;
      break;
    case MG_MQTT_CMD_PUBLISH: {
      p = scanto(p, &mm->topic);
      if (p > end) return MG_MQTT_ERROR_MALFORMED_MSG;
      if (mm->qos > 0) {
        if (end - p < 2) return MG_MQTT_ERROR_MALFORMED_MSG;
        mm->message_id = getu16(p);
        p += 2;
      }
      mm->payload.p = p;
      mm->payload.len = end - p;
      break;
    }
    case MG_MQTT_CMD_SUBSCRIBE:
      if (end - p < 2) return MG_MQTT_ERROR_MALFORMED_MSG;
      mm->message_id = getu16(p);
      p += 2;
      /*
       * topic expressions are left in the payload and can be parsed with
       * `mg_mqtt_next_subscribe_topic`
       */
      mm->payload.p = p;
      mm->payload.len = end - p;
      break;
    default:
      /* Unhandled command */
      break;
  }

  mm->len = end - io->buf;
  return mm->len;
}

static void mqtt_handler(struct mg_connection *nc, int ev,
                         void *ev_data MG_UD_ARG(void *user_data)) {
  struct mbuf *io = &nc->recv_mbuf;
  struct mg_mqtt_message mm;
  memset(&mm, 0, sizeof(mm));

  nc->handler(nc, ev, ev_data MG_UD_ARG(user_data));

  switch (ev) {
    case MG_EV_ACCEPT:
      if (nc->proto_data == NULL) mg_set_protocol_mqtt(nc);
      break;
    case MG_EV_RECV: {
      /* There can be multiple messages in the buffer, process them all. */
      while (1) {
        int len = parse_mqtt(io, &mm);
        if (len < 0) {
          if (len == MG_MQTT_ERROR_MALFORMED_MSG) {
            /* Protocol error. */
            nc->flags |= MG_F_CLOSE_IMMEDIATELY;
          } else if (len == MG_MQTT_ERROR_INCOMPLETE_MSG) {
            /* Not fully buffered, let's check if we have a chance to get more
             * data later */
            if (nc->recv_mbuf_limit > 0 &&
                nc->recv_mbuf.len >= nc->recv_mbuf_limit) {
              LOG(LL_ERROR, ("%p recv buffer (%lu bytes) exceeds the limit "
                             "%lu bytes, and not drained, closing",
                             nc, (unsigned long) nc->recv_mbuf.len,
                             (unsigned long) nc->recv_mbuf_limit));
              nc->flags |= MG_F_CLOSE_IMMEDIATELY;
            }
          } else {
            /* Should never be here */
            LOG(LL_ERROR, ("%p invalid len: %d, closing", nc, len));
            nc->flags |= MG_F_CLOSE_IMMEDIATELY;
          }
          break;
        }

        nc->handler(nc, MG_MQTT_EVENT_BASE + mm.cmd, &mm MG_UD_ARG(user_data));
        mbuf_remove(io, len);
      }
      break;
    }
    case MG_EV_POLL: {
      struct mg_mqtt_proto_data *pd =
          (struct mg_mqtt_proto_data *) nc->proto_data;
      double now = mg_time();
      if (pd->keep_alive > 0 && pd->last_control_time > 0 &&
          (now - pd->last_control_time) > pd->keep_alive) {
        LOG(LL_DEBUG, ("Send PINGREQ"));
        mg_mqtt_ping(nc);
      }
      break;
    }
  }
}

static void mg_mqtt_proto_data_destructor(void *proto_data) {
  MG_FREE(proto_data);
}

static struct mg_str mg_mqtt_next_topic_component(struct mg_str *topic) {
  struct mg_str res = *topic;
  const char *c = mg_strchr(*topic, '/');
  if (c != NULL) {
    res.len = (c - topic->p);
    topic->len -= (res.len + 1);
    topic->p += (res.len + 1);
  } else {
    topic->len = 0;
  }
  return res;
}

/* Refernce: https://mosquitto.org/man/mqtt-7.html */
int mg_mqtt_match_topic_expression(struct mg_str exp, struct mg_str topic) {
  struct mg_str ec, tc;
  if (exp.len == 0) return 0;
  while (1) {
    ec = mg_mqtt_next_topic_component(&exp);
    tc = mg_mqtt_next_topic_component(&topic);
    if (ec.len == 0) {
      if (tc.len != 0) return 0;
      if (exp.len == 0) break;
      continue;
    }
    if (mg_vcmp(&ec, "+") == 0) {
      if (tc.len == 0 && topic.len == 0) return 0;
      continue;
    }
    if (mg_vcmp(&ec, "#") == 0) {
      /* Must be the last component in the expression or it's invalid. */
      return (exp.len == 0);
    }
    if (mg_strcmp(ec, tc) != 0) {
      return 0;
    }
  }
  return (tc.len == 0 && topic.len == 0);
}

int mg_mqtt_vmatch_topic_expression(const char *exp, struct mg_str topic) {
  return mg_mqtt_match_topic_expression(mg_mk_str(exp), topic);
}

void mg_set_protocol_mqtt(struct mg_connection *nc) {
  nc->proto_handler = mqtt_handler;
  nc->proto_data = MG_CALLOC(1, sizeof(struct mg_mqtt_proto_data));
  nc->proto_data_destructor = mg_mqtt_proto_data_destructor;
}

static void mg_send_mqtt_header(struct mg_connection *nc, uint8_t cmd,
                                uint8_t flags, size_t len) {
  struct mg_mqtt_proto_data *pd = (struct mg_mqtt_proto_data *) nc->proto_data;
  uint8_t buf[1 + sizeof(size_t)];
  uint8_t *vlen = &buf[1];

  buf[0] = (cmd << 4) | flags;

  /* mqtt variable length encoding */
  do {
    *vlen = len % 0x80;
    len /= 0x80;
    if (len > 0) *vlen |= 0x80;
    vlen++;
  } while (len > 0);

  mg_send(nc, buf, vlen - buf);
  pd->last_control_time = mg_time();
}

void mg_send_mqtt_handshake(struct mg_connection *nc, const char *client_id) {
  static struct mg_send_mqtt_handshake_opts opts;
  mg_send_mqtt_handshake_opt(nc, client_id, opts);
}

void mg_send_mqtt_handshake_opt(struct mg_connection *nc, const char *client_id,
                                struct mg_send_mqtt_handshake_opts opts) {
  struct mg_mqtt_proto_data *pd = (struct mg_mqtt_proto_data *) nc->proto_data;
  uint16_t id_len = 0, wt_len = 0, wm_len = 0, user_len = 0, pw_len = 0;
  uint16_t netbytes;
  size_t total_len;

  if (client_id != NULL) {
    id_len = strlen(client_id);
  }

  total_len = 7 + 1 + 2 + 2 + id_len;

  if (opts.user_name != NULL) {
    opts.flags |= MG_MQTT_HAS_USER_NAME;
  }
  if (opts.password != NULL) {
    opts.flags |= MG_MQTT_HAS_PASSWORD;
  }
  if (opts.will_topic != NULL && opts.will_message != NULL) {
    wt_len = strlen(opts.will_topic);
    wm_len = strlen(opts.will_message);
    opts.flags |= MG_MQTT_HAS_WILL;
  }
  if (opts.keep_alive == 0) {
    opts.keep_alive = 60;
  }

  if (opts.flags & MG_MQTT_HAS_WILL) {
    total_len += 2 + wt_len + 2 + wm_len;
  }
  if (opts.flags & MG_MQTT_HAS_USER_NAME) {
    user_len = strlen(opts.user_name);
    total_len += 2 + user_len;
  }
  if (opts.flags & MG_MQTT_HAS_PASSWORD) {
    pw_len = strlen(opts.password);
    total_len += 2 + pw_len;
  }

  mg_send_mqtt_header(nc, MG_MQTT_CMD_CONNECT, 0, total_len);
  mg_send(nc, "\00\04MQTT\04", 7);
  mg_send(nc, &opts.flags, 1);

  netbytes = htons(opts.keep_alive);
  mg_send(nc, &netbytes, 2);

  netbytes = htons(id_len);
  mg_send(nc, &netbytes, 2);
  mg_send(nc, client_id, id_len);

  if (opts.flags & MG_MQTT_HAS_WILL) {
    netbytes = htons(wt_len);
    mg_send(nc, &netbytes, 2);
    mg_send(nc, opts.will_topic, wt_len);

    netbytes = htons(wm_len);
    mg_send(nc, &netbytes, 2);
    mg_send(nc, opts.will_message, wm_len);
  }

  if (opts.flags & MG_MQTT_HAS_USER_NAME) {
    netbytes = htons(user_len);
    mg_send(nc, &netbytes, 2);
    mg_send(nc, opts.user_name, user_len);
  }
  if (opts.flags & MG_MQTT_HAS_PASSWORD) {
    netbytes = htons(pw_len);
    mg_send(nc, &netbytes, 2);
    mg_send(nc, opts.password, pw_len);
  }

  if (pd != NULL) {
    pd->keep_alive = opts.keep_alive;
  }
}

void mg_mqtt_publish(struct mg_connection *nc, const char *topic,
                     uint16_t message_id, int flags, const void *data,
                     size_t len) {
  uint16_t netbytes;
  uint16_t topic_len = strlen(topic);

  size_t total_len = 2 + topic_len + len;
  if (MG_MQTT_GET_QOS(flags) > 0) {
    total_len += 2;
  }

  mg_send_mqtt_header(nc, MG_MQTT_CMD_PUBLISH, flags, total_len);

  netbytes = htons(topic_len);
  mg_send(nc, &netbytes, 2);
  mg_send(nc, topic, topic_len);

  if (MG_MQTT_GET_QOS(flags) > 0) {
    netbytes = htons(message_id);
    mg_send(nc, &netbytes, 2);
  }

  mg_send(nc, data, len);
}

void mg_mqtt_subscribe(struct mg_connection *nc,
                       const struct mg_mqtt_topic_expression *topics,
                       size_t topics_len, uint16_t message_id) {
  uint16_t netbytes;
  size_t i;
  uint16_t topic_len;
  size_t total_len = 2;

  for (i = 0; i < topics_len; i++) {
    total_len += 2 + strlen(topics[i].topic) + 1;
  }

  mg_send_mqtt_header(nc, MG_MQTT_CMD_SUBSCRIBE, MG_MQTT_QOS(1), total_len);

  netbytes = htons(message_id);
  mg_send(nc, (char *) &netbytes, 2);

  for (i = 0; i < topics_len; i++) {
    topic_len = strlen(topics[i].topic);
    netbytes = htons(topic_len);
    mg_send(nc, &netbytes, 2);
    mg_send(nc, topics[i].topic, topic_len);
    mg_send(nc, &topics[i].qos, 1);
  }
}

int mg_mqtt_next_subscribe_topic(struct mg_mqtt_message *msg,
                                 struct mg_str *topic, uint8_t *qos, int pos) {
  unsigned char *buf = (unsigned char *) msg->payload.p + pos;
  int new_pos;

  if ((size_t) pos >= msg->payload.len) return -1;

  topic->len = buf[0] << 8 | buf[1];
  topic->p = (char *) buf + 2;
  new_pos = pos + 2 + topic->len + 1;
  if ((size_t) new_pos > msg->payload.len) return -1;
  *qos = buf[2 + topic->len];
  return new_pos;
}

void mg_mqtt_unsubscribe(struct mg_connection *nc, char **topics,
                         size_t topics_len, uint16_t message_id) {
  uint16_t netbytes;
  size_t i;
  uint16_t topic_len;
  size_t total_len = 2;

  for (i = 0; i < topics_len; i++) {
    total_len += 2 + strlen(topics[i]);
  }

  mg_send_mqtt_header(nc, MG_MQTT_CMD_UNSUBSCRIBE, MG_MQTT_QOS(1), total_len);

  netbytes = htons(message_id);
  mg_send(nc, (char *) &netbytes, 2);

  for (i = 0; i < topics_len; i++) {
    topic_len = strlen(topics[i]);
    netbytes = htons(topic_len);
    mg_send(nc, &netbytes, 2);
    mg_send(nc, topics[i], topic_len);
  }
}

void mg_mqtt_connack(struct mg_connection *nc, uint8_t return_code) {
  uint8_t unused = 0;
  mg_send_mqtt_header(nc, MG_MQTT_CMD_CONNACK, 0, 2);
  mg_send(nc, &unused, 1);
  mg_send(nc, &return_code, 1);
}

/*
 * Sends a command which contains only a `message_id` and a QoS level of 1.
 *
 * Helper function.
 */
static void mg_send_mqtt_short_command(struct mg_connection *nc, uint8_t cmd,
                                       uint16_t message_id) {
  uint16_t netbytes;
  uint8_t flags = (cmd == MG_MQTT_CMD_PUBREL ? 2 : 0);

  mg_send_mqtt_header(nc, cmd, flags, 2 /* len */);

  netbytes = htons(message_id);
  mg_send(nc, &netbytes, 2);
}

void mg_mqtt_puback(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_PUBACK, message_id);
}

void mg_mqtt_pubrec(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_PUBREC, message_id);
}

void mg_mqtt_pubrel(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_PUBREL, message_id);
}

void mg_mqtt_pubcomp(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_PUBCOMP, message_id);
}

void mg_mqtt_suback(struct mg_connection *nc, uint8_t *qoss, size_t qoss_len,
                    uint16_t message_id) {
  size_t i;
  uint16_t netbytes;

  mg_send_mqtt_header(nc, MG_MQTT_CMD_SUBACK, MG_MQTT_QOS(1), 2 + qoss_len);

  netbytes = htons(message_id);
  mg_send(nc, &netbytes, 2);

  for (i = 0; i < qoss_len; i++) {
    mg_send(nc, &qoss[i], 1);
  }
}

void mg_mqtt_unsuback(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_UNSUBACK, message_id);
}

void mg_mqtt_ping(struct mg_connection *nc) {
  mg_send_mqtt_header(nc, MG_MQTT_CMD_PINGREQ, 0, 0);
}

void mg_mqtt_pong(struct mg_connection *nc) {
  mg_send_mqtt_header(nc, MG_MQTT_CMD_PINGRESP, 0, 0);
}

void mg_mqtt_disconnect(struct mg_connection *nc) {
  mg_send_mqtt_header(nc, MG_MQTT_CMD_DISCONNECT, 0, 0);
}

#endif /* MG_ENABLE_MQTT */
