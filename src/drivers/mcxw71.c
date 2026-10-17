#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_MCXW71) && \
    MG_ENABLE_DRIVER_MCXW71

// ZLL, 802.15.4 link layer. See MCXW71 reference manual and NXP
// ieee_802_15_4 PHY sources: PhyPacketProcessor.c, PhyPlmeData.c, PhyISR.c
struct mg_zll {
  volatile uint32_t IRQSTS, PHY_CTRL, EVENT_TMR, TIMESTAMP, T1CMP, T2CMP,
      T2PRIMECMP, T3CMP, T4CMP, PA_PWR, CHANNEL_NUM0, LQI_AND_RSSI,
      MACSHORTADDRS0, MACLONGADDRS0_LSB, MACLONGADDRS0_MSB, RX_FRAME_FILTER,
      CCA_LQI_CTRL, CCA2_CTRL, RESERVED_0, DSM_CTRL, RESERVED_1,
      MACSHORTADDRS1, MACLONGADDRS1_LSB, MACLONGADDRS1_MSB, DUAL_PAN_CTRL,
      CHANNEL_NUM1, SAM_CTRL, SAM_TABLE, SAM_MATCH, SAM_FREE_IDX,
      SEQ_CTRL_STS, ACKDELAY, FILTERFAIL_CODE, RX_WTR_MARK, RESERVED_2,
      SLOT_PRELOAD, SEQ_STATE, TMR_PRESCALE, LENIENCY_LSB, LENIENCY_MSB,
      PART_ID, COEX_CTRL, COEX_PRIORITY, ENHACK_CTRL0;
};

#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 2)
#define MGI_ZLL ((struct mg_zll *) 0x58A01000)
#define MGI_PKT_RAM ((volatile uint8_t *) 0x58A08000)
#else
#define MGI_ZLL ((struct mg_zll *) 0x48A01000)
#define MGI_PKT_RAM ((volatile uint8_t *) 0x48A08000)
#endif
#define MGI_TX_PB MGI_PKT_RAM           // Length byte (incl. FCS), then frame
#define MGI_RX_PB (MGI_PKT_RAM + 0x80)  // Received frame, no length byte

#define MGI_IRQSTS_SEQIRQ MG_BIT(0)
#define MGI_IRQSTS_RXIRQ MG_BIT(2)
#define MGI_IRQSTS_RX_LEN(x) (((x) >> 24) & 0x7f)  // Frame length incl. FCS
#define MGI_PHY_CTRL_XCVSEQ 7U
#define MGI_PHY_CTRL_AUTOACK MG_BIT(3)
#define MGI_PHY_CTRL_RXACKRQD MG_BIT(4)
#define MGI_PHY_CTRL_CCABFRTX MG_BIT(5)
#define MGI_PHY_CTRL_SLOTTED MG_BIT(6)
#define MGI_PHY_CTRL_PROMISCUOUS MG_BIT(25)
#define MGI_PHY_CTRL_ALL_IRQ_MASKS 0x8007ff00U  // Bits 8..18, 31: mask all IRQs
#define MGI_SEQ_STATE_MASK 0x1fU
#define MGI_SEQ_STATE_RX_WU 0x10U         // RX warm up
#define MGI_SEQ_CTRL_NO_RX_RECYCLE MG_BIT(5)
#define MGI_FRAME_FILTER 0x70bU  // Beacon, data, command; versions 0..2
#define MGI_ACKDELAY_MASK 0x7fU
#define MGI_ACKDELAY_VALUE 0x7cU  // -4, 802.15.4 turnaround. See PhyHwInit()

enum { MGI_SEQ_IDLE = 0, MGI_SEQ_RX = 1, MGI_SEQ_TX = 2, MGI_SEQ_TR = 4 };  // PHY_CTRL[XCVSEQ]
#define MGI_FCF_ACK_REQUEST 0x20U  // First frame byte: acknowledgment request
#define MGI_TX_TIMEOUT 400U        // Symbols, 16 us: 127 bytes, then ACK wait
#define MGI_TX_TRIES 10            // Tries for unacknowledged frames
#define MGI_TX_BACKOFF 64U         // Symbols, 1 ms: backoff grows by this per try

