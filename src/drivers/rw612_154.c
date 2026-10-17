#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RW612_154) && \
    MG_ENABLE_DRIVER_RW612_154

// The 802.15.4 radio runs NXP combo firmware on CPU2, an OpenThread RCP. We
// are CPU3. Messages go through the IMU mailbox: IMU messages carry IMUMC
// messages, IMUMC messages carry HDLC framed Spinel. See NXP sources:
// mcuxsdk-core drivers/imu/fsl_imu.c, mcux-component
// imu_adapter/fsl_adapter_imu.c, connectivity framework
// platform/rw61x/fwk_platform_hdlc.c; OpenThread src/lib/spinel/spinel.h

// IMU message FIFO registers, one block per direction. See fsl_imu.h
struct mg_rw612_imu {
  volatile uint32_t WR_MSG, RD_MSG, STATUS, CNTL, RD_MSG_DBG;
};
#define MGI_IMU ((struct mg_rw612_imu *) 0x443ff804)       // CPU3 -> CPU2
#define MGI_IMU_PEER ((struct mg_rw612_imu *) 0x443ff818)  // CPU2 -> CPU3
#define MGI_IMU_LOCKED MG_BIT(0)          // STATUS: FIFO locked
#define MGI_IMU_ALMOST_FULL MG_BIT(1)     // STATUS: FIFO almost full
#define MGI_IMU_EMPTY MG_BIT(3)           // STATUS: FIFO empty
#define MGI_IMU_WR_PTR(s) (((s) >> 16) & 15U)
#define MGI_IMU_INT_CLR MG_BIT(0)         // CNTL: clear message ready IRQ
#define MGI_IMU_FLUSH MG_BIT(16)          // CNTL: flush send FIFO
#define MGI_IMU_WAIT_FOR_ACK MG_BIT(17)   // CNTL: lock send FIFO
#define MGI_IMU_WATERMARK(x) ((uint32_t) (x) << 20)
#define MGI_IMU_WATERMARK_MASK (15U << 20)

// IMU message: header, then payload words. See IMU_Msg_t
struct mg_rw612_msg {
  uint8_t type, subtype, seq, len;  // len: number of payload words
  uint32_t payload[8];
};

// Mailbox in SMU2 shared memory, at the address CPU2 firmware expects
struct mg_rw612_mbox {
  uint32_t magic[2];             // Sync pattern: [0] CPU2, [1] us
  struct mg_rw612_msg q[2][16];  // Message slots: [0] CPU2 -> us, [1] ours
};
#define MGI_MBOX ((volatile struct mg_rw612_mbox *) 0x443c0000)
#define MGI_TXBUF 0x443c1508U  // Our IMUMC buffers: 16 x 264 bytes
#define MGI_TXBUF_CNT 16
#define MGI_TXBUF_LEN 264U
#define MGI_SLEEP_FLAG (*(volatile uint32_t *) 0x443c2588)
#define MGI_SYNC_MAGIC 0xabcdef89U

// IMUMC message, carried in IMU payload words. See imumc_std_msg
struct mg_rw612_imumc {
  uint32_t src, dst;  // Endpoint addresses
  uint16_t rfu, idx;  // Reserved
  uint16_t len;       // Payload length, bytes
  uint16_t flags;     // Message flags
  uint32_t data;      // Payload buffer address
};
#define MGI_EPT_LOCAL 10   // Our 802.15.4 endpoint
#define MGI_EPT_REMOTE 20  // Firmware 802.15.4 endpoint

enum { MGI_MSG_CONTROL = 0, MGI_MSG_IMUMC = 8 };  // IMU message types
enum {                                            // Control message subtypes
  MGI_CTL_SYNC = 1,
  MGI_CTL_SYNC_ACK = 2,
  MGI_CTL_ACK_FOR_UNLOCK = 3,
  MGI_CTL_CMD_BUF_ADDR = 6,
  MGI_CTL_EPT_QUERY = 11,
  MGI_CTL_EPT_QUERY_RSP = 12,
  MGI_CTL_BUF_FREE = 13
};

