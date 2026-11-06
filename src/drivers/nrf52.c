#include "net_builtin.h"
#include "l2_mesh.h"
#include "log.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_NRF52) && MG_ENABLE_DRIVER_NRF52
#include <nrf.h>

static struct mg_tcpip_if *s_nrf52_ifp;
static struct mg_queue s_nrf52_rx;
static char s_nrf52_queue[2048];
static uint8_t s_nrf52_rxbuf[128] __attribute__((aligned(4)));
static uint8_t s_nrf52_txbuf[128] __attribute__((aligned(4)));
static volatile bool s_nrf52_txing, s_nrf52_done;
static uint64_t s_nrf52_started;

static void mgi_nrf52_listen(void) {
  NRF_RADIO->PACKETPTR = (uint32_t) s_nrf52_rxbuf;
  NRF_RADIO->SHORTS =
      RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk;
  NRF_RADIO->EVENTS_FRAMESTART = 0;
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
    struct mg_tcpip_driver_nrf52_data *data = s_nrf52_ifp->driver_data;
    struct mg_mesh_node *node = data->mesh;
    size_t len = s_nrf52_rxbuf[0] >= 2 ? s_nrf52_rxbuf[0] - 2U : 0;
    uint8_t *frame = s_nrf52_rxbuf + 1;
    uint16_t dst, src;
    if (len <= 125 &&
        mg_radio_802154.parse_hdr(&mg_radio_802154, frame, len, &dst, &src) !=
            0 &&
        (dst == 0xffffu || dst == node->radio_address)) {
      char *dst;
      if (mg_queue_book(&s_nrf52_rx, &dst, len) >= len) {
        memcpy(dst, frame, len);
        mg_queue_add(&s_nrf52_rx, len);
      } else {
        s_nrf52_ifp->ndrop++;
      }
    }
  }
  mgi_nrf52_listen();
}
static size_t mgi_nrf52_radio_tx(struct mg_tcpip_if *ifp, const void *buf,
                                 size_t len) {
  (void) ifp;
  if (len > 125) return 0;
  NVIC_DisableIRQ(RADIO_IRQn);
  if (s_nrf52_txing || NRF_RADIO->EVENTS_FRAMESTART) {
    NVIC_EnableIRQ(RADIO_IRQn);
    return 0;
  }
  mgi_nrf52_disable();
  s_nrf52_txbuf[0] = (uint8_t) (len + 2);
  memcpy(s_nrf52_txbuf + 1, buf, len);
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
static bool mgi_nrf52_radio_init(struct mg_tcpip_if *ifp) {
  s_nrf52_ifp = ifp;
  NRF_RADIO->POWER = 1;
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
  NRF_RADIO->FREQUENCY = 5 * (MG_802154_CHANNEL - 10);
  NRF_RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_Pos8dBm;
  // Correlator threshold 45 matches the RADIO reset value. A threshold of
  // 20 caused false busy detections and persistent L2 retries.
  NRF_RADIO->CCACTRL =
      (RADIO_CCACTRL_CCAMODE_CarrierMode << RADIO_CCACTRL_CCAMODE_Pos) |
      (2UL << RADIO_CCACTRL_CCACORRCNT_Pos) |
      (45UL << RADIO_CCACTRL_CCACORRTHRES_Pos);
  NRF_RADIO->INTENSET = RADIO_INTENSET_DISABLED_Msk;
  NVIC_SetPriority(RADIO_IRQn, 0);
  NVIC_EnableIRQ(RADIO_IRQn);
  mgi_nrf52_listen();
  return true;
}
static size_t mgi_nrf52_tx(const void *buf, size_t len,
                           struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_nrf52_data *data = ifp->driver_data;
  return data->mesh->strategy->tx(data->mesh, buf, len);
}

static size_t mgi_nrf52_rx(void *buf, size_t cap, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_nrf52_data *data = ifp->driver_data;
  size_t len = data->mesh->rx_len;
  if (len > cap) return 0;
  if (len != 0 && buf != ifp->recv_queue.buf)
    memcpy(buf, ifp->recv_queue.buf, len);
  data->mesh->rx_len = 0;
  return len;
}

static bool mgi_nrf52_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct mg_tcpip_driver_nrf52_data *data = ifp->driver_data;
  struct mg_mesh_node *node = data->mesh;
  uint64_t now = mg_micros();
  char *buf;
  size_t len;
  (void) s1;
  if (s_nrf52_txing && mg_micros() - s_nrf52_started > 60000) {
    NVIC_DisableIRQ(RADIO_IRQn);
    mgi_nrf52_disable();
    s_nrf52_txing = false;
    s_nrf52_done = true;
    mgi_nrf52_listen();
    NVIC_EnableIRQ(RADIO_IRQn);
  }
  if (s_nrf52_done) {
    s_nrf52_done = false;
    node->tx_at = now + node->strategy->radio->ifs_us;
  }
  node->now = now;
  while (node->rx_len == 0 && (len = mg_queue_next(&s_nrf52_rx, &buf)) != 0) {
    node->strategy->rx(node, buf, len);
    mg_queue_del(&s_nrf52_rx, len);
  }
  node->strategy->tick(node, now);
  return node->joined;
}

static bool mgi_nrf52_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_nrf52_data *data = ifp->driver_data;
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
  data->mesh->tx = mgi_nrf52_radio_tx;
  mg_queue_init(&s_nrf52_rx, s_nrf52_queue, sizeof(s_nrf52_queue));
  if (mgi_nrf52_radio_init(ifp)) return true;
  strategy->free(data->mesh);
  free(data->mesh);
  data->mesh = NULL;
  return false;
}

struct mg_tcpip_driver mg_tcpip_driver_nrf52 = {mgi_nrf52_init, mgi_nrf52_tx,
                                                mgi_nrf52_rx, mgi_nrf52_poll};
#endif
