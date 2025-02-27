/*
 * Copyright (c) 2019 Arm Limited. All rights reserved.
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
 */

#ifndef LWIP_LWIPOPTS_H
#define LWIP_LWIPOPTS_H

/* Options managed by RTE */
#include "lwipopts_rte.h"

/* See "lwip/opt.h" for all available configuration options */

/*
   ------------------------------------
   -------------- NO SYS --------------
   ------------------------------------
#define NO_SYS               managed by RTE
#define LWIP_TIMERS          managed by RTE
#define SYS_LIGHTWEIGHT_PROT managed by RTE
*/

/*
   ------------------------------------
   ---------- Memory options ----------
   ------------------------------------
*/
#define MEM_LIBC_MALLOC                 0
#define MEMP_MEM_MALLOC                 1
#define MEMP_MEM_INIT                   0
#define MEM_ALIGNMENT                   4
#define MEM_SIZE                        (10*1024)
#define MEMP_OVERFLOW_CHECK             0
#define MEMP_SANITY_CHECK               0
#define MEM_OVERFLOW_CHECK              0
#define MEM_SANITY_CHECK                0

/* ------------------------------------------------
   ---------- Internal Memory Pool Sizes ----------
   ------------------------------------------------
 */
#define MEMP_NUM_PBUF                   10
#define MEMP_NUM_RAW_PCB                4
#define MEMP_NUM_UDP_PCB                6
#define MEMP_NUM_TCP_PCB                10
#define MEMP_NUM_TCP_PCB_LISTEN         6
#define MEMP_NUM_TCP_SEG                12
#define MEMP_NUM_REASSDATA              16
#define MEMP_NUM_FRAG_PBUF              15
#define MEMP_NUM_ARP_QUEUE              10
#define MEMP_NUM_IGMP_GROUP             8
#define MEMP_NUM_NETBUF                 2
#define MEMP_NUM_NETCONN                10
#define MEMP_NUM_SELECT_CB              4
#define MEMP_NUM_TCPIP_MSG_API          8
#define MEMP_NUM_TCPIP_MSG_INPKT        8
#define MEMP_NUM_NETDB                  1
#define MEMP_NUM_LOCALHOSTLIST          1
#define PBUF_POOL_SIZE                  16

/*
   ---------------------------------
   ---------- ARP options ----------
   ---------------------------------
#define LWIP_ETHERNET      managed by RTE
*/
#define LWIP_ARP                        1
#define ARP_TABLE_SIZE                  10
#define ETHARP_SUPPORT_VLAN             0
#define ETH_PAD_SIZE                    2
#define ETHARP_SUPPORT_STATIC_ENTRIES   0

/*
   --------------------------------
   ---------- IP options ----------
   --------------------------------
#define LWIP_IPV4          managed by RTE
*/
#define IP_FORWARD                      0
#define IP_REASSEMBLY                   1
#define IP_FRAG                         1
#define IP_OPTIONS_ALLOWED              1
#define IP_REASS_MAX_PBUFS              10
#define IP_DEFAULT_TTL                  128
#define IP_SOF_BROADCAST                1
#define IP_SOF_BROADCAST_RECV           1

/*
   ----------------------------------
   ---------- ICMP options ----------
   ----------------------------------
*/
#define LWIP_ICMP                       1

/*
   ---------------------------------
   ---------- RAW options ----------
   ---------------------------------
*/
#define LWIP_RAW                        1

/*
   ----------------------------------
   ---------- DHCP options ----------
   ----------------------------------
*/
#define LWIP_DHCP                       1
#define LWIP_DHCP_CHECK_LINK_UP         0
#define LWIP_DHCP_BOOTP_FILE            0
#define LWIP_DHCP_GET_NTP_SRV           0
#define LWIP_DHCP_MAX_NTP_SERVERS       1

/*
   ------------------------------------
   ---------- AUTOIP options ----------
   ------------------------------------
*/
#define LWIP_AUTOIP                     0
#define LWIP_DHCP_AUTOIP_COOP           0
#define LWIP_DHCP_AUTOIP_COOP_TRIES     9

/*
   ----------------------------------
   ---------- IGMP options ----------
   ----------------------------------
*/
#define LWIP_IGMP                       0

/*
   ----------------------------------
   ---------- DNS options -----------
   ----------------------------------
*/
#define LWIP_DNS                        0
#define DNS_TABLE_SIZE                  4
#define DNS_MAX_NAME_LENGTH             256

/*
   ---------------------------------
   ---------- UDP options ----------
   ---------------------------------
*/
#define LWIP_UDP                        1
#define LWIP_UDPLITE                    0

