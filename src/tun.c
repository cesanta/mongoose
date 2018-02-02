/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_TUN

#include "common/cs_dbg.h"
#include "mongoose/src/http.h"
#include "mongoose/src/internal.h"
#include "mongoose/src/net.h"
#include "mongoose/src/net_if_tun.h"
#include "mongoose/src/tun.h"
#include "mongoose/src/util.h"

static void mg_tun_reconnect(struct mg_tun_client *client, int timeout);

static void mg_tun_init_client(struct mg_tun_client *client, struct mg_mgr *mgr,
                               struct mg_iface *iface, const char *dispatcher,
                               struct mg_tun_ssl_opts ssl) {
  client->mgr = mgr;
  client->iface = iface;
  client->disp_url = dispatcher;
  client->last_stream_id = 0;
  client->ssl = ssl;

  client->disp = NULL;      /* will be set by mg_tun_reconnect */
  client->listener = NULL;  /* will be set by mg_do_bind */
  client->reconnect = NULL; /* will be set by mg_tun_reconnect */
}

void mg_tun_log_frame(struct mg_tun_frame *frame) {
  LOG(LL_DEBUG, ("Got TUN frame: type=0x%x, flags=0x%x stream_id=0x%x, "
                 "len=%d",
                 frame->type, frame->flags, (unsigned int) frame->stream_id,
                 (int) frame->body.len));
#if MG_ENABLE_HEXDUMP
  {
    char hex[512];
    mg_hexdump(frame->body.p, frame->body.len, hex, sizeof(hex) - 1);
    hex[sizeof(hex) - 1] = '\0';
    LOG(LL_DEBUG, ("body:\n%s", hex));
  }
#else
  LOG(LL_DEBUG, ("body: '%.*s'", (int) frame->body.len, frame->body.p));
#endif
}

static void mg_tun_close_all(struct mg_tun_client *client) {
  struct mg_connection *nc;
  for (nc = client->mgr->active_connections; nc != NULL; nc = nc->next) {
    if (nc->iface == client->iface && !(nc->flags & MG_F_LISTENING)) {
      LOG(LL_DEBUG, ("Closing tunneled connection %p", nc));
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      /* mg_close_conn(nc); */
    }
  }
}

static void mg_tun_client_handler(struct mg_connection *nc, int ev,
                                  void *ev_data MG_UD_ARG(void *user_data)) {
#if !MG_ENABLE_CALLBACK_USERDATA
  void *user_data = nc->user_data;
#else
  (void) nc;
#endif
  struct mg_tun_client *client = (struct mg_tun_client *) user_data;

  switch (ev) {
    case MG_EV_CONNECT: {
      int err = *(int *) ev_data;

      if (err) {
        LOG(LL_ERROR, ("Cannot connect to the tunnel dispatcher: %d", err));
      } else {
        LOG(LL_INFO, ("Connected to the tunnel dispatcher"));
      }
      break;
    }
    case MG_EV_HTTP_REPLY: {
      struct http_message *hm = (struct http_message *) ev_data;

      if (hm->resp_code != 200) {
        LOG(LL_ERROR,
            ("Tunnel dispatcher reply non-OK status code %d", hm->resp_code));
      }
      break;
    }
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      LOG(LL_INFO, ("Tunnel dispatcher handshake done"));
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *) ev_data;
      struct mg_connection *tc;
      struct mg_tun_frame frame;

      if (mg_tun_parse_frame(wm->data, wm->size, &frame) == -1) {
        LOG(LL_ERROR, ("Got invalid tun frame dropping"));
        break;
      }

      mg_tun_log_frame(&frame);

      tc = mg_tun_if_find_conn(client, frame.stream_id);
      if (tc == NULL) {
        if (frame.body.len > 0) {
          LOG(LL_DEBUG, ("Got frame after receiving end has been closed"));
        }
        break;
      }
      if (frame.body.len > 0) {
        mg_if_recv_tcp_cb(tc, (void *) frame.body.p, frame.body.len,
                          0 /* own */);
      }
      if (frame.flags & MG_TUN_F_END_STREAM) {
        LOG(LL_DEBUG, ("Closing tunneled connection because got end of stream "
                       "from other end"));
        tc->flags |= MG_F_CLOSE_IMMEDIATELY;
        mg_close_conn(tc);
      }
      break;
    }
    case MG_EV_CLOSE: {
      LOG(LL_DEBUG, ("Closing all tunneled connections"));
      /*
       * The client might have been already freed when the listening socket is
       * closed.
       */
      if (client != NULL) {
        mg_tun_close_all(client);
        client->disp = NULL;
        LOG(LL_INFO, ("Dispatcher connection is no more, reconnecting"));
        /* TODO(mkm): implement exp back off */
        mg_tun_reconnect(client, MG_TUN_RECONNECT_INTERVAL);
      }
      break;
    }
    default:
      break;
  }
}

