#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_ATCMD) && MG_ENABLE_DRIVER_ATCMD

#define MG_HDLC_FLAG 0x7e  // HDLC frame delimiter
#define MG_HDLC_ESC 0x7d   // HDLC escape byte for byte stuffing

#define MG_ATCMD_AT_TIMEOUT 2000

static size_t print_atcmd(void (*out)(char, void *), void *arg, va_list *ap) {
  struct mg_str s = va_arg(*ap, struct mg_str);
  for (size_t i = 0; i < s.len; i++) out(s.buf[i] < 0x20 ? '.' : s.buf[i], arg);
  return s.len;
}

static void mg_atcmd_reset(struct mg_tcpip_driver_atcmd_data *dd) {
  dd->script_index = 0;
  dd->deadline = 0;
  if (dd->reset) dd->reset(dd->usart);
}

static bool mg_atcmd_handle(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_atcmd_data *dd = (struct mg_tcpip_driver_atcmd_data *) ifp->driver_data;
  if (dd->script == NULL || dd->script_index < 0) return true;
  if (dd->deadline == 0) dd->deadline = mg_millis() + MG_ATCMD_AT_TIMEOUT;
  for (;;) {
    if (dd->script_index % 2 == 0) {  // send AT command
      const char *cmd = dd->script[dd->script_index];
      MG_DEBUG(("send AT[%d]: %M", dd->script_index, print_atcmd, mg_str(cmd)));
      while (*cmd) dd->tx(dd->usart, *cmd++);
      dd->script_index++;
      ifp->recv_queue.head = 0;
    } else {  // check AT command response
      const char *expect = dd->script[dd->script_index];
      struct mg_queue *q = &ifp->recv_queue;
      for (;;) {
        int c;
        bool is_timeout = dd->deadline > 0 && mg_millis() > dd->deadline;
        bool is_overflow = q->head >= q->size - 1;
        if (is_timeout || is_overflow) {
          MG_ERROR(("AT error: %s, retrying...", is_timeout ? "timeout" : "overflow"));
          mg_atcmd_reset(dd);
          return false;  // FAIL
        }
        if ((c = dd->rx(dd->usart)) < 0) return false;  // no data
        q->buf[q->head++] = c;
        if (mg_match(mg_str_n(q->buf, q->head), mg_str(expect), NULL)) {
          MG_DEBUG(("recv AT[%d]: %M", dd->script_index, print_atcmd,
                    mg_str_n(q->buf, q->head)));
          dd->script_index++;
          q->head = 0;
          break;
        }
      }
    }
    if (dd->script[dd->script_index] == NULL) {
      MG_DEBUG(("finished AT script"));
      dd->script_index = -1;
      return true;
    }
  }
}

static void byte_stuff(struct mg_tcpip_driver_atcmd_data *dd, uint8_t b);
static bool byte_unstuff(struct mg_tcpip_driver_atcmd_data *dd, uint8_t *p);

static size_t mg_atcmd_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_atcmd_data *dd = (struct mg_tcpip_driver_atcmd_data *) ifp->driver_data;
  struct mg_queue *q = &ifp->recv_queue;
  
  if (!dd->link) return 0;

  while (q->head < q->size) { // read as many bytes as possible
    uint8_t b;
    int c = dd->rx(dd->usart);
    if (c < 0) return 0;      // no more bytes, exit
    b = (uint8_t) c;
    if (b == MG_HDLC_FLAG) {
      if (q->head == 0) { // first flag: skip
        dd->unstuffing = false;
        continue;
      } else { // last flag: end of frame, exit loop
        break;
      }
    }
    if (!dd->no_byte_stuff && !byte_unstuff(dd, &b)) continue;
    q->buf[q->head++] = b;
  }
  len = (q->head <= len) ? q->head : 0;
  memmove(buf, q->buf, len);
  q->head = 0;
  return len;
}

static size_t mg_atcmd_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_atcmd_data *dd = (struct mg_tcpip_driver_atcmd_data *) ifp->driver_data;
  uint8_t *p = (uint8_t *) buf;
  size_t n = len;

  if (!dd->link) return 0;

  dd->tx(dd->usart, MG_HDLC_FLAG);
  while (n--) {
    if (dd->no_byte_stuff) {
      dd->tx(dd->usart, *p++);
    } else {
      byte_stuff(dd, *p++);
    }
  }
  dd->tx(dd->usart, MG_HDLC_FLAG);
  return len;
}

static bool mg_atcmd_init(struct mg_tcpip_if *ifp) {
  ifp->recv_queue.size = 1506;  // PPP MTU=1500 + header + HDLC w/ no flags
  return true;
}

static bool mg_atcmd_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct mg_tcpip_driver_atcmd_data *dd = (struct mg_tcpip_driver_atcmd_data *) ifp->driver_data;
  dd->link = mg_atcmd_handle(ifp);
  return s1 ? dd->link :false;
}


struct mg_tcpip_driver mg_tcpip_driver_atcmd = {mg_atcmd_init, mg_atcmd_tx, mg_atcmd_rx, mg_atcmd_poll};



static void byte_stuff(struct mg_tcpip_driver_atcmd_data *dd, uint8_t b) {
  if ((b < 0x20) || (b == MG_HDLC_ESC) || (b == MG_HDLC_FLAG)) {
    dd->tx(dd->usart, MG_HDLC_ESC);
    dd->tx(dd->usart, b ^ 0x20);
  } else {
    dd->tx(dd->usart, b);
  }
}

static bool byte_unstuff(struct mg_tcpip_driver_atcmd_data *dd, uint8_t *p) {
  if (!dd->unstuffing) {
    if (*p == MG_HDLC_ESC) {
      dd->unstuffing = true;
      return false;
    } else {
      return true;
    }
  }
  dd->unstuffing = false;
  *p ^= 0x20;
  return true;
}

#endif
