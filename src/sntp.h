#pragma once

#include "net.h"

struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fn_data);
void mg_sntp_send(struct mg_connection *c, unsigned long utc);
int mg_sntp_parse(const unsigned char *buf, size_t len, struct timeval *tv);
