#include "ws.h"

#include "base64.h"
#include "fmt.h"
#include "http.h"
#include "log.h"
#include "printf.h"
#include "sha1.h"
#include "url.h"
#include "util.h"

struct ws_msg {
  uint8_t flags;
  size_t header_len;
  size_t data_len;
};

size_t mg_ws_vprintf(struct mg_connection *c, int op, const char *fmt,
                     va_list *ap) {
  size_t len = c->send.len;
  size_t n = mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, ap);
  mg_ws_wrap(c, c->send.len - len, op);
  return n;
}

size_t mg_ws_printf(struct mg_connection *c, int op, const char *fmt, ...) {
  size_t len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_ws_vprintf(c, op, fmt, &ap);
  va_end(ap);
  return len;
}

static void ws_handshake(struct mg_connection *c, const struct mg_str *wskey,
                         const struct mg_str *wsproto, const char *fmt,
                         va_list *ap) {
  const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  unsigned char sha[20], b64_sha[30];

  mg_sha1_ctx sha_ctx;
  mg_sha1_init(&sha_ctx);
  mg_sha1_update(&sha_ctx, (unsigned char *) wskey->ptr, wskey->len);
  mg_sha1_update(&sha_ctx, (unsigned char *) magic, 36);
  mg_sha1_final(sha, &sha_ctx);
  mg_base64_encode(sha, sizeof(sha), (char *) b64_sha, sizeof(b64_sha));
  mg_xprintf(mg_pfn_iobuf, &c->send,
             "HTTP/1.1 101 Switching Protocols\r\n"
             "Upgrade: websocket\r\n"
             "Connection: Upgrade\r\n"
             "Sec-WebSocket-Accept: %s\r\n",
             b64_sha);
  if (fmt != NULL) mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, ap);
  if (wsproto != NULL) {
    mg_printf(c, "Sec-WebSocket-Protocol: %.*s\r\n", (int) wsproto->len,
              wsproto->ptr);
  }
  mg_send(c, "\r\n", 2);
}

static uint32_t be32(const uint8_t *p) {
  return (((uint32_t) p[3]) << 0) | (((uint32_t) p[2]) << 8) |
         (((uint32_t) p[1]) << 16) | (((uint32_t) p[0]) << 24);
}

static size_t ws_process(uint8_t *buf, size_t len, struct ws_msg *msg) {
  size_t i, n = 0, mask_len = 0;
  memset(msg, 0, sizeof(*msg));
  if (len >= 2) {
    n = buf[1] & 0x7f;                // Frame length
    mask_len = buf[1] & 128 ? 4 : 0;  // last bit is a mask bit
    msg->flags = buf[0];
    if (n < 126 && len >= mask_len) {
      msg->data_len = n;
      msg->header_len = 2 + mask_len;
    } else if (n == 126 && len >= 4 + mask_len) {
      msg->header_len = 4 + mask_len;
      msg->data_len = (((size_t) buf[2]) << 8) | buf[3];
    } else if (len >= 10 + mask_len) {
      msg->header_len = 10 + mask_len;
      msg->data_len =
          (size_t) (((uint64_t) be32(buf + 2) << 32) + be32(buf + 6));
    }
  }
  // Sanity check, and integer overflow protection for the boundary check below
  // data_len should not be larger than 1 Gb
  if (msg->data_len > 1024 * 1024 * 1024) return 0;
  if (msg->header_len + msg->data_len > len) return 0;
  if (mask_len > 0) {
    uint8_t *p = buf + msg->header_len, *m = p - mask_len;
    for (i = 0; i < msg->data_len; i++) p[i] ^= m[i & 3];
  }
  return msg->header_len + msg->data_len;
}