// Spinel commands, properties and statuses we use
enum { MGI_CMD_RESET = 1, MGI_CMD_GET = 2, MGI_CMD_SET = 3, MGI_CMD_IS = 6 };
enum {
  MGI_PROP_LAST_STATUS = 0,
  MGI_PROP_NCP_VERSION = 2,
  MGI_PROP_PHY_ENABLED = 0x20,
  MGI_PROP_PHY_CHAN = 0x21,
  MGI_PROP_LADDR = 0x34,
  MGI_PROP_SADDR = 0x35,
  MGI_PROP_PANID = 0x36,
  MGI_PROP_RAW_STREAM = 0x37,
  MGI_PROP_STREAM_RAW = 0x71
};
#define MGI_RESET_STACK 2         // RESET argument: reset the RCP stack only
#define MGI_STATUS_RESET_MIN 112  // LAST_STATUS: RCP has reset
#define MGI_STATUS_RESET_MAX 127
#define MGI_TX_TIMEOUT_MS 200  // Wait for transmit done, incl. retries
#define MGI_RSP_TIMEOUT_MS 500
#define MGI_RXQ_CNT 4

// Driver state, one radio
struct mg_rw612 {
  bool up;                  // IMU link synchronised
  bool ept_ready;           // IMUMC endpoints connected
  bool reset;               // RCP reported a reset
  uint8_t seq;              // IMU message sequence number
  uint16_t free_bufs;       // Free IMUMC buffers, bit per buffer
  uint8_t hdlc[300];        // HDLC frame being received, unescaped
  size_t hlen;              // Bytes in hdlc
  bool esc;                 // Previous byte was an escape
  uint8_t tid;              // Spinel transaction ID of the pending request
  bool rsp;                 // Response to the pending request received
  uint32_t rsp_status;      // Its status, if LAST_STATUS, else 0
  char version[64];         // RCP version string
  uint8_t rxq[MGI_RXQ_CNT][127];  // Received frames, no FCS
  uint8_t rxlen[MGI_RXQ_CNT];     // Their lengths
  uint8_t rxhead, rxtail;         // Queue indexes
  uint16_t pan_id, short_addr;    // Addresses programmed in the RCP
  uint8_t eui[8];
  uint8_t channel;
};
static struct mg_rw612 s_rw612;

static void mg_rw612_imu_send(uint8_t type, uint8_t subtype,
                              const uint32_t *payload, uint8_t len) {
  volatile struct mg_rw612_msg *m;
  uint64_t expire = mg_millis() + 100;
  uint8_t i;
  while ((MGI_IMU->STATUS & (MGI_IMU_ALMOST_FULL | MGI_IMU_LOCKED)) &&
         mg_millis() < expire) {
    (void) 0;
  }
  m = &MGI_MBOX->q[1][MGI_IMU_WR_PTR(MGI_IMU->STATUS)];
  m->type = type, m->subtype = subtype, m->seq = s_rw612.seq++, m->len = len;
  for (i = 0; i < len; i++) m->payload[i] = payload[i];
  MGI_IMU->WR_MSG = (uint32_t) (uintptr_t) m;
}

static void mg_rw612_control(uint8_t subtype, uint32_t a, uint32_t b,
                             uint8_t len) {
  uint32_t payload[2] = {a, b};
  mg_rw612_imu_send(MGI_MSG_CONTROL, subtype, payload, len);
}

static void mg_rw612_queue_rx(const uint8_t *frame, size_t len) {
  struct mg_rw612 *s = &s_rw612;
  uint8_t next = (uint8_t) ((s->rxhead + 1) % MGI_RXQ_CNT);
  if (next == s->rxtail || len > sizeof(s->rxq[0])) return;  // Full: drop
  memcpy(s->rxq[s->rxhead], frame, len);
  s->rxlen[s->rxhead] = (uint8_t) len;
  s->rxhead = next;
}

// Spinel packed unsigned integer: 7 bits per byte, LSB first
static size_t mg_rw612_unpack(const uint8_t *p, size_t len, uint32_t *v) {
  size_t i;
  *v = 0;
  for (i = 0; i < len && i < 4; i++) {
    *v |= (uint32_t) (p[i] & 0x7f) << (7 * i);
    if ((p[i] & 0x80) == 0) return i + 1;
  }
  return 0;
}

