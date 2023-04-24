#include "mqtt.h"
#include "arch.h"
#include "base64.h"
#include "event.h"
#include "log.h"
#include "url.h"
#include "util.h"

#define MQTT_CLEAN_SESSION 0x02
#define MQTT_HAS_WILL 0x04
#define MQTT_WILL_RETAIN 0x20
#define MQTT_HAS_PASSWORD 0x40
#define MQTT_HAS_USER_NAME 0x80

void mg_mqtt_send_header(struct mg_connection *c, uint8_t cmd, uint8_t flags,
                         uint32_t len) {
  uint8_t buf[1 + sizeof(len)], *vlen = &buf[1];
  buf[0] = (uint8_t) ((cmd << 4) | flags);
  do {
    *vlen = len % 0x80;
    len /= 0x80;
    if (len > 0) *vlen |= 0x80;
    vlen++;
  } while (len > 0 && vlen < &buf[sizeof(buf)]);
  mg_send(c, buf, (size_t) (vlen - buf));
}

static void mg_send_u16(struct mg_connection *c, uint16_t value) {
  mg_send(c, &value, sizeof(value));
}

void mg_mqtt_login(struct mg_connection *c, const struct mg_mqtt_opts *opts) {
  char rnd[10], client_id[21], zero = 0;
  struct mg_str cid = opts->client_id;
  uint32_t total_len = 7 + 1 + 2 + 2;
  uint8_t hdr[8] = {0, 4, 'M', 'Q', 'T', 'T', opts->version, 0};

  if (cid.len == 0) {
    mg_random(rnd, sizeof(rnd));
    mg_hex(rnd, sizeof(rnd), client_id);
    client_id[sizeof(client_id) - 1] = '\0';
    cid = mg_str(client_id);
  }

  if (hdr[6] == 0) hdr[6] = 4;  // If version is not set, use 4 (3.1.1)
  c->is_mqtt5 = hdr[6] == 5;    // Set version 5 flag
  hdr[7] = (uint8_t) ((opts->will_qos & 3) << 3);  // Connection flags
  if (opts->user.len > 0) {
    total_len += 2 + (uint32_t) opts->user.len;
    hdr[7] |= MQTT_HAS_USER_NAME;
  }
  if (opts->pass.len > 0) {
    total_len += 2 + (uint32_t) opts->pass.len;
    hdr[7] |= MQTT_HAS_PASSWORD;
  }
  if (opts->will_topic.len > 0 && opts->will_message.len > 0) {
    total_len +=
        4 + (uint32_t) opts->will_topic.len + (uint32_t) opts->will_message.len;
    hdr[7] |= MQTT_HAS_WILL;
  }
  if (opts->clean || cid.len == 0) hdr[7] |= MQTT_CLEAN_SESSION;
  if (opts->will_retain) hdr[7] |= MQTT_WILL_RETAIN;
  total_len += (uint32_t) cid.len;
  if (c->is_mqtt5) total_len += 1U + (hdr[7] & MQTT_HAS_WILL ? 1U : 0);

  mg_mqtt_send_header(c, MQTT_CMD_CONNECT, 0, total_len);
  mg_send(c, hdr, sizeof(hdr));
  // keepalive == 0 means "do not disconnect us!"
  mg_send_u16(c, mg_htons((uint16_t) opts->keepalive));

  if (c->is_mqtt5) mg_send(c, &zero, sizeof(zero));  // V5 properties
  mg_send_u16(c, mg_htons((uint16_t) cid.len));
  mg_send(c, cid.ptr, cid.len);

  if (hdr[7] & MQTT_HAS_WILL) {
    if (c->is_mqtt5) mg_send(c, &zero, sizeof(zero));  // will props
    mg_send_u16(c, mg_htons((uint16_t) opts->will_topic.len));
    mg_send(c, opts->will_topic.ptr, opts->will_topic.len);
    mg_send_u16(c, mg_htons((uint16_t) opts->will_message.len));
    mg_send(c, opts->will_message.ptr, opts->will_message.len);
  }
  if (opts->user.len > 0) {
    mg_send_u16(c, mg_htons((uint16_t) opts->user.len));
    mg_send(c, opts->user.ptr, opts->user.len);
  }
  if (opts->pass.len > 0) {
    mg_send_u16(c, mg_htons((uint16_t) opts->pass.len));
    mg_send(c, opts->pass.ptr, opts->pass.len);
  }
}

