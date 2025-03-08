/*
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <mlan_api.h>
#include <wm_net.h>
#include <wmlog.h>
#ifdef RW610
#include <wifi-imu.h>
#else
#include <wifi-sdio.h>
#endif
#include <wifi-internal.h>

#if defined(SDK_OS_FREE_RTOS)

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/sys.h"
#if CONFIG_IPV6
#include "lwip/ethip6.h"
#endif /* CONFIG_IPV6 */
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ethernet.h"
#include "netif/ppp/pppoe.h"


#define NET_MAC_ADDR_LEN 6

#define NET_IPV4_ADDR_U32(x) (x).in_addr.s_addr

/*
 * Packets of this type need o be handled
 * for WPS and Supplicant
 */
#define ETHTYPE_EAPOL 0x888EU /* EAPOL */

PACK_STRUCT_BEGIN
/* This is an Token-Ring LLC structure */
struct eth_llc_hdr
{
    PACK_STRUCT_FIELD(u8_t dsap);      /* destination SAP */
    PACK_STRUCT_FIELD(u8_t ssap);      /* source SAP */
    PACK_STRUCT_FIELD(u8_t llc);       /* LLC control field */
    PACK_STRUCT_FIELD(u8_t protid[3]); /* protocol id */
    PACK_STRUCT_FIELD(u16_t type);     /* ether type field */
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

#elif __ZEPHYR__

/* This is an Token-Ring LLC structure */
struct eth_llc_hdr
{
    t_u8 dsap;      /* destination SAP */
    t_u8 ssap;      /* source SAP */
    t_u8 llc;       /* LLC control field */
    t_u8 protid[3]; /* protocol id */
    t_u16 type;     /* ether type field */
} __packed;

#endif

#define SIZEOF_ETH_LLC_HDR (8U)
#ifdef __ZEPHYR__
#define SIZEOF_ETH_HDR (14U)
#endif

/* Define those to better describe your network interface. */
#define IFNAME0 'm'
#define IFNAME1 'l'

/*
 * It was observed that Wi-Fi card does not wakeup
 * immediately after call to wlan_wakeup_card.
 * The code tries to wakeup the card by writing
 * in SDIO register.
 * It waits for 20 ms for AWAKE event from Wi-Fi card,
 * if AWAKE event is not generated or received  by MCU code
 * it returns error and does not send a packet out.
 * This is observed with  8801 Wi-Fi card.
 * So for 8801 based platforms the wait time is now 35 ms.
 */
#define MAX_WAIT_TIME            35
#define MAX_INTERFACES_SUPPORTED 3U

/* The time to block waiting for input. */
#define emacBLOCK_TIME_WAITING_FOR_INPUT ((portTickType)100)

#if FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER
#define NETIF_TX_BUFFERS 32
#define NETIF_RX_BUFFERS 32

/**
 * npx wifi driver structure.
 */
struct nxp_wifi_device
{
    /** Set to 1 when owner is software (ready to read), 0 for Wi-Fi. */
    uint32_t rx_desc[NETIF_RX_BUFFERS];
    /** Set to 1 when owner is Wi-Fi, 0 for software. */
    uint32_t tx_desc[NETIF_TX_BUFFERS];
    /** RX pbuf pointer list */
    struct pbuf *rx_pbuf[NETIF_RX_BUFFERS];
    /** TX pbuf pointer list */
    struct pbuf *tx_pbuf[NETIF_TX_BUFFERS];

    /** Circular buffer head pointer for packet received. */
    uint32_t us_rx_head;
    /** Circular buffer tail pointer for packet to be read. */
    uint32_t us_rx_tail;
    /** Circular buffer head pointer by upper layer (buffer to be sent). */
    uint32_t us_tx_head;
    /** Circular buffer tail pointer incremented by handlers (buffer sent). */
    uint32_t us_tx_tail;
    /** RX task notification semaphore. */
    sys_sem_t sync_sem;
};
#endif
/*------------------------------------------------------*/
extern int wlan_get_mac_address(uint8_t *dest);
extern void wlan_wake_up_card(void);


#if CONFIG_WPA_SUPP
// void (*l2_packet_rx_callback)(const struct pbuf *p);
#endif /* CONFIG_HOST_SUPP */

#if UAP_SUPPORT
void wrapper_wlan_update_uap_rxrate_info(RxPD *rxpd);
#endif

int wrapper_wlan_handle_rx_packet(t_u16 datalen, RxPD *rxpd, void *p, void *payload);

#if CONFIG_NET_MONITOR
void user_recv_monitor_data(const t_u8 *rcvdata);
#endif

#if defined(SDK_OS_FREE_RTOS)

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif
{
    struct eth_addr *ethaddr;
    /* Interface to bss type identification that tells the FW wherether
       the data is for STA for UAP */
    t_u8 interface;
    /* Add whatever per-interface state that is needed here. */
};

#endif
