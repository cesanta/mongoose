#include "net_builtin.h"
#include "l2_mesh.h"
#include "log.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_MG24) && MG_ENABLE_DRIVER_MG24

#include <rail.h>
#include <rail_ieee802154.h>

// RAIL delivers raw frames and transmit completion from interrupt context.
#define MG_MG24_TX_TIMEOUT_US 60000

static struct mg_tcpip_if *s_mg24_ifp;
static RAIL_Handle_t s_mg24_rail;
static struct mg_queue s_mg24_rx;
static char s_mg24_rxbuf[2048];
static volatile bool s_mg24_txing, s_mg24_tx_done;
static RAIL_Time_t s_mg24_txstart;  // Send start, us
static uint8_t s_mg24_txfifo[256] __attribute__((aligned(4)));  // Power of 2

// Copy a received PSDU, stripping the PHR and any stored FCS.
static void mg_mg24_rx_frame(RAIL_Handle_t h) {
  RAIL_RxPacketInfo_t info;
  uint8_t f[128];
  size_t n;
  char *dst;
  if (RAIL_GetRxPacketInfo(h, RAIL_RX_PACKET_HANDLE_NEWEST, &info) ==
          RAIL_RX_PACKET_HANDLE_INVALID ||
      info.packetStatus != RAIL_RX_PACKET_READY_SUCCESS ||
      info.packetBytes <= 1 || info.packetBytes > sizeof(f)) {
    return;
  }
  RAIL_CopyRxPacket(f, &info);
  n = info.packetBytes - 1U;  // Strip PHR
  if (n == f[0]) n -= 2;      // FCS stored, strip it
  if (mg_queue_book(&s_mg24_rx, &dst, n) >= n) {
    memcpy(dst, f + 1, n);
    mg_queue_add(&s_mg24_rx, n);
  } else {
    s_mg24_ifp->ndrop++;
  }
}

// RAIL event callback, runs in interrupt context
static void mg_mg24_events(RAIL_Handle_t h, RAIL_Events_t events) {
  if (events & RAIL_EVENT_CAL_NEEDED) {
    RAIL_Calibrate(h, NULL, RAIL_CAL_ALL_PENDING);
  }
  if (events & RAIL_EVENT_RX_PACKET_RECEIVED) mg_mg24_rx_frame(h);
  if (events & RAIL_EVENTS_TX_COMPLETION) s_mg24_tx_done = true;
}

// RAIL adds the FCS.
static size_t mgi_mg24_radio_tx(struct mg_tcpip_if *ifp, const void *buf,
                                size_t len) {
  uint8_t phr = (uint8_t) (len + 2);  // FCS is added by hardware
  (void) ifp;
  if (len > 125 || s_mg24_txing) return 0;
  RAIL_WriteTxFifo(s_mg24_rail, &phr, 1, true);  // Reset FIFO, write PHR
  RAIL_WriteTxFifo(s_mg24_rail, (const uint8_t *) buf, (uint16_t) len, false);
  s_mg24_tx_done = false;
  s_mg24_txing = true, s_mg24_txstart = RAIL_GetTime();
  if (RAIL_StartTx(s_mg24_rail, MG_802154_CHANNEL, RAIL_TX_OPTIONS_DEFAULT,
                   NULL) != RAIL_STATUS_NO_ERROR) {
    s_mg24_txing = false;
    return 0;
  }
  return len;
}

