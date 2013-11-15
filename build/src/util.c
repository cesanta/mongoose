#include "internal.h"

// Return fake connection structure. Used for logging, if connection
// is not applicable at the moment of logging.
static struct mg_connection *fc(struct mg_context *ctx) {
  static struct mg_connection fake_connection;
  fake_connection.ctx = ctx;
  // See https://github.com/cesanta/mongoose/issues/236
  fake_connection.event.user_data = ctx->user_data;
  return &fake_connection;
}
