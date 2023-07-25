// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
#pragma once

#include "mongoose.h"

#if !defined(HTTP_URL)
#define HTTP_URL "http://0.0.0.0:8000"
#endif

#if !defined(HTTPS_URL)
#define HTTPS_URL "https://0.0.0.0:8443"
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

void web_init(struct mg_mgr *mgr);
