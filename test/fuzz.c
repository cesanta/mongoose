#include "mongoose.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size < 5 ) {
    return 0;
  }

  /* get a null-terminated string to use */
  char *null_terminated = malloc(size+1);
  memcpy(null_terminated, data, size);
  null_terminated[size] = '\0';

  /* http parsing */
  struct mg_http_message req;
  memset(&req, 0x0, sizeof(struct mg_http_message));
  mg_http_parse((const char *) data, size, &req);

  /* some http parsing */
  struct mg_str body;
  body.ptr = null_terminated;
  body.len = size;
  char buf[256];
  mg_http_get_var(&body, "key", buf, sizeof(buf));

  /* url parse */
  mg_url_port(null_terminated);

  /* DNS parsing */
  struct mg_dns_message dns_msg;
  mg_dns_parse((const uint8_t *)null_terminated, size, &dns_msg);

  /* mqtt */
  struct mg_mqtt_message m;
  mg_mqtt_parse(data, size, &m);

  /* sntp */
  struct timeval tv;
  mg_sntp_parse(data, size, &tv);

  /* base64 */
  char *b64_encoded = (char*) malloc(size*2);
  mg_base64_encode((const unsigned char *)data, size, b64_encoded);
  mg_base64_decode((char *)data, size, b64_encoded);
  free(b64_encoded);

  /* cleanup */
  free(null_terminated);

  return 0;
}
