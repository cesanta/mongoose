#include "mqtt.h"
#include "arch.h"
#include "base64.h"
#include "event.h"
#include "log.h"
#include "private.h"
#include "url.h"
#include "util.h"

#define MQTT_CMD_CONNECT 1
#define MQTT_CMD_CONNACK 2
#define MQTT_CMD_PUBLISH 3
#define MQTT_CMD_PUBACK 4
#define MQTT_CMD_PUBREC 5
#define MQTT_CMD_PUBREL 6
#define MQTT_CMD_PUBCOMP 7
#define MQTT_CMD_SUBSCRIBE 8
#define MQTT_CMD_SUBACK 9
#define MQTT_CMD_UNSUBSCRIBE 10
#define MQTT_CMD_UNSUBACK 11
#define MQTT_CMD_PINGREQ 12
#define MQTT_CMD_PINGRESP 13
#define MQTT_CMD_DISCONNECT 14

#define MQTT_CLEAN_SESSION 0x02
#define MQTT_HAS_WILL 0x04
#define MQTT_WILL_RETAIN 0x20
#define MQTT_HAS_PASSWORD 0x40
#define MQTT_HAS_USER_NAME 0x80
#define MQTT_GET_WILL_QOS(flags) (((flags) &0x18) >> 3)
#define MQTT_SET_WILL_QOS(flags, qos) (flags) = ((flags) & ~0x18) | ((qos) << 3)
#define MQTT_QOS(qos) ((qos) << 1)
#define MQTT_GET_QOS(flags) (((flags) &0x6) >> 1)
#define MQTT_SET_QOS(flags, qos) (flags) = ((flags) & ~0x6) | ((qos) << 1)

enum { MQTT_OK, MQTT_INCOMPLETE, MQTT_MALFORMED };

struct mqtt_message {
  size_t len, topic_offset, topic_len, data_offset, data_len;
  uint16_t id;
  uint8_t cmd, qos, connack_ret_code;
};

static void mqtt_send_header(struct mg_connection *c, uint8_t cmd,
                             uint8_t flags, uint32_t len) {
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

  mqtt_send_header(c, MQTT_CMD_CONNECT, 0, total_len);
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
  LOG(LL_DEBUG, ("%p [%.*s] -> [%.*s]", c->fd, (int) topic->len,
                 (char *) topic->ptr, (int) data->len, (char *) data->ptr));
  if (MQTT_GET_QOS(flags) > 0) total_len += 2;
  mqtt_send_header(c, MQTT_CMD_PUBLISH, flags, total_len);
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
  mqtt_send_header(c, MQTT_CMD_SUBSCRIBE, (uint8_t) MQTT_QOS(qos), total_len);
  if (++s_id == 0) ++s_id;
  mg_send_u16(c, mg_htons(s_id));
  mg_send_u16(c, mg_htons((uint16_t) topic->len));
  mg_send(c, topic->ptr, topic->len);
  mg_send(c, &qos, sizeof(qos));
}

static int parse(const uint8_t *in, size_t inlen, struct mqtt_message *m) {
  uint8_t lc = 0, *p, *end;
  uint32_t len = 0, len_len = 0;

  if (inlen < 2) return MQTT_INCOMPLETE;
  m->cmd = in[0] >> 4;
  m->qos = (in[0] >> 1) & 3;

  len = len_len = 0;
  p = (uint8_t *) in + 1;
  while ((size_t)(p - in) < inlen) {
    lc = *((uint8_t *) p++);
    len += (lc & 0x7f) << 7 * len_len;
    len_len++;
    if (!(lc & 0x80)) break;
    if (len_len > 4) return MQTT_MALFORMED;
  }

  end = p + len;
  if (lc & 0x80 || end > in + inlen) return MQTT_INCOMPLETE;
  m->len = (int) (end - in);

  switch (m->cmd) {
    case MQTT_CMD_CONNACK:
      if (end - p < 2) return MQTT_MALFORMED;
      m->connack_ret_code = p[1];
      break;
    case MQTT_CMD_PUBACK:
    case MQTT_CMD_PUBREC:
    case MQTT_CMD_PUBREL:
    case MQTT_CMD_PUBCOMP:
    case MQTT_CMD_SUBACK:
      if (p + 2 > end) return MQTT_MALFORMED;
      m->id = (p[0] << 8) | p[1];
      break;
    case MQTT_CMD_PUBLISH: {
      if (p + 2 > end) return MQTT_MALFORMED;
      m->topic_len = (p[0] << 8) | p[1];
      m->topic_offset = p + 2 - in;
      p += 2 + m->topic_len;
      if (p > end) return MQTT_MALFORMED;
      if (m->qos > 0) {
        if (p + 2 > end) return MQTT_MALFORMED;
        m->id = (p[0] << 8) | p[1];
        p += 2;
      }
      if (p > end) return MQTT_MALFORMED;
      m->data_offset = p - in;
      m->data_len = end - p;
    }
    default:
      break;
  }
  return MQTT_OK;
}

int mg_mqtt_parse(const unsigned char *s, int n, struct mg_mqtt_message *mm) {
  struct mqtt_message m;
  int rc = parse(s, n, &m);
  if (rc == MQTT_OK) {
    mm->topic.ptr = (char *) s + m.topic_offset;
    mm->topic.len = m.topic_len;
    mm->data.ptr = (char *) s + m.data_offset;
    mm->data.len = m.data_len;
  }
  return rc;
}

static void mqtt_cb(struct mg_connection *c, int ev, void *ev_data,
                    void *fn_data) {
  if (ev == MG_EV_READ) {
    struct mqtt_message m;
    memset(&m, 0, sizeof(m));
    for (;;) {
      int rc = parse(c->recv.buf, c->recv.len, &m);
      if (rc == MQTT_MALFORMED) {
        LOG(LL_ERROR, ("%p MQTT malformed message", c->fd));
        c->is_closing = 1;
        break;
      } else if (rc == MQTT_OK) {
        LOG(LL_VERBOSE_DEBUG,
            ("%p MQTT CMD %d len %d [%.*s]", c->fd, m.cmd, (int) m.len,
             (int) m.data_len, (char *) c->recv.buf + m.data_offset));
        switch (m.cmd) {
          case MQTT_CMD_CONNACK:
            mg_call(c, MG_EV_MQTT_OPEN, &m.connack_ret_code);
            if (m.connack_ret_code == 0) {
              LOG(LL_INFO, ("%p Connected", c->fd));
            } else {
              LOG(LL_ERROR,
                  ("%p MQTT auth failed, code %d", c->fd, m.connack_ret_code));
              c->is_closing = 1;
            }
            break;
          case MQTT_CMD_PUBLISH: {
            struct mg_mqtt_message evd = {
                {(char *) c->recv.buf + m.topic_offset, m.topic_len},
                {(char *) c->recv.buf + m.data_offset, m.data_len}};
            LOG(LL_DEBUG, ("%p [%.*s] -> [%.*s]", c->fd, (int) evd.topic.len,
                           evd.topic.ptr, (int) evd.data.len, evd.data.ptr));
            mg_call(c, MG_EV_MQTT_MSG, &evd);
            break;
          }
        }
        mg_iobuf_delete(&c->recv, m.len);
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
