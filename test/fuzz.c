// https://llvm.org/docs/LibFuzzer.html

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

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_http_serve_opts opts = {.root_dir = "."};
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_serve_dir(c, (struct mg_http_message *) ev_data, &opts);
  }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  mg_log_set(MG_LL_INFO);

  struct mg_dns_message dm;
  mg_dns_parse(data, size, &dm);
  mg_dns_parse(NULL, 0, &dm);

  struct mg_http_message hm;
  if (mg_http_parse((const char *) data, size, &hm) > 0) {
    mg_crc32(0, hm.method.buf, hm.method.len);
    mg_crc32(0, hm.uri.buf, hm.uri.len);
    mg_crc32(0, hm.uri.buf, hm.uri.len);
    for (size_t i = 0; i < sizeof(hm.headers) / sizeof(hm.headers[0]); i++) {
      struct mg_str *k = &hm.headers[i].name, *v = &hm.headers[i].value;
      mg_crc32(0, k->buf, k->len);
      mg_crc32(0, v->buf, v->len);
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
    mg_crc32(0, mm.topic.buf, mm.topic.len);
    mg_crc32(0, mm.data.buf, mm.data.len);
    mg_crc32(0, mm.dgram.buf, mm.dgram.len);
  }
  mg_mqtt_parse(NULL, 0, 0, &mm);
  if (mg_mqtt_parse(data, size, 5, &mm) == MQTT_OK) {
    mg_crc32(0, mm.topic.buf, mm.topic.len);
    mg_crc32(0, mm.data.buf, mm.data.len);
    mg_crc32(0, mm.dgram.buf, mm.dgram.len);
  }
  mg_mqtt_parse(NULL, 0, 5, &mm);

  mg_sntp_parse(data, size);
  mg_sntp_parse(NULL, 0);

  char buf[size * 4 / 3 + 5];  // At least 4 chars and nul termination
  mg_base64_decode((char *) data, size, buf, sizeof(buf));
  mg_base64_decode(NULL, 0, buf, sizeof(buf));
  mg_base64_encode(data, size, buf, sizeof(buf));
  mg_base64_encode(NULL, 0, buf, sizeof(buf));

  mg_match(mg_str_n((char *) data, size), mg_str_n((char *) data, size), NULL);

  struct mg_str entry, s = mg_str_n((char *) data, size);
  while (mg_span(s, &entry, &s, ',')) entry.len = 0;

  int n;
  mg_json_get(mg_str_n((char *) data, size), "$", &n);
  mg_json_get(mg_str_n((char *) data, size), "$.a.b", &n);
  mg_json_get(mg_str_n((char *) data, size), "$[0]", &n);

  // Test built-in TCP/IP stack
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
      uint16_t eth_types[] = {0x800, 0x806, 0x86dd}; // IPv4, ARP, IPv6
      memcpy(eth->dst, mif.mac, 6);  // Set valid destination MAC
      // send all handled eth types, then 2 random ones
      if (i >= (sizeof(eth_types) / sizeof(eth_types[0]) + 2)) i = 0;
      if (i < (sizeof(eth_types) / sizeof(eth_types[0]))) eth->type = (eth_types[i++]);
      // build proper layer-3 datagrams, to be able to exercise layers above
      if (eth->type == mg_htons(0x800) && size > (sizeof(*eth) + sizeof(struct ip))) {             // IPv4
        static size_t j;
        uint8_t ip_protos[] = {1, 6, 17}; // ICMP, TCP, UDP
        struct ip *ip4 = (struct ip *) (eth + 1);
        ip4->ver = (ip4->ver & ~0xf0) | (4 << 4);
        // send all handled IP protos, then 2 random ones
        if (j >= (sizeof(ip_protos) / sizeof(ip_protos[0]) + 2)) j = 0;
        if (j < (sizeof(ip_protos) / sizeof(ip_protos[0]))) ip4->proto = (ip_protos[j++]);
        if (ip4->proto == 1) { // ICMP
        } else if (ip4->proto == 6) { // TCP
        } else if (ip4->proto == 17) { // UDP
          if (size > (sizeof(*eth) + sizeof(struct ip) + sizeof(struct udp))) {
            static size_t k;
            uint16_t udp_ports[] = {67, 68}; // DHCP server and client
            struct udp *udp = (struct udp *) (ip4 + 1);
            // send all handled UDP ports, then 2 random ones
            if (k >= (sizeof(udp_ports) / sizeof(udp_ports[0]) + 2)) k = 0;
            if (k < (sizeof(udp_ports) / sizeof(udp_ports[0]))) udp->dport = mg_htons(udp_ports[k++]);
          }
        }
      } else if (eth->type == mg_htons(0x806)) {      // ARP

      } else if (eth->type == mg_htons(0x86dd) && size > (sizeof(*eth) + sizeof(struct ip6))) {     // IPv6
        static size_t j;
        uint8_t ip6_protos[] = {6, 17, 58}; // TCP, UDP, ICMPv6
        struct ip6 *ip6 = (struct ip6 *) (eth + 1);
        ip6->ver = (ip6->ver & ~0xf0) | (6 << 4);
        // send all handled IPv6 "next headers", then 2 random ones
        if (j >= (sizeof(ip6_protos) / sizeof(ip6_protos[0]) + 2)) j = 0;
        if (j < (sizeof(ip6_protos) / sizeof(ip6_protos[0]))) ip6->next = (ip6_protos[j++]);
        if (ip6->next == 6) { // TCP
        } else if (ip6->next == 17) { // UDP
        } else if (ip6->next == 58) { // ICMPv6
          if (size >= (sizeof(*eth) + sizeof(struct ip6) + sizeof(struct icmp6))) {
            static size_t k;
            uint8_t icmp6_types[] = {128, 134, 135, 136}; // Echo Request, RA, NS, NA
            struct icmp6 *icmp6 = (struct icmp6 *) (ip6 + 1);
            // send all handled ICMPv6 types, then 2 random ones
            if (k >= (sizeof(icmp6_types) / sizeof(icmp6_types[0]) + 2)) k = 0;
            if (k < (sizeof(icmp6_types) / sizeof(icmp6_types[0]))) icmp6->type = icmp6_types[k++];
          }
        }
      }
    }

    mg_tcpip_rx(&mif, pkt, size);

    // Test HTTP serving (via our built-in TCP/IP stack)
    const char *url = "http://localhost:12345";
    struct mg_connection *c = mg_http_connect(&mgr, url, fn, NULL);
    mg_iobuf_add(&c->recv, 0, data, size);
    c->pfn(c, MG_EV_READ, NULL); // manually invoke protocol event handler

    free(pkt);
    mg_mgr_free(&mgr);
  }

  return 0;
}

#if defined(MAIN)
int main(int argc, char *argv[]) {
  int res = EXIT_FAILURE;
  if (argc > 1) {
    struct mg_str data = mg_file_read(&mg_fs_posix, argv[1]);
    if (data.buf != NULL) {
      LLVMFuzzerTestOneInput((uint8_t *) data.buf, data.len);
      res = EXIT_SUCCESS;
    }
    free(data.buf);
  }
  return res;
}
#endif
