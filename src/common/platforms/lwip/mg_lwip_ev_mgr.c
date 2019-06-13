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
    int i = md->start_index;
    int sig = md->sig_queue[i].sig;
    struct mg_connection *nc = md->sig_queue[i].nc;
    struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
    md->start_index = (i + 1) % MG_SIG_QUEUE_LEN;
    md->sig_queue_len--;
    mgos_unlock();
    if (nc->iface == NULL || nc->mgr == NULL) continue;
    switch (sig) {
      case MG_SIG_CONNECT_RESULT: {
        mg_if_connect_cb(nc, cs->err);
        break;
      }
      case MG_SIG_CLOSE_CONN: {
        mg_close_conn(nc);
        break;
      }
      case MG_SIG_RECV: {
        cs->recv_pending = 0;
        mg_if_can_recv_cb(nc);
        mbuf_trim(&nc->recv_mbuf);
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
  LOG(LL_INFO, ("Mongoose %s, LwIP %u.%u.%u", MG_VERSION, LWIP_VERSION_MAJOR,
                LWIP_VERSION_MINOR, LWIP_VERSION_REVISION));
  iface->data = MG_CALLOC(1, sizeof(struct mg_ev_mgr_lwip_data));
#if !NO_SYS && !LWIP_TCPIP_CORE_LOCKING
  sys_sem_new(&s_tcpip_call_lock_sem, 1);
  sys_sem_new(&s_tcpip_call_sync_sem, 0);
#endif
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
    if (!mg_if_poll(nc, now)) continue;
    if (nc->sock != INVALID_SOCKET &&
        !(nc->flags & (MG_F_UDP | MG_F_LISTENING)) && cs->pcb.tcp != NULL &&
        cs->pcb.tcp->unsent != NULL) {
      mg_lwip_netif_run_on_tcpip(tcp_output_tcpip, cs->pcb.tcp);
    }
    if (nc->ev_timer_time > 0) {
      if (num_timers == 0 || nc->ev_timer_time < min_timer) {
        min_timer = nc->ev_timer_time;
      }
      num_timers++;
    }

    if (nc->sock != INVALID_SOCKET) {
      if (mg_lwip_if_can_send(nc, cs)) {
        mg_if_can_send_cb(nc);
        mbuf_trim(&nc->send_mbuf);
      }
      if (cs->rx_chain != NULL) {
        mg_if_can_recv_cb(nc);
      } else if (cs->draining_rx_chain) {
        /*
         * If the connection is about to close, and rx_chain is finally empty,
         * send the MG_SIG_CLOSE_CONN signal
         */
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

#endif /* MG_NET_IF == MG_NET_IF_LWIP_LOW_LEVEL */
