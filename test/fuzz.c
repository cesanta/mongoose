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

// deduplicate layer-3 mocking among layer-2s
struct fuzz_l3_state {
  size_t ip_proto, udp_port;
  size_t ip6_next, icmp6_type;
};

static void fuzz_ip4(void *buf, size_t len, struct fuzz_l3_state *st) {
  if (len >= sizeof(struct ip)) {
    uint8_t ip_protos[] = {1, 6, 17}; // ICMP, TCP, UDP
    struct ip *ip4 = (struct ip *) buf;
    ip4->ver = (uint8_t) ((ip4->ver & ~0xf0) | (4 << 4));
    // send all handled IP protos, then 2 random ones
    if (st->ip_proto >= (sizeof(ip_protos) / sizeof(ip_protos[0]) + 2)) st->ip_proto = 0;
    if (st->ip_proto < (sizeof(ip_protos) / sizeof(ip_protos[0]))) ip4->proto = ip_protos[st->ip_proto++];
    if (ip4->proto == 1) { // ICMP
    } else if (ip4->proto == 6) { // TCP
    } else if (ip4->proto == 17 && len >= sizeof(struct ip) + sizeof(struct udp)) { // UDP
      uint16_t udp_ports[] = {67, 68}; // DHCP server and client
      struct udp *udp = (struct udp *) (ip4 + 1);
      // send all handled UDP ports, then 2 random ones
      if (st->udp_port >= (sizeof(udp_ports) / sizeof(udp_ports[0]) + 2)) st->udp_port = 0;
      if (st->udp_port < (sizeof(udp_ports) / sizeof(udp_ports[0]))) udp->dport = mg_htons(udp_ports[st->udp_port++]);
    }
  }
}

