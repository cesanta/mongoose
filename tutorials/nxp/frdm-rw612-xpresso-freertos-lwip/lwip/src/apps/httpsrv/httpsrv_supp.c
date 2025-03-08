/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 *   HTTPSRV support functions.
 */

#include "httpsrv.h"
#include "httpsrv_prv.h"
#include "httpsrv_supp.h"
#include "httpsrv_script.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>
#include "httpsrv_base64.h"
#include "httpsrv_tls.h"
#include "lwip/def.h"
#include "lwip/sockets.h"

#define HTTPSRV_CALLBACK_TASK_NAME "HTTP server callback handler"

/*
 * string table item
 */
typedef struct httpsrv_table_row
{
    int id;
    char *str;
} HTTPSRV_TABLE_ROW;

/* Structure defining MIME types table row */
typedef struct httpsrv_content_table_row
{
    uint32_t length;  /* extension string length (strlen) */
    char *ext;        /* extension string */
    int content_type; /* extension MIME type string */
    bool use_cache;   /* cache use */
} HTTPSRV_CONTENT_TABLE_ROW;

/*
 * content type
 */
static const HTTPSRV_TABLE_ROW content_type[] = {{HTTPSRV_CONTENT_TYPE_PLAIN, "text/plain"},
                                                 {HTTPSRV_CONTENT_TYPE_HTML, "text/html"},
                                                 {HTTPSRV_CONTENT_TYPE_CSS, "text/css"},
                                                 {HTTPSRV_CONTENT_TYPE_GIF, "image/gif"},
                                                 {HTTPSRV_CONTENT_TYPE_JPG, "image/jpeg"},
                                                 {HTTPSRV_CONTENT_TYPE_PNG, "image/png"},
                                                 {HTTPSRV_CONTENT_TYPE_SVG, "image/svg+xml"},
                                                 {HTTPSRV_CONTENT_TYPE_JS, "application/javascript"},
                                                 {HTTPSRV_CONTENT_TYPE_XML, "application/xml"},
                                                 {HTTPSRV_CONTENT_TYPE_ZIP, "application/zip"},
                                                 {HTTPSRV_CONTENT_TYPE_PDF, "application/pdf"},
                                                 {HTTPSRV_CONTENT_TYPE_OCTETSTREAM, "application/octet-stream"},
                                                 {HTTPSRV_CONTENT_TYPE_FORMURLENC, "application/x-www-form-urlencoded"},
                                                 {HTTPSRV_CONTENT_TYPE_FORMDATA, "multipart/form-data"},
                                                 {0, 0}};

/*
 * Response status to reason conversion table
 */
static const HTTPSRV_TABLE_ROW reason_phrase[] = {
    {HTTPSRV_CODE_CONTINUE, "Continue"},
    {HTTPSRV_CODE_UPGRADE, "Switching Protocols"},
    {HTTPSRV_CODE_OK, "OK"},
    {HTTPSRV_CODE_CREATED, "Created"},
    {HTTPSRV_CODE_ACCEPTED, "Accepted"},
    {HTTPSRV_CODE_NON_AUTHORITATIVE, "Non-Authoritative Information"},
    {HTTPSRV_CODE_NO_CONTENT, "No Content"},
    {HTTPSRV_CODE_RESET_CONTENT, "Reset Content"},
    {HTTPSRV_CODE_PARTIAL_CONTENT, "Partial Content"},
    {HTTPSRV_CODE_MULTIPLE_CHOICES, "Multiple Choices"},
    {HTTPSRV_CODE_MOVED_PERMANENTLY, "Moved Permanently"},
    {HTTPSRV_CODE_FOUND, "Found"},
    {HTTPSRV_CODE_SEE_OTHER, "See Other"},
    {HTTPSRV_CODE_NOT_MODIFIED, "Not Modified"},
    {HTTPSRV_CODE_USE_PROXY, "Use Proxy"},
    {HTTPSRV_CODE_TEMPORARY_REDIRECT, "Temporary Redirect"},
    {HTTPSRV_CODE_BAD_REQ, "Bad Request"},
    {HTTPSRV_CODE_UNAUTHORIZED, "Unauthorized"},
    {HTTPSRV_CODE_PAYMENT_REQUIRED, "Payment Required"},
    {HTTPSRV_CODE_FORBIDDEN, "Forbidden"},
    {HTTPSRV_CODE_NOT_FOUND, "Not Found"},
    {HTTPSRV_CODE_METHOD_NOT_ALLOWED, "Method Not Allowed"},
    {HTTPSRV_CODE_NOT_ACCEPTABLE, "Not Acceptable"},
    {HTTPSRV_CODE_PROXY_AUTH_REQUIRED, "Proxy Authentication Required"},
    {HTTPSRV_CODE_REQUEST_TIMEOUT, "Request Time-out"},
    {HTTPSRV_CODE_CONFLICT, "Conflict"},
    {HTTPSRV_CODE_GONE, "Gone"},
    {HTTPSRV_CODE_NO_LENGTH, "Length Required"},
    {HTTPSRV_CODE_PRECONDITION_FAILED, "Precondition Failed"},
    {HTTPSRV_CODE_ENTITY_TOO_LARGE, "Request Entity Too Large"},
    {HTTPSRV_CODE_URI_TOO_LONG, "Request-URI Too Large"},
    {HTTPSRV_CODE_UNSUPPORTED_MEDIA, "Unsupported Media Type"},
    {HTTPSRV_CODE_RANGE_NOT_SATISFIABLE, "Requested range not satisfiable"},
    {HTTPSRV_CODE_EXPECTATION_FAILED, "Expectation Failed"},
    {HTTPSRV_CODE_UPGRADE_REQUIRED, "Upgrade Required"},
    {HTTPSRV_CODE_FIELD_TOO_LARGE, "Request Header Fields Too Large"},
    {HTTPSRV_CODE_INTERNAL_ERROR, "Internal Server Error"},
    {HTTPSRV_CODE_NOT_IMPLEMENTED, "Not Implemented"},
    {HTTPSRV_CODE_BAD_GATEWAY, "Bad Gateway"},
    {HTTPSRV_CODE_SERVICE_UNAVAILABLE, "Service Unavailable"},
    {HTTPSRV_CODE_GATEWAY_TIMEOUT, "Gateway Time-out"},
    {HTTPSRV_CODE_VERSION_NOT_SUPPORTED, "HTTP Version not supported"},
    {0, 0}};

