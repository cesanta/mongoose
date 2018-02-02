/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_HTTP && MG_ENABLE_HTTP_WEBSOCKET

#include "common/cs_sha1.h"

#ifndef MG_WEBSOCKET_PING_INTERVAL_SECONDS
#define MG_WEBSOCKET_PING_INTERVAL_SECONDS 5
#endif

#define FLAGS_MASK_FIN (1 << 7)
#define FLAGS_MASK_OP 0x0f

static int mg_is_ws_fragment(unsigned char flags) {
  return (flags & FLAGS_MASK_FIN) == 0 ||
         (flags & FLAGS_MASK_OP) == WEBSOCKET_OP_CONTINUE;
}

static int mg_is_ws_first_fragment(unsigned char flags) {
  return (flags & FLAGS_MASK_FIN) == 0 &&
         (flags & FLAGS_MASK_OP) != WEBSOCKET_OP_CONTINUE;
}

static int mg_is_ws_control_frame(unsigned char flags) {
  unsigned char op = (flags & FLAGS_MASK_OP);
  return op == WEBSOCKET_OP_CLOSE || op == WEBSOCKET_OP_PING ||
         op == WEBSOCKET_OP_PONG;
}

static void mg_handle_incoming_websocket_frame(struct mg_connection *nc,
                                               struct websocket_message *wsm) {
  if (wsm->flags & 0x8) {
    mg_call(nc, nc->handler, nc->user_data, MG_EV_WEBSOCKET_CONTROL_FRAME, wsm);
  } else {
    mg_call(nc, nc->handler, nc->user_data, MG_EV_WEBSOCKET_FRAME, wsm);
  }
}

static struct mg_ws_proto_data *mg_ws_get_proto_data(struct mg_connection *nc) {
  struct mg_http_proto_data *htd = mg_http_get_proto_data(nc);
  return (htd != NULL ? &htd->ws_data : NULL);
}

/*
 * Sends a Close websocket frame with the given data, and closes the underlying
 * connection. If `len` is ~0, strlen(data) is used.
 */
static void mg_ws_close(struct mg_connection *nc, const void *data,
                        size_t len) {
  if ((int) len == ~0) {
    len = strlen((const char *) data);
  }
  mg_send_websocket_frame(nc, WEBSOCKET_OP_CLOSE, data, len);
  nc->flags |= MG_F_SEND_AND_CLOSE;
}

