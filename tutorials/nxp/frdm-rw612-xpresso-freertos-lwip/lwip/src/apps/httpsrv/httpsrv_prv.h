/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef HTTPSRV_PRV_H_
#define HTTPSRV_PRV_H_

#include "httpsrv.h"
#include "httpsrv_port.h"
#include "httpsrv_fs.h"
#include "httpsrv_ws_prv.h"
#include "httpsrv_tls.h"

#include <stdio.h>

#include "lwip/sys.h"

#define HTTPSRV_PRODUCT_STRING      "HTTPSRV/0.1 - NXP Embedded Web Server v0.1"
#define HTTPSRV_PROTOCOL_STRING     "HTTP/1.1"
#define HTTPSRV_CGI_VERSION_STRING  "CGI/1.1"
#define HTTPSRV_VALID               (1)
#define HTTPSRV_INVALID             (0)
#define HTTPSRV_SES_BUF_SIZE_PRV    (HTTPSRV_CFG_SES_BUFFER_SIZE)
#define HTTPSRV_TMP_BUFFER_SIZE     (128)
#define HTTPSRV_PLUGIN_NUM_MESSAGES (5)

#define HTTPSRV_FLAG_PROCESS_HEADER     (1 << 0) /* Flag for indication of header processing */
#define HTTPSRV_FLAG_HAS_HOST           (1 << 1) /* Flag determining if request header has "host" field */
#define HTTPSRV_FLAG_DO_UPGRADE         (1 << 2) /* Flag indicating if client requested connection upgrade. */
#define HTTPSRV_FLAG_IS_CACHEABLE       (1 << 3) /* Determines if response is cacheable */
#define HTTPSRV_FLAG_IS_TRANSCODED      (1 << 4) /* Transcoding flag */
#define HTTPSRV_FLAG_IS_KEEP_ALIVE      (1 << 5) /* Keep-alive status for the session */
#define HTTPSRV_FLAG_KEEP_ALIVE_ENABLED (1 << 6) /* Keep-alive enabled/disabled for session */
#define HTTPSRV_FLAG_HAS_CONTENT_LENGTH (1 << 7) /* Flag signalizing presence of Content-Length in request. */
#define HTTPSRV_FLAG_HEADER_SENT        (1 << 8) /* Flag signalizing if response header was sent. */

/*
**  Wildcard typedef for CGI/SSI callback prototype
*/
typedef int32_t (*HTTPSRV_FN_CALLBACK)(void *param);

/*
 * Plugin callbacks prototypes
 */
typedef void *(*HTTPSRV_PLUGIN_CREATE_FUNC)(void *message);
typedef void (*HTTPSRV_PLUGIN_DESTROY_FUNC)(void *plugin_context);
typedef void (*HTTPSRV_PLUGIN_WAIT_FUNC)(void *plugin_context);
typedef void (*HTTPSRV_PLUGIN_PROCESS_FUNC)(void *message, void *plugin_context);

/*
 * HTTP server plugin callbacks.
 */
typedef struct httpsrv_plugin_callbacks
{
    HTTPSRV_PLUGIN_CREATE_FUNC create;
    HTTPSRV_PLUGIN_DESTROY_FUNC destroy;
    HTTPSRV_PLUGIN_WAIT_FUNC wait;
    HTTPSRV_PLUGIN_PROCESS_FUNC process;
} HTTPSRV_PLUGIN_CALLBACKS;

/*
**  Wildcard data type for CGI/SSI callback link structure
*/
typedef struct httpsrv_fn_link_struct
{
    char *fn_name;
    HTTPSRV_FN_CALLBACK callback;
} HTTPSRV_FN_LINK_STRUCT;

/*
** Types of callbacks
*/
typedef enum httpsrv_callback_type
{
    HTTPSRV_CGI_CALLBACK,
    HTTPSRV_SSI_CALLBACK
} HTTPSRV_CALLBACK_TYPE;

/*
 * HTTP session state machine status
 */
typedef enum httpsrv_ses_state
{
    HTTPSRV_SES_WAIT_REQ,
    HTTPSRV_SES_PROCESS_REQ,
    HTTPSRV_SES_END_REQ,
    HTTPSRV_SES_CLOSE,
    HTTPSRV_SES_RESP
} HTTPSRV_SES_STATE;

/*
 * Protocols to which session can be upgraded to.
 *
 */
typedef enum httpsrv_upgrade_prot
{
    HTTPSRV_NO_PROTOCOL,
    HTTPSRV_WS_PROTOCOL,
    HTTPSRV_TLS_PROTOCOL
} HTTPSRV_UPGRADE_PROT;

/*
 * HTTP request parameters
 */
