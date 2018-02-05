/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mg_internal.h"
#include "mg_mqtt_server.h"

#if MG_ENABLE_MQTT_BROKER

static void mg_mqtt_session_init(struct mg_mqtt_broker *brk,
                                 struct mg_mqtt_session *s,
                                 struct mg_connection *nc) {
  s->brk = brk;
  s->subscriptions = NULL;
  s->num_subscriptions = 0;
  s->nc = nc;
}

static void mg_mqtt_add_session(struct mg_mqtt_session *s) {
  LIST_INSERT_HEAD(&s->brk->sessions, s, link);
}

static void mg_mqtt_remove_session(struct mg_mqtt_session *s) {
  LIST_REMOVE(s, link);
}

static void mg_mqtt_destroy_session(struct mg_mqtt_session *s) {
  size_t i;
  for (i = 0; i < s->num_subscriptions; i++) {
    MG_FREE((void *) s->subscriptions[i].topic);
  }
  MG_FREE(s->subscriptions);
  MG_FREE(s);
}

static void mg_mqtt_close_session(struct mg_mqtt_session *s) {
  mg_mqtt_remove_session(s);
  mg_mqtt_destroy_session(s);
}

void mg_mqtt_broker_init(struct mg_mqtt_broker *brk, void *user_data) {
  LIST_INIT(&brk->sessions);
  brk->user_data = user_data;
}

static void mg_mqtt_broker_handle_connect(struct mg_mqtt_broker *brk,
                                          struct mg_connection *nc) {
  struct mg_mqtt_session *s =
      (struct mg_mqtt_session *) MG_CALLOC(1, sizeof *s);
  if (s == NULL) {
    /* LCOV_EXCL_START */
    mg_mqtt_connack(nc, MG_EV_MQTT_CONNACK_SERVER_UNAVAILABLE);
    return;
    /* LCOV_EXCL_STOP */
  }

  /* TODO(mkm): check header (magic and version) */

  mg_mqtt_session_init(brk, s, nc);
  nc->priv_2 = s;
  mg_mqtt_add_session(s);

  mg_mqtt_connack(nc, MG_EV_MQTT_CONNACK_ACCEPTED);
}

static void mg_mqtt_broker_handle_subscribe(struct mg_connection *nc,
                                            struct mg_mqtt_message *msg) {
  struct mg_mqtt_session *ss = (struct mg_mqtt_session *) nc->priv_2;
  uint8_t qoss[MG_MQTT_MAX_SESSION_SUBSCRIPTIONS];
  size_t num_subs = 0;
  struct mg_str topic;
  uint8_t qos;
  int pos;
  struct mg_mqtt_topic_expression *te;

  for (pos = 0;
       (pos = mg_mqtt_next_subscribe_topic(msg, &topic, &qos, pos)) != -1;) {
    if (num_subs >= sizeof(MG_MQTT_MAX_SESSION_SUBSCRIPTIONS) ||
        (ss->num_subscriptions + num_subs >=
         MG_MQTT_MAX_SESSION_SUBSCRIPTIONS)) {
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      return;
    }
    qoss[num_subs++] = qos;
  }

  if (num_subs > 0) {
    te = (struct mg_mqtt_topic_expression *) MG_REALLOC(
        ss->subscriptions,
        sizeof(*ss->subscriptions) * (ss->num_subscriptions + num_subs));
    if (te == NULL) {
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      return;
    }
    ss->subscriptions = te;
    for (pos = 0;
         pos < (int) msg->payload.len &&
             (pos = mg_mqtt_next_subscribe_topic(msg, &topic, &qos, pos)) != -1;
         ss->num_subscriptions++) {
      te = &ss->subscriptions[ss->num_subscriptions];
      te->topic = (char *) MG_MALLOC(topic.len + 1);
      te->qos = qos;
      memcpy((char *) te->topic, topic.p, topic.len);
      ((char *) te->topic)[topic.len] = '\0';
    }
  }

  if (pos == (int) msg->payload.len) {
    mg_mqtt_suback(nc, qoss, num_subs, msg->message_id);
  } else {
    /* We did not fully parse the payload, something must be wrong. */
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}

static void mg_mqtt_broker_handle_publish(struct mg_mqtt_broker *brk,
                                          struct mg_mqtt_message *msg) {
  struct mg_mqtt_session *s;
  size_t i;

  for (s = mg_mqtt_next(brk, NULL); s != NULL; s = mg_mqtt_next(brk, s)) {
    for (i = 0; i < s->num_subscriptions; i++) {
      if (mg_mqtt_vmatch_topic_expression(s->subscriptions[i].topic,
                                          msg->topic)) {
        char buf[100], *p = buf;
        mg_asprintf(&p, sizeof(buf), "%.*s", (int) msg->topic.len,
                    msg->topic.p);
        if (p == NULL) {
          return;
        }
        mg_mqtt_publish(s->nc, p, 0, 0, msg->payload.p, msg->payload.len);
        if (p != buf) {
          MG_FREE(p);
        }
        break;
      }
    }
  }
}

void mg_mqtt_broker(struct mg_connection *nc, int ev, void *data) {
  struct mg_mqtt_message *msg = (struct mg_mqtt_message *) data;
  struct mg_mqtt_broker *brk;

  if (nc->listener) {
    brk = (struct mg_mqtt_broker *) nc->listener->priv_2;
  } else {
    brk = (struct mg_mqtt_broker *) nc->priv_2;
  }

  switch (ev) {
    case MG_EV_ACCEPT:
      if (nc->proto_data == NULL) mg_set_protocol_mqtt(nc);
      nc->priv_2 = NULL; /* Clear up the inherited pointer to broker */
      break;
    case MG_EV_MQTT_CONNECT:
      if (nc->priv_2 == NULL) {
        mg_mqtt_broker_handle_connect(brk, nc);
      } else {
        /* Repeated CONNECT */
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
      break;
    case MG_EV_MQTT_SUBSCRIBE:
      if (nc->priv_2 != NULL) {
        mg_mqtt_broker_handle_subscribe(nc, msg);
      } else {
        /* Subscribe before CONNECT */
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
      break;
    case MG_EV_MQTT_PUBLISH:
      if (nc->priv_2 != NULL) {
        mg_mqtt_broker_handle_publish(brk, msg);
      } else {
        /* Publish before CONNECT */
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
      break;
    case MG_EV_CLOSE:
      if (nc->listener && nc->priv_2 != NULL) {
        mg_mqtt_close_session((struct mg_mqtt_session *) nc->priv_2);
      }
      break;
  }
}

struct mg_mqtt_session *mg_mqtt_next(struct mg_mqtt_broker *brk,
                                     struct mg_mqtt_session *s) {
  return s == NULL ? LIST_FIRST(&brk->sessions) : LIST_NEXT(s, link);
}

#endif /* MG_ENABLE_MQTT_BROKER */
