/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if MG_ENABLE_NET_IF_PIC32

int mg_pic32_if_create_conn(struct mg_connection *nc) {
  (void) nc;
  return 1;
}

void mg_pic32_if_recved(struct mg_connection *nc, size_t len) {
  (void) nc;
  (void) len;
}

void mg_pic32_if_add_conn(struct mg_connection *nc) {
  (void) nc;
}

void mg_pic32_if_init(struct mg_iface *iface) {
  (void) iface;
  (void) mg_get_errno(); /* Shutup compiler */
}

void mg_pic32_if_free(struct mg_iface *iface) {
  (void) iface;
}

void mg_pic32_if_remove_conn(struct mg_connection *nc) {
  (void) nc;
}

void mg_pic32_if_destroy_conn(struct mg_connection *nc) {
  if (nc->sock == INVALID_SOCKET) return;
  /* For UDP, only close outgoing sockets or listeners. */
  if (!(nc->flags & MG_F_UDP)) {
    /* Close TCP */
    TCPIP_TCP_Close((TCP_SOCKET) nc->sock);
  } else if (nc->listener == NULL) {
    /* Only close outgoing UDP or listeners. */
    TCPIP_UDP_Close((UDP_SOCKET) nc->sock);
  }

  nc->sock = INVALID_SOCKET;
}

int mg_pic32_if_listen_udp(struct mg_connection *nc, union socket_address *sa) {
  nc->sock = TCPIP_UDP_ServerOpen(
      sa->sin.sin_family == AF_INET ? IP_ADDRESS_TYPE_IPV4
                                    : IP_ADDRESS_TYPE_IPV6,
      ntohs(sa->sin.sin_port),
      sa->sin.sin_addr.s_addr == 0 ? 0 : (IP_MULTI_ADDRESS *) &sa->sin);
  if (nc->sock == INVALID_SOCKET) {
    return -1;
  }
  return 0;
}

void mg_pic32_if_udp_send(struct mg_connection *nc, const void *buf,
                          size_t len) {
  mbuf_append(&nc->send_mbuf, buf, len);
}

void mg_pic32_if_tcp_send(struct mg_connection *nc, const void *buf,
                          size_t len) {
  mbuf_append(&nc->send_mbuf, buf, len);
}

int mg_pic32_if_listen_tcp(struct mg_connection *nc, union socket_address *sa) {
  nc->sock = TCPIP_TCP_ServerOpen(
      sa->sin.sin_family == AF_INET ? IP_ADDRESS_TYPE_IPV4
                                    : IP_ADDRESS_TYPE_IPV6,
      ntohs(sa->sin.sin_port),
      sa->sin.sin_addr.s_addr == 0 ? 0 : (IP_MULTI_ADDRESS *) &sa->sin);
  memcpy(&nc->sa, sa, sizeof(*sa));
  if (nc->sock == INVALID_SOCKET) {
    return -1;
  }
  return 0;
}

static int mg_accept_conn(struct mg_connection *lc) {
  struct mg_connection *nc;
  TCP_SOCKET_INFO si;
  union socket_address sa;

  nc = mg_if_accept_new_conn(lc);

  if (nc == NULL) {
    return 0;
  }

  nc->sock = lc->sock;
  nc->flags &= ~MG_F_LISTENING;

  if (!TCPIP_TCP_SocketInfoGet((TCP_SOCKET) nc->sock, &si)) {
    return 0;
  }

  if (si.addressType == IP_ADDRESS_TYPE_IPV4) {
    sa.sin.sin_family = AF_INET;
    sa.sin.sin_port = htons(si.remotePort);
    sa.sin.sin_addr.s_addr = si.remoteIPaddress.v4Add.Val;
  } else {
    /* TODO(alashkin): do something with _potential_ IPv6 */
    memset(&sa, 0, sizeof(sa));
  }

  mg_if_accept_tcp_cb(nc, (union socket_address *) &sa, sizeof(sa));

  return mg_pic32_if_listen_tcp(lc, &lc->sa) >= 0;
}

char *inet_ntoa(struct in_addr in) {
  static char addr[17];
  snprintf(addr, sizeof(addr), "%d.%d.%d.%d", (int) in.S_un.S_un_b.s_b1,
           (int) in.S_un.S_un_b.s_b2, (int) in.S_un.S_un_b.s_b3,
           (int) in.S_un.S_un_b.s_b4);
  return addr;
}

static void mg_handle_send(struct mg_connection *nc) {
  uint16_t bytes_written = 0;
  if (nc->flags & MG_F_UDP) {
    if (!TCPIP_UDP_RemoteBind(
            (UDP_SOCKET) nc->sock,
            nc->sa.sin.sin_family == AF_INET ? IP_ADDRESS_TYPE_IPV4
                                             : IP_ADDRESS_TYPE_IPV6,
            ntohs(nc->sa.sin.sin_port), (IP_MULTI_ADDRESS *) &nc->sa.sin)) {
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      return;
    }
    bytes_written = TCPIP_UDP_TxPutIsReady((UDP_SOCKET) nc->sock, 0);
    if (bytes_written >= nc->send_mbuf.len) {
      if (TCPIP_UDP_ArrayPut((UDP_SOCKET) nc->sock,
                             (uint8_t *) nc->send_mbuf.buf,
                             nc->send_mbuf.len) != nc->send_mbuf.len) {
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        bytes_written = 0;
      }
    }
  } else {
    bytes_written = TCPIP_TCP_FifoTxFreeGet((TCP_SOCKET) nc->sock);
    if (bytes_written != 0) {
      if (bytes_written > nc->send_mbuf.len) {
        bytes_written = nc->send_mbuf.len;
      }
      if (TCPIP_TCP_ArrayPut((TCP_SOCKET) nc->sock,
                             (uint8_t *) nc->send_mbuf.buf,
                             bytes_written) != bytes_written) {
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        bytes_written = 0;
      }
    }
  }

  mg_if_sent_cb(nc, bytes_written);
}

