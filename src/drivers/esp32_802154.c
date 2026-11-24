#include "l2_mesh.h"
#include "log.h"
#include "util.h"
#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_ESP32_802154) && \
    MG_ENABLE_DRIVER_ESP32_802154

#if MG_ARCH != MG_ARCH_ESP32
#error "MG_ENABLE_DRIVER_ESP32_802154 needs MG_ARCH_ESP32 (ESP-IDF)"
#endif

// ESP-IDF owns the PHY, coexistence and interrupts. Its callbacks run in
// interrupt context and pass radio frames through a lock-free queue, read by
// mgi_esp32_802154_poll(). In both directions frame[0] is the PHR, the length
// including FCS
#include <esp_attr.h>
#include <esp_ieee802154.h>
#include <esp_timer.h>

static struct mg_tcpip_if *s_esp32_802154_ifp;
static struct mg_queue s_esp32_802154_rx;
static char s_esp32_802154_rxbuf[2048];
static volatile bool s_esp32_802154_txing, s_esp32_802154_tx_done;
static uint64_t s_esp32_802154_txstart;  // Send start, us
// Frame for the radio: PHR, the length including FCS, then the PSDU
static uint8_t s_esp32_802154_txframe[128] __attribute__((aligned(4)));

// Queue a received PSDU without its FCS
static void mg_esp32_802154_rx_frame(const uint8_t *psdu, size_t len) {
  mg_mesh_rx_queue(&s_esp32_802154_rx, s_esp32_802154_ifp, psdu, len);
}

// ESP-IDF only: it accepts a frame and may refuse it later, because of Wi-Fi
// coexistence, a busy channel or an abort. Such a frame is sent again after
// this pause, until MG_MESH_TX_TIMEOUT_US. Without it, the mesh would count
// the refused frame as sent
#define MGI_ESP32_802154_RETRY_US 200

static volatile bool s_esp32_802154_retry;         // A resend is pending
static volatile uint32_t s_esp32_802154_retry_at;  // When to resend, us

// ESP-IDF's esp_ieee802154 driver calls the three functions below by name,
// from interrupt context. They are global only for that reason, and IRAM_ATTR
// keeps them in IRAM
void IRAM_ATTR esp_ieee802154_receive_done(uint8_t *frame,
                                           esp_ieee802154_frame_info_t *info) {
  (void) info;
  if (frame[0] > 2) mg_esp32_802154_rx_frame(frame + 1, frame[0] - 2U);
  esp_ieee802154_receive_handle_done(frame);
}

void IRAM_ATTR esp_ieee802154_transmit_done(const uint8_t *frame,
                                            const uint8_t *ack,
                                            esp_ieee802154_frame_info_t *info) {
  (void) frame, (void) info;
  if (ack != NULL) esp_ieee802154_receive_handle_done(ack);
  s_esp32_802154_tx_done = true;
}

void IRAM_ATTR esp_ieee802154_transmit_failed(const uint8_t *frame,
                                              esp_ieee802154_tx_error_t err) {
  (void) frame;
  if (err == ESP_IEEE802154_TX_ERR_COEXIST ||
      err == ESP_IEEE802154_TX_ERR_CCA_BUSY ||
      err == ESP_IEEE802154_TX_ERR_ABORT) {
    s_esp32_802154_retry_at =
        (uint32_t) esp_timer_get_time() + MGI_ESP32_802154_RETRY_US;
    s_esp32_802154_retry = true;  // Set last: the poll reads it first
  } else {
    s_esp32_802154_tx_done = true;
  }
}

static bool mg_esp32_802154_hw_init(uint16_t address) {
  // rx_when_idle: the driver returns to receive after each transmission
  return esp_ieee802154_enable() == ESP_OK &&
         esp_ieee802154_set_promiscuous(false) == ESP_OK &&
         esp_ieee802154_set_coordinator(false) == ESP_OK &&
         esp_ieee802154_set_rx_when_idle(true) == ESP_OK &&
         esp_ieee802154_set_panid(MG_802154_PAN_ID) == ESP_OK &&
         esp_ieee802154_set_short_address(address) == ESP_OK &&
         esp_ieee802154_set_channel(MG_802154_CHANNEL) == ESP_OK &&
         esp_ieee802154_set_txpower(0) == ESP_OK &&
         esp_ieee802154_receive() == ESP_OK;
}

