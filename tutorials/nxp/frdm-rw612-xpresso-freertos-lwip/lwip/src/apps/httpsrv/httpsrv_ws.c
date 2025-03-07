/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*   This file contains implementation of WebSocket protocol.*/

#include "httpsrv_ws_prv.h"

#if HTTPSRV_CFG_WEBSOCKET_ENABLED

#include "httpsrv_sha1.h"
#include "httpsrv_base64.h"
#include "httpsrv_utf8.h"
#include "httpsrv_supp.h"

#define UINT16MAX 65536

static uint32_t ws_read_frame_header(uint8_t *data, WS_FRAME_STRUCT *dst, uint32_t length);
static void ws_write_frame(uint8_t *dst, WS_FRAME_STRUCT *src);
static void ws_unmask_data(uint8_t *data, uint32_t mask, uint32_t length, uint8_t mask_offset);
static WS_ERROR_CODE ws_check_frame_header(WS_CONTEXT_STRUCT *context, WS_FRAME_STRUCT *frame);
static uint32_t ws_get_header_size(WS_FRAME_STRUCT *frame);
static inline uint32_t ws_get_frame_size(WS_FRAME_STRUCT *frame);
static int32_t ws_data_frame(WS_CONTEXT_STRUCT *context);
static int32_t ws_process_frame(WS_CONTEXT_STRUCT *context);
static void ws_bad_frame(WS_CONTEXT_STRUCT *context, WS_ERROR_CODE frame_status);
static uint32_t ws_check_close_reason(uint16_t reason);
static uint32_t ws_close_frame(WS_CONTEXT_STRUCT *context);
static int ws_ping_frame(WS_CONTEXT_STRUCT *context);
static uint32_t ws_pong_frame(WS_CONTEXT_STRUCT *context);
static inline void ws_update_mask_offset(WS_CONTEXT_STRUCT *context);
static uint32_t ws_process_api_calls(WS_CONTEXT_STRUCT *context);
static int ws_send_control_frame(WS_CONTEXT_STRUCT *context, uint8_t *data, uint32_t length, uint32_t opcode);
static int ws_send_frame(WS_CONTEXT_STRUCT *context, WS_FRAME_STRUCT *frame);
static uint32_t ws_check_utf8(WS_CONTEXT_STRUCT *context, uint8_t *data, uint32_t length);
static uint32_t ws_validate_data(WS_CONTEXT_STRUCT *context);
static bool ws_check_utf8_finalize(uint8_t *data);
static uint32_t ws_recv(WS_CONTEXT_STRUCT *context);
static uint32_t ws_recv_fail(WS_CONTEXT_STRUCT *context);
static uint32_t ws_init(HTTPSRV_SESSION_STRUCT *session, WS_CONTEXT_STRUCT **context_out);
static void ws_deinit(WS_CONTEXT_STRUCT *context);
static uint32_t ws_process(WS_CONTEXT_STRUCT *context);

/*
 * WebSocket session task.
 */
void ws_session_run(HTTPSRV_SESSION_STRUCT *session)
{
    WS_CONTEXT_STRUCT *context;

    if (ws_init(session, &context) != WS_ERR_OK)
    {
        /* Exit on error.*/
        if (context != NULL)
        {
            httpsrv_mem_free(context);
        }
    }
    else
    {
        while (ws_process(context) != WS_ERR_FAIL)
        {
        }

        ws_deinit(context);
    }
}

/*
 * Upgrade HTTP session to WebSocket session.
 */
void ws_handshake(WS_HANDSHAKE_STRUCT *handshake)
{
    const char *guid = WS_GUID_STRING;
    SHA1_CTX sha1_context;
    char sha1_sum[SHA1_DIGEST_SIZE];

    /* Get SHA-1 of key and WebSocket GUID and encode it in base64 */
    SHA1_Init(&sha1_context);
    SHA1_Update(&sha1_context, (uint8_t *)handshake->key, WS_KEY_LENGTH);
    SHA1_Update(&sha1_context, (uint8_t *)guid, WS_GUID_LENGTH);
    SHA1_Final(&sha1_context, (uint8_t *)sha1_sum);
    base64_encode_binary(sha1_sum, handshake->accept, SHA1_DIGEST_SIZE);
}

/*
 * Initialize WebSocket protocol.
 */
