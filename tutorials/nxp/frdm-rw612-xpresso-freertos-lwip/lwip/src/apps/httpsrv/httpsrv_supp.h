/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016,2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 *   HTTPSRV support functions header.
 */

#ifndef HTTP_SUPP_H_
#define HTTP_SUPP_H_

#include <stdbool.h>
#include <stdint.h>

#define ERR_PAGE_FORMAT "<HTML><HEAD><TITLE>%s</TITLE></HEAD>\n<BODY><H1>%s</H1>\n</BODY></HTML>\n"
#include "httpsrv_prv.h"
#include "httpsrv.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t httpsrv_read(HTTPSRV_SESSION_STRUCT *session, char *dst, int32_t len);
int32_t httpsrv_write(HTTPSRV_SESSION_STRUCT *session, char *src, int32_t len);
int32_t httpsrv_ses_flush(HTTPSRV_SESSION_STRUCT *session);

void httpsrv_sendhdr(HTTPSRV_SESSION_STRUCT *session, int32_t content_len, bool has_entity);
HTTPSRV_SES_STATE httpsrv_sendfile(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session);
void httpsrv_send_err_page(HTTPSRV_SESSION_STRUCT *session, const char *title, const char *text);

int32_t httpsrv_req_hdr(HTTPSRV_SESSION_STRUCT *session, char *buffer);
int32_t httpsrv_req_line(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session, char *buffer);
uint32_t httpsrv_req_check(HTTPSRV_SESSION_STRUCT *session);

const HTTPSRV_AUTH_REALM_STRUCT *httpsrv_req_realm(HTTPSRV_STRUCT *server, char *path);
int httpsrv_check_auth(const HTTPSRV_AUTH_REALM_STRUCT *realm, const HTTPSRV_AUTH_USER_STRUCT *user);
void httpsrv_destroy_server(HTTPSRV_STRUCT *server);
HTTPSRV_STRUCT *httpsrv_create_server(HTTPSRV_PARAM_STRUCT *params);
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
const WS_PLUGIN_STRUCT *httpsrv_get_ws_plugin(const WS_PLUGIN_STRUCT *table, char *resource);
#endif
int httpsrv_recv(HTTPSRV_SESSION_STRUCT *session, char *buffer, size_t length, int flags);
int httpsrv_send(HTTPSRV_SESSION_STRUCT *session, const char *buffer, size_t length, int flags);
char *httpsrv_get_query(char *src);
int httpsrv_wait_for_conn(HTTPSRV_STRUCT *server);
int httpsrv_accept(int sock);
void httpsrv_abort(int sock);

void *httpsrv_mem_alloc_zero(size_t xSize);
void httpsrv_url_decode(char *url);
void httpsrv_url_cleanup(char *url);
char *httpsrv_path_create(const char *root, char *filename);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_SUPP_H_ */
