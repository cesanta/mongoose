// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "main.h"
#include "net.h"

extern RNG_HandleTypeDef hrng;

#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

extern void xPortSysTickHandler(void);
void SysTick_Handler (void) {
  HAL_IncTick();
  // xPortSysTickHandler() must be called after vTaskStartScheduler() and mx_init() takes longer than 1ms 
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    xPortSysTickHandler();
}

void mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r;
    HAL_RNG_GenerateRandomNumber(&hrng, &r);
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}

static void server(void *args) {
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  MG_INFO(("Initialising application..."));
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) mg_mgr_poll(&mgr, 1000);  // Infinite event loop
  (void) args;
}

static void blinker(void *args) {
  for (;;) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);   // Blink On-board blue LED
    vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
  }
  (void) args;
}

// https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/TCP_Networking_Tutorial_Initialising_TCP.html
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t ev) {
  static bool mongoose_started = false;
  MG_INFO(("FreeRTOS-Plus-TCP net event: %d", ev));
  if (ev == eNetworkUp && mongoose_started == false) {
    uint32_t ip = FreeRTOS_GetIPAddress();
    MG_INFO(("READY, IP: %M", mg_print_ip4, &ip));
    xTaskCreate(server, "server", 2048, 0, configMAX_PRIORITIES - 1, NULL);
    mongoose_started = true;
  }
}

extern void mx_init(void);
struct hwaddr { uint8_t macaddr[6]; };

int main(void) {
  mx_init();                // Setup clock and all peripherals configured in CubeMX
                                // Initialise random number generator
                            // Initialise ethernet pins
  test_init();  // for internal testing purposes only

  uint8_t macaddr[6] = GENERATE_LOCALLY_ADMINISTERED_MAC();
  // required for fallback if DHCP fails
  static const uint8_t ipaddr[4] = {192, 168, 0, 77};	
  static const uint8_t netmask[4] = {255, 255, 255, 0};
  static const uint8_t dnsaddr[4] = {8, 8, 8, 8};
  static const uint8_t gwaddr[4] = {192, 168, 0, 1};
  FreeRTOS_IPInit(ipaddr, netmask, gwaddr, dnsaddr, macaddr);
  MG_INFO(("MAC: %M. Waiting for IP...", mg_print_mac, macaddr));

  // Start tasks. NOTE: stack sizes are in 32-bit words
  xTaskCreate(blinker, "blinker", 128, ":)", configMAX_PRIORITIES - 1, NULL);

  vTaskStartScheduler();  // This blocks
  return 0;
}

// Hooks for FreeRTOS-Plus-TCP network interface
#include "stm32f7xx_hal_conf.h"
#include "stm32fxx_hal_eth.h"
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth) {
  if (heth->Instance == ETH) {
    HAL_NVIC_SetPriority(ETH_IRQn, ipconfigMAC_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
    __HAL_RCC_ETH_CLK_ENABLE();
  }
}

uint32_t ulApplicationGetNextSequenceNumber(uint32_t a, uint16_t b, uint32_t c,
                                            uint16_t d) {
  (void) a, (void) b, (void) c, (void) d;
  uint32_t x;  
  HAL_RNG_GenerateRandomNumber(&hrng, &x);
  return x;
}

BaseType_t xApplicationGetRandomNumber(uint32_t *p) {
  HAL_RNG_GenerateRandomNumber(&hrng, p);
  return pdTRUE;
}