uint32_t ws_init(HTTPSRV_SESSION_STRUCT *session, WS_CONTEXT_STRUCT **context_out)
{
    const WS_PLUGIN_STRUCT *plugin = session->plugin;
    WS_CONTEXT_STRUCT *context;

/* Set receive timeout to ping period (in seconds) */
#if LWIP_SO_RCVTIMEO
    {
        struct timeval timeval_option;
        timeval_option.tv_sec  = WSCFG_PING_PERIOD; /* seconds */
        timeval_option.tv_usec = 0;                 /* and microseconds */
        (void)lwip_setsockopt(session->sock, SOL_SOCKET, SO_RCVTIMEO, (const void *)&timeval_option,
                              sizeof(timeval_option));
    }
#endif

    /* Create WebSocket context structure */
    context = httpsrv_mem_alloc_zero(sizeof(WS_CONTEXT_STRUCT));
    if (context == NULL)
    {
        *context_out = NULL;
        return (WS_ERR_FAIL);
    }
    *context_out = context;

    context->session = session;

    /* Set handle so it is available for callbacks */
    context->u_context.handle = (uint32_t)context;
    context->state            = WS_STATE_OPEN;

    /* Init UTF-8 buffer. */
    memset((void *)context->utf8.data, UTF8_TAIL_MIN, sizeof(context->utf8.data));

    /* Setup buffers - re-use HTTP session buffer; split it in half */
    context->tx_buffer.data   = (uint8_t *)session->buffer.data;
    context->tx_buffer.size   = HTTPSRV_SES_BUF_SIZE_PRV / 2;
    context->tx_buffer.offset = 0;

    context->hdr_buffer.data   = context->tx_buffer.data + context->tx_buffer.size;
    context->hdr_buffer.size   = WS_MAX_CLIENT_HEADER_LENGTH;
    context->hdr_buffer.offset = 0;

    context->rx_buffer.data   = context->hdr_buffer.data + context->hdr_buffer.size;
    context->rx_buffer.size   = HTTPSRV_SES_BUF_SIZE_PRV / 2 - WS_MAX_CLIENT_HEADER_LENGTH;
    context->rx_buffer.offset = 0;

    context->ctrl_buffer.data = (uint8_t *)httpsrv_mem_alloc(WS_CONTROL_FRAME_LENGTH_MAX);
    if (context->ctrl_buffer.data == NULL)
    {
        return (WS_ERR_FAIL);
    }
    context->ctrl_buffer.size   = WS_CONTROL_FRAME_LENGTH_MAX;
    context->ctrl_buffer.offset = 0;

    context->actual_buffer = &context->hdr_buffer;

    /* Init message queue for API calls*/
    if (sys_mbox_new(&context->api_queue, WS_NUM_MESSAGES) != ERR_OK)
    {
        httpsrv_mem_free(context->ctrl_buffer.data);
        return (WS_ERR_FAIL);
    }

    /* Call user connect callback and set session state */
    if (plugin->on_connect)
    {
        plugin->on_connect(plugin->cookie, context->u_context);
    }
    return (WS_ERR_OK);
}

/*
 * Process WebSocket session.
 */
static uint32_t ws_process(WS_CONTEXT_STRUCT *context)
{
    WS_FRAME_STRUCT *rec_frame;
    WS_BUFFER_STRUCT *buffer;
    uint32_t retval;
    uint32_t api_status;

    retval    = WS_ERR_PASS;
    buffer    = context->actual_buffer;
    rec_frame = &context->frame;

    api_status = ws_process_api_calls(context);
    if (api_status == WS_ERR_FAIL)
    {
        retval = WS_ERR_FAIL;
        goto EXIT;
    }
    else if (api_status == WS_DO_RECV)
    {
        if (ws_recv(context) != WS_ERR_PASS)
        {
            retval = WS_ERR_FAIL;
            goto EXIT;
        }
        buffer = context->actual_buffer;

        /* Unmask data and set context pointer to it */
        ws_unmask_data(buffer->data + (buffer->offset - context->rec_data_size), rec_frame->mask_key,
                       context->rec_data_size, context->mask_offset);
        ws_update_mask_offset(context);
        if (ws_validate_data(context) != WS_ERR_PASS)
        {
            ws_bad_frame(context, WS_ERR_NO_UTF8);
            retval = WS_ERR_FAIL;
            goto EXIT;
        }

        /* Process frame */
        if (ws_process_frame(context) == HTTPSRV_ERR)
        {
            retval = WS_ERR_FAIL;
            goto EXIT;
        }
    }

EXIT:
    return (retval);
}

/*
 * Receive data from socket.
 */
