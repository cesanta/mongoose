#include "log.h"
#include "net_builtin.h"
#include "radio.h"
#include "util.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_MCXW71) && \
    MG_ENABLE_DRIVER_MCXW71

// TrustZone: code built for the secure state (-mcmse) uses the secure alias
// of the radio registers, 0x5xxxxxxx. Otherwise, the non-secure alias
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 2)
#define MGI_RF_BASE 0x58A00000U
#define MGI_PKT_RAM ((volatile uint8_t *) 0x58A08000)
#else
#define MGI_RF_BASE 0x48A00000U
#define MGI_PKT_RAM ((volatile uint8_t *) 0x48A08000)
#endif

#define MGI_NVIC_ISER ((volatile uint32_t *) 0xE000E100)  // Enable IRQ
#define MGI_NVIC_ICER ((volatile uint32_t *) 0xE000E180)  // Disable IRQ
#if MG_RADIO == MG_RADIO_BLE
#define MGI_IRQN 54  // RF_Generic_IRQn
#else
#define MGI_IRQN 53  // RF_802_15_4_IRQn
#endif

// Keep the IRQ handler out while we change the radio. The NVIC disable is
// atomic, so register updates here and there do not race
static void mg_mcxw71_lock(void) {
  MGI_NVIC_ICER[MGI_IRQN / 32] = MG_BIT(MGI_IRQN % 32);
  __asm volatile("dsb; isb" ::: "memory");
}

static struct mg_tcpip_if *s_mcxw71_ifp;
static volatile bool s_mcxw71_txing, s_mcxw71_done;
static uint64_t s_mcxw71_sendstart;

#if MG_RADIO == MG_RADIO_BLE

// GENFSK Bluetooth LE uncoded mode, RM Table 489. S0 + 8-bit length, then
// our frame. The PHY supplies the preamble, whitening and 24-bit CRC.
struct mg_genfsk {
  volatile uint32_t IRQ_CTRL, EVENT_TMR, T1_CMP, T2_CMP, TIMESTAMP, XCVR_CTRL,
      XCVR_STS, XCVR_CFG, CHANNEL_NUM0, TX_POWER, NTW_ADR_CTRL, NTW_ADR[4],
      RX_WATERMARK, DSM_CTRL, PART_ID, SLOT_PRELOAD, SLOT_TIME, TURNAROUND_TIME,
      ACKDELAY, RXDELAY, TXDELAY, PACKET_CFG, H0_CFG, H1_CFG, CRC_CFG,
      LENGTH_ADJ, TIMESTAMP_RX_DONE, TIMESTAMP_TX_DONE, MULT_PKT_CTRL,
      RPA_WL_STATUS, LENGTH_MAX, EVENT_TMR_LD, EVENT_TMR_ADD, ENH_FEATURE;
  uint32_t reserved[21];
  volatile uint32_t SEQ_STS;
};
#define MGI_GFSK ((struct mg_genfsk *) (MGI_RF_BASE + 0x2000))
#define MGI_TX_PB MGI_PKT_RAM  // Access address, S0, length, frame; no CRC
#define MGI_RX_PB (MGI_PKT_RAM + 0x1000)  // S0, length, frame, CRC; no AA
#define MGI_RADIO_MAX 251
#define MGI_GFSK_IRQ_EN (MG_BIT(26) | MG_BIT(16))  // Master + sequence end
// BLE preamble starts with the access address's first bit; transmit LSBit
// first.
#define MGI_GFSK_CFG \
  (MG_BIT(24) | (((MG_RADIO_NETWORK & 1U) ? 0x55U : 0xaaU) << 16) | (1U << 4))

static void mg_mcxw71_abort(void) {
  // NXP CONNRF-767: do not abort during TX warmup.
  while (MGI_GFSK->SEQ_STS & MG_BIT(2)) (void) 0;
  MGI_GFSK->XCVR_CTRL = 11;  // ABORT_ALL
  while (MGI_GFSK->XCVR_CTRL & MG_BIT(31)) (void) 0;
  MGI_GFSK->IRQ_CTRL = MGI_GFSK_IRQ_EN | 0xffffU;  // W1C status
}

