// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "tusb.h"

#define LED1 PIN('N', 1)  // On-board LED pin
#define LED2 PIN('N', 0)  // On-board LED pin
#define LED3 PIN('F', 4)  // On-board LED pin
#define LED4 PIN('F', 0)  // On-board LED pin

static struct mg_tcpip_if *s_ifp;
const uint8_t tud_network_mac_address[6] = {2, 2, 0x84, 0x6A, 0x96, 0};

static void blink_cb(void *arg) {  // Blink periodically
  gpio_toggle(LED1);
  (void) arg;
}

static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

bool tud_network_recv_cb(const uint8_t *buf, uint16_t len) {
  mg_tcpip_qwrite((void *) buf, len, s_ifp);
  // MG_INFO(("RECV %hu", len));
  // mg_hexdump(buf, len);
  tud_network_recv_renew();
  return true;
}

void tud_network_init_cb(void) {
}

void USB0_Handler(void) {  // USB interrupt handler
  tud_int_handler(0);      // Pass control to TinyUSB
}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg) {
  // MG_INFO(("SEND %hu", arg));
  memcpy(dst, ref, arg);
  return arg;
}

static size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  if (!tud_ready()) return 0;
  while (!tud_network_can_xmit(len)) tud_task();
  tud_network_xmit((void *) buf, len);
  (void) ifp;
  return len;
}

static bool usb_up(struct mg_tcpip_if *ifp) {
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
  gpio_output(LED1);              // Setup blue LED
  uart_init(UART_DEBUG, 115200);  // Initialise debug printf

  struct mg_mgr mgr;        // Initialise
  mg_mgr_init(&mgr);        // Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level

  MG_INFO(("Init TCP/IP stack ..."));
  struct mg_tcpip_driver driver = {.tx = usb_tx, .up = usb_up};
  struct mg_tcpip_if mif = {.mac = {2, 0, 1, 2, 3, 0x77},
                            .ip = mg_htonl(MG_U32(192, 168, 3, 1)),
                            .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                            .enable_dhcp_server = true,
                            .driver = &driver,
                            .recv_queue.size = 4096};
  s_ifp = &mif;
  mg_tcpip_init(&mgr, &mif);
  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);
  mg_http_listen(&mgr, "tcp://0.0.0.0:80", fn, &mgr);

  MG_INFO(("Init USB ..."));
  gpio_init(PIN('L', 6), GPIO_MODE_ANALOG, GPIO_OTYPE_PUSH_PULL,
            GPIO_SPEED_HIGH, GPIO_PULL_NONE, 0);  // D_P
  gpio_init(PIN('L', 7), GPIO_MODE_ANALOG, GPIO_OTYPE_PUSH_PULL,
            GPIO_SPEED_HIGH, GPIO_PULL_NONE, 0);  // D_N
  gpio_init(PIN('B', 1), GPIO_MODE_ANALOG, GPIO_OTYPE_PUSH_PULL,
            GPIO_SPEED_HIGH, GPIO_PULL_NONE, 0);  // VBUS
  gpio_init(PIN('B', 0), GPIO_MODE_ANALOG, GPIO_OTYPE_PUSH_PULL,
            GPIO_SPEED_HIGH, GPIO_PULL_NONE, 0);  // ID
  SYSCTL->RCGCUSB |= BIT(0);                      // Enable USB clock
  SYSCTL->SRUSB |= BIT(0);                        // Reset peripheral
  SYSCTL->SRUSB &= ~BIT(0);
  USB0->CC = 0x207;  // Enable clock and configure divisor for 60MHz clock
  USB0->GPCS = 3;
  USB0->LPMCNTRL &= ~BIT(0);
  USB0->LPMCNTRL = 0;
  MG_INFO(("USB Controller version %d", 1 + (USB0->PP & 0xF)));
  MG_INFO(("IS: %x", USB0->IS));
  if (USB0->EPCISC & BIT(0))  // check PF flag,
    USB0->EPCISC |= BIT(0);   // clear PF interrupt if so

  tusb_init();

  MG_INFO(("Init done, starting main loop ..."));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
    tud_task();
  }

  return 0;
}
