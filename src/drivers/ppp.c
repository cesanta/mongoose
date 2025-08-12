#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_PPP) && MG_ENABLE_DRIVER_PPP

#define MG_PPP_FLAG 0x7e  // PPP frame delimiter
#define MG_PPP_ESC 0x7d   // PPP escape byte for special characters
#define MG_PPP_ADDR 0xff
#define MG_PPP_CTRL 0x03

#define MG_PPP_PROTO_IP 0x0021
#define MG_PPP_PROTO_LCP 0xc021
#define MG_PPP_PROTO_IPCP 0x8021

#define MG_PPP_IPCP_REQ 1
#define MG_PPP_IPCP_ACK 2
#define MG_PPP_IPCP_NACK 3
#define MG_PPP_IPCP_IPADDR 3

#define MG_PPP_LCP_CFG_REQ 1
#define MG_PPP_LCP_CFG_ACK 2
#define MG_PPP_LCP_CFG_NACK 3
#define MG_PPP_LCP_CFG_REJECT 4
#define MG_PPP_LCP_CFG_TERM_REQ 5
#define MG_PPP_LCP_CFG_TERM_ACK 6

#define MG_PPP_AT_TIMEOUT 2000

static size_t print_atcmd(void (*out)(char, void *), void *arg, va_list *ap) {
  struct mg_str s = va_arg(*ap, struct mg_str);
  for (size_t i = 0; i < s.len; i++) out(s.buf[i] < 0x20 ? '.' : s.buf[i], arg);
  return s.len;
}

static void mg_ppp_reset(struct mg_tcpip_driver_ppp_data *dd) {
  dd->script_index = 0;
  dd->deadline = 0;
  if (dd->reset) dd->reset(dd->uart);
}

