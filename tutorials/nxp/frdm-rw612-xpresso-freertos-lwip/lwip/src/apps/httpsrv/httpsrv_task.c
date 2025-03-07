/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 *   HTTPSRV tasks and session processing.
 */

#include "httpsrv.h"
#include "httpsrv_prv.h"
#include "httpsrv_supp.h"
#include "httpsrv_script.h"
#include "lwip/def.h"

#define HTTPSRV_SESSION_TASK_NAME "HTTP server session"

#if (defined(HTTPSRV_CFG_WEBSOCKET_ENABLED))
#if (HTTPSRV_CFG_WEBSOCKET_ENABLED != 0)
static void httpsrv_plugin_run(void *server_ptr, void *session_ptr);
#endif
#endif

static int httpsrv_req_read(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session);
static HTTPSRV_SES_STATE httpsrv_req_do(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session);
static HTTPSRV_SES_STATE httpsrv_response(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session);

static inline void httpsrv_ses_set_state(HTTPSRV_SESSION_STRUCT *session, HTTPSRV_SES_STATE new_state);
static HTTPSRV_SESSION_STRUCT *httpsrv_ses_alloc(HTTPSRV_STRUCT *server, int sock);
static void httpsrv_ses_free(HTTPSRV_SESSION_STRUCT *session);
static void httpsrv_ses_close(HTTPSRV_SESSION_STRUCT *session);
static int httpsrv_ses_init(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session, const int sock);
static void httpsrv_session_task(void *arg);

/*
 ** HTTPSRV main task which creates new task for each new client request
 */
