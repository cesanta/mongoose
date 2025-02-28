/*
 * Copyright (c) 2013-2021 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * $Revision:   V5.5.2
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       RTX Configuration definitions
 *
 * -----------------------------------------------------------------------------
 */
 
#ifndef RTX_CONFIG_H_
#define RTX_CONFIG_H_
 
#ifdef   _RTE_
#include "RTE_Components.h"
#ifdef    RTE_RTX_CONFIG_H
#include  RTE_RTX_CONFIG_H
#endif
#endif
 
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
 
// <h>System Configuration
// =======================
 
//   <o>Global Dynamic Memory size [bytes] <0-1073741824:8>
//   <i> Defines the combined global dynamic memory size.
//   <i> Default: 32768
#ifndef OS_DYNAMIC_MEM_SIZE
#define OS_DYNAMIC_MEM_SIZE         65536
#endif
 
//   <o>Kernel Tick Frequency [Hz] <1-1000000>
//   <i> Defines base time unit for delays and timeouts.
//   <i> Default: 1000 (1ms tick)
#ifndef OS_TICK_FREQ
#define OS_TICK_FREQ                1000
#endif
 
//   <e>Round-Robin Thread switching
//   <i> Enables Round-Robin Thread switching.
#ifndef OS_ROBIN_ENABLE
#define OS_ROBIN_ENABLE             1
#endif
 
//     <o>Round-Robin Timeout <1-1000>
//     <i> Defines how many ticks a thread will execute before a thread switch.
//     <i> Default: 5
#ifndef OS_ROBIN_TIMEOUT
#define OS_ROBIN_TIMEOUT            5
#endif
 
//   </e>
 
//   <o>ISR FIFO Queue
//      <4=>  4 entries    <8=>   8 entries   <12=>  12 entries   <16=>  16 entries
//     <24=> 24 entries   <32=>  32 entries   <48=>  48 entries   <64=>  64 entries
//     <96=> 96 entries  <128=> 128 entries  <196=> 196 entries  <256=> 256 entries
//   <i> RTOS Functions called from ISR store requests to this buffer.
//   <i> Default: 16 entries
#ifndef OS_ISR_FIFO_QUEUE
#define OS_ISR_FIFO_QUEUE           16
#endif
 
//   <q>Object Memory usage counters
//   <i> Enables object memory usage counters (requires RTX source variant).
#ifndef OS_OBJ_MEM_USAGE
#define OS_OBJ_MEM_USAGE            0
#endif
 
// </h>
 
// <h>Thread Configuration
// =======================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_THREAD_OBJ_MEM
#define OS_THREAD_OBJ_MEM           0
#endif
 
//     <o>Number of user Threads <1-1000>
//     <i> Defines maximum number of user threads that can be active at the same time.
//     <i> Applies to user threads with system provided memory for control blocks.
#ifndef OS_THREAD_NUM
#define OS_THREAD_NUM               1
#endif
 
//     <o>Number of user Threads with default Stack size <0-1000>
//     <i> Defines maximum number of user threads with default stack size.
//     <i> Applies to user threads with zero stack size specified.
#ifndef OS_THREAD_DEF_STACK_NUM
#define OS_THREAD_DEF_STACK_NUM     0
#endif
 
//     <o>Total Stack size [bytes] for user Threads with user-provided Stack size <0-1073741824:8>
//     <i> Defines the combined stack size for user threads with user-provided stack size.
//     <i> Applies to user threads with user-provided stack size and system provided memory for stack.
//     <i> Default: 0
#ifndef OS_THREAD_USER_STACK_SIZE
#define OS_THREAD_USER_STACK_SIZE   0
#endif
 
//   </e>
 
//   <o>Default Thread Stack size [bytes] <96-1073741824:8>
//   <i> Defines stack size for threads with zero stack size specified.
//   <i> Default: 3072
#ifndef OS_STACK_SIZE
#define OS_STACK_SIZE               3072
#endif
 
//   <o>Idle Thread Stack size [bytes] <72-1073741824:8>
//   <i> Defines stack size for Idle thread.
//   <i> Default: 512
#ifndef OS_IDLE_THREAD_STACK_SIZE
#define OS_IDLE_THREAD_STACK_SIZE   512
#endif
 
//   <o>Idle Thread TrustZone Module Identifier
//   <i> Defines TrustZone Thread Context Management Identifier.
//   <i> Applies only to cores with TrustZone technology.
//   <i> Default: 0 (not used)
#ifndef OS_IDLE_THREAD_TZ_MOD_ID
#define OS_IDLE_THREAD_TZ_MOD_ID    0
#endif
 
//   <q>Stack overrun checking
//   <i> Enables stack overrun check at thread switch (requires RTX source variant).
//   <i> Enabling this option increases slightly the execution time of a thread switch.
#ifndef OS_STACK_CHECK
#define OS_STACK_CHECK              1
#endif
 
//   <q>Stack usage watermark
//   <i> Initializes thread stack with watermark pattern for analyzing stack usage.
//   <i> Enabling this option increases significantly the execution time of thread creation.
#ifndef OS_STACK_WATERMARK
#define OS_STACK_WATERMARK          0
#endif
 