/*
** Extension -> content type conversion table.
** This table rows MUST be ordered by size and alphabetically
** so we can list through it quickly
*/
static const HTTPSRV_CONTENT_TABLE_ROW content_tbl[] = {
    /* Size,          extension, MIME type,                        Cache? */
    {sizeof("js") - 1, "js", HTTPSRV_CONTENT_TYPE_JS, true},
    {sizeof("css") - 1, "css", HTTPSRV_CONTENT_TYPE_CSS, true},
    {sizeof("gif") - 1, "gif", HTTPSRV_CONTENT_TYPE_GIF, true},
    {sizeof("htm") - 1, "htm", HTTPSRV_CONTENT_TYPE_HTML, true},
    {sizeof("jpg") - 1, "jpg", HTTPSRV_CONTENT_TYPE_JPG, true},
    {sizeof("pdf") - 1, "pdf", HTTPSRV_CONTENT_TYPE_PDF, false},
    {sizeof("png") - 1, "png", HTTPSRV_CONTENT_TYPE_PNG, true},
    {sizeof("svg") - 1, "svg", HTTPSRV_CONTENT_TYPE_SVG, true},
    {sizeof("txt") - 1, "txt", HTTPSRV_CONTENT_TYPE_PLAIN, false},
    {sizeof("xml") - 1, "xml", HTTPSRV_CONTENT_TYPE_XML, false},
    {sizeof("zip") - 1, "zip", HTTPSRV_CONTENT_TYPE_ZIP, false},
    {sizeof("html") - 1, "html", HTTPSRV_CONTENT_TYPE_HTML, true},
    {sizeof("shtm") - 1, "shtm", HTTPSRV_CONTENT_TYPE_HTML, false},
    {sizeof("shtml") - 1, "shtml", HTTPSRV_CONTENT_TYPE_HTML, false},
    /* Following row MUST have length set to zero so we have proper array termination */
    {0, "", HTTPSRV_CONTENT_TYPE_OCTETSTREAM, false}};

static uint32_t httpsrv_sendextstr(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session, uint32_t length);
static void httpsrv_print(HTTPSRV_SESSION_STRUCT *session, char *format, ...);
static char *httpsrv_get_table_str(HTTPSRV_TABLE_ROW *table, const int32_t id);
static int httpsrv_get_table_int(HTTPSRV_TABLE_ROW *table, char *str);
static void httpsrv_process_file_type(char *extension, HTTPSRV_SESSION_STRUCT *session);
static int32_t httpsrv_set_params(HTTPSRV_STRUCT *server, HTTPSRV_PARAM_STRUCT *params);
static int32_t httpsrv_init_socket(HTTPSRV_STRUCT *server);
static int httpsrv_basic_auth(char *auth_string, char **user_ptr, char **pass_ptr);
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
static void *httpsrv_ws_alloc(HTTPSRV_SESSION_STRUCT *session);
#endif

#if HTTPSRV_CFG_WEBSOCKET_ENABLED

/*
** Find plugin for selected resource in table.
**
** IN:
**      WS_PLUGIN_STRUCT* table - table to search in.
**      char*                   resource - resource name.
**
** OUT:
**      none
**
** Return Value:
**      WS_PLUGIN_STRUCT* plugin structure. NULL if not found.
*/
const WS_PLUGIN_STRUCT *httpsrv_get_ws_plugin(const WS_PLUGIN_STRUCT *table, char *resource)
{
    const WS_PLUGIN_STRUCT *retval = NULL;

    if ((table != NULL) && (resource != NULL))
    {
        while (table->resource != NULL)
        {
            if (strcmp(resource, table->resource) == 0)
            {
                retval = table;
                break;
            }
            table++;
        }
    }
    return (retval);
}

/*
 * Allocate WebSocket handshake structure.
 */
void *httpsrv_ws_alloc(HTTPSRV_SESSION_STRUCT *session)
{
    if (session->ws_handshake == NULL)
    {
        session->ws_handshake = httpsrv_mem_alloc_zero(sizeof(WS_HANDSHAKE_STRUCT));
    }
    if (session->ws_handshake == NULL)
    {
        session->response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
    }
    return (session->ws_handshake);
}

#endif

/*
 * Wait for incoming connection, return socket with activity or error.
 */
int httpsrv_wait_for_conn(HTTPSRV_STRUCT *server)
{
    fd_set readset;
    int32_t retval = -1;

    FD_ZERO(&readset);
    FD_SET(server->sock, &readset);

    if (lwip_select(server->sock + 1, (fd_set *)&readset, 0, 0, 0) == 1)
    {
        if (FD_ISSET(server->sock, &readset))
        {
            retval = server->sock;
        }
    }
    return (retval);
}

/*
 * Allocate server structure, init sockets, etc.
 */
HTTPSRV_STRUCT *httpsrv_create_server(HTTPSRV_PARAM_STRUCT *params)
{
    HTTPSRV_STRUCT *server = NULL;
    int32_t error;

    if ((server = httpsrv_mem_alloc_zero(sizeof(HTTPSRV_STRUCT))) == NULL)
    {
        return (NULL);
    }

    error = httpsrv_set_params(server, params);
    if (error != HTTPSRV_OK)
    {
        goto EXIT;
    }

    error = sys_sem_new(&server->ses_cnt, server->params.max_ses);
    if (error != ERR_OK)
    {
        goto EXIT;
    }

    /* Allocate space for session pointers */
    server->session = httpsrv_mem_alloc_zero(sizeof(HTTPSRV_SESSION_STRUCT *) * server->params.max_ses);
    if (server->session == NULL)
    {
        goto EXIT;
    }

    /* Init sockets. */
    error = httpsrv_init_socket(server);

    if (error != HTTPSRV_OK)
    {
        goto EXIT;
    }
    return (server);

EXIT:
    httpsrv_destroy_server(server);
    httpsrv_mem_free(server);
    return (NULL);
}

/*
 * Close sockets, free memory etc.
 */
void httpsrv_destroy_server(HTTPSRV_STRUCT *server)
{
    uint32_t n = 0;
    bool wait  = false;

    if (server)
    {
        if (server->valid)
        {
            server->valid = HTTPSRV_INVALID;
            if (server->sock != -1)
            {
                lwip_close(server->sock);
                server->sock = -1;
            }
        }

        if (server->session)
        {
            /* Invalidate sessions (this is signal for session tasks to end them) */
            for (n = 0; n < server->params.max_ses; n++)
            {
                if (server->session[n])
                {
                    server->session[n]->valid = HTTPSRV_INVALID;
                }
            }
            /* Wait until all session tasks end */
            do
            {
                wait = false;
                for (n = 0; n < server->params.max_ses; n++)
                {
                    if (server->session[n])
                    {
                        wait = true;
                        break;
                    }
                }
                taskYIELD();
            } while (wait);

            httpsrv_mem_free((void *)server->session);
            server->session = NULL;
        }

        if (server->ses_cnt)
        {
            sys_sem_free(&server->ses_cnt);
        }

        /* server->finished is deallocated later */

#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
        if (server->tls_ctx)
        {
            httpsrv_tls_release(server->tls_ctx);
        }
#endif

        /* Free memory */
        server->params.root_dir   = NULL;
        server->params.index_page = NULL;
    }
}

