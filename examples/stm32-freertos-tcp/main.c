// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved

#include "device.h"
#include "mongoose.h"

static const char *s_debug_level = "2";
static const char *s_listening_address = "http://0.0.0.0:80";

// Event handler for the listening connection.
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "hello, %s!\n", "world");
  }
  (void) fn_data;
  (void) ev_data;
}

static void server(void *args) {
  struct mg_mgr mgr;
  mg_log_set(s_debug_level);
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, s_listening_address, cb, &mgr);  // Web listener
  while (args == NULL) mg_mgr_poll(&mgr, 1000);         // Infinite event loop
  mg_mgr_free(&mgr);                                    // Unreachable
}

static void blinker(void *args) {
  uint16_t pin = ((char *) args)[0] == '1' ? LED2 : LED3;
  int ms = pin == LED2 ? 750 : 1130;
  for (;;) {
    gpio_toggle(pin);
    vTaskDelay(pdMS_TO_TICKS(ms));
    LOG(LL_INFO, ("blink %s", (char *) args));
  }
}

int main(void) {
  init_hardware();
#ifdef SEMIHOSTING
  extern void initialise_monitor_handles(void);
  initialise_monitor_handles();
#endif
  xTaskCreate(server, "server", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(blinker, "blinker", 256, "1", configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(blinker, "blinker", 256, "2", configMAX_PRIORITIES - 1, NULL);
  vTaskStartScheduler();  // This blocks
  return 0;               // Unreachable
}

// Stubs for FreeRTOS-TCP network interface
uint32_t ulApplicationGetNextSequenceNumber(uint32_t a, uint16_t b, uint32_t c,
                                            uint16_t d) {
  return a ^ b ^ c ^ d;
}
BaseType_t xApplicationGetRandomNumber(uint32_t *p) {
  *p = 0;
  return 1;
}
uint32_t SystemCoreClock = 216000000;
uint32_t HAL_GetTick(void) {
  return 250;
}
uint32_t HAL_RCC_GetHCLKFreq(void) {
  return SystemCoreClock;
}