static size_t mkhdr(size_t len, int op, bool is_client, uint8_t *buf) {
  size_t n = 0;
  buf[0] = (uint8_t) (op | 128);
  if (len < 126) {
    buf[1] = (unsigned char) len;
    n = 2;
  } else if (len < 65536) {
    uint16_t tmp = mg_htons((uint16_t) len);
    buf[1] = 126;
    memcpy(&buf[2], &tmp, sizeof(tmp));
    n = 4;
  } else {
    uint32_t tmp;
    buf[1] = 127;
    tmp = mg_htonl((uint32_t) (((uint64_t) len) >> 32));
    memcpy(&buf[2], &tmp, sizeof(tmp));
    tmp = mg_htonl((uint32_t) (len & 0xffffffffU));
    memcpy(&buf[6], &tmp, sizeof(tmp));
    n = 10;
  }
  if (is_client) {
    buf[1] |= 1 << 7;  // Set masking flag
    mg_random(&buf[n], 4);
    n += 4;
  }
  return n;
}

static void mg_ws_mask(struct mg_connection *c, size_t len) {
  if (c->is_client && c->send.buf != NULL) {
    size_t i;
    uint8_t *p = c->send.buf + c->send.len - len, *mask = p - 4;
    for (i = 0; i < len; i++) p[i] ^= mask[i & 3];
  }
}

size_t mg_ws_send(struct mg_connection *c, const void *buf, size_t len,
                  int op) {
  uint8_t header[14];
  size_t header_len = mkhdr(len, op, c->is_client, header);
  mg_send(c, header, header_len);
  MG_VERBOSE(("WS out: %d [%.*s]", (int) len, (int) len, buf));
  mg_send(c, buf, len);
  mg_ws_mask(c, len);
  return header_len + len;
}

static bool mg_ws_client_handshake(struct mg_connection *c) {
  int n = mg_http_get_request_len(c->recv.buf, c->recv.len);
  if (n < 0) {
    mg_error(c, "not http");  // Some just, not an HTTP request
  } else if (n > 0) {
    if (n < 15 || memcmp(c->recv.buf + 9, "101", 3) != 0) {
      mg_error(c, "ws handshake error");
    } else {
      struct mg_http_message hm;
      if (mg_http_parse((char *) c->recv.buf, c->recv.len, &hm)) {
        c->is_websocket = 1;
        mg_call(c, MG_EV_WS_OPEN, &hm);
      } else {
        mg_error(c, "ws handshake error");
      }
    }
    mg_iobuf_del(&c->recv, 0, (size_t) n);
  } else {
    return true;  // Request is not yet received, quit event handler
  }
  return false;  // Continue event handler
}

static void mg_ws_cb(struct mg_connection *c, int ev, void *ev_data,
                     void *fn_data) {
  struct ws_msg msg;
  size_t ofs = (size_t) c->pfn_data;

  // assert(ofs < c->recv.len);
  if (ev == MG_EV_READ) {
    if (c->is_client && !c->is_websocket && mg_ws_client_handshake(c)) return;

    while (ws_process(c->recv.buf + ofs, c->recv.len - ofs, &msg) > 0) {
      char *s = (char *) c->recv.buf + ofs + msg.header_len;
      struct mg_ws_message m = {{s, msg.data_len}, msg.flags};
      size_t len = msg.header_len + msg.data_len;
      uint8_t final = msg.flags & 128, op = msg.flags & 15;
      // MG_VERBOSE ("fin %d op %d len %d [%.*s]", final, op,
      //                       (int) m.data.len, (int) m.data.len, m.data.ptr));
      switch (op) {
        case WEBSOCKET_OP_CONTINUE:
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_PING:
          MG_DEBUG(("%s", "WS PONG"));
          mg_ws_send(c, s, msg.data_len, WEBSOCKET_OP_PONG);
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_PONG:
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_TEXT:
        case WEBSOCKET_OP_BINARY:
          if (final) mg_call(c, MG_EV_WS_MSG, &m);
          break;
        case WEBSOCKET_OP_CLOSE:
          MG_DEBUG(("%lu WS CLOSE", c->id));
          mg_call(c, MG_EV_WS_CTL, &m);
          // Echo the payload of the received CLOSE message back to the sender
          mg_ws_send(c, m.data.ptr, m.data.len, WEBSOCKET_OP_CLOSE);
          c->is_draining = 1;
          break;
        default:
          // Per RFC6455, close conn when an unknown op is recvd
          mg_error(c, "unknown WS op %d", op);
          break;
      }

      // Handle fragmented frames: strip header, keep in c->recv
      if (final == 0 || op == 0) {
        if (op) ofs++, len--, msg.header_len--;       // First frame
        mg_iobuf_del(&c->recv, ofs, msg.header_len);  // Strip header
        len -= msg.header_len;
        ofs += len;
        c->pfn_data = (void *) ofs;
        // MG_INFO(("FRAG %d [%.*s]", (int) ofs, (int) ofs, c->recv.buf));
      }
      // Remove non-fragmented frame
      if (final && op) mg_iobuf_del(&c->recv, ofs, len);
      // Last chunk of the fragmented frame
      if (final && !op) {
        m.flags = c->recv.buf[0];
        m.data = mg_str_n((char *) &c->recv.buf[1], (size_t) (ofs - 1));
        mg_call(c, MG_EV_WS_MSG, &m);
        mg_iobuf_del(&c->recv, 0, ofs);
        ofs = 0;
        c->pfn_data = NULL;
      }
    }
  }
  (void) fn_data;
  (void) ev_data;
}

