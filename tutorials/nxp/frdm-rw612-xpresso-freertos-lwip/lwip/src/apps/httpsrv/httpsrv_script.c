/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2019, 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Comments:
 *
 *   HTTPSRV script functions.
 */

#include "httpsrv.h"
#include "httpsrv_prv.h"
#include "httpsrv_supp.h"
#include "httpsrv_script.h"

/*
** Function for searching callback for name in function table (SSI/CGI)
**
** IN:
**      HTTPSRV_FN_LINK_STRUCT* table - table to search in.
**
**       char* name - name to search.
** OUT:
**      stack_size - pointer to variable to store callback stack size to.
**
** Return Value:
**      HTTPSRV_FN_CALLBACK - function callback if successfull, NULL if not found
*/
HTTPSRV_FN_CALLBACK httpsrv_find_callback(HTTPSRV_FN_LINK_STRUCT *table, char *name /*, uint32_t* stack_size*/)
{
    HTTPSRV_FN_CALLBACK retval = NULL;

    if (table && name)
    {
        while ((table->callback != NULL) && (*(table->callback) != NULL))
        {
            if (0 == strcmp(name, table->fn_name))
            {
                retval = (HTTPSRV_FN_CALLBACK)table->callback;
                break;
            }
            table++;
        }
    }

    return (retval);
}

/*
** Function for CGI calling
**
** IN:
**      HTTPSRV_CGI_CALLBACK_FN function - pointer to user function to be called as CGI
**
**      HTTPSRV_SCRIPT_MSG* msg_ptr - pointer to message containing data required for CGI parameter
** OUT:
**      none
**
** Return Value:
**      none
*/
#define HTTPSRV_ADDR_STR_SIZE sizeof("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255")
void httpsrv_call_cgi(HTTPSRV_CGI_CALLBACK_FN function,
                      HTTPSRV_SESSION_STRUCT *session /* Session requesting script */,
                      char *name /* Function name */)
{
    HTTPSRV_CGI_REQ_STRUCT cgi_param;
    char server_ip[HTTPSRV_ADDR_STR_SIZE];
    char remote_ip[HTTPSRV_ADDR_STR_SIZE];
    struct sockaddr_storage l_address;
    struct sockaddr_storage r_address;
    socklen_t length = sizeof(struct sockaddr);

    /* Fill callback parameter */
    cgi_param.ses_handle     = (uint32_t)session;
    cgi_param.request_method = session->request.method;
    cgi_param.content_type   = (HTTPSRV_CONTENT_TYPE)session->request.content_type;
    cgi_param.content_length = session->request.content_length;

    getsockname(session->sock, (struct sockaddr *)&l_address, &length);
    getpeername(session->sock, (struct sockaddr *)&r_address, &length);

#if LWIP_IPV6
    if (l_address.ss_family == AF_INET6)
    {
        inet_ntop(l_address.ss_family, ((struct sockaddr_in6 *)&l_address)->sin6_addr.s6_addr, server_ip,
                  sizeof(server_ip));
        inet_ntop(r_address.ss_family, ((struct sockaddr_in6 *)&r_address)->sin6_addr.s6_addr, remote_ip,
                  sizeof(remote_ip));
        cgi_param.server_port = ((struct sockaddr_in6 *)&l_address)->sin6_port;
    }
    else
#endif
#if LWIP_IPV4
        if (l_address.ss_family == AF_INET)
    {
        inet_ntop(l_address.ss_family, &((struct sockaddr_in *)&l_address)->sin_addr.s_addr, server_ip,
                  sizeof(server_ip));
        inet_ntop(r_address.ss_family, &((struct sockaddr_in *)&r_address)->sin_addr.s_addr, remote_ip,
                  sizeof(remote_ip));
        cgi_param.server_port = ((struct sockaddr_in *)&l_address)->sin_port;
    }
    else
#endif
    {
    }

    cgi_param.auth_type         = HTTPSRV_AUTH_BASIC;
    cgi_param.remote_user       = session->request.auth.user_id;
    cgi_param.remote_addr       = remote_ip;
    cgi_param.server_name       = server_ip;
    cgi_param.script_name       = name;
    cgi_param.server_protocol   = HTTPSRV_PROTOCOL_STRING;
    cgi_param.server_software   = HTTPSRV_PRODUCT_STRING;
    cgi_param.query_string      = session->request.query;
    cgi_param.gateway_interface = HTTPSRV_CGI_VERSION_STRING;

    /* Call the function */
    function(&cgi_param);
}