static uint32_t ws_recv(WS_CONTEXT_STRUCT *context)
{
    WS_FRAME_STRUCT *frame;
    WS_BUFFER_STRUCT *buffer;
    uint32_t received_size;
    uint32_t req_length;

    buffer                 = context->actual_buffer;
    context->rec_data_size = 0;

    if (buffer == &context->hdr_buffer)
    {
        req_length                  = WS_MIN_CLIENT_HEADER_LENGTH;
        context->hdr_buffer.offset  = 0;
        context->ctrl_buffer.offset = 0;
        context->mask_offset        = 0;
    }
    else
    {
        req_length = WS_MIN(buffer->size, context->remaining_data);
    }

    frame = &context->frame;

    while (req_length > 0)
    {
        uint32_t size;

        size = WS_MIN(buffer->size - buffer->offset, req_length);
        if (size == 0)
        {
            break;
        }

        received_size = httpsrv_recv(context->session, (char *)(buffer->data + buffer->offset), size, 0);
        if (received_size < 1) /* 0 means connection is closed.*/
        {
            return (ws_recv_fail(context));
        }

        buffer->offset += received_size;

        /* If we are reading frame header, receive data until whole header is in buffer. */
        if (buffer == &context->hdr_buffer)
        {
            req_length = ws_read_frame_header(buffer->data, frame, buffer->offset);
            if (req_length == 0)
            {
                WS_ERROR_CODE frame_status;

                frame_status = ws_check_frame_header(context, frame);
                if (frame_status != WS_ERR_OK)
                {
                    /* Report bad frame to client and close connection. */
                    ws_bad_frame(context, frame_status);
                    return (WS_ERR_BAD_FRAME);
                }
                if (frame->opcode & WS_OPCODE_CONTROL_MASK)
                {
                    buffer     = &context->ctrl_buffer;
                    req_length = frame->length;
                }
                else
                {
                    buffer     = &context->rx_buffer;
                    req_length = WS_MIN(frame->length, buffer->size);
                }
                frame->data             = buffer->data;
                context->remaining_data = frame->length;
            }
            continue;
        }
        else
        {
            req_length -= received_size;
            context->rec_data_size += received_size;
            context->remaining_data -= received_size;
        }
    }
    context->actual_buffer = buffer;
    return (WS_ERR_PASS);
}

/*
 * Receive from socket failed.
 */
static uint32_t ws_recv_fail(WS_CONTEXT_STRUCT *context)
{
    const WS_PLUGIN_STRUCT *plugin;
    int optval;
    socklen_t optlen = sizeof(optval);

    plugin = context->session->plugin;

    /* If receive timed out and ping was not send, send it */
    if (((lwip_getsockopt(context->session->sock, SOL_SOCKET, SO_ERROR, &optval, &optlen) == 0) &&
         (optval == EWOULDBLOCK)) &&
        (context->state != WS_STATE_WAIT_PONG))
    {
        ws_send_control_frame(context, (uint8_t *)WS_PING_STRING, strlen(WS_PING_STRING), WS_OPCODE_PING);
        context->state = WS_STATE_WAIT_PONG;
        return (WS_ERR_FAIL);
    }
    /*
     * Socket is gone or response to ping was not received (client
     * disconnected, etc.)
     */
    else
    {
        WS_USER_CONTEXT_STRUCT *user_context;

        user_context        = &context->u_context;
        user_context->error = WS_ERR_SOCKET;
        if (plugin->on_error)
        {
            plugin->on_error(plugin->cookie, *user_context);
        }
        user_context->error = WS_ERR_OK;
        return (WS_ERR_FAIL);
    }
}

/*
 * Validate input data.
 */
static uint32_t ws_validate_data(WS_CONTEXT_STRUCT *context)
{
    WS_FRAME_STRUCT *frame;
    WS_BUFFER_STRUCT *buffer;
    WS_USER_CONTEXT_STRUCT *user_context;
    uint32_t result;

    user_context = &context->u_context;
    buffer       = context->actual_buffer;
    frame        = &context->frame;
    result       = WS_ERR_PASS;

    if (((frame->opcode == WS_OPCODE_TEXT) ||
         ((frame->opcode == WS_OPCODE_CONTINUATION) && (user_context->data.type == WS_DATA_TEXT))))
    {
        uint8_t *data;
        uint32_t length;

        data   = buffer->data + (buffer->offset - context->rec_data_size);
        length = WS_MIN(context->rec_data_size, frame->length);
        result = ws_check_utf8(context, data, length);
    }
    return (result);
}

/*
 * Read all message from API message queue.
 */
