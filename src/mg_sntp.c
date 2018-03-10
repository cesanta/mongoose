/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#include "mg_internal.h"
#include "mg_sntp.h"
#include "mg_util.h"

#if MG_ENABLE_SNTP

#define SNTP_TIME_OFFSET 2208988800

#ifndef SNTP_TIMEOUT
#define SNTP_TIMEOUT 10
#endif

#ifndef SNTP_ATTEMPTS
#define SNTP_ATTEMPTS 3
#endif

static uint64_t mg_get_sec(uint64_t val) {
  return (val & 0xFFFFFFFF00000000) >> 32;
}

static uint64_t mg_get_usec(uint64_t val) {
  uint64_t tmp = (val & 0x00000000FFFFFFFF);
  tmp *= 1000000;
  tmp >>= 32;
  return tmp;
}

static void mg_ntp_to_tv(uint64_t val, struct timeval *tv) {
  uint64_t tmp;
  tmp = mg_get_sec(val);
  tmp -= SNTP_TIME_OFFSET;
  tv->tv_sec = tmp;
  tv->tv_usec = mg_get_usec(val);
}

static void mg_get_ntp_ts(const char *ntp, uint64_t *val) {
  uint32_t tmp;
  memcpy(&tmp, ntp, sizeof(tmp));
  tmp = ntohl(tmp);
  *val = (uint64_t) tmp << 32;
  memcpy(&tmp, ntp + 4, sizeof(tmp));
  tmp = ntohl(tmp);
  *val |= tmp;
}

void mg_sntp_send_request(struct mg_connection *c) {
  uint8_t buf[48] = {0};
  /*
   * header - 8 bit:
   * LI (2 bit) - 3 (not in sync), VN (3 bit) - 4 (version),
   * mode (3 bit) - 3 (client)
   */
  buf[0] = (3 << 6) | (4 << 3) | 3;

/*
 * Next fields should be empty in client request
 * stratum, 8 bit
 * poll interval, 8 bit
 * rrecision, 8 bit
 * root delay, 32 bit
 * root dispersion, 32 bit
 * ref id, 32 bit
 * ref timestamp, 64 bit
 * originate Timestamp, 64 bit
 * receive Timestamp, 64 bit
*/

/*
 * convert time to sntp format (sntp starts from 00:00:00 01.01.1900)
 * according to rfc868 it is 2208988800L sec
 * this information is used to correct roundtrip delay
 * but if local clock is absolutely broken (and doesn't work even
 * as simple timer), it is better to disable it
*/
#ifndef MG_SNTP_NO_DELAY_CORRECTION
  uint32_t sec;
  sec = htonl((uint32_t)(mg_time() + SNTP_TIME_OFFSET));
  memcpy(&buf[40], &sec, sizeof(sec));
#endif

  mg_send(c, buf, sizeof(buf));
}

#ifndef MG_SNTP_NO_DELAY_CORRECTION
static uint64_t mg_calculate_delay(uint64_t t1, uint64_t t2, uint64_t t3) {
  /* roundloop delay = (T4 - T1) - (T3 - T2) */
  uint64_t d1 = ((mg_time() + SNTP_TIME_OFFSET) * 1000000) -
                (mg_get_sec(t1) * 1000000 + mg_get_usec(t1));
  uint64_t d2 = (mg_get_sec(t3) * 1000000 + mg_get_usec(t3)) -
                (mg_get_sec(t2) * 1000000 + mg_get_usec(t2));

  return (d1 > d2) ? d1 - d2 : 0;
}
#endif

MG_INTERNAL int mg_sntp_parse_reply(const char *buf, int len,
                                    struct mg_sntp_message *msg) {
  uint8_t hdr;
  uint64_t trsm_ts_T3, delay = 0;
  int mode;
  struct timeval tv;

  if (len < 48) {
    return -1;
  }

  hdr = buf[0];

  if ((hdr & 0x38) >> 3 != 4) {
    /* Wrong version */
    return -1;
  }

  mode = hdr & 0x7;
  if (mode != 4 && mode != 5) {
    /* Not a server reply */
    return -1;
  }

  memset(msg, 0, sizeof(*msg));

  msg->kiss_of_death = (buf[1] == 0); /* Server asks to not send requests */

  mg_get_ntp_ts(&buf[40], &trsm_ts_T3);

#ifndef MG_SNTP_NO_DELAY_CORRECTION
  {
    uint64_t orig_ts_T1, recv_ts_T2;
    mg_get_ntp_ts(&buf[24], &orig_ts_T1);
    mg_get_ntp_ts(&buf[32], &recv_ts_T2);
    delay = mg_calculate_delay(orig_ts_T1, recv_ts_T2, trsm_ts_T3);
  }
#endif

  mg_ntp_to_tv(trsm_ts_T3, &tv);

  msg->time = (double) tv.tv_sec + (((double) tv.tv_usec + delay) / 1000000.0);

  return 0;
}

