// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
#pragma once

#include "mongoose.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(HTTP_URL)
#define HTTP_URL "http://0.0.0.0:8000"
#endif

#if !defined(HTTPS_URL)
#define HTTPS_URL "https://0.0.0.0:8443"
#endif

void web_init(struct mg_mgr *mgr);              // Initialise Web UI
bool web_load_settings(void *buf, size_t len);  // Load from disk/flash
bool web_save_settings(void *buf, size_t len);  // Save to disk/flash

#define SETTINGS_MAGIC 0xaabbccdd

#ifdef __cplusplus
}
#endif
