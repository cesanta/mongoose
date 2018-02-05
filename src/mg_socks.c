/*
 * Copyright (c) 2017 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_SOCKS

#include "mg_socks.h"
#include "mg_internal.h"

/*
 *  https://www.ietf.org/rfc/rfc1928.txt paragraph 3, handle client handshake
 *
 *  +----+----------+----------+
 *  |VER | NMETHODS | METHODS  |
 *  +----+----------+----------+
 *  | 1  |    1     | 1 to 255 |
 *  +----+----------+----------+
 */
static void mg_socks5_handshake(struct mg_connection *c) {
  struct mbuf *r = &c->recv_mbuf;
  if (r->buf[0] != MG_SOCKS_VERSION) {
    c->flags |= MG_F_CLOSE_IMMEDIATELY;
  } else if (r->len > 2 && (size_t) r->buf[1] + 2 <= r->len) {
    /* https://www.ietf.org/rfc/rfc1928.txt paragraph 3 */
    unsigned char reply[2] = {MG_SOCKS_VERSION, MG_SOCKS_HANDSHAKE_FAILURE};
    int i;
    for (i = 2; i < r->buf[1] + 2; i++) {
      /* TODO(lsm): support other auth methods */
      if (r->buf[i] == MG_SOCKS_HANDSHAKE_NOAUTH) reply[1] = r->buf[i];
    }
    mbuf_remove(r, 2 + r->buf[1]);
    mg_send(c, reply, sizeof(reply));
    c->flags |= MG_SOCKS_HANDSHAKE_DONE; /* Mark handshake done */
  }
}

static void disband(struct mg_connection *c) {
  struct mg_connection *c2 = (struct mg_connection *) c->user_data;
  if (c2 != NULL) {
    c2->flags |= MG_F_SEND_AND_CLOSE;
    c2->user_data = NULL;
  }
  c->flags |= MG_F_SEND_AND_CLOSE;
  c->user_data = NULL;
}

static void relay_data(struct mg_connection *c) {
  struct mg_connection *c2 = (struct mg_connection *) c->user_data;
  if (c2 != NULL) {
    mg_send(c2, c->recv_mbuf.buf, c->recv_mbuf.len);
    mbuf_remove(&c->recv_mbuf, c->recv_mbuf.len);
  } else {
    c->flags |= MG_F_SEND_AND_CLOSE;
  }
}

static void serv_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_CLOSE) {
    disband(c);
  } else if (ev == MG_EV_RECV) {
    relay_data(c);
  } else if (ev == MG_EV_CONNECT) {
    int res = *(int *) ev_data;
    if (res != 0) LOG(LL_ERROR, ("connect error: %d", res));
  }
}

static void mg_socks5_connect(struct mg_connection *c, const char *addr) {
  struct mg_connection *serv = mg_connect(c->mgr, addr, serv_ev_handler);
  serv->user_data = c;
  c->user_data = serv;
}

/*
 *  Request, https://www.ietf.org/rfc/rfc1928.txt paragraph 4
 *
 *  +----+-----+-------+------+----------+----------+
 *  |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
 *  +----+-----+-------+------+----------+----------+
 *  | 1  |  1  | X'00' |  1   | Variable |    2     |
 *  +----+-----+-------+------+----------+----------+
 */
static void mg_socks5_handle_request(struct mg_connection *c) {
  struct mbuf *r = &c->recv_mbuf;
  unsigned char *p = (unsigned char *) r->buf;
  unsigned char addr_len = 4, reply = MG_SOCKS_SUCCESS;
  int ver, cmd, atyp;
  char addr[300];

  if (r->len < 8) return; /* return if not fully buffered. min DST.ADDR is 2 */
  ver = p[0];
  cmd = p[1];
  atyp = p[3];

  /* TODO(lsm): support other commands */
  if (ver != MG_SOCKS_VERSION || cmd != MG_SOCKS_CMD_CONNECT) {
    reply = MG_SOCKS_CMD_NOT_SUPPORTED;
  } else if (atyp == MG_SOCKS_ADDR_IPV4) {
    addr_len = 4;
    if (r->len < (size_t) addr_len + 6) return; /* return if not buffered */
    snprintf(addr, sizeof(addr), "%d.%d.%d.%d:%d", p[4], p[5], p[6], p[7],
             p[8] << 8 | p[9]);
    mg_socks5_connect(c, addr);
  } else if (atyp == MG_SOCKS_ADDR_IPV6) {
    addr_len = 16;
    if (r->len < (size_t) addr_len + 6) return; /* return if not buffered */
    snprintf(addr, sizeof(addr), "[%x:%x:%x:%x:%x:%x:%x:%x]:%d",
             p[4] << 8 | p[5], p[6] << 8 | p[7], p[8] << 8 | p[9],
             p[10] << 8 | p[11], p[12] << 8 | p[13], p[14] << 8 | p[15],
             p[16] << 8 | p[17], p[18] << 8 | p[19], p[20] << 8 | p[21]);
    mg_socks5_connect(c, addr);
  } else if (atyp == MG_SOCKS_ADDR_DOMAIN) {
    addr_len = p[4] + 1;
    if (r->len < (size_t) addr_len + 6) return; /* return if not buffered */
    snprintf(addr, sizeof(addr), "%.*s:%d", p[4], p + 5,
             p[4 + addr_len] << 8 | p[4 + addr_len + 1]);
    mg_socks5_connect(c, addr);
  } else {
    reply = MG_SOCKS_ADDR_NOT_SUPPORTED;
  }

  /*
   *  Reply, https://www.ietf.org/rfc/rfc1928.txt paragraph 5
   *
   *  +----+-----+-------+------+----------+----------+
   *  |VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
   *  +----+-----+-------+------+----------+----------+
   *  | 1  |  1  | X'00' |  1   | Variable |    2     |
   *  +----+-----+-------+------+----------+----------+
   */
  {
    unsigned char buf[] = {MG_SOCKS_VERSION, reply, 0};
    mg_send(c, buf, sizeof(buf));
  }
  mg_send(c, r->buf + 3, addr_len + 1 + 2);

  mbuf_remove(r, 6 + addr_len);      /* Remove request from the input stream */
  c->flags |= MG_SOCKS_CONNECT_DONE; /* Mark ourselves as connected */
}

static void socks_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_RECV) {
    if (!(c->flags & MG_SOCKS_HANDSHAKE_DONE)) mg_socks5_handshake(c);
    if (c->flags & MG_SOCKS_HANDSHAKE_DONE &&
        !(c->flags & MG_SOCKS_CONNECT_DONE)) {
      mg_socks5_handle_request(c);
    }
    if (c->flags & MG_SOCKS_CONNECT_DONE) relay_data(c);
  } else if (ev == MG_EV_CLOSE) {
    disband(c);
  }
  (void) ev_data;
}

void mg_set_protocol_socks(struct mg_connection *c) {
  c->proto_handler = socks_handler;
}
#endif
