#include "hal.h"
#include "tusb.h"
extern void tusb_hal_nrf_power_event(uint32_t event);
void USBD_IRQHandler(void) {
  tud_int_handler(0);
}
void POWER_CLOCK_IRQHandler(void) {
  if (NRF_POWER->EVENTS_USBDETECTED) {
    NRF_POWER->EVENTS_USBDETECTED = 0;
    tusb_hal_nrf_power_event(0);
  }
  if (NRF_POWER->EVENTS_USBREMOVED) {
    NRF_POWER->EVENTS_USBREMOVED = 0;
    tusb_hal_nrf_power_event(1);
  }
  if (NRF_POWER->EVENTS_USBPWRRDY) {
    NRF_POWER->EVENTS_USBPWRRDY = 0;
    tusb_hal_nrf_power_event(2);
  }
}
void usb_init(void) {
  NRF_POWER->INTENSET = POWER_INTENSET_USBDETECTED_Msk |
                        POWER_INTENSET_USBREMOVED_Msk |
                        POWER_INTENSET_USBPWRRDY_Msk;
  NVIC_SetPriority(POWER_CLOCK_IRQn, 3);
  NVIC_SetPriority(USBD_IRQn, 3);
  if (NRF_POWER->USBREGSTATUS & POWER_USBREGSTATUS_VBUSDETECT_Msk)
    tusb_hal_nrf_power_event(0);
  if (NRF_POWER->USBREGSTATUS & POWER_USBREGSTATUS_OUTPUTRDY_Msk)
    tusb_hal_nrf_power_event(2);
  NRF_POWER->EVENTS_USBDETECTED = 0;
  NRF_POWER->EVENTS_USBPWRRDY = 0;
  NRF_POWER->EVENTS_USBREMOVED = 0;
  NVIC_ClearPendingIRQ(POWER_CLOCK_IRQn);
  tusb_init();
  NVIC_EnableIRQ(POWER_CLOCK_IRQn);
}
void usb_poll(void) {
  tud_task();
  tud_cdc_write_flush();
}
void usb_log(char ch, void *param) {
  (void) param;
  if (tud_cdc_connected()) tud_cdc_write_char(ch);
}
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
  cdc_line_coding_t lc;
  (void) rts;
  tud_cdc_n_get_line_coding(itf, &lc);
  if (!dtr && lc.bit_rate == 1200) {
    NRF_POWER->GPREGRET = 0x57;
    NVIC_SystemReset();
  }
}
static const tusb_desc_device_t s_dev = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = 64,
    .idVendor = 0xcafe,
    .idProduct = 0x4001,
    .bcdDevice = 0x0100,
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 3,
    .bNumConfigurations = 1};
uint8_t const *tud_descriptor_device_cb(void) {
  return (const uint8_t *) &s_dev;
}
static const uint8_t s_cfg[] = {
    TUD_CONFIG_DESCRIPTOR(1, 2, 0, TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN, 0,
                          100),
    TUD_CDC_DESCRIPTOR(0, 0, 0x81, 8, 0x02, 0x82, 64)};
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
  (void) index;
  return s_cfg;
}
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  static uint16_t buf[32];
  const char *strings[] = {"", "Cesanta", "XIAO nRF52840", "mongoose-nrf52840"};
  size_t n = 0;
  (void) langid;
  if (index == 0) {
    buf[1] = 0x0409;
    n = 1;
  } else if (index < 4) {
    const char *s = strings[index];
    while (s[n] && n < 31) {
      buf[n + 1] = s[n];
      n++;
    }
  } else
    return NULL;
  buf[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * n + 2));
  return buf;
}