// Handle a received Spinel frame: header, command, property, value
static void mg_rw612_spinel(const uint8_t *f, size_t len) {
  struct mg_rw612 *s = &s_rw612;
  uint32_t cmd, prop, status = 0;
  size_t n, ofs = 1;
  uint8_t tid = f[0] & 15;
  if (len < 3 || (n = mg_rw612_unpack(f + ofs, len - ofs, &cmd)) == 0) return;
  ofs += n;
  if ((n = mg_rw612_unpack(f + ofs, len - ofs, &prop)) == 0) return;
  ofs += n;
  if (cmd != MGI_CMD_IS) return;
  if (prop == MGI_PROP_STREAM_RAW && tid == 0 && len - ofs >= 2) {
    size_t flen = (size_t) (f[ofs] | (f[ofs + 1] << 8));  // Incl. FCS
    if (flen >= 2 && ofs + 2 + flen <= len) {
      mg_rw612_queue_rx(f + ofs + 2, flen - 2);
    }
  } else if (prop == MGI_PROP_LAST_STATUS) {
    mg_rw612_unpack(f + ofs, len - ofs, &status);
    if (tid == 0 && status >= MGI_STATUS_RESET_MIN &&
        status <= MGI_STATUS_RESET_MAX) {
      s->reset = true;
    }
  } else if (prop == MGI_PROP_NCP_VERSION) {
    n = len - ofs < sizeof(s->version) - 1 ? len - ofs : sizeof(s->version) - 1;
    memcpy(s->version, f + ofs, n);
    s->version[n] = '\0';
  }
  if (tid != 0 && tid == s->tid) s->rsp = true, s->rsp_status = status;
}

// HDLC-lite: 0x7e delimits frames, 0x7d escapes, FCS-16 at the end
static uint16_t mg_rw612_fcs(uint16_t fcs, uint8_t c) {
  int i;
  fcs ^= c;
  for (i = 0; i < 8; i++) fcs = (fcs & 1) ? (fcs >> 1) ^ 0x8408 : fcs >> 1;
  return fcs;
}

static void mg_rw612_hdlc_rx(const uint8_t *p, size_t len) {
  struct mg_rw612 *s = &s_rw612;
  size_t i, j;
  for (i = 0; i < len; i++) {
    uint8_t c = p[i];
    if (c == 0x7e) {
      if (s->hlen >= 3) {
        uint16_t fcs = 0xffff;
        for (j = 0; j < s->hlen; j++) fcs = mg_rw612_fcs(fcs, s->hdlc[j]);
        if (fcs == 0xf0b8) mg_rw612_spinel(s->hdlc, s->hlen - 2);
      }
      s->hlen = 0, s->esc = false;
    } else if (c == 0x7d) {
      s->esc = true;
    } else if (s->hlen < sizeof(s->hdlc)) {
      s->hdlc[s->hlen++] = s->esc ? c ^ 0x20 : c;
      s->esc = false;
    }
  }
}

static void mg_rw612_control_rx(volatile struct mg_rw612_msg *m) {
  struct mg_rw612 *s = &s_rw612;
  switch (m->subtype) {
    case MGI_CTL_ACK_FOR_UNLOCK:
      MGI_IMU->CNTL &= ~MGI_IMU_WAIT_FOR_ACK;
      break;
    case MGI_CTL_SYNC:  // As HAL_ImuCtrlHandler(): acknowledge twice
      mg_rw612_control(MGI_CTL_SYNC_ACK, 0, 0, 0);
      mg_rw612_control(MGI_CTL_SYNC_ACK, 0, 0, 0);
      s->up = true;
      break;
    case MGI_CTL_SYNC_ACK:
    case MGI_CTL_CMD_BUF_ADDR:
      s->up = true;
      break;
    case MGI_CTL_EPT_QUERY: {  // Firmware asks if our endpoint exists
      bool ours = m->payload[0] == MGI_EPT_LOCAL;
      mg_rw612_control(MGI_CTL_EPT_QUERY_RSP, m->payload[0], ours, 2);
      if (ours) s->ept_ready = true;
      break;
    }
    case MGI_CTL_EPT_QUERY_RSP:
      if (m->payload[0] == MGI_EPT_REMOTE && m->payload[1] != 0) {
        s->ept_ready = true;
      }
      break;
    case MGI_CTL_BUF_FREE: {  // Firmware is done with one of our buffers
      uint32_t i = (m->payload[0] - MGI_TXBUF) / MGI_TXBUF_LEN;
      if (i < MGI_TXBUF_CNT) s->free_bufs |= (uint16_t) MG_BIT(i);
      break;
    }
    default:
      break;
  }
}