static uint32_t ws_process_api_calls(WS_CONTEXT_STRUCT *context)
{
    WS_API_CALL_MSG *message;
    uint32_t retval;
    int32_t active = 0;
    fd_set readset;
    fd_set exceptset;
    struct timeval timeout;
    void *mem;

    retval = WS_ERR_PASS;

    while (active == 0)
    {
        FD_ZERO(&readset);
        FD_ZERO(&exceptset);

        FD_SET(context->session->sock, &readset);
        FD_SET(context->session->sock, &exceptset);

        timeout.tv_sec  = 0;
        timeout.tv_usec = 1;

        /* Wait for any activity on socket - received data or exception. */
        active = select(context->session->sock + 1, &readset, NULL, &exceptset, &timeout);
        if (active == -1)
        {
            retval = WS_ERR_FAIL;
            break;
        }
        else if (active >= 1)
        {
            if (FD_ISSET(context->session->sock, &exceptset))
            {
                /* Close connection.*/
                retval = WS_ERR_FAIL;
                break;
            }

            if (FD_ISSET(context->session->sock, &readset))
            {
                /* Socket is read-ready. */
                retval = WS_DO_RECV;
            }
        }

        /* Check Messages. Indicate call of user API. */
        if (sys_mbox_valid(&context->api_queue))
        {
            while (sys_mbox_tryfetch(&context->api_queue, &mem) != SYS_MBOX_EMPTY)
            {
                if (mem != NULL)
                {
                    message = (WS_API_CALL_MSG *)mem;

                    uint16_t close_reason;

                    switch (message->command)
                    {
                        case WS_COMMAND_SEND:
                            /* If user transfer is in progress, set opcode to continuation. */
                            if (context->u_transfer == 1)
                            {
                                message->frame.opcode = WS_OPCODE_CONTINUATION;
                            }
                            /* No FIN -> set user transfer in progress flag. */
                            if (message->frame.fin == false)
                            {
                                context->u_transfer = 1;
                            }
                            if (ws_send_frame(context, &message->frame) == -1)
                            {
                                retval = WS_ERR_FAIL;
                            }
                            break;
                        case WS_COMMAND_CLOSE:
                            /* Send close frame, set correct state. */
                            retval       = WS_ERR_SERVER;
                            close_reason = PP_NTOHS(WS_CLOSE_GOING_AWAY);
                            if (ws_send_control_frame(context, (uint8_t *)&close_reason, sizeof(close_reason),
                                                      WS_OPCODE_CLOSE) == -1)
                            {
                                retval = WS_ERR_FAIL;
                            }
                            context->state = WS_STATE_CLOSING;
                            break;
                        default:
                            break;
                    }

                    if ((message->command != WS_COMMAND_SEND) || (retval == WS_ERR_FAIL))
                    {
                        httpsrv_mem_free(message);
                        goto EXIT;
                    }
                    httpsrv_mem_free(message);
                }
            }
        }
    } /* while */
EXIT:
    return (retval);
}

/*
 * Update mask offset according to remaining data.
 */
static inline void ws_update_mask_offset(WS_CONTEXT_STRUCT *context)
{
    WS_FRAME_STRUCT *frame;

    frame = &context->frame;
    /*
     * If we do not store data on the beginning of buffer, correct mask
     * offset must be calculated.
     */
    if ((context->remaining_data != 0) && (context->remaining_data < frame->length))
    {
        context->mask_offset =
            ((((frame->length - context->remaining_data) / 4) * 4) + 4) - (frame->length - context->remaining_data);
    }
}

/*
 * Process received frame.
 */
static int32_t ws_process_frame(WS_CONTEXT_STRUCT *context)
{
    WS_FRAME_STRUCT *frame;
    WS_USER_CONTEXT_STRUCT *user_context;
    int retval;

    retval       = HTTPSRV_ERR;
    user_context = &context->u_context;
    frame        = &context->frame;

    switch (frame->opcode)
    {
        case WS_OPCODE_TEXT:
            user_context->data.type = WS_DATA_TEXT;
            retval                  = ws_data_frame(context);
            break;
        case WS_OPCODE_BINARY:
            user_context->data.type = WS_DATA_BINARY;
            retval                  = ws_data_frame(context);
            break;
        case WS_OPCODE_CONTINUATION:
            retval = ws_data_frame(context);
            break;
        case WS_OPCODE_CLOSE:
            retval = ws_close_frame(context);
            break;
        case WS_OPCODE_PING:
            retval = ws_ping_frame(context);
            break;
        case WS_OPCODE_PONG:
            retval = ws_pong_frame(context);
            break;
        default:
            ws_bad_frame(context, WS_ERR_BAD_OPCODE);
            break;
    }
    if (retval != HTTPSRV_ERR)
    {
        retval = HTTPSRV_OK;
    }
    return (retval);
}

/*
 * Validate UTF-8 sequence.
 */
