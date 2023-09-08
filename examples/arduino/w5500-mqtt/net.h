#pragma once
#include "mongoose.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct mg_tcpip_spi spi;
extern struct mg_mgr mgr;
void mqtt_publish(const char *message);
void net_init(void);
void exec_command(const char *req, size_t req_len);

#ifdef __cplusplus
}
#endif
