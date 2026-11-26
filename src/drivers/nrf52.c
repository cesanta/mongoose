#include "log.h"
#include "net_builtin.h"
#include "radio.h"
#include "util.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_NRF52) && MG_ENABLE_DRIVER_NRF52
#include <nrf.h>

// A packet in RAM: the radio header, then the frame
#if MG_RADIO == MG_RADIO_BLE
#define MGI_NRF52_HDR 2    // S0 byte, unused, then the frame length
#define MGI_NRF52_MAX 251  // Largest frame. The CRC is not in RAM
#define MGI_NRF52_RXSTART EVENTS_ADDRESS  // A frame is being received
#define MGI_NRF52_MHZ                                  \
  (MG_RADIO_CHANNEL < 11 ? 2404 + 2 * MG_RADIO_CHANNEL \
                         : 2406 + 2 * MG_RADIO_CHANNEL)
#else
#define MGI_NRF52_HDR 1    // PHR: the frame length, FCS included
#define MGI_NRF52_MAX 125  // Largest frame, without the 2 byte FCS
#define MGI_NRF52_RXSTART EVENTS_FRAMESTART  // A frame is being received
#endif

#define MGI_NRF52_IOLEN (MGI_NRF52_HDR + MGI_NRF52_MAX + 2)

static struct mg_tcpip_if *s_nrf52_ifp;
static uint8_t s_nrf52_rxbuf[MGI_NRF52_IOLEN] MG_8BYTE_ALIGNED;
static uint8_t s_nrf52_txbuf[MGI_NRF52_IOLEN] MG_8BYTE_ALIGNED;
static volatile bool s_nrf52_txing, s_nrf52_done;
static uint64_t s_nrf52_started;

static void mgi_nrf52_listen(void) {
  NRF_RADIO->PACKETPTR = (uint32_t) s_nrf52_rxbuf;
  NRF_RADIO->SHORTS =
      RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk;
  NRF_RADIO->MGI_NRF52_RXSTART = 0;
  NRF_RADIO->EVENTS_END = 0;
  NRF_RADIO->TASKS_RXEN = 1;
}

static void mgi_nrf52_disable(void) {
  NRF_RADIO->SHORTS = 0;
  NRF_RADIO->EVENTS_DISABLED = 0;
  NRF_RADIO->TASKS_DISABLE = 1;
  while (!NRF_RADIO->EVENTS_DISABLED) (void) 0;
  NRF_RADIO->EVENTS_DISABLED = 0;
}

