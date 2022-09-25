#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_LOG 0
#define MG_ENABLE_LINES 1
#define MG_ENABLE_MIP 1

#include "mongoose.c"

#include "driver_mock.c"

#ifdef __cplusplus
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *, size_t);
#else
int LLVMFuzzerTestOneInput(const uint8_t *, size_t);
#endif

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  mg_log_set(MG_LL_NONE);

  struct mg_dns_message dm;
  mg_dns_parse(data, size, &dm);
  mg_dns_parse(NULL, 0, &dm);

  struct mg_http_message hm;
  mg_http_parse((const char *) data, size, &hm);
  mg_http_parse(NULL, 0, &hm);

  struct mg_str body = mg_str_n((const char *) data, size);
  char tmp[256];
  mg_http_get_var(&body, "key", tmp, sizeof(tmp));
  mg_http_get_var(&body, "key", NULL, 0);
  mg_url_decode((char *) data, size, tmp, sizeof(tmp), 1);
  mg_url_decode((char *) data, size, tmp, 1, 1);
  mg_url_decode(NULL, 0, tmp, 1, 1);

  struct mg_mqtt_message mm;
  mg_mqtt_parse(data, size, 0, &mm);
  mg_mqtt_parse(NULL, 0, 0, &mm);
  mg_mqtt_parse(data, size, 5, &mm);
  mg_mqtt_parse(NULL, 0, 5, &mm);

  mg_sntp_parse(data, size);
  mg_sntp_parse(NULL, 0);

  char buf[size * 4 / 3 + 5];  // At least 4 chars and nul termination
  mg_base64_decode((char *) data, (int) size, buf);
  mg_base64_decode(NULL, 0, buf);
  mg_base64_encode(data, (int) size, buf);
  mg_base64_encode(NULL, 0, buf);

  mg_globmatch((char *) data, size, (char *) data, size);

  struct mg_str k, v, s = mg_str_n((char *) data, size);
  while (mg_commalist(&s, &k, &v)) k.len = v.len = 0;

  int n;
  mg_json_get(mg_str_n((char *) data, size), "$", &n);
  mg_json_get(mg_str_n((char *) data, size), "$.a.b", &n);
  mg_json_get(mg_str_n((char *) data, size), "$[0]", &n);

  if (size > 0) {
    struct mip_cfg cfg = {};
    size_t pktlen = 1540;
    char t[sizeof(struct mip_if) + pktlen * 2 + 0 /* qlen */];
    struct mip_if *ifp = (struct mip_if *) t;
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    if_init(ifp, &mgr, &cfg, &mip_driver_mock, NULL, pktlen, 0);

    // Make a copy of the random data, in order to modify it
    uint8_t pkt[size];
    struct eth *eth = (struct eth *) pkt;
    memcpy(pkt, data, size);
    if (size > sizeof(*eth)) {
      static uint8_t i;
      uint16_t types[] = {0x800, 0x800, 0x806, 0x86dd};
      memcpy(eth->dst, ifp->mac, 6);  // Set valid destination MAC
      eth->type = mg_htons(types[i++ & 3]);
    }

    mip_rx(ifp, (void *) pkt, size);
    mgr.priv = NULL;  // Don't let Mongoose free() ifp
    mg_mgr_free(&mgr);
  }

  return 0;
}
