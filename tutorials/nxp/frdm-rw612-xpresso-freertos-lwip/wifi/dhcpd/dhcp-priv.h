/** @file dhcp-priv.h
 *
 *  @brief DHCP PVT Header
 *
 *  Copyright 2008-2020 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __DHCP_PRIV_H__
#define __DHCP_PRIV_H__

#include <wmlog.h>

#define dhcp_e(...) wmlog_e("dhcp", ##__VA_ARGS__)
#define dhcp_w(...) wmlog_w("dhcp", ##__VA_ARGS__)

#if CONFIG_DHCP_SERVER_DEBUG
#define dhcp_d(...) wmlog("dhcp", ##__VA_ARGS__)
#else
#define dhcp_d(...)
#endif /* ! CONFIG_DHCP_DEBUG */

#define SERVER_BUFFER_SIZE        1024
#define MAC_IP_CACHE_SIZE         8
#define SEND_RESPONSE(w, x, y, z) dhcp_send_response(w, x, y, z)

struct client_mac_cache
{
    uint8_t client_mac[6]; /* mac address of the connected device */
    uint32_t client_ip;    /* ip address of the connected device */
};

struct dhcp_server_data
{
    int sock;
    int count_clients;        /* to keep count of cached devices */
    char msg[SERVER_BUFFER_SIZE];
    struct sockaddr_in saddr; /* dhcp server address */
    struct sockaddr_in baddr; /* broadcast address */
    struct client_mac_cache ip_mac_mapping[MAC_IP_CACHE_SIZE];
    uint32_t netmask;         /* network order */
    uint32_t my_ip;           /* network order */
    uint32_t client_ip;       /* last address that was requested, network
                               * order */
    uint32_t current_ip;      /* keep track of assigned IP addresses */
};

int dhcp_server_init(void *intrfc_handle);
void dhcpd_task(osa_task_param_t arg);
int dhcp_send_halt(void);
int dhcp_free_allocations(void);
int dhcp_create_and_bind_udp_socket(struct sockaddr_in *address, void *intrfc_handle);
int dhcp_send_response(int sock, struct sockaddr *addr, char *msg, int len);
#endif
