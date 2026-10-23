#pragma once
#include <stddef.h>
#include <stdint.h>

#define MG_ARCH MG_ARCH_CUBE

// Current memory usage in bytes
size_t hal_get_heap(void);
size_t hal_get_stack(void);
#define MG_HEALTH_HEAP_USED hal_get_heap()
#define MG_HEALTH_STACK_USED hal_get_stack()

extern uint32_t _Min_Stack_Size;
extern unsigned char _end, _sstack;
#define MG_HEALTH_STACK_TOTAL ((size_t) (uintptr_t) &_Min_Stack_Size)
#define MG_HEALTH_HEAP_TOTAL \
  ((size_t) ((uintptr_t) &_sstack - (uintptr_t) &_end))

// Ethernet driver tunables:
// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 5
// #define MG_TCPIP_PHY_ADDR 0  // PHY address

// Uncomment this for static IP configuration:
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)     // IP
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)      // Gateway
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)  // Netmask