static void fuzz_ip6(void *buf, size_t len, struct fuzz_l3_state *st) {
  if (len >= sizeof(struct ip6)) {
    uint8_t ip6_protos[] = {6, 17, 58}; // TCP, UDP, ICMPv6
    struct ip6 *ip6 = (struct ip6 *) buf;
    ip6->ver = (uint8_t) ((ip6->ver & ~0xf0) | (6 << 4));
    // send all handled IPv6 "next headers", then 2 random ones
    if (st->ip6_next >= (sizeof(ip6_protos) / sizeof(ip6_protos[0]) + 2)) st->ip6_next = 0;
    if (st->ip6_next < (sizeof(ip6_protos) / sizeof(ip6_protos[0]))) ip6->next = ip6_protos[st->ip6_next++];
    if (ip6->next == 6) { // TCP
    } else if (ip6->next == 17) { // UDP
    } else if (ip6->next == 58 && len >= sizeof(struct ip6) + sizeof(struct icmp6)) { // ICMPv6
      uint8_t icmp6_types[] = {128, 134, 135, 136}; // Echo Request, RA, NS, NA
      struct icmp6 *icmp6 = (struct icmp6 *) (ip6 + 1);
      // send all handled ICMPv6 types, then 2 random ones
      if (st->icmp6_type >= (sizeof(icmp6_types) / sizeof(icmp6_types[0]) + 2)) st->icmp6_type = 0;
      if (st->icmp6_type < (sizeof(icmp6_types) / sizeof(icmp6_types[0]))) icmp6->type = icmp6_types[st->icmp6_type++];
    }
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
    {
      struct mg_mqtt_prop prop;
      size_t ofs = 0;
      while ((ofs = mg_mqtt_next_prop(&mm, &prop, ofs)) > 0) {
          mg_crc32(0, prop.key.buf, prop.key.len);
          mg_crc32(0, prop.val.buf, prop.val.len);
      }
    }
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
    static size_t t;
    struct mg_tcpip_if mif = {.ip = 1,
                              .mask = 255,
                              .gw = 1,
                              .gw_ready = true,
                              .state = MG_TCPIP_STATE_READY,
#if MG_ENABLE_IPV6
                              .ip6[0] = 1,
                              .prefix[0] = 1,
                              .prefix_len = 64,
                              .gw6[0] = 1,
                              .gw6_ready = true,
                              .state6 = MG_TCPIP_STATE_READY,  // so mg_send() works and RS stops
#endif
                              .driver = &mg_tcpip_driver_mock};
    struct mg_mgr mgr;

    if (t >= MG_TCPIP_L2_USER) t = 0; // defined in l2.h
    mif.l2type = (enum mg_l2type) t++; // configure to a L2 type

    mg_mgr_init(&mgr);
    mg_tcpip_init(&mgr, &mif);

    // Make a copy of the random data, in order to modify it
    void *pkt = malloc(size);
    struct eth *eth = (struct eth *) pkt;
    memcpy(pkt, data, size);
    if ((mif.l2type == MG_TCPIP_L2_ETH || mif.l2type == MG_TCPIP_L2_PPPoE) && size > sizeof(*eth)) {
      static size_t i, v;
      static struct fuzz_l3_state l3;
      uint8_t *ethpay = (uint8_t *) (eth + 1);
      size_t ethhdrsz = sizeof(*eth);
      uint16_t etype;
      // eth_types[] exists in l2_eth.c
      memcpy(eth->dst, mif.mac, 6);  // Set valid destination MAC
      // send all handled eth types, then 2 random ones
      if (i >= (sizeof(eth_types) / sizeof(eth_types[0]) + 2)) i = 0;
      if (i < (sizeof(eth_types) / sizeof(eth_types[0]))) eth->type = mg_htons(eth_types[i++]);
      etype = mg_ntohs(eth->type);
      if (v >= 10) v = 0; // 802.1Q tag 3 out of 10 frames
      if (v < 3) mif.l2data.eth.vlan_id = 123;
      if (size > sizeof(*eth) + sizeof(struct qtag)) {
        struct qtag *qtag = (struct qtag *) &eth->type;
        if (v == 0) {
          // v=0: configured for VLAN 123, untagged frame, discard path
        } else if (v == 1) {
          // v=1: configured for VLAN 123, tagged VLAN 123, accept path
          qtag->tpid = mg_htons(0x8100);
          qtag->tci = mg_htons(123);
          MG_STORE_BE16(qtag + 1, etype);
          ethhdrsz += sizeof(*qtag);
          ethpay += sizeof(*qtag);
        } else if (v == 2) {
          // v=2: configured for VLAN 123, tagged random VLAN, mismatch path
          qtag->tpid = mg_htons(0x8100);
          MG_STORE_BE16(qtag + 1, etype);
          ethhdrsz += sizeof(*qtag);
          ethpay += sizeof(*qtag);
        } else if (v == 3) {
          // v=3: unconfigured VLAN, tagged frame, discard path
          qtag->tpid = mg_htons(0x8100);
          MG_STORE_BE16(qtag + 1, etype);
          ethhdrsz += sizeof(*qtag);
          ethpay += sizeof(*qtag);
        } // else unconfigured VLAN, untagged frame, normal path
      }
      ++v;
      if (etype == 0x8863 && size > (ethhdrsz + sizeof(struct pppoe))) { // PPPoE disc
        static size_t j;
        uint8_t pppoe_codes[] = {MG_PPPoE_PADO, MG_PPPoE_PADS, MG_PPPoE_PADT}; // defined in l2_ppp.c
        struct pppoe *pppoe = (struct pppoe *) ethpay;
        pppoe->vertype = 0x11;
        // send all handled PPPoE codes, then 2 random ones
        if (j >= (sizeof(pppoe_codes) / sizeof(pppoe_codes[0]) + 2)) j = 0;
        if (j < (sizeof(pppoe_codes) / sizeof(pppoe_codes[0]))) pppoe->code = (pppoe_codes[j++]);
        if (pppoe->code == MG_PPPoE_PADO) {
          static size_t k;
          // send id=0, then 2 random ids
          if (k >= 3) k = 0;
          if (k == 0) pppoe->id = 0;
          ++k;
        } else if (pppoe->code == MG_PPPoE_PADS) {
          static size_t k;
          // send id=x, then 2 random ids
          if (k >= 3) k = 0;
          if (k == 0) pppoe->id = 0x1234;
          ++k;
        } else if (pppoe->code == MG_PPPoE_PADT) { // keep separate so they do not correlate too much
          static size_t k;
          // send id=x, then 2 random ids
          if (k >= 3) k = 0;
          if (k == 0) pppoe->id = 0x1234;
          ++k;
        }
      } else if (etype == 0x8864 && size > (ethhdrsz + sizeof(struct pppoe))) { // PPPoE sess
        // PPP rx is covered via HDLC too; PPPoE tx wrapping may remain unexercised.
        struct pppoe *pppoe = (struct pppoe *) ethpay;
        static size_t k, l;
        // send code=0, then 2 random codes
        if (k >= 3) k = 0;
        if (k == 0)  {
          static size_t m;
          pppoe->code = 0;
          // send id=x, then 2 random ids
          if (m >= 3) m = 0;
          if (m == 0) pppoe->id = 0x1234;
          ++m;
        }
        ++k;
        // set s_state to valid and invalid
        if (l >= 2) l = 0;
        if (l == 0) s_state = MG_PPPoE_ST_SESS; // defined in l2_ppp.c
        if (l == 1) s_state = MG_PPPoE_ST_REQ;
        ++l;
        // PPP exercised in other block
      }
      // build proper layer-3 datagrams, to be able to exercise layers above
      else if (etype == 0x800 && size > (ethhdrsz + sizeof(struct ip))) {         // IPv4
        fuzz_ip4(ethpay, size - ethhdrsz, &l3);
      } else if (etype == 0x806) {      // ARP

      } else if (etype == 0x86dd && size > (ethhdrsz + sizeof(struct ip6))) {     // IPv6
        fuzz_ip6(ethpay, size - ethhdrsz, &l3);
      }
    } else if (mif.l2type == MG_TCPIP_L2_PPP && size > (sizeof(struct hdlc_) + sizeof(struct ppp) + 2)) {
      static size_t i;
      static struct fuzz_l3_state l3;
      uint16_t ppp_protos[] = {
          MG_PPP_PROTO_LCP, MG_PPP_PROTO_IPCP, MG_PPP_PROTO_IP, MG_PPP_PROTO_IPV6CP, MG_PPP_PROTO_IPV6
      };
      struct hdlc_ *hdlc = (struct hdlc_ *) pkt;
      struct ppp *ppp = (struct ppp *) (hdlc + 1);
      size_t len = size - sizeof(*hdlc) - sizeof(*ppp) - 2;
      hdlc->addr = MG_PPP_ADDR;
      hdlc->ctrl = MG_PPP_CTRL;
      s_lcpup = true;
      // send all handled PPP protocols, then 2 random ones
      if (i >= (sizeof(ppp_protos) / sizeof(ppp_protos[0]) + 2)) i = 0;
      if (i < (sizeof(ppp_protos) / sizeof(ppp_protos[0]))) ppp->proto = mg_htons(ppp_protos[i++]);
      if (ppp->proto == mg_htons(MG_PPP_PROTO_LCP) && len >= sizeof(struct lcp)) {
        static size_t j, k;
        uint8_t lcp_codes[] = {MG_PPP_LCP_CFG_REQ, MG_PPP_LCP_CFG_ACK, MG_PPP_LCP_CFG_TERM_REQ, MG_PPP_LCP_ECHO_REQ};
        struct lcp *lcp = (struct lcp *) (ppp + 1);
        // send all handled LCP codes, then 2 random ones
        if (j >= (sizeof(lcp_codes) / sizeof(lcp_codes[0]) + 2)) j = 0;
        if (j < (sizeof(lcp_codes) / sizeof(lcp_codes[0]))) lcp->code = lcp_codes[j++];
        // send valid length, then 2 random lengths
        if (k >= 3) k = 0;
        if (k == 0) lcp->len = mg_htons((uint16_t) sizeof(*lcp));
        ++k;
      } else if (ppp->proto == mg_htons(MG_PPP_PROTO_IPCP) && len >= 10) {
        static size_t j, k, m;
        uint8_t ipcp_codes[] = {MG_PPP_IPCP_CFG_REQ, MG_PPP_IPCP_CFG_ACK, MG_PPP_IPCP_CFG_NACK, MG_PPP_IPCP_CFG_REJECT};
        struct ipcp *ipcp = (struct ipcp *) (ppp + 1);
        uint8_t *opts = (uint8_t *) (ipcp + 1);
        // send all handled IPCP codes, then 2 random ones
        if (j >= (sizeof(ipcp_codes) / sizeof(ipcp_codes[0]) + 2)) j = 0;
        if (j < (sizeof(ipcp_codes) / sizeof(ipcp_codes[0]))) ipcp->code = ipcp_codes[j++];
        // send valid length, then 2 random lengths
        if (m >= 3) m = 0;
        if (m == 0) ipcp->len = mg_htons(10);
        ++m;
        // send valid IPADDR option, then 2 random option types
        if (k >= 3) k = 0;
        if (k == 0) {
          static size_t l;
          opts[0] = MG_PPP_IPCP_OPT_IPADDR;
          // for valid option type, send valid length, then random length
          if (l >= 2) l = 0;
          if (l == 0) opts[1] = 6;
          ++l;
        }
        ++k;
      } else if (ppp->proto == mg_htons(MG_PPP_PROTO_IP) && len >= sizeof(struct ip)) {
        fuzz_ip4(ppp + 1, len, &l3);
      } else if (ppp->proto == mg_htons(MG_PPP_PROTO_IPV6CP) && len >= 14) {
        static size_t j, k, m;
        uint8_t ipv6cp_codes[] = {MG_PPP_IPV6CP_CFG_REQ, MG_PPP_IPV6CP_CFG_ACK, MG_PPP_IPV6CP_CFG_NACK, MG_PPP_IPV6CP_CFG_REJECT};
        struct ipv6cp *ipv6cp = (struct ipv6cp *) (ppp + 1);
        uint8_t *opts = (uint8_t *) (ipv6cp + 1);
        // send all handled IPV6CP codes, then 2 random ones
        if (j >= (sizeof(ipv6cp_codes) / sizeof(ipv6cp_codes[0]) + 2)) j = 0;
        if (j < (sizeof(ipv6cp_codes) / sizeof(ipv6cp_codes[0]))) ipv6cp->code = ipv6cp_codes[j++];
        // send valid length, then 2 random lengths
        if (m >= 3) m = 0;
        if (m == 0) ipv6cp->len = mg_htons(14);
        ++m;
        // send valid IFCID option, then 2 random option types
        if (k >= 3) k = 0;
        if (k == 0) {
          static size_t l;
          opts[0] = MG_PPP_IPV6CP_OPT_IFCID;
          // for valid option type, send valid length, then random length
          if (l >= 2) l = 0;
          if (l == 0) {
            static size_t o;
            opts[1] = 10;
            // for valid option length, send non-zero IFCID, then 2 random IFCIDs
            if (o >= 3) o = 0;
            if (o == 0) opts[9] = 1;
            ++o;
          }
          ++l;
        }
        ++k;
      } else if (ppp->proto == mg_htons(MG_PPP_PROTO_IPV6) && len >= sizeof(struct ip6)) {
        fuzz_ip6(ppp + 1, len, &l3);
      }
    }

#if defined(MAIN)
    printf("Sending to net_builtin:\n");
    mg_hexdump(pkt, size);
#endif
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