// Process all received IMU messages. See HAL_ImuReceive()
static void mg_rw612_poll_imu(void) {
  bool unlock = false;
  while ((MGI_IMU_PEER->STATUS & MGI_IMU_EMPTY) == 0) {
    struct mg_rw612_msg m;
    volatile struct mg_rw612_msg *p;
    uint8_t i;
    MGI_SLEEP_FLAG &= ~1U;
    p = (volatile struct mg_rw612_msg *) (uintptr_t) MGI_IMU->RD_MSG;
    if (MGI_IMU_PEER->STATUS & MGI_IMU_LOCKED) unlock = true;
    if (p == NULL) continue;
    m.type = p->type, m.subtype = p->subtype, m.len = p->len;
    for (i = 0; i < 8; i++) m.payload[i] = p->payload[i];
    if (m.type == MGI_MSG_CONTROL) {
      mg_rw612_control_rx(&m);
    } else if (m.type == MGI_MSG_IMUMC) {
      struct mg_rw612_imumc im;
      memcpy(&im, m.payload, sizeof(im));
      if (im.dst == MGI_EPT_LOCAL) {
        mg_rw612_hdlc_rx((const uint8_t *) (uintptr_t) im.data, im.len);
      }
      mg_rw612_control(MGI_CTL_BUF_FREE, im.data, 0, 1);
    }
  }
  MGI_IMU->CNTL |= MGI_IMU_INT_CLR;
  MGI_SLEEP_FLAG |= 1U;
  if (unlock) mg_rw612_control(MGI_CTL_ACK_FOR_UNLOCK, 0, 0, 0);
}

// Send bytes to the firmware endpoint, in IMUMC buffer sized chunks
static bool mg_rw612_imumc_send(const uint8_t *p, size_t len) {
  struct mg_rw612 *s = &s_rw612;
  while (len > 0) {
    size_t n = len < MGI_TXBUF_LEN ? len : MGI_TXBUF_LEN;
    uint64_t expire = mg_millis() + 100;
    struct mg_rw612_imumc im;
    uint32_t i;
    while (s->free_bufs == 0) {
      mg_rw612_poll_imu();
      if (mg_millis() > expire) return false;
    }
    for (i = 0; (s->free_bufs & MG_BIT(i)) == 0; i++) (void) 0;
    s->free_bufs &= (uint16_t) ~MG_BIT(i);
    memset(&im, 0, sizeof(im));
    im.src = MGI_EPT_LOCAL, im.dst = MGI_EPT_REMOTE, im.len = (uint16_t) n;
    im.data = MGI_TXBUF + i * MGI_TXBUF_LEN;
    memcpy((void *) (uintptr_t) im.data, p, n);
    mg_rw612_imu_send(MGI_MSG_IMUMC, 0, (uint32_t *) &im, sizeof(im) / 4);
    p += n, len -= n;
  }
  return true;
}

