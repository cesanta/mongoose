/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/tcpip.h"
#include "board.h"
#include "app.h"
#include "wpl.h"
#include "timers.h"
#include "fsl_debug_console.h"
#include <stdio.h>
#include "FreeRTOS.h"

#include "mongoose.h"
#include "net.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef AP_SSID
#define AP_SSID "YOUR_SSID"
#endif

#ifndef AP_PASSWORD
#define AP_PASSWORD "YOUR_PASSWORD"
#endif

#define WIFI_NETWORK_LABEL "my_wifi"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Link lost callback */
static void LinkStatusChangeCallback(bool linkState)
{
    if (linkState == false)
    {
        PRINTF("-------- LINK LOST --------\r\n");
    }
    else
    {
        PRINTF("-------- LINK REESTABLISHED --------\r\n");
    }
}

/* Connect to the external AP */
static void ConnectTo()
{
    int32_t result;

    /* Add Wi-Fi network */
    result = WPL_AddNetwork(AP_SSID, AP_PASSWORD, WIFI_NETWORK_LABEL);
    if (result == WPLRET_SUCCESS)
    {
        PRINTF("Connecting as client to ssid: %s with password %s\r\n", AP_SSID, AP_PASSWORD);
        result = WPL_Join(WIFI_NETWORK_LABEL);
    }

    if (result != WPLRET_SUCCESS)
    {
        PRINTF("[!] Cannot connect to Wi-Fi\r\n[!]ssid: %s\r\n[!]passphrase: %s\r\n", AP_SSID, AP_PASSWORD);

        while (1)
        {
            __BKPT(0);
        }
    }
    else
    {
        PRINTF("[i] Connected to Wi-Fi\r\nssid: %s\r\n[!]passphrase: %s\r\n", AP_SSID, AP_PASSWORD);
        char ip[16];
        WPL_GetIP(ip, 1);
        PRINTF(" Now join that network on your device and connect to this IP: %s\r\n", ip);
    }
}

/*!
 * @brief The main task function
 */
static void main_task(void *arg)
{
    uint32_t result = 0;

    /* Initialize Wi-Fi board */
    PRINTF("[i] Initializing Wi-Fi connection... \r\n");

    result = WPL_Init();
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("[!] WPL Init failed: %d\r\n", (uint32_t)result);
        __BKPT(0);
    }

    result = WPL_Start(LinkStatusChangeCallback);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("[!] WPL Start failed %d\r\n", (uint32_t)result);
        __BKPT(0);
    }

    PRINTF("[i] Successfully initialized Wi-Fi module\r\n");
    ConnectTo();

    struct mg_mgr mgr;
    mg_mgr_init(&mgr);      // Initialise event manager
    mg_log_set(MG_LL_DEBUG);

    MG_INFO(("Initialising application..."));
    web_init(&mgr);

    MG_INFO(("Starting event loop"));
    for (;;) {
    	mg_mgr_poll(&mgr, 10);
    }
    vTaskDelete(NULL);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    /* Initialize the hardware */
    BOARD_InitHardware();

    /* Create the main Task */
    if (xTaskCreate(main_task, "main_task", 2048, NULL, configMAX_PRIORITIES - 1, NULL) != pdPASS)
    {
        PRINTF("[!] MAIN Task creation failed!\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    for (;;)
        ;
}
