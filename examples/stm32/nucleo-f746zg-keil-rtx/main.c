// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "cmsis_os.h"
#include "hal.h"
#include "main.h"
#include "mongoose.h"
#include "net.h"

#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

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

static void server(const void *args) {
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

static void blinker(const void *args) {
  for (;;) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);  // Blink On-board blue LED
    osDelay(BLINK_PERIOD_MS);               // CMSIS-RTOS v1 uses milliseconds
  }
  (void) args;
}

extern void mx_init(void);
osThreadDef(server, osPriorityNormal, 1,
            8192);  // Create the server thread with a stack size of 8KB
osThreadDef(blinker, osPriorityNormal, 1,
            0);  // Create the blinker thread with default stack size

int main(void) {  // this is not actually baremetal main() but the "main" thread
  osKernelInitialize();  // Stop kernel
  mx_init();             // Setup clock and all peripherals configured in CubeMX
                         // Initialise random number generator
                         // Initialise ethernet pins
  test_init();  // for internal testing purposes only
  osThreadCreate(osThread(blinker), NULL);  // Create the blinker thread
  osThreadCreate(osThread(server), NULL);   // Create the server thread
  osKernelStart();  // Start kernel again and exit main thread
  return 0;
}