static void mg_mcxw71_listen(void) {
  MGI_GFSK->IRQ_CTRL = MGI_GFSK_IRQ_EN | 0xffffU;
  MGI_GFSK->XCVR_CFG = MGI_GFSK_CFG;
  MGI_GFSK->XCVR_CTRL = 5;  // RX_START_NOW
  MGI_NVIC_ISER[MGI_IRQN / 32] = MG_BIT(MGI_IRQN % 32);
}

static void mg_mcxw71_go(void) {
  mg_mcxw71_abort();
  MGI_GFSK->XCVR_CFG = MGI_GFSK_CFG;  // ABORT disables packet processing
  MGI_GFSK->XCVR_CTRL = 1;            // TX_START_NOW
}

void RF_Generic_IRQHandler(void);
void RF_Generic_IRQHandler(void) {
  uint32_t status = MGI_GFSK->IRQ_CTRL;
  if (!(status & MG_BIT(0))) return;
  mg_mcxw71_abort();
  if (s_mcxw71_txing) {
    s_mcxw71_txing = false;
    s_mcxw71_done = true;
  } else if ((status & (MG_BIT(2) | MG_BIT(10))) ==
             (MG_BIT(2) | MG_BIT(10))) {  // RX and CRC valid
    uint8_t frame[MGI_RADIO_MAX];
    size_t i, n = MGI_RX_PB[1];
    if (n > 0 && n <= sizeof(frame)) {
      for (i = 0; i < n; i++) frame[i] = MGI_RX_PB[i + 2];
      mg_radio_rx_queue(s_mcxw71_ifp, frame, n);
    }
  }
  mg_mcxw71_listen();
}

static bool mgi_mcxw71_radio_init(struct mg_tcpip_if *ifp, uint16_t addr) {
  s_mcxw71_ifp = ifp;
  (void) addr;  // Frames are filtered by mg_radio_rx_queue()
  MGI_GFSK->IRQ_CTRL = 0xffffU;
  mg_mcxw71_abort();
  // GENFSK channels are MHz above 2360; MG_RADIO_CHANNEL is a BLE data index.
  MGI_GFSK->CHANNEL_NUM0 = MG_RADIO_CHANNEL < 11 ? 44 + 2 * MG_RADIO_CHANNEL
                                                 : 46 + 2 * MG_RADIO_CHANNEL;
  MGI_GFSK->TX_POWER = 0x3f;
  MGI_GFSK->NTW_ADR_CTRL = 0x301;  // 32-bit access address, match address 0
  MGI_GFSK->NTW_ADR[0] = MG_RADIO_NETWORK;
  MGI_GFSK->PACKET_CFG = 0x000800c8;        // H0=8 bits, length=8, AA=4 bytes
  MGI_GFSK->H0_CFG = MGI_GFSK->H1_CFG = 0;  // No header filtering
  MGI_GFSK->LENGTH_ADJ = 3;  // Length excludes the three CRC bytes
  MGI_GFSK->MULT_PKT_CTRL = 0;
  MGI_GFSK->LENGTH_MAX = 16U << 16;       // 256 bytes; software caps at 251
  MGI_GFSK->ENH_FEATURE = 9 | MG_BIT(6);  // BLE uncoded, alternate rate: 2M
  MGI_GFSK->XCVR_CFG = MGI_GFSK_CFG;
  mg_mcxw71_listen();
  return true;
}

#else
#define MGI_ZLL ((struct mg_zll *) (MGI_RF_BASE + 0x1000))
#define MGI_RADIO_MAX 125

// ZLL, 802.15.4 link layer. See MCXW71 reference manual and NXP
// ieee_802_15_4 PHY sources: PhyPacketProcessor.c, PhyPlmeData.c, PhyISR.c
struct mg_zll {
  volatile uint32_t IRQSTS, PHY_CTRL, EVENT_TMR, TIMESTAMP, T1CMP, T2CMP,
      T2PRIMECMP, T3CMP, T4CMP, PA_PWR, CHANNEL_NUM0, LQI_AND_RSSI,
      MACSHORTADDRS0, MACLONGADDRS0_LSB, MACLONGADDRS0_MSB, RX_FRAME_FILTER,
      CCA_LQI_CTRL, CCA2_CTRL, RESERVED_0, DSM_CTRL, RESERVED_1, MACSHORTADDRS1,
      MACLONGADDRS1_LSB, MACLONGADDRS1_MSB, DUAL_PAN_CTRL, CHANNEL_NUM1,
      SAM_CTRL, SAM_TABLE, SAM_MATCH, SAM_FREE_IDX, SEQ_CTRL_STS, ACKDELAY,
      FILTERFAIL_CODE, RX_WTR_MARK, RESERVED_2, SLOT_PRELOAD, SEQ_STATE,
      TMR_PRESCALE, LENIENCY_LSB, LENIENCY_MSB, PART_ID, COEX_CTRL,
      COEX_PRIORITY, ENHACK_CTRL0;
};

