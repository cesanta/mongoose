#pragma once

#include "hal.h"

#define configUSE_PREEMPTION 1
#define configCPU_CLOCK_HZ CPU_FREQUENCY
#define configTICK_RATE_HZ 1000
#define configMAX_PRIORITIES 5
#define configUSE_16_BIT_TICKS 0
#define configUSE_TICK_HOOK 0
#define configUSE_IDLE_HOOK 0
#define configUSE_TIMERS 0
#define configUSE_CO_ROUTINES 0
#define configUSE_MALLOC_FAILED_HOOK 0
#define configMINIMAL_STACK_SIZE 128
#define configTOTAL_HEAP_SIZE (1024 * 64)
#define INCLUDE_vTaskDelay 1
#define INCLUDE_xTaskGetSchedulerState 1  // trying

#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS __NVIC_PRIO_BITS
#else
#define configPRIO_BITS 4
#endif
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
#define configKERNEL_INTERRUPT_PRIORITY \
  (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
  (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configASSERT(expr) \
  if (!(expr)) printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr)

#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
//#define xPortSysTickHandler SysTick_Handler
