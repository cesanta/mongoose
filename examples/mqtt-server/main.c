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

// Event handler function
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    LOG(LL_DEBUG, ("cmd %d qos %d", mm->cmd, mm->qos));
    switch (mm->cmd) {
      case MQTT_CMD_CONNECT: {
        // Client connects. Return success, do not check user/password
        uint8_t response[] = {0, 0};
        mg_mqtt_send_header(c, MQTT_CMD_CONNACK, 0, sizeof(response));
        mg_send(c, response, sizeof(response));
        break;
      }
      case MQTT_CMD_SUBSCRIBE: {
        // Client subscribes
        int pos = 4;  // Initial topic offset, where ID ends
        uint8_t qos;
        struct mg_str topic;
        while ((pos = mg_mqtt_next_sub(mm, &topic, &qos, pos)) > 0) {
          struct sub *sub = calloc(1, sizeof(*sub));
          sub->c = c;
          sub->topic = mg_strdup(topic);
          sub->qos = qos;
          LIST_ADD_HEAD(struct sub, &s_subs, sub);
          LOG(LL_INFO,
              ("SUB %p [%.*s]", c->fd, (int) sub->topic.len, sub->topic.ptr));
        }
        break;
      }
      case MQTT_CMD_PUBLISH: {
        // Client published message. Push to all subscribed channels
        LOG(LL_INFO, ("PUB %p [%.*s] -> [%.*s]", c->fd, (int) mm->data.len,
                      mm->data.ptr, (int) mm->topic.len, mm->topic.ptr));
        for (struct sub *sub = s_subs; sub != NULL; sub = sub->next) {
          if (mg_strcmp(mm->topic, sub->topic) != 0) continue;
          mg_mqtt_pub(sub->c, &mm->topic, &mm->data);
        }
        break;
      }
    }
  } else if (ev == MG_EV_CLOSE) {
    // Client disconnects. Remove from the subscription list
    for (struct sub *next, *sub = s_subs; sub != NULL; sub = next) {
      next = sub->next;
      if (c != sub->c) continue;
      LOG(LL_INFO,
          ("UNSUB %p [%.*s]", c->fd, (int) sub->topic.len, sub->topic.ptr));
      LIST_DELETE(struct sub, &s_subs, sub);
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;                            // Event manager
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_mqtt_listen(&mgr, s_listen_on, fn, NULL);  // Create MQTT listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  return 0;
}
