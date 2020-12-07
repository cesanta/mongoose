#include "mongoose.h"


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  struct mg_dns_message dm;
  mg_dns_parse(data, size, &dm);

  struct mg_http_message hm;
  mg_http_parse((const char *) data, size, &hm);

  struct mg_str body = mg_str_n((const char *) data, size);
  char tmp[256];
  mg_http_get_var(&body, "key", tmp, sizeof(tmp));

  struct mg_mqtt_message mm;
  mg_mqtt_parse(data, size, &mm);

  struct timeval tv;
  mg_sntp_parse(data, size, &tv);

  char buf[size * 4 / 3 + 5];  // At least 4 chars and nul termination
  mg_base64_decode((char *) data, size, buf);
  mg_base64_encode(data, size, buf);

  return 0;
}
