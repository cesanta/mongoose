// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "main.h"
#include "net.h"
#include "cmsis_os2.h" 

#define BLINK_PERIOD_MS 1000  // LED blinking period in millis


void mg_random(void *buf, size_t len) {  // Use on-board RNG
  extern RNG_HandleTypeDef hrng;
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
    osDelay((osKernelGetTickFreq() * BLINK_PERIOD_MS) / 1000U);
  }
  (void) args;
}

osThreadId_t s_am;

// see rl_net_ds.h
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {
  if (option == NET_DHCP_OPTION_IP_ADDRESS) osThreadFlagsSet(s_am, 1<<0);
  (void)if_num, (void)val, (void)len;
}

__NO_RETURN static void app_main (void *args) { 
  uint8_t ipaddr[NET_ADDR_IP4_LEN];
  char ip[40];

  netInitialize();
  MG_INFO(("Waiting for IP..."));
  osThreadFlagsWait (1<<0, osFlagsWaitAny, osWaitForever); // wait for DHCP
  osThreadFlagsClear(1<<0);
  netIF_GetOption (NET_IF_CLASS_ETH, netIF_OptionIP4_Address, ipaddr, sizeof(ipaddr));
  netIP_ntoa (NET_ADDR_IP4, ipaddr, ip, sizeof(ip));
  MG_INFO(("READY, IP: %s", ip));

  const osThreadAttr_t server_attr = {
    .stack_size = 8192                            // Create the server thread with a stack size of 8KB
  };
  osThreadNew(server, NULL, &server_attr);
  osThreadTerminate(osThreadGetId());
  for(;;);
  (void)args;
}

extern void mx_init(void);

int main(void) {
  mx_init();                // Setup clock and all peripherals configured in CubeMX
                                // Initialise random number generator
                            // Initialise ethernet pins
  test_init();  // for internal testing purposes only

  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(blinker, NULL, NULL);  // Create the blinker thread with a default stack size
  s_am = osThreadNew(app_main, NULL, NULL);  // Create the thread that will start networking, use a default stack size
  osKernelStart();  // This blocks     
  return 0;
}

