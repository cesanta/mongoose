/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef HTTPSRV_SCRIPT_H_
#define HTTPSRV_SCRIPT_H_

#include "httpsrv_prv.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t httpsrv_wait_for_cgi(HTTPSRV_SESSION_STRUCT *session);
void httpsrv_detach_script(HTTPSRV_DET_TASK_PARAM *task_params);
HTTPSRV_FN_CALLBACK httpsrv_find_callback(HTTPSRV_FN_LINK_STRUCT *table, char *name);
void httpsrv_call_cgi(HTTPSRV_CGI_CALLBACK_FN function, HTTPSRV_SESSION_STRUCT *session, char *name);
void httpsrv_call_ssi(HTTPSRV_SSI_CALLBACK_FN function, HTTPSRV_SESSION_STRUCT *session, char *name);
void httpsrv_process_cgi(HTTPSRV_STRUCT *server, HTTPSRV_SESSION_STRUCT *session, char *cgi_name);
void httpsrv_script_handler(HTTPSRV_STRUCT *server,
                            HTTPSRV_SESSION_STRUCT *session,
                            HTTPSRV_CALLBACK_TYPE type,
                            char *name);

#ifdef __cplusplus
}
#endif

#endif