static uint32_t ws_check_utf8(WS_CONTEXT_STRUCT *context, uint8_t *data, uint32_t length)
{
    uint8_t *bad;
    uint32_t missing_now;
    uint32_t missing_prev;

    missing_now  = 0;
    missing_prev = context->utf8.missing;

    if (length == 0)
    {
        return (WS_ERR_PASS);
    }

    /*
     * If validation in previous step required more data to complete.
     */
    if (missing_prev != 0)
    {
        uint32_t size;

        size = WS_MIN(missing_prev, length);
        memcpy(context->utf8.data + context->utf8.index, data, size);
        context->utf8.index += size;

        if (!utf8_is_valid(context->utf8.data, context->utf8.index, &bad, &missing_now))
        {
            /*
             * If validation failed for second time, there are no missing bytes
             * required for validation and we have last frame of message,
             * validation failed.
             */
            if ((missing_now == 0) || ((context->remaining_data == 0) && (context->frame.fin == 1)) ||
                !ws_check_utf8_finalize(context->utf8.data))
            {
                return (WS_ERR_FAIL);
            }
            context->utf8.missing = missing_now;
            return (WS_ERR_PASS);
        }
        /*
         * If validation is successful reset stored UTF-8 context  and continue
         * with next step.
         */
        data += size;
        length -= size;
        memset((void *)context->utf8.data, UTF8_TAIL_MIN, sizeof(context->utf8.data));
        context->utf8.missing = 0;
        context->utf8.index   = 0;
    }

    if (!utf8_is_valid(data, length, &bad, &missing_now))
    {
        uint32_t size;

        /* Validation failed and there are no missing bytes. */
        if ((missing_now == 0) || (context->frame.fin == 1))
        {
            return (WS_ERR_FAIL);
        }
        size = WS_MIN(sizeof(context->utf8.data) - missing_now, length);
        memcpy(context->utf8.data, bad, size);
        context->utf8.missing = missing_now;
        context->utf8.index   = size;
    }
    return (WS_ERR_PASS);
}

/*
 * Finalize UTF-8 check.
 */
static bool ws_check_utf8_finalize(uint8_t *data)
{
    uint8_t c;

    c = *data;

    /* Find first zero bit and call check function. */
    if ((c | 0x7F) != 0xFF)
    {
        return (utf8_check_1(data));
    }
    else if ((c | 0xDF) != 0xFF)
    {
        return (utf8_check_2(data));
    }
    else if ((c | 0xEF) != 0xFF)
    {
        return (utf8_check_3(data));
    }
    else if ((c | 0xF7) != 0xFF)
    {
        return (utf8_check_4(data));
    }
    else
    {
        return (false);
    }
}

/*
 * Check validity of close reason.
 */
static uint32_t ws_check_close_reason(uint16_t reason)
{
    uint32_t retval = WS_ERR_FAIL;

    reason = ntohs(reason);

    /* Check if close reason lies within valid range */
    if ((reason >= WS_CLOSE_RESERVED_MIN) && (reason <= WS_CLOSE_PRIVATE_MAX))
    {
        if (((reason <= WS_CLOSE_SERVER_ERR) && (reason != WS_CLOSE_RESERVED1) && (reason != WS_CLOSE_RESERVED2) &&
             (reason != WS_CLOSE_RESERVED3)) ||
            (reason >= WS_CLOSE_APP_MIN))
        {
            retval = WS_ERR_PASS;
        }
    }
    return (retval);
}

/*
 * Process close frame.
 */
static uint32_t ws_close_frame(WS_CONTEXT_STRUCT *context)
{
    WS_FRAME_STRUCT *frame;
    uint16_t close_reason;
    uint32_t retval;

    retval = HTTPSRV_OK;
    frame  = &context->frame;
    /* We initiated close and we are waiting for close from client */
    if (context->state == WS_STATE_CLOSING)
    {
        context->state = WS_STATE_CLOSED;
        retval         = (uint32_t)WS_DO_DEINIT;
    }
    /* Client initiated close, so we reply with close frame + proper code */
    else if (context->remaining_data == 0)
    {
        if (frame->length == 0)
        {
            close_reason = ntohs(WS_CLOSE_OK);
        }
        else if (frame->length >= 2)
        {
            close_reason = *((uint16_t *)frame->data);

            /* Check close reason. */
            if (ws_check_close_reason(close_reason) != WS_ERR_PASS)
            {
                close_reason = ntohs(WS_CLOSE_PROT_ERROR);
            }

            /* If there is some message, verify that it is UTF-8 string. */
            if ((frame->length > 2) && (ws_check_utf8(context, frame->data + sizeof(uint16_t),
                                                      frame->length - sizeof(uint16_t)) != WS_ERR_PASS))
            {
                close_reason = ntohs(WS_CLOSE_BAD_TYPE);
            }
        }
        else
        {
            close_reason = ntohs(WS_CLOSE_PROT_ERROR);
        }
        ws_send_control_frame(context, (uint8_t *)&close_reason, sizeof(close_reason), WS_OPCODE_CLOSE);
        context->state = WS_STATE_CLOSED;
        retval         = (uint32_t)WS_DO_DEINIT;
    }
    return (retval);
}

/*
 * Process ping frame.
 */
static int ws_ping_frame(WS_CONTEXT_STRUCT *context)
{
    WS_FRAME_STRUCT *frame;
    int retval;

    frame = &context->frame;

    retval                 = ws_send_control_frame(context, frame->data, frame->length, WS_OPCODE_PONG);
    context->actual_buffer = &context->hdr_buffer;
    return (retval);
}

/*
 * Process pong frame.
 */
