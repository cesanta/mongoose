/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_NET_IF == MG_NET_IF_LWIP_LOW_LEVEL

#ifndef MG_SIG_QUEUE_LEN
#define MG_SIG_QUEUE_LEN 32
#endif

struct mg_ev_mgr_lwip_signal {
  int sig;
  struct mg_connection *nc;
};

struct mg_ev_mgr_lwip_data {
  struct mg_ev_mgr_lwip_signal sig_queue[MG_SIG_QUEUE_LEN];
  int sig_queue_len;
  int start_index;
};

void mg_lwip_post_signal(enum mg_sig_type sig, struct mg_connection *nc) {
  struct mg_ev_mgr_lwip_data *md =
      (struct mg_ev_mgr_lwip_data *) nc->iface->data;
  mgos_lock();
  if (md->sig_queue_len >= MG_SIG_QUEUE_LEN) {
    mgos_unlock();
    return;
  }
  int end_index = (md->start_index + md->sig_queue_len) % MG_SIG_QUEUE_LEN;
  md->sig_queue[end_index].sig = sig;
  md->sig_queue[end_index].nc = nc;
  md->sig_queue_len++;
  mg_lwip_mgr_schedule_poll(nc->mgr);
  mgos_unlock();
}

void mg_ev_mgr_lwip_process_signals(struct mg_mgr *mgr) {
  struct mg_ev_mgr_lwip_data *md =
      (struct mg_ev_mgr_lwip_data *) mgr->ifaces[MG_MAIN_IFACE]->data;
  while (md->sig_queue_len > 0) {
    mgos_lock();
    int sig = md->sig_queue[md->start_index].sig;
    struct mg_connection *nc = md->sig_queue[md->start_index].nc;
    struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
    md->start_index = (md->start_index + 1) % MG_SIG_QUEUE_LEN;
    md->sig_queue_len--;
    mgos_unlock();
    if (nc->iface == NULL || nc->mgr == NULL) continue;
    switch (sig) {
      case MG_SIG_CONNECT_RESULT: {
#if MG_ENABLE_SSL
        if (cs->err == 0 && (nc->flags & MG_F_SSL) &&
            !(nc->flags & MG_F_SSL_HANDSHAKE_DONE)) {
          mg_lwip_ssl_do_hs(nc);
        } else
#endif
        {
          mg_if_connect_cb(nc, cs->err);
        }
        break;
      }
      case MG_SIG_CLOSE_CONN: {
        nc->flags |= MG_F_SEND_AND_CLOSE;
        mg_close_conn(nc);
        break;
      }
      case MG_SIG_RECV: {
        cs->recv_pending = 0;
        if (nc->flags & MG_F_UDP) {
          mg_lwip_handle_recv_udp(nc);
        } else {
          mg_lwip_handle_recv_tcp(nc);
        }
        break;
      }
      case MG_SIG_TOMBSTONE: {
        break;
      }
      case MG_SIG_ACCEPT: {
        mg_lwip_handle_accept(nc);
        break;
      }
    }
  }
}

void mg_lwip_if_init(struct mg_iface *iface) {
  LOG(LL_INFO, ("%p Mongoose init", iface));
  iface->data = MG_CALLOC(1, sizeof(struct mg_ev_mgr_lwip_data));
}

void mg_lwip_if_free(struct mg_iface *iface) {
  MG_FREE(iface->data);
  iface->data = NULL;
}

void mg_lwip_if_add_conn(struct mg_connection *nc) {
  (void) nc;
}

void mg_lwip_if_remove_conn(struct mg_connection *nc) {
  struct mg_ev_mgr_lwip_data *md =
      (struct mg_ev_mgr_lwip_data *) nc->iface->data;
  /* Walk the queue and null-out further signals for this conn. */
  for (int i = 0; i < MG_SIG_QUEUE_LEN; i++) {
    if (md->sig_queue[i].nc == nc) {
      md->sig_queue[i].sig = MG_SIG_TOMBSTONE;
    }
  }
}

