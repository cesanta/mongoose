/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*   This file contains private definitions for websocket protocol */

#ifndef __httpsrv_ws_prv_h__
#define __httpsrv_ws_prv_h__

#include "httpsrv.h"

#if HTTPSRV_CFG_WEBSOCKET_ENABLED

#include <stdint.h>
#include <stdbool.h>
#include "httpsrv_ws.h"

/* GUID as defined in RFC6455. */
#define WS_GUID_STRING "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define WS_GUID_LENGTH (36)
/* WebSocket protocol version. */
#define WS_PROTOCOL_VERSION (13)
/* 16 bytes base64 encoded. */
#define WS_KEY_LENGTH (24)
/* Length base64 encoded SHA-1 of GUID concatenated with client key. */
#define WS_ACCEPT_LENGTH (28)

#define WS_CHAT_PROTOCOL_MASK      0x00000001
#define WS_SUPERCHAT_PROTOCOL_MASK 0x00000002

#define WS_AVAIL_PROTOCOL_STRING "chat"

#define WS_OPCODE_CLOSE        (0x8)
#define WS_OPCODE_PING         (0x9)
#define WS_OPCODE_PONG         (0xA)
#define WS_OPCODE_TEXT         (0x1)
#define WS_OPCODE_BINARY       (0x2)
#define WS_OPCODE_CONTINUATION (0x0)
#define WS_OPCODE_CONTROL_MASK (0x8)

/* Minimal header size for any frame (bytes). */
#define WS_MIN_HEADER_LENGTH (2)
/* Minimal header size for frame send by client (header+mask) in bytes. */
#define WS_MIN_CLIENT_HEADER_LENGTH (WS_MIN_HEADER_LENGTH + sizeof(uint32_t))
#define WS_SERVER_HEADER_LENGTH     (4)
/* Maximal header size. */
#define WS_MAX_CLIENT_HEADER_LENGTH (WS_MIN_CLIENT_HEADER_LENGTH + sizeof(uint64_t))
#define WS_LENGTH_NON_EXT           (125)
#define WS_LENGTH_EXT16             (126)
#define WS_LENGTH_EXT64             (127)
#define WS_CONTROL_FRAME_LENGTH_MAX (WS_LENGTH_NON_EXT)

#define WS_CLOSE_OK         (1000)
#define WS_CLOSE_GOING_AWAY (1001)
#define WS_CLOSE_PROT_ERROR (1002)
#define WS_CLOSE_BAD_DATA   (1003)
#define WS_CLOSE_RESERVED1  (1004)
#define WS_CLOSE_RESERVED2  (1005)
#define WS_CLOSE_RESERVED3  (1006)
#define WS_CLOSE_BAD_TYPE   (1007)
#define WS_CLOSE_POLICY     (1008)
#define WS_CLOSE_TOO_BIG    (1009)
#define WS_CLOSE_EXTENSION  (1010)
#define WS_CLOSE_SERVER_ERR (1011)
#define WS_CLOSE_RESERVED4  (1015)

#define WS_CLOSE_NOT_USED_MIN (0)
#define WS_CLOSE_NOT_USED_MAX (999)
#define WS_CLOSE_RESERVED_MIN WS_CLOSE_OK
#define WS_CLOSE_RESERVED_MAX (2999)
#define WS_CLOSE_APP_MIN      (3000)
#define WS_CLOSE_APP_MAX      (3999)
#define WS_CLOSE_PRIVATE_MIN  (4000)
#define WS_CLOSE_PRIVATE_MAX  (4999)

#define WS_FIN_MASK       (0x80)
#define WS_RSV_MASK       (0x70)
#define WS_OPCODE_MASK    (0x0F)
#define WS_MASK_FLAG_MASK (0x80)
#define WS_LENGTH_MASK    (0x7F)

#define WS_GET_FIN(x)       ((*x & WS_FIN_MASK) >> 7)
#define WS_GET_RSV(x)       ((*x & WS_RSV_MASK) >> 4)
#define WS_GET_OPCODE(x)    (*x & WS_OPCODE_MASK)
#define WS_GET_MASK_FLAG(x) ((*x & WS_MASK_FLAG_MASK) >> 7)
#define WS_GET_LENGTH(x)    (*x & WS_LENGTH_MASK)

#define WS_PING_STRING  "HTTPSRV-PING-MESSAGE"
#define WS_MIN(a, b)    (((a) < (b)) ? (a) : (b))
#define WS_NUM_MESSAGES (5)
#define WS_DO_DEINIT    (HTTPSRV_ERR)

#define WS_ERR_PASS (1)
#define WS_ERR_FAIL (0)
#define WS_DO_RECV  (2)

