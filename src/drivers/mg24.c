#include "log.h"
#include "net_builtin.h"
#include "radio.h"
#include "util.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_MG24) && MG_ENABLE_DRIVER_MG24

#include <rail.h>

// A packet in the RAIL FIFOs: the radio header, then the frame
#if MG_RADIO == MG_RADIO_BLE
#include <rail_ble.h>
#define MGI_MG24_HDR 2    // S0 byte, unused, then the frame length
#define MGI_MG24_MAX 251  // Largest frame. RAIL adds and strips the CRC
// RAIL channel: the RF channel, 2402 MHz + 2 MHz steps. Data channels 0-10
// are RF channels 1-11, data channels 11-36 are RF channels 13-38
#define MGI_MG24_CHANNEL \
  (MG_RADIO_CHANNEL < 11 ? MG_RADIO_CHANNEL + 1 : MG_RADIO_CHANNEL + 2)
#else
#include <rail_ieee802154.h>
#define MGI_MG24_HDR 1    // PHR: the frame length, FCS included
#define MGI_MG24_MAX 125  // Largest frame, without the 2 byte FCS
#define MGI_MG24_CHANNEL MG_RADIO_CHANNEL
#endif

// RAIL delivers raw frames and transmit completion from interrupt context.

static struct mg_tcpip_if *s_mg24_ifp;
static RAIL_Handle_t s_mg24_rail;
static volatile bool s_mg24_txing, s_mg24_tx_done;
static RAIL_Time_t s_mg24_txstart;  // Send start, us
static uint8_t s_mg24_txfifo[256] __attribute__((aligned(4)));  // Power of 2

// Copy a received frame, stripping the radio header and any stored CRC
static void mg_mg24_rx_frame(RAIL_Handle_t h) {
  static uint8_t f[MGI_MG24_HDR + MGI_MG24_MAX + 2];  // Interrupt use only
  RAIL_RxPacketInfo_t info;
  size_t n;
  if (RAIL_GetRxPacketInfo(h, RAIL_RX_PACKET_HANDLE_NEWEST, &info) ==
          RAIL_RX_PACKET_HANDLE_INVALID ||
      info.packetStatus != RAIL_RX_PACKET_READY_SUCCESS ||
      info.packetBytes <= MGI_MG24_HDR || info.packetBytes > sizeof(f)) {
    return;
  }
  RAIL_CopyRxPacket(f, &info);
#if MG_RADIO == MG_RADIO_BLE
  n = f[1];
  if (n + MGI_MG24_HDR > (size_t) info.packetBytes) return;
#else
  n = info.packetBytes - 1U;  // Strip PHR
  if (n == f[0]) n -= 2;      // FCS stored, strip it
#endif
  mg_radio_rx_queue(s_mg24_ifp, f + MGI_MG24_HDR, n);
}

// RAIL event callback, runs in interrupt context
static void mg_mg24_events(RAIL_Handle_t h, RAIL_Events_t events) {
  if (events & RAIL_EVENT_CAL_NEEDED) {
    RAIL_Calibrate(h, NULL, RAIL_CAL_ALL_PENDING);
  }
  if (events & RAIL_EVENT_RX_PACKET_RECEIVED) mg_mg24_rx_frame(h);
  if (events & RAIL_EVENTS_TX_COMPLETION) s_mg24_tx_done = true;
}

// RAIL adds the CRC or FCS
static size_t mgi_mg24_radio_tx(struct mg_tcpip_if *ifp, const void *buf,
                                size_t len) {
#if MG_RADIO == MG_RADIO_BLE
  uint8_t hdr[MGI_MG24_HDR] = {0, (uint8_t) len};
#else
  uint8_t hdr[MGI_MG24_HDR] = {(uint8_t) (len + 2)};  // FCS added by hardware
#endif
  (void) ifp;
  if (len > MGI_MG24_MAX || s_mg24_txing) return 0;
  RAIL_WriteTxFifo(s_mg24_rail, hdr, sizeof(hdr), true);  // Reset, write hdr
  RAIL_WriteTxFifo(s_mg24_rail, (const uint8_t *) buf, (uint16_t) len, false);
  s_mg24_tx_done = false;
  s_mg24_txing = true, s_mg24_txstart = RAIL_GetTime();
  if (RAIL_StartTx(s_mg24_rail, MGI_MG24_CHANNEL, RAIL_TX_OPTIONS_DEFAULT,
                   NULL) != RAIL_STATUS_NO_ERROR) {
    s_mg24_txing = false;
    return 0;
  }
  return len;
}

