#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_LOG 0
#define MG_ENABLE_LINES 1
#define MG_ENABLE_TCPIP 1
#define MG_IO_SIZE (32 * 1024 * 1024)  // Big IO size for fast resizes

#include "mongoose.c"

#include "driver_mock.c"

#ifdef __cplusplus
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *, size_t);
#else
int LLVMFuzzerTestOneInput(const uint8_t *, size_t);
#endif

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_http_serve_opts opts = {.root_dir = "."};
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_serve_dir(c, (struct mg_http_message *) ev_data, &opts);
  }
  (void) fn_data;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  mg_log_set(MG_LL_NONE);

  struct mg_dns_message dm;
  mg_dns_parse(data, size, &dm);
  mg_dns_parse(NULL, 0, &dm);

  struct mg_http_message hm;
  if (mg_http_parse((const char *) data, size, &hm) > 0) {
    mg_crc32(0, hm.method.ptr, hm.method.len);
    mg_crc32(0, hm.uri.ptr, hm.uri.len);
    mg_crc32(0, hm.uri.ptr, hm.uri.len);
    for (size_t i = 0; i < sizeof(hm.headers) / sizeof(hm.headers[0]); i++) {
      struct mg_str *k = &hm.headers[i].name, *v = &hm.headers[i].value;
      mg_crc32(0, k->ptr, k->len);
      mg_crc32(0, v->ptr, v->len);
    }
  }
  mg_http_parse(NULL, 0, &hm);

  struct mg_str body = mg_str_n((const char *) data, size);
  char tmp[256];
  mg_http_get_var(&body, "key", tmp, sizeof(tmp));
  mg_http_get_var(&body, "key", NULL, 0);
  mg_url_decode((char *) data, size, tmp, sizeof(tmp), 1);
  mg_url_decode((char *) data, size, tmp, 1, 1);
  mg_url_decode(NULL, 0, tmp, 1, 1);

  struct mg_mqtt_message mm;
  if (mg_mqtt_parse(data, size, 0, &mm) == MQTT_OK) {
    mg_crc32(0, mm.topic.ptr, mm.topic.len);
    mg_crc32(0, mm.data.ptr, mm.data.len);
    mg_crc32(0, mm.dgram.ptr, mm.dgram.len);
  }
  mg_mqtt_parse(NULL, 0, 0, &mm);
  if (mg_mqtt_parse(data, size, 5, &mm) == MQTT_OK) {
    mg_crc32(0, mm.topic.ptr, mm.topic.len);
    mg_crc32(0, mm.data.ptr, mm.data.len);
    mg_crc32(0, mm.dgram.ptr, mm.dgram.len);
  }
  mg_mqtt_parse(NULL, 0, 5, &mm);

  mg_sntp_parse(data, size);
  mg_sntp_parse(NULL, 0);

  char buf[size * 4 / 3 + 5];  // At least 4 chars and nul termination
  mg_base64_decode((char *) data, size, buf, sizeof(buf));
  mg_base64_decode(NULL, 0, buf, sizeof(buf));
  mg_base64_encode(data, size, buf, sizeof(buf));
  mg_base64_encode(NULL, 0, buf, sizeof(buf));

  mg_globmatch((char *) data, size, (char *) data, size);

  struct mg_str k, v, s = mg_str_n((char *) data, size);
  while (mg_commalist(&s, &k, &v)) k.len = v.len = 0;

  int n;
  mg_json_get(mg_str_n((char *) data, size), "$", &n);
  mg_json_get(mg_str_n((char *) data, size), "$.a.b", &n);
  mg_json_get(mg_str_n((char *) data, size), "$[0]", &n);

  if (size > 0) {
    struct mg_tcpip_if mif = {.ip = 0x01020304,
                              .mask = 255,
                              .gw = 0x01010101,
                              .driver = &mg_tcpip_driver_mock};
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_tcpip_init(&mgr, &mif);

    // Make a copy of the random data, in order to modify it
    void *pkt = malloc(size);
    struct eth *eth = (struct eth *) pkt;
    memcpy(pkt, data, size);
    if (size > sizeof(*eth)) {
      static size_t i;
      uint16_t eth_types[] = {0x800, 0x800, 0x806, 0x86dd};
      memcpy(eth->dst, mif.mac, 6);  // Set valid destination MAC
      eth->type = mg_htons(eth_types[i++]);
      if (i >= sizeof(eth_types) / sizeof(eth_types[0])) i = 0;
    }

    mg_tcpip_rx(&mif, pkt, size);

    // Test HTTP serving
    const char *url = "http://localhost:12345";
    struct mg_connection *c = mg_http_connect(&mgr, url, fn, NULL);
    mg_iobuf_add(&c->recv, 0, data, size);
    c->pfn(c, MG_EV_READ, NULL, NULL);

    mg_mgr_free(&mgr);
    free(pkt);
    mg_tcpip_free(&mif);
  }

  return 0;
}

#if defined(MAIN)
int main(int argc, char *argv[]) {
  int res = EXIT_FAILURE;
  if (argc > 1) {
    size_t len = 0;
    char *buf = mg_file_read(&mg_fs_posix, argv[1], &len);
    if (buf != NULL) {
      LLVMFuzzerTestOneInput((uint8_t *) buf, len);
      res = EXIT_SUCCESS;
    }
    free(buf);
  }
  return res;
}
#endif
