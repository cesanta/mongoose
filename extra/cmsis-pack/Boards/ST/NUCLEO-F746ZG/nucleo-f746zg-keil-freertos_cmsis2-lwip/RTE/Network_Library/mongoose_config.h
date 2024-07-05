// See https://mongoose.ws/documentation/#build-options
#pragma once

// <<< Use Configuration Wizard in Context Menu >>>

// <o MG_ARCH> Build environment
//   <MG_ARCH_ARMCC=> Keil MDK
//   <MG_ARCH_FREERTOS=> FreeRTOS
//   <MG_ARCH_CMSIS_RTOS1=> CMSIS-RTOS v1
//   <MG_ARCH_CMSIS_RTOS2=> CMSIS-RTOS v2
//   <MG_ARCH_NEWLIB=> ARM GCC + Newlib
#define MG_ARCH MG_ARCH_CMSIS_RTOS2

// <q> Enable builtin TCP/IP stack
#define MG_ENABLE_TCPIP 0

// <h> Network Drivers for builtin TCP/IP stack
// <q> Enable STM23Fxx Ethernet driver
#define MG_ENABLE_DRIVER_STM32F 0

// <q> Enable STM32Hxx Ethernet driver
#define MG_ENABLE_DRIVER_STM32H 0

// <q> Enable IMXRT Ethernet driver
#define MG_ENABLE_DRIVER_IMXRT 0

// <q> Enable W5500 Ethernet driver
#define MG_ENABLE_DRIVER_W5500 0

// <q> Enable TI TM4C Ethernet driver
#define MG_ENABLE_DRIVER_TM4C 0

// </h>

// <o MG_TLS> TLS support
//   <MG_TLS_NONE=> None
//   <MG_TLS_BUILTIN=> Built-in TLS 1.3 stack
//   <MG_TLS_MBED=> MbedTLS
#define MG_TLS MG_TLS_NONE

// <q> Enable custom mg_millis() 
#define MG_ENABLE_CUSTOM_MILLIS 0

// <q> Enable custom mg_random() 
#define MG_ENABLE_CUSTOM_RANDOM 1

// <q> Enable packed (embedded) filesystem
#define MG_ENABLE_PACKED_FS 1

// <<< end of configuration section >>>

// Add your customization below this comment
#define MG_ENABLE_LWIP 1


