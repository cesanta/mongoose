/** @file dhcp-server.c
 *
 *  @brief This file provides the DHCP Server
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** dhcp-server.c: The DHCP Server
 */
#include <string.h>

#include <osa.h>
#include <wm_net.h>
#include <dhcp-server.h>
#include <wlan.h>

#include "dhcp-bootp.h"
#include "dns.h"
#include "dhcp-priv.h"

#define DEFAULT_DHCP_ADDRESS_TIMEOUT (24U * 60U * 60U * 1U) /* 1 day */
#define CLIENT_IP_NOT_FOUND          0x00000000U

uint32_t dhcp_address_timeout = DEFAULT_DHCP_ADDRESS_TIMEOUT;
OSA_MUTEX_HANDLE_DEFINE(dhcpd_mutex_Handle);

#ifndef __ZEPHYR__
static int ctrl = -1;
#else
static int ctrl_sockpair[2];
#endif

#define CTRL_PORT 12679
static char ctrl_msg[16];

struct dhcp_server_data dhcps;
static void get_broadcast_addr(struct sockaddr_in *addr);
static int get_ip_addr_from_interface(uint32_t *ip, void *interface_handle);
static int get_netmask_from_interface(uint32_t *nm, void *interface_handle);
static int send_gratuitous_arp(uint32_t ip);
static int ac_add(uint8_t *chaddr, uint32_t client_ip);
static uint32_t ac_lookup_mac(uint8_t *chaddr);
static uint8_t *ac_lookup_ip(uint32_t client_ip);
static bool ac_not_full(void);

static int ac_add(uint8_t *chaddr, uint32_t client_ip)
{
    /* adds ip-mac mapping in cache */
    if (ac_not_full())
    {
        dhcps.ip_mac_mapping[dhcps.count_clients].client_mac[0] = chaddr[0];
        dhcps.ip_mac_mapping[dhcps.count_clients].client_mac[1] = chaddr[1];
        dhcps.ip_mac_mapping[dhcps.count_clients].client_mac[2] = chaddr[2];
        dhcps.ip_mac_mapping[dhcps.count_clients].client_mac[3] = chaddr[3];
        dhcps.ip_mac_mapping[dhcps.count_clients].client_mac[4] = chaddr[4];
        dhcps.ip_mac_mapping[dhcps.count_clients].client_mac[5] = chaddr[5];
        dhcps.ip_mac_mapping[dhcps.count_clients].client_ip     = client_ip;
        dhcps.count_clients++;
        return WM_SUCCESS;
    }
    return -WM_FAIL;
}

static uint32_t ac_lookup_mac(uint8_t *chaddr)
{
    /* returns ip address, if mac address is present in cache */
    int i;
    for (i = 0; i < dhcps.count_clients && i < MAC_IP_CACHE_SIZE; i++)
    {
        if ((dhcps.ip_mac_mapping[i].client_mac[0] == chaddr[0]) &&
            (dhcps.ip_mac_mapping[i].client_mac[1] == chaddr[1]) &&
            (dhcps.ip_mac_mapping[i].client_mac[2] == chaddr[2]) &&
            (dhcps.ip_mac_mapping[i].client_mac[3] == chaddr[3]) &&
            (dhcps.ip_mac_mapping[i].client_mac[4] == chaddr[4]) &&
            (dhcps.ip_mac_mapping[i].client_mac[5] == chaddr[5]))
        {
            return dhcps.ip_mac_mapping[i].client_ip;
        }
    }
    return CLIENT_IP_NOT_FOUND;
}

static uint8_t *ac_lookup_ip(uint32_t client_ip)
{
    /* returns mac address, if ip address is present in cache */
    int i;
    for (i = 0; i < dhcps.count_clients && i < MAC_IP_CACHE_SIZE; i++)
    {
        if ((dhcps.ip_mac_mapping[i].client_ip) == client_ip)
        {
            return dhcps.ip_mac_mapping[i].client_mac;
        }
    }
    return NULL;
}