static bool mg_esp32_802154_hw_tx(void) {
  s_esp32_802154_retry = false;
  return esp_ieee802154_transmit(s_esp32_802154_txframe, false) == ESP_OK;
}

static void mg_esp32_802154_hw_abort(void) {
  s_esp32_802154_retry = false;
  esp_ieee802154_receive();  // Stops the current operation
}

// Resend a refused frame when its pause is over. If the radio does not take
// it, give up: the frame counts as done
static void mg_esp32_802154_hw_poll(void) {
  uint32_t now;
  if (!s_esp32_802154_retry) return;
  now = (uint32_t) esp_timer_get_time();
  if ((int32_t) (now - s_esp32_802154_retry_at) < 0) return;
  s_esp32_802154_retry = false;
  if (esp_ieee802154_transmit(s_esp32_802154_txframe, false) != ESP_OK) {
    s_esp32_802154_tx_done = true;
  }
}

// Mesh radio transmit. The hardware adds the FCS
static size_t mgi_esp32_802154_radio_tx(struct mg_tcpip_if *ifp,
                                        const void *buf, size_t len) {
  (void) ifp;
  if (len > 125 || s_esp32_802154_txing) return 0;
  s_esp32_802154_txframe[0] = (uint8_t) (len + 2);
  memcpy(s_esp32_802154_txframe + 1, buf, len);
  s_esp32_802154_tx_done = false;
  s_esp32_802154_txing = true, s_esp32_802154_txstart = mg_micros();
  if (!mg_esp32_802154_hw_tx()) {
    s_esp32_802154_txing = false;
    return 0;
  }
  return len;
}

static size_t mgi_esp32_802154_tx(const void *buf, size_t len,
                                  struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_esp32_802154_data *data = ifp->driver_data;
  return data->mesh->strategy->tx(data->mesh, buf, len);
}

static size_t mgi_esp32_802154_rx(void *buf, size_t cap,
                                  struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_esp32_802154_data *data = ifp->driver_data;
  size_t len = data->mesh->rx_len;
  if (len > cap) return 0;
  if (len != 0 && buf != ifp->recv_queue.buf)
    memcpy(buf, ifp->recv_queue.buf, len);
  data->mesh->rx_len = 0;
  return len;
}

static bool mgi_esp32_802154_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct mg_tcpip_driver_esp32_802154_data *data = ifp->driver_data;
  struct mg_mesh_node *node = data->mesh;
  uint64_t now = mg_micros();
  (void) s1;
  node->now = now;
  mg_esp32_802154_hw_poll();
  if (s_esp32_802154_txing &&
      (s_esp32_802154_tx_done ||
       now - s_esp32_802154_txstart > MG_MESH_TX_TIMEOUT_US)) {
    if (!s_esp32_802154_tx_done) mg_esp32_802154_hw_abort();
    s_esp32_802154_txing = false;
    node->tx_at = now + node->strategy->radio->ifs_us;
  }
  mg_mesh_rx_drain(node, &s_esp32_802154_rx);
  node->strategy->tick(node, now);
  return node->joined;
}

static bool mgi_esp32_802154_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_esp32_802154_data *data = ifp->driver_data;
  const struct mg_mesh *strategy = data->strategy;
  uint16_t address =
      (uint16_t) (2 + ((unsigned) ifp->mac[4] * 256 + ifp->mac[5]) % 65532u);
  if (strategy == NULL) strategy = &mg_mesh_default;
  data->mesh = mg_calloc(1, sizeof(*data->mesh));
  if (data->mesh == NULL) return false;
  if (!strategy->init(data->mesh, ifp, NULL, address)) {
    mg_free(data->mesh);
    data->mesh = NULL;
    return false;
  }
  data->mesh->strategy = strategy;
  data->mesh->tx = mgi_esp32_802154_radio_tx;
  mg_queue_init(&s_esp32_802154_rx, s_esp32_802154_rxbuf,
                sizeof(s_esp32_802154_rxbuf));
  s_esp32_802154_ifp = ifp;
  if (mg_esp32_802154_hw_init(address)) return true;
  MG_ERROR(("802.15.4 radio init failed"));
  data->mesh->strategy->free(data->mesh);
  mg_free(data->mesh);
  data->mesh = NULL;
  return false;
}

struct mg_tcpip_driver mg_tcpip_driver_esp32_802154 = {
    mgi_esp32_802154_init, mgi_esp32_802154_tx, mgi_esp32_802154_rx,
    mgi_esp32_802154_poll};
#endif
