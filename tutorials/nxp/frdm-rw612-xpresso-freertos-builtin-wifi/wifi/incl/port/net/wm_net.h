/*
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*!\file wm_net.h
 *\brief This file provides interface for network abstraction layer.
 *
 * This provides the calls related to the network layer.
 */

// Modified to work with Mongoose built-in TCP/IP by Cesanta Software Ltd.

#ifndef _WM_NET_H_
#define _WM_NET_H_

#include <string.h>

#include <osa.h>
#include <wmtypes.h>
#include <wmerrno.h>

// need this dependency due to assumptions satisfied below
#include "mongoose.h"
// vendor libs have strong lwIP dependencies
#define htonl(x) mg_htonl(x)
#define ipaddr_addr(x) 0    // A Mongoose environment does not need this nor works this way

// instead of exposing wlan.h, libs cast to "the same" structure and enums we must replicate here and expose to them
enum net_address_types
{
    /** static IP address */
    NET_ADDR_TYPE_STATIC = 0,
    /** Dynamic  IP address*/
    NET_ADDR_TYPE_DHCP = 1,
    /** Link level address */
    NET_ADDR_TYPE_LLA = 2,
};

/** This data structure represents an IPv4 address */
struct net_ipv4_config
{
    /** Set to \ref ADDR_TYPE_DHCP to use DHCP to obtain the IP address or
     *  \ref ADDR_TYPE_STATIC to use a static IP. In case of static IP
     *  address ip, gw, netmask and dns members must be specified.  When
     *  using DHCP, the ip, gw, netmask and dns are overwritten by the
     *  values obtained from the DHCP server. They should be zeroed out if
     *  not used. */
    enum net_address_types addr_type;
    /** The system's IP address in network order. */
    unsigned address;
    /** The system's default gateway in network order. */
    unsigned gw;
    /** The system's subnet mask in network order. */
    unsigned netmask;
    /** The system's primary dns server in network order. */
    unsigned dns1;
    /** The system's secondary dns server in network order. */
    unsigned dns2;
};

#if CONFIG_IPV6
/** This data structure represents an IPv6 address */
struct net_ipv6_config
{
    /** The system's IPv6 address in network order. */
    unsigned address[4];
    /** The address type: linklocal, site-local or global. */
    unsigned char addr_type;
    /** The state of IPv6 address (Tentative, Preferred, etc). */
    unsigned char addr_state;
};
#endif

/** Network IP configuration.
 *
 *  This data structure represents the network IP configuration
 *  for IPv4 as well as IPv6 addresses
 */
struct net_ip_config
{
#if CONFIG_IPV6
    /** The network IPv6 address configuration that should be
     * associated with this interface. */
    struct net_ipv6_config ipv6[CONFIG_MAX_IPV6_ADDRESSES];
    /** The network IPv6 valid addresses count */
    size_t ipv6_count;
#endif
    /** The network IPv4 address configuration that should be
     * associated with this interface. */
    struct net_ipv4_config ipv4;
};

// libs only handle one private var, so we need to craft a "packet buffer" structure to be able to hold data length
#pragma pack(push, 1)
struct pb {
  uint16_t len;
  uint8_t data[1];
};
#pragma pack(pop)



/** Deactivate the dhcp timer
 *
 */
void net_stop_dhcp_timer(void);

/** set MAC hardware address to lwip network interface
 *
 * \param[in] stamac sta MAC address.
 * \param[in] uapmac uap MAC address.
 *
 */
void net_wlan_set_mac_address(unsigned char *stamac, unsigned char *uapmac);

/** Skip a number of bytes at the start of a stack buffer
 *
 * \param[in] buf input stack buffer.
 * \param[in] in_offset offset to skip.
 *
 * \return the payload pointer after skip a number of bytes
 */
static inline uint8_t *net_stack_buffer_skip(void *buf, uint16_t in_offset)
{
	return ((struct pb *)buf)->data + in_offset;
}

/** Free a buffer allocated from stack memory
 *
 * \param[in] buf stack buffer pointer.
 *
 */
static inline void net_stack_buffer_free(void *buf)
{
    free(buf);
}

/** Copy (part of) the contents of a packet buffer to an application supplied buffer
 *
 * \param[in] stack_buffer the stack buffer from which to copy data.
 * \param[in] dst the destination buffer.
 * \param[in] len length of data to copy.
 * \param[in] offset offset into the stack buffer from where to begin copying
 * \return copy status based on stack definition.
 */
static inline int net_stack_buffer_copy_partial(void *stack_buffer, void *dst, uint16_t len, uint16_t offset)
{
    memcpy(dst, ((struct pb *)stack_buffer)->data + offset, len);
	return 1;
}

/** Get the data payload inside the stack buffer.
 *
 * \param[in] buf input stack buffer.
 *
 * \return the payload pointer of the stack buffer.
 */