static bool ac_not_full(void)
{
    /* returns true if cache is not full */
    return (dhcps.count_clients < MAC_IP_CACHE_SIZE);
}

static bool ac_valid_ip(uint32_t requested_ip)
{
    /* skip over our own address, the network address or the
     * broadcast address
     */
    if (requested_ip == ntohl(dhcps.my_ip) || (requested_ip == ntohl(dhcps.my_ip & dhcps.netmask)) ||
        (requested_ip == ntohl((dhcps.my_ip | (0xffffffff & ~dhcps.netmask)))))
    {
        return false;
    }
    if (ac_lookup_ip(htonl(requested_ip)) != NULL)
    {
        return false;
    }
    return true;
}

static void write_u32(char *dest, uint32_t be_value)
{
    *dest++ = be_value & 0xFFU;
    *dest++ = (be_value >> 8) & 0xFFU;
    *dest++ = (be_value >> 16) & 0xFFU;
    *dest   = be_value >> 24;
}

/* Configure the DHCP dynamic IP lease time*/
int dhcp_server_lease_timeout(uint32_t val)
{
    if ((val == 0U) || (val > (60U * 60U * 24U * 49700U)))
    {
        return -EINVAL;
    }
    else
    {
        dhcp_address_timeout = val;
        return WM_SUCCESS;
    }
}

/* calculate the address to give out to the next DHCP DISCOVER request
 *
 * DHCP clients will be assigned addresses in sequence in the subnet's address space.
 */
static unsigned int next_yiaddr(void)
{
#if CONFIG_DHCP_SERVER_DEBUG
    struct in_addr ip;
#endif
    uint32_t new_ip;
    struct bootp_header *hdr = (struct bootp_header *)(void *)dhcps.msg;

    /* if device requesting for ip address is already registered,
     * if yes, assign previous ip address to it
     */
    new_ip = ac_lookup_mac(hdr->chaddr);
    if (new_ip == (CLIENT_IP_NOT_FOUND))
    {
        /* next IP address in the subnet */
        dhcps.current_ip = ntohl(dhcps.my_ip & dhcps.netmask) | ((dhcps.current_ip + 1U) & ntohl(~dhcps.netmask));
        while (!ac_valid_ip(dhcps.current_ip))
        {
            dhcps.current_ip = ntohl(dhcps.my_ip & dhcps.netmask) | ((dhcps.current_ip + 1) & ntohl(~dhcps.netmask));
        }

        new_ip = htonl(dhcps.current_ip);

        if (ac_add(hdr->chaddr, new_ip) != WM_SUCCESS)
        {
            dhcp_w("No space to store new mapping..");
        }
    }

#if CONFIG_DHCP_SERVER_DEBUG
    ip.s_addr = new_ip;
    dhcp_d("New client IP will be %s", inet_ntoa(ip));
    ip.s_addr = dhcps.my_ip & dhcps.netmask;
#endif

    return new_ip;
}