typedef struct httpsrv_req_struct
{
    HTTPSRV_REQ_METHOD method;       /* Request method (GET, POST, HEAD) */
    int content_type;                /* Request entity content type */
    int32_t content_length;          /* Content length */
    uint32_t pending;                /* Number of bytes in request not parsed */
    uint32_t lines;                  /* Number of request lines */
    char *path;                      /* Requested path */
    char *query;                     /* Data send in URL */
    HTTPSRV_AUTH_USER_STRUCT auth;   /* Authentication credentials received from client */
    HTTPSRV_UPGRADE_PROT upgrade_to; /* Protocol to upgrade to. Zero = no upgrade. */
} HTTPSRV_REQ_STRUCT;

/*
 * HTTP response parameters
 */
typedef struct httpsrv_res_struct
{
    HTTPSRV_FS_FILE *file;                       /* Handle to a file to send */
    int32_t status_code;                         /* Status code - httpsrv_sendhdr transforms it to a text */
    int32_t length;                              /* Response length */
    const HTTPSRV_AUTH_REALM_STRUCT *auth_realm; /* Authentication realm */
    int content_type;                            /* Content type */
    char script_buffer[3];                       /* Buffer for script tag search. */
} HTTPSRV_RES_STRUCT;

/*
 * HTTP session buffer
 */
typedef struct httpsrv_buffer
{
    uint32_t offset; /* Write offset */
    char *data;      /* Buffer data */
} HTTPSRV_BUFF_STRUCT;

/*
 * Session process function prototype
 */
typedef void (*HTTPSRV_SES_FUNC)(void *server, void *session);

/*
 * HTTP session structure
 */
typedef struct httpsrv_session_struct
{
    HTTPSRV_SES_FUNC process_func; /* Session process function */
    HTTPSRV_SES_STATE state;       /* HTTP session state */
    volatile uint32_t valid;       /* Any value different than HTTPSRV_VALID means session is invalid */
    volatile int sock;             /* Session socket */
    volatile uint32_t
        time;         /* Session time. Updated when there is some activity in session. Used for timeout detection. */
    uint32_t timeout; /* Session timeout in ms. timeout_time = time + timeout */
    HTTPSRV_BUFF_STRUCT buffer;        /* Session internal read/write buffer */
    HTTPSRV_REQ_STRUCT request;        /* Data read from the request */
    HTTPSRV_RES_STRUCT response;       /* Response data */
    sys_sem_t lock;                    /* Session lock */
    volatile sys_thread_t script_tid;  /* Session script handler */
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
    const WS_PLUGIN_STRUCT *plugin;    /* Plugin to be invoked for session. */
    WS_HANDSHAKE_STRUCT *ws_handshake; /* WebSocket hand-shake */
#endif
#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
    httpsrv_tls_sock_t tls_sock;
#endif
    uint32_t flags; /* Session flags */

} HTTPSRV_SESSION_STRUCT;

/*
 * Message to be send to plugin handler task.
 */
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
typedef struct httpsrv_plugin_msg
{
    uint32_t sock;
    uint8_t *buffer;
    WS_PLUGIN_STRUCT *plugin;
    HTTPSRV_PLUGIN_CALLBACKS *callbacks;
} HTTPSRV_PLUGIN_MSG;
#endif

/*
** HTTP server main structure.
*/
typedef struct httpsrv_struct
{
    HTTPSRV_PARAM_STRUCT params;               /* Server parameters */
    volatile int sock;                         /* Listening socket */
    HTTPSRV_SESSION_STRUCT *volatile *session; /* Array of pointers to sessions */
    volatile uint32_t valid;                   /* Any value different than HTTPSRV_VALID means session is invalid */
    volatile sys_thread_t server_tid;          /* Server task ID */
    void *script_msgq;                         /* Message queue for CGI */
    sys_sem_t ses_cnt;                         /* Session counter */
    sys_sem_t finished;        /* Server finished, field is used after httpsrv_destroy_server is called */
#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
    httpsrv_tls_ctx_t tls_ctx; /* TLS context */
#endif
} HTTPSRV_STRUCT;

/*
** Parameter for session task
*/
typedef struct httpsrv_ses_task_param
{
    HTTPSRV_STRUCT *server; /* Pointer to server structure */
    HTTPSRV_SESSION_STRUCT *volatile *session_p;
} HTTPSRV_SES_TASK_PARAM;

/*
 * Parameters for detached script task
 */
typedef struct httpsrv_det_task_param
{
    HTTPSRV_SESSION_STRUCT *session;   /* Session to start task for. */
    HTTPSRV_CALLBACK_TYPE type;        /* Callback type (SSI/CGI) */
    HTTPSRV_FN_CALLBACK user_function; /* User function to be called */
    uint32_t stack_size;               /* Stack size for detached task */
    char *script_name;                 /* Name of script */
} HTTPSRV_DET_TASK_PARAM;

#ifdef __cplusplus
extern "C" {
#endif

void httpsrv_server_task(void *arg);
void httpsrv_http_process(void *server_ptr, void *session_ptr);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_PRV_H_ */