void mg_mqtt_pub(struct mg_connection *c, struct mg_str topic,
                 struct mg_str data, int qos, bool retain) {
  uint8_t flags = (uint8_t) (((qos & 3) << 1) | (retain ? 1 : 0)), zero = 0;
  uint32_t len = 2 + (uint32_t) topic.len + (uint32_t) data.len;
  MG_DEBUG(("%lu [%.*s] -> [%.*s]", c->id, (int) topic.len, (char *) topic.ptr,
            (int) data.len, (char *) data.ptr));
  if (qos > 0) len += 2;
  if (c->is_mqtt5) len++;
  mg_mqtt_send_header(c, MQTT_CMD_PUBLISH, flags, len);
  mg_send_u16(c, mg_htons((uint16_t) topic.len));
  mg_send(c, topic.ptr, topic.len);
  if (qos > 0) {
    if (++c->mgr->mqtt_id == 0) ++c->mgr->mqtt_id;
    mg_send_u16(c, mg_htons(c->mgr->mqtt_id));
  }
  if (c->is_mqtt5) mg_send(c, &zero, sizeof(zero));
  mg_send(c, data.ptr, data.len);
}

void mg_mqtt_sub(struct mg_connection *c, struct mg_str topic, int qos) {
  uint8_t qos_ = qos & 3, zero = 0;
  uint32_t len = 2 + (uint32_t) topic.len + 2 + 1 + (c->is_mqtt5 ? 1 : 0);
  mg_mqtt_send_header(c, MQTT_CMD_SUBSCRIBE, 2, len);
  if (++c->mgr->mqtt_id == 0) ++c->mgr->mqtt_id;
  mg_send_u16(c, mg_htons(c->mgr->mqtt_id));
  if (c->is_mqtt5) mg_send(c, &zero, sizeof(zero));
  mg_send_u16(c, mg_htons((uint16_t) topic.len));
  mg_send(c, topic.ptr, topic.len);
  mg_send(c, &qos_, sizeof(qos_));
}

int mg_mqtt_parse(const uint8_t *buf, size_t len, uint8_t version,
                  struct mg_mqtt_message *m) {
  uint8_t lc = 0, *p, *end;
  uint32_t n = 0, len_len = 0;

  memset(m, 0, sizeof(*m));
  m->dgram.ptr = (char *) buf;
  if (len < 2) return MQTT_INCOMPLETE;
  m->cmd = (uint8_t) (buf[0] >> 4);
  m->qos = (buf[0] >> 1) & 3;

  n = len_len = 0;
  p = (uint8_t *) buf + 1;
  while ((size_t) (p - buf) < len) {
    lc = *((uint8_t *) p++);
    n += (uint32_t) ((lc & 0x7f) << 7 * len_len);
    len_len++;
    if (!(lc & 0x80)) break;
    if (len_len >= 4) return MQTT_MALFORMED;
  }
  end = p + n;
  if ((lc & 0x80) || (end > buf + len)) return MQTT_INCOMPLETE;
  m->dgram.len = (size_t) (end - buf);

  switch (m->cmd) {
    case MQTT_CMD_CONNACK:
      if (end - p < 2) return MQTT_MALFORMED;
      m->ack = p[1];
      break;
    case MQTT_CMD_PUBACK:
    case MQTT_CMD_PUBREC:
    case MQTT_CMD_PUBREL:
    case MQTT_CMD_PUBCOMP:
    case MQTT_CMD_SUBSCRIBE:
    case MQTT_CMD_SUBACK:
    case MQTT_CMD_UNSUBSCRIBE:
    case MQTT_CMD_UNSUBACK:
      if (p + 2 > end) return MQTT_MALFORMED;
      m->id = (uint16_t) ((((uint16_t) p[0]) << 8) | p[1]);
      p += 2;
      break;
    case MQTT_CMD_PUBLISH: {
      if (p + 2 > end) return MQTT_MALFORMED;
      m->topic.len = (uint16_t) ((((uint16_t) p[0]) << 8) | p[1]);
      m->topic.ptr = (char *) p + 2;
      p += 2 + m->topic.len;
      if (p > end) return MQTT_MALFORMED;
      if (m->qos > 0) {
        if (p + 2 > end) return MQTT_MALFORMED;
        m->id = (uint16_t) ((((uint16_t) p[0]) << 8) | p[1]);
        p += 2;
      }
      if (p > end) return MQTT_MALFORMED;
      if (version == 5 && p + 2 < end) p += 1 + p[0];  // Skip options
      if (p > end) return MQTT_MALFORMED;
      m->data.ptr = (char *) p;
      m->data.len = (size_t) (end - p);
      break;
    }
    default:
      break;
  }
  return MQTT_OK;
}

