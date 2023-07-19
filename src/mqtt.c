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

struct mg_mqtt_pmap {
  uint8_t id;
  uint8_t type;
};

static const struct mg_mqtt_pmap s_prop_map[] = {
    {MQTT_PROP_PAYLOAD_FORMAT_INDICATOR, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_MESSAGE_EXPIRY_INTERVAL, MQTT_PROP_TYPE_INT},
    {MQTT_PROP_CONTENT_TYPE, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_RESPONSE_TOPIC, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_CORRELATION_DATA, MQTT_PROP_TYPE_BINARY_DATA},
    {MQTT_PROP_SUBSCRIPTION_IDENTIFIER, MQTT_PROP_TYPE_VARIABLE_INT},
    {MQTT_PROP_SESSION_EXPIRY_INTERVAL, MQTT_PROP_TYPE_INT},
    {MQTT_PROP_ASSIGNED_CLIENT_IDENTIFIER, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_SERVER_KEEP_ALIVE, MQTT_PROP_TYPE_SHORT},
    {MQTT_PROP_AUTHENTICATION_METHOD, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_AUTHENTICATION_DATA, MQTT_PROP_TYPE_BINARY_DATA},
    {MQTT_PROP_REQUEST_PROBLEM_INFORMATION, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_WILL_DELAY_INTERVAL, MQTT_PROP_TYPE_INT},
    {MQTT_PROP_REQUEST_RESPONSE_INFORMATION, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_RESPONSE_INFORMATION, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_SERVER_REFERENCE, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_REASON_STRING, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_RECEIVE_MAXIMUM, MQTT_PROP_TYPE_SHORT},
    {MQTT_PROP_TOPIC_ALIAS_MAXIMUM, MQTT_PROP_TYPE_SHORT},
    {MQTT_PROP_TOPIC_ALIAS, MQTT_PROP_TYPE_SHORT},
    {MQTT_PROP_MAXIMUM_QOS, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_RETAIN_AVAILABLE, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_USER_PROPERTY, MQTT_PROP_TYPE_STRING_PAIR},
    {MQTT_PROP_MAXIMUM_PACKET_SIZE, MQTT_PROP_TYPE_INT},
    {MQTT_PROP_WILDCARD_SUBSCRIPTION_AVAILABLE, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_SUBSCRIPTION_IDENTIFIER_AVAILABLE, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_SHARED_SUBSCRIPTION_AVAILABLE, MQTT_PROP_TYPE_BYTE}};

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

static void mg_send_u32(struct mg_connection *c, uint32_t value) {
  mg_send(c, &value, sizeof(value));
}

static uint8_t varint_size(size_t length) {
  uint8_t bytes_needed = 0;
  do {
    bytes_needed++;
    length /= 0x80;
  } while (length > 0);
  return bytes_needed;
}

static size_t encode_varint(uint8_t *buf, size_t value) {
  size_t len = 0;

  do {
    uint8_t byte = (uint8_t) (value % 128);
    value /= 128;
    if (value > 0) byte |= 0x80;
    buf[len++] = byte;
  } while (value > 0);

  return len;
}

static size_t decode_varint(const uint8_t *buf, size_t len, size_t *value) {
  size_t multiplier = 1, offset;
  *value = 0;

  for (offset = 0; offset < 4 && offset < len; offset++) {
    uint8_t encoded_byte = buf[offset];
    *value += (encoded_byte & 0x7f) * multiplier;
    multiplier *= 128;

    if ((encoded_byte & 0x80) == 0) return offset + 1;
  }

  return 0;
}

static int mqtt_prop_type_by_id(uint8_t prop_id) {
  size_t i, num_properties = sizeof(s_prop_map) / sizeof(s_prop_map[0]);
  for (i = 0; i < num_properties; ++i) {
    if (s_prop_map[i].id == prop_id) return s_prop_map[i].type;
  }
  return -1;  // Property ID not found
}