static unsigned int make_response(char *msg, enum dhcp_message_type type)
{
    struct bootp_header *hdr;
    struct bootp_option *opt;
    char *offset = msg;

    hdr         = (struct bootp_header *)(void *)offset;
    hdr->op     = BOOTP_OP_RESPONSE;
    hdr->htype  = 1;
    hdr->hlen   = 6;
    hdr->hops   = 0;
    hdr->ciaddr = 0;
    hdr->yiaddr = (type == DHCP_MESSAGE_ACK) ? dhcps.client_ip : 0U;
    hdr->yiaddr = (type == DHCP_MESSAGE_OFFER) ? next_yiaddr() : hdr->yiaddr;
    hdr->siaddr = 0;
    hdr->riaddr = 0;
    offset += sizeof(struct bootp_header);

    opt                    = (struct bootp_option *)(void *)offset;
    opt->type              = BOOTP_OPTION_DHCP_MESSAGE;
    *(uint8_t *)opt->value = (uint8_t)type;
    opt->length            = 1;
    offset += sizeof(struct bootp_option) + opt->length;

    if (type == DHCP_MESSAGE_NAK)
    {
        return (unsigned int)(offset - msg);
    }

    opt       = (struct bootp_option *)(void *)offset;
    opt->type = BOOTP_OPTION_SUBNET_MASK;
    write_u32(opt->value, dhcps.netmask);
    opt->length = 4;
    offset += sizeof(struct bootp_option) + opt->length;

    opt       = (struct bootp_option *)(void *)offset;
    opt->type = BOOTP_OPTION_ADDRESS_TIME;
    write_u32(opt->value, htonl(dhcp_address_timeout));
    opt->length = 4;
    offset += sizeof(struct bootp_option) + opt->length;

    opt       = (struct bootp_option *)(void *)offset;
    opt->type = BOOTP_OPTION_DHCP_SERVER_ID;
    write_u32(opt->value, dhcps.my_ip);
    opt->length = 4;
    offset += sizeof(struct bootp_option) + opt->length;

    opt       = (struct bootp_option *)(void *)offset;
    opt->type = BOOTP_OPTION_ROUTER;
    write_u32(opt->value, dhcps.my_ip);
    opt->length = 4;
    offset += sizeof(struct bootp_option) + opt->length;

    opt       = (struct bootp_option *)(void *)offset;
    opt->type = BOOTP_OPTION_NAMESERVER;
    write_u32(opt->value, dns_get_nameserver());
    opt->length = 4;
    offset += sizeof(struct bootp_option) + opt->length;

    opt       = (struct bootp_option *)(void *)offset;
    opt->type = BOOTP_END_OPTION;
    offset++;

    return (unsigned int)(offset - msg);
}

int dhcp_get_ip_from_mac(uint8_t *client_mac, uint32_t *client_ip)
{
    *client_ip = ac_lookup_mac(client_mac);
    if (*client_ip == CLIENT_IP_NOT_FOUND)
    {
        return -WM_FAIL;
    }
    return WM_SUCCESS;
}

int dhcp_send_response(int sock, struct sockaddr *addr, char *msg, int len)
{
    int nb;
    unsigned int sent = 0;
    while (sent < len)
    {
        nb = sendto(sock, msg + sent, len - sent, 0, addr, sizeof(struct sockaddr_in));
        if (nb < 0)
        {
            dhcp_e("failed to send response");
            return -WM_E_DHCPD_RESP_SEND;
        }
        sent += nb;
    }

    dhcp_d("sent response, %d bytes %s", sent, inet_ntoa(((struct sockaddr_in *)addr)->sin_addr));
    return WM_SUCCESS;
}