struct mg_connection *mg_ws_connect(struct mg_mgr *mgr, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) {
    char nonce[16], key[30];
    struct mg_str host = mg_url_host(url);
    mg_random(nonce, sizeof(nonce));
    mg_base64_encode((unsigned char *) nonce, sizeof(nonce), key, sizeof(key));
    mg_xprintf(mg_pfn_iobuf, &c->send,
               "GET %s HTTP/1.1\r\n"
               "Upgrade: websocket\r\n"
               "Host: %.*s\r\n"
               "Connection: Upgrade\r\n"
               "Sec-WebSocket-Version: 13\r\n"
               "Sec-WebSocket-Key: %s\r\n",
               mg_url_uri(url), (int) host.len, host.ptr, key);
    if (fmt != NULL) {
      va_list ap;
      va_start(ap, fmt);
      mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, &ap);
      va_end(ap);
    }
    mg_xprintf(mg_pfn_iobuf, &c->send, "\r\n");
    c->pfn = mg_ws_cb;
    c->pfn_data = NULL;
  }
  return c;
}

void mg_ws_upgrade(struct mg_connection *c, struct mg_http_message *hm,
                   const char *fmt, ...) {
  struct mg_str *wskey = mg_http_get_header(hm, "Sec-WebSocket-Key");
  c->pfn = mg_ws_cb;
  c->pfn_data = NULL;
  if (wskey == NULL) {
    mg_http_reply(c, 426, "", "WS upgrade expected\n");
    c->is_draining = 1;
  } else {
    struct mg_str *wsproto = mg_http_get_header(hm, "Sec-WebSocket-Protocol");
    va_list ap;
    va_start(ap, fmt);
    ws_handshake(c, wskey, wsproto, fmt, &ap);
    va_end(ap);
    c->is_websocket = 1;
    c->is_resp = 0;
    mg_call(c, MG_EV_WS_OPEN, hm);
  }
}

size_t mg_ws_wrap(struct mg_connection *c, size_t len, int op) {
  uint8_t header[14], *p;
  size_t header_len = mkhdr(len, op, c->is_client, header);

  // NOTE: order of operations is important!
  mg_iobuf_add(&c->send, c->send.len, NULL, header_len);
  p = &c->send.buf[c->send.len - len];         // p points to data
  memmove(p, p - header_len, len);             // Shift data
  memcpy(p - header_len, header, header_len);  // Prepend header
  mg_ws_mask(c, len);                          // Mask data

  return c->send.len;
}
