// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#include "mongoose.h"

#if !defined(HTTP_URL)
#define HTTP_URL "http://0.0.0.0:8000"
#endif

#if !defined(HTTPS_URL)
#define HTTPS_URL "http://0.0.0.0:8443"
#endif

void web_init(struct mg_mgr *mgr);