// Send a Spinel frame, HDLC encoded, and wait for the response with the
// same transaction ID. Return false on timeout
static bool mg_rw612_request(uint8_t *f, size_t len, uint32_t timeout_ms) {
  struct mg_rw612 *s = &s_rw612;
  uint8_t buf[2 * 160 + 6];
  uint16_t fcs = 0xffff;
  size_t i, n = 0;
  uint64_t expire = mg_millis() + timeout_ms;
  s->tid = (uint8_t) (s->tid % 15 + 1);
  f[0] = (uint8_t) (0x80 | s->tid);  // Header: flag, interface 0, TID
  s->rsp = false;
  buf[n++] = 0x7e;
  for (i = 0; i < len + 2 && n + 3 < sizeof(buf); i++) {
    uint8_t c;
    if (i < len) {
      c = f[i], fcs = mg_rw612_fcs(fcs, c);
    } else {
      c = (uint8_t) (i == len ? ~fcs & 0xff : ~fcs >> 8);
    }
    if (c == 0x7e || c == 0x7d || c == 0x11 || c == 0x13 || c == 0xf8) {
      buf[n++] = 0x7d, c ^= 0x20;
    }
    buf[n++] = c;
  }
  buf[n++] = 0x7e;
  if (!mg_rw612_imumc_send(buf, n)) return false;
  while (!s->rsp && mg_millis() < expire) mg_rw612_poll_imu();
  return s->rsp;
}

static bool mg_rw612_set(uint32_t prop, const uint8_t *val, size_t len) {
  uint8_t f[16];
  if (len > sizeof(f) - 3) return false;
  f[1] = MGI_CMD_SET, f[2] = (uint8_t) prop;  // Properties used are < 0x80
  memcpy(f + 3, val, len);
  return mg_rw612_request(f, len + 3, MGI_RSP_TIMEOUT_MS) &&
         s_rw612.rsp_status == 0;
}

// Program channel, PAN ID, short address and EUI-64 when they change
static void mg_rw612_addrs(struct mg_tcpip_if *ifp) {
  struct mg_rw612 *s = &s_rw612;
  struct lowpan_data *l = &ifp->l2data.lowpan;
  uint8_t v[2];
  if (s->channel != l->channel) {
    s->channel = l->channel;
    if (!mg_rw612_set(MGI_PROP_PHY_CHAN, &l->channel, 1)) MG_ERROR(("chan"));
  }
  if (s->pan_id != l->pan_id) {
    s->pan_id = l->pan_id;
    v[0] = (uint8_t) l->pan_id, v[1] = (uint8_t) (l->pan_id >> 8);
    if (!mg_rw612_set(MGI_PROP_PANID, v, 2)) MG_ERROR(("panid"));
  }
  if (s->short_addr != l->short_addr) {
    s->short_addr = l->short_addr;
    v[0] = (uint8_t) l->short_addr, v[1] = (uint8_t) (l->short_addr >> 8);
    if (!mg_rw612_set(MGI_PROP_SADDR, v, 2)) MG_ERROR(("saddr"));
  }
  if (memcmp(s->eui, ifp->mac, 8) != 0) {
    memcpy(s->eui, ifp->mac, 8);
    if (!mg_rw612_set(MGI_PROP_LADDR, s->eui, 8)) MG_ERROR(("laddr"));
  }
}

// Log an init error, return false
static bool mg_rw612_fail(const char *msg) {
  MG_ERROR(("%s", msg));
  return false;
}