/*
   ---------------------------------
   ---------- TCP options ----------
   ---------------------------------
*/
#define LWIP_TCP                        1
#define TCP_QUEUE_OOSEQ                 0
#define LWIP_TCP_SACK_OUT               0
#define LWIP_TCP_MAX_SACK_NUM           4
#define TCP_MSS                         1460
#define TCP_OOSEQ_MAX_BYTES             0
#define TCP_OOSEQ_MAX_PBUFS             0
#define TCP_LISTEN_BACKLOG              1
#define TCP_DEFAULT_LISTEN_BACKLOG      0xff
#define LWIP_EVENT_API                  0
#define LWIP_CALLBACK_API               1
#define LWIP_WND_SCALE                  0
#define TCP_RCV_SCALE                   0
#define LWIP_TCP_PCB_NUM_EXT_ARGS       0
#define LWIP_ALTCP                      0
#define LWIP_ALTCP_TLS                  0

/*
   ----------------------------------
   ---------- Pbuf options ----------
   ----------------------------------
*/
#define PBUF_LINK_ENCAPSULATION_HLEN    0

/*
   ------------------------------------------------
   ---------- Network Interfaces options ----------
   ------------------------------------------------
*/
#define LWIP_SINGLE_NETIF               0
#define LWIP_NETIF_HOSTNAME             0
#define LWIP_NETIF_API                  0
#define LWIP_NETIF_STATUS_CALLBACK      1
#define LWIP_NETIF_EXT_STATUS_CALLBACK  0
#define LWIP_NETIF_LINK_CALLBACK        1
#define LWIP_NETIF_REMOVE_CALLBACK      0
#define LWIP_NETIF_HWADDRHINT           1

/*
   ------------------------------------
   ---------- LOOPIF options ----------
   ------------------------------------
*/
#define LWIP_HAVE_LOOPIF                0
#define LWIP_LOOPIF_MULTICAST           0
#define LWIP_NETIF_LOOPBACK             0
#define LWIP_LOOPBACK_MAX_PBUFS         0

/*
   ------------------------------------
   ---------- Thread options ----------
   ------------------------------------
*/
#define TCPIP_THREAD_NAME               "tcpip_thread"
#define TCPIP_THREAD_STACKSIZE          1024
#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 1)
#define TCPIP_MBOX_SIZE                 6
#define SLIPIF_THREAD_NAME              "slipif_loop"
#define SLIPIF_THREAD_STACKSIZE         0
#define SLIPIF_THREAD_PRIO              1
#define DEFAULT_THREAD_NAME             "lwIP"
#define DEFAULT_THREAD_STACKSIZE        1024
#define DEFAULT_THREAD_PRIO             (configMAX_PRIORITIES - 1)
#define DEFAULT_RAW_RECVMBOX_SIZE       6
#define DEFAULT_UDP_RECVMBOX_SIZE       6
#define DEFAULT_TCP_RECVMBOX_SIZE       6
#define DEFAULT_ACCEPTMBOX_SIZE         6

/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
#define LWIP_NETCONN                    0
#define LWIP_TCPIP_TIMEOUT              1
#define LWIP_NETCONN_SEM_PER_THREAD     0
#define LWIP_NETCONN_FULLDUPLEX         0

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
#define LWIP_SOCKET                     1
#define LWIP_COMPAT_SOCKETS             1
#define LWIP_POSIX_SOCKETS_IO_NAMES     1
#define LWIP_TCP_KEEPALIVE              1
#define LWIP_SO_SNDTIMEO                0
#define LWIP_SO_RCVTIMEO                0
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD 0
#define LWIP_SO_RCVBUF                  0
#define LWIP_SO_LINGER                  0
#define SO_REUSE                        0
#define SO_REUSE_RXTOALL                0
#define LWIP_SOCKET_SELECT              1
#define LWIP_SOCKET_POLL                1

/*
   ----------------------------------------
   ---------- Statistics options ----------
   ----------------------------------------
*/
#define LWIP_STATS                      0

/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/
#define LWIP_CHECKSUM_CTRL_PER_NETIF    0

/*
   ---------------------------------------
   ---------- IPv6 options ---------------
   ---------------------------------------
#define LWIP_IPV6           managed by RTE
*/
#define LWIP_IPV6_FORWARD               0
#define LWIP_IPV6_FRAG                  1
#define LWIP_IPV6_REASS                 1
#define LWIP_IPV6_AUTOCONFIG            1
#define LWIP_ICMP6                      1
#define LWIP_IPV6_MLD                   0
#define MEMP_NUM_MLD6_GROUP             4
#define LWIP_ND6_NUM_NEIGHBORS          10
#define LWIP_ND6_NUM_DESTINATIONS       10
#define LWIP_ND6_NUM_PREFIXES           5
#define LWIP_ND6_NUM_ROUTERS            3
#define LWIP_ND6_ALLOW_RA_UPDATES       1
#define LWIP_ND6_TCP_REACHABILITY_HINTS 1
#define LWIP_IPV6_DHCP6                 0
#define LWIP_IPV6_DHCP6_STATEFUL        0
#define LWIP_IPV6_DHCP6_STATELESS       1
#define LWIP_DHCP6_GET_NTP_SRV          0
#define LWIP_DHCP6_MAX_NTP_SERVERS      1

#endif /* LWIP_LWIPOPTS_H */
