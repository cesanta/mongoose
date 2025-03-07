/*
 * Copyright 2020-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wpl.h"
#ifndef WPL_NO_WLAN_INIT
#include "wlan_bt_fw.h"
#endif
#include "wlan.h"
#include "wifi.h"
#include "wm_net.h"
#include "dhcp-server.h"
#include <stdio.h>
#include "event_groups.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_JSON_NETWORK_RECORD_LENGTH 185U

#define WPL_SYNC_TIMEOUT_MS portMAX_DELAY

#define UAP_NETWORK_NAME "uap-network"

#define EVENT_BIT(event) ((uint32_t)1U << (event))

#define WPL_SYNC_INIT_GROUP (EVENT_BIT(WPL_EVENT_INITIALIZED) | EVENT_BIT(WPL_EVENT_INITIALIZATION_FAILED))

#define WPL_SYNC_CONNECT_GROUP                                                                  \
    (EVENT_BIT(WPL_EVENT_SUCCESS) | EVENT_BIT(WPL_EVENT_CONNECT_FAILED) |                    \
        EVENT_BIT(WPL_EVENT_NETWORK_NOT_FOUND) | EVENT_BIT(WPL_EVENT_NETWORK_AUTH_FAILED) | \
        EVENT_BIT(WPL_EVENT_ADDRESS_FAILED))

#define WPL_SYNC_DISCONNECT_GROUP EVENT_BIT(WPL_EVENT_USER_DISCONNECT)

#define WPL_SYNC_UAP_START_GROUP (EVENT_BIT(WPL_EVENT_UAP_SUCCESS) | EVENT_BIT(WPL_EVENT_UAP_START_FAILED))

#define WPL_SYNC_UAP_STOP_GROUP (EVENT_BIT(WPL_EVENT_UAP_STOPPED) | EVENT_BIT(WPL_EVENT_UAP_STOP_FAILED))

#define WPL_SYNC_SCAN_GROUP EVENT_BIT(WPL_EVENT_SCAN_DONE)

#define WPL_EVENT_UNUSED                0
#define WPL_EVENT_INITIALIZED           1
#define WPL_EVENT_INITIALIZATION_FAILED 2
#define WPL_EVENT_SUCCESS               3
#define WPL_EVENT_CONNECT_FAILED        4
#define WPL_EVENT_NETWORK_NOT_FOUND     5
#define WPL_EVENT_NETWORK_AUTH_FAILED   6
#define WPL_EVENT_ADDRESS_FAILED        7
#define WPL_EVENT_USER_DISCONNECT       8
#define WPL_EVENT_UAP_SUCCESS           9
#define WPL_EVENT_UAP_START_FAILED      10
#define WPL_EVENT_UAP_STOPPED           11
#define WPL_EVENT_UAP_STOP_FAILED       12
#define WPL_EVENT_SCAN_DONE             13
#define __WPL_EVENT_COUNT               14
#define __WPL_EVENT_MAX                 24

#if __WPL_EVENT_COUNT >= __WPL_EVENT_MAX
#error "__WPL_EVENT_COUNT >= __WPL_EVENT_MAX"
#endif

typedef enum _wpl_state
{
    WPL_NOT_INITIALIZED,
    WPL_INITIALIZED,
    WPL_STARTED,
} wpl_state_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static wpl_state_t s_wplState            = WPL_NOT_INITIALIZED;
static bool s_wplStaConnected            = false;
static bool s_wplUapActivated            = false;
static EventGroupHandle_t s_wplSyncEvent = NULL;
static linkLostCb_t s_linkLostCb         = NULL;
static char *ssids_json                  = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int wlan_event_callback(enum wlan_event_reason reason, void *data);
static int WLP_process_results(unsigned int count);
static int WPL_map_event(enum wlan_event_reason reason);

/*******************************************************************************
 * Code
 ******************************************************************************/

