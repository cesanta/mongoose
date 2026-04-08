// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "rtsp.h"

int main(void) {
  struct mg_mgr mgr;

  struct mg_rtsp_opts opts = {
      0,
      0,
      0,
      mg_rtsp_get_mp4_frame,
      (void *) "test.mp4",
      mg_rtsp_get_mp4_audio,
      (void *) "test.mp4",
      {0x12, 0x10},
  };

  mg_mgr_init(&mgr);
  mg_rtsp_listen(&mgr, "rtsp://0.0.0.0:8554/cam", &opts);

  while (true) {
    mg_mgr_poll(&mgr, 10);
  }
  mg_mgr_free(&mgr);
  return 0;
}
