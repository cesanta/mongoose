// Cloud connector API to https://mdash.net - device management cloud

#pragma once

#include "net.h"

#ifndef MG_MDASH_URL
#define MG_MDASH_URL "wss://mdash.net/api/v2/rpc"
#endif

#ifndef MG_MDASH_KEY
#define MG_MDASH_KEY ""
#endif

#ifndef MG_MDASH_FIRMWARE_VERSION
#define MG_MDASH_FIRMWARE_VERSION "1.0.0"
#endif

#ifndef MG_ENABLE_MDASH
#define MG_ENABLE_MDASH 0
#endif

void mg_mdash_init(struct mg_mgr *);
void mg_mdash_poll(struct mg_mgr *);