//   <o>Processor mode for Thread execution
//     <0=> Unprivileged mode
//     <1=> Privileged mode
//   <i> Default: Privileged mode
#ifndef OS_PRIVILEGE_MODE
#define OS_PRIVILEGE_MODE           1
#endif
 
// </h>
 
// <h>Timer Configuration
// ======================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_TIMER_OBJ_MEM
#define OS_TIMER_OBJ_MEM            0
#endif
 
//     <o>Number of Timer objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_TIMER_NUM
#define OS_TIMER_NUM                1
#endif
 
//   </e>
 
//   <o>Timer Thread Priority
//      <8=> Low
//     <16=> Below Normal  <24=> Normal  <32=> Above Normal
//     <40=> High
//     <48=> Realtime
//   <i> Defines priority for timer thread
//   <i> Default: High
#ifndef OS_TIMER_THREAD_PRIO
#define OS_TIMER_THREAD_PRIO        40
#endif
 
//   <o>Timer Thread Stack size [bytes] <0-1073741824:8>
//   <i> Defines stack size for Timer thread.
//   <i> May be set to 0 when timers are not used.
//   <i> Default: 512
#ifndef OS_TIMER_THREAD_STACK_SIZE
#define OS_TIMER_THREAD_STACK_SIZE  512
#endif
 
//   <o>Timer Thread TrustZone Module Identifier
//   <i> Defines TrustZone Thread Context Management Identifier.
//   <i> Applies only to cores with TrustZone technology.
//   <i> Default: 0 (not used)
#ifndef OS_TIMER_THREAD_TZ_MOD_ID
#define OS_TIMER_THREAD_TZ_MOD_ID   0
#endif
 
//   <o>Timer Callback Queue entries <0-256>
//   <i> Number of concurrent active timer callback functions.
//   <i> May be set to 0 when timers are not used.
//   <i> Default: 4
#ifndef OS_TIMER_CB_QUEUE
#define OS_TIMER_CB_QUEUE           4
#endif
 
// </h>
 
// <h>Event Flags Configuration
// ============================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_EVFLAGS_OBJ_MEM
#define OS_EVFLAGS_OBJ_MEM          0
#endif
 
//     <o>Number of Event Flags objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_EVFLAGS_NUM
#define OS_EVFLAGS_NUM              1
#endif
 
//   </e>
 
// </h>
 
// <h>Mutex Configuration
// ======================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_MUTEX_OBJ_MEM
#define OS_MUTEX_OBJ_MEM            0
#endif
 
//     <o>Number of Mutex objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_MUTEX_NUM
#define OS_MUTEX_NUM                1
#endif
 
//   </e>
 
// </h>
 
// <h>Semaphore Configuration
// ==========================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_SEMAPHORE_OBJ_MEM
#define OS_SEMAPHORE_OBJ_MEM        0
#endif
 
//     <o>Number of Semaphore objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_SEMAPHORE_NUM
#define OS_SEMAPHORE_NUM            1
#endif
 
//   </e>
 
// </h>
 
// <h>Memory Pool Configuration
// ============================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_MEMPOOL_OBJ_MEM
#define OS_MEMPOOL_OBJ_MEM          0
#endif
 
//     <o>Number of Memory Pool objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_MEMPOOL_NUM
#define OS_MEMPOOL_NUM              1
#endif
 
//     <o>Data Storage Memory size [bytes] <0-1073741824:8>
//     <i> Defines the combined data storage memory size.
//     <i> Applies to objects with system provided memory for data storage.
//     <i> Default: 0
#ifndef OS_MEMPOOL_DATA_SIZE
#define OS_MEMPOOL_DATA_SIZE        0
#endif
 
//   </e>
 
// </h>
 
// <h>Message Queue Configuration
// ==============================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_MSGQUEUE_OBJ_MEM
#define OS_MSGQUEUE_OBJ_MEM         0
#endif
 
//     <o>Number of Message Queue objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_MSGQUEUE_NUM
#define OS_MSGQUEUE_NUM             1
#endif
 
//     <o>Data Storage Memory size [bytes] <0-1073741824:8>
//     <i> Defines the combined data storage memory size.
//     <i> Applies to objects with system provided memory for data storage.
//     <i> Default: 0
#ifndef OS_MSGQUEUE_DATA_SIZE
#define OS_MSGQUEUE_DATA_SIZE       0
#endif
 
//   </e>
 
// </h>
 
// <h>Event Recorder Configuration
// ===============================
 
//   <e>Global Initialization
//   <i> Initialize Event Recorder during 'osKernelInitialize'.
#ifndef OS_EVR_INIT
#define OS_EVR_INIT                 0
#endif
 
//     <q>Start recording
//     <i> Start event recording after initialization.
#ifndef OS_EVR_START
#define OS_EVR_START                1
#endif
 
//     <h>Global Event Filter Setup
//     <i> Initial recording level applied to all components.
//       <o.0>Error events
//       <o.1>API function call events
//       <o.2>Operation events
//       <o.3>Detailed operation events
//     </h>
#ifndef OS_EVR_LEVEL
#define OS_EVR_LEVEL                0x00U
#endif
 