static inline void *net_stack_buffer_get_payload(void *buf)
{
	return ((struct pb *)buf)->data;
}

/** Converts Internet host address in network byte order to a string in IPv4
 * dotted-decimal notation
 *
 * \param[in] addr IP address in network byte order.
 * \param[out] cp buffer in which IPv4 dotted-decimal string is returned.
 *
 */
static inline void net_inet_ntoa(unsigned long addr, char *cp)
{
	// This is only called by WPL, we don't need to call this function so we don't serve this
}

/** Check whether buffer is IPv4 or IPV6 packet type
 *
 * \param[in] buffer pointer to buffer where packet to be checked located.
 *
 * \return true if buffer packet type matches with IPv4 or IPv6, false otherwise.
 *
 */
static inline bool net_is_ip_or_ipv6(const uint8_t *b)
{
	// this does not account for 802.1Q-tagged traffic, no need to, this is called
    // when sending and we don't send that.
    return (b[12]==0x08 && b[13]== 0x00) || (b[12]==0x86 && b[13]== 0xdd);
}

/** Initialize TCP/IP networking stack
 *
 * \return WM_SUCCESS on success
 * \return -WM_FAIL otherwise
 */
int net_wlan_init(void);

/** DeInitialize TCP/IP networking stack
 *
 * \return WM_SUCCESS on success
 * \return -WM_FAIL otherwise
 */
int net_wlan_deinit(void);

/** Get STA interface netif structure pointer
 *
 * \return A pointer to STA interface netif structure
 */
struct netif *net_get_sta_interface(void);

/** Get uAP interface netif structure pointer
 *
 * \return A pointer to uAP interface netif structure
 *
 */
struct netif *net_get_uap_interface(void);

/** Get interface name for given netif
 *
 * \param[out] pif_name Buffer to store interface name
 * \param[in] iface Interface to get the name
 *
 * \return WM_SUCCESS on success
 * \return -WM_FAIL otherwise
 *
 */
int net_get_if_name_netif(char *pif_name, struct netif *iface);

/** Get client data index for storing private data in * netif.
 *
 * \return allocated client data index, -1 if error or
 *         not supported.
 */
int net_alloc_client_data_id();

/** Get station interface handle
 *
 * Some APIs require the interface handle to be passed to them. The handle can
 * be retrieved using this API.
 *
 * \return station interface handle
 */
void *net_get_sta_handle(void);
#define net_get_mlan_handle() net_get_sta_handle()

/** Get micro-AP interface handle
 *
 * Some APIs require the interface handle to be passed to them. The handle can
 * be retrieved using this API.
 *
 * \return micro-AP interface handle
 */
void *net_get_uap_handle(void);

/** Take interface up
 *
 * Change interface state to up. Use net_get_sta_handle(),
 * net_get_uap_handle() to get interface handle.
 *
 * \param[in] intrfc_handle interface handle
 *
 * \return void
 */
void net_interface_up(void *intrfc_handle);

/** Take interface down
 *
 * Change interface state to down. Use net_get_sta_handle(),
 * net_get_uap_handle() to get interface handle.
 *
 * \param[in] intrfc_handle interface handle
 *
 * \return void
 */
void net_interface_down(void *intrfc_handle);

/** Stop DHCP client on given interface
 *
 * Stop the DHCP client on given interface state. Use net_get_sta_handle(),
 * net_get_uap_handle() to get interface handle.
 *
 * \param[in] intrfc_handle interface handle
 *
 * \return void
 */
void net_interface_dhcp_stop(void *intrfc_handle);

/** Cleanup DHCP client on given interface
 *
 * Cleanup the DHCP client on given interface state. Use net_get_sta_handle(),
 * net_get_uap_handle() to get interface handle.
 *
 * \param[in] intrfc_handle interface handle
 *
 */
void net_interface_dhcp_cleanup(void *intrfc_handle);

/** Configure IP address for interface
 *
 * \param[in] addr Address that needs to be configured.
 * \param[in] intrfc_handle Handle for network interface to be configured.
 *
 * \return WM_SUCCESS on success or an error code.
 */
int net_configure_address(struct net_ip_config *addr, void *intrfc_handle);

/** Configure DNS server address
 *
 * \param[in] ip IP address of the DNS server to set
 * \param[in] role Network wireless BSS Role
 *
 */
void net_configure_dns(struct net_ip_config *ip, unsigned int role);

/** Get interface IP Address in \ref net_ip_config
 *
 * This function will get the IP address of a given interface. Use
 * net_get_sta_handle(), net_get_uap_handle() to get
 * interface handle.
 *
 * \param[out] addr \ref net_ip_config
 * \param[in] intrfc_handle interface handle
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_addr(struct net_ip_config *addr, void *intrfc_handle);


#endif /* _WM_NET_H_ */