static uint32_t ws_pong_frame(WS_CONTEXT_STRUCT *context)
{
    uint32_t retval;

    retval = HTTPSRV_OK;

    /* If pong is solicited, reset state to open */
    if (context->state == WS_STATE_WAIT_PONG)
    {
        context->state = WS_STATE_OPEN;
    }
    context->actual_buffer = &context->hdr_buffer;
    return (retval);
}

/*
 * Call user callback if required.
 */
static int32_t ws_data_frame(WS_CONTEXT_STRUCT *context)
{
    WS_BUFFER_STRUCT *buffer;
    WS_USER_CONTEXT_STRUCT *user_context;
    WS_FRAME_STRUCT *frame;
    const WS_PLUGIN_STRUCT *plugin;
    int32_t retval = HTTPSRV_OK;

    plugin       = context->session->plugin;
    user_context = &context->u_context;
    buffer       = context->actual_buffer;
    frame        = &context->frame;

    /* Store length of received data in user context. */
    user_context->data.length += WS_MIN(frame->length, context->rec_data_size);
    user_context->data.data_ptr = buffer->data;

    /*
     * If there is no space for frame header or all data are read,
     * call on_message callback with received data.
     */
    if ((buffer->size - buffer->offset) == 0)
    {
        if (plugin->on_message)
        {
            retval = plugin->on_message(plugin->cookie, *user_context);
        }
        else
        {
            retval = 0;
        }
        user_context->fin_flag      = 0;
        user_context->data.length   = 0;
        user_context->data.data_ptr = NULL;
        context->remaining_data -= user_context->data.length;
        context->actual_buffer->offset = 0;
    }
    else if (context->remaining_data == 0)
    {
        if (frame->fin == true)
        {
            user_context->fin_flag = 1;
            context->rx_state      = WS_RX_START;
            if (plugin->on_message)
            {
                retval = plugin->on_message(plugin->cookie, *user_context);
            }
            else
            {
                retval = 0;
            }
            user_context->fin_flag         = 0;
            user_context->data.length      = 0;
            user_context->data.data_ptr    = NULL;
            context->actual_buffer->offset = 0;
        }
        context->actual_buffer = &context->hdr_buffer;
    }
    /*
     * If frame is first in transmission and there are going to be continuation
     * frames, set correct receive state; so we can detect wrong frame sequence.
     */
    if ((frame->fin == false) && (frame->opcode != WS_OPCODE_CONTINUATION) && (context->rx_state == WS_RX_START))
    {
        context->rx_state = WS_RX_CONT;
    }
    return retval;
}

/*
 * Release WebSocket.
 */
static void ws_deinit(WS_CONTEXT_STRUCT *context)
{
    const WS_PLUGIN_STRUCT *plugin;

    plugin = context->session->plugin;

    /* Call user disconnect callback */
    if (plugin->on_disconnect)
    {
        plugin->on_disconnect(plugin->cookie, context->u_context);
    }

    if (context->api_queue != NULL)
    {
        void *message;
        /* Delete and drain the api_queue. */
        while (sys_mbox_tryfetch(&context->api_queue, &message) != SYS_MBOX_EMPTY)
        {
            if (message)
            {
                httpsrv_mem_free(message);
            }
        }
        sys_mbox_free(&context->api_queue);
    }

    if (context->ctrl_buffer.data != NULL)
    {
        httpsrv_mem_free(context->ctrl_buffer.data);
    }

    httpsrv_mem_free(context);

    /*Socket is closed later.*/
}

/*
 * Unmask data from client.
 */
static void ws_unmask_data(uint8_t *data, uint32_t mask, uint32_t length, uint8_t mask_offset)
{
    uint32_t i;
    uint32_t words;
    uint32_t mask_key = mask;

    /* If there is offset, shift mask to correct position first. */
    if (mask_offset > 0)
    {
        mask_offset = mask_offset * 8;
        mask_key <<= mask_offset;
        mask_key |= mask >> (sizeof(mask) * 8 - mask_offset);
    }

    words = length / 4;

    /* Unmask whole 32-bit long words first. */
    for (i = 0; i < words; i++)
    {
        *(((uint32_t *)data) + i) ^= mask_key;
    }

    /* Unmask rest of data (less then four bytes). */
    for (i = words * 4; i < length; i++)
    {
        data[i] ^= *((uint8_t *)&mask_key + i % 4);
    }
}

/*
 * Fill WebSocket frame structure from received data
 */