// Symbol timer, 16 us, 24 bits
static uint32_t mg_mcxw71_symbols(void) {
  return MGI_ZLL->EVENT_TMR >> 8;
}

static void mg_mcxw71_delay(uint32_t symbols) {
  uint32_t start = mg_mcxw71_symbols();
  while (((mg_mcxw71_symbols() - start) & 0xffffffU) < symbols) (void) 0;
}

// Wait for the sequence to end, up to MGI_TX_TIMEOUT symbols
static bool mg_mcxw71_wait(void) {
  uint32_t start = mg_mcxw71_symbols();
  while (((mg_mcxw71_symbols() - start) & 0xffffffU) < MGI_TX_TIMEOUT) {
    if (MGI_ZLL->IRQSTS & MGI_IRQSTS_SEQIRQ) return true;
  }
  return false;
}

// Stop the current sequence, wait for the sequencer to go idle. See PhyAbort()
static void mg_mcxw71_idle(void) {
  if (MGI_ZLL->PHY_CTRL & MGI_PHY_CTRL_XCVSEQ) {
    while ((MGI_ZLL->SEQ_STATE & MGI_SEQ_STATE_MASK) == MGI_SEQ_STATE_RX_WU) (void) 0;
    MGI_ZLL->PHY_CTRL &= ~MGI_PHY_CTRL_XCVSEQ;
    while (MGI_ZLL->SEQ_STATE & MGI_SEQ_STATE_MASK) (void) 0;
  }
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;  // Clear all flags, write 1 to clear
}

// Start a new sequence: MGI_SEQ_RX or MGI_SEQ_TX
static void mg_mcxw71_start(uint32_t seq) {
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;
  MGI_ZLL->PHY_CTRL |= seq;
}

// Program our addresses for hardware filtering and auto-ACK: PAN ID, short
// address and EUI-64 (little endian), when they change. The L2 sets them
static void mg_mcxw71_addrs(struct mg_tcpip_if *ifp) {
  struct lowpan_data *l = &ifp->l2data.lowpan;
  const uint8_t *e = ifp->mac;
  uint32_t s = ((uint32_t) l->short_addr << 16) | l->pan_id;
  uint32_t lo = (uint32_t) (e[7] | (e[6] << 8) | (e[5] << 16)) |
                ((uint32_t) e[4] << 24);
  uint32_t hi = (uint32_t) (e[3] | (e[2] << 8) | (e[1] << 16)) |
                ((uint32_t) e[0] << 24);
  if (MGI_ZLL->MACSHORTADDRS0 != s || MGI_ZLL->MACLONGADDRS0_LSB != lo ||
      MGI_ZLL->MACLONGADDRS0_MSB != hi) {
    mg_mcxw71_idle();
    MGI_ZLL->MACSHORTADDRS0 = s;
    MGI_ZLL->MACLONGADDRS0_LSB = lo, MGI_ZLL->MACLONGADDRS0_MSB = hi;
    mg_mcxw71_start(MGI_SEQ_RX);
  }
}