void httpsrv_server_task(void *arg)
{
    HTTPSRV_STRUCT *server = (HTTPSRV_STRUCT *)arg;

    while (1)
    {
        int i;
        int error;
        int new_sock;

        /* limit number of opened sessions */
        sys_arch_sem_wait(&server->ses_cnt, 0);

        /* Get socket with incoming connection (IPv4 or IPv6) */
        int connsock = httpsrv_wait_for_conn(server);
        if (server->valid == HTTPSRV_INVALID)
        {
            break;
        }
        if (connsock == -1)
        {
            taskYIELD();
        }
        else
        {
            new_sock = httpsrv_accept(server->sock);
            if (new_sock < 0)
            {
                sys_sem_signal(&server->ses_cnt);
                /* We probably run out of sockets. Wait some time then try again to prevent session tasks resource
                 * starvation */
                sys_msleep(100);
            }
            else
            {
#if ((defined(HTTPSRV_CFG_SEND_TIMEOUT) && (HTTPSRV_CFG_SEND_TIMEOUT != 0)) || \
     (defined(HTTPSRV_CFG_RECEIVE_TIMEOUT) && (HTTPSRV_CFG_RECEIVE_TIMEOUT != 0)))
                struct timeval timeval_option;
#endif

                /* Set socket options */
#if (defined(HTTPSRV_CFG_SEND_TIMEOUT) && (HTTPSRV_CFG_SEND_TIMEOUT != 0))
                timeval_option.tv_sec  = HTTPSRV_CFG_SEND_TIMEOUT / 1000;          /* seconds */
                timeval_option.tv_usec = (HTTPSRV_CFG_SEND_TIMEOUT % 1000) * 1000; /* and microseconds */
                error = lwip_setsockopt(new_sock, SOL_SOCKET, SO_SNDTIMEO, (const void *)&timeval_option,
                                        sizeof(timeval_option));
                if (error != 0)
                {
                    httpsrv_abort(new_sock);
                    sys_sem_signal(&server->ses_cnt);
                    break;
                }
#endif
#if (defined(HTTPSRV_CFG_RECEIVE_TIMEOUT) && (HTTPSRV_CFG_RECEIVE_TIMEOUT != 0))
                timeval_option.tv_sec  = HTTPSRV_CFG_RECEIVE_TIMEOUT / 1000;          /* seconds */
                timeval_option.tv_usec = (HTTPSRV_CFG_RECEIVE_TIMEOUT % 1000) * 1000; /* and microseconds */
                error = lwip_setsockopt(new_sock, SOL_SOCKET, SO_RCVTIMEO, (const void *)&timeval_option,
                                        sizeof(timeval_option));
                if (error != 0)
                {
                    httpsrv_abort(new_sock);
                    sys_sem_signal(&server->ses_cnt);
                    break;
                }
#endif
                /* Find empty session */
                for (i = 0; i < server->params.max_ses; i++)
                {
                    if (server->session[i] == NULL)
                    {
                        break;
                    }
                }

                if (i != server->params.max_ses)
                {
                    HTTPSRV_SESSION_STRUCT *session;

                    /* Allocate session */
                    session = httpsrv_ses_alloc(server, new_sock);
                    if (session)
                    {
                        HTTPSRV_SES_TASK_PARAM *ses_param;

                        /* Allocate session task parameter */
                        ses_param = httpsrv_mem_alloc_zero(sizeof(HTTPSRV_SES_TASK_PARAM));

                        if (ses_param != NULL)
                        {
                            if (ERR_OK == httpsrv_ses_init(server, session, new_sock))
                            {
                                /* Disable keep-alive for last session so we have at least one session free (not blocked
                                 * by keep-alive timeout) */
                                if (i == server->params.max_ses - 1)
                                {
                                    session->flags &= ~HTTPSRV_FLAG_KEEP_ALIVE_ENABLED;
                                }

                                server->session[i] = session;

                                ses_param->server    = server;
                                ses_param->session_p = &server->session[i];

                                /* Try to create task for session */
                                if (xTaskCreate(httpsrv_session_task, HTTPSRV_SESSION_TASK_NAME,
#if ((defined(HTTPSRV_CFG_WOLFSSL_ENABLE) && (HTTPSRV_CFG_WOLFSSL_ENABLE != 0)) || \
     (defined(HTTPSRV_CFG_MBEDTLS_ENABLE) && (HTTPSRV_CFG_MBEDTLS_ENABLE != 0)))
                                                (server->tls_ctx != NULL) ? HTTPSRV_CFG_HTTPS_SESSION_STACK_SIZE :
                                                                            HTTPSRV_CFG_HTTP_SESSION_STACK_SIZE,
#else
                                                HTTPSRV_CFG_HTTP_SESSION_STACK_SIZE,
#endif
                                                ses_param, server->params.task_prio, NULL) != pdPASS)
                                {
                                    httpsrv_ses_close(session);
                                    httpsrv_ses_free(session);
                                    server->session[i] = NULL;
                                    sys_sem_signal(&server->ses_cnt);
                                    httpsrv_mem_free(ses_param);
                                }
                            }
                            else
                            {
                                httpsrv_ses_close(session);
                                httpsrv_ses_free(session);
                                sys_sem_signal(&server->ses_cnt);
                                httpsrv_mem_free(ses_param);
                            }
                        }
                        else
                        {
                            httpsrv_ses_close(session);
                            httpsrv_ses_free(session);
                            httpsrv_abort(new_sock);
                            sys_sem_signal(&server->ses_cnt);
                        }
                    }
                    else
                    {
                        httpsrv_abort(new_sock);
                        sys_sem_signal(&server->ses_cnt);
                    }
                }
                else
                {
                    httpsrv_abort(new_sock);
                    sys_sem_signal(&server->ses_cnt);
                }
            }
        }
    }
    /* Server release.*/
    httpsrv_destroy_server(server);
    server->server_tid = 0;
    sys_sem_signal(&server->finished);

    vTaskDelete(NULL);
}

/*
 ** Session task.
 ** This task is responsible for session creation, processing and cleanup.
 */
static void httpsrv_session_task(void *arg)
{
    HTTPSRV_SES_TASK_PARAM *ses_param = (HTTPSRV_SES_TASK_PARAM *)arg;
    HTTPSRV_STRUCT *server            = ses_param->server;
    HTTPSRV_SESSION_STRUCT *session   = *ses_param->session_p;

    while (session->valid)
    {
        /* Run state machine for session */
        session->process_func(server, session);
        taskYIELD();
    }
    httpsrv_ses_close(session);
    httpsrv_ses_free(session);
    *ses_param->session_p = NULL;

    /* Cleanup and end task */
    httpsrv_mem_free(ses_param);
    sys_sem_signal(&server->ses_cnt);
    vTaskDelete(NULL);
}

