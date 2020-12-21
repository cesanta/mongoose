#include "arch.h"
#include "base64.h"
#include "event.h"
#include "log.h"
#include "mqtt.h"
#include "private.h"
#include "url.h"
#include "util.h"

#define MQTT_CLEAN_SESSION 0x02
#define MQTT_HAS_WILL 0x04
#define MQTT_WILL_RETAIN 0x20
#define MQTT_HAS_PASSWORD 0x40
#define MQTT_HAS_USER_NAME 0x80
#define MQTT_GET_WILL_QOS(flags) (((flags) &0x18) >> 3)
#define MQTT_SET_WILL_QOS(flags, qos) (flags) = ((flags) & ~0x18) | ((qos) << 3)

enum { MQTT_OK, MQTT_INCOMPLETE, MQTT_MALFORMED };

void mg_mqtt_send_header(struct mg_connection *c, uint8_t cmd, uint8_t flags,
                         uint32_t len) {
  uint8_t buf[1 + sizeof(len)], *vlen = &buf[1];
  buf[0] = (cmd << 4) | flags;
  do {
    *vlen = len % 0x80;
    len /= 0x80;
    if (len > 0) *vlen |= 0x80;
    vlen++;
  } while (len > 0 && vlen < &buf[sizeof(buf)]);
  mg_send(c, buf, vlen - buf);
}

static void mg_send_u16(struct mg_connection *c, uint16_t value) {
  mg_send(c, &value, sizeof(value));
}

static void mqtt_login(struct mg_connection *c, const char *url,
                       struct mg_mqtt_opts *opts) {
  uint32_t total_len = 7 + 1 + 2 + 2;
  uint16_t flags = (opts->qos & 3) << 3;
  struct mg_str user = mg_url_user(url);
  struct mg_str pass = mg_url_pass(url);

  if (user.len > 0) {
    total_len += 2 + (uint32_t) user.len;
    flags |= MQTT_HAS_USER_NAME;
  }
  if (pass.len > 0) {
    total_len += 2 + (uint32_t) pass.len;
    flags |= MQTT_HAS_PASSWORD;
  }
  if (opts->will_topic.len > 0 && opts->will_message.len > 0) {
    total_len +=
        4 + (uint32_t) opts->will_topic.len + (uint32_t) opts->will_message.len;
    flags |= MQTT_HAS_WILL;
  }
  if (opts->clean || opts->client_id.len == 0) flags |= MQTT_CLEAN_SESSION;
  if (opts->will_retain) flags |= MQTT_WILL_RETAIN;
  total_len += (uint32_t) opts->client_id.len;

  mg_mqtt_send_header(c, MQTT_CMD_CONNECT, 0, total_len);
  mg_send(c, "\00\04MQTT\04", 7);
  mg_send(c, &flags, 1);
  // keepalive == 0 means "do not disconnect us!"
  mg_send_u16(c, mg_htons((uint16_t) opts->keepalive));
  mg_send_u16(c, mg_htons((uint16_t) opts->client_id.len));
  mg_send(c, opts->client_id.ptr, opts->client_id.len);
  if (flags & MQTT_HAS_WILL) {
    mg_send_u16(c, mg_htons((uint16_t) opts->will_topic.len));
    mg_send(c, opts->will_topic.ptr, opts->will_topic.len);
    mg_send_u16(c, mg_htons((uint16_t) opts->will_message.len));
    mg_send(c, opts->will_message.ptr, opts->will_message.len);
  }
  if (user.len > 0) {
    mg_send_u16(c, mg_htons((uint16_t) user.len));
    mg_send(c, user.ptr, user.len);
  }
  if (pass.len > 0) {
    mg_send_u16(c, mg_htons((uint16_t) pass.len));
    mg_send(c, pass.ptr, pass.len);
  }
}

void mg_mqtt_pub(struct mg_connection *c, struct mg_str *topic,
                 struct mg_str *data) {
  uint8_t flags = MQTT_QOS(1);
  uint32_t total_len = 2 + (uint32_t) topic->len + (uint32_t) data->len;
  LOG(LL_DEBUG, ("%lu [%.*s] -> [%.*s]", c->id, (int) topic->len,
                 (char *) topic->ptr, (int) data->len, (char *) data->ptr));
  if (MQTT_GET_QOS(flags) > 0) total_len += 2;
  mg_mqtt_send_header(c, MQTT_CMD_PUBLISH, flags, total_len);
  mg_send_u16(c, mg_htons((uint16_t) topic->len));
  mg_send(c, topic->ptr, topic->len);
  if (MQTT_GET_QOS(flags) > 0) {
    static uint16_t s_id;
    if (++s_id == 0) s_id++;
    mg_send_u16(c, mg_htons(s_id));
  }
  mg_send(c, data->ptr, data->len);
}

void mg_mqtt_sub(struct mg_connection *c, struct mg_str *topic) {
  static uint16_t s_id;
  uint8_t qos = 1;
  uint32_t total_len = 2 + (uint32_t) topic->len + 2 + 1;
  mg_mqtt_send_header(c, MQTT_CMD_SUBSCRIBE, (uint8_t) MQTT_QOS(qos),
                      total_len);
  if (++s_id == 0) ++s_id;
  mg_send_u16(c, mg_htons(s_id));
  mg_send_u16(c, mg_htons((uint16_t) topic->len));
  mg_send(c, topic->ptr, topic->len);
  mg_send(c, &qos, sizeof(qos));
}

