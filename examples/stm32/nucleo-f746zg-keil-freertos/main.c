// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "main.h"
#include "mongoose.h"
#include "net.h"

#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

extern void xPortSysTickHandler(void);
void SysTick_Handler(void) {
  HAL_IncTick();
  // xPortSysTickHandler() must be called after vTaskStartScheduler() and
  // mx_init() takes longer than 1ms
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    xPortSysTickHandler();
}

void mg_random(void *buf, size_t len) {  // Use on-board RNG
  extern RNG_HandleTypeDef hrng;
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r;
    HAL_RNG_GenerateRandomNumber(&hrng, &r);
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}

static void timer_fn(void *arg) {
  struct mg_tcpip_if *ifp = arg;                         // And show
  const char *names[] = {"down", "up", "req", "ready"};  // network stats
  MG_INFO(("Ethernet: %s, IP: %M, rx:%u, tx:%u, dr:%u, er:%u",
           names[ifp->state], mg_print_ip4, &ifp->ip, ifp->nrecv, ifp->nsent,
           ifp->ndrop, ifp->nerr));
}

static void server(void *args) {
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise Mongoose network stack
  struct mg_tcpip_driver_stm32_data driver_data = {.mdc_cr = 4};
  struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(),
                            // Uncomment below for static configuration:
                            // .ip = mg_htonl(MG_U32(192, 168, 0, 223)),
                            // .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                            // .gw = mg_htonl(MG_U32(192, 168, 0, 1)),
                            .driver = &mg_tcpip_driver_stm32,
                            .driver_data = &driver_data};
  mg_tcpip_init(&mgr, &mif);
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, &mif);

  MG_INFO(("MAC: %M. Waiting for IP...", mg_print_mac, mif.mac));
  while (mif.state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) mg_mgr_poll(&mgr, 1);  // Infinite event loop
  (void) args;
}

static void blinker(void *args) {
  for (;;) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);  // Blink On-board blue LED
    vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
  }
  (void) args;
}

extern void mx_init(void);

int main(void) {
  mx_init();  // Setup clock and all peripherals configured in CubeMX
              // Initialise random number generator
              // Initialise ethernet pins
  test_init();  // for internal testing purposes only
  // Start tasks. NOTE: stack sizes are in 32-bit words
  xTaskCreate(blinker, "blinker", 128, ":)", configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(server, "server", 2048, 0, configMAX_PRIORITIES - 1, NULL);

  vTaskStartScheduler();  // This blocks
  return 0;
}