/*
 ** Function for session allocation
 **
 ** IN:
 **      HTTPSRV_STRUCT *server - pointer to server structure (needed for session parameters).
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      HTTPSRV_SESSION_STRUCT* - pointer to allocated session. Non-zero if allocation was OK, NULL otherwise
 */
static HTTPSRV_SESSION_STRUCT *httpsrv_ses_alloc(HTTPSRV_STRUCT *server, int sock)
{
    HTTPSRV_SESSION_STRUCT *session = NULL;

    if (server)
    {
        session = httpsrv_mem_alloc_zero(sizeof(HTTPSRV_SESSION_STRUCT));
        if (session)
        {
            session->sock = -1;

            /* Alloc URI */
            session->request.path = httpsrv_mem_alloc_zero(server->params.max_uri + 1);
            if (NULL == session->request.path)
            {
                goto ERROR;
            }

            /* Alloc session buffer */
            session->buffer.data = httpsrv_mem_alloc_zero(sizeof(char) * HTTPSRV_SES_BUF_SIZE_PRV);
            if (NULL == session->buffer.data)
            {
                goto ERROR;
            }
#if ((defined(HTTPSRV_CFG_WOLFSSL_ENABLE) && (HTTPSRV_CFG_WOLFSSL_ENABLE != 0)) || \
     (defined(HTTPSRV_CFG_MBEDTLS_ENABLE) && (HTTPSRV_CFG_MBEDTLS_ENABLE != 0)))
            if (server->tls_ctx != 0)
            {
                session->tls_sock = httpsrv_tls_socket(server->tls_ctx, sock);
                if (session->tls_sock == 0)
                {
                    goto ERROR;
                }
            }
#endif
        }
    }

    return session;

ERROR:
    if (session->request.path)
    {
        httpsrv_mem_free(session->request.path);
        session->request.path = NULL;
    }
    if (session->buffer.data)
    {
        httpsrv_mem_free(session->buffer.data);
        session->buffer.data = NULL;
    }
    httpsrv_mem_free(session);
    return (NULL);
}

/*
 ** Function used to free session structure
 **
 ** IN:
 **      HTTPSRV_SESSION_STRUCT* session - session structure pointer
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      none
 */
static void httpsrv_ses_free(HTTPSRV_SESSION_STRUCT *session)
{
    if (session)
    {
        if (session->request.path)
        {
            httpsrv_mem_free(session->request.path);
        }
        if (session->request.auth.user_id)
        {
            httpsrv_mem_free(session->request.auth.user_id);
        }
        if (session->buffer.data)
        {
            httpsrv_mem_free(session->buffer.data);
        }
#if (defined(HTTPSRV_CFG_WEBSOCKET_ENABLED) && (HTTPSRV_CFG_WEBSOCKET_ENABLED != 0))
        if (session->ws_handshake)
        {
            httpsrv_mem_free(session->ws_handshake);
        }
#endif /* HTTPSRV_CFG_WEBSOCKET_ENABLED */
        httpsrv_mem_free(session);
    }
}

/*
 ** Function used to init session structure
 **
 ** IN:
 **      HTTPSRV_SESSION_STRUCT* session - session structure pointer
 **      HTTPSRV_STRUCT *server - pointer to server structure (needed for session parameters)
 **      const int sock - socket handle used for communication with client
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      error code - ERR_MEM or ERR_OK
 */
static int httpsrv_ses_init(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session, int sock)
{
    int error = ERR_MEM;
    if (server && session)
    {
        session->state   = HTTPSRV_SES_WAIT_REQ;
        session->sock    = sock;
        session->valid   = HTTPSRV_VALID;
        session->timeout = HTTPSRV_CFG_SES_TIMEOUT;
        session->flags |= HTTPSRV_FLAG_PROCESS_HEADER;
        if (HTTPSRV_CFG_KEEPALIVE_ENABLED)
        {
            session->flags |= HTTPSRV_FLAG_KEEP_ALIVE_ENABLED | HTTPSRV_FLAG_IS_KEEP_ALIVE;
        }
        session->time         = sys_now();
        session->process_func = httpsrv_http_process;
        error                 = sys_sem_new(&session->lock, 1);
    }
    return error;
}

