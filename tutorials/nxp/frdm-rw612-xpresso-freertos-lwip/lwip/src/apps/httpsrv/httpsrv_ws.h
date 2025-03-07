/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/* *   This file contains public definitions for websocket protocol */
#ifndef __httpsrv_ws_h__
#define __httpsrv_ws_h__

#include "httpsrv.h"

#if HTTPSRV_CFG_WEBSOCKET_ENABLED

/*
 * WebSocket data type
 */
typedef enum ws_data_type
{
    WS_DATA_INVALID,
    WS_DATA_TEXT,
    WS_DATA_BINARY
} WS_DATA_TYPE;

/*
 * WebSocket errors
 */
typedef enum ws_error_code
{
    WS_ERR_OK,
    WS_ERR_SOCKET,
    WS_ERR_BAD_FRAME,
    WS_ERR_BAD_OPCODE,
    WS_ERR_BAD_SEQ,
    WS_ERR_NO_UTF8,
    WS_ERR_SERVER
} WS_ERROR_CODE;

/*
 * WebSocket data structure
 */
typedef struct ws_data_struct
{
    /* Pointer to user data. */
    uint8_t *data_ptr;
    /* Length of user data. */
    uint32_t length;
    /* Type of data. */
    WS_DATA_TYPE type;
} WS_DATA_STRUCT;

/*
 * Structure passed as parameter to user callbacks
 */
typedef struct ws_user_context_struct
{
    /* WebSocket handle. */
    uint32_t handle;
    /* Error code if error occurs. */
    WS_ERROR_CODE error;
    /* Data structure. */
    WS_DATA_STRUCT data;
    /* Flag signalizing end of message. */
    uint32_t fin_flag;
} WS_USER_CONTEXT_STRUCT;

/*
 * WebSocket callback function prototype.
 */
typedef uint32_t (*WS_CALLBACK_FN)(void *param, WS_USER_CONTEXT_STRUCT context);

/*
 * Structure defining WebSocket plugin.
 *
 * Usually one resource on server is mapped to one plugin.
 */
typedef struct ws_plugin_struct
{
    /* Path of resource causing plugin invocation. */
    char *resource;
    /* on_connect - when client connects to server. */
    WS_CALLBACK_FN on_connect;
    /* on_message - when message is received from client. */
    WS_CALLBACK_FN on_message;
    /* on_error - when error occurs. */
    WS_CALLBACK_FN on_error;
    /* on_disconnect - when client disconnects from server.*/
    WS_CALLBACK_FN on_disconnect;
    /* callback parameter(s) */
    void *cookie;
} WS_PLUGIN_STRUCT;

#ifdef __cplusplus
extern "C" {
#endif

int32_t WS_send(WS_USER_CONTEXT_STRUCT *context);
int32_t WS_close(uint32_t handle);

#ifdef __cplusplus
extern "C" {
#endif

#endif /* HTTPSRV_CFG_WEBSOCKET_ENABLED */

#endif
