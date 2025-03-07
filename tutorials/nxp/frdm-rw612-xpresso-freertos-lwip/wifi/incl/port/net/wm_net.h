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

#ifndef _WM_NET_H_
#define _WM_NET_H_

#include <string.h>

#include <osa.h>
#include <wmtypes.h>
#include <wmerrno.h>

#if defined(SDK_OS_FREE_RTOS)

#include <lwip/opt.h>
#include <lwip/sys.h>
#include <lwip/tcpip.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <lwip/stats.h>
#include <lwip/icmp.h>
#include <lwip/igmp.h>
#include <lwip/ip.h>
#include <lwip/inet_chksum.h>
#include <lwip/pbuf.h>
#include <lwip/api.h>
#include <lwip/netifapi.h>
#include <lwip/dns.h>
#include <lwip/dhcp.h>
#include <lwip/prot/dhcp.h>
#include <lwip/ip_addr.h>
#include <lwip/prot/autoip.h>
#include <netif/etharp.h>

#ifndef LWIP_TCPIP_CORE_LOCKING
#error "LWIP TCP/IP Core Locking is not enabled"
#endif

#if CONFIG_IPV6 && !LWIP_IPV6
#error "CONFIG_IPV6 is enabled, but LWIP_IPV6 is not, enable it from lwipopts.h"
#elif LWIP_IPV6 && !CONFIG_IPV6
#error "LWIP_IPV6 is enabled, but CONFIG_IPV6 is not, enable it from wifi_config.h"
#endif

#if CONFIG_IPV6 && LWIP_IPV6
#if !CONFIG_MAX_IPV6_ADDRESSES
#error "Define CONFIG_MAX_IPV6_ADDRESSES same as LWIP_IPV6_NUM_ADDRESSES in wifi_config.h"
#else
#if CONFIG_MAX_IPV6_ADDRESSES != LWIP_IPV6_NUM_ADDRESSES
#error "CONFIG_MAX_IPV6_ADDRESSES must be equal to LWIP_IPV6_NUM_ADDRESSES"
#endif
#endif
#endif

#if (!defined(LWIP_NETIF_EXT_STATUS_CALLBACK) || (LWIP_NETIF_EXT_STATUS_CALLBACK == 0))
#error "Define LWIP_NETIF_EXT_STATUS_CALLBACK as 1 in lwipopts.h"
#endif

#if CONFIG_WPA_SUPP
#if (!defined(LWIP_NUM_NETIF_CLIENT_DATA) || (LWIP_NUM_NETIF_CLIENT_DATA < 2))
#error "Define LWIP_NUM_NETIF_CLIENT_DATA atleast 2 in lwipopts.h"
#endif
#endif

#elif __ZEPHYR__

#include <zephyr/kernel.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_pkt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

#if (CONFIG_POSIX_API)
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/sys/select.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/unistd.h>
#else
#include <zephyr/net/socket.h>
#endif

#define NETIF_NAMESIZE       6
#define NETIF_MAX_HWADDR_LEN 6

/* copy zephyr struct net if */
struct netif
{
    /** The net_if_dev instance the net_if is related to */
    struct net_if_dev *if_dev;
#if (CONFIG_NET_STATISTICS_PER_INTERFACE)
    /** Network statistics related to this network interface */
    struct net_stats stats;
#endif /* CONFIG_NET_STATISTICS_PER_INTERFACE */

    /** Network interface instance configuration */
    struct net_if_config config;

#if (CONFIG_NET_POWER_MANAGEMENT)
    /** Keep track of packets pending in traffic queues. This is
     * needed to avoid putting network device driver to sleep if
     * there are packets waiting to be sent.
     */
    int tx_pending;
#endif

    struct k_mutex lock;
    struct k_mutex tx_lock;
};

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif
{
    struct net_eth_addr ethaddr;
    /* Interface to bss type identification that tells the FW wherether
       the data is for STA for UAP */
    uint8_t interface;
    /* Add whatever per-interface state that is needed here. */
};