static uint32_t ws_read_frame_header(uint8_t *data, WS_FRAME_STRUCT *dst, uint32_t length)
{
    uint8_t *src = data;
    uint32_t req_length;

    /* Minimum is always required. */
    req_length = WS_MIN_CLIENT_HEADER_LENGTH;
    if ((src == NULL) || (dst == NULL))
    {
        return (req_length);
    }

    if (length < req_length)
    {
        return (req_length - length);
    }

    dst->fin      = WS_GET_FIN(src);
    dst->reserved = WS_GET_RSV(src);
    dst->opcode   = WS_GET_OPCODE(src);
    src++;
    dst->masked = WS_GET_MASK_FLAG(src);
    dst->length = WS_GET_LENGTH(src);
    src++;
    if (dst->length == WS_LENGTH_EXT16)
    {
        /* Length of 126 indicates that length is stored in next two bytes. */
        req_length += sizeof(uint16_t);
        if (length < req_length)
        {
            return (req_length - length);
        }
        dst->length = ntohs(*((uint16_t *)src));
        src += sizeof(uint16_t);
    }
    else if (dst->length == WS_LENGTH_EXT64)
    {
        /* Length of 127 indicates that length is stored in next eight bytes. */
        req_length += sizeof(uint64_t);
        if (length < req_length)
        {
            return (req_length - length);
        }
        /* Only frame sizes up to 32-bit long are supported by this implementation. */
        dst->length = ntohl(*((uint32_t *)src + 1));
        src += sizeof(uint64_t);
    }
    if (dst->masked == true)
    {
        dst->mask_key = *((uint32_t *)src);
        src += sizeof(uint32_t);
    }
    dst->data = data;
    return (0);
}

/*
 * Create frame header in buffer from frame structure.
 */
static void ws_write_frame(uint8_t *dst, WS_FRAME_STRUCT *src)
{
    memmove(dst + ws_get_header_size(src), src->data, src->length);
    *dst = 0;
    if (src->fin)
    {
        *dst |= WS_FIN_MASK;
    }
    *dst |= *((uint8_t *)(&src->opcode)) & WS_OPCODE_MASK;
    dst++;
    *dst = 0;

    if (src->length <= WS_LENGTH_NON_EXT)
    {
        *dst++ = *((uint8_t *)(&src->length)) & WS_LENGTH_MASK;
    }
    else if ((src->length > WS_LENGTH_NON_EXT) & (src->length < UINT16MAX))
    {
        *dst++             = WS_LENGTH_EXT16;
        *((uint16_t *)dst) = htons(src->length & 0xFFFF);
        dst += sizeof(uint16_t);
    }
    else if (src->length >= UINT16MAX)
    {
        *dst++             = WS_LENGTH_EXT64;
        *((uint64_t *)dst) = htonl(src->length);
        dst += sizeof(uint64_t);
    }

    if (src->masked)
    {
        *((uint32_t *)dst) = src->mask_key;
        dst += sizeof(uint32_t);
    }
}

/*
 * Check frame validity
 */
static WS_ERROR_CODE ws_check_frame_header(WS_CONTEXT_STRUCT *context, WS_FRAME_STRUCT *frame)
{
    WS_ERROR_CODE retval = WS_ERR_OK;

    /* Data MUST be masked. */
    if (frame->masked == false)
    {
        retval = WS_ERR_BAD_FRAME;
    }

    /* There MUST be non-zero mask key. */
    else if (frame->mask_key == 0)
    {
        retval = WS_ERR_BAD_FRAME;
    }

    /* Reserved part of frame header MUST be zero. */
    if (frame->reserved > 0)
    {
        retval = WS_ERR_BAD_FRAME;
    }

    /* Opcode MUST be valid. */
    if (((frame->opcode > WS_OPCODE_BINARY) && (frame->opcode < WS_OPCODE_CLOSE)) || (frame->opcode > WS_OPCODE_PONG))
    {
        retval = WS_ERR_BAD_FRAME;
    }

    if ((frame->opcode & WS_OPCODE_CONTROL_MASK) &&       /* If frame is control frame and */
        ((frame->length > WS_CONTROL_FRAME_LENGTH_MAX) || /* its length is greater than 125 bytes or */
         (frame->fin == false)                            /* it is fragmented. */
         ))
    {
        retval = WS_ERR_BAD_FRAME; /* Frame is invalid. */
    }

    if (((context->rx_state == WS_RX_CONT) &&         /* If we expect continuation and */
         !(frame->opcode & WS_OPCODE_CONTROL_MASK) && /* frame is not control frame and */
         (frame->opcode != WS_OPCODE_CONTINUATION)    /* frame is not continuation. */
         ) ||                                         /* Or if */
        ((context->rx_state == WS_RX_START) &&        /* We expect first frame in sequence and */
         (frame->opcode == WS_OPCODE_CONTINUATION)    /* frame is continuation. */
         ))
    {
        retval = WS_ERR_BAD_SEQ; /* Bad frame sequence is received. */
    }

    return (retval);
}

/*
 * Resolve bad frame, and report it to client.
 */