static bool mg_ppp_atcmd_handle(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_ppp_data *dd =
      (struct mg_tcpip_driver_ppp_data *) ifp->driver_data;
  if (dd->script == NULL || dd->script_index < 0) return true;
  if (dd->deadline == 0) dd->deadline = mg_millis() + MG_PPP_AT_TIMEOUT;
  for (;;) {
    if (dd->script_index % 2 == 0) {  // send AT command
      const char *cmd = dd->script[dd->script_index];
      MG_DEBUG(("send AT[%d]: %M", dd->script_index, print_atcmd, mg_str(cmd)));
      while (*cmd) dd->tx(dd->uart, *cmd++);
      dd->script_index++;
      ifp->recv_queue.head = 0;
    } else {  // check AT command response
      const char *expect = dd->script[dd->script_index];
      struct mg_queue *q = &ifp->recv_queue;
      for (;;) {
        int c;
        int is_timeout = dd->deadline > 0 && mg_millis() > dd->deadline;
        int is_overflow = q->head >= q->size - 1;
        if (is_timeout || is_overflow) {
          MG_ERROR(("AT error: %s, retrying...",
                    is_timeout ? "timeout" : "overflow"));
          mg_ppp_reset(dd);
          return false;  // FAIL: timeout
        }
        if ((c = dd->rx(dd->uart)) < 0) return false;  // no data
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

static bool mg_ppp_init(struct mg_tcpip_if *ifp) {
  ifp->recv_queue.size = 3000;  // MTU=1500, worst case escaping = 2x
  return true;
}

// Calculate FCS/CRC for PPP frames. Could be implemented faster using lookup
// tables.
static uint32_t fcs_do(uint32_t fcs, uint8_t x) {
  for (int i = 0; i < 8; i++) {
    fcs = ((fcs ^ x) & 1) ? (fcs >> 1) ^ 0x8408 : fcs >> 1;
    x >>= 1;
  }
  return fcs;
}

static bool mg_ppp_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) s1;
  return ifp->driver_data != NULL;
}

// Transmit a single byte as part of the PPP frame (escaped, if needed)
static void mg_ppp_tx_byte(struct mg_tcpip_driver_ppp_data *dd, uint8_t b) {
  if ((b < 0x20) || (b == MG_PPP_ESC) || (b == MG_PPP_FLAG)) {
    dd->tx(dd->uart, MG_PPP_ESC);
    dd->tx(dd->uart, b ^ 0x20);
  } else {
    dd->tx(dd->uart, b);
  }
}

// Transmit a single PPP frame for the given protocol
static void mg_ppp_tx_frame(struct mg_tcpip_driver_ppp_data *dd, uint16_t proto,
                            uint8_t *data, size_t datasz) {
  uint16_t crc;
  uint32_t fcs = 0xffff;

  dd->tx(dd->uart, MG_PPP_FLAG);
  mg_ppp_tx_byte(dd, MG_PPP_ADDR);
  mg_ppp_tx_byte(dd, MG_PPP_CTRL);
  mg_ppp_tx_byte(dd, proto >> 8);
  mg_ppp_tx_byte(dd, proto & 0xff);
  fcs = fcs_do(fcs, MG_PPP_ADDR);
  fcs = fcs_do(fcs, MG_PPP_CTRL);
  fcs = fcs_do(fcs, proto >> 8);
  fcs = fcs_do(fcs, proto & 0xff);
  for (unsigned int i = 0; i < datasz; i++) {
    mg_ppp_tx_byte(dd, data[i]);
    fcs = fcs_do(fcs, data[i]);
  }
  crc = fcs & 0xffff;
  mg_ppp_tx_byte(dd, ~crc);  // send CRC, note the byte order
  mg_ppp_tx_byte(dd, ~crc >> 8);
  dd->tx(dd->uart, MG_PPP_FLAG);  // end of frame
}

// Send Ethernet frame as PPP frame
static size_t mg_ppp_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_ppp_data *dd =
      (struct mg_tcpip_driver_ppp_data *) ifp->driver_data;
  if (ifp->state != MG_TCPIP_STATE_READY) return 0;
  // XXX: what if not an IP protocol?
  mg_ppp_tx_frame(dd, MG_PPP_PROTO_IP, (uint8_t *) buf + 14, len - 14);
  return len;
}

// Given a full PPP frame, unescape it in place and verify FCS, returns actual
// data size on success or 0 on error.
static size_t mg_ppp_verify_frame(uint8_t *buf, size_t bufsz) {
  int unpack = 0;
  uint16_t crc;
  size_t pktsz = 0;
  uint32_t fcs = 0xffff;
  for (unsigned int i = 0; i < bufsz; i++) {
    if (unpack == 0) {
      if (buf[i] == 0x7d) {
        unpack = 1;
      } else {
        buf[pktsz] = buf[i];
        fcs = fcs_do(fcs, buf[pktsz]);
        pktsz++;
      }
    } else {
      unpack = 0;
      buf[pktsz] = buf[i] ^ 0x20;
      fcs = fcs_do(fcs, buf[pktsz]);
      pktsz++;
    }
  }
  crc = fcs & 0xffff;
  if (crc != 0xf0b8) {
    MG_DEBUG(("bad crc: %04x", crc));
    return 0;
  }
  if (pktsz < 6 || buf[0] != MG_PPP_ADDR || buf[1] != MG_PPP_CTRL) {
    return 0;
  }
  return pktsz - 2;  // strip FCS
}

// fetch as much data as we can, until a single PPP frame is received
static size_t mg_ppp_rx_frame(struct mg_tcpip_driver_ppp_data *dd,
                              struct mg_queue *q) {
  while (q->head < q->size) {
    int c;
    if ((c = dd->rx(dd->uart)) < 0) {
      return 0;
    }
    if (c == MG_PPP_FLAG) {
      if (q->head > 0) {
        break;
      } else {
        continue;
      }
    }
    q->buf[q->head++] = c;
  }

  size_t n = mg_ppp_verify_frame((uint8_t *) q->buf, q->head);
  if (n == 0) {
    MG_DEBUG(("invalid PPP frame of %d bytes", q->head));
    q->head = 0;
    return 0;
  }
  q->head = n;
  return q->head;
}

static void mg_ppp_handle_lcp(struct mg_tcpip_if *ifp, uint8_t *lcp,
                              size_t lcpsz) {
  uint8_t id;
  uint16_t len;
  struct mg_tcpip_driver_ppp_data *dd =
      (struct mg_tcpip_driver_ppp_data *) ifp->driver_data;
  if (lcpsz < 4) return;
  id = lcp[1];
  len = (((uint16_t) lcp[2]) << 8) | (lcp[3]);
  switch (lcp[0]) {
    case MG_PPP_LCP_CFG_REQ: {
      if (len == 4) {
        MG_DEBUG(("LCP config request of %d bytes, acknowledging...", len));
        lcp[0] = MG_PPP_LCP_CFG_ACK;
        mg_ppp_tx_frame(dd, MG_PPP_PROTO_LCP, lcp, len);
        lcp[0] = MG_PPP_LCP_CFG_REQ;
        mg_ppp_tx_frame(dd, MG_PPP_PROTO_LCP, lcp, len);
      } else {
        MG_DEBUG(("LCP config request of %d bytes, rejecting...", len));
        lcp[0] = MG_PPP_LCP_CFG_REJECT;
        mg_ppp_tx_frame(dd, MG_PPP_PROTO_LCP, lcp, len);
      }
    } break;
    case MG_PPP_LCP_CFG_TERM_REQ: {
      uint8_t ack[4] = {MG_PPP_LCP_CFG_TERM_ACK, id, 0, 4};
      MG_DEBUG(("LCP termination request, acknowledging..."));
      mg_ppp_tx_frame(dd, MG_PPP_PROTO_LCP, ack, sizeof(ack));
      mg_ppp_reset(dd);
      ifp->state = MG_TCPIP_STATE_UP;
      if (dd->reset) dd->reset(dd->uart);
    } break;
  }
}

static void mg_ppp_handle_ipcp(struct mg_tcpip_if *ifp, uint8_t *ipcp,
                               size_t ipcpsz) {
  struct mg_tcpip_driver_ppp_data *dd =
      (struct mg_tcpip_driver_ppp_data *) ifp->driver_data;
  uint16_t len;
  uint8_t id;
  uint8_t req[] = {
      MG_PPP_IPCP_REQ, 0, 0, 10, MG_PPP_IPCP_IPADDR, 6, 0, 0, 0, 0};
  if (ipcpsz < 4) return;
  id = ipcp[1];
  len = (((uint16_t) ipcp[2]) << 8) | (ipcp[3]);
  switch (ipcp[0]) {
    case MG_PPP_IPCP_REQ:
      MG_DEBUG(("got IPCP config request, acknowledging..."));
      if (len >= 10 && ipcp[4] == MG_PPP_IPCP_IPADDR) {
        uint8_t *ip = ipcp + 6;
        MG_DEBUG(("host ip: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]));
      }
      ipcp[0] = MG_PPP_IPCP_ACK;
      mg_ppp_tx_frame(dd, MG_PPP_PROTO_IPCP, ipcp, len);
      req[1] = id;
      // Request IP address 0.0.0.0
      mg_ppp_tx_frame(dd, MG_PPP_PROTO_IPCP, req, sizeof(req));
      break;
    case MG_PPP_IPCP_ACK:
      // This usually does not happen, as our "preferred" IP address is invalid
      MG_DEBUG(("got IPCP config ack, link is online now"));
      ifp->state = MG_TCPIP_STATE_READY;
      break;
    case MG_PPP_IPCP_NACK:
      MG_DEBUG(("got IPCP config nack"));
      // NACK contains our "suggested" IP address, use it
      if (len >= 10 && ipcp[4] == MG_PPP_IPCP_IPADDR) {
        uint8_t *ip = ipcp + 6;
        MG_DEBUG(("ipcp ack, ip: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]));
        ipcp[0] = MG_PPP_IPCP_REQ;
        mg_ppp_tx_frame(dd, MG_PPP_PROTO_IPCP, ipcp, len);
        ifp->ip = ifp->mask = MG_IPV4(ip[0], ip[1], ip[2], ip[3]);
        ifp->state = MG_TCPIP_STATE_READY;
      }
      break;
  }
}

static size_t mg_ppp_rx(void *ethbuf, size_t ethlen, struct mg_tcpip_if *ifp) {
  uint8_t *eth = ethbuf;
  size_t ethsz = 0;
  struct mg_tcpip_driver_ppp_data *dd =
      (struct mg_tcpip_driver_ppp_data *) ifp->driver_data;
  uint8_t *buf = (uint8_t *) ifp->recv_queue.buf;

  if (!mg_ppp_atcmd_handle(ifp)) return 0;

  size_t bufsz = mg_ppp_rx_frame(dd, &ifp->recv_queue);
  if (!bufsz) return 0;
  uint16_t proto = (((uint16_t) buf[2]) << 8) | (uint16_t) buf[3];
  switch (proto) {
    case MG_PPP_PROTO_LCP: mg_ppp_handle_lcp(ifp, buf + 4, bufsz - 4); break;
    case MG_PPP_PROTO_IPCP: mg_ppp_handle_ipcp(ifp, buf + 4, bufsz - 4); break;
    case MG_PPP_PROTO_IP:
      MG_VERBOSE(("got IP packet of %d bytes", bufsz - 4));
      memmove(eth + 14, buf + 4, bufsz - 4);
      memmove(eth, ifp->mac, 6);
      memmove(eth + 6, "\xff\xff\xff\xff\xff\xff", 6);
      eth[12] = 0x08;
      eth[13] = 0x00;
      ethsz = bufsz - 4 + 14;
      ifp->recv_queue.head = 0;
      return ethsz;
#if 0
    default:
      MG_DEBUG(("unknown PPP frame:"));
      mg_hexdump(ppp->buf, ppp->bufsz);
#endif
  }
  ifp->recv_queue.head = 0;
  return 0;
  (void) ethlen;
}

struct mg_tcpip_driver mg_tcpip_driver_ppp = {mg_ppp_init, mg_ppp_tx, mg_ppp_rx,
                                              mg_ppp_poll};

#endif