typedef struct
{
    struct net_if *netif;
    uint8_t mac_address[6];
    struct net_addr ipaddr;
    struct net_addr nmask;
    struct net_addr gw;
    struct ethernetif state;
#if (CONFIG_NET_STATISTICS_WIFI)
    struct net_stats_wifi stats;
#endif
    scan_result_cb_t scan_cb;
    uint16_t max_bss_cnt;
} interface_t;
#elif defined(FSL_RTOS_THREADX)

#include "nx_api.h"
// #include "nxd_bsd.h"

#endif

#define NET_SUCCESS WM_SUCCESS
#define NET_ERROR   (-WM_FAIL)
#define NET_ENOBUFS ENOBUFS

#define NET_BLOCKING_OFF 1
#define NET_BLOCKING_ON  0

/* Error Codes
 * lwIP provides all standard errnos defined in arch.h, hence no need to
 * redefine them here.
 * */

/* To be consistent with naming convention */
#define net_socket(domain, type, protocol)            socket(domain, type, protocol)
#define net_select(nfd, read, write, except, timeout) select(nfd, read, write, except, timeout)
#define net_bind(sock, addr, len)                     bind(sock, addr, len)
#define net_listen(sock, backlog)                     listen(sock, backlog)
#define net_close(c)                                  close((c))
#define net_accept(sock, addr, len)                   accept(sock, addr, len)
#define net_shutdown(c, b)                            shutdown(c, b)
#define net_connect(sock, addr, len)                  connect(sock, addr, len)
#define net_read(sock, data, len)                     read(sock, data, len)
#define net_write(sock, data, len)                    write(sock, data, len)

#ifdef __ZEPHYR__
/* directly map this to the zephyr internal functions */
#define inet_aton(cp, addr) inet_pton(AF_INET, cp, (char *)addr)
#endif