#define MGI_TX_PB MGI_PKT_RAM           // Length byte (incl. FCS), then frame
#define MGI_RX_PB (MGI_PKT_RAM + 0x80)  // Received frame, no length byte

#define MGI_IRQSTS_SEQIRQ MG_BIT(0)
#define MGI_IRQSTS_RXIRQ MG_BIT(2)
#define MGI_IRQSTS_FILTERFAIL MG_BIT(5)  // Frame not for us
#define MGI_IRQSTS_CRCVALID MG_BIT(15)
#define MGI_IRQSTS_RX_LEN(x) (((x) >> 24) & 0x7f)  // Frame length incl. FCS
#define MGI_PHY_CTRL_XCVSEQ 7U
#define MGI_PHY_CTRL_ALL_IRQ_MASKS 0x8007ff00U  // Bits 8..18, 31: mask all IRQs
#define MGI_PHY_CTRL_SEQMSK MG_BIT(8)           // Mask the sequence end IRQ
#define MGI_PHY_CTRL_TRCV_MSK MG_BIT(31)        // Mask all transceiver IRQs
#define MGI_SEQ_STATE_MASK 0x1fU
#define MGI_SEQ_STATE_RX_WU 0x10U  // RX warm up
#define MGI_SEQ_CTRL_NO_RX_RECYCLE MG_BIT(5)
#define MGI_FRAME_FILTER 0x70bU  // Beacon, data, command; versions 0..2

// PHY_CTRL[XCVSEQ]
enum { MGI_SEQ_IDLE = 0, MGI_SEQ_RX = 1, MGI_SEQ_TX = 2 };

// Stop the current sequence, wait for the sequencer to go idle. See PhyAbort()
static void mg_mcxw71_abort(void) {
  if (MGI_ZLL->PHY_CTRL & MGI_PHY_CTRL_XCVSEQ) {
    while ((MGI_ZLL->SEQ_STATE & MGI_SEQ_STATE_MASK) == MGI_SEQ_STATE_RX_WU)
      (void) 0;
    MGI_ZLL->PHY_CTRL &= ~MGI_PHY_CTRL_XCVSEQ;
    while (MGI_ZLL->SEQ_STATE & MGI_SEQ_STATE_MASK) (void) 0;
  }
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;
}

// Start receiving, with the sequence end IRQ on, see RF_802_15_4_IRQHandler()
static void mg_mcxw71_listen(void) {
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;
  MGI_ZLL->PHY_CTRL |= MGI_SEQ_RX;
  MGI_ZLL->PHY_CTRL &= ~(MGI_PHY_CTRL_SEQMSK | MGI_PHY_CTRL_TRCV_MSK);
  MGI_NVIC_ISER[MGI_IRQN / 32] = MG_BIT(MGI_IRQN % 32);
}

// Start the TX sequence for the loaded frame. Its end: see the IRQ handler
static void mg_mcxw71_go(void) {
  MGI_ZLL->PHY_CTRL |= MGI_PHY_CTRL_TRCV_MSK | MGI_PHY_CTRL_SEQMSK;
  mg_mcxw71_abort();
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;
  MGI_ZLL->PHY_CTRL |= MGI_SEQ_TX;
  MGI_ZLL->PHY_CTRL &= ~(MGI_PHY_CTRL_SEQMSK | MGI_PHY_CTRL_TRCV_MSK);
}

