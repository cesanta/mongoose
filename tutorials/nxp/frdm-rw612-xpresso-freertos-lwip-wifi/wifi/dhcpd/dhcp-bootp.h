/** @file dhcp-bootp.h
 *
 *  @brief BOOTP Header
 *
 *  Copyright 2008-2020 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __DHCP_BOOTP_H_
#define __DHCP_BOOTP_H_

#define DHCP_SERVER_PORT 67
#define DHCP_CLIENT_PORT 68

#define NAMESERVER_PORT 53

#define BOOTP_OP_REQUEST  1
#define BOOTP_OP_RESPONSE 2

#define BOOTP_OPTION_SUBNET_MASK    1
#define BOOTP_OPTION_ROUTER         3
#define BOOTP_OPTION_NAMESERVER     6
#define BOOTP_OPTION_REQUESTED_IP   50
#define BOOTP_OPTION_ADDRESS_TIME   51
#define BOOTP_OPTION_DHCP_MESSAGE   53U
#define BOOTP_OPTION_DHCP_SERVER_ID 54
#define BOOTP_END_OPTION            0xFF

enum dhcp_message_type
{
    DHCP_MESSAGE_DISCOVER = 1,
    DHCP_MESSAGE_OFFER    = 2,
    DHCP_MESSAGE_REQUEST  = 3,
    DHCP_MESSAGE_DECLINE  = 4,
    DHCP_MESSAGE_ACK      = 5,
    DHCP_MESSAGE_NAK      = 6,
    DHCP_MESSAGE_RELEASE  = 7,
    DHCP_MESSAGE_INFORM   = 8,
    DHCP_NO_RESPONSE      = 255
};

PACK_START struct bootp_header
{
    uint8_t op;            /* message type */
    uint8_t htype;         /* hardware type (1 = ethernet) */
    uint8_t hlen;          /* hardware address length (6) */
    uint8_t hops;          /* (0) */
    uint32_t xid;          /* transaction ID */
    uint16_t secs;         /* seconds elapsed */
    uint16_t flags;        /* bootp flags */
    uint32_t ciaddr;       /* client IP address */
    uint32_t yiaddr;       /* your IP address */
    uint32_t siaddr;       /* next server IP address */
    uint32_t riaddr;       /* relay agent IP address */
    uint8_t chaddr[6];     /* client MAC address */
    uint8_t pad[10 + 192]; /* 10 octets of padding, 192 octets of
          padding */
    uint32_t cookie;       /* magic cookie */
} PACK_END;

PACK_START struct bootp_option
{
    uint8_t type;
    uint8_t length;
    char value[0];
} PACK_END;
#define ETH_P_RARP 0x8035

#define ETH_HW_ADDR_LEN 6
#define IP_ADDR_LEN     4
#define ARP_FRAME_TYPE  0x0806
#define ETHER_HW_TYPE   1
#define IP_PROTO_TYPE   0x0800
#define OP_ARP_REQUEST  2
#define SOCK_PACKET     10

struct arp_packet
{
    uint8_t targ_hw_addr[ETH_HW_ADDR_LEN];
    uint8_t src_hw_addr[ETH_HW_ADDR_LEN];
    uint16_t frame_type;
    uint16_t hw_type;
    uint16_t prot_type;
    uint8_t hw_addr_size;
    uint8_t prot_addr_size;
    uint16_t op;
    uint8_t sndr_hw_addr[ETH_HW_ADDR_LEN];
    char sndr_ip_addr[IP_ADDR_LEN];
    uint8_t rcpt_hw_addr[ETH_HW_ADDR_LEN];
    char rcpt_ip_addr[IP_ADDR_LEN];
    uint8_t padding[18];
};

#endif