static void mg_tun_do_reconnect(struct mg_tun_client *client) {
  struct mg_connection *dc;
  struct mg_connect_opts opts;
  memset(&opts, 0, sizeof(opts));
#if MG_ENABLE_SSL
  opts.ssl_cert = client->ssl.ssl_cert;
  opts.ssl_key = client->ssl.ssl_key;
  opts.ssl_ca_cert = client->ssl.ssl_ca_cert;
#endif
  /* HTTP/Websocket listener */
  if ((dc = mg_connect_ws_opt(client->mgr, MG_CB(mg_tun_client_handler, client),
                              opts, client->disp_url, MG_TUN_PROTO_NAME,
                              NULL)) == NULL) {
    LOG(LL_ERROR,
        ("Cannot connect to WS server on addr [%s]\n", client->disp_url));
    return;
  }

  client->disp = dc;
#if !MG_ENABLE_CALLBACK_USERDATA
  dc->user_data = client;
#endif
}

void mg_tun_reconnect_ev_handler(struct mg_connection *nc, int ev,
                                 void *ev_data MG_UD_ARG(void *user_data)) {
#if !MG_ENABLE_CALLBACK_USERDATA
  void *user_data = nc->user_data;
#else
  (void) nc;
#endif
  struct mg_tun_client *client = (struct mg_tun_client *) user_data;
  (void) ev_data;

  switch (ev) {
    case MG_EV_TIMER:
      if (!(client->listener->flags & MG_F_TUN_DO_NOT_RECONNECT)) {
        mg_tun_do_reconnect(client);
      } else {
        /* Reconnecting is suppressed, we'll check again at the next poll */
        mg_tun_reconnect(client, 0);
      }
      break;
  }
}

static void mg_tun_reconnect(struct mg_tun_client *client, int timeout) {
  if (client->reconnect == NULL) {
    client->reconnect = mg_add_sock(client->mgr, INVALID_SOCKET,
                                    MG_CB(mg_tun_reconnect_ev_handler, client));
#if !MG_ENABLE_CALLBACK_USERDATA
    client->reconnect->user_data = client;
#endif
  }
  client->reconnect->ev_timer_time = mg_time() + timeout;
}

static struct mg_tun_client *mg_tun_create_client(struct mg_mgr *mgr,
                                                  const char *dispatcher,
                                                  struct mg_tun_ssl_opts ssl) {
  struct mg_tun_client *client = NULL;
  struct mg_iface *iface = mg_find_iface(mgr, &mg_tun_iface_vtable, NULL);
  if (iface == NULL) {
    LOG(LL_ERROR, ("The tun feature requires the manager to have a tun "
                   "interface enabled"));
    return NULL;
  }

  client = (struct mg_tun_client *) MG_MALLOC(sizeof(*client));
  mg_tun_init_client(client, mgr, iface, dispatcher, ssl);
  iface->data = client;

  /*
   * We need to give application a chance to set MG_F_TUN_DO_NOT_RECONNECT on a
   * listening connection right after mg_tun_bind_opt() returned it, so we
   * should use mg_tun_reconnect() here, instead of mg_tun_do_reconnect()
   */
  mg_tun_reconnect(client, 0);
  return client;
}