static bool mg_mcxw71_init(struct mg_tcpip_if *ifp) {
  uint8_t channel = ifp->l2data.lowpan.channel;
  if (ifp->l2type != MG_TCPIP_L2_LOWPAN || channel < 11 || channel > 26) {
    MG_ERROR(("Use MG_TCPIP_L2_LOWPAN, channel 11..26"));
    return false;
  }
  MGI_ZLL->SEQ_CTRL_STS |= MGI_SEQ_CTRL_NO_RX_RECYCLE;
  // Mask all interrupts, we poll. Filter frames by address, auto-ACK
  MGI_ZLL->PHY_CTRL = MGI_PHY_CTRL_ALL_IRQ_MASKS | MGI_PHY_CTRL_AUTOACK;
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;
  MGI_ZLL->SAM_CTRL = 0;
  MGI_ZLL->RX_FRAME_FILTER = MGI_FRAME_FILTER;
  MGI_ZLL->ACKDELAY = (MGI_ZLL->ACKDELAY & ~MGI_ACKDELAY_MASK) | MGI_ACKDELAY_VALUE;
  MGI_ZLL->TMR_PRESCALE = 5;  // 16 us timebase, one symbol
  MGI_ZLL->CHANNEL_NUM0 = channel;
  MGI_ZLL->PA_PWR = 0x3f;     // PA slices, max output power
  mg_mcxw71_start(MGI_SEQ_RX);
  mg_mcxw71_addrs(ifp);
  return true;
}

static size_t mg_mcxw71_tx(const void *buf, size_t len,
                           struct mg_tcpip_if *ifp) {
  const uint8_t *p = (const uint8_t *) buf;
  size_t i;
  bool ok = false, ack = len > 0 && (p[0] & MGI_FCF_ACK_REQUEST);
  int tries;
  if (len == 0 || len > 125) return 0;  // 127 bytes max, incl. 2 bytes FCS
  mg_mcxw71_idle();
  MGI_TX_PB[0] = (uint8_t) (len + 2);  // FCS is added by hardware
  for (i = 0; i < len; i++) MGI_TX_PB[i + 1] = p[i];  // 8-bit access only
  MGI_ZLL->PHY_CTRL &= ~(MGI_PHY_CTRL_CCABFRTX | MGI_PHY_CTRL_RXACKRQD | MGI_PHY_CTRL_SLOTTED);
  if (ack) MGI_ZLL->PHY_CTRL |= MGI_PHY_CTRL_RXACKRQD;
  // Frames with ACK request: transmit, then receive the ACK (TR sequence),
  // retry with growing backoff. See PhyPdDataRequest(), PhyISR.c
  for (tries = 0; tries < (ack ? MGI_TX_TRIES : 1) && !ok; tries++) {
    if (tries > 0) mg_mcxw71_delay(MGI_TX_BACKOFF * (uint32_t) tries);
    mg_mcxw71_start(ack ? MGI_SEQ_TR : MGI_SEQ_TX);
    ok = mg_mcxw71_wait() && (!ack || (MGI_ZLL->IRQSTS & MGI_IRQSTS_RXIRQ));
    mg_mcxw71_idle();
  }
  MGI_ZLL->PHY_CTRL &= ~MGI_PHY_CTRL_RXACKRQD;
  mg_mcxw71_start(MGI_SEQ_RX);
  if (!ok) MG_DEBUG(("TX failed, %d tries", tries));
  (void) ifp;
  return ok ? len : 0;
}

static size_t mg_mcxw71_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  uint32_t irqsts = MGI_ZLL->IRQSTS;
  size_t i, n = 0;
  mg_mcxw71_addrs(ifp);
  if ((irqsts & MGI_IRQSTS_SEQIRQ) == 0) return 0;  // Still receiving
  if (irqsts & MGI_IRQSTS_RXIRQ) {
    n = MGI_IRQSTS_RX_LEN(irqsts);
    n = n >= 2 ? n - 2 : 0;  // Strip FCS
    if (n > len) n = len;
    for (i = 0; i < n; i++) ((uint8_t *) buf)[i] = MGI_RX_PB[i];
  }
  mg_mcxw71_idle();
  mg_mcxw71_start(MGI_SEQ_RX);
  (void) ifp;
  return n;
}

static bool mg_mcxw71_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp, (void) s1;
  return true;  // Radio link is always up
}

struct mg_tcpip_driver mg_tcpip_driver_mcxw71 = {mg_mcxw71_init, mg_mcxw71_tx,
                                                 mg_mcxw71_rx, mg_mcxw71_poll};

#endif