enum net_address_types
{
    /** static IP address */
    NET_ADDR_TYPE_STATIC = 0,
    /** Dynamic  IP address*/
    NET_ADDR_TYPE_DHCP = 1,
    /** Link level address */
    NET_ADDR_TYPE_LLA = 2,
    /** For Bridge Mode, no IP address */
    NET_ADDR_TYPE_BRIDGE_MODE = 3,
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

/** Set hostname for network interface
 *
 * \param[in] hostname Hostname to be set.
 *
 * \note NULL is a valid value for hostname.
 *
 * \return WM_SUCESS
 */
int net_dhcp_hostname_set(char *hostname);

/** Deactivate the dhcp timer
 *
 */
void net_stop_dhcp_timer(void);

/** Set socket blocking option as on or off
 *
 * \param[in] sock socket number to be set for blocking option.
 * \param[in] state set blocking on or off
 *
 * \return WM_SUCESS otherwise standard LWIP error codes.
 */
static inline int net_socket_blocking(int sock, int state)
{
#if defined(SDK_OS_FREE_RTOS)
    return ioctlsocket(sock, FIONBIO, &state);
#elif __ZEPHYR__
    /* TODO: implement */
    return 0;
#endif
}

/** Get error number from provided socket
 *
 * \param[in] sock socket number to get error number.
 *
 * \return error number.
 */
static inline int net_get_sock_error(int sock)
{
    int ret = 0;
#if defined(SDK_OS_FREE_RTOS)

    switch (errno)
    {
        case EWOULDBLOCK:
            ret = -WM_E_AGAIN;
            break;
        case EBADF:
            ret = -WM_E_BADF;
            break;
        case ENOBUFS:
            ret = -WM_E_NOMEM;
            break;
        default:
            ret = errno;
            break;
    }
#endif

    return ret;
}

/** Converts Internet host address from the IPv4 dotted-decimal notation into
 * binary form (in network byte order)
 *
 * \param[in] cp IPv4 host address in dotted-decimal notation.
 *
 * \return IPv4 address in binary form
 */
static inline uint32_t net_inet_aton(const char *cp)
{
#if defined(SDK_OS_FREE_RTOS)

    struct in_addr addr;
    addr.s_addr = 0;

#if defined(SDK_OS_FREE_RTOS)
    (void)inet_aton(cp, ((void *)&addr));
#elif __ZEPHYR__
    (void)net_addr_pton(AF_INET, cp, &addr);
#endif
    return addr.s_addr;
#else
    char *token;
    char *rest = (char *)cp;
    uint32_t addr[4], i = 0;

    while ((token = strtok_r(rest, ".", &rest)))
    {
        addr[i++] = atoi(token);
    }

    return IP_ADDRESS(addr[0], addr[1], addr[2], addr[3]);
#endif
}

/** set MAC hardware address to lwip network interface
 *
 * \param[in] stamac sta MAC address.
 * \param[in] uapmac uap MAC address.
 *
 */
void net_wlan_set_mac_address(unsigned char *stamac, unsigned char *uapmac);

#if defined(SDK_OS_FREE_RTOS)

/** Skip a number of bytes at the start of a stack buffer
 *
 * \param[in] buf input stack buffer.
 * \param[in] in_offset offset to skip.
 *
 * \return the payload pointer after skip a number of bytes
 */
static inline uint8_t *net_stack_buffer_skip(void *buf, uint16_t in_offset)
{
#if defined(SDK_OS_FREE_RTOS)
    uint16_t out_offset = 0;
    struct pbuf *p      = pbuf_skip((struct pbuf *)buf, in_offset, &out_offset);
    return (uint8_t *)(p->payload) + out_offset;
#elif __ZEPHYR__
    uint16_t offset_left = in_offset;
    struct net_buf *frag = ((struct net_pkt *)buf)->frags;
    while (frag && (frag->len <= offset_left))
    {
        offset_left = offset_left - frag->len;
        frag        = frag->frags;
    }
    return (uint8_t *)(frag->data + offset_left);
#endif
}

/** Free a buffer allocated from stack memory
 *
 * \param[in] buf stack buffer pointer.
 *
 */
static inline void net_stack_buffer_free(void *buf)
{
#if defined(SDK_OS_FREE_RTOS)
    pbuf_free((struct pbuf *)buf);
#elif __ZEPHYR__
    net_pkt_unref((struct net_pkt *)buf);
#endif
}

/** Copy (part of) the contents of a packet buffer to an application supplied buffer
 *
 * \param[in] stack_buffer the stack buffer from which to copy data.
 * \param[in] dst the destination buffer.
 * \param[in] len length of data to copy.
 * \param[in] offset offset into the stack buffer from where to begin copying
 * \return copy status based on stack definition.
 */
#if defined(SDK_OS_FREE_RTOS)
static inline int net_stack_buffer_copy_partial(void *stack_buffer, void *dst, uint16_t len, uint16_t offset)
{
    return pbuf_copy_partial((const struct pbuf *)stack_buffer, dst, len, offset);
}
#elif __ZEPHYR__
int net_stack_buffer_copy_partial(void *stack_buffer, void *dst, uint16_t len, uint16_t offset);
#endif

/** Get the data payload inside the stack buffer.
 *
 * \param[in] buf input stack buffer.
 *
 * \return the payload pointer of the stack buffer.
 */
static inline void *net_stack_buffer_get_payload(void *buf)
{
#if defined(SDK_OS_FREE_RTOS)
    return ((struct pbuf *)buf)->payload;
#elif __ZEPHYR__
    return net_pkt_data((struct net_pkt *)buf);
#endif
}

#if CONFIG_WIFI_PKT_FWD
/** Send packet from Wi-Fi driver
 *
 * \param[in] interface Wi-Fi interface.
 * \param[in] stack_buffer net stack buffer pointer.
 *
 * \return WM_SUCCESS on success
 * \return -WM_FAIL otherwise
 */
int net_wifi_pkt_fwd(uint8_t interface, void *stack_buffer);
#endif

/**
 * Get network host entry
 *
 * \param[in] cp Hostname or an IPv4 address in the standard dot notation.
 * \param[in] hentry Pointer to pointer of host entry structure.
 *
 * \note This function is not thread safe. If thread safety is required
 * please use lwip_getaddrinfo() - lwip_freeaddrinfo() combination.
 *
 * \return WM_SUCESS if operation successful.
 * \return -WM_FAIL if operation fails.
 */
static inline int net_gethostbyname(const char *cp, struct hostent **hentry)
{
    struct hostent *he;
    if ((he = gethostbyname(cp)) == NULL)
    {
        return -WM_FAIL;
    }

    *hentry = he;

    return WM_SUCCESS;
}
#else
/** Skip a number of bytes at the start of a stack buffer
 *
 * \param[in] buf input stack buffer.
 * \param[in] in_offset offset to skip.
 *
 * \return the payload pointer after skip a number of bytes
 */
static inline uint8_t *net_stack_buffer_skip(void *buf, uint16_t in_offset)
{
    uint16_t out_offset = in_offset;
    return (uint8_t *)(buf) + out_offset;
}
#endif

/** Converts Internet host address in network byte order to a string in IPv4
 * dotted-decimal notation
 *
 * \param[in] addr IP address in network byte order.
 * \param[out] cp buffer in which IPv4 dotted-decimal string is returned.
 *
 */
static inline void net_inet_ntoa(unsigned long addr, char *cp)
{
#if defined(SDK_OS_FREE_RTOS)
    struct ip4_addr saddr;
    saddr.addr = addr;
    /* No length, sigh! */
    (void)strcpy(cp, inet_ntoa(saddr));
#elif __ZEPHYR__
    struct in_addr saddr;

    saddr.s_addr = addr;
    net_addr_ntop(AF_INET, &saddr, cp, NET_IPV4_ADDR_LEN);
#endif
}
#if defined(SDK_OS_FREE_RTOS)

/** Check whether buffer is IPv4 or IPV6 packet type
 *
 * \param[in] buffer pointer to buffer where packet to be checked located.
 *
 * \return true if buffer packet type matches with IPv4 or IPv6, false otherwise.
 *
 */
static inline bool net_is_ip_or_ipv6(const uint8_t *buffer)
{
#if defined(SDK_OS_FREE_RTOS)
    if (((const struct eth_hdr *)(const void *)buffer)->type == PP_HTONS(ETHTYPE_IP) ||
        ((const struct eth_hdr *)(const void *)buffer)->type == PP_HTONS(ETHTYPE_IPV6))
#elif __ZEPHYR__
    if (((const struct net_eth_hdr *)(const void *)buffer)->type == htons(ETH_P_IP) ||
        ((const struct net_eth_hdr *)(const void *)buffer)->type == htons(ETH_P_IPV6))
#endif
    {
        return true;
    }

    return false;
}
#endif

/** Get interface handle from socket descriptor
 *
 * Given a socket descriptor this API returns which interface it is bound with.
 *
 * \param[in] sock socket descriptor
 *
 * \return[out] interface handle
 */
void *net_sock_to_interface(int sock);

/** Initialize TCP/IP networking stack
 *
 * \return WM_SUCCESS on success
 * \return -WM_FAIL otherwise
 */
int net_wlan_init(void);

/** DiInitialize TCP/IP networking stack
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

#if UAP_SUPPORT
/** Get uAP interface netif structure pointer
 *
 * \return A pointer to uAP interface netif structure
 *
 */
struct netif *net_get_uap_interface(void);
#endif

#if defined(SDK_OS_FREE_RTOS)

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
#endif

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

#if UAP_SUPPORT
/** Get micro-AP interface handle
 *
 * Some APIs require the interface handle to be passed to them. The handle can
 * be retrieved using this API.
 *
 * \return micro-AP interface handle
 */
void *net_get_uap_handle(void);
#endif

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

#if CONFIG_IPV6
/** Get interface IPv6 Addresses & their states in \ref net_ip_config
 *
 * This function will get the IPv6 addresses & address states of a given
 * interface. Use net_get_sta_handle() to get interface handle.
 *
 * \param[out] addr \ref net_ip_config
 * \param[in] intrfc_handle interface handle
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_ipv6_addr(struct net_ip_config *addr, void *intrfc_handle);

/** Get list of preferred IPv6 Addresses of a given interface
 * in \ref net_ip_config
 *
 * This function will get the list of IPv6 addresses whose address state
 * is Preferred.
 * Use net_get_sta_handle() to get interface handle.
 *
 * \param[out] addr \ref net_ip_config
 * \param[in] intrfc_handle interface handle
 *
 * \return Number of IPv6 addresses whose address state is Preferred
 */
int net_get_if_ipv6_pref_addr(struct net_ip_config *addr, void *intrfc_handle);

/** Get the description of IPv6 address state
 *
 * This function will get the IPv6 address state description like -
 * Invalid, Preferred, Deprecated
 *
 * \param[in] addr_state Address state
 *
 * \return IPv6 address state description
 */
char *ipv6_addr_state_to_desc(unsigned char addr_state);

/** Get the description of IPv6 address
 *
 * This function will get the IPv6 address type description like -
 * Linklocal, Global, Sitelocal, Uniquelocal
 *
 * \param[in] ipv6_conf Pointer to IPv6 configuration of type \ref net_ipv6_config
 *
 * \return IPv6 address description
 */
char *ipv6_addr_addr_to_desc(struct net_ipv6_config *ipv6_conf);

/** Get the description of IPv6 address type
 *
 * This function will get the IPv6 address type description like -
 * Linklocal, Global, Sitelocal, Uniquelocal
 *
 * \param[in] ipv6_conf Pointer to IPv6 configuration of type \ref net_ipv6_config
 *
 * \return IPv6 address type description
 */
char *ipv6_addr_type_to_desc(struct net_ipv6_config *ipv6_conf);
#endif /* CONFIG_IPV6 */

/** Get interface Name string containing name and number
 *
 * This function will get the string containing name and number for given interface.
 * Use net_get_sta_handle(), net_get_uap_handle() to get
 * interface handle.
 *
 * \param[out] if_name interface name pointer
 * \param[in] intrfc_handle interface handle
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_name(char *if_name, void *intrfc_handle);

/** Get interface IP Address
 *
 * This function will get the IP Address of a given interface. Use
 * net_get_sta_handle(), net_get_uap_handle() to get
 * interface handle.
 *
 * \param[out] ip ip address pointer
 * \param[in] intrfc_handle interface handle
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_ip_addr(uint32_t *ip, void *intrfc_handle);

/** Get interface IP Subnet-Mask
 *
 * This function will get the Subnet-Mask of a given interface. Use
 * net_get_sta_handle(), net_get_uap_handle() to get
 * interface handle.
 *
 * \param[in] mask Subnet   Mask pointer
 * \param[in] intrfc_handle interface
 *
 * \return WM_SUCCESS on success otherwise error code.
 */
int net_get_if_ip_mask(uint32_t *nm, void *intrfc_handle);

/** Initialize the network stack
 *
 *  This function initializes the network stack. This function is
 *  called by wlan_start().
 *
 *  Applications may optionally call this function directly:
 *  if they wish to use the networking stack (loopback interface)
 *  without the wlan functionality.
 *  if they wish to initialize the networking stack even before wlan
 *  comes up.
 *
 * \note This function may safely be called multiple times.
 */
void net_ipv4stack_init(void);

#if defined(SDK_OS_FREE_RTOS)

#if CONFIG_IPV6

/** Initialize the IPv6 network stack
 *
 * \param[in] netif network interface on which ipv6 stack is initialized.
 *
 */
void net_ipv6stack_init(struct netif *netif);
#endif

#endif

#if defined(FSL_RTOS_THREADX)
void dhcp_stat(void);
#endif

/** Display network statistics
 */
void net_stat(void);


#ifdef MGMT_RX
void rx_mgmt_register_callback(int (*rx_mgmt_cb_fn)(const enum wlan_bss_type bss_type,
                                                    const wifi_mgmt_frame_t *frame,
                                                    const size_t len));

void rx_mgmt_deregister_callback(void);
#endif

#ifdef __ZEPHYR__
int nxp_wifi_internal_tx(const struct device *dev, struct net_pkt *pkt);
void nxp_wifi_internal_register_rx_cb(int (*rx_cb_fn)(struct net_if *iface, struct net_pkt *pkt));
const struct netif *net_if_get_binding(const char *ifname);
const struct freertos_wpa_supp_dev_ops *net_if_get_dev_config(struct netif *iface);
#endif

#endif /* _WM_NET_H_ */