// Returns the size of the properties section, without the
// size of the content's length
static size_t get_properties_length(struct mg_mqtt_prop *props, size_t count) {
  size_t i, size = 0;
  for (i = 0; i < count; i++) {
    size++;  // identifier
    switch (mqtt_prop_type_by_id(props[i].id)) {
      case MQTT_PROP_TYPE_STRING_PAIR:
        size += (uint32_t) (props[i].val.len + props[i].key.len +
                            2 * sizeof(uint16_t));
        break;
      case MQTT_PROP_TYPE_STRING:
        size += (uint32_t) (props[i].val.len + sizeof(uint16_t));
        break;
      case MQTT_PROP_TYPE_BINARY_DATA:
        size += (uint32_t) (props[i].val.len + sizeof(uint16_t));
        break;
      case MQTT_PROP_TYPE_VARIABLE_INT:
        size += varint_size((uint32_t) props[i].iv);
        break;
      case MQTT_PROP_TYPE_INT:
        size += (uint32_t) sizeof(uint32_t);
        break;
      case MQTT_PROP_TYPE_SHORT:
        size += (uint32_t) sizeof(uint16_t);
        break;
      case MQTT_PROP_TYPE_BYTE:
        size += (uint32_t) sizeof(uint8_t);
        break;
      default:
        return size;  // cannot parse further down
    }
  }

  return size;
}

// returns the entire size of the properties section, including the
// size of the variable length of the content
static size_t get_props_size(struct mg_mqtt_prop *props, size_t count) {
  size_t size = get_properties_length(props, count);
  size += varint_size(size);
  return size;
}

static void mg_send_mqtt_properties(struct mg_connection *c,
                                    struct mg_mqtt_prop *props, size_t nprops) {
  size_t total_size = get_properties_length(props, nprops);
  uint8_t buf_v[4] = {0, 0, 0, 0};
  uint8_t buf[4] = {0, 0, 0, 0};
  size_t i, len = encode_varint(buf, total_size);

  mg_send(c, buf, (size_t) len);
  for (i = 0; i < nprops; i++) {
    mg_send(c, &props[i].id, sizeof(props[i].id));
    switch (mqtt_prop_type_by_id(props[i].id)) {
      case MQTT_PROP_TYPE_STRING_PAIR:
        mg_send_u16(c, mg_htons((uint16_t) props[i].key.len));
        mg_send(c, props[i].key.ptr, props[i].key.len);
        mg_send_u16(c, mg_htons((uint16_t) props[i].val.len));
        mg_send(c, props[i].val.ptr, props[i].val.len);
        break;
      case MQTT_PROP_TYPE_BYTE:
        mg_send(c, &props[i].iv, sizeof(uint8_t));
        break;
      case MQTT_PROP_TYPE_SHORT:
        mg_send_u16(c, mg_htons((uint16_t) props[i].iv));
        break;
      case MQTT_PROP_TYPE_INT:
        mg_send_u32(c, mg_htonl((uint32_t) props[i].iv));
        break;
      case MQTT_PROP_TYPE_STRING:
        mg_send_u16(c, mg_htons((uint16_t) props[i].val.len));
        mg_send(c, props[i].val.ptr, props[i].val.len);
        break;
      case MQTT_PROP_TYPE_BINARY_DATA:
        mg_send_u16(c, mg_htons((uint16_t) props[i].val.len));
        mg_send(c, props[i].val.ptr, props[i].val.len);
        break;
      case MQTT_PROP_TYPE_VARIABLE_INT:
        len = encode_varint(buf_v, props[i].iv);
        mg_send(c, buf_v, (size_t) len);
        break;
    }
  }
}

size_t mg_mqtt_next_prop(struct mg_mqtt_message *msg, struct mg_mqtt_prop *prop,
                         size_t ofs) {
  uint8_t *i = (uint8_t *) msg->dgram.ptr + msg->props_start + ofs;
  uint8_t *end = (uint8_t *) msg->dgram.ptr + msg->dgram.len;
  size_t new_pos = ofs, len;
  prop->id = i[0];

  if (ofs >= msg->dgram.len || ofs >= msg->props_start + msg->props_size)
    return 0;
  i++, new_pos++;

  switch (mqtt_prop_type_by_id(prop->id)) {
    case MQTT_PROP_TYPE_STRING_PAIR:
      prop->key.len = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      prop->key.ptr = (char *) i + 2;
      i += 2 + prop->key.len;
      prop->val.len = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      prop->val.ptr = (char *) i + 2;
      new_pos += 2 * sizeof(uint16_t) + prop->val.len + prop->key.len;
      break;
    case MQTT_PROP_TYPE_BYTE:
      prop->iv = (uint8_t) i[0];
      new_pos++;
      break;
    case MQTT_PROP_TYPE_SHORT:
      prop->iv = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      new_pos += sizeof(uint16_t);
      break;
    case MQTT_PROP_TYPE_INT:
      prop->iv = ((uint32_t) i[0] << 24) | ((uint32_t) i[1] << 16) |
                 ((uint32_t) i[2] << 8) | i[3];
      new_pos += sizeof(uint32_t);
      break;
    case MQTT_PROP_TYPE_STRING:
      prop->val.len = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      prop->val.ptr = (char *) i + 2;
      new_pos += 2 + prop->val.len;
      break;
    case MQTT_PROP_TYPE_BINARY_DATA:
      prop->val.len = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      prop->val.ptr = (char *) i + 2;
      new_pos += 2 + prop->val.len;
      break;
    case MQTT_PROP_TYPE_VARIABLE_INT:
      len = decode_varint(i, (size_t) (end - i), (size_t *) &prop->iv);
      new_pos = (!len) ? 0 : new_pos + len;
      break;
    default:
      new_pos = 0;
  }

  return new_pos;
}