static int WPL_map_event(enum wlan_event_reason reason)
{
    switch (reason)
    {
        case WLAN_REASON_INITIALIZED:
            return WPL_EVENT_INITIALIZED;
        case WLAN_REASON_INITIALIZATION_FAILED:
            return WPL_EVENT_INITIALIZATION_FAILED;
        case WLAN_REASON_SUCCESS:
            return WPL_EVENT_SUCCESS;
        case WLAN_REASON_CONNECT_FAILED:
            return WPL_EVENT_CONNECT_FAILED;
        case WLAN_REASON_NETWORK_NOT_FOUND:
            return WPL_EVENT_NETWORK_NOT_FOUND;
        case WLAN_REASON_NETWORK_AUTH_FAILED:
            return WPL_EVENT_NETWORK_AUTH_FAILED;
        case WLAN_REASON_ADDRESS_FAILED:
            return WPL_EVENT_ADDRESS_FAILED;
        case WLAN_REASON_USER_DISCONNECT:
            return WPL_EVENT_USER_DISCONNECT;
        case WLAN_REASON_UAP_SUCCESS:
            return WPL_EVENT_UAP_SUCCESS;
        case WLAN_REASON_UAP_START_FAILED:
            return WPL_EVENT_UAP_START_FAILED;
        case WLAN_REASON_UAP_STOPPED:
            return WPL_EVENT_UAP_STOPPED;
        case WLAN_REASON_UAP_STOP_FAILED:
            return WPL_EVENT_UAP_STOP_FAILED;
        default:
            return WPL_EVENT_UNUSED;
    }
}

/* Callback Function passed to WLAN Connection Manager. The callback function
 * gets called when there are WLAN Events that need to be handled by the
 * application.
 */
int wlan_event_callback(enum wlan_event_reason reason, void *data)
{
#ifdef WPL_DEBUG
    PRINTF("-------- wlan_event_callback %d --------\r\n", reason);
#endif
    if (s_wplState >= WPL_INITIALIZED)
    {
        (void)xEventGroupSetBits(s_wplSyncEvent, EVENT_BIT((uint32_t)WPL_map_event(reason)));
    }

    switch (reason)
    {
        case WLAN_REASON_SUCCESS:
            if (s_wplStaConnected)
            {
                s_linkLostCb(true);
            }
            break;

        case WLAN_REASON_AUTH_SUCCESS:
            break;

        case WLAN_REASON_CONNECT_FAILED:
        case WLAN_REASON_NETWORK_NOT_FOUND:
        case WLAN_REASON_NETWORK_AUTH_FAILED:
            if (s_wplStaConnected)
            {
                s_linkLostCb(false);
            }
            break;

        case WLAN_REASON_ADDRESS_SUCCESS:
            break;
        case WLAN_REASON_ADDRESS_FAILED:
            break;
        case WLAN_REASON_LINK_LOST:
            if (s_wplStaConnected)
            {
                s_linkLostCb(false);
            }
            break;

        case WLAN_REASON_CHAN_SWITCH:
            break;
        case WLAN_REASON_WPS_DISCONNECT:
            break;
        case WLAN_REASON_USER_DISCONNECT:
            break;
        case WLAN_REASON_INITIALIZED:
            break;
        case WLAN_REASON_INITIALIZATION_FAILED:
            break;
        case WLAN_REASON_PS_ENTER:
            break;
        case WLAN_REASON_PS_EXIT:
            break;
        case WLAN_REASON_UAP_SUCCESS:
            break;

        case WLAN_REASON_UAP_CLIENT_ASSOC:
#ifdef WPL_DEBUG
            PRINTF("Client => ");
            print_mac((const char *)data);
            PRINTF("Associated with Soft AP\r\n");
#endif
            break;
        case WLAN_REASON_UAP_CLIENT_DISSOC:
#ifdef WPL_DEBUG
            PRINTF("Client => ");
            print_mac((const char *)data);
            PRINTF("Dis-Associated from Soft AP\r\n");
#endif
            break;

        case WLAN_REASON_UAP_START_FAILED:
            break;
        case WLAN_REASON_UAP_STOP_FAILED:
            break;
        case WLAN_REASON_UAP_STOPPED:
            break;
        default:
#ifdef WPL_DEBUG
            PRINTF("Unknown Wifi CB Reason %d\r\n", reason);
#endif
            break;
    }

    return WM_SUCCESS;
}

wpl_ret_t WPL_Init(void)
{
    wpl_ret_t status = WPLRET_SUCCESS;

    if (s_wplState != WPL_NOT_INITIALIZED)
    {
        status = WPLRET_FAIL;
    }

    if (status == WPLRET_SUCCESS)
    {
        if (s_wplSyncEvent == NULL)
        {
            s_wplSyncEvent = xEventGroupCreate();
        }

        if (s_wplSyncEvent == NULL)
        {
            status = WPLRET_FAIL;
        }
    }

#ifndef WPL_NO_WLAN_INIT
    if (status == WPLRET_SUCCESS)
    {
        const int ret = wlan_init(wlan_fw_bin, wlan_fw_bin_len);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }
#endif

    if (status == WPLRET_SUCCESS)
    {
        s_wplState = WPL_INITIALIZED;
    }

    return status;
}

