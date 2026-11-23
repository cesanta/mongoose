#include "log.h"
#include "net_builtin.h"
#include "radio.h"
#include "util.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_ESP32) && MG_ENABLE_DRIVER_ESP32

#if MG_RADIO != MG_RADIO_802154
#error "MG_ENABLE_DRIVER_ESP32 supports MG_RADIO_802154 only"
#endif

#if MG_ARCH != MG_ARCH_ESP32
#error "MG_ENABLE_DRIVER_ESP32 needs MG_ARCH_ESP32 (ESP-IDF)"
#endif

// ESP-IDF owns the PHY, coexistence and interrupts. Its callbacks run in
// interrupt context and pass radio frames through a lock-free queue, read by
// mgi_esp32_poll(). In both directions frame[0] is the PHR, the length
// including FCS
#include <esp_attr.h>
#include <esp_ieee802154.h>
#include <esp_timer.h>

static struct mg_tcpip_if *s_esp32_ifp;
static volatile bool s_esp32_txing, s_esp32_tx_done;
static uint64_t s_esp32_txstart;  // Send start, us
// Frame for the radio: PHR, the length including FCS, then the PSDU
static uint8_t s_esp32_txframe[128] __attribute__((aligned(4)));

// ESP-IDF only: it accepts a frame and may refuse it later, because of Wi-Fi
// coexistence, a busy channel or an abort. Such a frame is sent again after
// this pause, until MG_RADIO_TX_TIMEOUT_US. Without it, the mesh would count
// the refused frame as sent
#define MGI_ESP32_RETRY_US 200

static volatile bool s_esp32_retry;         // A resend is pending
static volatile uint32_t s_esp32_retry_at;  // When to resend, us

// ESP-IDF's esp_ieee802154 driver calls the three functions below by name,
// from interrupt context. They are global only for that reason, and IRAM_ATTR
// keeps them in IRAM
void IRAM_ATTR esp_ieee802154_receive_done(uint8_t *frame,
                                           esp_ieee802154_frame_info_t *info) {
  (void) info;
  if (frame[0] > 2) mg_radio_rx_queue(s_esp32_ifp, frame + 1, frame[0] - 2U);
  esp_ieee802154_receive_handle_done(frame);
}

void IRAM_ATTR esp_ieee802154_transmit_done(const uint8_t *frame,
                                            const uint8_t *ack,
                                            esp_ieee802154_frame_info_t *info) {
  (void) frame, (void) info;
  if (ack != NULL) esp_ieee802154_receive_handle_done(ack);
  s_esp32_tx_done = true;
}

void IRAM_ATTR esp_ieee802154_transmit_failed(const uint8_t *frame,
                                              esp_ieee802154_tx_error_t err) {
  (void) frame;
  if (err == ESP_IEEE802154_TX_ERR_COEXIST ||
      err == ESP_IEEE802154_TX_ERR_CCA_BUSY ||
      err == ESP_IEEE802154_TX_ERR_ABORT) {
    s_esp32_retry_at = (uint32_t) esp_timer_get_time() + MGI_ESP32_RETRY_US;
    s_esp32_retry = true;  // Set last: the poll reads it first
  } else {
    s_esp32_tx_done = true;
  }
}

// rx_when_idle: the driver returns to receive after each transmission
static bool mgi_esp32_radio_init(struct mg_tcpip_if *ifp, uint16_t addr) {
  s_esp32_ifp = ifp;
  if (esp_ieee802154_enable() == ESP_OK &&
      esp_ieee802154_set_promiscuous(false) == ESP_OK &&
      esp_ieee802154_set_coordinator(false) == ESP_OK &&
      esp_ieee802154_set_rx_when_idle(true) == ESP_OK &&
      esp_ieee802154_set_panid(MG_RADIO_NETWORK) == ESP_OK &&
      esp_ieee802154_set_short_address(addr) == ESP_OK &&
      esp_ieee802154_set_channel(MG_RADIO_CHANNEL) == ESP_OK &&
      esp_ieee802154_set_txpower(0) == ESP_OK &&
      esp_ieee802154_receive() == ESP_OK) {
    return true;
  }
  MG_ERROR(("802.15.4 radio init failed"));
  return false;
}

static bool mg_esp32_hw_tx(void) {
  s_esp32_retry = false;
  return esp_ieee802154_transmit(s_esp32_txframe, false) == ESP_OK;
}

static void mg_esp32_hw_abort(void) {
  s_esp32_retry = false;
  esp_ieee802154_receive();  // Stops the current operation
}

// Resend a refused frame when its pause is over. If the radio does not take
// it, give up: the frame counts as done
static void mg_esp32_hw_poll(void) {
  uint32_t now;
  if (!s_esp32_retry) return;
  now = (uint32_t) esp_timer_get_time();
  if ((int32_t) (now - s_esp32_retry_at) < 0) return;
  s_esp32_retry = false;
  if (esp_ieee802154_transmit(s_esp32_txframe, false) != ESP_OK) {
    s_esp32_tx_done = true;
  }
}

// Mesh radio transmit. The hardware adds the FCS
static size_t mgi_esp32_radio_tx(struct mg_tcpip_if *ifp, const void *buf,
                                 size_t len) {
  (void) ifp;
  if (len > 125 || s_esp32_txing) return 0;
  s_esp32_txframe[0] = (uint8_t) (len + 2);
  memcpy(s_esp32_txframe + 1, buf, len);
  s_esp32_tx_done = false;
  s_esp32_txing = true, s_esp32_txstart = mg_micros();
  if (!mg_esp32_hw_tx()) {
    s_esp32_txing = false;
    return 0;
  }
  return len;
}

static bool mgi_esp32_radio_poll(struct mg_tcpip_if *ifp) {
  bool done = false;
  (void) ifp;
  mg_esp32_hw_poll();
  if (s_esp32_txing && (s_esp32_tx_done || mg_micros() - s_esp32_txstart >
                                               MG_RADIO_TX_TIMEOUT_US)) {
    if (!s_esp32_tx_done) mg_esp32_hw_abort();
    s_esp32_txing = false;
    done = true;
  }
  return done;
}

static bool mgi_esp32_init(struct mg_tcpip_if *ifp) {
  return mg_radio_init(ifp, mgi_esp32_radio_init, mgi_esp32_radio_poll,
                       mgi_esp32_radio_tx);
}

struct mg_tcpip_driver mg_tcpip_driver_esp32 = {mgi_esp32_init, mg_radio_tx,
                                                mg_radio_rx, mg_radio_poll};
#endif
