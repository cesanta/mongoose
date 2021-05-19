// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved

#include "device.h"
#include "mongoose.h"

static const char *s_debug_level = "4";
static const char *s_listening_address = "http://0.0.0.0:80";

// Event handler for the listening connection.
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
#if MG_ENABLE_FS
    struct mg_http_serve_opts opts = {.root_dir = "/"};
    mg_http_serve_dir(c, ev_data, &opts);
#else
    mg_http_reply(c, 200, "", "hello, %s!\n", "world");
#endif
  }
  (void) fn_data;
  (void) ev_data;
}

static void server(void *args) {
  struct mg_mgr mgr;
  mg_log_set(s_debug_level);
  mg_mgr_init(&mgr);
  LOG(LL_INFO, ("Starting Mongoose v%s", MG_VERSION));  // Tell the world
  mg_http_listen(&mgr, s_listening_address, cb, &mgr);  // Web listener
  while (args == NULL) mg_mgr_poll(&mgr, 1000);         // Infinite event loop
  mg_mgr_free(&mgr);                                    // Unreachable
}

static void blinker(void *args) {
  uint16_t pin = ((char *) args)[0] == '1' ? LED2 : LED3;
  int ms = pin == LED2 ? 3750 : 5130;
  for (;;) {
    gpio_toggle(pin);
    vTaskDelay(pdMS_TO_TICKS(ms));
    LOG(LL_INFO, ("blink %s,  RAM: %u", (char *) args, xPortGetFreeHeapSize()));
  }
}

// Start Mongoose server when network is ready
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t ev) {
  static bool mongoose_started = false;
  LOG(LL_INFO, ("FreeRTOS net event %d, up: %d", ev, eNetworkUp));
  if (ev == eNetworkUp && mongoose_started == false) {
    xTaskCreate(server, "server", 8192, NULL, configMAX_PRIORITIES - 1, NULL);
    mongoose_started = true;
  }
}

static void init_heap(void) {
  extern uint32_t _end, _estack;
  uint8_t *ptr = (uint8_t *) ((((uint32_t) &_end) + 7) & ~7U);
  uint32_t len = (uint32_t)((char *) &_estack - (char *) &_end);
  HeapRegion_t regions[] = {{ptr, len}, {NULL, 0}};
  vPortDefineHeapRegions(regions);
}

int main(void) {
  init_hardware();
  init_heap();

  // Initialise networking task. Can be done before scheduler is started
  static const uint8_t macaddr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
  static const uint8_t ipaddr[4] = {192, 168, 0, 77};
  static const uint8_t netmask[4] = {255, 255, 255, 0};
  static const uint8_t dnsaddr[4] = {8, 8, 8, 8};
  static const uint8_t gwaddr[4] = {192, 168, 0, 1};
  FreeRTOS_IPInit(ipaddr, netmask, gwaddr, dnsaddr, macaddr);

  xTaskCreate(blinker, "blinker", 512, "1", configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(blinker, "blinker", 512, "2", configMAX_PRIORITIES - 1, NULL);
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
uint32_t HAL_RCC_GetHCLKFreq(void) {
  return SystemCoreClock;
}

uint32_t HAL_GetTick(void) {
  return configTICK_RATE_HZ;
}

#include "stm32f7xx_hal_conf.h"
#include "stm32fxx_hal_eth.h"
void HAL_ETH_MspInit( ETH_HandleTypeDef* heth ) {
    GPIO_InitTypeDef GPIO_InitStructure;
    if (heth->Instance == ETH) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();

        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL; 
        GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.Pin =  GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);
        
        __HAL_RCC_ETH_CLK_ENABLE();
        HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );
    }
}

int vLoggingPrintf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buf[100];
  int n = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  write(1, buf, n);
  return n;
}

void *_sbrk(ptrdiff_t incr) {
  (void) incr;
  return NULL;
}