wpl_ret_t WPL_Start(linkLostCb_t callbackFunction)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    EventBits_t syncBit;

    if (s_wplState != WPL_INITIALIZED)
    {
        status = WPLRET_NOT_READY;
    }

    if (status == WPLRET_SUCCESS)
    {
        (void)xEventGroupClearBits(s_wplSyncEvent, WPL_SYNC_INIT_GROUP);

        ret = wlan_start(&wlan_event_callback);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        syncBit = xEventGroupWaitBits(s_wplSyncEvent, WPL_SYNC_INIT_GROUP, pdTRUE, pdFALSE, WPL_SYNC_TIMEOUT_MS);
        if ((syncBit & EVENT_BIT(WPL_EVENT_INITIALIZED)) != 0U)
        {
            s_linkLostCb = callbackFunction;
            status       = WPLRET_SUCCESS;
        }
        else if ((syncBit & EVENT_BIT(WPL_EVENT_INITIALIZATION_FAILED)) != 0U)
        {
            status = WPLRET_FAIL;
        }
        else
        {
            status = WPLRET_TIMEOUT;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        s_wplState = WPL_STARTED;
    }

    return status;
}

wpl_ret_t WPL_Stop(void)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;

    if (s_wplState != WPL_STARTED)
    {
        status = WPLRET_NOT_READY;
    }

    if (status == WPLRET_SUCCESS)
    {
        ret = wlan_stop();
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        s_wplState = WPL_INITIALIZED;
    }

    return status;
}

