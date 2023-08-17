// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example socks5 server. To test,
//   1. Run `make` to start this server on port 1080
//   2. Run `curl`

#include "mongoose.h"

static const char *s_lsn = "tcp://localhost:1080";  // Listening address

enum {
  VERSION = 5,                  // Socks protocol version
  STATE_HANDSHAKE = 0,          // Connection state: in handshake
  STATE_REQUEST = 1,            // Connection state: connecting
  STATE_ESTABLISHED = 2,        // Connection state: established
  HANDSHAKE_NOAUTH = 0,         // Handshake method - no authentication
  HANDSHAKE_GSSAPI = 1,         // Handshake method - GSSAPI auth
  HANDSHAKE_USERPASS = 2,       // Handshake method - user/password auth
  HANDSHAKE_FAILURE = 0xff,     // Handshake method - failure
  CMD_CONNECT = 1,              // Command: CONNECT
  CMD_BIND = 2,                 // Command: BIND
  CMD_UDP_ASSOCIATE = 3,        // Command: UDP ASSOCIATE
  ADDR_TYPE_IPV4 = 1,           // Address type: IPv4
  ADDR_TYPE_DOMAIN = 3,         // Address type: Domain name
  ADDR_TYPE_IPV6 = 4,           // Address type: IPv6
  RESP_SUCCESS = 0,             // Response: success
  RESP_FAILURE = 1,             // Response: failure
  RESP_NOT_ALLOWED = 2,         // Response status
  RESP_NET_UNREACHABLE = 3,     // Response status
  RESP_HOST_UNREACHABLE = 4,    // Response status
  RESP_CONN_REFUSED = 5,        // Response status
  RESP_TTL_EXPIRED = 6,         // Response status
  RESP_CMD_NOT_SUPPORTED = 7,   // Response status
  RESP_ADDR_NOT_SUPPORTED = 8,  // Response status
};

//  https://www.ietf.org/rfc/rfc1928.txt paragraph 3, handle client handshake
//  +----+----------+----------+
//  |VER | NMETHODS | METHODS  |
//  +----+----------+----------+
//  | 1  |    1     | 1 to 255 |
//  +----+----------+----------+
static void handshake(struct mg_connection *c) {
  struct mg_iobuf *r = &c->recv;
  if (r->buf[0] != VERSION) {
    c->is_draining = 1;
  } else if (r->len > 2 && (size_t) r->buf[1] + 2 <= r->len) {
    /* https://www.ietf.org/rfc/rfc1928.txt paragraph 3 */
    uint8_t reply[2] = {VERSION, HANDSHAKE_FAILURE};
    int i;
    for (i = 2; i < r->buf[1] + 2; i++) {
      // TODO(lsm): support other auth methods
      if (r->buf[i] == HANDSHAKE_NOAUTH) reply[1] = r->buf[i];
    }
    mg_iobuf_del(r, 0, 2 + r->buf[1]);
    mg_send(c, reply, sizeof(reply));
    c->data[0] = STATE_REQUEST;
  }
}

static void disband(struct mg_connection *c) {
  struct mg_connection *c2 = (struct mg_connection *) c->fn_data;
  if (c2 != NULL) {
    c2->is_draining = 1;
    c2->fn_data = NULL;
  }
  c->is_draining = 1;
  c->fn_data = NULL;
}

static void exchange(struct mg_connection *c) {
  struct mg_connection *c2 = (struct mg_connection *) c->fn_data;
  if (c2 != NULL) {
    mg_send(c2, c->recv.buf, c->recv.len);
    mg_iobuf_del(&c->recv, 0, c->recv.len);
  } else {
    c->is_draining = 1;
  }
}

static void fn2(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_READ) {
    exchange(c);
  } else if (ev == MG_EV_CLOSE) {
    disband(c);
  }
  (void) ev_data;
  (void) fn_data;
}