static int mg_deliver_websocket_data(struct mg_connection *nc) {
  /* Using unsigned char *, cause of integer arithmetic below */
  uint64_t i, data_len = 0, frame_len = 0, new_data_len = nc->recv_mbuf.len,
              len, mask_len = 0, header_len = 0;
  struct mg_ws_proto_data *wsd = mg_ws_get_proto_data(nc);
  unsigned char *new_data = (unsigned char *) nc->recv_mbuf.buf,
                *e = (unsigned char *) nc->recv_mbuf.buf + nc->recv_mbuf.len;
  uint8_t flags;
  int ok, reass;

  if (wsd->reass_len > 0) {
    /*
     * We already have some previously received data which we need to
     * reassemble and deliver to the client code when we get the final
     * fragment.
     *
     * NOTE: it doesn't mean that the current message must be a continuation:
     * it might be a control frame (Close, Ping or Pong), which should be
     * handled without breaking the fragmented message.
     */

    size_t existing_len = wsd->reass_len;
    assert(new_data_len >= existing_len);

    new_data += existing_len;
    new_data_len -= existing_len;
  }

  flags = new_data[0];

  reass = new_data_len > 0 && mg_is_ws_fragment(flags) &&
          !(nc->flags & MG_F_WEBSOCKET_NO_DEFRAG);

  if (reass && mg_is_ws_control_frame(flags)) {
    /*
     * Control frames can't be fragmented, so if we encounter fragmented
     * control frame, close connection immediately.
     */
    mg_ws_close(nc, "fragmented control frames are illegal", ~0);
    return 0;
  } else if (new_data_len > 0 && !reass && !mg_is_ws_control_frame(flags) &&
             wsd->reass_len > 0) {
    /*
     * When in the middle of a fragmented message, only the continuations
     * and control frames are allowed.
     */
    mg_ws_close(nc, "non-continuation in the middle of a fragmented message",
                ~0);
    return 0;
  }

  if (new_data_len >= 2) {
    len = new_data[1] & 0x7f;
    mask_len = new_data[1] & FLAGS_MASK_FIN ? 4 : 0;
    if (len < 126 && new_data_len >= mask_len) {
      data_len = len;
      header_len = 2 + mask_len;
    } else if (len == 126 && new_data_len >= 4 + mask_len) {
      header_len = 4 + mask_len;
      data_len = ntohs(*(uint16_t *) &new_data[2]);
    } else if (new_data_len >= 10 + mask_len) {
      header_len = 10 + mask_len;
      data_len = (((uint64_t) ntohl(*(uint32_t *) &new_data[2])) << 32) +
                 ntohl(*(uint32_t *) &new_data[6]);
    }
  }

  frame_len = header_len + data_len;
  ok = (frame_len > 0 && frame_len <= new_data_len);

  /* Check for overflow */
  if (frame_len < header_len || frame_len < data_len) {
    ok = 0;
    mg_ws_close(nc, "overflowed message", ~0);
  }

  if (ok) {
    size_t cleanup_len = 0;
    struct websocket_message wsm;

    wsm.size = (size_t) data_len;
    wsm.data = new_data + header_len;
    wsm.flags = flags;

    /* Apply mask if necessary */
    if (mask_len > 0) {
      for (i = 0; i < data_len; i++) {
        new_data[i + header_len] ^= (new_data + header_len - mask_len)[i % 4];
      }
    }

    if (reass) {
      /* This is a message fragment */

      if (mg_is_ws_first_fragment(flags)) {
        /*
         * On the first fragmented frame, skip the first byte (op) and also
         * reset size to 1 (op), it'll be incremented with the data len below.
         */
        new_data += 1;
        wsd->reass_len = 1 /* op */;
      }

      /* Append this frame to the reassembled buffer */
      memmove(new_data, wsm.data, e - wsm.data);
      wsd->reass_len += wsm.size;
      nc->recv_mbuf.len -= wsm.data - new_data;

      if (flags & FLAGS_MASK_FIN) {
        /* On last fragmented frame - call user handler and remove data */
        wsm.flags = FLAGS_MASK_FIN | nc->recv_mbuf.buf[0];
        wsm.data = (unsigned char *) nc->recv_mbuf.buf + 1 /* op */;
        wsm.size = wsd->reass_len - 1 /* op */;
        cleanup_len = wsd->reass_len;
        wsd->reass_len = 0;

        /* Pass reassembled message to the client code. */
        mg_handle_incoming_websocket_frame(nc, &wsm);
        mbuf_remove(&nc->recv_mbuf, cleanup_len); /* Cleanup frame */
      }
    } else {
      /*
       * This is a complete message, not a fragment. It might happen in between
       * of a fragmented message (in this case, WebSocket protocol requires
       * current message to be a control frame).
       */
      cleanup_len = (size_t) frame_len;

      /* First of all, check if we need to react on a control frame. */
      switch (flags & FLAGS_MASK_OP) {
        case WEBSOCKET_OP_PING:
          mg_send_websocket_frame(nc, WEBSOCKET_OP_PONG, wsm.data, wsm.size);
          break;

        case WEBSOCKET_OP_CLOSE:
          mg_ws_close(nc, wsm.data, wsm.size);
          break;
      }

      /* Pass received message to the client code. */
      mg_handle_incoming_websocket_frame(nc, &wsm);

      /* Cleanup frame */
      memmove(nc->recv_mbuf.buf + wsd->reass_len,
              nc->recv_mbuf.buf + wsd->reass_len + cleanup_len,
              nc->recv_mbuf.len - wsd->reass_len - cleanup_len);
      nc->recv_mbuf.len -= cleanup_len;
    }
  }

  return ok;
}