wpl_ret_t WPL_Start_AP(const char *ssid, const char *password, int chan)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    enum wlan_security_type security = WLAN_SECURITY_NONE;
    EventBits_t syncBit;
    struct wlan_network uap_network;
    size_t ssid_len = strlen(ssid);
    size_t password_len = strlen(password);

    if ((s_wplState != WPL_STARTED) || (s_wplUapActivated != false))
    {
        status = WPLRET_NOT_READY;
    }

    if ((ssid_len == 0U) || (ssid_len > IEEEtypes_SSID_SIZE))
    {
        status = WPLRET_BAD_PARAM;
    }

    if (((0U < password_len) && (password_len < WPL_WIFI_PASSWORD_MIN_LEN)) ||
        (password_len > WPL_WIFI_PASSWORD_LENGTH))
    {
        status = WPLRET_BAD_PARAM;
    }

    if (status == WPLRET_SUCCESS)
    {
        if (password_len == 0U)
        {
            security = WLAN_SECURITY_NONE;
        }
        else
        {
            security = WLAN_SECURITY_WPA2;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        wlan_initialize_uap_network(&uap_network);

        (void)memcpy(uap_network.ssid, ssid, ssid_len);
        uap_network.ip.ipv4.address  = ipaddr_addr(WPL_WIFI_AP_IP_ADDR);
        uap_network.ip.ipv4.gw       = ipaddr_addr(WPL_WIFI_AP_IP_ADDR);
        uap_network.channel          = (unsigned int)chan;
        uap_network.security.type    = security;
        uap_network.security.psk_len = (uint8_t)password_len;
        (void)strncpy(uap_network.security.psk, password, password_len);
    }

    if (status == WPLRET_SUCCESS)
    {
        ret = wlan_add_network(&uap_network);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        (void)xEventGroupClearBits(s_wplSyncEvent, WPL_SYNC_UAP_START_GROUP);

        ret = wlan_start_network(uap_network.name);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
        else
        {
            syncBit =
                xEventGroupWaitBits(s_wplSyncEvent, WPL_SYNC_UAP_START_GROUP, pdTRUE, pdFALSE, WPL_SYNC_TIMEOUT_MS);
            if ((syncBit & EVENT_BIT(WPL_EVENT_UAP_SUCCESS)) != 0U)
            {
                status = WPLRET_SUCCESS;
            }
            else if ((syncBit & EVENT_BIT(WPL_EVENT_UAP_START_FAILED)) != 0U)
            {
                status = WPLRET_FAIL;
            }
            else
            {
                status = WPLRET_TIMEOUT;
            }
        }

        if (status != WPLRET_SUCCESS)
        {
            wlan_remove_network(uap_network.name);
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        ret = dhcp_server_start(net_get_uap_handle());
        if (ret != WM_SUCCESS)
        {
            wlan_stop_network(uap_network.name);
            wlan_remove_network(uap_network.name);
            status = WPLRET_FAIL;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        s_wplUapActivated = true;
    }

    return status;
}

wpl_ret_t WPL_Stop_AP(void)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    EventBits_t syncBit;

    if ((s_wplState != WPL_STARTED) || (s_wplUapActivated != true))
    {
        status = WPLRET_NOT_READY;
    }

    if (status == WPLRET_SUCCESS)
    {
        dhcp_server_stop();

        (void)xEventGroupClearBits(s_wplSyncEvent, WPL_SYNC_UAP_START_GROUP);

        ret = wlan_stop_network(UAP_NETWORK_NAME);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
        else
        {
            syncBit =
                xEventGroupWaitBits(s_wplSyncEvent, WPL_SYNC_UAP_STOP_GROUP, pdTRUE, pdFALSE, WPL_SYNC_TIMEOUT_MS);
            if ((syncBit & EVENT_BIT(WPL_EVENT_UAP_STOPPED)) != 0U)
            {
                status = WPLRET_SUCCESS;
            }
            else if ((syncBit & EVENT_BIT(WPL_EVENT_UAP_STOP_FAILED)) != 0U)
            {
                status = WPLRET_FAIL;
            }
            else
            {
                status = WPLRET_TIMEOUT;
            }
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        ret = wlan_remove_network(UAP_NETWORK_NAME);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        s_wplUapActivated = false;
    }

    return status;
}

static int WLP_process_results(unsigned int count)
{
    int ret                             = 0;
    struct wlan_scan_result scan_result = {0};
    uint32_t ssids_json_len             = count * MAX_JSON_NETWORK_RECORD_LENGTH;

    /* Add length of "{"networks":[]}" */
    ssids_json_len += 15U;

    ssids_json = pvPortMalloc(ssids_json_len);
    if (ssids_json == NULL)
    {
        PRINTF("[!] Memory allocation failed\r\n");
        (void)xEventGroupSetBits(s_wplSyncEvent, EVENT_BIT(WPL_EVENT_SCAN_DONE));
        return WM_FAIL;
    }

    /* Start building JSON */
    (void)strcpy(ssids_json, "{\"networks\":[");
    uint32_t ssids_json_idx = strlen(ssids_json);

    for (uint32_t i = 0; i < count; i++)
    {
        ret = wlan_get_scan_result(i, &scan_result);
        if (ret == WM_SUCCESS)
        {
            PRINTF("%s\r\n", scan_result.ssid);
            PRINTF("     BSSID         : %02X:%02X:%02X:%02X:%02X:%02X\r\n", (unsigned int)scan_result.bssid[0],
                   (unsigned int)scan_result.bssid[1], (unsigned int)scan_result.bssid[2],
                   (unsigned int)scan_result.bssid[3], (unsigned int)scan_result.bssid[4],
                   (unsigned int)scan_result.bssid[5]);
            PRINTF("     RSSI          : %ddBm\r\n", -(int)scan_result.rssi);
            PRINTF("     Channel       : %d\r\n", (int)scan_result.channel);

            char security[40];
            security[0] = '\0';

            if (scan_result.wpa2_entp == 1U)
            {
                (void)strcat(security, "WPA2_ENTP ");
            }
            if (scan_result.wep == 1U)
            {
                (void)strcat(security, "WEP ");
            }
            if (scan_result.wpa == 1U)
            {
                (void)strcat(security, "WPA ");
            }
            if (scan_result.wpa2 == 1U)
            {
                (void)strcat(security, "WPA2 ");
            }
            if (scan_result.wpa3_sae == 1U)
            {
                (void)strcat(security, "WPA3_SAE ");
            }

            if (i != 0U)
            {
                /* Add ',' separator before next entry */
                ssids_json[ssids_json_idx++] = ',';
            }

            ret = snprintf(
                ssids_json + ssids_json_idx, ssids_json_len - ssids_json_idx - 1U,
                "{\"ssid\":\"%s\",\"bssid\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\"signal\":\"%ddBm\",\"channel\":%d,"
                "\"security\":\"%s\"}",
                scan_result.ssid, (unsigned int)scan_result.bssid[0], (unsigned int)scan_result.bssid[1],
                (unsigned int)scan_result.bssid[2], (unsigned int)scan_result.bssid[3], (unsigned int)scan_result.bssid[4],
                (unsigned int)scan_result.bssid[5], -(int)scan_result.rssi, (int)scan_result.channel, security);
            if (ret > 0)
            {
                ssids_json_idx += (uint32_t)ret;
            }
            else
            {
                PRINTF("[!] JSON creation failed\r\n");
                vPortFree(ssids_json);
                ssids_json = NULL;
                (void)xEventGroupSetBits(s_wplSyncEvent, EVENT_BIT(WPL_EVENT_SCAN_DONE));
                return WM_FAIL;
            }
        }
    }

    /* End of JSON "]}" */
    (void)strcpy(ssids_json + ssids_json_idx, "]}");

    (void)xEventGroupSetBits(s_wplSyncEvent, EVENT_BIT(WPL_EVENT_SCAN_DONE));
    return WM_SUCCESS;
}

char *WPL_Scan(void)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    EventBits_t syncBit;

    if (s_wplState != WPL_STARTED)
    {
        status = WPLRET_NOT_READY;
    }

    if (status == WPLRET_SUCCESS)
    {
        ret = wlan_scan(&WLP_process_results);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        syncBit = xEventGroupWaitBits(s_wplSyncEvent, WPL_SYNC_SCAN_GROUP, pdTRUE, pdFALSE, WPL_SYNC_TIMEOUT_MS);
        if ((syncBit & EVENT_BIT(WPL_EVENT_SCAN_DONE)) != 0U)
        {
            status = WPLRET_SUCCESS;
        }
        else
        {
            status = WPLRET_TIMEOUT;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        return ssids_json;
    }

    return NULL;
}

wpl_ret_t WPL_AddNetworkWithSecurity(const char *ssid, const char *password, const char *label, wpl_security_t security)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    struct wlan_network sta_network;
    memset(&sta_network, 0, sizeof(struct wlan_network));

    size_t ssid_len = strlen(ssid);
    size_t password_len = strlen(password);
    size_t label_len = strlen(label);

    if (s_wplState != WPL_STARTED)
    {
        status = WPLRET_NOT_READY;
    }

    if ((label_len == 0U) || (label_len > WLAN_NETWORK_NAME_MAX_LENGTH))
    {
        status = WPLRET_BAD_PARAM;
    }

    if ((ssid_len == 0U) || (ssid_len > IEEEtypes_SSID_SIZE))
    {
        status = WPLRET_BAD_PARAM;
    }

    if (((0U < password_len) && (password_len < WPL_WIFI_PASSWORD_MIN_LEN)) ||
        (password_len > WPL_WIFI_PASSWORD_LENGTH))
    {
        status = WPLRET_BAD_PARAM;
    }

    if (status == WPLRET_SUCCESS)
    {
        if (password_len == 0U)
        {
            sta_network.security.type = WLAN_SECURITY_NONE;
        }
        else
        {
            switch (security)
            {
                case WPL_SECURITY_WILDCARD:
                    sta_network.security.type = WLAN_SECURITY_WILDCARD;
                    sta_network.security.mfpc = true;
                    sta_network.security.mfpr = true;
                    sta_network.security.password_len = password_len;
                    strncpy(sta_network.security.password, password, password_len);
                    sta_network.security.psk_len = (uint8_t)password_len;
                    strncpy(sta_network.security.psk, password, password_len);
                    break;
                case WPL_SECURITY_WPA3_SAE:
                    sta_network.security.type = WLAN_SECURITY_WPA3_SAE;
                    sta_network.security.mfpc = true;
                    sta_network.security.mfpr = true;
                    sta_network.security.password_len = password_len;
                    strncpy(sta_network.security.password, password, password_len);
                    break;
                default:
                    PRINTF("[!] Unimplemented security type (%d)\r\n", security);
                    status = WPLRET_BAD_PARAM;
                    break;
            }
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        strncpy(sta_network.name, label, label_len);
        strncpy(sta_network.ssid, ssid, ssid_len);
        sta_network.ip.ipv4.addr_type = ADDR_TYPE_DHCP;
        sta_network.ssid_specific     = 1;

        ret = wlan_add_network(&sta_network);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    return status;
}

wpl_ret_t WPL_AddNetwork(const char *ssid, const char *password, const char *label)
{
    return WPL_AddNetworkWithSecurity(ssid, password, label, WPL_SECURITY_WILDCARD);
}

wpl_ret_t WPL_RemoveNetwork(const char *label)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    size_t label_len = strlen(label);

    if (s_wplState != WPL_STARTED)
    {
        status = WPLRET_NOT_READY;
    }

    if ((label_len == 0U) || (label_len > WLAN_NETWORK_NAME_MAX_LENGTH))
    {
        status = WPLRET_BAD_PARAM;
    }

    if (status == WPLRET_SUCCESS)
    {
        ret = wlan_remove_network(label);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    return status;
}

wpl_ret_t WPL_Join(char *label)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    EventBits_t syncBit;
    size_t label_len = strlen(label);

    if ((s_wplState != WPL_STARTED) || (s_wplStaConnected != false))
    {
        status = WPLRET_NOT_READY;
    }

    if ((label_len == 0U) || (label_len > WLAN_NETWORK_NAME_MAX_LENGTH))
    {
        status = WPLRET_BAD_PARAM;
    }

    if (status == WPLRET_SUCCESS)
    {
        (void)xEventGroupClearBits(s_wplSyncEvent, WPL_SYNC_CONNECT_GROUP);

        ret = wlan_connect(label);
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        syncBit = xEventGroupWaitBits(s_wplSyncEvent, WPL_SYNC_CONNECT_GROUP, pdTRUE, pdFALSE, WPL_SYNC_TIMEOUT_MS);
        if ((syncBit & EVENT_BIT(WLAN_REASON_SUCCESS)) != 0U)
        {
            status = WPLRET_SUCCESS;
        }
        else if ((syncBit & EVENT_BIT(WPL_EVENT_CONNECT_FAILED)) != 0U)
        {
            status = WPLRET_FAIL;
        }
        else if ((syncBit & EVENT_BIT(WPL_EVENT_NETWORK_NOT_FOUND)) != 0U)
        {
            status = WPLRET_NOT_FOUND;
        }
        else if ((syncBit & EVENT_BIT(WPL_EVENT_NETWORK_AUTH_FAILED)) != 0U)
        {
            status = WPLRET_AUTH_FAILED;
        }
        else if ((syncBit & EVENT_BIT(WPL_EVENT_ADDRESS_FAILED)) != 0U)
        {
            status = WPLRET_ADDR_FAILED;
        }
        else
        {
            status = WPLRET_TIMEOUT;
        }

        if (status != WPLRET_SUCCESS)
        {
            /* Abort the next connection attempt */
            (void)WPL_Leave();
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        s_wplStaConnected = true;
    }

    return status;
}

wpl_ret_t WPL_Leave(void)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    EventBits_t syncBit;

    if (s_wplState != WPL_STARTED)
    {
        status = WPLRET_NOT_READY;
    }

    enum wlan_connection_state connection_state = WLAN_DISCONNECTED;
    ret = wlan_get_connection_state(&connection_state);
    if (ret != WM_SUCCESS)
    {
        status = WPLRET_FAIL;
    }
    else
    {
        if (connection_state == WLAN_DISCONNECTED)
        {
            s_wplStaConnected = false;
            return WPLRET_SUCCESS;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        (void)xEventGroupClearBits(s_wplSyncEvent, WPL_SYNC_DISCONNECT_GROUP);

        ret = wlan_disconnect();
        if (ret != WM_SUCCESS)
        {
            status = WPLRET_FAIL;
        }
    }

    if (status == WPLRET_SUCCESS)
    {
        syncBit = xEventGroupWaitBits(s_wplSyncEvent, WPL_SYNC_DISCONNECT_GROUP, pdTRUE, pdFALSE, WPL_SYNC_TIMEOUT_MS);
        if ((syncBit & EVENT_BIT(WPL_EVENT_USER_DISCONNECT)) != 0U)
        {
            status = WPLRET_SUCCESS;
        }
        else
        {
            status = WPLRET_TIMEOUT;
        }
    }

    s_wplStaConnected = false;

    return status;
}

wpl_ret_t WPL_GetIP(char *ip, int client)
{
    wpl_ret_t status = WPLRET_SUCCESS;
    int ret;
    struct wlan_ip_config addr;

    if (ip == NULL)
    {
        status = WPLRET_FAIL;
    }

    if (status == WPLRET_SUCCESS)
    {
        if (client == 1)
        {
            ret = wlan_get_address(&addr);
        }
        else
        {
            ret = wlan_get_uap_address(&addr);
        }

        if (ret == WM_SUCCESS)
        {
            net_inet_ntoa(addr.ipv4.address, ip);
        }
        else
        {
            status = WPLRET_FAIL;
        }
    }

    return status;
}