/*
 ** Function used to close session
 **
 ** IN:
 **      HTTPSRV_SESSION_STRUCT* session - session structure pointer
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      none
 */
static void httpsrv_ses_close(HTTPSRV_SESSION_STRUCT *session)
{
    if (session != NULL)
    {
        if (session->response.file)
        {
            HTTPSRV_FS_close(session->response.file);
            session->response.file = NULL;
        }
#if ((defined(HTTPSRV_CFG_WOLFSSL_ENABLE) && (HTTPSRV_CFG_WOLFSSL_ENABLE != 0)) || \
     (defined(HTTPSRV_CFG_MBEDTLS_ENABLE) && (HTTPSRV_CFG_MBEDTLS_ENABLE != 0)))
        if (session->tls_sock != 0)
        {
            httpsrv_tls_shutdown(session->tls_sock);
            session->tls_sock = 0;
        }
#endif
        if (session->sock != -1)
        {
            lwip_shutdown(session->sock, SHUT_WR);
            lwip_close(session->sock);
            session->sock = -1;
        }
        if (session->lock != NULL)
        {
            sys_sem_free(&session->lock);
        }
    }
}

/*
 ** HTTP session state machine
 **
 ** IN:
 **      HTTPSRV_SESSION_STRUCT* session - session structure pointer.
 **      HTTPSRV_STRUCT *server - pointer to server structure (needed for session parameters).
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      none
 */
void httpsrv_http_process(void *server_ptr, void *session_ptr)
{
    uint32_t time_interval;
    uint32_t time_now;
    int result;
    HTTPSRV_STRUCT *server          = (HTTPSRV_STRUCT *)server_ptr;
    HTTPSRV_SESSION_STRUCT *session = (HTTPSRV_SESSION_STRUCT *)session_ptr;

    if (!session->valid)
    {
        session->state = HTTPSRV_SES_CLOSE;
        return;
    }

    /* check session timeout */
    time_now      = sys_now();
    time_interval = time_now - session->time;
    if (time_interval > session->timeout)
    {
        if (session->script_tid != 0)
        {
            vTaskDelete(session->script_tid);
            session->script_tid = 0;
        }
        session->state = HTTPSRV_SES_CLOSE;
    }

    switch (session->state)
    {
        case HTTPSRV_SES_WAIT_REQ:
            result = httpsrv_req_read(server, session);

            if (result == HTTPSRV_ERR)
            {
                httpsrv_ses_set_state(session, HTTPSRV_SES_CLOSE);
            }
            else if (result == HTTPSRV_FAIL)
            {
                httpsrv_ses_set_state(session, HTTPSRV_SES_RESP);
            }
            else if ((result == HTTPSRV_OK) && !(session->flags & HTTPSRV_FLAG_PROCESS_HEADER))
            {
                session->response.status_code = httpsrv_req_check(session);
                if (session->response.status_code != HTTPSRV_CODE_OK)
                {
                    httpsrv_ses_set_state(session, HTTPSRV_SES_RESP);
                }
                else
                {
                    session->state = HTTPSRV_SES_PROCESS_REQ;
                }
            }
            break;
        case HTTPSRV_SES_PROCESS_REQ:
            httpsrv_ses_set_state(session, httpsrv_req_do(server, session));
            break;

        case HTTPSRV_SES_RESP:
            httpsrv_ses_set_state(session, httpsrv_response(server, session));
            session->time = sys_now();
            break;

        case HTTPSRV_SES_END_REQ:
            if (!(session->flags & HTTPSRV_FLAG_IS_KEEP_ALIVE))
            {
                httpsrv_ses_set_state(session, HTTPSRV_SES_CLOSE);
            }
            else
            {
                /* Re-init session */
                httpsrv_ses_set_state(session, HTTPSRV_SES_WAIT_REQ);
                if (session->response.file)
                {
                    HTTPSRV_FS_close(session->response.file);
                    session->response.file = NULL;
                }
                memset(&session->response, 0, sizeof(session->response));
                if (session->request.auth.user_id != NULL)
                {
                    httpsrv_mem_free(session->request.auth.user_id);
                }
                session->request.auth.user_id  = NULL;
                session->request.auth.password = NULL;
                session->time                  = sys_now();
                session->timeout               = HTTPSRV_CFG_KEEPALIVE_TIMEOUT;
                session->flags                 = HTTPSRV_FLAG_IS_KEEP_ALIVE | HTTPSRV_FLAG_PROCESS_HEADER;
            }
            break;
        case HTTPSRV_SES_CLOSE:
        default:
            session->valid = HTTPSRV_INVALID;
            break;
    }
}