static void mqtt_cb(struct mg_connection *c, int ev, void *ev_data,
                    void *fn_data) {
  if (ev == MG_EV_READ) {
    for (;;) {
      uint8_t version = c->is_mqtt5 ? 5 : 4;
      struct mg_mqtt_message mm;
      int rc = mg_mqtt_parse(c->recv.buf, c->recv.len, version, &mm);
      if (rc == MQTT_MALFORMED) {
        MG_ERROR(("%lu MQTT malformed message", c->id));
        c->is_closing = 1;
        break;
      } else if (rc == MQTT_OK) {
        MG_VERBOSE(("%lu MQTT CMD %d len %d [%.*s]", c->id, mm.cmd,
                    (int) mm.dgram.len, (int) mm.data.len, mm.data.ptr));
        switch (mm.cmd) {
          case MQTT_CMD_CONNACK:
            mg_call(c, MG_EV_MQTT_OPEN, &mm.ack);
            if (mm.ack == 0) {
              MG_DEBUG(("%lu Connected", c->id));
            } else {
              MG_ERROR(("%lu MQTT auth failed, code %d", c->id, mm.ack));
              c->is_closing = 1;
            }
            break;
          case MQTT_CMD_PUBLISH: {
            MG_DEBUG(("%lu [%.*s] -> [%.*s]", c->id, (int) mm.topic.len,
                      mm.topic.ptr, (int) mm.data.len, mm.data.ptr));
            if (mm.qos > 0) {
              uint16_t id = mg_htons(mm.id);
              mg_mqtt_send_header(c, MQTT_CMD_PUBACK, 0, sizeof(id));
              mg_send(c, &id, sizeof(id));
            }
            mg_call(c, MG_EV_MQTT_MSG, &mm);
            break;
          }
        }
        mg_call(c, MG_EV_MQTT_CMD, &mm);
        mg_iobuf_del(&c->recv, 0, mm.dgram.len);
      } else {
        break;
      }
    }
  }
  (void) ev_data;
  (void) fn_data;
}

void mg_mqtt_ping(struct mg_connection *nc) {
  mg_mqtt_send_header(nc, MQTT_CMD_PINGREQ, 0, 0);
}

void mg_mqtt_pong(struct mg_connection *nc) {
  mg_mqtt_send_header(nc, MQTT_CMD_PINGRESP, 0, 0);
}

void mg_mqtt_disconnect(struct mg_connection *nc) {
  mg_mqtt_send_header(nc, MQTT_CMD_DISCONNECT, 0, 0);
}

struct mg_connection *mg_mqtt_connect(struct mg_mgr *mgr, const char *url,
                                      const struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) {
    struct mg_mqtt_opts empty;
    memset(&empty, 0, sizeof(empty));
    mg_mqtt_login(c, opts == NULL ? &empty : opts);
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