/*
** Function for socket initialization (both IPv4 and IPv6)
**
** IN:
**      HTTPSRV_STRUCT* server - server structure pointer
**
**      uint16_t      family - IP protocol family
** OUT:
**      none
**
** Return Value:
**      uint32_t      error code. HTTPSRV_OK if everything went right, positive number otherwise
*/
static int32_t httpsrv_init_socket(HTTPSRV_STRUCT *server)
{
    int error;
    int option;

    if ((server->sock = lwip_socket(server->params.address.ss_family, SOCK_STREAM, 0)) < 0)
    {
        return (HTTPSRV_CREATE_FAIL);
    }

    /* Set socket options */
    /* Disable Nagle algorithm.*/
    option = 1;
    error  = lwip_setsockopt(server->sock, IPPROTO_TCP, TCP_NODELAY, (const void *)&option, sizeof(option));
    if (error)
    {
        return (HTTPSRV_SOCKOPT_FAIL);
    }

#if SO_REUSE
    /* set options to reuse local ip addr for socket */
    option = 1;
    error  = lwip_setsockopt(server->sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&option, sizeof(option));
    if (error)
    {
        return (HTTPSRV_SOCKOPT_FAIL);
    }
#endif

    /* SO_RCVBUF is not implemented for TCP.*/
    /* SO_SNDBUF is not implemented.*/

    /* Bind socket */
#if LWIP_IPV6
    if (server->params.address.ss_family == AF_INET6)
    {
        error = lwip_bind(server->sock, (struct sockaddr *)&server->params.address, sizeof(struct sockaddr_in6));
    }
    else
#endif
#if LWIP_IPV4
        if (server->params.address.ss_family == AF_INET)
    {
        error = lwip_bind(server->sock, (struct sockaddr *)&server->params.address, sizeof(struct sockaddr_in));
    }
    else
#endif
    {
        error = -1;
    }
    if (error == -1)
    {
        return (HTTPSRV_BIND_FAIL);
    }

    /* Listen */
    error = listen(server->sock, 0);
    if (error == -1)
    {
        return (HTTPSRV_LISTEN_FAIL);
    }
    return (HTTPSRV_OK);
}

/*
** Internal function for server parameters initialization
**
** IN:
**      HTTPSRV_STRUCT* server - server structure pointer
**
**      HTTPSRV_PARAM_STRUCT* params - pointer to user parameters if there are any
** OUT:
**      none
**
** Return Value:
**      uint32_t      error code. HTTPSRV_OK if everything went right, positive number otherwise
*/
static int32_t httpsrv_set_params(HTTPSRV_STRUCT *server, HTTPSRV_PARAM_STRUCT *params)
{
#if LWIP_IPV6
    server->params.address.ss_family                              = AF_INET6;
    ((struct sockaddr_in6 *)(&server->params.address))->sin6_port = PP_HTONS(HTTPSRV_CFG_DEFAULT_HTTP_PORT);
#elif LWIP_IPV4
    server->params.address.ss_family                            = AF_INET;
    ((struct sockaddr_in *)(&server->params.address))->sin_port = PP_HTONS(HTTPSRV_CFG_DEFAULT_HTTP_PORT);
#endif

    server->params.max_uri     = HTTPSRV_CFG_DEFAULT_URL_LEN;
    server->params.max_ses     = HTTPSRV_CFG_DEFAULT_SES_CNT;
    server->params.root_dir    = "";
    server->params.index_page  = HTTPSRV_CFG_DEFAULT_INDEX_PAGE;
    server->params.cgi_lnk_tbl = NULL;
    server->params.ssi_lnk_tbl = NULL;
    server->params.task_prio   = HTTPSRV_CFG_DEFAULT_PRIORITY;
    server->params.auth_table  = NULL;
#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
    server->tls_ctx = NULL;
#endif

    /* If there is parameters structure copy nonzero values to server */
    if (params != NULL)
    {
        if (params->address.ss_family)
            server->params.address.ss_family = params->address.ss_family;

#if LWIP_IPV6
        if (server->params.address.ss_family == AF_INET6)
        {
            if (((struct sockaddr_in6 *)(&params->address))->sin6_port)
                ((struct sockaddr_in6 *)(&server->params.address))->sin6_port =
                    ((struct sockaddr_in6 *)(&params->address))->sin6_port;
        }
        else
#endif
#if LWIP_IPV4
            if (server->params.address.ss_family == AF_INET)
        {
            if (((struct sockaddr_in *)(&params->address))->sin_port)
                ((struct sockaddr_in *)(&server->params.address))->sin_port =
                    ((struct sockaddr_in *)(&params->address))->sin_port;
        }
        else
#endif
        {
        } /* wrong family detexcted by socket().*/

        if (params->max_uri)
            server->params.max_uri = params->max_uri;
        if (params->max_ses)
            server->params.max_ses = params->max_ses;
        if (params->root_dir)
        {
            server->params.root_dir = params->root_dir;
        }
        if (params->index_page)
        {
            server->params.index_page = params->index_page;
        }
        if (params->cgi_lnk_tbl)
            server->params.cgi_lnk_tbl = params->cgi_lnk_tbl;
        if (params->ssi_lnk_tbl)
            server->params.ssi_lnk_tbl = params->ssi_lnk_tbl;
        if (params->task_prio)
            server->params.task_prio = params->task_prio;
        if (params->auth_table)
            server->params.auth_table = params->auth_table;
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
        if (params->ws_tbl)
            server->params.ws_tbl = params->ws_tbl;
#endif

#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
        if (params->tls_param)
        {
            server->tls_ctx = httpsrv_tls_init(params->tls_param);
            if (server->tls_ctx == NULL)
            {
                return (HTTPSRV_ERR);
            }

            /* Set default HTTPS port.*/
#if LWIP_IPV6
            if (server->params.address.ss_family == AF_INET6)
            {
                if (((struct sockaddr_in6 *)(&params->address))->sin6_port == 0)

                    ((struct sockaddr_in6 *)(&server->params.address))->sin6_port =
                        PP_HTONS(HTTPSRV_CFG_DEFAULT_HTTPS_PORT);
            }
            else
#endif
#if LWIP_IPV4
                if (server->params.address.ss_family == AF_INET)
            {
                /* Set default port.*/
                if (((struct sockaddr_in *)(&params->address))->sin_port == 0)
                {
                    ((struct sockaddr_in *)(&server->params.address))->sin_port =
                        PP_HTONS(HTTPSRV_CFG_DEFAULT_HTTPS_PORT);
                }
            }
            else
#endif
            {
            }
        }
#endif
        /* Server must run with lower priority than TCP/IP task. */
        if (server->params.task_prio > TCPIP_THREAD_PRIO)
        {
            server->params.task_prio = TCPIP_THREAD_PRIO;
        }
    }

    return (HTTPSRV_OK);
}

/*
 * Accept connection from client.
 */
