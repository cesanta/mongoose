/*
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

// Modified to work with Mongoose built-in TCP/IP by Cesanta Software Ltd.

#include <mlan_api.h>
#include <wm_net.h>
#include <wmlog.h>
#ifdef RW610
#include <wifi-imu.h>
#else
#include <wifi-sdio.h>
#endif
#include <wifi-internal.h>

#define NETIF_NAMESIZE 4

struct netif {
  uint8_t mac[6];
};

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
#define MAX_WAIT_TIME 35

extern int wlan_get_mac_address(uint8_t *dest);
extern void wlan_wake_up_card(void);

#if CONFIG_WPA_SUPP
// void (*l2_packet_rx_callback)(const struct pbuf *p);
#endif /* CONFIG_HOST_SUPP */

void wrapper_wlan_update_uap_rxrate_info(RxPD *rxpd);

int wrapper_wlan_handle_rx_packet(t_u16 datalen, RxPD *rxpd, void *p,
                                  void *payload);

int wrapper_wlan_handle_amsdu_rx_packet(const t_u8 *rcvdata,
                                        const t_u16 datalen);

#if CONFIG_NET_MONITOR
void user_recv_monitor_data(const t_u8 *rcvdata);
#endif
