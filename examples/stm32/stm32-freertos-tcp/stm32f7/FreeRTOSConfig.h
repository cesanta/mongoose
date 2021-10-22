#pragma once

#define configUSE_PREEMPTION 1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#define configCPU_CLOCK_HZ ((unsigned long) 216000000)
#define configSYSTICK_CLOCK_HZ (configCPU_CLOCK_HZ / 8)
#define configTICK_RATE_HZ 250
#define configMAX_PRIORITIES 5
#define configMINIMAL_STACK_SIZE 512
#define configMAX_TASK_NAME_LEN 16
#define configUSE_16_BIT_TICKS 0
#define configIDLE_SHOULD_YIELD 1
#define configUSE_TASK_NOTIFICATIONS 1
#define configUSE_MUTEXES 1
#define configUSE_RECURSIVE_MUTEXES 1
#define configUSE_COUNTING_SEMAPHORES 1
#define configQUEUE_REGISTRY_SIZE 8

#define configSUPPORT_STATIC_ALLOCATION 0
#define configSUPPORT_DYNAMIC_ALLOCATION 1

#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0
#define configCHECK_FOR_STACK_OVERFLOW 0
#define configUSE_MALLOC_FAILED_HOOK 0
#define configUSE_DAEMON_TASK_STARTUP_HOOK 0

#define configGENERATE_RUN_TIME_STATS 0
#define configUSE_TRACE_FACILITY 0
#define configUSE_STATS_FORMATTING_FUNCTIONS 0

#define configUSE_CO_ROUTINES 0
#define configMAX_CO_ROUTINE_PRIORITIES 1

#define configUSE_TIMERS 0
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH 10
#define configTIMER_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE

#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS __NVIC_PRIO_BITS
#else
#define configPRIO_BITS 4
#endif
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 4
#define configKERNEL_INTERRUPT_PRIORITY \
  (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
  (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskDelete           1
#define INCLUDE_vTaskCleanUpResources     0
#define INCLUDE_vTaskSuspend          1
#define INCLUDE_vTaskDelayUntil         0
#define INCLUDE_vTaskDelay            1
#define INCLUDE_xTimerPendFunctionCall      0
#define INCLUDE_eTaskGetState         1
#define INCLUDE_pcTaskGetTaskName       1
#define INCLUDE_xTaskGetSchedulerState      1

#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
