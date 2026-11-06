/*
 * FreeRTOS Kernel V10.6.2
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef MPU_SYSCALL_NUMBERS_H
#define MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#define SYSTEM_CALL_xTaskGenericNotify                     0
#define SYSTEM_CALL_xTaskGenericNotifyWait                 1
#define SYSTEM_CALL_xTimerGenericCommand                   2
#define SYSTEM_CALL_xEventGroupWaitBits                    3
#define SYSTEM_CALL_xTaskDelayUntil                        4
#define SYSTEM_CALL_xTaskAbortDelay                        5
#define SYSTEM_CALL_vTaskDelay                             6
#define SYSTEM_CALL_uxTaskPriorityGet                      7
#define SYSTEM_CALL_eTaskGetState                          8
#define SYSTEM_CALL_vTaskGetInfo                           9
#define SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#define SYSTEM_CALL_vTaskSuspend                           11
#define SYSTEM_CALL_vTaskResume                            12
#define SYSTEM_CALL_xTaskGetTickCount                      13
#define SYSTEM_CALL_uxTaskGetNumberOfTasks                 14
#define SYSTEM_CALL_pcTaskGetName                          15
#define SYSTEM_CALL_ulTaskGetRunTimeCounter                16
#define SYSTEM_CALL_ulTaskGetRunTimePercent                17
#define SYSTEM_CALL_ulTaskGetIdleRunTimePercent            18
#define SYSTEM_CALL_ulTaskGetIdleRunTimeCounter            19
#define SYSTEM_CALL_vTaskSetApplicationTaskTag             20
#define SYSTEM_CALL_xTaskGetApplicationTaskTag             21
#define SYSTEM_CALL_vTaskSetThreadLocalStoragePointer      22
#define SYSTEM_CALL_pvTaskGetThreadLocalStoragePointer     23
#define SYSTEM_CALL_uxTaskGetSystemState                   24
#define SYSTEM_CALL_uxTaskGetStackHighWaterMark            25
#define SYSTEM_CALL_uxTaskGetStackHighWaterMark2           26
#define SYSTEM_CALL_xTaskGetCurrentTaskHandle              27
#define SYSTEM_CALL_xTaskGetSchedulerState                 28
#define SYSTEM_CALL_vTaskSetTimeOutState                   29
#define SYSTEM_CALL_xTaskCheckForTimeOut                   30
#define SYSTEM_CALL_ulTaskGenericNotifyTake                31
#define SYSTEM_CALL_xTaskGenericNotifyStateClear           32
#define SYSTEM_CALL_ulTaskGenericNotifyValueClear          33
#define SYSTEM_CALL_xQueueGenericSend                      34
#define SYSTEM_CALL_uxQueueMessagesWaiting                 35
#define SYSTEM_CALL_uxQueueSpacesAvailable                 36
#define SYSTEM_CALL_xQueueReceive                          37
#define SYSTEM_CALL_xQueuePeek                             38
#define SYSTEM_CALL_xQueueSemaphoreTake                    39
#define SYSTEM_CALL_xQueueGetMutexHolder                   40
#define SYSTEM_CALL_xQueueTakeMutexRecursive               41
#define SYSTEM_CALL_xQueueGiveMutexRecursive               42
#define SYSTEM_CALL_xQueueSelectFromSet                    43
#define SYSTEM_CALL_xQueueAddToSet                         44
#define SYSTEM_CALL_vQueueAddToRegistry                    45
#define SYSTEM_CALL_vQueueUnregisterQueue                  46
#define SYSTEM_CALL_pcQueueGetName                         47
#define SYSTEM_CALL_pvTimerGetTimerID                      48
#define SYSTEM_CALL_vTimerSetTimerID                       49
#define SYSTEM_CALL_xTimerIsTimerActive                    50
#define SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         51
#define SYSTEM_CALL_pcTimerGetName                         52
#define SYSTEM_CALL_vTimerSetReloadMode                    53
#define SYSTEM_CALL_xTimerGetReloadMode                    54
#define SYSTEM_CALL_uxTimerGetReloadMode                   55
#define SYSTEM_CALL_xTimerGetPeriod                        56
#define SYSTEM_CALL_xTimerGetExpiryTime                    57
#define SYSTEM_CALL_xEventGroupClearBits                   58
#define SYSTEM_CALL_xEventGroupSetBits                     59
#define SYSTEM_CALL_xEventGroupSync                        60
#define SYSTEM_CALL_uxEventGroupGetNumber                  61
#define SYSTEM_CALL_vEventGroupSetNumber                   62
#define SYSTEM_CALL_xStreamBufferSend                      63
#define SYSTEM_CALL_xStreamBufferReceive                   64
#define SYSTEM_CALL_xStreamBufferIsFull                    65
#define SYSTEM_CALL_xStreamBufferIsEmpty                   66
#define SYSTEM_CALL_xStreamBufferSpacesAvailable           67
#define SYSTEM_CALL_xStreamBufferBytesAvailable            68
#define SYSTEM_CALL_xStreamBufferSetTriggerLevel           69
#define SYSTEM_CALL_xStreamBufferNextMessageLengthBytes    70
#define NUM_SYSTEM_CALLS                                   71  /* Total number of system calls. */

#endif /* MPU_SYSCALL_NUMBERS_H */
