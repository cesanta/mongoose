// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Ethernet over radio, through a mesh. Mongoose sees an Ethernet interface,
// its driver splits Ethernet frames into radio frames. Two parts plug in:
// - struct mg_radio: a radio standard, e.g. mg_radio_802154
// - struct mg_mesh: how the mesh is built and routed, e.g. mg_mesh_star

#pragma once

#include "mongoose.h"

// A radio standard
struct mg_radio {
  const char *name;  // For logs
};

// A mesh strategy
struct mg_mesh {
  const char *name;  // For logs
};