static int process_dhcp_message(char *msg, int len)
{
    struct bootp_header *hdr;
    struct bootp_option *opt;
    uint8_t response_type = (uint8_t)DHCP_NO_RESPONSE;
    unsigned int consumed = 0;
    bool got_ip           = 0;
    bool need_ip          = 0;
    int ret               = WM_SUCCESS;
    bool got_client_ip    = 0;
    uint32_t new_ip;

    if ((msg == NULL) || len < sizeof(struct bootp_header) + sizeof(struct bootp_option) + 1U)
    {
        return -WM_E_DHCPD_INVALID_INPUT;
    }

    hdr = (struct bootp_header *)(void *)msg;

    switch (hdr->op)
    {
        case BOOTP_OP_REQUEST:
            dhcp_d("bootp request");
            break;
        case BOOTP_OP_RESPONSE:
            dhcp_d("bootp response");
            break;
        default:
            dhcp_e("invalid op code: %d", hdr->op);
            return -WM_E_DHCPD_INVALID_OPCODE;
    }

    if (hdr->htype != 1U || hdr->hlen != 6U)
    {
        dhcp_e("invalid htype or hlen");
        return -WM_E_DHCPD_INCORRECT_HEADER;
    }

    dhcp_d("client MAC: %02X:%02X:%02X:%02X:%02X:%02X", hdr->chaddr[0], hdr->chaddr[1], hdr->chaddr[2], hdr->chaddr[3],
           hdr->chaddr[4], hdr->chaddr[5]);

    dhcp_d("magic cookie: 0x%X", hdr->cookie);

    len -= sizeof(struct bootp_header);
    opt = (struct bootp_option *)(void *)(msg + sizeof(struct bootp_header));
    while (len > 0 && opt->type != BOOTP_END_OPTION)
    {
        if (opt->type == BOOTP_OPTION_DHCP_MESSAGE && opt->length == 1U)
        {
            dhcp_d("found DHCP message option");
            switch (*(uint8_t *)opt->value)
            {
                case DHCP_MESSAGE_DISCOVER:
                    dhcp_d("DHCP discover");
                    response_type = (uint8_t)DHCP_MESSAGE_OFFER;
                    break;

                case DHCP_MESSAGE_REQUEST:
                    dhcp_d("DHCP request");
                    need_ip = 1;
                    if (hdr->ciaddr != 0x0000000U)
                    {
                        dhcps.client_ip = hdr->ciaddr;
                        got_client_ip   = 1;
                    }
                    break;

                default:
                    dhcp_d("ignoring message type %d", *(uint8_t *)opt->value);
                    break;
            }
        }
        if (opt->type == BOOTP_OPTION_REQUESTED_IP && opt->length == 4U)
        {
            dhcp_d("found REQUESTED IP option %hhu.%hhu.%hhu.%hhu", opt->value[0], opt->value[1], opt->value[2],
                   opt->value[3]);
            (void)memcpy((uint8_t *)&dhcps.client_ip, (uint8_t *)opt->value, 4);
            got_client_ip = 1;
        }

        if (got_client_ip)
        {
            /* requested address outside of subnet */
            if ((dhcps.client_ip & dhcps.netmask) == (dhcps.my_ip & dhcps.netmask))
            {
                /* When client requests an IP address,
                 * DHCP-server checks if the valid
                 * IP-MAC entry is present in the
                 * ip-mac cache, if yes, also checks
                 * if the requested IP is same as the
                 * IP address present in IP-MAC entry,
                 * if yes, it allows the device to
                 * continue with the requested IP
                 * address.
                 */
                new_ip = ac_lookup_mac(hdr->chaddr);
                if (new_ip != (CLIENT_IP_NOT_FOUND))
                {
                    /* if new_ip is equal to requested ip */
                    if (new_ip == dhcps.client_ip)
                    {
                        got_ip = 1;
                    }
                    else
                    {
                        got_ip = 0;
                    }
                }
                else if (ac_valid_ip(ntohl(dhcps.client_ip)))
                {
                    /* When client requests with an IP
                     * address that is within subnet range
                     * and not assigned to any other client,
                     * then dhcp-server allows that device
                     * to continue with that IP address.
                     * And if IP-MAC cache is not full then
                     * adds this entry in cache.
                     */
                    if (ac_not_full())
                    {
                        (void)ac_add(hdr->chaddr, dhcps.client_ip);
                    }
                    else
                    {
                        dhcp_w(
                            "No space to store new "
                            "mapping..");
                    }
                    got_ip = 1;
                }
                else
                { /* Do Nothing */
                }
            }
        }

        /* look at the next option (if any) */
        consumed = sizeof(struct bootp_option) + opt->length;
        len -= consumed;
        opt = (struct bootp_option *)(void *)((char *)opt + consumed);
        if (need_ip)
        {
            response_type = (uint8_t)(got_ip ? DHCP_MESSAGE_ACK : DHCP_MESSAGE_NAK);
        }
    }

    if (response_type != DHCP_NO_RESPONSE)
    {
        ret = make_response(msg, (enum dhcp_message_type)response_type);
        ret = SEND_RESPONSE(dhcps.sock, (struct sockaddr *)(void *)&dhcps.baddr, msg, ret);
        if (response_type == DHCP_MESSAGE_ACK)
        {
            (void)send_gratuitous_arp(dhcps.my_ip);
        }
        return WM_SUCCESS;
    }

    dhcp_d("ignoring DHCP packet");
    return WM_SUCCESS;
}

