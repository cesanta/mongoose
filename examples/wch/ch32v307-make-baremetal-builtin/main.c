// Copyright (c) 2023 Cesanta Software Limited

#include "hal.h"
#include "mongoose.h"
#include "net.h"

#define BLINK_PERIOD_MS 1000  // LED_PIN blinking period in millis

// This flash space resides at after the 0-wait 320k area
static char *s_flash_space = (char *) (0x8000000 + 320 * 1024);

bool web_load_settings(void *buf, size_t len) {
  if (*(uint32_t *) s_flash_space != SETTINGS_MAGIC) return false;
  memcpy(buf, s_flash_space, len);
  return true;
}

bool web_save_settings(void *buf, size_t len) {
  return mg_flash_write(s_flash_space, buf, len);
}

static void timer_fn(void *arg) {
  gpio_toggle(LED_PIN);                                  // Blink LED_PIN
  struct mg_tcpip_if *ifp = arg;                         // And show
  const char *names[] = {"down", "up", "req", "ready"};  // network stats
  MG_INFO(("Ethernet: %s, IP: %M, rx:%u, tx:%u, dr:%u, er:%u RAM: %lu/%lu",
           names[ifp->state], mg_print_ip4, &ifp->ip, ifp->nrecv, ifp->nsent,
           ifp->ndrop, ifp->nerr, hal_ram_used(), hal_ram_free()));
}

// https://mongoose.ws/documentation/#2-minute-integration-guide
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_http_reply(c, 200, "", "ok %p\r\n", hm);
  }
}

int main(void) {
  struct mg_mgr mgr;

  hal_init();
  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);

  MG_INFO(("Starting, CPU freq %g MHz", (double) SystemCoreClock / 1000000));

  // Print chip RAM/Flash configuration, and set to 64/256
  const char *sizes[] = {"128/192", "96/224", "64/256", "32/288"};
  uint32_t mode = (FLASH->OBR >> 8) & 3U;
  MG_INFO(("RAM/FLASH configuration: %s", sizes[mode]));
  // if (mode != 2) set_ram_size(2);

  extern char _end[], _heap_end[];
  MG_INFO(("Heap size: %lu bytes. RAM: used %lu, free %lu", _heap_end - _end,
           hal_ram_used(), hal_ram_free()));

  // Initialise Mongoose network stack
  struct mg_tcpip_driver_stm32f_data driver_data = {.mdc_cr = 1, .phy_addr = 1};
  struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(),
                            // Uncomment below for static configuration:
                            // .ip = mg_htonl(MG_U32(192, 168, 0, 223)),
                            // .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                            // .gw = mg_htonl(MG_U32(192, 168, 0, 1)),
                            .driver = &mg_tcpip_driver_stm32f,
                            .driver_data = &driver_data};
  mg_tcpip_init(&mgr, &mif);
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, &mif);

  MG_INFO(("MAC: %M. Waiting for IP...", mg_print_mac, mif.mac));
  while (mif.state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