static bool mgi_mg24_radio_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_mg24_data *data = ifp->driver_data;
  struct mg_mesh_node *node = data->mesh;
  RAIL_Config_t cfg = {0};
  RAIL_IEEE802154_Config_t cfg154 = {0};
  RAIL_TxPowerConfig_t pwr;
  RAIL_StateTransitions_t to_rx = {RAIL_RF_STATE_RX, RAIL_RF_STATE_RX};
  RAIL_Events_t events = RAIL_EVENT_CAL_NEEDED | RAIL_EVENT_RX_PACKET_RECEIVED |
                         RAIL_EVENTS_TX_COMPLETION;
  cfg.eventsCallback = mg_mg24_events;
  if ((s_mg24_rail = RAIL_Init(&cfg, NULL)) == NULL) {
    MG_ERROR(("RAIL_Init failed"));
    return false;
  }
  cfg154.ackConfig.enable = false;
  cfg154.ackConfig.ackTimeout = 864;
  cfg154.ackConfig.rxTransitions = to_rx;
  cfg154.ackConfig.txTransitions = to_rx;
  cfg154.timings.idleToRx = 100, cfg154.timings.idleToTx = 100;
  cfg154.timings.rxToTx = 192, cfg154.timings.txToRx = 182;
  cfg154.framesMask = RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES;
  cfg154.promiscuousMode = false;  // Filter by PAN ID and our short address
  pwr.mode = RAIL_TX_POWER_MODE_2P4GIG_HP, pwr.voltage = 3300;
  pwr.rampTime = 10;
  if (RAIL_ConfigCal(s_mg24_rail, RAIL_CAL_ALL) != RAIL_STATUS_NO_ERROR ||
      RAIL_ConfigEvents(s_mg24_rail, RAIL_EVENTS_ALL, events) !=
          RAIL_STATUS_NO_ERROR ||
      RAIL_IEEE802154_Config2p4GHzRadio(s_mg24_rail) != RAIL_STATUS_NO_ERROR ||
      RAIL_IEEE802154_Init(s_mg24_rail, &cfg154) != RAIL_STATUS_NO_ERROR ||
      RAIL_IEEE802154_SetPanId(s_mg24_rail, MG_802154_PAN_ID, 0) !=
          RAIL_STATUS_NO_ERROR ||
      RAIL_IEEE802154_SetShortAddress(s_mg24_rail, node->radio_address, 0) !=
          RAIL_STATUS_NO_ERROR ||
      RAIL_SetTxTransitions(s_mg24_rail, &to_rx) != RAIL_STATUS_NO_ERROR ||
      RAIL_SetRxTransitions(s_mg24_rail, &to_rx) != RAIL_STATUS_NO_ERROR ||
      RAIL_ConfigTxPower(s_mg24_rail, &pwr) != RAIL_STATUS_NO_ERROR ||
      RAIL_SetTxPower(s_mg24_rail, RAIL_TX_POWER_LEVEL_2P4_HP_MAX) !=
          RAIL_STATUS_NO_ERROR ||
      RAIL_SetTxFifo(s_mg24_rail, s_mg24_txfifo, 0, sizeof(s_mg24_txfifo)) !=
          sizeof(s_mg24_txfifo)) {
    MG_ERROR(("RAIL setup failed"));
    return false;
  }
  s_mg24_ifp = ifp;
  return RAIL_StartRx(s_mg24_rail, MG_802154_CHANNEL, NULL) ==
         RAIL_STATUS_NO_ERROR;
}

static size_t mgi_mg24_tx(const void *buf, size_t len,
                          struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_mg24_data *data = ifp->driver_data;
  return data->mesh->strategy->tx(data->mesh, buf, len);
}

static size_t mgi_mg24_rx(void *buf, size_t cap, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_mg24_data *data = ifp->driver_data;
  size_t len = data->mesh->rx_len;
  if (len > cap) return 0;
  if (len != 0 && buf != ifp->recv_queue.buf)
    memcpy(buf, ifp->recv_queue.buf, len);
  data->mesh->rx_len = 0;
  return len;
}

static bool mgi_mg24_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct mg_tcpip_driver_mg24_data *data = ifp->driver_data;
  struct mg_mesh_node *node = data->mesh;
  uint64_t now = mg_micros();
  char *buf;
  size_t len;
  (void) s1;
  node->now = now;
  if (s_mg24_txing && (s_mg24_tx_done || RAIL_GetTime() - s_mg24_txstart >
                                             MG_MG24_TX_TIMEOUT_US)) {
    if (!s_mg24_tx_done) {
      RAIL_Idle(s_mg24_rail, RAIL_IDLE_ABORT, true);
      RAIL_StartRx(s_mg24_rail, MG_802154_CHANNEL, NULL);
    }
    s_mg24_txing = false;
    node->tx_at = now + node->strategy->radio->ifs_us;
  }
  while (node->rx_len == 0 && (len = mg_queue_next(&s_mg24_rx, &buf)) != 0) {
    node->strategy->rx(node, buf, len);
    mg_queue_del(&s_mg24_rx, len);
  }
  node->strategy->tick(node, now);
  return node->joined;
}
static bool mgi_mg24_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_mg24_data *data = ifp->driver_data;
  const struct mg_mesh *strategy = data->strategy;
  uint16_t address =
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
  data->mesh->tx = mgi_mg24_radio_tx;
  mg_queue_init(&s_mg24_rx, s_mg24_rxbuf, sizeof(s_mg24_rxbuf));
  if (mgi_mg24_radio_init(ifp)) return true;
  data->mesh->strategy->free(data->mesh);
  free(data->mesh);
  data->mesh = NULL;
  return false;
}
struct mg_tcpip_driver mg_tcpip_driver_mg24 = {mgi_mg24_init, mgi_mg24_tx,
                                               mgi_mg24_rx, mgi_mg24_poll};
#endif
