#pragma once
#include "FreeRTOS.h"
#include "task.h"

#define MG_ARCH MG_ARCH_CUBE
#define MG_ENABLE_FREERTOS 1

#define MG_HEALTH_STACK_TOTAL (8U * 1024U)
// Peak stack usage, FreeRTOS reports the minimum free space in stack words
#define MG_HEALTH_STACK_USED \
  (MG_HEALTH_STACK_TOTAL - \
   (size_t) uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t))
#define MG_HEALTH_HEAP_TOTAL ((size_t) configTOTAL_HEAP_SIZE)
#define MG_HEALTH_HEAP_USED (MG_HEALTH_HEAP_TOTAL - xPortGetFreeHeapSize())

// Ethernet driver tunables:
// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 5
// #define MG_TCPIP_PHY_ADDR 0  // PHY address

// Uncomment this for static IP configuration:
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)     // IP
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)      // Gateway
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)  // Netmask