void mg_tun_destroy_client(struct mg_tun_client *client) {
  /*
   *  NOTE:
   * `client` is NULL in case of OOM
   * `client->disp` is NULL if connection failed
   * `client->iface is NULL is `mg_find_iface` failed
   */

  if (client != NULL && client->disp != NULL) {
    /* the dispatcher connection handler will in turn close all tunnels */
    client->disp->flags |= MG_F_CLOSE_IMMEDIATELY;
    /* this is used as a signal to other tun handlers that the party is over */
    client->disp->user_data = NULL;
  }

  if (client != NULL && client->reconnect != NULL) {
    client->reconnect->flags |= MG_F_CLOSE_IMMEDIATELY;
  }

  if (client != NULL && client->iface != NULL) {
    client->iface->data = NULL;
  }

  MG_FREE(client);
}

static struct mg_connection *mg_tun_do_bind(struct mg_tun_client *client,
                                            MG_CB(mg_event_handler_t handler,
                                                  void *user_data),
                                            struct mg_bind_opts opts) {
  struct mg_connection *lc;
  opts.iface = client->iface;
  lc = mg_bind_opt(client->mgr, ":1234" /* dummy port */,
                   MG_CB(handler, user_data), opts);
  client->listener = lc;
  return lc;
}

struct mg_connection *mg_tun_bind_opt(struct mg_mgr *mgr,
                                      const char *dispatcher,
                                      MG_CB(mg_event_handler_t handler,
                                            void *user_data),
                                      struct mg_bind_opts opts) {
#if MG_ENABLE_SSL
  struct mg_tun_ssl_opts ssl = {opts.ssl_cert, opts.ssl_key, opts.ssl_ca_cert};
#else
  struct mg_tun_ssl_opts ssl = {0};
#endif
  struct mg_tun_client *client = mg_tun_create_client(mgr, dispatcher, ssl);
  if (client == NULL) {
    return NULL;
  }
#if MG_ENABLE_SSL
  /* these options don't make sense in the local mouth of the tunnel */
  opts.ssl_cert = NULL;
  opts.ssl_key = NULL;
  opts.ssl_ca_cert = NULL;
#endif
  return mg_tun_do_bind(client, MG_CB(handler, user_data), opts);
}

int mg_tun_parse_frame(void *data, size_t len, struct mg_tun_frame *frame) {
  const size_t header_size = sizeof(uint32_t) + sizeof(uint8_t) * 2;
  if (len < header_size) {
    return -1;
  }

  frame->type = *(uint8_t *) (data);
  frame->flags = *(uint8_t *) ((char *) data + 1);
  memcpy(&frame->stream_id, (char *) data + 2, sizeof(uint32_t));
  frame->stream_id = ntohl(frame->stream_id);
  frame->body.p = (char *) data + header_size;
  frame->body.len = len - header_size;
  return 0;
}

void mg_tun_send_frame(struct mg_connection *ws, uint32_t stream_id,
                       uint8_t type, uint8_t flags, struct mg_str msg) {
  stream_id = htonl(stream_id);
  {
    struct mg_str parts[] = {
        {(char *) &type, sizeof(type)},
        {(char *) &flags, sizeof(flags)},
        {(char *) &stream_id, sizeof(stream_id)},
        {msg.p, msg.len} /* vc6 doesn't like just `msg` here */};
    mg_send_websocket_framev(ws, WEBSOCKET_OP_BINARY, parts,
                             sizeof(parts) / sizeof(parts[0]));
  }
}

#endif /* MG_ENABLE_TUN */
