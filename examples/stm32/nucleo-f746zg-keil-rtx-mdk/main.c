// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "main.h"
#include "net.h"
#include "cmsis_os.h" 

#define BLINK_PERIOD_MS 1000  // LED blinking period in millis


void mg_random(void *buf, size_t len) {  // Use on-board RNG
  extern RNG_HandleTypeDef hrng;
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r;
    HAL_RNG_GenerateRandomNumber(&hrng, &r);
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}

static void server(const void *args) {
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  MG_INFO(("Initialising application..."));
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) mg_mgr_poll(&mgr, 1000);  // Infinite event loop
  (void) args;
}

static void blinker(const void *args) {
  for (;;) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);   // Blink On-board blue LED
    osDelay(BLINK_PERIOD_MS); // CMSIS-RTOS v1 uses milliseconds
  }
  (void) args;
}

osThreadDef(server, osPriorityNormal, 1, 8192); // Create the server thread with a stack size of 8KB

osThreadId s_am;

// see rl_net_ds.h
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {
  if (option == NET_DHCP_OPTION_IP_ADDRESS) osSignalSet(s_am, 1<<0);
  (void)if_num, (void)val, (void)len;
}

static void app_main (const void *args) { 
  uint8_t ipaddr[NET_ADDR_IP4_LEN];
  char ip[40];
  netInitialize();
  MG_INFO(("Waiting for IP..."));
  osSignalWait(1<<0, osWaitForever); // wait for DHCP
  osSignalClear(s_am, 1<<0);
  netIF_GetOption (NET_IF_CLASS_ETH, netIF_OptionIP4_Address, ipaddr, sizeof(ipaddr));
  netIP_ntoa (NET_ADDR_IP4, ipaddr, ip, sizeof(ip));
  MG_INFO(("READY, IP: %s", ip));
  osThreadCreate(osThread(server), NULL);  // Create the server thread
  osThreadTerminate(osThreadGetId());
  for(;;);
  (void)args;
}

osThreadDef(blinker, osPriorityNormal, 1, 0); // Create the blinker thread with default stack size
osThreadDef(app_main, osPriorityNormal, 1, 1024); // Create the thread that will start networking with a stack size of 1KB

extern void mx_init(void);
 
int main(void) {		// this is not actually baremetal main() but the "main" thread
  osKernelInitialize();                 // Stop kernel
  mx_init();                // Setup clock and all peripherals configured in CubeMX
                                // Initialise random number generator
                            // Initialise ethernet pins
  test_init();  // for internal testing purposes only

  osThreadCreate(osThread(blinker), NULL);  // Create the blinker thread
  s_am = osThreadCreate(osThread(app_main), NULL);  // Create the network start thread
  osKernelStart();  // Start kernel again and exit main thread
  return 0;
}