int httpsrv_accept(int sock)
{
    struct sockaddr remote_addr;
    socklen_t length;

    memset(&remote_addr, 0, sizeof(remote_addr));
    length = sizeof(remote_addr);

    return (lwip_accept(sock, &remote_addr, &length));
}

/*
 * Abort connection on socket.
 */
void httpsrv_abort(int sock)
{
    //    struct linger l_options;

    /* Set linger options for RST flag sending. */
    //    l_options.l_onoff = 1;
    //    l_options.l_linger_ms = 0;
    //   setsockopt(sock, SOL_SOCKET, SO_LINGER, &l_options, sizeof(l_options));
    lwip_close(sock);
}

/*
 * Receive data from socket.
 */
int httpsrv_recv(HTTPSRV_SESSION_STRUCT *session, char *buffer, size_t length, int flags)
{
    int result;
#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
    if (session->tls_sock != 0)
    {
        result = httpsrv_tls_recv(session->tls_sock, buffer, length, flags);
    }
    else
#endif
    {
        result = lwip_recv(session->sock, buffer, length, flags);
    }

    return result;
}

/*
 * Send data to socket
 */
int httpsrv_send(HTTPSRV_SESSION_STRUCT *session, const char *buffer, size_t length, int flags)
{
    int result;
#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
    if (session->tls_sock != 0)
    {
        result = httpsrv_tls_send(session->tls_sock, buffer, length, flags);
    }
    else
#endif
    {
        result = lwip_send(session->sock, buffer, length, flags);
    }

    return result;
}

/*
** Send HTTP header according to the session response structure.
**
** IN:
**      HTTPSRV_SESSION_STRUCT* session - session used for transmission
**      int32_t                content_len - content length
**      bool                 has_entity - flag indicating if HTTP entity is going to be send following header.
**
** OUT:
**      none
**
** Return Value:
**      none
*/
void httpsrv_sendhdr(HTTPSRV_SESSION_STRUCT *session, int32_t content_len, bool has_entity)
{
    char *connection_state;
    char *phrase;

    if (session->flags & HTTPSRV_FLAG_HEADER_SENT)
    {
        return;
    }
    phrase = httpsrv_get_table_str((HTTPSRV_TABLE_ROW *)reason_phrase, session->response.status_code);
    if (phrase == NULL)
    {
        phrase = "-";
    }
    httpsrv_print(session, HTTPSRV_PROTOCOL_STRING " %d %s\r\n", session->response.status_code, phrase);

    if (session->flags & HTTPSRV_FLAG_DO_UPGRADE)
    {
        connection_state = "Upgrade";
    }
    else
    {
        connection_state = (session->flags & HTTPSRV_FLAG_IS_KEEP_ALIVE) ? "Keep-Alive" : "close";
    }
    httpsrv_print(session, "Connection: %s\r\n", connection_state);

#if HTTPSRV_CFG_WEBSOCKET_ENABLED
    /* Send WebSocket protocol handshake response if there was WebSocket upgrade request. */
    if (session->request.upgrade_to == HTTPSRV_WS_PROTOCOL)
    {
        httpsrv_print(session, "Upgrade: WebSocket\r\n");

        if (session->ws_handshake->version != WS_PROTOCOL_VERSION)
        {
            httpsrv_print(session, "Sec-WebSocket-Version: %d\r\n", WS_PROTOCOL_VERSION);
        }
        if (session->ws_handshake->protocols & WS_CHAT_PROTOCOL_MASK)
        {
            httpsrv_print(session, "Sec-WebSocket-Protocol: %s\r\n", WS_AVAIL_PROTOCOL_STRING);
        }
        if (strlen(session->ws_handshake->accept) == WS_ACCEPT_LENGTH)
        {
            httpsrv_print(session, "Sec-WebSocket-Accept: %s\r\n", session->ws_handshake->accept);
        }
    }
#endif

    httpsrv_print(session, "Server: %s\r\n", HTTPSRV_PRODUCT_STRING);

    /* Check authorization */
    if (session->response.status_code == HTTPSRV_CODE_UNAUTHORIZED)
    {
        httpsrv_print(session, "WWW-Authenticate: Basic realm=\"%s\"\r\n", session->response.auth_realm->name);
    }

    /* If there will be entity body send content type */
    if (has_entity)
    {
        httpsrv_print(session, "Content-Type: %s\r\n",
                      httpsrv_get_table_str((HTTPSRV_TABLE_ROW *)content_type, session->response.content_type));
    }

    if (session->response.status_code != HTTPSRV_CODE_UPGRADE)
    {
        httpsrv_print(session, "Cache-Control: ");
        if (session->flags & HTTPSRV_FLAG_IS_CACHEABLE)
        {
            httpsrv_print(session, "max-age=%d\r\n", HTTPSRV_CFG_CACHE_MAXAGE);
        }
        else
        {
            if (session->response.auth_realm != NULL)
            {
                httpsrv_print(session, "no-store\r\n");
            }
            else
            {
                httpsrv_print(session, "no-cache\r\n");
            }
        }
    }
    /* Only non zero length cause sending Content-Length header field */
    if (content_len > 0)
    {
        httpsrv_print(session, "Content-Length: %d\r\n", content_len);
    }

    /* Handle transfer encoding. */
    if (session->flags & HTTPSRV_FLAG_IS_TRANSCODED)
    {
        httpsrv_print(session, "Transfer-Encoding: chunked\r\n");
    }
    /* End of header */
    httpsrv_print(session, "\r\n");

    /* Commented out to prevent problems with file system on KHCI USB */
    // if ((content_len == 0) && (!has_entity))
    {
        httpsrv_ses_flush(session);
    }
    session->flags |= HTTPSRV_FLAG_HEADER_SENT;
}

/*
** Send error page to client
**
** IN:
**      HTTPSRV_SESSION_STRUCT* session - session used for transmission
**      const char*             title - title of error page
**      const char*             text - text displayed on error page
**
** OUT:
**      none
**
** Return Value:
**      none
*/
void httpsrv_send_err_page(HTTPSRV_SESSION_STRUCT *session, const char *title, const char *text)
{
    uint32_t length;
    char *page;

    length = strlen(title) + strlen(text) + sizeof(ERR_PAGE_FORMAT) + 1;
    page   = httpsrv_mem_alloc(length);

    session->response.content_type = HTTPSRV_CONTENT_TYPE_HTML;

    if (page != NULL)
    {
        snprintf(page, length, ERR_PAGE_FORMAT, title, text);
        httpsrv_sendhdr(session, strlen(page), 1);
        httpsrv_write(session, page, strlen(page));
        httpsrv_ses_flush(session);
        httpsrv_mem_free(page);
    }
    else
    {
        httpsrv_sendhdr(session, 0, 0);
    }
}

