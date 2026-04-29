// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_dash_init(&mgr);

  for (;;) {
    mg_mgr_poll(&mgr, 10);
    mg_dash_poll(&mgr);
  }

  return 0;
}