void mg_mqtt_login(struct mg_connection *c, const struct mg_mqtt_opts *opts) {
  char rnd[10], client_id[21];
  struct mg_str cid = opts->client_id;
  size_t total_len = 7 + 1 + 2 + 2;
  uint8_t hdr[8] = {0, 4, 'M', 'Q', 'T', 'T', opts->version, 0};

  if (cid.len == 0) {
    mg_random(rnd, sizeof(rnd));
    mg_hex(rnd, sizeof(rnd), client_id);
    client_id[sizeof(client_id) - 1] = '\0';
    cid = mg_str(client_id);
  }

  if (hdr[6] == 0) hdr[6] = 4;  // If version is not set, use 4 (3.1.1)
  c->is_mqtt5 = hdr[6] == 5;    // Set version 5 flag
  hdr[7] = (uint8_t) ((opts->qos & 3) << 3);  // Connection flags
  if (opts->user.len > 0) {
    total_len += 2 + (uint32_t) opts->user.len;
    hdr[7] |= MQTT_HAS_USER_NAME;
  }
  if (opts->pass.len > 0) {
    total_len += 2 + (uint32_t) opts->pass.len;
    hdr[7] |= MQTT_HAS_PASSWORD;
  }
  if (opts->topic.len > 0 && opts->message.len > 0) {
    total_len += 4 + (uint32_t) opts->topic.len + (uint32_t) opts->message.len;
    hdr[7] |= MQTT_HAS_WILL;
  }
  if (opts->clean || cid.len == 0) hdr[7] |= MQTT_CLEAN_SESSION;
  if (opts->retain) hdr[7] |= MQTT_WILL_RETAIN;
  total_len += (uint32_t) cid.len;
  if (c->is_mqtt5) {
    total_len += get_props_size(opts->props, opts->num_props);
    if (hdr[7] & MQTT_HAS_WILL)
      total_len += get_props_size(opts->will_props, opts->num_will_props);
  }

  mg_mqtt_send_header(c, MQTT_CMD_CONNECT, 0, (uint32_t) total_len);
  mg_send(c, hdr, sizeof(hdr));
  // keepalive == 0 means "do not disconnect us!"
  mg_send_u16(c, mg_htons((uint16_t) opts->keepalive));

  if (c->is_mqtt5) mg_send_mqtt_properties(c, opts->props, opts->num_props);

  mg_send_u16(c, mg_htons((uint16_t) cid.len));
  mg_send(c, cid.ptr, cid.len);

  if (hdr[7] & MQTT_HAS_WILL) {
    if (c->is_mqtt5)
      mg_send_mqtt_properties(c, opts->will_props, opts->num_will_props);

    mg_send_u16(c, mg_htons((uint16_t) opts->topic.len));
    mg_send(c, opts->topic.ptr, opts->topic.len);
    mg_send_u16(c, mg_htons((uint16_t) opts->message.len));
    mg_send(c, opts->message.ptr, opts->message.len);
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

void mg_mqtt_pub(struct mg_connection *c, const struct mg_mqtt_opts *opts) {
  uint8_t flags = (uint8_t) (((opts->qos & 3) << 1) | (opts->retain ? 1 : 0));
  size_t len = 2 + opts->topic.len + opts->message.len;
  MG_DEBUG(("%lu [%.*s] -> [%.*s]", c->id, (int) opts->topic.len,
            (char *) opts->topic.ptr, (int) opts->message.len,
            (char *) opts->message.ptr));
  if (opts->qos > 0) len += 2;
  if (c->is_mqtt5) len += get_props_size(opts->props, opts->num_props);

  mg_mqtt_send_header(c, MQTT_CMD_PUBLISH, flags, (uint32_t) len);
  mg_send_u16(c, mg_htons((uint16_t) opts->topic.len));
  mg_send(c, opts->topic.ptr, opts->topic.len);
  if (opts->qos > 0) {
    if (++c->mgr->mqtt_id == 0) ++c->mgr->mqtt_id;
    mg_send_u16(c, mg_htons(c->mgr->mqtt_id));
  }

  if (c->is_mqtt5) mg_send_mqtt_properties(c, opts->props, opts->num_props);

  mg_send(c, opts->message.ptr, opts->message.len);
}

void mg_mqtt_sub(struct mg_connection *c, const struct mg_mqtt_opts *opts) {
  uint8_t qos_ = opts->qos & 3;
  size_t plen = c->is_mqtt5 ? get_props_size(opts->props, opts->num_props) : 0;
  size_t len = 2 + opts->topic.len + 2 + 1 + plen;

  mg_mqtt_send_header(c, MQTT_CMD_SUBSCRIBE, 2, (uint32_t) len);
  if (++c->mgr->mqtt_id == 0) ++c->mgr->mqtt_id;
  mg_send_u16(c, mg_htons(c->mgr->mqtt_id));
  if (c->is_mqtt5) mg_send_mqtt_properties(c, opts->props, opts->num_props);

  mg_send_u16(c, mg_htons((uint16_t) opts->topic.len));
  mg_send(c, opts->topic.ptr, opts->topic.len);
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
      if (version == 5 && p + 2 < end) {
        len_len = (uint32_t) decode_varint(p, (size_t) (end - p), &m->props_size);
        if (!len_len) return MQTT_MALFORMED;
        m->props_start = (size_t) (p + len_len - buf);
        p += len_len + m->props_size;
      }
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
              uint16_t id = mg_ntohs(mm.id);
              uint32_t remaining_len = sizeof(id);
              if (c->is_mqtt5) remaining_len += 2;  // 3.4.2

              mg_mqtt_send_header(
                  c,
                  (uint8_t) (mm.qos == 2 ? MQTT_CMD_PUBREC : MQTT_CMD_PUBACK),
                  0, remaining_len);
              mg_send(c, &id, sizeof(id));

              if (c->is_mqtt5) {
                uint16_t zero = 0;
                mg_send(c, &zero, sizeof(zero));
              }
            }
            mg_call(c, MG_EV_MQTT_MSG, &mm);  // let the app handle qos stuff
            break;
          }
          case MQTT_CMD_PUBREC: {  // MQTT5: 3.5.2-1 TODO(): variable header rc
            uint16_t id = mg_ntohs(mm.id);
            uint32_t remaining_len = sizeof(id);  // MQTT5 3.6.2-1
            mg_mqtt_send_header(c, MQTT_CMD_PUBREL, 2, remaining_len);
            mg_send(c, &id, sizeof(id));  // MQTT5 3.6.1-1, flags = 2
            break;
          }
          case MQTT_CMD_PUBREL: {  // MQTT5: 3.6.2-1 TODO(): variable header rc
            uint16_t id = mg_ntohs(mm.id);
            uint32_t remaining_len = sizeof(id);  // MQTT5 3.7.2-1
            mg_mqtt_send_header(c, MQTT_CMD_PUBCOMP, 0, remaining_len);
            mg_send(c, &id, sizeof(id));
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

void mg_mqtt_disconnect(struct mg_connection *c,
                        const struct mg_mqtt_opts *opts) {
  size_t len = 0;
  if (c->is_mqtt5) len = 1 + get_props_size(opts->props, opts->num_props);
  mg_mqtt_send_header(c, MQTT_CMD_DISCONNECT, 0, (uint32_t) len);

  if (c->is_mqtt5) {
    uint8_t zero = 0;
    mg_send(c, &zero, sizeof(zero));  // reason code
    mg_send_mqtt_properties(c, opts->props, opts->num_props);
  }
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