/*
** Convert file extension to content type and determine what kind of cache control should be used.
**
** IN:
**      char* extension - extension to convert
**
** IN/OUT:
**      HTTPSRV_SESSION_STRUCT* session - session pointer
**
** Return Value:
**      none
*/
static void httpsrv_process_file_type(char *extension, HTTPSRV_SESSION_STRUCT *session)
{
    const HTTPSRV_CONTENT_TABLE_ROW *row = content_tbl;
    uint32_t length                      = 0;

    if (extension != NULL)
    {
        length = strlen(extension) - 1;
        /* Move pointer after the dot. */
        extension++;
    }
    else
    {
        goto NO_EXT;
    }

    /* List through table rows until length match */
    while ((row->length) && (row->length < length))
    {
        row++;
    }

    /* Do a search in valid rows */
    while (row->length == length)
    {
        if (lwip_stricmp(extension, row->ext) == 0)
        {
            session->response.content_type = row->content_type;
            if (row->use_cache)
            {
                session->flags |= HTTPSRV_FLAG_IS_CACHEABLE;
            }

            if (session->response.auth_realm != NULL)
            {
                /* If authentication is required, then response MUST NOT be cached */
                session->flags &= ~HTTPSRV_FLAG_IS_CACHEABLE;
            }
            return;
        }
        row++;
    }

NO_EXT:
    session->response.content_type = HTTPSRV_CONTENT_TYPE_OCTETSTREAM;
    session->flags &= ~HTTPSRV_FLAG_IS_CACHEABLE;
}

/*
** Send file to client
**
** IN:
**      HTTPSRV_SESSION_STRUCT* session - session used for transmission
**      HTTPSRV_STRUCT*         server - server structure
**
** OUT:
**      none
**
** Return Value:
**      none
*/
HTTPSRV_SES_STATE httpsrv_sendfile(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session)
{
    char *ext;
    int length;
    char *buffer;
    HTTPSRV_SES_STATE retval;

    buffer = session->buffer.data;

    ext = strrchr(session->request.path, '.');
    httpsrv_process_file_type(ext, session);

    /* Check if file has server side includes */
    if ((0 == lwip_stricmp(ext, ".shtml")) || (0 == lwip_stricmp(ext, ".shtm")))
    {
        /*
         * Disable keep-alive for this session otherwise we would have to
         * wait for session timeout.
         */
        session->flags &= ~HTTPSRV_FLAG_IS_KEEP_ALIVE;
        httpsrv_sendhdr(session, 0, 1);

        HTTPSRV_FS_fseek(session->response.file, session->response.length, HTTPSRV_FS_IO_SEEK_SET);

        length = HTTPSRV_FS_read(session->response.file, buffer + session->buffer.offset,
                                 HTTPSRV_SES_BUF_SIZE_PRV - session->buffer.offset);
        if (length > 0)
        {
            uint32_t offset;

            offset = httpsrv_sendextstr(server, session, length);
            session->response.length += offset;
            session->response.length += session->buffer.offset;
            httpsrv_ses_flush(session);
        }
    }
    else
    {
        httpsrv_sendhdr(session, HTTPSRV_FS_size(session->response.file), 1);
        HTTPSRV_FS_fseek(session->response.file, session->response.length, HTTPSRV_FS_IO_SEEK_SET);

        length = HTTPSRV_FS_read(session->response.file, buffer + session->buffer.offset,
                                 HTTPSRV_SES_BUF_SIZE_PRV - session->buffer.offset);
        //  fread(buffer+session->buffer.offset, 1, HTTPSRV_SES_BUF_SIZE_PRV-session->buffer.offset,
        //  session->response.file);
        if (length > 0)
        {
            session->buffer.offset += length;
            length = httpsrv_ses_flush(session);
            if (length != -1)
            {
                session->response.length += length;
            }
        }
    }

    if (length <= 0)
    {
        retval = HTTPSRV_SES_END_REQ;
    }
    else
    {
        retval = HTTPSRV_SES_RESP;
    }
    return (retval);
}

/*
** Send extended string to socket (dynamic web pages).
**
** IN:
**      HTTPSRV_STRUCT         *server - server structure.
**      HTTPSRV_SESSION_STRUCT *session - session for sending.
**      char                   *str - string to send.
**      uint32_t                length - length of source string.
**
** OUT:
**      none
**
** Return Value:
**      int - number of bytes processed.
*/
static uint32_t httpsrv_sendextstr(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session, uint32_t length)
{
    char *src;
    char *t_buffer;
    uint32_t n;
    uint32_t retval;

    t_buffer = session->response.script_buffer;
    src      = session->buffer.data + session->buffer.offset;

    n = strlen(t_buffer);

    if (n == 0) /* Start searching for script token. */
    {
        uint32_t n_send;
        uint32_t max;
        uint32_t i;
        char token[] = "<%";

        i   = 0;
        max = length;

        for (n_send = 0; (n_send < max) && (token[i]); n_send++)
        {
            if (src[n_send] == token[i])
            {
                t_buffer[i] = token[i];
                i++;
                max = ((n_send + 2) < length) ? n_send + 2 : length;
            }
            else
            {
                i                                  = 0;
                max                                = length;
                session->response.script_buffer[0] = 0;
            }
        }
        session->buffer.offset = n_send - i;
        retval                 = i;
    }
    else if (n == 1) /* There was already the less-than sign.*/
    {
        if (*src == '%')
        {
            /* There is script token spanning over two buffers. */
            t_buffer[n] = *src;
            retval      = 1;
        }
        else
        {
            /* There was no script token, send missing less-than sign. */
            httpsrv_send(session, session->response.script_buffer, n, 0);
            memset(session->response.script_buffer, 0, sizeof(session->response.script_buffer));
            retval = 0;
        }
    }
    else /* Script token beginning found, find end. */
    {
        uint32_t name_length;
        char fname[HTTPSRV_CFG_MAX_SCRIPT_LN] = {0};
        uint32_t term_length;

        name_length = strcspn(src, " ;%<>\r\n\t\f");

        if ((src[name_length] == '%') && (src[name_length + 1] == '>'))
        {
            term_length = 2;
        }
        else
        {
            term_length = 1;
        }

        if ((name_length > 1) && (name_length < HTTPSRV_CFG_MAX_SCRIPT_LN))
        {
            snprintf(fname, name_length + 1, "%s", src);
            /* Wait until SSI is processed. */
            httpsrv_script_handler(server, session, HTTPSRV_SSI_CALLBACK, fname);
            memset(session->response.script_buffer, 0, sizeof(session->response.script_buffer));
        }
        retval = name_length + term_length;
    }
    return (retval);
}