/*
** Function for SSI calling
**
** IN:
**      HTTPSRV_SSI_CALLBACK_FN function - pointer to user function to be called as SSI
**
**      HTTPSRV_SCRIPT_MSG* msg_ptr - pointer to message containing data required for SSI parameter
** OUT:
**      none
**
** Return Value:
**      none
*/
void httpsrv_call_ssi(HTTPSRV_SSI_CALLBACK_FN function,
                      HTTPSRV_SESSION_STRUCT *session /* Session requesting script */,
                      char *name /* Function name */)
{
    char *tmp;
    HTTPSRV_SSI_PARAM_STRUCT ssi_param;

    ssi_param.ses_handle = (uint32_t)session;
    tmp                  = strchr(name, ':');
    if (tmp != NULL)
    {
        *tmp++ = '\0';
    }
    else
    {
        tmp = "";
    }
    ssi_param.com_param = tmp;

    function(&ssi_param);
}

/*
** Task for CGI/SSI handling.
*/
void httpsrv_script_handler(HTTPSRV_STRUCT *server,
                            HTTPSRV_SESSION_STRUCT *session /* Session requesting script */,
                            HTTPSRV_CALLBACK_TYPE type /* Type of callback */,
                            char *name /* Function name */)
{
    /* Read messages */
    if (server && name && session)
    {
        HTTPSRV_FN_CALLBACK user_function;
        HTTPSRV_FN_LINK_STRUCT *table;
        char *separator;

        user_function = NULL;
        /*
         * There are two options:
         * 1. User set stack size to 0 and script callback will be run from this task.
         * 2. User set stack size > 0 and script callback will be run in separate task.
         */
        switch (type)
        {
            case HTTPSRV_CGI_CALLBACK:
                table         = (HTTPSRV_FN_LINK_STRUCT *)server->params.cgi_lnk_tbl;
                user_function = httpsrv_find_callback(table, name);

                /* Option No.1a - Run User CGI function here. */
                if (user_function)
                {
                    httpsrv_call_cgi((HTTPSRV_CGI_CALLBACK_FN)user_function, session, name);
                    httpsrv_ses_flush(session);
                }
                break;

            case HTTPSRV_SSI_CALLBACK:
                table = (HTTPSRV_FN_LINK_STRUCT *)server->params.ssi_lnk_tbl;

                /* Set separator to null character temporarily. */
                separator = strchr(name, ':');
                if (separator != NULL)
                {
                    *separator = '\0';
                }

                user_function = httpsrv_find_callback(table, name);

                if (separator != NULL)
                {
                    *separator = ':';
                }

                /* Option No.1b - Run User SSI function here. */
                if (user_function)
                {
                    httpsrv_call_ssi((HTTPSRV_SSI_CALLBACK_FN)user_function, session, name);
                    httpsrv_ses_flush(session);
                }
                break;

            default:
                break;
        }
    }
}

/*
** Function for CGI request processing
**
** IN:
**      HTTPSRV_SESSION_STRUCT* session - session structure pointer.
**      HTTPSRV_STRUCT*         server - pointer to server structure (needed for session parameters).
**      char*                   cgi_name - name of cgi function.
**
** OUT:
**      none
**
** Return Value:
**      none
*/
void httpsrv_process_cgi(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session, char *cgi_name)
{
    httpsrv_script_handler(server, session, HTTPSRV_CGI_CALLBACK, cgi_name);

    /*
    ** There is some unread content from client after CGI finished.
    ** It must be read and discarded if we have keep-alive enabled
    ** so it does not affect next request.
    */
    if (session->request.content_length)
    {
        char tmp[HTTPSRV_TMP_BUFFER_SIZE];
        int32_t length = session->request.content_length;

        while (length > 0)
        {
            int32_t retval;
            int32_t chunk;

            chunk = sizeof(tmp);
            if (length < chunk)
            {
                chunk = length;
            }
            retval = httpsrv_read(session, tmp, chunk);
            if (retval <= 0)
            {
                break;
            }
            length -= retval;
        }

        session->request.content_length = length;
    }
    return;
}