static void mg_sntp_handler(struct mg_connection *c, int ev,
                            void *ev_data MG_UD_ARG(void *user_data)) {
  struct mbuf *io = &c->recv_mbuf;
  struct mg_sntp_message msg;

  c->handler(c, ev, ev_data MG_UD_ARG(user_data));

  switch (ev) {
    case MG_EV_RECV: {
      if (mg_sntp_parse_reply(io->buf, io->len, &msg) < 0) {
        DBG(("Invalid SNTP packet received (%d)", (int) io->len));
        c->handler(c, MG_SNTP_MALFORMED_REPLY, NULL MG_UD_ARG(user_data));
      } else {
        c->handler(c, MG_SNTP_REPLY, (void *) &msg MG_UD_ARG(user_data));
      }

      mbuf_remove(io, io->len);
      break;
    }
  }
}

int mg_set_protocol_sntp(struct mg_connection *c) {
  if ((c->flags & MG_F_UDP) == 0) {
    return -1;
  }

  c->proto_handler = mg_sntp_handler;

  return 0;
}

struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr,
                                      MG_CB(mg_event_handler_t event_handler,
                                            void *user_data),
                                      const char *sntp_server_name) {
  struct mg_connection *c = NULL;
  char url[100], *p_url = url;
  const char *proto = "", *port = "", *tmp;

  /* If port is not specified, use default (123) */
  tmp = strchr(sntp_server_name, ':');
  if (tmp != NULL && *(tmp + 1) == '/') {
    tmp = strchr(tmp + 1, ':');
  }

  if (tmp == NULL) {
    port = ":123";
  }

  /* Add udp:// if needed */
  if (strncmp(sntp_server_name, "udp://", 6) != 0) {
    proto = "udp://";
  }

  mg_asprintf(&p_url, sizeof(url), "%s%s%s", proto, sntp_server_name, port);

  c = mg_connect(mgr, p_url, event_handler MG_UD_ARG(user_data));

  if (c == NULL) {
    goto cleanup;
  }

  mg_set_protocol_sntp(c);

cleanup:
  if (p_url != url) {
    MG_FREE(p_url);
  }

  return c;
}

struct sntp_data {
  mg_event_handler_t hander;
  int count;
};

static void mg_sntp_util_ev_handler(struct mg_connection *c, int ev,
                                    void *ev_data MG_UD_ARG(void *user_data)) {
#if !MG_ENABLE_CALLBACK_USERDATA
  void *user_data = c->user_data;
#endif
  struct sntp_data *sd = (struct sntp_data *) user_data;

  switch (ev) {
    case MG_EV_CONNECT:
      if (*(int *) ev_data != 0) {
        mg_call(c, sd->hander, c->user_data, MG_SNTP_FAILED, NULL);
        break;
      }
    /* fallthrough */
    case MG_EV_TIMER:
      if (sd->count <= SNTP_ATTEMPTS) {
        mg_sntp_send_request(c);
        mg_set_timer(c, mg_time() + 10);
        sd->count++;
      } else {
        mg_call(c, sd->hander, c->user_data, MG_SNTP_FAILED, NULL);
        c->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
      break;
    case MG_SNTP_MALFORMED_REPLY:
      mg_call(c, sd->hander, c->user_data, MG_SNTP_FAILED, NULL);
      c->flags |= MG_F_CLOSE_IMMEDIATELY;
      break;
    case MG_SNTP_REPLY:
      mg_call(c, sd->hander, c->user_data, MG_SNTP_REPLY, ev_data);
      c->flags |= MG_F_CLOSE_IMMEDIATELY;
      break;
    case MG_EV_CLOSE:
      MG_FREE(user_data);
      c->user_data = NULL;
      break;
  }
}

struct mg_connection *mg_sntp_get_time(struct mg_mgr *mgr,
                                       mg_event_handler_t event_handler,
                                       const char *sntp_server_name) {
  struct mg_connection *c;
  struct sntp_data *sd = (struct sntp_data *) MG_CALLOC(1, sizeof(*sd));
  if (sd == NULL) {
    return NULL;
  }

  c = mg_sntp_connect(mgr, MG_CB(mg_sntp_util_ev_handler, sd),
                      sntp_server_name);
  if (c == NULL) {
    MG_FREE(sd);
    return NULL;
  }

  sd->hander = event_handler;
#if !MG_ENABLE_CALLBACK_USERDATA
  c->user_data = sd;
#endif

  return c;
}

#endif /* MG_ENABLE_SNTP */