//  Request, https://www.ietf.org/rfc/rfc1928.txt paragraph 4
//  +----+-----+-------+------+----------+----------+
//  |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
//  +----+-----+-------+------+----------+----------+
//  | 1  |  1  | X'00' |  1   | Variable |    2     |
//  +----+-----+-------+------+----------+----------+
static void request(struct mg_connection *c) {
  struct mg_iobuf *r = &c->recv;
  uint8_t *p = r->buf, addr_len = 4, reply = RESP_SUCCESS;
  int ver, cmd, atyp;
  char addr[1024];

  if (r->len < 8) return;  // return if not fully buffered. min DST.ADDR is 2
  ver = p[0];
  cmd = p[1];
  atyp = p[3];

  // TODO(lsm): support other commands
  if (ver != VERSION || cmd != CMD_CONNECT) {
    reply = RESP_CMD_NOT_SUPPORTED;
  } else if (atyp == ADDR_TYPE_IPV4) {
    addr_len = 4;
    if (r->len < (size_t) addr_len + 6) return; /* return if not buffered */
    snprintf(addr, sizeof(addr), "tcp://%d.%d.%d.%d:%d", p[4], p[5], p[6], p[7],
             p[8] << 8 | p[9]);
    c->fn_data = mg_connect(c->mgr, addr, fn2, c);
  } else if (atyp == ADDR_TYPE_IPV6) {
    addr_len = 16;
    if (r->len < (size_t) addr_len + 6) return; /* return if not buffered */
    snprintf(addr, sizeof(addr), "tcp://[%x:%x:%x:%x:%x:%x:%x:%x]:%d",
             p[4] << 8 | p[5], p[6] << 8 | p[7], p[8] << 8 | p[9],
             p[10] << 8 | p[11], p[12] << 8 | p[13], p[14] << 8 | p[15],
             p[16] << 8 | p[17], p[18] << 8 | p[19], p[20] << 8 | p[21]);
    c->fn_data = mg_connect(c->mgr, addr, fn2, c);
  } else if (atyp == ADDR_TYPE_DOMAIN) {
    addr_len = p[4] + 1;
    if (r->len < (size_t) addr_len + 6) return; /* return if not buffered */
    snprintf(addr, sizeof(addr), "tcp://%.*s:%d", p[4], p + 5,
             p[4 + addr_len] << 8 | p[4 + addr_len + 1]);
    c->fn_data = mg_connect(c->mgr, addr, fn2, c);
  } else {
    reply = RESP_ADDR_NOT_SUPPORTED;
  }

  //  Reply, https://www.ietf.org/rfc/rfc1928.txt paragraph 5
  //
  //  +----+-----+-------+------+----------+----------+
  //  |VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
  //  +----+-----+-------+------+----------+----------+
  //  | 1  |  1  | X'00' |  1   | Variable |    2     |
  //  +----+-----+-------+------+----------+----------+
  {
    uint8_t buf[] = {VERSION, reply, 0};
    mg_send(c, buf, sizeof(buf));
  }
  mg_send(c, r->buf + 3, addr_len + 1 + 2);
  mg_iobuf_del(r, 0, 6 + addr_len);  // Remove request from the input stream
  c->data[0] = STATE_ESTABLISHED;   // Mark ourselves as connected
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_READ) {
    // We use the first label byte as a state
    if (c->data[0] == STATE_HANDSHAKE) handshake(c);
    if (c->data[0] == STATE_REQUEST) request(c);
    if (c->data[0] == STATE_ESTABLISHED) exchange(c);
  } else if (ev == MG_EV_CLOSE) {
    disband(c);
  }
  (void) fn_data;
  (void) ev_data;
}

int main(void) {
  struct mg_mgr mgr;                     // Event manager
  mg_log_set(MG_LL_DEBUG);               // Set log level
  mg_mgr_init(&mgr);                     // Initialise event manager
  mg_listen(&mgr, s_lsn, fn, NULL);      // Create client connection
  while (true) mg_mgr_poll(&mgr, 1000);  // Infinite event loop
  mg_mgr_free(&mgr);                     // Free resources
  return 0;
}