time_t mg_lwip_if_poll(struct mg_iface *iface, int timeout_ms) {
  struct mg_mgr *mgr = iface->mgr;
  int n = 0;
  double now = mg_time();
  struct mg_connection *nc, *tmp;
  double min_timer = 0;
  int num_timers = 0;
#if 0
  DBG(("begin poll @%u", (unsigned int) (now * 1000)));
#endif
  mg_ev_mgr_lwip_process_signals(mgr);
  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
    tmp = nc->next;
    n++;
    if ((nc->flags & MG_F_CLOSE_IMMEDIATELY) ||
        ((nc->flags & MG_F_SEND_AND_CLOSE) && (nc->flags & MG_F_UDP) &&
         (nc->send_mbuf.len == 0))) {
      mg_close_conn(nc);
      continue;
    }
    mg_if_poll(nc, now);
    mg_if_timer(nc, now);
#if MG_ENABLE_SSL
    if ((nc->flags & MG_F_SSL) && cs != NULL && cs->pcb.tcp != NULL &&
        cs->pcb.tcp->state == ESTABLISHED) {
      if (((nc->flags & MG_F_WANT_WRITE) ||
           ((nc->send_mbuf.len > 0) &&
            (nc->flags & MG_F_SSL_HANDSHAKE_DONE))) &&
          cs->pcb.tcp->snd_buf > 0) {
        /* Can write more. */
        if (nc->flags & MG_F_SSL_HANDSHAKE_DONE) {
          if (!(nc->flags & MG_F_CONNECTING)) mg_lwip_ssl_send(nc);
        } else {
          mg_lwip_ssl_do_hs(nc);
        }
      }
      if (cs->rx_chain != NULL || (nc->flags & MG_F_WANT_READ)) {
        if (nc->flags & MG_F_SSL_HANDSHAKE_DONE) {
          if (!(nc->flags & MG_F_CONNECTING)) mg_lwip_ssl_recv(nc);
        } else {
          mg_lwip_ssl_do_hs(nc);
        }
      }
    } else
#endif /* MG_ENABLE_SSL */
    {
      if (nc->send_mbuf.len > 0 && !(nc->flags & MG_F_CONNECTING)) {
        mg_lwip_send_more(nc);
      }
    }
    if (nc->sock != INVALID_SOCKET &&
        !(nc->flags & (MG_F_UDP | MG_F_LISTENING)) && cs->pcb.tcp != NULL &&
        cs->pcb.tcp->unsent != NULL) {
      tcpip_callback(tcp_output_tcpip, cs->pcb.tcp);
    }
    if (nc->ev_timer_time > 0) {
      if (num_timers == 0 || nc->ev_timer_time < min_timer) {
        min_timer = nc->ev_timer_time;
      }
      num_timers++;
    }

    if (nc->sock != INVALID_SOCKET) {
      /* Try to consume data from cs->rx_chain */
      mg_lwip_consume_rx_chain_tcp(nc);

      /*
       * If the connection is about to close, and rx_chain is finally empty,
       * send the MG_SIG_CLOSE_CONN signal
       */
      if (cs->draining_rx_chain && cs->rx_chain == NULL) {
        mg_lwip_post_signal(MG_SIG_CLOSE_CONN, nc);
      }
    }
  }
#if 0
  DBG(("end poll @%u, %d conns, %d timers (min %u), next in %d ms",
       (unsigned int) (now * 1000), n, num_timers,
       (unsigned int) (min_timer * 1000), timeout_ms));
#endif
  (void) timeout_ms;
  return now;
}

uint32_t mg_lwip_get_poll_delay_ms(struct mg_mgr *mgr) {
  struct mg_connection *nc;
  double now;
  double min_timer = 0;
  int num_timers = 0;
  mg_ev_mgr_lwip_process_signals(mgr);
  for (nc = mg_next(mgr, NULL); nc != NULL; nc = mg_next(mgr, nc)) {
    struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
    if (nc->ev_timer_time > 0) {
      if (num_timers == 0 || nc->ev_timer_time < min_timer) {
        min_timer = nc->ev_timer_time;
      }
      num_timers++;
    }
    if (nc->send_mbuf.len > 0
#if MG_ENABLE_SSL
        || (nc->flags & MG_F_WANT_WRITE)
#endif
            ) {
      int can_send = 0;
      /* We have stuff to send, but can we? */
      if (nc->flags & MG_F_UDP) {
        /* UDP is always ready for sending. */
        can_send = (cs->pcb.udp != NULL);
      } else {
        can_send = (cs->pcb.tcp != NULL && cs->pcb.tcp->snd_buf > 0);
      }
      /* We want and can send, request a poll immediately. */
      if (can_send) return 0;
    }
  }
  uint32_t timeout_ms = ~0;
  now = mg_time();
  if (num_timers > 0) {
    /* If we have a timer that is past due, do a poll ASAP. */
    if (min_timer < now) return 0;
    double timer_timeout_ms = (min_timer - now) * 1000 + 1 /* rounding */;
    if (timer_timeout_ms < timeout_ms) {
      timeout_ms = timer_timeout_ms;
    }
  }
  return timeout_ms;
}

#endif /* MG_NET_IF == MG_NET_IF_LWIP_LOW_LEVEL */