/*
 ** Function for request parsing
 **
 ** IN:
 **      HTTPSRV_SESSION_STRUCT* session - session structure pointer.
 **      HTTPSRV_STRUCT *server - pointer to server structure (needed for session parameters).
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      int - zero if request is valid, negative value if invalid.
 */
static int httpsrv_req_read(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session)
{
    char *line_start;
    int read;
    int retval;
    char *line_end;
    uint32_t unprocessed_size;

    line_start = session->buffer.data;
    line_end   = NULL;
    retval     = HTTPSRV_OK;

    /* Read data */
    read = httpsrv_recv(session, session->buffer.data + session->buffer.offset,
                        HTTPSRV_SES_BUF_SIZE_PRV - session->buffer.offset, 0);
    if (read < 1) /* 0 means connection is closed.*/
    {
        int optval;
        socklen_t optlen = sizeof(optval);

        if ((lwip_getsockopt(session->sock, SOL_SOCKET, SO_ERROR, &optval, &optlen) == 0) && (optval == EWOULDBLOCK))
        {
            retval = HTTPSRV_OK;
        }
        else
        {
            retval = HTTPSRV_ERR;
        }
        goto EXIT;
    }

    unprocessed_size = read;
    /* Process buffer line by line. End of line is \n or \r\n */
    while (1)
    {
        uint32_t max_length;

        max_length = (session->buffer.data + HTTPSRV_SES_BUF_SIZE_PRV) - line_start;
        line_end   = memchr(line_start, (int)'\n', max_length);
        if (line_end == NULL)
        {
            break;
        }

        /* Null terminate the line */
        *line_end = '\0';
        if ((line_end != session->buffer.data) && (*(line_end - 1) == '\r'))
        {
            *(line_end - 1) = '\0';
        }
        session->request.lines++;
        /* Subtract line length from size of unprocessed data */
        unprocessed_size -= (line_end - line_start + 1) - session->request.pending;
        session->request.pending = 0;

        /* Found an empty line => end of header */
        if (strlen(line_start) == 0)
        {
            session->flags &= ~HTTPSRV_FLAG_PROCESS_HEADER;
            session->request.lines = 0;
            break;
        }

        if (session->request.lines == 1)
        {
            if (httpsrv_req_line(server, session, line_start) != HTTPSRV_OK)
            {
                session->buffer.offset = 0;
                retval                 = HTTPSRV_FAIL;
                goto EXIT;
            }
        }
        else
        {
            if (httpsrv_req_hdr(session, line_start) != HTTPSRV_OK)
            {
                session->buffer.offset = 0;
                retval                 = HTTPSRV_FAIL;
                goto EXIT;
            }
        }
        /* Set start of next line after end of current line */
        line_start = line_end + 1;
        /* Check buffer boundary */
        if (line_start > (session->buffer.data + HTTPSRV_SES_BUF_SIZE_PRV))
        {
            line_start = session->buffer.data + HTTPSRV_SES_BUF_SIZE_PRV;
        }
    }

    session->request.pending += unprocessed_size;
    if (session->request.pending >= HTTPSRV_SES_BUF_SIZE_PRV)
    {
        session->response.status_code = HTTPSRV_CODE_FIELD_TOO_LARGE;
        session->buffer.offset        = 0;
        retval                        = HTTPSRV_FAIL;
        goto EXIT;
    }

    /* There were no valid lines in buffer */
    if ((unprocessed_size >= HTTPSRV_SES_BUF_SIZE_PRV) && (session->request.lines == 0))
    {
        session->buffer.offset = 0;
    }
    /* If there are some unprocessed data, move it at the beginning of buffer/set correct buffer offset. */
    else if (unprocessed_size != 0)
    {
        /* Copy rest of data to beginning of buffer and save offset for next reading. */
        if (line_end != NULL)
        {
            memmove(session->buffer.data, line_end + 1, unprocessed_size);
        }
        else
        {
            memmove(session->buffer.data, line_start, unprocessed_size);
        }
        session->buffer.offset = unprocessed_size;
    }
    /* Clear the buffer so we don't have some old data there. */
    memset(session->buffer.data + session->buffer.offset, 0, HTTPSRV_SES_BUF_SIZE_PRV - session->buffer.offset);
EXIT:
    return (retval);
}

