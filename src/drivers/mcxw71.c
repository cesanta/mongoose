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

// TrustZone: code built for the secure state (-mcmse) uses the secure alias
// of the radio registers, 0x5xxxxxxx. Otherwise, the non-secure alias
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
#define MGI_PHY_CTRL_ALL_IRQ_MASKS 0x8007ff00U  // Bits 8..18, 31: mask all IRQs
#define MGI_SEQ_STATE_MASK 0x1fU
#define MGI_SEQ_STATE_RX_WU 0x10U         // RX warm up
#define MGI_SEQ_CTRL_NO_RX_RECYCLE MG_BIT(5)
#define MGI_FRAME_FILTER 0x70bU  // Beacon, data, command; versions 0..2

enum { MGI_SEQ_IDLE = 0, MGI_SEQ_RX = 1, MGI_SEQ_TX = 2 };  // PHY_CTRL[XCVSEQ]
#define MGI_TX_TIMEOUT 400U  // Symbols, 16 us: 127 bytes

// Symbol timer, 16 us, 24 bits
static uint32_t mg_mcxw71_symbols(void) {
  return MGI_ZLL->EVENT_TMR >> 8;
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
    while ((MGI_ZLL->SEQ_STATE & MGI_SEQ_STATE_MASK) == MGI_SEQ_STATE_RX_WU)
      (void) 0;
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

static bool mg_mcxw71_init(struct mg_tcpip_if *ifp) {
  struct mg_l2_802154_data *d =
      (struct mg_l2_802154_data *) ifp->driver_data;
  if (d == NULL || d->channel < 11 || d->channel > 26) {
    MG_ERROR(("Set driver_data, channel 11..26"));
    return false;
  }
  MGI_ZLL->SEQ_CTRL_STS |= MGI_SEQ_CTRL_NO_RX_RECYCLE;
  MGI_ZLL->PHY_CTRL = MGI_PHY_CTRL_ALL_IRQ_MASKS;  // Mask all IRQs, we poll
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;
  MGI_ZLL->SAM_CTRL = 0;
  MGI_ZLL->RX_FRAME_FILTER = MGI_FRAME_FILTER;
  MGI_ZLL->TMR_PRESCALE = 5;  // 16 us timebase, one symbol
  MGI_ZLL->CHANNEL_NUM0 = d->channel;
  MGI_ZLL->MACSHORTADDRS0 = (0xfffeU << 16) | d->pan_id;  // Filter by PAN ID
  MGI_ZLL->PA_PWR = 0x3f;  // PA slices, max output power
  mg_mcxw71_start(MGI_SEQ_RX);
  return true;
}

// Send one radio frame: MAC header and payload, no FCS
static size_t mg_mcxw71_send(const uint8_t *p, size_t len,
                             struct mg_tcpip_if *ifp) {
  size_t i;
  bool ok;
  mg_mcxw71_idle();
  MGI_TX_PB[0] = (uint8_t) (len + 2);  // FCS is added by hardware
  for (i = 0; i < len; i++) MGI_TX_PB[i + 1] = p[i];  // 8-bit access only
  mg_mcxw71_start(MGI_SEQ_TX);
  ok = mg_mcxw71_wait();
  mg_mcxw71_idle();
  mg_mcxw71_start(MGI_SEQ_RX);
  (void) ifp;
  return ok ? len : 0;
}

static size_t mg_mcxw71_tx(const void *buf, size_t len,
                           struct mg_tcpip_if *ifp) {
  return mg_l2_802154_tx(ifp, buf, len, mg_mcxw71_send);
}

static size_t mg_mcxw71_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  uint32_t irqsts = MGI_ZLL->IRQSTS;
  uint8_t f[125];
  size_t i, n = 0;
  if ((irqsts & MGI_IRQSTS_SEQIRQ) == 0) return 0;  // Still receiving
  if (irqsts & MGI_IRQSTS_RXIRQ) {
    n = MGI_IRQSTS_RX_LEN(irqsts);
    n = n >= 2 ? n - 2 : 0;  // Strip FCS
    if (n > sizeof(f)) n = sizeof(f);
    for (i = 0; i < n; i++) f[i] = MGI_RX_PB[i];
  }
  mg_mcxw71_idle();
  mg_mcxw71_start(MGI_SEQ_RX);
  return mg_l2_802154_rx(ifp, f, n, buf, len);
}

static bool mg_mcxw71_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp, (void) s1;
  return true;  // Radio link is always up
}

struct mg_tcpip_driver mg_tcpip_driver_mcxw71 = {mg_mcxw71_init, mg_mcxw71_tx,
                                                 mg_mcxw71_rx, mg_mcxw71_poll};

#endif
