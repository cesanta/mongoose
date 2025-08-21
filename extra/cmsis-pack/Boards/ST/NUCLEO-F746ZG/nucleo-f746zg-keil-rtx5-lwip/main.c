// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "net.h"
#include "main.h"
#include "cmsis_os2.h" 
#include "ethernetif.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"

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


static void netw(void *args) {
  struct netif *netif = (struct netif *) args;
  for (;;) {
    ethernetif_check_link(netif);
    ethernetif_poll(netif);
  }
}

static void netw_init (struct netif *netif) {
  ip4_addr_t ipaddr, netmask, gw;
  tcpip_init(NULL, NULL);
  ipaddr.addr = IPADDR_ANY;  // otherwise, set your static configuration
  netmask.addr = IPADDR_ANY;
  gw.addr = IPADDR_ANY;
  netif_add(netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
  netif_set_default(netif);
  netif_set_up(netif);
  dhcp_start(netif);  // remove for static configuration
}

static struct netif s_netif;

__NO_RETURN static void app_main (void *args) { 
  netw_init(&s_netif);
  osThreadNew(netw, &s_netif, NULL);  // Create the Ethernet link/rx thread with a default stack size
  MG_INFO(("Waiting for IP..."));
  while(ip4_addr_isany_val(*netif_ip4_addr(&s_netif)))
    osDelay((osKernelGetTickFreq() * 200U) / 1000U);
  MG_INFO(("READY, IP: %s", ip4addr_ntoa(netif_ip4_addr(&s_netif))));
  
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
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(blinker, NULL, NULL);  // Create the blinker thread with a default stack size
  osThreadNew(app_main, NULL, NULL);  // Create the thread that will start networking, use a default stack size
  osKernelStart();  // This blocks     
  return 0;
}

