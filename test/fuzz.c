#include "mongoose.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  struct http_message req;
  mg_parse_http((const char *) data, size, &req, 0);
  mg_parse_http((const char *) data, size, &req, 1);
  return 0;
}