struct ws_mask_ctx {
  size_t pos; /* zero means unmasked */
  uint32_t mask;
};

static uint32_t mg_ws_random_mask(void) {
  uint32_t mask;
/*
 * The spec requires WS client to generate hard to
 * guess mask keys. From RFC6455, Section 5.3:
 *
 * The unpredictability of the masking key is essential to prevent
 * authors of malicious applications from selecting the bytes that appear on
 * the wire.
 *
 * Hence this feature is essential when the actual end user of this API
 * is untrusted code that wouldn't have access to a lower level net API
 * anyway (e.g. web browsers). Hence this feature is low prio for most
 * mongoose use cases and thus can be disabled, e.g. when porting to a platform
 * that lacks rand().
 */
#if MG_DISABLE_WS_RANDOM_MASK
  mask = 0xefbeadde; /* generated with a random number generator, I swear */
#else
  if (sizeof(long) >= 4) {
    mask = (uint32_t) rand();
  } else if (sizeof(long) == 2) {
    mask = (uint32_t) rand() << 16 | (uint32_t) rand();
  }
#endif
  return mask;
}

static void mg_send_ws_header(struct mg_connection *nc, int op, size_t len,
                              struct ws_mask_ctx *ctx) {
  int header_len;
  unsigned char header[10];

  header[0] =
      (op & WEBSOCKET_DONT_FIN ? 0x0 : FLAGS_MASK_FIN) | (op & FLAGS_MASK_OP);
  if (len < 126) {
    header[1] = (unsigned char) len;
    header_len = 2;
  } else if (len < 65535) {
    uint16_t tmp = htons((uint16_t) len);
    header[1] = 126;
    memcpy(&header[2], &tmp, sizeof(tmp));
    header_len = 4;
  } else {
    uint32_t tmp;
    header[1] = 127;
    tmp = htonl((uint32_t)((uint64_t) len >> 32));
    memcpy(&header[2], &tmp, sizeof(tmp));
    tmp = htonl((uint32_t)(len & 0xffffffff));
    memcpy(&header[6], &tmp, sizeof(tmp));
    header_len = 10;
  }

  /* client connections enable masking */
  if (nc->listener == NULL) {
    header[1] |= 1 << 7; /* set masking flag */
    mg_send(nc, header, header_len);
    ctx->mask = mg_ws_random_mask();
    mg_send(nc, &ctx->mask, sizeof(ctx->mask));
    ctx->pos = nc->send_mbuf.len;
  } else {
    mg_send(nc, header, header_len);
    ctx->pos = 0;
  }
}

static void mg_ws_mask_frame(struct mbuf *mbuf, struct ws_mask_ctx *ctx) {
  size_t i;
  if (ctx->pos == 0) return;
  for (i = 0; i < (mbuf->len - ctx->pos); i++) {
    mbuf->buf[ctx->pos + i] ^= ((char *) &ctx->mask)[i % 4];
  }
}

