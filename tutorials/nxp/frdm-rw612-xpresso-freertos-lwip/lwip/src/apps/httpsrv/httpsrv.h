/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*   Header for HTTPSRV.*/

#ifndef _HTTPSRV_H_
#define _HTTPSRV_H_

#include "httpsrv_config.h"

#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "httpsrv_fs.h"
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
#include "httpsrv_ws.h"
#endif

/*
** HTTP server return codes
*/
#define HTTPSRV_OK           0
#define HTTPSRV_ERR          -1
#define HTTPSRV_BIND_FAIL    -2
#define HTTPSRV_LISTEN_FAIL  -3
#define HTTPSRV_CREATE_FAIL  -5
#define HTTPSRV_BAD_FAMILY   -6
#define HTTPSRV_SOCKOPT_FAIL -7
#define HTTPSRV_SES_INVALID  -8
#define HTTPSRV_FAIL         -9

/*
** HTTP server status codes
*/
#define HTTPSRV_CODE_CONTINUE              (100) /* Continue */
#define HTTPSRV_CODE_UPGRADE               (101) /* Switching Protocols */
#define HTTPSRV_CODE_OK                    (200) /* OK */
#define HTTPSRV_CODE_CREATED               (201) /* Created */
#define HTTPSRV_CODE_ACCEPTED              (202) /* Accepted */
#define HTTPSRV_CODE_NON_AUTHORITATIVE     (203) /* Non-Authoritative Information */
#define HTTPSRV_CODE_NO_CONTENT            (204) /* No Content */
#define HTTPSRV_CODE_RESET_CONTENT         (205) /* Reset Content */
#define HTTPSRV_CODE_PARTIAL_CONTENT       (206) /* Partial Content */
#define HTTPSRV_CODE_MULTIPLE_CHOICES      (300) /* Multiple Choices */
#define HTTPSRV_CODE_MOVED_PERMANENTLY     (301) /* Moved Permanently */
#define HTTPSRV_CODE_FOUND                 (302) /* Found */
#define HTTPSRV_CODE_SEE_OTHER             (303) /* See Other */
#define HTTPSRV_CODE_NOT_MODIFIED          (304) /* Not Modified */
#define HTTPSRV_CODE_USE_PROXY             (305) /* Use Proxy */
#define HTTPSRV_CODE_TEMPORARY_REDIRECT    (307) /* Temporary Redirect */
#define HTTPSRV_CODE_BAD_REQ               (400) /* Bad Request */
#define HTTPSRV_CODE_UNAUTHORIZED          (401) /* Unauthorized */
#define HTTPSRV_CODE_PAYMENT_REQUIRED      (402) /* Payment Required */
#define HTTPSRV_CODE_FORBIDDEN             (403) /* Forbidden */
#define HTTPSRV_CODE_NOT_FOUND             (404) /* Not Found */
#define HTTPSRV_CODE_METHOD_NOT_ALLOWED    (405) /* Method Not Allowed */
#define HTTPSRV_CODE_NOT_ACCEPTABLE        (406) /* Not Acceptable */
#define HTTPSRV_CODE_PROXY_AUTH_REQUIRED   (407) /* Proxy Authentication Required */
#define HTTPSRV_CODE_REQUEST_TIMEOUT       (408) /* Request Time-out */
#define HTTPSRV_CODE_CONFLICT              (409) /* Conflict */
#define HTTPSRV_CODE_GONE                  (410) /* Gone */
#define HTTPSRV_CODE_NO_LENGTH             (411) /* Length Required */
#define HTTPSRV_CODE_PRECONDITION_FAILED   (412) /* Precondition Failed */
#define HTTPSRV_CODE_ENTITY_TOO_LARGE      (413) /* Request Entity Too Large */
#define HTTPSRV_CODE_URI_TOO_LONG          (414) /* Request-URI Too Large */
#define HTTPSRV_CODE_UNSUPPORTED_MEDIA     (415) /* Unsupported Media Type */
#define HTTPSRV_CODE_RANGE_NOT_SATISFIABLE (416) /* Requested range not satisfiable */
#define HTTPSRV_CODE_EXPECTATION_FAILED    (417) /* Expectation Failed */
#define HTTPSRV_CODE_UPGRADE_REQUIRED      (426) /* Upgrade Required */
#define HTTPSRV_CODE_FIELD_TOO_LARGE       (431) /* Request Header Fields Too Large */
#define HTTPSRV_CODE_INTERNAL_ERROR        (500) /* Internal Server Error */
#define HTTPSRV_CODE_NOT_IMPLEMENTED       (501) /* Not Implemented */
#define HTTPSRV_CODE_BAD_GATEWAY           (502) /* Bad Gateway */
#define HTTPSRV_CODE_SERVICE_UNAVAILABLE   (503) /* Service Unavailable */
#define HTTPSRV_CODE_GATEWAY_TIMEOUT       (504) /* Gateway Time-out */
#define HTTPSRV_CODE_VERSION_NOT_SUPPORTED (505) /* HTTP Version not supported */

