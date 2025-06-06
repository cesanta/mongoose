/** @file dhcp-server-main.c
 *
 *  @brief This file provides CLI based APIs for the DHCP Server
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** dhcp-server-main.c: CLI based APIs for the DHCP Server
 */
#include <string.h>

#include <osa.h>
#include <wm_net.h>
#include <dhcp-server.h>

#include "dhcp-priv.h"

#if !CONFIG_DHCP_SERVER_STACK_SIZE
#define CONFIG_DHCP_SERVER_STACK_SIZE 2048
#endif

static bool dhcpd_running;

void dhcpd_task(osa_task_param_t arg);

/* OSA_TASKS: name, priority, instances, stackSz, useFloat */
static OSA_TASK_DEFINE(dhcpd_task, WLAN_TASK_PRI_HIGH, 1, CONFIG_DHCP_SERVER_STACK_SIZE, 0);

OSA_TASK_HANDLE_DEFINE(dhcpd_task_Handle);

/*
 * API
 */

int dhcp_server_start(void *intrfc_handle)
{
    int ret;
    osa_status_t status;

    dhcp_d("DHCP server start request");
    if (dhcpd_running)
    {
        return -WM_E_DHCPD_SERVER_RUNNING;
    }
    ret = dhcp_server_init(intrfc_handle);
    if (ret != WM_SUCCESS)
    {
        dhcp_e("Failed to initialize dhcp server");
        return ret;
    }

    status = OSA_TaskCreate((osa_task_handle_t)dhcpd_task_Handle, OSA_TASK(dhcpd_task), NULL);
    if (status != KOSA_StatusSuccess)
    {
        (void)dhcp_free_allocations();
        return -WM_E_DHCPD_THREAD_CREATE;
    }

    dhcpd_running = 1;
    return WM_SUCCESS;
}

void dhcp_server_stop(void)
{
    dhcp_d("DHCP server stop request");
    if (dhcpd_running)
    {
        if (dhcp_send_halt() != WM_SUCCESS)
        {
            dhcp_w("failed to send halt to DHCP thread");
            return;
        }

        OSA_TimeDelay(50);

        if (OSA_TaskDestroy((osa_task_handle_t)dhcpd_task_Handle) != KOSA_StatusSuccess)
        {
            dhcp_w("failed to delete thread");
        }
        dhcpd_running = 0;
    }
    else
    {
        dhcp_w("server not dhcpd_running.");
    }
}