void mg_send_websocket_frame(struct mg_connection *nc, int op, const void *data,
                             size_t len) {
  struct ws_mask_ctx ctx;
  DBG(("%p %d %d", nc, op, (int) len));
  mg_send_ws_header(nc, op, len, &ctx);
  mg_send(nc, data, len);

  mg_ws_mask_frame(&nc->send_mbuf, &ctx);

  if (op == WEBSOCKET_OP_CLOSE) {
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}

void mg_send_websocket_framev(struct mg_connection *nc, int op,
                              const struct mg_str *strv, int strvcnt) {
  struct ws_mask_ctx ctx;
  int i;
  int len = 0;
  for (i = 0; i < strvcnt; i++) {
    len += strv[i].len;
  }

  mg_send_ws_header(nc, op, len, &ctx);

  for (i = 0; i < strvcnt; i++) {
    mg_send(nc, strv[i].p, strv[i].len);
  }

  mg_ws_mask_frame(&nc->send_mbuf, &ctx);

  if (op == WEBSOCKET_OP_CLOSE) {
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}

void mg_printf_websocket_frame(struct mg_connection *nc, int op,
                               const char *fmt, ...) {
  char mem[MG_VPRINTF_BUFFER_SIZE], *buf = mem;
  va_list ap;
  int len;

  va_start(ap, fmt);
  if ((len = mg_avprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
    mg_send_websocket_frame(nc, op, buf, len);
  }
  va_end(ap);

  if (buf != mem && buf != NULL) {
    MG_FREE(buf);
  }
}

MG_INTERNAL void mg_ws_handler(struct mg_connection *nc, int ev,
                               void *ev_data MG_UD_ARG(void *user_data)) {
  mg_call(nc, nc->handler, nc->user_data, ev, ev_data);

  switch (ev) {
    case MG_EV_RECV:
      do {
      } while (mg_deliver_websocket_data(nc));
      break;
    case MG_EV_POLL:
      /* Ping idle websocket connections */
      {
        time_t now = *(time_t *) ev_data;
        if (nc->flags & MG_F_IS_WEBSOCKET &&
            now > nc->last_io_time + MG_WEBSOCKET_PING_INTERVAL_SECONDS) {
          mg_send_websocket_frame(nc, WEBSOCKET_OP_PING, "", 0);
        }
      }
      break;
    default:
      break;
  }
#if MG_ENABLE_CALLBACK_USERDATA
  (void) user_data;
#endif
}

#ifndef MG_EXT_SHA1
void mg_hash_sha1_v(size_t num_msgs, const uint8_t *msgs[],
                    const size_t *msg_lens, uint8_t *digest) {
  size_t i;
  cs_sha1_ctx sha_ctx;
  cs_sha1_init(&sha_ctx);
  for (i = 0; i < num_msgs; i++) {
    cs_sha1_update(&sha_ctx, msgs[i], msg_lens[i]);
  }
  cs_sha1_final(digest, &sha_ctx);
}
#else
extern void mg_hash_sha1_v(size_t num_msgs, const uint8_t *msgs[],
                           const size_t *msg_lens, uint8_t *digest);
#endif

MG_INTERNAL void mg_ws_handshake(struct mg_connection *nc,
                                 const struct mg_str *key,
                                 struct http_message *hm) {
  static const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  const uint8_t *msgs[2] = {(const uint8_t *) key->p, (const uint8_t *) magic};
  const size_t msg_lens[2] = {key->len, 36};
  unsigned char sha[20];
  char b64_sha[30];
  struct mg_str *s;

  mg_hash_sha1_v(2, msgs, msg_lens, sha);
  mg_base64_encode(sha, sizeof(sha), b64_sha);
  mg_printf(nc, "%s",
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n");

  s = mg_get_http_header(hm, "Sec-WebSocket-Protocol");
  if (s != NULL) {
    mg_printf(nc, "Sec-WebSocket-Protocol: %.*s\r\n", (int) s->len, s->p);
  }
  mg_printf(nc, "Sec-WebSocket-Accept: %s%s", b64_sha, "\r\n\r\n");

  DBG(("%p %.*s %s", nc, (int) key->len, key->p, b64_sha));
}

void mg_send_websocket_handshake2(struct mg_connection *nc, const char *path,
                                  const char *host, const char *protocol,
                                  const char *extra_headers) {
  mg_send_websocket_handshake3(nc, path, host, protocol, extra_headers, NULL,
                               NULL);
}

void mg_send_websocket_handshake3(struct mg_connection *nc, const char *path,
                                  const char *host, const char *protocol,
                                  const char *extra_headers, const char *user,
                                  const char *pass) {
  mg_send_websocket_handshake3v(nc, mg_mk_str(path), mg_mk_str(host),
                                mg_mk_str(protocol), mg_mk_str(extra_headers),
                                mg_mk_str(user), mg_mk_str(pass));
}

void mg_send_websocket_handshake3v(struct mg_connection *nc,
                                   const struct mg_str path,
                                   const struct mg_str host,
                                   const struct mg_str protocol,
                                   const struct mg_str extra_headers,
                                   const struct mg_str user,
                                   const struct mg_str pass) {
  struct mbuf auth;
  char key[25];
  uint32_t nonce[4];
  nonce[0] = mg_ws_random_mask();
  nonce[1] = mg_ws_random_mask();
  nonce[2] = mg_ws_random_mask();
  nonce[3] = mg_ws_random_mask();
  mg_base64_encode((unsigned char *) &nonce, sizeof(nonce), key);

  mbuf_init(&auth, 0);
  if (user.len > 0) {
    mg_basic_auth_header(user, pass, &auth);
  }

  /*
   * NOTE: the  (auth.buf == NULL ? "" : auth.buf) is because cc3200 libc is
   * broken: it doesn't like zero length to be passed to %.*s
   * i.e. sprintf("f%.*so", (int)0, NULL), yields `f\0o`.
   * because it handles NULL specially (and incorrectly).
   */
  mg_printf(nc,
            "GET %.*s HTTP/1.1\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "%.*s"
            "Sec-WebSocket-Version: 13\r\n"
            "Sec-WebSocket-Key: %s\r\n",
            (int) path.len, path.p, (int) auth.len,
            (auth.buf == NULL ? "" : auth.buf), key);

  /* TODO(mkm): take default hostname from http proto data if host == NULL */
  if (host.len > 0) {
    int host_len = (int) (path.p - host.p); /* Account for possible :PORT */
    mg_printf(nc, "Host: %.*s\r\n", host_len, host.p);
  }
  if (protocol.len > 0) {
    mg_printf(nc, "Sec-WebSocket-Protocol: %.*s\r\n", (int) protocol.len,
              protocol.p);
  }
  if (extra_headers.len > 0) {
    mg_printf(nc, "%.*s", (int) extra_headers.len, extra_headers.p);
  }
  mg_printf(nc, "\r\n");

  mbuf_free(&auth);
}

void mg_send_websocket_handshake(struct mg_connection *nc, const char *path,
                                 const char *extra_headers) {
  struct mg_str null_str = MG_NULL_STR;
  mg_send_websocket_handshake3v(
      nc, mg_mk_str(path), null_str /* host */, null_str /* protocol */,
      mg_mk_str(extra_headers), null_str /* user */, null_str /* pass */);
}

struct mg_connection *mg_connect_ws_opt(
    struct mg_mgr *mgr, MG_CB(mg_event_handler_t ev_handler, void *user_data),
    struct mg_connect_opts opts, const char *url, const char *protocol,
    const char *extra_headers) {
  struct mg_str null_str = MG_NULL_STR;
  struct mg_str host = MG_NULL_STR, path = MG_NULL_STR, user_info = MG_NULL_STR;
  struct mg_connection *nc =
      mg_connect_http_base(mgr, MG_CB(ev_handler, user_data), opts, "http",
                           "ws", "https", "wss", url, &path, &user_info, &host);
  if (nc != NULL) {
    mg_send_websocket_handshake3v(nc, path, host, mg_mk_str(protocol),
                                  mg_mk_str(extra_headers), user_info,
                                  null_str);
  }
  return nc;
}

struct mg_connection *mg_connect_ws(
    struct mg_mgr *mgr, MG_CB(mg_event_handler_t ev_handler, void *user_data),
    const char *url, const char *protocol, const char *extra_headers) {
  struct mg_connect_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_connect_ws_opt(mgr, MG_CB(ev_handler, user_data), opts, url,
                           protocol, extra_headers);
}
#endif /* MG_ENABLE_HTTP && MG_ENABLE_HTTP_WEBSOCKET */