/*
 ** Function for request processing
 **
 ** IN:
 **      HTTPSRV_SESSION_STRUCT* session - session structure pointer.
 **      HTTPSRV_STRUCT *server - pointer to server structure (needed for session parameters).
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      none
 */

static HTTPSRV_SES_STATE httpsrv_req_do(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session)
{
    char *suffix;
    const char *root_dir = NULL;
    char *full_path;
    HTTPSRV_SES_STATE retval;

    retval = HTTPSRV_SES_RESP;

    /* Check authentication */
    session->response.auth_realm = httpsrv_req_realm(server, session->request.path);
    if (session->response.auth_realm != NULL)
    {
        if (!httpsrv_check_auth(session->response.auth_realm, &session->request.auth))
        {
            session->response.status_code = HTTPSRV_CODE_UNAUTHORIZED;
            if (session->request.auth.user_id != NULL)
            {
                httpsrv_mem_free(session->request.auth.user_id);
                session->request.auth.user_id  = NULL;
                session->request.auth.password = NULL;
            }
            goto EXIT;
        }
    }

    root_dir = server->params.root_dir;

    /* Save query string */
    session->request.query = httpsrv_get_query(session->request.path);

    /* Check if requested resource is CGI script */
    if ((suffix = strrchr(session->request.path, '.')) != 0)
    {
        if (0 == lwip_stricmp(suffix, ".cgi"))
        {
            *suffix = '\0';
            httpsrv_process_cgi(server, session, session->request.path + 1); /* +1 because of slash */
            *suffix = '.';
            retval  = HTTPSRV_SES_END_REQ;
            goto EXIT;
        }
    }

    /* If request is POST on something else than CGI, report error to client. */
    if (session->request.method == HTTPSRV_REQ_POST)
    {
        session->response.status_code = HTTPSRV_CODE_METHOD_NOT_ALLOWED;
        goto EXIT;
    }

#if (defined(HTTPSRV_CFG_WEBSOCKET_ENABLED) && (HTTPSRV_CFG_WEBSOCKET_ENABLED != 0))
    /* Check if resource is plugin */
    session->plugin = httpsrv_get_ws_plugin(server->params.ws_tbl, session->request.path);
    if (session->plugin != NULL)
    {
        retval                        = HTTPSRV_SES_RESP;
        session->response.status_code = HTTPSRV_CODE_UPGRADE;
        goto EXIT;
    }
#endif

    /* If client requested root, set requested path to index page */
    if (session->request.path[0] == '/' && session->request.path[1] == '\0')
    {
        uint32_t offset;
        uint32_t length;
        const char *index   = server->params.index_page;
        uint32_t max_length = server->params.max_uri;

        length = strlen(index);
        if (length > max_length)
        {
            length = max_length;
        }

        offset = ((index[0] == '\\') || (index[0] == '/')) ? 1 : 0;
        memcpy(session->request.path + 1, index + offset, length);
    }

    /* Get full file path */
    full_path = httpsrv_path_create(root_dir, session->request.path);
    if (full_path == NULL)
    {
        session->response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
        goto EXIT;
    }

    session->response.file   = HTTPSRV_FS_open(full_path);
    session->response.length = 0;
    if (!session->response.file)
    {
        session->response.status_code = HTTPSRV_CODE_NOT_FOUND;
    }
    httpsrv_mem_free(full_path);

EXIT:
    return (retval);
}

