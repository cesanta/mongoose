// Copyright (c) 2023-2024 Cesanta Software Limited
// All rights reserved
#pragma once

#include "mongoose.h"

#ifdef __cplusplus
extern "C" {
#endif

#if MG_ENABLE_FATFS
#define HAL_FS (&mg_fs_fat)
#define HAL_ROOT_DIR "/"
#elif MG_ENABLE_PACKED_FS
#define HAL_FS (&mg_fs_packed)
#define HAL_ROOT_DIR "/"
#else
#define HAL_FS (&mg_fs_posix)
#define HAL_ROOT_DIR "./"
#endif

#define HAL_WEB_ROOT_DIR HAL_ROOT_DIR "web_root"

#if defined(HTTP_URL) && defined(HTTPS_URL)
// Allow to override via the build flags
#elif MG_ENABLE_TCPIP
#define HTTP_URL "http://0.0.0.0:80"     // Embedded build:
#define HTTPS_URL "https://0.0.0.0:443"  // Use standard privileged ports
#else
#define HTTP_URL "http://0.0.0.0:8000"    // Workstation build:
#define HTTPS_URL "https://0.0.0.0:8443"  // Use non-privileged ports
#endif

#define MAX_DEVICE_NAME 40
#define MAX_EVENTS_NO 400
#define MAX_EVENT_TEXT_SIZE 10
#define EVENTS_PER_PAGE 20

// Event log entry
struct ui_event {
  uint8_t type, prio;
  unsigned long timestamp;
  char text[10];
};

void net_init(struct mg_mgr *mgr);

#ifdef __cplusplus
}
#endif