int mg_mqtt_parse(const uint8_t *buf, size_t len, struct mg_mqtt_message *m) {
  uint8_t lc = 0, *p, *end;
  uint32_t n = 0, len_len = 0;

  memset(m, 0, sizeof(*m));
  m->dgram.ptr = (char *) buf;
  if (len < 2) return MQTT_INCOMPLETE;
  m->cmd = buf[0] >> 4;
  m->qos = (buf[0] >> 1) & 3;

  n = len_len = 0;
  p = (uint8_t *) buf + 1;
  while ((size_t)(p - buf) < len) {
    lc = *((uint8_t *) p++);
    n += (lc & 0x7f) << 7 * len_len;
    len_len++;
    if (!(lc & 0x80)) break;
    if (len_len >= 4) return MQTT_MALFORMED;
  }
  end = p + n;
  if (lc & 0x80 || end > buf + len) return MQTT_INCOMPLETE;
  m->dgram.len = end - buf;

  switch (m->cmd) {
    case MQTT_CMD_CONNACK:
      if (end - p < 2) return MQTT_MALFORMED;
      m->ack = p[1];
      break;
    case MQTT_CMD_PUBACK:
    case MQTT_CMD_PUBREC:
    case MQTT_CMD_PUBREL:
    case MQTT_CMD_PUBCOMP:
    case MQTT_CMD_SUBACK:
      if (p + 2 > end) return MQTT_MALFORMED;
      m->id = (p[0] << 8) | p[1];
      break;
    case MQTT_CMD_SUBSCRIBE: {
      if (p + 2 > end) return MQTT_MALFORMED;
      m->id = (p[0] << 8) | p[1];
      p += 2;
      break;
    }
    case MQTT_CMD_PUBLISH: {
      if (p + 2 > end) return MQTT_MALFORMED;
      m->topic.len = (p[0] << 8) | p[1];
      m->topic.ptr = (char *) p + 2;
      p += 2 + m->topic.len;
      if (p > end) return MQTT_MALFORMED;
      if (m->qos > 0) {
        if (p + 2 > end) return MQTT_MALFORMED;
        m->id = (p[0] << 8) | p[1];
        p += 2;
      }
      if (p > end) return MQTT_MALFORMED;
      m->data.ptr = (char *) p;
      m->data.len = end - p;
      break;
    }
    default:
      break;
  }
  return MQTT_OK;
}

int mg_mqtt_next_sub(struct mg_mqtt_message *msg, struct mg_str *topic,
                     uint8_t *qos, int pos) {
  unsigned char *buf = (unsigned char *) msg->dgram.ptr + pos;
  int new_pos;
  if ((size_t) pos >= msg->dgram.len) return -1;

  topic->len = buf[0] << 8 | buf[1];
  topic->ptr = (char *) buf + 2;
  new_pos = pos + 2 + topic->len + 1;
  if ((size_t) new_pos > msg->dgram.len) return -1;
  *qos = buf[2 + topic->len];
  return new_pos;
}

static void mqtt_cb(struct mg_connection *c, int ev, void *ev_data,
                    void *fn_data) {
  if (ev == MG_EV_READ) {
    for (;;) {
      struct mg_mqtt_message mm;
      int rc = mg_mqtt_parse(c->recv.buf, c->recv.len, &mm);
      if (rc == MQTT_MALFORMED) {
        LOG(LL_ERROR, ("%lu MQTT malformed message", c->id));
        c->is_closing = 1;
        break;
      } else if (rc == MQTT_OK) {
        LOG(LL_VERBOSE_DEBUG,
            ("%p MQTT CMD %d len %d [%.*s]", c->fd, mm.cmd, (int) mm.dgram.len,
             (int) mm.data.len, mm.data.ptr));
        switch (mm.cmd) {
          case MQTT_CMD_CONNACK:
            mg_call(c, MG_EV_MQTT_OPEN, &mm.ack);
            if (mm.ack == 0) {
              LOG(LL_INFO, ("%lu Connected", c->id));
            } else {
              LOG(LL_ERROR, ("%lu MQTT auth failed, code %d", c->id, mm.ack));
              c->is_closing = 1;
            }
            break;
          case MQTT_CMD_PUBLISH: {
            LOG(LL_DEBUG, ("%lu [%.*s] -> [%.*s]", c->id, (int) mm.topic.len,
                           mm.topic.ptr, (int) mm.data.len, mm.data.ptr));
            mg_call(c, MG_EV_MQTT_MSG, &mm);
            break;
          }
        }
        mg_call(c, MG_EV_MQTT_CMD, &mm);
        mg_iobuf_delete(&c->recv, mm.dgram.len);
      } else {
        break;
      }
    }
  }
  (void) ev_data;
  (void) fn_data;
}

struct mg_connection *mg_mqtt_connect(struct mg_mgr *mgr, const char *url,
                                      struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) {
    struct mg_mqtt_opts empty;
    memset(&empty, 0, sizeof(empty));
    mqtt_login(c, url, opts == NULL ? &empty : opts);
    c->pfn = mqtt_cb;
  }
  return c;
}

struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_listen(mgr, url, fn, fn_data);
  if (c != NULL) c->pfn = mqtt_cb, c->pfn_data = mgr;
  return c;
}