void RADIO_IRQHandler(void);
void RADIO_IRQHandler(void) {
  if (!NRF_RADIO->EVENTS_DISABLED) return;
  NRF_RADIO->EVENTS_DISABLED = 0;
  if (s_nrf52_txing) {
    s_nrf52_txing = false;
    s_nrf52_done = true;
  } else if (NRF_RADIO->CRCSTATUS) {
#if MG_RADIO == MG_RADIO_BLE
    size_t len = s_nrf52_rxbuf[1];
#else
    size_t len = s_nrf52_rxbuf[0] >= 2 ? s_nrf52_rxbuf[0] - 2U : 0;
#endif
    mg_radio_rx_queue(s_nrf52_ifp, s_nrf52_rxbuf + MGI_NRF52_HDR, len);
  }
  mgi_nrf52_listen();
}
static size_t mgi_nrf52_radio_tx(struct mg_tcpip_if *ifp, const void *buf,
                                 size_t len) {
  (void) ifp;
  if (len > MGI_NRF52_MAX) return 0;
  NVIC_DisableIRQ(RADIO_IRQn);
  if (s_nrf52_txing || NRF_RADIO->MGI_NRF52_RXSTART) {
    NVIC_EnableIRQ(RADIO_IRQn);
    return 0;
  }
  mgi_nrf52_disable();
#if MG_RADIO == MG_RADIO_BLE
  s_nrf52_txbuf[0] = 0, s_nrf52_txbuf[1] = (uint8_t) len;
#else
  s_nrf52_txbuf[0] = (uint8_t) (len + 2);
#endif
  memcpy(s_nrf52_txbuf + MGI_NRF52_HDR, buf, len);
  s_nrf52_done = false;
  s_nrf52_txing = true;
  s_nrf52_started = mg_micros();
  NRF_RADIO->PACKETPTR = (uint32_t) s_nrf52_txbuf;
  NRF_RADIO->SHORTS =
      RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk;
  NRF_RADIO->TASKS_TXEN = 1;
  NVIC_EnableIRQ(RADIO_IRQn);
  return len;
}
static bool mgi_nrf52_radio_init(struct mg_tcpip_if *ifp, uint16_t addr) {
  s_nrf52_ifp = ifp;
  (void) addr;  // Frames are filtered by mg_radio_rx_queue()
  NRF_RADIO->POWER = 1;
#if MG_RADIO == MG_RADIO_BLE
  // Standard BLE packet: 2 byte preamble, access address, S0 byte, 8 bit
  // length, payload, 24 bit CRC over everything after the access address,
  // initial value 0x555555. Whitening seeded by the channel index
  NRF_RADIO->MODE = RADIO_MODE_MODE_Ble_2Mbit;
  NRF_RADIO->PCNF0 = (1UL << RADIO_PCNF0_S0LEN_Pos) |
                     (8UL << RADIO_PCNF0_LFLEN_Pos) |
                     (RADIO_PCNF0_PLEN_16bit << RADIO_PCNF0_PLEN_Pos);
  NRF_RADIO->PCNF1 = ((uint32_t) MGI_NRF52_MAX << RADIO_PCNF1_MAXLEN_Pos) |
                     (3UL << RADIO_PCNF1_BALEN_Pos) | RADIO_PCNF1_WHITEEN_Msk;
  NRF_RADIO->BASE0 = (uint32_t) MG_RADIO_NETWORK << 8;
  NRF_RADIO->PREFIX0 = ((uint32_t) MG_RADIO_NETWORK >> 24) & 0xff;
  NRF_RADIO->TXADDRESS = 0;
  NRF_RADIO->RXADDRESSES = 1;
  NRF_RADIO->CRCCNF =
      3 | (RADIO_CRCCNF_SKIPADDR_Skip << RADIO_CRCCNF_SKIPADDR_Pos);
  NRF_RADIO->CRCPOLY = 0x65b;
  NRF_RADIO->CRCINIT = 0x555555;
  NRF_RADIO->DATAWHITEIV = MG_RADIO_CHANNEL;
  NRF_RADIO->FREQUENCY = MGI_NRF52_MHZ - 2400;
  NRF_RADIO->MODECNF0 = (RADIO_MODECNF0_DTX_Center << RADIO_MODECNF0_DTX_Pos) |
                        RADIO_MODECNF0_RU_Fast;
#else
  NRF_RADIO->MODE = RADIO_MODE_MODE_Ieee802154_250Kbit;
  NRF_RADIO->PCNF0 = (8UL << RADIO_PCNF0_LFLEN_Pos) |
                     (RADIO_PCNF0_PLEN_32bitZero << RADIO_PCNF0_PLEN_Pos) |
                     RADIO_PCNF0_CRCINC_Msk;
  NRF_RADIO->PCNF1 = 127;
  NRF_RADIO->CRCCNF = 0x202;
  NRF_RADIO->CRCPOLY = 0x11021;
  NRF_RADIO->CRCINIT = 0;
  NRF_RADIO->MODECNF0 = (RADIO_MODECNF0_DTX_Center << RADIO_MODECNF0_DTX_Pos) |
                        RADIO_MODECNF0_RU_Fast;
  NRF_RADIO->SFD = 0xa7;
  NRF_RADIO->FREQUENCY = 5 * (MG_RADIO_CHANNEL - 10);
  // Correlator threshold 45 matches the RADIO reset value. A threshold of
  // 20 caused false busy detections and persistent L2 retries.
  NRF_RADIO->CCACTRL =
      (RADIO_CCACTRL_CCAMODE_CarrierMode << RADIO_CCACTRL_CCAMODE_Pos) |
      (2UL << RADIO_CCACTRL_CCACORRCNT_Pos) |
      (45UL << RADIO_CCACTRL_CCACORRTHRES_Pos);
#endif
  NRF_RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_Pos8dBm;
  NRF_RADIO->INTENSET = RADIO_INTENSET_DISABLED_Msk;
  NVIC_SetPriority(RADIO_IRQn, 0);
  NVIC_EnableIRQ(RADIO_IRQn);
  mgi_nrf52_listen();
  return true;
}

static bool mgi_nrf52_radio_poll(struct mg_tcpip_if *ifp) {
  bool done;
  (void) ifp;
  if (s_nrf52_txing && mg_micros() - s_nrf52_started > MG_RADIO_TX_TIMEOUT_US) {
    NVIC_DisableIRQ(RADIO_IRQn);
    mgi_nrf52_disable();
    s_nrf52_txing = false;
    s_nrf52_done = true;
    mgi_nrf52_listen();
    NVIC_EnableIRQ(RADIO_IRQn);
  }
  done = s_nrf52_done;
  s_nrf52_done = false;
  return done;
}

static bool mgi_nrf52_init(struct mg_tcpip_if *ifp) {
  return mg_radio_init(ifp, mgi_nrf52_radio_init, mgi_nrf52_radio_poll,
                       mgi_nrf52_radio_tx);
}

struct mg_tcpip_driver mg_tcpip_driver_nrf52 = {mgi_nrf52_init, mg_radio_tx,
                                                mg_radio_rx, mg_radio_poll};
#endif
