// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "net.h"

#define LED1 PIN('B', 0)   // On-board LED pin (green)
#define LED2 PIN('B', 7)   // On-board LED pin (blue)
#define LED3 PIN('B', 14)  // On-board LED pin (red)

#define LED LED2              // Use blue LED for blinking
#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

uint32_t ticks;  // serve a tick counter for HAL functions
extern void xPortSysTickHandler(void);
void SysTick_Handler(void) {
  // serve ticks for HAL functions, is simpler
  ++ticks;
  // xPortSysTickHandler() must be called after vTaskStartScheduler(),
  // initialization is fast and in practice this check is not needed
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    xPortSysTickHandler();
}

void mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}

static void ethernet_init(void) {
  // Initialise Ethernet. Enable MAC GPIO pins, see
  // https://www.farnell.com/datasheets/2014265.pdf section 6.10
  uint16_t pins[] = {PIN('A', 1),  PIN('A', 2),  PIN('A', 7),
                     PIN('B', 13), PIN('C', 1),  PIN('C', 4),
                     PIN('C', 5),  PIN('G', 11), PIN('G', 13)};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    gpio_init(pins[i], GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_INSANE,
              GPIO_PULL_NONE, 11);  // 11 is the Ethernet function
  }
  NVIC_SetPriority(ETH_IRQn, ipconfigMAC_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ(ETH_IRQn);                // Setup Ethernet IRQ handler
  SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;  // Use RMII. Goes first!
  RCC->AHB1ENR |=
      RCC_AHB1ENR_ETHMACEN | RCC_AHB1ENR_ETHMACTXEN | RCC_AHB1ENR_ETHMACRXEN;
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
  gpio_init(LED, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_MEDIUM,
            GPIO_PULL_NONE, 0);
  for (;;) {
    gpio_toggle(LED);
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

int main(void) {
  uart_init(UART_DEBUG, 115200);  // Initialise UART

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
#include "stm32fxx_hal_eth.h"
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth) {
  if (heth->Instance == ETH) ethernet_init();
}

uint32_t ulApplicationGetNextSequenceNumber(uint32_t a, uint16_t b, uint32_t c,
                                            uint16_t d) {
  (void) a, (void) b, (void) c, (void) d;
  return rng_read();
}

BaseType_t xApplicationGetRandomNumber(uint32_t *p) {
  *p = rng_read();
  return pdTRUE;
}