static void dhcp_clean_sockets(void)
{
    int ret;

#ifndef __ZEPHYR__

    if (ctrl != -1)
    {
        ret = net_close(ctrl);
        if (ret != 0)
        {
            dhcp_w("Failed to close control socket: %d", net_get_sock_error(ctrl));
        }
        ctrl = -1;
    }
#endif

    if (dhcps.sock != -1)
    {
        ret = net_close(dhcps.sock);
        if (ret != 0)
        {
            dhcp_w("Failed to close dhcp socket: %d", net_get_sock_error(dhcps.sock));
        }
        dhcps.sock = -1;
    }
}

#ifdef __ZEPHYR__

static int register_ctrl_sock(void)
{
    int ret;

    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, ctrl_sockpair);

    if (ret != 0)
    {
        return -1;
    }

    return 0;
}
#endif

void dhcpd_task(void *arg)
{
    int ret;
    struct sockaddr_in caddr;
#ifndef __ZEPHYR__
    static int one = 1;
    struct sockaddr_in ctrl_listen;
    int addr_len = 0;
#endif
    int max_sock;
    int len;
    socklen_t flen = sizeof(caddr);
    fd_set rfds;

#ifndef __ZEPHYR__

    (void)memset(&ctrl_listen, 0, sizeof(struct sockaddr_in));

    /* create listening control socket */
    ctrl = net_socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ctrl < 0)
    {
        ret = net_get_sock_error(ctrl);
        if (ret != 0)
        {
            dhcp_e("Failed to create control socket: %d.", ret);
        }

        goto done;
    }
    if (setsockopt(ctrl, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one)) == -1)
    {
        dhcp_e("failed to set SO_REUSEADDR");
        (void)net_close(ctrl);
        goto done;
    }
    ctrl_listen.sin_family      = PF_INET;
    ctrl_listen.sin_port        = htons(CTRL_PORT);
    ctrl_listen.sin_addr.s_addr = net_inet_aton("127.0.0.1");
    addr_len                    = (int)sizeof(struct sockaddr_in);
    ret                         = net_bind(ctrl, (struct sockaddr *)(void *)&ctrl_listen, addr_len);
    if (ret < 0)
    {
        dhcp_e("Failed to bind control socket: %d ret %d", ctrl, ret);
        goto done;
    }
#else
    ret = register_ctrl_sock();
    if (ret < 0)
    {
        dhcp_e("Failed to create control socket: %d.", ret);
        goto done;
    }
