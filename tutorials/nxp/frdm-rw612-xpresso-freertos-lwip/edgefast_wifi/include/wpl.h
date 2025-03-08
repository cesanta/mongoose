/*
 * Copyright 2020-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _WPL_H_
#define _WPL_H_

#include "stdbool.h"

#define WPL_WIFI_SSID_LENGTH      32U
#define WPL_WIFI_PASSWORD_MIN_LEN 8U
#define WPL_WIFI_PASSWORD_LENGTH  63U

/* IP Address of Wi-Fi interface in AP (Access Point) mode */
#ifndef WPL_WIFI_AP_IP_ADDR
#define WPL_WIFI_AP_IP_ADDR "192.168.1.1"
#endif /* WPL_WIFI_AP_IP_ADDR */

typedef void (*linkLostCb_t)(bool linkState);

typedef enum _wpl_ret
{
    WPLRET_SUCCESS,
    WPLRET_FAIL,
    WPLRET_NOT_FOUND,
    WPLRET_AUTH_FAILED,
    WPLRET_ADDR_FAILED,
    WPLRET_NOT_CONNECTED,
    WPLRET_NOT_READY,
    WPLRET_TIMEOUT,
    WPLRET_BAD_PARAM,
} wpl_ret_t;

typedef enum _wpl_security
{
    /* Used when the user only knows SSID and password. This option should be used
     * for WPA2 security and lower. */
    WPL_SECURITY_WILDCARD,
    /* Use WPA3 SAE security */
    WPL_SECURITY_WPA3_SAE,
} wpl_security_t;

/**
 * @brief  Initialize Wi-Fi driver and WPL layer.
 *         Create an internal Event structure used for WPL layer (blocking API).
 *         Set WPL layer state to WPL_INITIALIZED.
 *         Should be the first function called from the WPL layer API.
 *
 * @return WPLRET_SUCCESS Wi-Fi driver and WPL layer initialized successfully.
 */
wpl_ret_t WPL_Init(void);

/**
 * @brief  Start Wi-Fi driver and register an application link state callback.
 *         Set WPL layer state to WPL_STARTED.
 *         WPL_Start should be called only after WPL_Init was successfully performed.
 *
 * @param  callbackFunction Function which will be called from WPL layer in order to
 *                          notify upper application that Wi-Fi link is lost or re/established.
 *
 * @return WPLRET_SUCCESS Wi-Fi driver started successfully.
 */
wpl_ret_t WPL_Start(linkLostCb_t callbackFunction);

/**
 * @brief  Stop Wi-Fi driver.
 *         Set WPL layer state to WPL_NOT_INITIALIZED.
 *         WPL_Stop should be called only after WPL_Start was successfully performed.
 *
 * @return WPLRET_SUCCESS Wi-Fi driver is stopped.
 */
wpl_ret_t WPL_Stop(void);

/**
 * @brief  Create an AP (Access Point) network profile and start Wi-Fi AP interface based on this profile.
 *         If AP mode is started successfully, start a DHCP server.
 *         If everything goes well, other devices can connect to this network.
 *         If anything goes wrong, provided network profile is deleted, AP and DHCP are stopped.
 *         WPL_Start_AP fails if AP interface is already up.
 *         WPL_Start_AP should be called only after WPL_Start was successfully performed.
 *
 * @param  ssid Name of the AP network to be created.
 * @param  password Password of the AP network to be created.
 * @param  chan Channel of the AP network to be created.
 *
 * @return WPLRET_SUCCESS Network profile created, Wi-Fi AP interface up, DHCP server running.
 */
wpl_ret_t WPL_Start_AP(const char *ssid, const char *password, int chan);

/**
 * @brief  Stop DHCP server, stop Wi-Fi AP (Access Point) interface and delete AP network profile.
 *         WPL_Stop_AP should be called only after WPL_Start_AP was successfully performed.
 *
 * @return WPLRET_SUCCESS DHCP server stopped, AP interface down, AP network profile deleted.
 */
wpl_ret_t WPL_Stop_AP(void);

/**
 * @brief  Scan for nearby Wi-Fi networks.
           Print available networks information and store them in an internal buffer in JSON fomrat.
           The returned buffer is dynamically allocated, caller is responsible for deallocation.
 *         WPL_Scan should be called only after WPL_Start was successfully performed.
 *
 * @return Pointer to buffer with scan results.
 */
char *WPL_Scan(void);

/**
 * @brief  Create and save a new STA (Station) network profile.
 *         This STA network profile can be used in future (WPL_RemoveNetwork / WPL_Join) calls based on its label.
 *         WPL_AddNetwork should be called only after WPL_Start was successfully performed.
 *
 * @param  ssid Name of the STA network to be created.
 * @param  password Password of the STA network to be created.
 * @param  label Alias for the network to be added. A network may be referred by its label.
 * @param  security Prefered security type. Refer to wpl_security_t for list of options.
 *
 * @return WPLRET_SUCCESS New STA network profile was successfully saved.
 */
wpl_ret_t WPL_AddNetworkWithSecurity(const char *ssid, const char *password, const char *label, wpl_security_t security);

/**
 * @brief  Create and save a new STA (Station) network profile.
 *         This STA network profile can be used in future (WPL_RemoveNetwork / WPL_Join) calls based on its label.
 *         WPL_AddNetwork should be called only after WPL_Start was successfully performed.
 *
 * @param  ssid Name of the STA network to be created.
 * @param  password Password of the STA network to be created.
 * @param  label Alias for the network to be added. A network may be referred by its label.
 *
 * @return WPLRET_SUCCESS New STA network profile was successfully saved.
 */
wpl_ret_t WPL_AddNetwork(const char *ssid, const char *password, const char *label);

/**
 * @brief  Delete a previously added STA (Station) network profile.
 *         The profile to be deleted is referred by its label and should have been previously added using
 * WPL_AddNetwork. WPL_RemoveNetwork should be called only after WPL_AddNetwork was successfully performed (for this
 * network).
 *
 * @param  label Alias for the network to be deleted. Label was set by WPL_AddNetwork.
 *
 * @return WPLRET_SUCCESS The profile network is deleted.
 */
wpl_ret_t WPL_RemoveNetwork(const char *label);

/**
 * @brief  Connect to a Wi-Fi network.
 *         Wi-Fi network is chosen by a STA network label.
 *         The Wi-Fi network to connect to is referred by its label and should have been previously added using
 * WPL_AddNetwork. WPL_Join should be called only after WPL_AddNetwork was successfully performed (for this network).
 *
 * @param  label Alias for the network to connect to. Label was set by WPL_AddNetwork.
 *
 * @return WPLRET_SUCCESS Device joined the Wi-Fi network using its STA interface.
 */
wpl_ret_t WPL_Join(char *label);

/**
 * @brief  Disconnect from currently connected Wi-Fi network.
 *         WPL_Leave should be called only after WPL_Join was successfully performed.
 *
 * @return WPLRET_SUCCESS Device left the Wi-Fi network it was connected to.
 */
wpl_ret_t WPL_Leave(void);

/**
 * @brief  Get IP for AP interface or STA interface.
 *
 * @param  ip Pointer where IP should be stored.
 * @param  client 0 for AP, 1 for STA.
 *
 * @return WPLRET_SUCCESS if the IP was successfully retrieved.
 */
wpl_ret_t WPL_GetIP(char *ip, int client);

#endif /* _WPL_H_ */
