#include "ws.h"

#include "base64.h"
#include "http.h"
#include "log.h"
#include "private.h"
#include "sha1.h"
#include "url.h"
#include "util.h"

struct ws_msg {
  uint8_t flags;
  size_t header_len;
  size_t data_len;
};

static void ws_handshake(struct mg_connection *c, const char *key,
                         size_t key_len, const char *fmt, va_list ap) {
  const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  unsigned char sha[20], b64_sha[30];
  char mem[128], *buf = mem;

  mg_sha1_ctx sha_ctx;
  mg_sha1_init(&sha_ctx);
  mg_sha1_update(&sha_ctx, (unsigned char *) key, key_len);
  mg_sha1_update(&sha_ctx, (unsigned char *) magic, 36);
  mg_sha1_final(sha, &sha_ctx);
  mg_base64_encode(sha, sizeof(sha), (char *) b64_sha);
  buf[0] = '\0';
  if (fmt != NULL) mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  mg_printf(c,
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: %s\r\n"
            "%s\r\n",
            b64_sha, buf);
  if (buf != mem) free(buf);
}

static size_t ws_process(uint8_t *buf, size_t len, struct ws_msg *msg) {
  size_t i, n = 0, mask_len = 0;
  memset(msg, 0, sizeof(*msg));
  if (len >= 2) {
    n = buf[1] & 0x7f;
    mask_len = buf[1] & WEBSOCKET_FLAGS_MASK_FIN ? 4 : 0;
    msg->flags = *(unsigned char *) buf;
    if (n < 126 && len >= mask_len) {
      msg->data_len = n;
      msg->header_len = 2 + mask_len;
    } else if (n == 126 && len >= 4 + mask_len) {
      msg->header_len = 4 + mask_len;
      msg->data_len = mg_ntohs(*(uint16_t *) &buf[2]);
    } else if (len >= 10 + mask_len) {
      msg->header_len = 10 + mask_len;
      msg->data_len =
          (int) (((uint64_t) mg_ntohl(*(uint32_t *) &buf[2])) << 32) +
          mg_ntohl(*(uint32_t *) &buf[6]);
    }
  }
  if (msg->header_len + msg->data_len > len) return 0;
  if (mask_len > 0) {
    uint8_t *p = buf + msg->header_len, *m = p - mask_len;
    for (i = 0; i < msg->data_len; i++) p[i] ^= m[i & 3];
  }
  return msg->header_len + msg->data_len;
}

size_t mg_ws_send(struct mg_connection *c, const char *buf, size_t len,
                  int op) {
  unsigned char header[10], mask[4];
  size_t i, header_len = 0;
  header[0] = (uint8_t)(op | WEBSOCKET_FLAGS_MASK_FIN);
  if (len < 126) {
    header[1] = (unsigned char) len;
    header_len = 2;
  } else if (len < 65536) {
    uint16_t tmp = mg_htons((uint16_t) len);
    header[1] = 126;
    memcpy(&header[2], &tmp, sizeof(tmp));
    header_len = 4;
  } else {
    uint32_t tmp;
    header[1] = 127;
    tmp = mg_htonl((uint32_t)((uint64_t) len >> 32));
    memcpy(&header[2], &tmp, sizeof(tmp));
    tmp = mg_htonl((uint32_t)(len & 0xffffffff));
    memcpy(&header[6], &tmp, sizeof(tmp));
    header_len = 10;
  }
  if (c->is_client) header[1] |= 1 << 7;  // Set masking flag
  mg_send(c, header, header_len);
  if (c->is_client) {
    mg_random(mask, sizeof(mask));
    mg_send(c, mask, sizeof(mask));
    header_len += sizeof(mask);
  }
  LOG(LL_VERBOSE_DEBUG, ("WS out: %d [%.*s]", (int) len, (int) len, buf));
  mg_send(c, buf, len);
  if (c->is_client && c->send.buf != NULL) {
    uint8_t *p = c->send.buf + c->send.len - len;
    for (i = 0; i < len; i++) p[i] ^= mask[i & 3];
  }
  return header_len + len;
}