#endif

    OSA_MutexLock((osa_mutex_handle_t)dhcpd_mutex_Handle, osaWaitForever_c);

    while (true)
    {
        FD_ZERO(&rfds);
        FD_SET(dhcps.sock, &rfds);
#ifndef __ZEPHYR__
        FD_SET(ctrl, &rfds);
#else
        FD_SET(ctrl_sockpair[0], &rfds);
#endif
        max_sock = dns_get_maxsock(&rfds);

#ifndef __ZEPHYR__
        max_sock = (max_sock > ctrl) ? max_sock : ctrl;
#endif

        ret = net_select(max_sock + 1, &rfds, NULL, NULL, NULL);

        /* Error in select? */
        if (ret < 0)
        {
            dhcp_e("select failed: %d", ret);
            goto done;
        }

        /* check the control socket */
#ifndef __ZEPHYR__
        if (FD_ISSET(ctrl, &rfds) != 0)
        {
            ret = recvfrom(ctrl, ctrl_msg, sizeof(ctrl_msg), 0, (struct sockaddr *)0, (socklen_t *)0);
#else
        if (FD_ISSET(ctrl_sockpair[0], &rfds) != 0)
        {
            ret = recv(ctrl_sockpair[0], &ctrl_msg, sizeof(ctrl_msg), 0);
#endif
            if (ret == -1)
            {
                dhcp_e(
                    "Failed to get control"
                    " message: %d\r\n",
#ifndef __ZEPHYR__
                    ctrl
#else
                    ctrl_sockpair[0]
#endif
                );
            }
            else
            {
                if (strcmp(ctrl_msg, "HALT") == 0)
                {
                    goto done;
                }
            }
        }

        if (FD_ISSET(dhcps.sock, &rfds) != 0)
        {
            len = recvfrom(dhcps.sock, dhcps.msg, sizeof(dhcps.msg), 0, (struct sockaddr *)(void *)&caddr, &flen);
            if (len > 0)
            {
                dhcp_d("recved msg on dhcp sock len: %d", len);
                (void)process_dhcp_message(dhcps.msg, len);
            }
        }

        dns_process_packet();
    }

done:
    dhcp_clean_sockets();
    dns_free_allocations();
#ifndef __ZEPHYR__
#if CONFIG_WPA_SUPP
    netconn_thread_cleanup();
#endif
#else
    close(ctrl_sockpair[0]);
    close(ctrl_sockpair[1]);
#endif
    (void)OSA_MutexUnlock((osa_mutex_handle_t)dhcpd_mutex_Handle);
    while (true)
    {
        OSA_TimeDelay(60000);
    }
}

int dhcp_create_and_bind_udp_socket(struct sockaddr_in *address, void *intrfc_handle)
{
    int one = 1;
    int ret;
    struct ifreq req;

    (void)memset(req.ifr_name, 0, sizeof(req.ifr_name));
    (void)net_get_if_name(req.ifr_name, intrfc_handle);

    int sock = net_socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        dhcp_e("failed to create a socket");
        return -WM_FAIL;
    }

    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));
    if (ret == -1)
    {
        /* This is unimplemented in lwIP, hence do not return */
        dhcp_e("failed to set SO_REUSEADDR");
    }

#ifndef __ZEPHYR__
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&one, sizeof(one)) == -1)
    {
        dhcp_e("failed to set SO_BROADCAST");
        (void)net_close(sock);
        return -WM_FAIL;
    }
#endif

    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, &req, sizeof(struct ifreq)) == -1)
    {
        dhcp_e("failed to set SO_BINDTODEVICE");
        (void)net_close(sock);
        return -WM_FAIL;
    }

    (void)net_socket_blocking(sock, NET_BLOCKING_OFF);

    ret = net_bind(sock, (struct sockaddr *)(void *)address, sizeof(struct sockaddr));

    if (ret != 0)
    {
        dhcp_e("failed to bind server socket");
        dhcp_e("socket err: %d", net_get_sock_error(sock));
        (void)net_close(sock);
        return -WM_FAIL;
    }
    return sock;
}

