// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example MQTT server. Usage:
//  1. Start this server, type `make`
//  2. Install mosquitto MQTT client
//  3. In one terminal, run:   mosquitto_sub -h localhost -t foo -t bar
//  4. In another, run:        mosquitto_pub -h localhost -t foo -m hi

#include "mongoose.h"

static const char *s_listen_on = "mqtt://0.0.0.0:1883";

// A list of subscription, held in memory
struct sub {
  struct sub *next;
  struct mg_connection *c;
  struct mg_str topic;
  uint8_t qos;
};
static struct sub *s_subs = NULL;

// Handle interrupts, like Ctrl-C
static int s_signo;
static void signal_handler(int signo) {
  s_signo = signo;
}

static size_t mg_mqtt_next_topic(struct mg_mqtt_message *msg,
                                 struct mg_str *topic, uint8_t *qos,
                                 size_t pos) {
  unsigned char *buf = (unsigned char *) msg->dgram.ptr + pos;
  size_t new_pos;
  if (pos >= msg->dgram.len) return 0;

  topic->len = (size_t) (((unsigned) buf[0]) << 8 | buf[1]);
  topic->ptr = (char *) buf + 2;
  new_pos = pos + 2 + topic->len + (qos == NULL ? 0 : 1);
  if ((size_t) new_pos > msg->dgram.len) return 0;
  if (qos != NULL) *qos = buf[2 + topic->len];
  return new_pos;
}

size_t mg_mqtt_next_sub(struct mg_mqtt_message *msg, struct mg_str *topic,
                        uint8_t *qos, size_t pos) {
  uint8_t tmp;
  return mg_mqtt_next_topic(msg, topic, qos == NULL ? &tmp : qos, pos);
}

size_t mg_mqtt_next_unsub(struct mg_mqtt_message *msg, struct mg_str *topic,
                          size_t pos) {
  return mg_mqtt_next_topic(msg, topic, NULL, pos);
}

// Event handler function
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_DEBUG(("cmd %d qos %d", mm->cmd, mm->qos));
    switch (mm->cmd) {
      case MQTT_CMD_CONNECT: {
        // Client connects
        if (mm->dgram.len < 9) {
          mg_error(c, "Malformed MQTT frame");
        } else if (mm->dgram.ptr[8] != 4) {
          mg_error(c, "Unsupported MQTT version %d", mm->dgram.ptr[8]);
        } else {
          uint8_t response[] = {0, 0};
          mg_mqtt_send_header(c, MQTT_CMD_CONNACK, 0, sizeof(response));
          mg_send(c, response, sizeof(response));
        }
        break;
      }
      case MQTT_CMD_SUBSCRIBE: {
        // Client subscribes
        size_t pos = 4;  // Initial topic offset, where ID ends
        uint8_t qos, resp[256];
        struct mg_str topic;
        int num_topics = 0;
        while ((pos = mg_mqtt_next_sub(mm, &topic, &qos, pos)) > 0) {
          struct sub *sub = calloc(1, sizeof(*sub));
          sub->c = c;
          sub->topic = mg_strdup(topic);
          sub->qos = qos;
          LIST_ADD_HEAD(struct sub, &s_subs, sub);
          MG_INFO(
              ("SUB %p [%.*s]", c->fd, (int) sub->topic.len, sub->topic.ptr));
          // Change '+' to '*' for topic matching using mg_match
          for (size_t i = 0; i < sub->topic.len; i++) {
            if (sub->topic.ptr[i] == '+') ((char *) sub->topic.ptr)[i] = '*';
          }
          resp[num_topics++] = qos;
        }
        mg_mqtt_send_header(c, MQTT_CMD_SUBACK, 0, num_topics + 2);
        uint16_t id = mg_htons(mm->id);
        mg_send(c, &id, 2);
        mg_send(c, resp, num_topics);
        break;
      }
      case MQTT_CMD_PUBLISH: {
        // Client published message. Push to all subscribed channels
        MG_INFO(("PUB %p [%.*s] -> [%.*s]", c->fd, (int) mm->data.len,
                 mm->data.ptr, (int) mm->topic.len, mm->topic.ptr));
        for (struct sub *sub = s_subs; sub != NULL; sub = sub->next) {
          if (mg_match(mm->topic, sub->topic, NULL)) {
            struct mg_mqtt_opts pub_opts;
            memset(&pub_opts, 0, sizeof(pub_opts));
            pub_opts.topic = mm->topic;
            pub_opts.message = mm->data;
            pub_opts.qos = 1, pub_opts.retain = false;
            mg_mqtt_pub(sub->c, &pub_opts);
          }
        }
        break;
      }
      case MQTT_CMD_PINGREQ: {
        // The server must send a PINGRESP packet in response to a PINGREQ packet [MQTT-3.12.4-1]
        MG_INFO(("PINGREQ %p -> PINGRESP", c->fd));
        mg_mqtt_send_header(c, MQTT_CMD_PINGRESP, 0, 0);
        break;
      }
    }
  } else if (ev == MG_EV_ACCEPT) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CLOSE) {
    // Client disconnects. Remove from the subscription list
    for (struct sub *next, *sub = s_subs; sub != NULL; sub = next) {
      next = sub->next;
      if (c != sub->c) continue;
      MG_INFO(("UNSUB %p [%.*s]", c->fd, (int) sub->topic.len, sub->topic.ptr));
      LIST_DELETE(struct sub, &s_subs, sub);
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;                // Event manager
  signal(SIGINT, signal_handler);   // Setup signal handlers - exist event
  signal(SIGTERM, signal_handler);  // manager loop on SIGINT and SIGTERM
  mg_mgr_init(&mgr);                // Initialise event manager
  MG_INFO(("Starting on %s", s_listen_on));      // Inform that we're starting
  mg_mqtt_listen(&mgr, s_listen_on, fn, NULL);   // Create MQTT listener
  while (s_signo == 0) mg_mgr_poll(&mgr, 1000);  // Event loop, 1s timeout
  mg_mgr_free(&mgr);                             // Cleanup
  return 0;
}