static void mg_ws_cb(struct mg_connection *c, int ev, void *ev_data,
                     void *fn_data) {
  struct ws_msg msg;

  if (ev == MG_EV_READ) {
    if (!c->is_websocket && c->is_client) {
      int n = mg_http_get_request_len(c->recv.buf, c->recv.len);
      if (n < 0) {
        c->is_closing = 1;  // Some just, not an HTTP request
      } else if (n > 0) {
        if (n < 15 || memcmp(c->recv.buf + 9, "101", 3) != 0) {
          LOG(LL_ERROR,
              ("%lu WS handshake error: %.*s", c->id, 15, c->recv.buf));
          c->is_closing = 1;
        } else {
          struct mg_http_message hm;
          mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
          c->is_websocket = 1;
          mg_call(c, MG_EV_WS_OPEN, &hm);
        }
        mg_iobuf_delete(&c->recv, n);
      } else {
        return;  // A request is not yet received
      }
    }

    while (ws_process(c->recv.buf, c->recv.len, &msg) > 0) {
      char *s = (char *) c->recv.buf + msg.header_len;
      struct mg_ws_message m = {{s, msg.data_len}, msg.flags};
      switch (msg.flags & WEBSOCKET_FLAGS_MASK_OP) {
        case WEBSOCKET_OP_PING:
          LOG(LL_DEBUG, ("%s", "WS PONG"));
          mg_ws_send(c, s, msg.data_len, WEBSOCKET_OP_PONG);
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_PONG:
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_CLOSE:
          LOG(LL_ERROR, ("%lu Got WS CLOSE", c->id));
          mg_call(c, MG_EV_WS_CTL, &m);
          c->is_closing = 1;
          return;
        default: {
          mg_call(c, MG_EV_WS_MSG, &m);
          break;
        }
      }
      mg_iobuf_delete(&c->recv, msg.header_len + msg.data_len);
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
    char nonce[16], key[30], mem1[128], mem2[256], *buf1 = mem1, *buf2 = mem2;
    struct mg_str host = mg_url_host(url);
    int n1 = 0, n2 = 0;
    if (fmt != NULL) {
      va_list ap;
      va_start(ap, fmt);
      n1 = mg_vasprintf(&buf1, sizeof(mem1), fmt, ap);
      va_end(ap);
    }
    // Send handshake request
    // mg_url_host(url, host, sizeof(host));
    mg_random(nonce, sizeof(nonce));
    mg_base64_encode((unsigned char *) nonce, sizeof(nonce), key);
    // LOG(LL_DEBUG, "%p [%s]", uri, uri == NULL ? "???" : uri);
    n2 = mg_asprintf(&buf2, sizeof(mem2),
                     "GET %s HTTP/1.1\r\n"
                     "Upgrade: websocket\r\n"
                     "Host: %.*s\r\n"
                     "Connection: Upgrade\r\n"
                     "%.*s"
                     "Sec-WebSocket-Version: 13\r\n"
                     "Sec-WebSocket-Key: %s\r\n"
                     "\r\n",
                     mg_url_uri(url), (int) host.len, host.ptr, n1, buf1, key);
    mg_send(c, buf2, n2);
    if (buf1 != mem1) free(buf1);
    if (buf2 != mem2) free(buf2);
    c->pfn = mg_ws_cb;
    c->fn_data = fn_data;
  }
  return c;
}

void mg_ws_upgrade(struct mg_connection *c, struct mg_http_message *hm,
                   const char *fmt, ...) {
  struct mg_str *wskey = mg_http_get_header(hm, "Sec-WebSocket-Key");
  c->pfn = mg_ws_cb;
  if (wskey != NULL) {
    va_list ap;
    va_start(ap, fmt);
    ws_handshake(c, wskey->ptr, wskey->len, fmt, ap);
    va_end(ap);
  }
  c->is_websocket = 1;
}
