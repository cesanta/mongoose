#include "l2_mesh.h"
#include "log.h"
#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_MCXW71) && \
    MG_ENABLE_DRIVER_MCXW71

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
#define MGI_IRQSTS_FILTERFAIL MG_BIT(5)  // Frame not for us
#define MGI_IRQSTS_CRCVALID MG_BIT(15)
#define MGI_IRQSTS_RX_LEN(x) (((x) >> 24) & 0x7f)  // Frame length incl. FCS
#define MGI_PHY_CTRL_XCVSEQ 7U
#define MGI_PHY_CTRL_ALL_IRQ_MASKS 0x8007ff00U  // Bits 8..18, 31: mask all IRQs
#define MGI_PHY_CTRL_SEQMSK MG_BIT(8)           // Mask the sequence end IRQ
#define MGI_PHY_CTRL_TRCV_MSK MG_BIT(31)        // Mask all transceiver IRQs
#define MGI_IRQN 53                             // RF_802_15_4_IRQn
#define MGI_NVIC_ISER ((volatile uint32_t *) 0xE000E100)  // Enable IRQ
#define MGI_NVIC_ICER ((volatile uint32_t *) 0xE000E180)  // Disable IRQ
#define MGI_SEQ_STATE_MASK 0x1fU
#define MGI_SEQ_STATE_RX_WU 0x10U  // RX warm up
#define MGI_SEQ_CTRL_NO_RX_RECYCLE MG_BIT(5)
#define MGI_FRAME_FILTER 0x70bU  // Beacon, data, command; versions 0..2

// PHY_CTRL[XCVSEQ]
enum { MGI_SEQ_IDLE = 0, MGI_SEQ_RX = 1, MGI_SEQ_TX = 2 };
#define MGI_SEND_TIMEOUT_MS 60

static struct mg_tcpip_if *s_mcxw71_ifp;
static struct mg_queue s_mcxw71_rx;
static char s_mcxw71_queue[2048];
static volatile bool s_mcxw71_txing, s_mcxw71_done;
static uint64_t s_mcxw71_sendstart;

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

// Keep the IRQ handler out while we change the radio. The NVIC disable is
// atomic, so PHY_CTRL updates here and there do not race
static void mg_mcxw71_lock(void) {
  MGI_NVIC_ICER[MGI_IRQN / 32] = MG_BIT(MGI_IRQN % 32);
  __asm volatile("dsb; isb" ::: "memory");
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
        {
          char *dst;
          if (mg_queue_book(&s_mcxw71_rx, &dst, n) >= n) {
            memcpy(dst, frame, n);
            mg_queue_add(&s_mcxw71_rx, n);
          } else {
            s_mcxw71_ifp->ndrop++;
          }
        }
      }
    }
    mg_mcxw71_listen();
  }
}

static size_t mgi_mcxw71_radio_tx(struct mg_tcpip_if *ifp, const void *buf,
                                  size_t len) {
  size_t i;
  (void) ifp;
  if (len > 125 || s_mcxw71_txing) return 0;
  mg_mcxw71_lock();
  MGI_TX_PB[0] = (uint8_t) (len + 2);  // FCS is added by hardware
  for (i = 0; i < len; i++)
    MGI_TX_PB[i + 1] = ((const uint8_t *) buf)[i];  // 8-bit access only
  s_mcxw71_done = false;
  s_mcxw71_txing = true;
  s_mcxw71_sendstart = mg_millis();
  mg_mcxw71_go();
  MGI_NVIC_ISER[MGI_IRQN / 32] = MG_BIT(MGI_IRQN % 32);
  return len;
}

static bool mgi_mcxw71_radio_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_mcxw71_data *data = ifp->driver_data;
  struct mg_mesh_node *node = data->mesh;
  s_mcxw71_ifp = ifp;
  MGI_ZLL->SEQ_CTRL_STS |= MGI_SEQ_CTRL_NO_RX_RECYCLE;
  MGI_ZLL->PHY_CTRL = MGI_PHY_CTRL_ALL_IRQ_MASKS;
  MGI_ZLL->IRQSTS = MGI_ZLL->IRQSTS;
  MGI_ZLL->SAM_CTRL = 0;
  MGI_ZLL->RX_FRAME_FILTER = MGI_FRAME_FILTER;
  MGI_ZLL->CHANNEL_NUM0 = MG_802154_CHANNEL;
  MGI_ZLL->MACSHORTADDRS0 =
      ((uint32_t) node->radio_address << 16) | MG_802154_PAN_ID;
  MGI_ZLL->PA_PWR = 0x3f;
  mg_mcxw71_listen();
  return true;
}

static size_t mgi_mcxw71_tx(const void *buf, size_t len,
                            struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_mcxw71_data *data = ifp->driver_data;
  return data->mesh->strategy->tx(data->mesh, buf, len);
}

static size_t mgi_mcxw71_rx(void *buf, size_t cap, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_mcxw71_data *data = ifp->driver_data;
  size_t len = data->mesh->rx_len;
  if (len > cap) return 0;
  if (len != 0 && buf != ifp->recv_queue.buf)
    memcpy(buf, ifp->recv_queue.buf, len);
  data->mesh->rx_len = 0;
  return len;
}

static bool mgi_mcxw71_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct mg_tcpip_driver_mcxw71_data *data = ifp->driver_data;
  struct mg_mesh_node *node = data->mesh;
  uint64_t now = mg_micros();
  char *buf;
  size_t len;
  (void) s1;
  if (s_mcxw71_txing &&
      mg_millis() > s_mcxw71_sendstart + MGI_SEND_TIMEOUT_MS) {
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
  if (s_mcxw71_done) {
    s_mcxw71_done = false;
    node->tx_at = now + node->strategy->radio->ifs_us;
  }
  node->now = now;
  while (node->rx_len == 0 && (len = mg_queue_next(&s_mcxw71_rx, &buf)) != 0) {
    node->strategy->rx(node, buf, len);
    mg_queue_del(&s_mcxw71_rx, len);
  }
  node->strategy->tick(node, now);
  return node->joined;
}

static bool mgi_mcxw71_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_mcxw71_data *data = ifp->driver_data;
  const struct mg_mesh *strategy = data->strategy;
  uint16_t address;
  address =
      (uint16_t) (2 + ((unsigned) ifp->mac[4] * 256 + ifp->mac[5]) % 65532u);
  if (strategy == NULL) strategy = &mg_mesh_default;
  data->mesh = calloc(1, sizeof(*data->mesh));
  if (data->mesh == NULL) return false;
  if (!strategy->init(data->mesh, ifp, NULL, address)) {
    free(data->mesh);
    data->mesh = NULL;
    return false;
  }
  data->mesh->strategy = strategy;
  data->mesh->tx = mgi_mcxw71_radio_tx;
  mg_queue_init(&s_mcxw71_rx, s_mcxw71_queue, sizeof(s_mcxw71_queue));
  if (mgi_mcxw71_radio_init(ifp)) return true;
  strategy->free(data->mesh);
  free(data->mesh);
  data->mesh = NULL;
  return false;
}

struct mg_tcpip_driver mg_tcpip_driver_mcxw71 = {
    mgi_mcxw71_init, mgi_mcxw71_tx, mgi_mcxw71_rx, mgi_mcxw71_poll};
#endif