int dhcp_server_init(void *intrfc_handle)
{
    int ret = WM_SUCCESS;
    osa_status_t status;

    (void)memset(&dhcps, 0, sizeof(dhcps));

    status = OSA_MutexCreate((osa_mutex_handle_t)dhcpd_mutex_Handle);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_E_DHCPD_MUTEX_CREATE;
    }

    get_broadcast_addr(&dhcps.baddr);
    dhcps.baddr.sin_port = htons(DHCP_CLIENT_PORT);

    if (get_ip_addr_from_interface(&dhcps.my_ip, intrfc_handle) < 0)
    {
        dhcp_e("failed to look up our IP address from interface");
        ret = -WM_E_DHCPD_IP_ADDR;
        goto out;
    }

    if (get_netmask_from_interface(&dhcps.netmask, intrfc_handle) < 0)
    {
        dhcp_e("failed to look up our netmask from interface");
        ret = -WM_E_DHCPD_NETMASK;
        goto out;
    }

    dhcps.saddr.sin_family      = AF_INET;
    dhcps.saddr.sin_addr.s_addr = INADDR_ANY;
    dhcps.saddr.sin_port        = htons(DHCP_SERVER_PORT);
    dhcps.sock                  = dhcp_create_and_bind_udp_socket(&dhcps.saddr, intrfc_handle);

    if (dhcps.sock < 0)
    {
        ret = -WM_E_DHCPD_SOCKET;
        goto out;
    }

    ret = dns_server_init(intrfc_handle);
    if (ret != WM_SUCCESS)
    {
        dhcp_e("Failed to start dhcp server. Err: %d", ret);
        goto out;
    }

    return WM_SUCCESS;

out:
    (void)OSA_MutexDestroy((osa_mutex_handle_t)dhcpd_mutex_Handle);
    return ret;
}

static int send_ctrl_msg(const char *msg)
{
    int ret;
#ifndef __ZEPHYR__
    int ctrl_tmp;
    struct sockaddr_in to_addr;

    /*
     * Create a temporary socket and send "HALT" message to control
     * socket listening on port CTRL_PORT (12679), this is done in order
     * to make sure that same socket does not get used from multiple
     * thread contexts.
     */
    ctrl_tmp = net_socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ctrl_tmp < 0)
    {
        ret = net_get_sock_error(ctrl_tmp);
        if (ret != 0)
        {
            dhcp_e("failed to create socket error:%d", ret);
        }
        return ret;
    }

    (void)memset((char *)&to_addr, 0, sizeof(to_addr));
    to_addr.sin_family      = PF_INET;
    to_addr.sin_port        = htons(CTRL_PORT);
    to_addr.sin_addr.s_addr = net_inet_aton("127.0.0.1");

    ret = sendto(ctrl_tmp, msg, strlen(msg) + 1U, 0, (struct sockaddr *)(void *)&to_addr, sizeof(to_addr));
    if (ret == -1)
    {
        ret = net_get_sock_error(ctrl_tmp);
        if (ret != 0)
        {
            dhcp_e("failed to send ctrl_msg error:%d", ret);
        }
    }
    else
    {
        ret = WM_SUCCESS;
    }

    (void)net_close(ctrl_tmp);
#else
    unsigned int retry = 0;

    if (ctrl_sockpair[1] < 0)
    {
        return -1;
    }

retry_send:
    ret = send(ctrl_sockpair[1], msg, strlen(msg), 0);
    if (ret < 0)
    {
        if (errno == EINTR || errno == EAGAIN || errno == EBUSY || errno == EWOULDBLOCK)
        {
            k_msleep(2);
            if (retry++ < 3)
            {
                goto retry_send;
            }
            else
            {
                dhcp_e("failed to send ctrl_msg error:%d", ret);
                return -1;
            }
        }
        else
        {
            dhcp_e("failed to send ctrl_msg error:%d", ret);
            return -1;
        }
    }

    ret = WM_SUCCESS;
#endif
    return ret;
}

int dhcp_send_halt(void)
{
    int ret = WM_SUCCESS;

    ret = send_ctrl_msg("HALT");
    if (ret != 0)
    {
        dhcp_w("Failed to send HALT: %d.", ret);
        return -WM_FAIL;
    }

    ret = dhcp_free_allocations();
    return ret;
}

int dhcp_free_allocations(void)
{
    osa_status_t status;

    /* Wait for 10 seconds */
    status = OSA_MutexLock((osa_mutex_handle_t)dhcpd_mutex_Handle, 10000);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }

    dhcp_clean_sockets();
    dns_free_allocations();

    status = OSA_MutexUnlock((osa_mutex_handle_t)dhcpd_mutex_Handle);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }

    return OSA_MutexDestroy((osa_mutex_handle_t)dhcpd_mutex_Handle);
}