static bool mg_rw612_154_init(struct mg_tcpip_if *ifp) {
  struct mg_rw612 *s = &s_rw612;
  uint8_t f[8], one = 1;
  uint64_t expire;
  if (ifp->l2type != MG_TCPIP_L2_LOWPAN || ifp->l2data.lowpan.channel < 11 ||
      ifp->l2data.lowpan.channel > 26) {
    MG_ERROR(("Use MG_TCPIP_L2_LOWPAN, channel 11..26"));
    return false;
  }
  if (MGI_MBOX->magic[0] != MGI_SYNC_MAGIC) {
    MG_ERROR(("Radio firmware is not running on CPU2"));
    return false;
  }
  memset(s, 0, sizeof(*s));
  s->free_bufs = 0xffff;
  s->pan_id = s->short_addr = 0xffff;  // Force programming
  // Reset our send FIFO, drop stale messages. See IMU_Init()
  MGI_IMU->CNTL |= MGI_IMU_FLUSH;
  MGI_IMU->CNTL &= ~(MGI_IMU_FLUSH | MGI_IMU_WAIT_FOR_ACK);
  MGI_IMU->CNTL = (MGI_IMU->CNTL & ~MGI_IMU_WATERMARK_MASK) |
                  MGI_IMU_WATERMARK(15);
  while ((MGI_IMU_PEER->STATUS & MGI_IMU_EMPTY) == 0) (void) MGI_IMU->RD_MSG;
  // Synchronise the IMU link, then connect the IMUMC endpoints
  MGI_MBOX->magic[1] = MGI_SYNC_MAGIC;
  mg_rw612_control(MGI_CTL_SYNC, 0, 0, 0);
  expire = mg_millis() + 1000;
  while (!s->up && mg_millis() < expire) mg_rw612_poll_imu();
  if (!s->up) return mg_rw612_fail("IMU sync timeout");
  expire = mg_millis() + 2000;
  while (!s->ept_ready && mg_millis() < expire) {
    uint64_t retry = mg_millis() + 100;
    mg_rw612_control(MGI_CTL_EPT_QUERY, MGI_EPT_REMOTE, 0, 1);
    while (!s->ept_ready && mg_millis() < retry) mg_rw612_poll_imu();
  }
  if (!s->ept_ready) return mg_rw612_fail("IMUMC endpoint timeout");
  // Reset the RCP stack, wait for its reset status, read its version
  f[1] = MGI_CMD_RESET, f[2] = MGI_RESET_STACK;
  mg_rw612_request(f, 3, MGI_RSP_TIMEOUT_MS);
  expire = mg_millis() + 1000;
  while (!s->reset && mg_millis() < expire) mg_rw612_poll_imu();
  if (!s->reset) return mg_rw612_fail("RCP reset timeout");
  f[1] = MGI_CMD_GET, f[2] = MGI_PROP_NCP_VERSION;
  mg_rw612_request(f, 3, MGI_RSP_TIMEOUT_MS);
  MG_INFO(("RCP: %s", s->version));
  if (!mg_rw612_set(MGI_PROP_PHY_ENABLED, &one, 1)) {
    return mg_rw612_fail("PHY enable failed");
  }
  mg_rw612_addrs(ifp);
  if (!mg_rw612_set(MGI_PROP_RAW_STREAM, &one, 1)) {
    return mg_rw612_fail("Raw stream enable failed");
  }
  return true;
}

static size_t mg_rw612_154_tx(const void *buf, size_t len,
                              struct mg_tcpip_if *ifp) {
  uint8_t f[3 + 2 + 127 + 4];
  size_t n = 3;
  if (len == 0 || len > 125) return 0;  // 127 bytes max, incl. 2 bytes FCS
  f[1] = MGI_CMD_SET, f[2] = MGI_PROP_STREAM_RAW;
  f[n++] = (uint8_t) (len + 2), f[n++] = 0;  // PSDU length, incl. FCS
  memcpy(f + n, buf, len), n += len;
  f[n++] = 0, f[n++] = 0;  // FCS, filled in by the radio
  f[n++] = ifp->l2data.lowpan.channel;
  f[n++] = 4;  // Max CSMA backoffs
  f[n++] = 3;  // Max frame retries, for frames with ACK request
  f[n++] = 1;  // CSMA-CA enabled
  if (!mg_rw612_request(f, n, MGI_TX_TIMEOUT_MS)) {
    MG_ERROR(("TX timeout"));
    return 0;
  }
  if (s_rw612.rsp_status != 0) {
    MG_VERBOSE(("TX failed, status %lu", (unsigned long) s_rw612.rsp_status));
    return 0;
  }
  return len;
}

static size_t mg_rw612_154_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct mg_rw612 *s = &s_rw612;
  size_t n = 0;
  mg_rw612_poll_imu();
  mg_rw612_addrs(ifp);
  if (s->rxtail != s->rxhead) {
    n = s->rxlen[s->rxtail];
    if (n > len) n = len;
    memcpy(buf, s->rxq[s->rxtail], n);
    s->rxtail = (uint8_t) ((s->rxtail + 1) % MGI_RXQ_CNT);
  }
  return n;
}

static bool mg_rw612_154_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp, (void) s1;
  return s_rw612.up;
}

struct mg_tcpip_driver mg_tcpip_driver_rw612_154 = {
    mg_rw612_154_init, mg_rw612_154_tx, mg_rw612_154_rx, mg_rw612_154_poll};

#endif