void RF_802_15_4_IRQHandler(void);
void RF_802_15_4_IRQHandler(void) {
  uint32_t irqsts = MGI_ZLL->IRQSTS;
  if ((irqsts & MGI_IRQSTS_SEQIRQ) != 0) {
    mg_mcxw71_abort();
    if (s_mcxw71_txing) {
      s_mcxw71_txing = false;
      s_mcxw71_done = true;
    } else if ((irqsts & (MGI_IRQSTS_RXIRQ | MGI_IRQSTS_FILTERFAIL |
                          MGI_IRQSTS_CRCVALID)) ==
               (MGI_IRQSTS_RXIRQ | MGI_IRQSTS_CRCVALID)) {
      uint8_t frame[125];
      size_t i, n = MGI_IRQSTS_RX_LEN(irqsts);
      n = n >= 2 ? n - 2 : 0;
      if (n > 0 && n <= sizeof(frame)) {
        for (i = 0; i < n; i++) frame[i] = MGI_RX_PB[i];
        mg_radio_rx_queue(s_mcxw71_ifp, frame, n);
      }
    }
    mg_mcxw71_listen();
  }
}

static bool mgi_mcxw71_radio_init(struct mg_tcpip_if *ifp, uint16_t addr) {
  s_mcxw71_ifp = ifp;
  MGI_ZLL->SEQ_CTRL_STS |= MGI_SEQ_CTRL_NO_RX_RECYCLE;
  MGI_ZLL->PHY_CTRL = MGI_PHY_CTRL_ALL_IRQ_MASKS;
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;
  MGI_ZLL->SAM_CTRL = 0;
  MGI_ZLL->RX_FRAME_FILTER = MGI_FRAME_FILTER;
  MGI_ZLL->CHANNEL_NUM0 = MG_RADIO_CHANNEL;
  MGI_ZLL->MACSHORTADDRS0 = ((uint32_t) addr << 16) | MG_RADIO_NETWORK;
  MGI_ZLL->PA_PWR = 0x3f;
  mg_mcxw71_listen();
  return true;
}

#endif

static size_t mgi_mcxw71_radio_tx(struct mg_tcpip_if *ifp, const void *buf,
                                  size_t len) {
  size_t i;
  (void) ifp;
  if (len > MGI_RADIO_MAX || s_mcxw71_txing) return 0;
  mg_mcxw71_lock();
#if MG_RADIO == MG_RADIO_BLE
  if (MGI_GFSK->SEQ_STS & MG_BIT(11)) {  // Do not interrupt a received frame
    MGI_NVIC_ISER[MGI_IRQN / 32] = MG_BIT(MGI_IRQN % 32);
    return 0;
  }
  for (i = 0; i < 4; i++)
    MGI_TX_PB[i] = (uint8_t) (MG_RADIO_NETWORK >> (8 * i));
  MGI_TX_PB[4] = 0, MGI_TX_PB[5] = (uint8_t) len;
  for (i = 0; i < len; i++) MGI_TX_PB[i + 6] = ((const uint8_t *) buf)[i];
#else
  MGI_TX_PB[0] = (uint8_t) (len + 2);  // FCS is added by hardware
  for (i = 0; i < len; i++)
    MGI_TX_PB[i + 1] = ((const uint8_t *) buf)[i];  // 8-bit access only
#endif
  s_mcxw71_done = false;
  s_mcxw71_txing = true;
  s_mcxw71_sendstart = mg_millis();
  mg_mcxw71_go();
  MGI_NVIC_ISER[MGI_IRQN / 32] = MG_BIT(MGI_IRQN % 32);
  return len;
}

static bool mgi_mcxw71_radio_poll(struct mg_tcpip_if *ifp) {
  bool done;
  (void) ifp;
  if (s_mcxw71_txing &&
      mg_millis() > s_mcxw71_sendstart + MG_RADIO_TX_TIMEOUT_US / 1000) {
    mg_mcxw71_lock();
    if (s_mcxw71_txing) {
      mg_mcxw71_abort();
      s_mcxw71_txing = false;
      s_mcxw71_done = true;
      mg_mcxw71_listen();
    } else {
      MGI_NVIC_ISER[MGI_IRQN / 32] = MG_BIT(MGI_IRQN % 32);
    }
  }
  done = s_mcxw71_done;
  s_mcxw71_done = false;
  return done;
}

static bool mgi_mcxw71_init(struct mg_tcpip_if *ifp) {
  return mg_radio_init(ifp, mgi_mcxw71_radio_init, mgi_mcxw71_radio_poll,
                       mgi_mcxw71_radio_tx);
}

struct mg_tcpip_driver mg_tcpip_driver_mcxw71 = {mgi_mcxw71_init, mg_radio_tx,
                                                 mg_radio_rx, mg_radio_poll};
#endif