static int send_gratuitous_arp(uint32_t ip)
{
    int sock;
    struct arp_packet pkt;
    struct sockaddr_in to_addr;
    to_addr.sin_family      = AF_INET;
    to_addr.sin_addr.s_addr = ip;
    pkt.frame_type          = htons(ARP_FRAME_TYPE);
    pkt.hw_type             = htons(ETHER_HW_TYPE);
    pkt.prot_type           = htons(IP_PROTO_TYPE);
    pkt.hw_addr_size        = ETH_HW_ADDR_LEN;
    pkt.prot_addr_size      = IP_ADDR_LEN;
    pkt.op                  = htons(OP_ARP_REQUEST);

    write_u32(pkt.sndr_ip_addr, ip);
    write_u32(pkt.rcpt_ip_addr, ip);

    (void)memset(pkt.targ_hw_addr, 0xff, ETH_HW_ADDR_LEN);
    (void)memset(pkt.rcpt_hw_addr, 0xff, ETH_HW_ADDR_LEN);
    (void)wlan_get_mac_address_uap(pkt.sndr_hw_addr);
    (void)memcpy(pkt.src_hw_addr, pkt.sndr_hw_addr, ETH_HW_ADDR_LEN);
    sock = net_socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        dhcp_e("Could not open socket to send Gratuitous ARP");
        return -WM_E_DHCPD_SOCKET;
    }
    (void)memset(pkt.padding, 0, sizeof(pkt.padding));

    if (sendto(sock, (char *)&pkt, sizeof(pkt), 0, (struct sockaddr *)(void *)&to_addr, sizeof(to_addr)) < 0)
    {
        dhcp_e("Failed to send Gratuitous ARP");
        (void)net_close(sock);
        return -WM_E_DHCPD_ARP_SEND;
    }
    dhcp_d("Gratuitous ARP sent");
    (void)net_close(sock);
    return WM_SUCCESS;
}

static void get_broadcast_addr(struct sockaddr_in *addr)
{
    addr->sin_family = AF_INET;
    /* limited broadcast addr (255.255.255.255) */
    addr->sin_addr.s_addr = 0xffffffffU;
#ifndef __ZEPHYR__
    addr->sin_len = (uint8_t)sizeof(struct sockaddr_in);
#endif
}

static int get_ip_addr_from_interface(uint32_t *ip, void *interface_handle)
{
    return net_get_if_ip_addr(ip, interface_handle);
}

static int get_netmask_from_interface(uint32_t *nm, void *interface_handle)
{
    return net_get_if_ip_mask(nm, interface_handle);
}

#ifndef __ZEPHYR__
void dhcp_stat(void)
{
    int i = 0;
    struct ip4_addr saddr;
    (void)PRINTF("DHCP Server Lease Duration : %d seconds\r\n", (int)dhcp_address_timeout);
    if (dhcps.count_clients == 0)
    {
        (void)PRINTF("No IP-MAC mapping stored\r\n");
    }
    else
    {
        (void)PRINTF("Client IP\tClient MAC\r\n");
        for (i = 0; i < dhcps.count_clients && i < MAC_IP_CACHE_SIZE; i++)
        {
            saddr.addr = dhcps.ip_mac_mapping[i].client_ip;
            (void)PRINTF("%s\t%02X:%02X:%02X:%02X:%02X:%02X\r\n", inet_ntoa(saddr),
                         dhcps.ip_mac_mapping[i].client_mac[0], dhcps.ip_mac_mapping[i].client_mac[1],
                         dhcps.ip_mac_mapping[i].client_mac[2], dhcps.ip_mac_mapping[i].client_mac[3],
                         dhcps.ip_mac_mapping[i].client_mac[4], dhcps.ip_mac_mapping[i].client_mac[5]);
        }
    }
}
#endif