static void ws_bad_frame(WS_CONTEXT_STRUCT *context, WS_ERROR_CODE frame_status)
{
    uint16_t close_reason = 0;
    WS_USER_CONTEXT_STRUCT *user_context;
    const WS_PLUGIN_STRUCT *plugin;

    plugin       = context->session->plugin;
    user_context = &context->u_context;

    /* Send close with protocol error. */
    switch (frame_status)
    {
        case WS_ERR_BAD_OPCODE:
            close_reason = htons(WS_CLOSE_POLICY);
            break;
        case WS_ERR_NO_UTF8:
            close_reason = htons(WS_CLOSE_BAD_TYPE);
            break;
        case WS_ERR_BAD_FRAME:
        case WS_ERR_BAD_SEQ:
        default:
            close_reason = htons(WS_CLOSE_PROT_ERROR);
            break;
    }
    ws_send_control_frame(context, (uint8_t *)&close_reason, sizeof(close_reason), WS_OPCODE_CLOSE);
    context->state = WS_STATE_CLOSING;

    /* Call user callback */
    user_context->error         = WS_ERR_BAD_FRAME;
    user_context->data.type     = WS_DATA_INVALID;
    user_context->data.data_ptr = NULL;
    user_context->data.length   = 0;
    if (plugin->on_error)
    {
        plugin->on_error(plugin->cookie, *user_context);
    }
    user_context->error = WS_ERR_OK;
}

/*
 * Get total frame size (header + data)
 */
static inline uint32_t ws_get_frame_size(WS_FRAME_STRUCT *frame)
{
    return (ws_get_header_size(frame) + frame->length);
}

/*
 * Get frame header size in bytes.
 */
static uint32_t ws_get_header_size(WS_FRAME_STRUCT *frame)
{
    uint32_t retval = sizeof(uint16_t);

    if ((frame->length > 0) && (frame->length >= WS_LENGTH_EXT16))
    {
        if (frame->length < UINT16MAX)
        {
            retval += sizeof(uint16_t);
        }
        else
        {
            retval += sizeof(uint64_t);
        }
    }

    if (frame->masked == true)
    {
        retval += sizeof(uint32_t);
    }

    return (retval);
}

/*
 * Send data to client. Use framing mechanism if required.
 */
static int ws_send_frame(WS_CONTEXT_STRUCT *context, WS_FRAME_STRUCT *frame)
{
    WS_BUFFER_STRUCT *buffer;
    uint32_t data_length;
    int retval;
    bool user_fin;
    const WS_PLUGIN_STRUCT *plugin;

    plugin      = context->session->plugin;
    buffer      = &context->tx_buffer;
    data_length = frame->length;
    user_fin    = frame->fin;

    do
    {
        /* First case: We have all data from user and it fits in buffer. */
        if (((data_length + ws_get_header_size(frame)) < buffer->size) && user_fin)
        {
            frame->fin    = true;
            frame->length = data_length;
        }
        /* Second case: We have data from user, but it does not fit in buffer. */
        else if (((data_length + ws_get_header_size(frame)) > buffer->size) && user_fin)
        {
            frame->fin    = false;
            frame->length = buffer->size;
        }
        /*
         * Third and fourth case: We do not have all data from user (no fin)
         * and it does fit in buffer OR we do have all data from user but
         * no fin flag.
         */
        else
        {
            frame->fin    = false;
            frame->length = data_length;
        }
        if (frame->fin == true)
        {
            context->u_transfer = 0;
        }
        /* Write frame to buffer and send it. */
        ws_write_frame(buffer->data, frame);
        retval = httpsrv_send(context->session, (char *)buffer->data, ws_get_frame_size(frame), 0);
        if (retval == -1)
        {
            break;
        }
        retval -= ws_get_header_size(frame);
        /*
         * Set opcode to continuation for next frame,
         * move data pointer, decrease remaining data length.
         */
        frame->opcode = WS_OPCODE_CONTINUATION;
        frame->data += retval;
        data_length -= retval;
    } while (data_length > 0);

    /* If send failed call error callback. */
    if (retval == -1)
    {
        WS_USER_CONTEXT_STRUCT *user_context;

        user_context                = &context->u_context;
        user_context->error         = WS_ERR_SOCKET;
        user_context->data.data_ptr = frame->data;
        user_context->data.type     = WS_DATA_INVALID;
        user_context->data.length   = frame->length;
        if (plugin->on_error)
        {
            plugin->on_error(plugin->cookie, *user_context);
        }
        user_context->error = WS_ERR_OK;
    }

    return (retval);
}

/*
 * Send control frame.
 */
static int ws_send_control_frame(WS_CONTEXT_STRUCT *context, uint8_t *data, uint32_t length, uint32_t opcode)
{
    WS_FRAME_STRUCT frame = {0};

    frame.data   = data;
    frame.length = length;
    frame.opcode = opcode;
    frame.fin    = true;

    return (ws_send_frame(context, &frame));
}

#endif /* HTTPSRV_CFG_WEBSOCKET_ENABLED */
