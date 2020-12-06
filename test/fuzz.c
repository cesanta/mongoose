#include "mongoose.h"


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  struct mg_dns_message dm;
  mg_dns_parse(data, size, &dm);

  struct mg_http_message hm;
  mg_http_parse((const char *) data, size, &hm);

  struct mg_mqtt_message mm;
  mg_mqtt_parse(data, size, &mm);

  struct timeval tv;
  mg_sntp_parse(data, size, &tv);

  return 0;
}