/*
** Authentication types
*/
typedef enum httpstv_auth_type
{
    HTTPSRV_AUTH_INVALID,
    HTTPSRV_AUTH_BASIC,
    HTTPSRV_AUTH_DIGEST /* Not supported yet! */
} HTTPSRV_AUTH_TYPE;

/*
 * HTTP request method type
 */
typedef enum httpsrv_req_method
{
    HTTPSRV_REQ_UNKNOWN,
    HTTPSRV_REQ_GET,
    HTTPSRV_REQ_POST,
    HTTPSRV_REQ_HEAD
} HTTPSRV_REQ_METHOD;

/*
 * HTTP content type
 */
typedef enum httpsrv_content_type
{
    HTTPSRV_CONTENT_TYPE_OCTETSTREAM = 1,
    HTTPSRV_CONTENT_TYPE_PLAIN,
    HTTPSRV_CONTENT_TYPE_HTML,
    HTTPSRV_CONTENT_TYPE_CSS,
    HTTPSRV_CONTENT_TYPE_GIF,
    HTTPSRV_CONTENT_TYPE_JPG,
    HTTPSRV_CONTENT_TYPE_PNG,
    HTTPSRV_CONTENT_TYPE_SVG,
    HTTPSRV_CONTENT_TYPE_JS,
    HTTPSRV_CONTENT_TYPE_ZIP,
    HTTPSRV_CONTENT_TYPE_XML,
    HTTPSRV_CONTENT_TYPE_PDF,
    HTTPSRV_CONTENT_TYPE_FORMURLENC,
    HTTPSRV_CONTENT_TYPE_FORMDATA,
} HTTPSRV_CONTENT_TYPE;

/*
** Authentication user structure
*/
typedef struct httpsrv_auth_user_struct
{
    char *user_id;  /* User ID - usually name*/
    char *password; /* Password */
} HTTPSRV_AUTH_USER_STRUCT;

/*
** Authentication realm structure
*/
typedef struct httpsrv_auth_realm_struct
{
    const char *name;                  /* Name of realm. Send to client so user know which login/pass should be used. */
    const char *path;                  /* Path to file/directory to protect. Relative to root directory */
    const HTTPSRV_AUTH_TYPE auth_type; /* Authentication type to use. */
    const HTTPSRV_AUTH_USER_STRUCT *users; /* Table of allowed users. */
} HTTPSRV_AUTH_REALM_STRUCT;

/*
** CGI request structure. Contains variables specified in RFC3875 (The Common Gateway Interface (CGI) Version 1.1).
** Structure is extended by session handle.
*/
typedef struct httpsrv_cgi_request_struct
{
    uint32_t ses_handle;               /* Session handle required for various read/write operations*/
                                       /*
                                        *  Following is subset of variables from RFC3875.
                                        ** Please see http://tools.ietf.org/html/rfc3875#section-4.1 for details
                                        */
    HTTPSRV_REQ_METHOD request_method; /* Request method (GET, POST, HEAD) see HTTPSRV_REQ_METHOD enum */
    HTTPSRV_CONTENT_TYPE content_type; /* Content type */
    uint32_t content_length;           /* Content length */
    uint32_t server_port;              /* Local connection port */
    char *remote_addr;                 /* Remote client address */
    char *server_name;                 /* Server hostname/IP */
    char *script_name;                 /* CGI name */
    char *server_protocol;             /* Server protocol name and version (HTTP/1.0) */
    char *server_software;             /* Server software identification string */
    char *query_string;                /* Request query string */
    char *gateway_interface;           /* Gateway interface type and version (CGI/1.1)*/
    char *remote_user;                 /* Remote user name  */
    HTTPSRV_AUTH_TYPE auth_type;       /* Auth type */
} HTTPSRV_CGI_REQ_STRUCT;

