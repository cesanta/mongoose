// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mcu.h"
#include "mongoose.h"
#include "tusb.h"

#define LED PIN('B', 7)  // On-board LED pin (blue)
static uint64_t s_ticks;
static struct mip_if *s_ifp;
uint32_t SystemCoreClock = SYS_FREQUENCY;
const uint8_t tud_network_mac_address[6] = {2, 2, 0x84, 0x6A, 0x96, 0};

static void blink_cb(void *arg) {  // Blink periodically
  gpio_toggle(LED);
  (void) arg;
}

uint64_t mg_millis(void) {  // Declare our own uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void SysTick_Handler(void) {  // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

bool tud_network_recv_cb(const uint8_t *buf, uint16_t len) {
  mip_qwrite((void *) buf, len, s_ifp);
  // MG_INFO(("RECV %hu", len));
  // mg_hexdump(buf, len);
  tud_network_recv_renew();
  return true;
}

void tud_network_init_cb(void) {
}

void OTG_FS_IRQHandler(void) {  // USB interrupt handler
  tud_int_handler(0);           // Pass control to TinyUSB
}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg) {
  // MG_INFO(("SEND %hu", arg));
  memcpy(dst, ref, arg);
  return arg;
}

static size_t usb_tx(const void *buf, size_t len, struct mip_if *ifp) {
  if (!tud_ready()) return 0;
  while (!tud_network_can_xmit(len)) tud_task();
  tud_network_xmit((void *) buf, len);
  (void) ifp;
  return len;
}

static bool usb_up(struct mip_if *ifp) {
  (void) ifp;
  return tud_inited() && tud_ready() && tud_connected();
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/debug")) {
      int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    } else {
      mg_http_reply(c, 200, "", "hi\n");
    }
  }
  (void) fn_data;
}

int main(void) {
  clock_init();                            // Set clock
  SysTick_Config(SystemCoreClock / 1000);  // Defined in core_cm4.h
  gpio_set_mode(LED, GPIO_MODE_OUTPUT);    // Setup blue LED
  uart_init(USART3, 115200);               // It is wired to the debug port

  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the MIP interface
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);

  MG_INFO(("Init TCP/IP stack ..."));
  struct mip_driver driver = {.tx = usb_tx, .rx = mip_driver_rx, .up = usb_up};
  struct mip_if mif = {.mac = {2, 0, 1, 2, 3, 0x77},
                       .ip = mg_htonl(MG_U32(192, 168, 3, 1)),
                       .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                       .enable_dhcp_server = true,
                       .driver = &driver,
                       .queue.len = 4096};
  s_ifp = &mif;
  mip_init(&mgr, &mif);
  mg_http_listen(&mgr, "tcp://0.0.0.0:80", fn, &mgr);

  MG_INFO(("Init USB ..."));
  gpio_init(PIN('A', 11), GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 10);  // D+
  gpio_init(PIN('A', 12), GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 10);  // D-
  gpio_init(PIN('A', 9), GPIO_MODE_INPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);  // VBUS
  gpio_init(PIN('A', 10), GPIO_MODE_AF, GPIO_OTYPE_OPEN_DRAIN, GPIO_SPEED_HIGH,
            GPIO_PULL_UP, 10);                    // ID
  RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;            // Enable USB FS clock
  tusb_init();

  MG_INFO(("Init done, starting main loop ..."));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
    tud_task();
  }

  return 0;
}