//     <h>RTOS Event Filter Setup
//     <i> Recording levels for RTX components.
//     <i> Only applicable if events for the respective component are generated.
 
//       <e.7>Memory Management
//       <i> Recording level for Memory Management events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_MEMORY_LEVEL
#define OS_EVR_MEMORY_LEVEL         0x81U
#endif
 
//       <e.7>Kernel
//       <i> Recording level for Kernel events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_KERNEL_LEVEL
#define OS_EVR_KERNEL_LEVEL         0x81U
#endif
 
//       <e.7>Thread
//       <i> Recording level for Thread events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_THREAD_LEVEL
#define OS_EVR_THREAD_LEVEL         0x85U
#endif
 
//       <e.7>Generic Wait
//       <i> Recording level for Generic Wait events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_WAIT_LEVEL
#define OS_EVR_WAIT_LEVEL           0x81U
#endif
 
//       <e.7>Thread Flags
//       <i> Recording level for Thread Flags events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_THFLAGS_LEVEL
#define OS_EVR_THFLAGS_LEVEL        0x81U
#endif
 
//       <e.7>Event Flags
//       <i> Recording level for Event Flags events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_EVFLAGS_LEVEL
#define OS_EVR_EVFLAGS_LEVEL        0x81U
#endif
 
//       <e.7>Timer
//       <i> Recording level for Timer events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_TIMER_LEVEL
#define OS_EVR_TIMER_LEVEL          0x81U
#endif
 
//       <e.7>Mutex
//       <i> Recording level for Mutex events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_MUTEX_LEVEL
#define OS_EVR_MUTEX_LEVEL          0x81U
#endif
 
//       <e.7>Semaphore
//       <i> Recording level for Semaphore events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_SEMAPHORE_LEVEL
#define OS_EVR_SEMAPHORE_LEVEL      0x81U
#endif
 
//       <e.7>Memory Pool
//       <i> Recording level for Memory Pool events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_MEMPOOL_LEVEL
#define OS_EVR_MEMPOOL_LEVEL        0x81U
#endif
 
//       <e.7>Message Queue
//       <i> Recording level for Message Queue events.
//         <o.0>Error events
//         <o.1>API function call events
//         <o.2>Operation events
//         <o.3>Detailed operation events
//       </e>
#ifndef OS_EVR_MSGQUEUE_LEVEL
#define OS_EVR_MSGQUEUE_LEVEL       0x81U
#endif
 
//     </h>
 
//   </e>
 
//   <h>RTOS Event Generation
//   <i> Enables event generation for RTX components (requires RTX source variant).
 
//     <q>Memory Management
//     <i> Enables Memory Management event generation.
#ifndef OS_EVR_MEMORY
#define OS_EVR_MEMORY               1
#endif
 
//     <q>Kernel
//     <i> Enables Kernel event generation.
#ifndef OS_EVR_KERNEL
#define OS_EVR_KERNEL               1
#endif
 
//     <q>Thread
//     <i> Enables Thread event generation.
#ifndef OS_EVR_THREAD
#define OS_EVR_THREAD               1
#endif
 
//     <q>Generic Wait
//     <i> Enables Generic Wait event generation.
#ifndef OS_EVR_WAIT
#define OS_EVR_WAIT                 1
#endif
 
//     <q>Thread Flags
//     <i> Enables Thread Flags event generation.
#ifndef OS_EVR_THFLAGS
#define OS_EVR_THFLAGS              1
#endif
 
//     <q>Event Flags
//     <i> Enables Event Flags event generation.
#ifndef OS_EVR_EVFLAGS
#define OS_EVR_EVFLAGS              1
#endif
 
//     <q>Timer
//     <i> Enables Timer event generation.
#ifndef OS_EVR_TIMER
#define OS_EVR_TIMER                1
#endif
 
//     <q>Mutex
//     <i> Enables Mutex event generation.
#ifndef OS_EVR_MUTEX
#define OS_EVR_MUTEX                1
#endif
 
//     <q>Semaphore
//     <i> Enables Semaphore event generation.
#ifndef OS_EVR_SEMAPHORE
#define OS_EVR_SEMAPHORE            1
#endif
 
//     <q>Memory Pool
//     <i> Enables Memory Pool event generation.
#ifndef OS_EVR_MEMPOOL
#define OS_EVR_MEMPOOL              1
#endif
 
//     <q>Message Queue
//     <i> Enables Message Queue event generation.
#ifndef OS_EVR_MSGQUEUE
#define OS_EVR_MSGQUEUE             1
#endif
 
//   </h>
 
// </h>
 
// Number of Threads which use standard C/C++ library libspace
// (when thread specific memory allocation is not used).
#if (OS_THREAD_OBJ_MEM == 0)
#ifndef OS_THREAD_LIBSPACE_NUM
#define OS_THREAD_LIBSPACE_NUM      4
#endif
#else
#define OS_THREAD_LIBSPACE_NUM      OS_THREAD_NUM
#endif
 
//------------- <<< end of configuration section >>> ---------------------------
 
#endif  // RTX_CONFIG_H_
