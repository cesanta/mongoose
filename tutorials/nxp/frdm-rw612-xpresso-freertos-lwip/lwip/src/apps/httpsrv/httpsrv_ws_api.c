/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*   This file contains API functions for WebSocket protocol.*/

#include "httpsrv.h"
#include "httpsrv_ws_prv.h"
#include "httpsrv_port.h"
#include "lwip/sys.h"

#if HTTPSRV_CFG_WEBSOCKET_ENABLED

/*
 * Send data through WebSocket.
 */
int32_t WS_send(WS_USER_CONTEXT_STRUCT *context)
{
    WS_CONTEXT_STRUCT *ws_context;
    WS_FRAME_STRUCT *frame;
    WS_DATA_STRUCT *data;
    WS_API_CALL_MSG *message;

    ws_context = (WS_CONTEXT_STRUCT *)context->handle;
    data       = &(context->data);

    /* Check input validity. */
    if ((data->type == WS_DATA_INVALID) || (data->data_ptr == NULL) || (ws_context == NULL))
    {
        return (HTTPSRV_ERR);
    }

    message = (WS_API_CALL_MSG *)httpsrv_mem_alloc(sizeof(WS_API_CALL_MSG));
    if (message == 0)
    {
        return HTTPSRV_ERR;
    }

    memset(message, 0, sizeof(WS_API_CALL_MSG));
    frame = &message->frame;
    /* Fill frame structure and send it */
    frame->opcode = data->type;
    frame->length = data->length;
    frame->data   = data->data_ptr;
    frame->fin    = (bool)context->fin_flag;

    message->command = WS_COMMAND_SEND;

    sys_mbox_post(&ws_context->api_queue, message);

    return (HTTPSRV_OK);
}

/*
 * Close WebSocket.
 */
int32_t WS_close(uint32_t handle)
{
    WS_CONTEXT_STRUCT *ws_context;
    WS_API_CALL_MSG *message;
    int32_t retval;

    ws_context = (WS_CONTEXT_STRUCT *)handle;

    retval = HTTPSRV_ERR;

    if (ws_context != NULL)
    {
        message = (WS_API_CALL_MSG *)httpsrv_mem_alloc(sizeof(WS_API_CALL_MSG));
        if (message == 0)
        {
            return HTTPSRV_ERR;
        }

        message->command = WS_COMMAND_CLOSE;

        sys_mbox_post(&ws_context->api_queue, message);

        retval = HTTPSRV_OK;
    }
    return (retval);
}

#endif /* HTTPSRV_CFG_WEBSOCKET_ENABLED */
