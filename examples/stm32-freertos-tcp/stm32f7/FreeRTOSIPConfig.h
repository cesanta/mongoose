#pragma once
#define FREERTOS_IP_CONFIG_H

#ifndef ipconfigHAS_DEBUG_PRINTF
#define ipconfigHAS_DEBUG_PRINTF 1
#endif
#if (ipconfigHAS_DEBUG_PRINTF == 1)
#define FreeRTOS_debug_printf(X) vLoggingPrintf X
#endif
#ifndef ipconfigHAS_PRINTF
#define ipconfigHAS_PRINTF 1
#endif
#if (ipconfigHAS_PRINTF == 1)
#define FreeRTOS_printf(X) vLoggingPrintf X
#endif

#define ipconfigBYTE_ORDER pdFREERTOS_LITTLE_ENDIAN
#define ipconfigREPLY_TO_INCOMING_PINGS       1
#define ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM (1)
#define ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM (1)
#define ipconfigZERO_COPY_RX_DRIVER (1)
#define ipconfigZERO_COPY_TX_DRIVER (1)
#define ipconfigIP_TASK_PRIORITY 4
#define niEMAC_HANDLER_TASK_PRIORITY 5
#define ipconfigIPERF_PRIORITY_IPERF_TASK 6
#define ipconfigIP_TASK_STACK_SIZE_WORDS (configMINIMAL_STACK_SIZE * 5)
extern UBaseType_t uxRand();
#define ipconfigRAND32() uxRand()
#define ipconfigUSE_NETWORK_EVENT_HOOK 1
#define ipconfigUSE_DHCP 0
#define ipconfigDHCP_REGISTER_HOSTNAME 0
#define ipconfigDHCP_USES_UNICAST 1
#define ipconfigMAXIMUM_DISCOVER_TX_PERIOD (pdMS_TO_TICKS(30000))
#define ipconfigARP_CACHE_ENTRIES 6
#define ipconfigMAX_ARP_RETRANSMISSIONS (5)
#define ipconfigMAX_ARP_AGE 150
#define ipconfigINCLUDE_FULL_INET_ADDR 0
#define ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS 20
#define ipconfigEVENT_QUEUE_LENGTH (ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS + 5)
#define ipconfigUDP_TIME_TO_LIVE 128
#define ipconfigTCP_TIME_TO_LIVE 128
#define ipconfigUSE_TCP 1
#define ipconfigSUPPORT_SELECT_FUNCTION 1
#define ipconfigUSE_TCP_WIN 0
#define ipconfigNETWORK_MTU 1500