/*
 ** Function for HTTP sending response, used only if request is not for CGI/SSI
 **
 ** IN:
 **      HTTPSRV_SESSION_STRUCT* session - session structure pointer.
 **      HTTPSRV_STRUCT *server - pointer to server structure (needed for session parameters).
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      none
 */

static HTTPSRV_SES_STATE httpsrv_response(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session)
{
    HTTPSRV_SES_STATE retval;

    retval = HTTPSRV_SES_END_REQ;

    switch (session->response.status_code)
    {
        case HTTPSRV_CODE_UPGRADE:
#if (defined(HTTPSRV_CFG_WEBSOCKET_ENABLED) && (HTTPSRV_CFG_WEBSOCKET_ENABLED != 0))
            if (session->request.upgrade_to == HTTPSRV_WS_PROTOCOL)
            {
                ws_handshake(session->ws_handshake);
                httpsrv_sendhdr(session, 0, 0);
            }
            session->process_func = httpsrv_plugin_run;
#endif
            break;
        case HTTPSRV_CODE_OK:
            if (session->request.method == HTTPSRV_REQ_HEAD)
            {
                httpsrv_sendhdr(session, HTTPSRV_FS_size(session->response.file), 0);
                retval = HTTPSRV_SES_END_REQ;
            }
            else if (session->request.method == HTTPSRV_REQ_GET)
            {
                retval = httpsrv_sendfile(server, session);
            }
            break;
        case HTTPSRV_CODE_UNAUTHORIZED:
            httpsrv_send_err_page(session, "Unauthorized", "Unauthorized!");
            break;
        case HTTPSRV_CODE_FORBIDDEN:
            httpsrv_send_err_page(session, "Forbidden", "Forbidden!");
            break;
        case HTTPSRV_CODE_URI_TOO_LONG:
            session->flags &= ~HTTPSRV_FLAG_IS_KEEP_ALIVE;
            httpsrv_send_err_page(session, "Uri too long", "Requested URI too long!");
            break;
        case HTTPSRV_CODE_NOT_FOUND:
            httpsrv_send_err_page(session, "Not Found", "Requested URL not found!");
            break;
        case HTTPSRV_CODE_METHOD_NOT_ALLOWED:
            httpsrv_send_err_page(session, "Method Not Allowed", "POST on static content is not allowed!");
            break;
        default:
            session->flags &= ~HTTPSRV_FLAG_IS_KEEP_ALIVE;
            httpsrv_sendhdr(session, 0, 0);
            break;
    }
    return (retval);
}

/*
 * Set new session state.
 */
static inline void httpsrv_ses_set_state(HTTPSRV_SESSION_STRUCT *session, HTTPSRV_SES_STATE new_state)
{
    session->state = new_state;
    if ((session->request.method != HTTPSRV_REQ_POST) ||
        ((session->response.status_code != HTTPSRV_CODE_OK) && (session->script_tid == 0)))
    {
        session->buffer.offset = 0;
    }
}

#if (defined(HTTPSRV_CFG_WEBSOCKET_ENABLED) && (HTTPSRV_CFG_WEBSOCKET_ENABLED != 0))
/*
 ** Run plugin - start plugin handler task send message to invoke plugin
 ** functions.
 ** IN:
 **      HTTPSRV_SESSION_STRUCT* session - session structure pointer
 **
 ** OUT:
 **      none
 **
 ** Return Value:
 **      none
 */

static void httpsrv_plugin_run(void *server_ptr, void *session_ptr)
{
    HTTPSRV_SESSION_STRUCT *session = (HTTPSRV_SESSION_STRUCT *)session_ptr;
    /* Run WebSocket session task. */
    ws_session_run(session);
    session->valid = HTTPSRV_INVALID;
}

#endif