/*
** Write data to buffer. If buffer is full during write flush it to client.
**
** IN:
**      HTTPSRV_SESSION_STRUCT *session - session used for write.
**      char*                   src     - pointer to data to send.
**      int32_t                length     - length of data in bytes.
**
** OUT:
**      none
**
** Return Value:
**      int32_t - number of bytes written.
*/
int32_t httpsrv_write(HTTPSRV_SESSION_STRUCT *session, char *src, int32_t length)
{
    uint32_t space = HTTPSRV_SES_BUF_SIZE_PRV - session->buffer.offset;
    int32_t retval = length;
    uint32_t n_send;

    /* User buffer is bigger than session buffer - send user data directly */
    if (length > HTTPSRV_SES_BUF_SIZE_PRV)
    {
        /* If there are some data already buffered send them to client first */
        if (httpsrv_ses_flush(session) == -1)
        {
            return (-1);
        }
        else
        {
            return (httpsrv_send(session, src, length, 0));
        }
    }

    /* No space in buffer - make some */
    if ((space < length) && (httpsrv_ses_flush(session) == -1))
    {
        return (-1);
    }

    /* Now we can save user data to buffer and eventually send them to client */
    space  = HTTPSRV_SES_BUF_SIZE_PRV - session->buffer.offset;
    n_send = (space < length) ? space : length;
    memcpy(session->buffer.data + session->buffer.offset, src, n_send);
    session->buffer.offset += n_send;

    if (session->buffer.offset >= HTTPSRV_SES_BUF_SIZE_PRV)
    {
        if (httpsrv_ses_flush(session) == -1)
        {
            return (-1);
        }
        else
        {
            retval = n_send;
        }
    }

    return (retval);
}

/*
** Read data from HTTP server.
**
** First copy data from session buffer if there are any and then read rest from socket if required.
**
** IN:
**      HTTPSRV_SESSION_STRUCT *session - session to use for reading.
**      char                   *dst - user buffer to read to.
**      int32_t               len - size of user buffer.
**
** OUT:
**      none
**
** Return Value:
**      int - number of bytes read.
*/
int32_t httpsrv_read(HTTPSRV_SESSION_STRUCT *session, char *dst, int32_t len)
{
    int read           = 0;
    uint32_t data_size = session->buffer.offset;

    /* If there are any data in buffer copy them to user buffer */
    if (data_size > 0)
    {
        uint32_t length = (data_size < len) ? data_size : len;
        uint32_t tail   = HTTPSRV_SES_BUF_SIZE_PRV - length;

        memcpy(dst, session->buffer.data, length);
        memmove(session->buffer.data, session->buffer.data + length, tail);
        memset(session->buffer.data + tail, 0, length);
        session->buffer.offset -= length;
        read = length;
    }

    /* If there is some space remaining in user buffer try to read from socket */
    while (read < len)
    {
        int received;

        received = httpsrv_recv(session, dst + read, len - read, 0);
        if (received > 0) /* 0 means connection is closed.*/
        {
            read += received;
        }
        else
        {
            break;
        }
    }

    return (read);
}

/*
** Get string for ID from table.
**
** IN:
**      HTTPSRV_TABLE_ROW  *table - table to be searched
**      int32_t          id - search ID
**
** OUT:
**      none
**
** Return Value:
**      char* - pointer to result. NULL if not found.
*/
static char *httpsrv_get_table_str(HTTPSRV_TABLE_ROW *table, const int32_t id)
{
    HTTPSRV_TABLE_ROW *ptr = table;

    while ((ptr->str) && (id != ptr->id))
    {
        ptr++;
    }
    return (ptr->str);
}

/*
** Print data to session
**
** IN:
**      HTTPSRV_SESSION_STRUCT* session - session used for transmission
**      char*                   format - format for snprintf function
**      void                    ...    - parameters to print
**
** OUT:
**      none
**
** Return Value:
**      none
*/
static void httpsrv_print(HTTPSRV_SESSION_STRUCT *session, char *format, ...)
{
    va_list ap;
    char *buffer = session->buffer.data;
    int buffer_space;

#if !defined(__CC_ARM) /* Workarounfd for Keil vsnprintf()*/
    uint32_t req_space = 0;

    buffer_space = HTTPSRV_SES_BUF_SIZE_PRV - session->buffer.offset;

    va_start(ap, format);
    /* First we always test if there is enough space in buffer. If there is not,
    ** we flush it first and then write. */
    req_space = vsnprintf(buffer + session->buffer.offset, 0, format, ap);
    va_end(ap);

    if (req_space > buffer_space)
#endif
    {
        httpsrv_ses_flush(session);
        buffer_space = HTTPSRV_SES_BUF_SIZE_PRV;
    }
    va_start(ap, format);
    session->buffer.offset += vsnprintf(buffer + session->buffer.offset, buffer_space, format, ap);
    va_end(ap);
}

/*
** Send data from session buffer to client.
**
** IN:
**      HTTPSRV_SESSION_STRUCT *session - session to use.
**
** OUT:
**      none
**
** Return Value:
**      int - number of bytes send.
*/
int32_t httpsrv_ses_flush(HTTPSRV_SESSION_STRUCT *session)
{
    int32_t send_total;
    uint32_t data_length;
    char *data;

    send_total  = 0;
    data_length = session->buffer.offset;
    data        = session->buffer.data;

    while (send_total < data_length)
    {
        int send_now;

        send_now = httpsrv_send(session, data + send_total, data_length - send_total, 0);
        if (send_now == -1)
        {
            send_total = -1;
            break;
        }
        send_total += send_now;
    }
    session->buffer.offset = 0;

    return (send_total);
}

/*
** Read HTTP method
**
** IN:
**      HTTPSRV_SESSION_STRUCT* session - session structure pointer.
**      HTTPSRV_STRUCT *server - pointer to server structure (needed for session parameters).
**      char* buffer - pointer to beginning of line with request.
**
** OUT:
**      none
**
** Return Value:
**      none
*/
int32_t httpsrv_req_line(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session, char *buffer)
{
    char *uri_begin = NULL;
    char *uri_end   = NULL;
    uint32_t written;

    if (strncmp(buffer, "GET ", 4) == 0)
    {
        session->request.method = HTTPSRV_REQ_GET;
    }
    else if (strncmp(buffer, "POST ", 5) == 0)
    {
        session->request.method = HTTPSRV_REQ_POST;
    }
    else if (strncmp(buffer, "HEAD ", 5) == 0)
    {
        session->request.method = HTTPSRV_REQ_HEAD;
    }
    else /* Unknown method - not implemented response */
    {
        session->response.status_code = HTTPSRV_CODE_NOT_IMPLEMENTED;
        return (HTTPSRV_ERR);
    }

    /* Parse remaining part of line */
    uri_begin = strchr(buffer, ' ');

    if (uri_begin != NULL)
    {
        uri_begin++;
        uri_end = strchr(uri_begin, ' ');
        if (uri_end != NULL)
        {
            *uri_end = '\0';
        }
    }
    else
    {
        session->request.path[0]      = '\0';
        session->response.status_code = HTTPSRV_CODE_BAD_REQ;
        return (HTTPSRV_ERR);
    }

    /* Pre-process URI */
    httpsrv_url_decode(uri_begin);
    httpsrv_url_cleanup(uri_begin);

    written = snprintf(session->request.path, server->params.max_uri, "%s", uri_begin);
    /* Check if whole URI is saved in buffer */
    if (written > server->params.max_uri - 1)
    {
        session->request.path[0] = '\0';
        /* URI is too long so we set proper status code for response */
        session->response.status_code = HTTPSRV_CODE_URI_TOO_LONG;
        return (HTTPSRV_ERR);
    }
    return (HTTPSRV_OK);
}

