
/*
 * Auto generated Run-Time-Environment Configuration File
 *      *** Do not modify ! ***
 *
 * Project: 'device-dashboard' 
 * Target:  'Target 1' 
 */

#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H


/*
 * Define the Device Header File: 
 */
#define CMSIS_device_header "stm32f7xx.h"

/* ARM.FreeRTOS::RTOS:Config:FreeRTOS:10.5.1 */
#define RTE_RTOS_FreeRTOS_CONFIG        /* RTOS FreeRTOS Config for FreeRTOS API */
/* ARM.FreeRTOS::RTOS:Core:Cortex-M:10.5.1 */
#define RTE_RTOS_FreeRTOS_CORE          /* RTOS FreeRTOS Core */
/* ARM.FreeRTOS::RTOS:Heap:Heap_4:10.5.1 */
#define RTE_RTOS_FreeRTOS_HEAP_4        /* RTOS FreeRTOS Heap 4 */
/* Keil.ARM Compiler::Compiler:I/O:STDOUT:User:1.2.0 */
#define RTE_Compiler_IO_STDOUT          /* Compiler I/O: STDOUT */
          #define RTE_Compiler_IO_STDOUT_User     /* Compiler I/O: STDOUT User */
/* Keil::CMSIS Driver:Ethernet MAC:1.12.0 */
#define RTE_Drivers_ETH_MAC0            /* Driver ETH_MAC0 */
/* Keil::CMSIS Driver:Ethernet PHY:LAN8742A:1.3.0 */
#define RTE_Drivers_PHY_LAN8742A        /* Driver PHY LAN8742A */
/* Keil::Device:STM32Cube Framework:STM32CubeMX:1.1.0 */
#define RTE_DEVICE_FRAMEWORK_CUBE_MX
/* Keil::Device:STM32Cube HAL:Common:1.3.0 */
#define RTE_DEVICE_HAL_COMMON
/* Keil::Device:STM32Cube HAL:Cortex:1.3.0 */
#define RTE_DEVICE_HAL_CORTEX
/* Keil::Device:STM32Cube HAL:DMA:1.3.0 */
#define RTE_DEVICE_HAL_DMA
/* Keil::Device:STM32Cube HAL:ETH:1.3.0 */
#define RTE_DEVICE_HAL_ETH
/* Keil::Device:STM32Cube HAL:GPIO:1.3.0 */
#define RTE_DEVICE_HAL_GPIO
/* Keil::Device:STM32Cube HAL:PWR:1.3.0 */
#define RTE_DEVICE_HAL_PWR
/* Keil::Device:STM32Cube HAL:RCC:1.3.0 */
#define RTE_DEVICE_HAL_RCC
/* Keil::Device:STM32Cube HAL:RNG:1.3.0 */
#define RTE_DEVICE_HAL_RNG
/* Keil::Device:STM32Cube HAL:UART:1.3.0 */
#define RTE_DEVICE_HAL_UART
/* Keil::Device:Startup:1.2.5 */
#define RTE_DEVICE_STARTUP_STM32F7XX    /* Device Startup for STM32F7 */
/* lwIP.lwIP::Network:API:2.2.0 */
#define RTE_Network_API                 /* Network API */
/* lwIP.lwIP::Network:CORE:IPv4:2.2.0 */
#define RTE_Network_Core                /* Network Core */
          #define RTE_Network_IPv4                /* Network IPv4 Stack */
/* lwIP.lwIP::Network:Driver:Ethernet:CMSIS Driver:2.2.0 */
#define RTE_Network_Driver_Ethernet     /* Network Driver Ethernet */
/* lwIP.lwIP::Network:Interface:Ethernet:2.2.0 */
#define RTE_Network_Interface_Ethernet  /* Network Interface Ethernet */
/* lwIP.lwIP::Network:RTOS:FreeRTOS:2.2.0 */
#define RTE_Network_RTOS                /* Network RTOS */
          #define RTE_Network_FreeRTOS            /* Network FreeRTOS */


#endif /* RTE_COMPONENTS_H */