#define WS_SESSION_TASK_NAME "WebSocket Session"

#define WS_PLUGIN_WAIT_ACTIVITY (250)

/* Websocket ping period. (sec)*/
#define WSCFG_PING_PERIOD (30)

/*
 * WebSocket handshake structure.
 *
 */
typedef struct ws_handshake_struct
{
    /* Requested protocol version. */
    uint32_t version;
    /* List of requested protocols. */
    uint32_t protocols;
    /* Request handshake key. */
    char key[WS_KEY_LENGTH + 1];
    /* Response to client key. */
    char accept[WS_ACCEPT_LENGTH + 1];
} WS_HANDSHAKE_STRUCT;

/*
 * Type of command called by user.
 */
typedef enum ws_api_command
{
    WS_COMMAND_RECV,
    WS_COMMAND_SEND,
    WS_COMMAND_CLOSE
} WS_API_COMMAND;

/*
 * WebSocket session state
 */
typedef enum ws_ses_state
{
    WS_STATE_OPENING,
    WS_STATE_OPEN,
    WS_STATE_WAIT_PONG,
    WS_STATE_CLOSING,
    WS_STATE_CLOSED
} WS_SES_STATE;

/*
 * Receive state
 */
typedef enum ws_rx_state
{
    WS_RX_START,
    WS_RX_CONT
} WS_RX_STATE;

/*
 * WebScoket buffer structure
 */
typedef struct ws_buffer_struct
{
    /* Data pointer. */
    uint8_t *data;
    /* Write offset. */
    uint32_t offset;
    /* buffer size. */
    uint32_t size;
} WS_BUFFER_STRUCT;

/*
 * WebSocket frame structure.
 */
typedef struct ws_frame_struct
{
    /* FIN flag. */
    bool fin;
    /* Operation code. */
    uint32_t opcode;
    /* Reserved field value. */
    uint32_t reserved;
    /* Flag signalizing masked data. */
    bool masked;
    /* Only frame length up to 32 bits is supported. */
    uint32_t length;
    /* Mask for data unmasking. */
    uint32_t mask_key;
    /* Pointer to received data. */
    uint8_t *data;
} WS_FRAME_STRUCT;

typedef struct ws_utf8_struct
{
    /* Array index. */
    uint8_t index;
    /* Buffer used for UTF-8 sequence validation. */
    uint8_t data[4];
    /* Number of missing bytes required to complete validation. */
    uint32_t missing;
} WS_UTF8_STRUCT;

/*forward declaration */
struct httpsrv_session_struct;

/*
 * WebSocket context
 */
typedef struct ws_context_struct
{
    /* Session used by WS. */
    struct httpsrv_session_struct *session;
    /* Transmit buffer. */
    WS_BUFFER_STRUCT tx_buffer;
    /* Receive data buffer. */
    WS_BUFFER_STRUCT rx_buffer;
    /* Buffer for control frame. */
    WS_BUFFER_STRUCT ctrl_buffer;
    /* Buffer for frame header. */
    WS_BUFFER_STRUCT hdr_buffer;
    /* Pointer to actual buffer. */
    WS_BUFFER_STRUCT *actual_buffer;
    /* Session state. */
    WS_SES_STATE state;
    /* Receive transfer state. */
    WS_RX_STATE rx_state;
    /* User context (for callbacks). */
    WS_USER_CONTEXT_STRUCT u_context;
    /* Received frame structure. */
    WS_FRAME_STRUCT frame;
    /* UTF-8 validation structure. */
    WS_UTF8_STRUCT utf8;
    /* Number of data bytes received. */
    uint32_t rec_data_size;
    /*
     * Number of bytes of data (not frame header)
     * remaining to read from socket.
     */
    uint32_t remaining_data;
    /* Message queue. Required for serialization of API calls. */
    sys_mbox_t api_queue;
    /* Flag signaling that user transfer is in progress. */
    uint8_t u_transfer;
    /* Mask offset. Required for data unmasking. */
    uint8_t mask_offset;
} WS_CONTEXT_STRUCT;

/*
 * Message send from API to session task
 */
typedef struct ws_api_call_msg
{
    /* Type of command (send or close). */
    WS_API_COMMAND command;
    /* Pointer to frame to be send (ignored for close command). */
    WS_FRAME_STRUCT frame;
} WS_API_CALL_MSG;

#ifdef __cplusplus
extern "C" {
#endif

void ws_session_task(void *init_ptr, void *creator);
void ws_handshake(WS_HANDSHAKE_STRUCT *handshake);
void ws_session_run(struct httpsrv_session_struct *session);

#ifdef __cplusplus
}
#endif

#endif /* HTTPSRV_CFG_WEBSOCKET_ENABLED */

#endif