/*
** Process one line of HTTP request header
**
** IN:
**      HTTPSRV_SESSION_STRUCT* session - session structure pointer.
**      HTTPSRV_STRUCT *server - pointer to server structure (needed for session parameters).
**      char* buffer - pointer to beginning of line with request.
**
** OUT:
**      none
**
** Return Value:
**      none
*/
int32_t httpsrv_req_hdr(HTTPSRV_SESSION_STRUCT *session, char *buffer)
{
    char *param_ptr = NULL;
    int32_t retval;

    retval = HTTPSRV_OK;

    if (strncmp(buffer, "Host: ", 6) == 0)
    {
        /*
         * We only do this to check request validity.
         * Virtual hosts are not supported.
         */
        /*
        param_ptr = buffer+6;
        strcpy(session->host, param_ptr);
        */
        session->flags |= HTTPSRV_FLAG_HAS_HOST;
    }
    else if (strncmp(buffer, "Connection: ", 12) == 0)
    {
        param_ptr = buffer + 12;

        if ((session->flags & HTTPSRV_FLAG_KEEP_ALIVE_ENABLED) &&
            ((strstr(param_ptr, "Keep-Alive")) || (strstr(param_ptr, "keep-alive"))))
        {
            session->flags |= HTTPSRV_FLAG_IS_KEEP_ALIVE;
        }
        else
        {
            session->flags &= ~HTTPSRV_FLAG_IS_KEEP_ALIVE;
        }
        if (strstr(param_ptr, "Upgrade") || strstr(param_ptr, "upgrade"))
        {
            session->flags |= HTTPSRV_FLAG_DO_UPGRADE;
        }
    }
    else if (strncmp(buffer, "Content-Length: ", 16) == 0)
    {
        unsigned long int value;

        param_ptr = buffer + 16;
        value     = strtoul(param_ptr, NULL, 10);
        if (value == ULONG_MAX)
        {
            retval = HTTPSRV_ERR;
            goto EXIT;
        }
        session->request.content_length = (uint32_t)value;
        session->flags |= HTTPSRV_FLAG_HAS_CONTENT_LENGTH;
    }
    else if (strncmp(buffer, "Content-Type: ", 14) == 0)
    {
        param_ptr = buffer + 14;

        session->request.content_type = httpsrv_get_table_int((HTTPSRV_TABLE_ROW *)content_type, param_ptr);
        /* If content type is unknown, treat it as octet/stream */
        if (session->request.content_type == 0)
        {
            session->request.content_type = HTTPSRV_CONTENT_TYPE_OCTETSTREAM;
        }
    }
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
    else if (strncmp(buffer, "Upgrade: ", 9) == 0)
    {
        param_ptr = buffer + 9;
        if (lwip_strnicmp(param_ptr, "websocket", 9) == 0)
        {
            session->request.upgrade_to = HTTPSRV_WS_PROTOCOL;
            session->ws_handshake       = httpsrv_ws_alloc(session);
            if (session->ws_handshake == NULL)
            {
                retval = HTTPSRV_ERR;
                goto EXIT;
            }
        }
    }
#endif
    else if (strncmp(buffer, "Authorization: ", 15) == 0)
    {
        param_ptr = buffer + 15;
        if (strncmp(param_ptr, "Basic ", 6) == 0)
        {
            char *user;
            char *pass;

            user = NULL;
            pass = NULL;
            if (httpsrv_basic_auth(param_ptr + 6, &user, &pass) == HTTPSRV_OK)
            {
                session->request.auth.user_id  = user;
                session->request.auth.password = pass;
            }
        }
    }
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
    else if (strncmp(buffer, "Sec-WebSocket-Key: ", 19) == 0)
    {
        param_ptr = buffer + 19;
        if (strlen(param_ptr) == WS_KEY_LENGTH)
        {
            session->ws_handshake = httpsrv_ws_alloc(session);
            if (session->ws_handshake == NULL)
            {
                retval = HTTPSRV_ERR;
                goto EXIT;
            }
            memcpy(session->ws_handshake->key, param_ptr, WS_KEY_LENGTH);
        }
    }
    else if (strncmp(buffer, "Sec-WebSocket-Protocol: ", 24) == 0)
    {
        char *substring;

        param_ptr             = buffer + 24;
        substring             = strtok(param_ptr, ", ;\t");
        session->ws_handshake = httpsrv_ws_alloc(session);
        if (session->ws_handshake == NULL)
        {
            retval = HTTPSRV_ERR;
            goto EXIT;
        }
        while (substring != NULL)
        {
            if (strcmp(substring, "chat") == 0)
            {
                session->ws_handshake->protocols |= WS_CHAT_PROTOCOL_MASK;
            }
            else if (strcmp(substring, "superchat") == 0)
            {
                session->ws_handshake->protocols |= WS_SUPERCHAT_PROTOCOL_MASK;
            }
            substring = strtok(NULL, ", ;");
        }
    }
    else if (strncmp(buffer, "Sec-WebSocket-Version: ", 23) == 0)
    {
        param_ptr             = buffer + 23;
        session->ws_handshake = httpsrv_ws_alloc(session);
        if (session->ws_handshake == NULL)
        {
            retval = HTTPSRV_ERR;
            goto EXIT;
        }
        session->ws_handshake->version = strtoul(param_ptr, NULL, 10);
    }
#endif
EXIT:
    return (retval);
}

/*
** Get realm for requested path
**
** IN:
**      char*            path - search path.
**      HTTPSRV_STRUCT*  server - pointer to server structure (needed for session parameters).
**
** OUT:
**      none
**
** Return Value:
**      HTTPSRV_AUTH_REALM_STRUCT* - authentication realm for requested path. Null if not found.
*/
const HTTPSRV_AUTH_REALM_STRUCT *httpsrv_req_realm(HTTPSRV_STRUCT *server, char *path)
{
    const HTTPSRV_AUTH_REALM_STRUCT *table;

    table = server->params.auth_table;

    if (table == NULL)
    {
        return (NULL);
    }

    while ((table->path != NULL) && (strstr(path, table->path) == NULL))
    {
        table++;
    }

    return (table->path ? table : NULL);
}

/* Get query string */
char *httpsrv_get_query(char *src)
{
    char *retval = NULL;

    retval = strchr(src, '?');
    if (NULL != retval)
    {
        *retval = '\0';
        retval++;
    }
    return (retval);
}