/*
** CGI response struct. This structure is filled by CGI function.
*/
typedef struct httpsrv_cgi_response_struct
{
    uint32_t ses_handle;               /* Session handle for reading/writing */
    HTTPSRV_CONTENT_TYPE content_type; /* Response content type */
    int32_t content_length;            /* Response content length */
    uint32_t status_code;              /* Status code (200, 404, etc.)*/
    char *data;                        /* Pointer to data to write */
    uint32_t data_length;              /* Length of data in bytes */
} HTTPSRV_CGI_RES_STRUCT;

/*
** Server side include parameter structure.
** Passed to user SSI function.
*/
typedef struct httpsrv_ssi_param_struct
{
    uint32_t ses_handle; /* Session handle for reading/writing */
    char *com_param;     /* Server side include command parameter (separated from command by ":") */
} HTTPSRV_SSI_PARAM_STRUCT;

/*
** Server side include callback prototype
*/
typedef int (*HTTPSRV_SSI_CALLBACK_FN)(HTTPSRV_SSI_PARAM_STRUCT *param);

/*
** SSI callback link structure
*/
typedef struct httpsrv_ssi_link_struct
{
    char *fn_name;                    /* Function name */
    HTTPSRV_SSI_CALLBACK_FN callback; /* Pointer to user function */
} HTTPSRV_SSI_LINK_STRUCT;

/*
** CGI callback prototype
*/
typedef int (*HTTPSRV_CGI_CALLBACK_FN)(HTTPSRV_CGI_REQ_STRUCT *param);

/*
** CGI callback link structure
*/
typedef struct httpsrv_cgi_link_struct
{
    char *fn_name;                    /* Function name */
    HTTPSRV_CGI_CALLBACK_FN callback; /* Pointer to user function */
} HTTPSRV_CGI_LINK_STRUCT;

#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
/*
** HTTP server TLS parameters
*/
typedef struct httpsrv_tls_param_struct
{
    const unsigned char *certificate_buffer;
    unsigned long certificate_buffer_size;
    const unsigned char *private_key_buffer;
    unsigned long private_key_buffer_size;
} HTTPSRV_TLS_PARAM_STRUCT;
#endif

/*
** HTTP server parameters
*/
typedef struct httpsrv_param_struct
{
    struct sockaddr_storage address;             /* Server address.*/
    const char *root_dir;                        /* root directory */
    const char *index_page;                      /* index page full path and name */
    uint32_t max_uri;                            /* maximal URI length */
    uint32_t max_ses;                            /* maximal sessions count */
    uint32_t task_prio;                          /* server task priority */
    const HTTPSRV_CGI_LINK_STRUCT *cgi_lnk_tbl;  /* cgi callback table. It is optional. */
    const HTTPSRV_SSI_LINK_STRUCT *ssi_lnk_tbl;  /* function callback table (dynamic web pages). It is optional. */
    const HTTPSRV_AUTH_REALM_STRUCT *auth_table; /* Table of authentication realms. It is optional. */
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
    const WS_PLUGIN_STRUCT *ws_tbl;              /* Table of WS plugins. It is optional. */
#endif
#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
    const HTTPSRV_TLS_PARAM_STRUCT *tls_param; /* TLS parameters. It is optional. */
#endif
} HTTPSRV_PARAM_STRUCT;

#ifdef __cplusplus
extern "C" {
#endif

/*
** Initialize and run HTTP server
** Returns server handle when successful, zero otherwise.
*/
uint32_t HTTPSRV_init(HTTPSRV_PARAM_STRUCT *params);

/*
** Stop and release HTTP server
*/
void HTTPSRV_release(uint32_t server_h);

uint32_t HTTPSRV_cgi_write(HTTPSRV_CGI_RES_STRUCT *response);
uint32_t HTTPSRV_cgi_read(uint32_t ses_handle, char *buffer, uint32_t length);
uint32_t HTTPSRV_ssi_write(uint32_t ses_handle, char *data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* _HTTPSRV_H_ */