#if MG_RADIO == MG_RADIO_BLE
// Raw BLE 2 Mbit/s PHY, standard packet as in nrf52.c. The 100 us state
// transitions fit within the mesh's 300 us gap between frames
static bool mgi_mg24_protocol_init(uint16_t addr) {
  RAIL_StateTiming_t timing = {0};
  (void) addr;
  timing.idleToRx = 100, timing.idleToTx = 100;
  timing.rxToTx = 100, timing.txToRx = 100;
  return RAIL_BLE_Init(s_mg24_rail) == RAIL_STATUS_NO_ERROR &&
         RAIL_BLE_ConfigPhy2MbpsViterbi(s_mg24_rail) == RAIL_STATUS_NO_ERROR &&
         RAIL_BLE_ConfigChannelRadioParams(s_mg24_rail, 0x555555,
                                           MG_RADIO_NETWORK, MG_RADIO_CHANNEL,
                                           false) == RAIL_STATUS_NO_ERROR &&
         RAIL_SetStateTiming(s_mg24_rail, &timing) == RAIL_STATUS_NO_ERROR;
}
#else
// IEEE 802.15.4 with hardware filtering by PAN ID and our short address
static bool mgi_mg24_protocol_init(uint16_t addr) {
  RAIL_IEEE802154_Config_t cfg154 = {0};
  RAIL_StateTransitions_t to_rx = {RAIL_RF_STATE_RX, RAIL_RF_STATE_RX};
  cfg154.ackConfig.enable = false;
  cfg154.ackConfig.ackTimeout = 864;
  cfg154.ackConfig.rxTransitions = to_rx;
  cfg154.ackConfig.txTransitions = to_rx;
  cfg154.timings.idleToRx = 100, cfg154.timings.idleToTx = 100;
  cfg154.timings.rxToTx = 192, cfg154.timings.txToRx = 182;
  cfg154.framesMask = RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES;
  cfg154.promiscuousMode = false;
  return RAIL_IEEE802154_Config2p4GHzRadio(s_mg24_rail) ==
             RAIL_STATUS_NO_ERROR &&
         RAIL_IEEE802154_Init(s_mg24_rail, &cfg154) == RAIL_STATUS_NO_ERROR &&
         RAIL_IEEE802154_SetPanId(s_mg24_rail, MG_RADIO_NETWORK, 0) ==
             RAIL_STATUS_NO_ERROR &&
         RAIL_IEEE802154_SetShortAddress(s_mg24_rail, addr, 0) ==
             RAIL_STATUS_NO_ERROR;
}
#endif

static bool mgi_mg24_radio_init(struct mg_tcpip_if *ifp, uint16_t addr) {
  RAIL_Config_t cfg = {0};
  RAIL_TxPowerConfig_t pwr;
  RAIL_StateTransitions_t to_rx = {RAIL_RF_STATE_RX, RAIL_RF_STATE_RX};
  RAIL_Events_t events = RAIL_EVENT_CAL_NEEDED | RAIL_EVENT_RX_PACKET_RECEIVED |
                         RAIL_EVENTS_TX_COMPLETION;
  cfg.eventsCallback = mg_mg24_events;
  if ((s_mg24_rail = RAIL_Init(&cfg, NULL)) == NULL) {
    MG_ERROR(("RAIL_Init failed"));
    return false;
  }
  pwr.mode = RAIL_TX_POWER_MODE_2P4GIG_HP, pwr.voltage = 3300;
  pwr.rampTime = 10;
  if (RAIL_ConfigCal(s_mg24_rail, RAIL_CAL_ALL) != RAIL_STATUS_NO_ERROR ||
      RAIL_ConfigEvents(s_mg24_rail, RAIL_EVENTS_ALL, events) !=
          RAIL_STATUS_NO_ERROR ||
      !mgi_mg24_protocol_init(addr) ||
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
  return RAIL_StartRx(s_mg24_rail, MGI_MG24_CHANNEL, NULL) ==
         RAIL_STATUS_NO_ERROR;
}

static bool mgi_mg24_radio_poll(struct mg_tcpip_if *ifp) {
  bool done = false;
  (void) ifp;
  if (s_mg24_txing && (s_mg24_tx_done || RAIL_GetTime() - s_mg24_txstart >
                                             MG_RADIO_TX_TIMEOUT_US)) {
    if (!s_mg24_tx_done) {
      RAIL_Idle(s_mg24_rail, RAIL_IDLE_ABORT, true);
      RAIL_StartRx(s_mg24_rail, MGI_MG24_CHANNEL, NULL);
    }
    s_mg24_txing = false;
    done = true;
  }
  return done;
}

static bool mgi_mg24_init(struct mg_tcpip_if *ifp) {
  return mg_radio_init(ifp, mgi_mg24_radio_init, mgi_mg24_radio_poll,
                       mgi_mg24_radio_tx);
}

struct mg_tcpip_driver mg_tcpip_driver_mg24 = {mgi_mg24_init, mg_radio_tx,
                                               mg_radio_rx, mg_radio_poll};
#endif