/*
** Function for checking of HTTP request validity
**
** IN:
**       HTTPSRV_SESSION_STRUCT *session - session to check
** OUT:
**      none
**
** Return Value:
**      uint32_t - Status code.
*/
uint32_t httpsrv_req_check(HTTPSRV_SESSION_STRUCT *session)
{
    char *backslash;
    HTTPSRV_REQ_STRUCT *request;

    request = &session->request;
    /* If method is not implemented return without request processing */
    if (request->method == HTTPSRV_REQ_UNKNOWN)
    {
        return (HTTPSRV_CODE_NOT_IMPLEMENTED);
    }
    /* We need content length for post requests */
    else if ((request->method == HTTPSRV_REQ_POST) && !(session->flags & HTTPSRV_FLAG_HAS_CONTENT_LENGTH))
    {
        return (HTTPSRV_CODE_NO_LENGTH);
    }
    /* Check request path and if there was host field in request header */
    backslash = strrchr(request->path, '/');
    if ((backslash == NULL) || !(session->flags & HTTPSRV_FLAG_HAS_HOST))
    {
        /* We have invalid request */
        return (HTTPSRV_CODE_BAD_REQ);
    }
    /* Check if upgrade request is valid */
    if (session->flags & HTTPSRV_FLAG_DO_UPGRADE)
    {
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
        /* Only WebSocket upgrade is supported */
        if ((request->upgrade_to != HTTPSRV_WS_PROTOCOL) || (session->ws_handshake->version > WS_PROTOCOL_VERSION) ||
            (strlen(session->ws_handshake->key) != WS_KEY_LENGTH))
        {
            return (HTTPSRV_CODE_BAD_REQ);
        }
        if (session->ws_handshake->version < WS_PROTOCOL_VERSION)
        {
            return (HTTPSRV_CODE_UPGRADE_REQUIRED);
        }
#else
        return (HTTPSRV_CODE_BAD_REQ);
#endif
    }
    return (HTTPSRV_CODE_OK);
}

/*
** Check user authentication credentials
**
** IN:
**      HTTPSRV_AUTH_REALM_STRUCT* realm - search realm.
**      HTTPSRV_AUTH_USER_STRUCT*  user - user to authenticate.
**
** OUT:
**      none
**
** Return Value:
**      int - 1 if user is successfully authenticated, zero otherwise.
*/
int httpsrv_check_auth(const HTTPSRV_AUTH_REALM_STRUCT *realm, const HTTPSRV_AUTH_USER_STRUCT *user)
{
    const HTTPSRV_AUTH_USER_STRUCT *users = NULL;

    if ((realm == NULL) || (user == NULL))
    {
        return (0);
    }

    users = realm->users;

    while (users->user_id != NULL)
    {
        if (!strcmp(users->user_id, user->user_id) && !strcmp(users->password, user->password))
        {
            return (1);
        }
        users++;
    }
    return (0);
}

/*
 * Decode URL encoded string.
 */
void httpsrv_url_decode(char *url)
{
    char *src = url;
    char *dst = url;

    while (*src != '\0')
    {
        if ((*src == '%') && (isxdigit((unsigned char)*(src + 1))) && (isxdigit((unsigned char)*(src + 2))))
        {
            *src       = *(src + 1);
            *(src + 1) = *(src + 2);
            *(src + 2) = '\0';
            *dst++     = strtol(src, NULL, 16);
            src += 3;
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

/*
 * URL cleanup (remove invalid path segments - /./ and /../)
 */
void httpsrv_url_cleanup(char *url)
{
    char *src = url;
    char *dst = url;

    while (*src != '\0')
    {
        if ((src[0] == '/') && (src[1] == '.'))
        {
            if (src[2] == '/')
            {
                src += 2;
            }
            else if ((src[2] == '.') && (src[3] == '/'))
            {
                src += 3;
            }
        }
        *dst++ = *src++;
    }
    *dst = '\0';
}

static int httpsrv_basic_auth(char *auth_string, char **user_ptr, char **pass_ptr)
{
    uint32_t decoded_length;
    uint32_t length;
    int retval;
    char *user;
    char *pass;

    pass           = NULL;
    user           = NULL;
    *user_ptr      = NULL;
    *pass_ptr      = NULL;
    retval         = HTTPSRV_OK;
    decoded_length = 0;

    length = strlen(auth_string);
    if ((length == 0) || (length % 4))
    {
        retval = HTTPSRV_ERR;
        goto EXIT;
    }
    if (!isbase64((const char *)auth_string))
    {
        retval = HTTPSRV_ERR;
        goto EXIT;
    }
    /* evaluate number of bytes required for worst case (no padding) */
    decoded_length = (length / 4) * 3 + 1;
    user           = httpsrv_mem_alloc_zero(sizeof(char) * decoded_length);
    if (user != NULL)
    {
        base64_decode(user, auth_string, decoded_length);
        *user_ptr = user;
    }
    else
    {
        retval = HTTPSRV_ERR;
        goto EXIT;
    }

    pass = strchr(user, ':');
    if (pass != NULL)
    {
        *pass     = '\0';
        pass      = pass + 1;
        *pass_ptr = pass;
    }
EXIT:
    return (retval);
}

/*
 * Join root directory and relative path.
 */
char *httpsrv_path_create(const char *root, char *filename)
{
    char *path;
    char *tmp;
    uint32_t root_length;
    uint32_t filename_length;
    uint32_t path_length;

    root_length     = strlen(root);
    filename_length = strlen(filename);
    /*
     * Length is worst case - +1 for terminating zero and +1 for potential
     * missing backslash
     */
    path_length = root_length + filename_length + 2;
    tmp         = filename;

    /* Correct path slashes */
    while (*tmp != '\0')
    {
        if (*tmp == '/')
        {
            *tmp = '\\';
        }
        tmp++;
    }

    path = (char *)httpsrv_mem_alloc_zero(path_length);
    if (path != NULL)
    {
        memcpy(path, root, root_length);
        if ((root[root_length - 1] != '\\') && (filename[0] != '\\'))
        {
            path[root_length] = '\\';
            root_length++;
        }
        memcpy(path + root_length, filename, filename_length);
    }
    return (path);
}

/*
** Get ID for string from table
**
** IN:
**      HTTPSRV_TABLE_ROW     *tbl - table to be searched
**      char*             str - search string
**      uint32_t           len - length of string
**
** OUT:
**      none
**
** Return Value:
**      ID corresponding to searched string. Zero if not found.
*/
static int httpsrv_get_table_int(HTTPSRV_TABLE_ROW *table, char *str)
{
    HTTPSRV_TABLE_ROW *ptr = table;

    while ((ptr->id) && (!strstr(str, ptr->str)))
    {
        ptr++;
    }
    return (ptr->id);
}

void *httpsrv_mem_alloc_zero(size_t xSize)
{
    void *result = httpsrv_mem_alloc(xSize);
    if (result)
    {
        memset(result, 0, xSize);
    }
    return result;
}