static void mg_handle_recv(struct mg_connection *nc) {
  uint16_t bytes_read = 0;
  uint8_t *buf = NULL;
  if (nc->flags & MG_F_UDP) {
    bytes_read = TCPIP_UDP_GetIsReady((UDP_SOCKET) nc->sock);
    if (bytes_read != 0 &&
        (nc->recv_mbuf_limit == -1 ||
         nc->recv_mbuf.len + bytes_read < nc->recv_mbuf_limit)) {
      buf = (uint8_t *) MG_MALLOC(bytes_read);
      if (TCPIP_UDP_ArrayGet((UDP_SOCKET) nc->sock, buf, bytes_read) !=
          bytes_read) {
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        bytes_read = 0;
        MG_FREE(buf);
      }
    }
  } else {
    bytes_read = TCPIP_TCP_GetIsReady((TCP_SOCKET) nc->sock);
    if (bytes_read != 0) {
      if (nc->recv_mbuf_limit != -1 &&
          nc->recv_mbuf_limit - nc->recv_mbuf.len > bytes_read) {
        bytes_read = nc->recv_mbuf_limit - nc->recv_mbuf.len;
      }
      buf = (uint8_t *) MG_MALLOC(bytes_read);
      if (TCPIP_TCP_ArrayGet((TCP_SOCKET) nc->sock, buf, bytes_read) !=
          bytes_read) {
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        MG_FREE(buf);
        bytes_read = 0;
      }
    }
  }

  if (bytes_read != 0) {
    mg_if_recv_tcp_cb(nc, buf, bytes_read, 1 /* own */);
  }
}

time_t mg_pic32_if_poll(struct mg_iface *iface, int timeout_ms) {
  struct mg_mgr *mgr = iface->mgr;
  double now = mg_time();
  struct mg_connection *nc, *tmp;

  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    tmp = nc->next;

    if (nc->flags & MG_F_CONNECTING) {
      /* processing connections */
      if (nc->flags & MG_F_UDP ||
          TCPIP_TCP_IsConnected((TCP_SOCKET) nc->sock)) {
        mg_if_connect_cb(nc, 0);
      }
    } else if (nc->flags & MG_F_LISTENING) {
      if (TCPIP_TCP_IsConnected((TCP_SOCKET) nc->sock)) {
        /* accept new connections */
        mg_accept_conn(nc);
      }
    } else {
      if (nc->send_mbuf.len != 0) {
        mg_handle_send(nc);
      }

      if (nc->recv_mbuf_limit == -1 ||
          nc->recv_mbuf.len < nc->recv_mbuf_limit) {
        mg_handle_recv(nc);
      }
    }
  }

  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    tmp = nc->next;
    if ((nc->flags & MG_F_CLOSE_IMMEDIATELY) ||
        (nc->send_mbuf.len == 0 && (nc->flags & MG_F_SEND_AND_CLOSE))) {
      mg_close_conn(nc);
    }
  }

  return now;
}

void mg_pic32_if_sock_set(struct mg_connection *nc, sock_t sock) {
  nc->sock = sock;
}

void mg_pic32_if_get_conn_addr(struct mg_connection *nc, int remote,
                               union socket_address *sa) {
  /* TODO(alaskin): not implemented yet */
}

void mg_pic32_if_connect_tcp(struct mg_connection *nc,
                             const union socket_address *sa) {
  nc->sock = TCPIP_TCP_ClientOpen(
      sa->sin.sin_family == AF_INET ? IP_ADDRESS_TYPE_IPV4
                                    : IP_ADDRESS_TYPE_IPV6,
      ntohs(sa->sin.sin_port), (IP_MULTI_ADDRESS *) &sa->sin);
  nc->err = (nc->sock == INVALID_SOCKET) ? -1 : 0;
}

void mg_pic32_if_connect_udp(struct mg_connection *nc) {
  nc->sock = TCPIP_UDP_ClientOpen(IP_ADDRESS_TYPE_ANY, 0, NULL);
  nc->err = (nc->sock == INVALID_SOCKET) ? -1 : 0;
}

/* clang-format off */
#define MG_PIC32_IFACE_VTABLE                                   \
  {                                                             \
    mg_pic32_if_init,                                           \
    mg_pic32_if_free,                                           \
    mg_pic32_if_add_conn,                                       \
    mg_pic32_if_remove_conn,                                    \
    mg_pic32_if_poll,                                           \
    mg_pic32_if_listen_tcp,                                     \
    mg_pic32_if_listen_udp,                                     \
    mg_pic32_if_connect_tcp,                                    \
    mg_pic32_if_connect_udp,                                    \
    mg_pic32_if_tcp_send,                                       \
    mg_pic32_if_udp_send,                                       \
    mg_pic32_if_recved,                                         \
    mg_pic32_if_create_conn,                                    \
    mg_pic32_if_destroy_conn,                                   \
    mg_pic32_if_sock_set,                                       \
    mg_pic32_if_get_conn_addr,                                  \
  }
/* clang-format on */

const struct mg_iface_vtable mg_pic32_iface_vtable = MG_PIC32_IFACE_VTABLE;
#if MG_NET_IF == MG_NET_IF_PIC32
const struct mg_iface_vtable mg_default_iface_vtable = MG_PIC32_IFACE_VTABLE;
#endif

#endif /* MG_ENABLE_NET_IF_PIC32 */
