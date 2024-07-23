// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// SNTP example using MIP and pcap driver

#include <pcap.h>
#include "mongoose.h"

#define MQTT_URL "mqtt://broker.emqx.io:1883"    // MQTT broker URL
#define MQTTS_URL "mqtts://mongoose.ws:8883"  // HiveMQ does not TLS1.3
#define MQTT_TOPIC "mg/rx"                       // Topic to subscribe to

// // Taken from broker.emqx.io
static const char *s_ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
    "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
    "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
    "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
    "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
    "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
    "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
    "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
    "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
    "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
    "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
    "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
    "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
    "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
    "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
    "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
    "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
    "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
    "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
    "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
    "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
    "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
    "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
    "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
    "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
    "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
    "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
    "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
    "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
    "-----END CERTIFICATE-----\n";

//     "-----BEGIN CERTIFICATE-----\n"
//     "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh"
//     "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
//     "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
//     "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n"
//     "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
//     "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n"
//     "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n"
//     "2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n"
//     "1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n"
//     "q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n"
//     "tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n"
//     "vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n"
//     "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n"
//     "5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n"
//     "1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n"
//     "NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n"
//     "Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n"
//     "8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n"
//     "pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n"
//     "MrY=\n"
//     "-----END CERTIFICATE-----\n";

static const char *s_tls_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBMTCB2aADAgECAgkAluqkgeuV/zUwCgYIKoZIzj0EAwIwEzERMA8GA1UEAwwI\n"
    "TW9uZ29vc2UwHhcNMjQwNTA3MTQzNzM2WhcNMzQwNTA1MTQzNzM2WjARMQ8wDQYD\n"
    "VQQDDAZzZXJ2ZXIwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASo3oEiG+BuTt5y\n"
    "ZRyfwNr0C+SP+4M0RG2pYkb2v+ivbpfi72NHkmXiF/kbHXtgmSrn/PeTqiA8M+mg\n"
    "BhYjDX+zoxgwFjAUBgNVHREEDTALgglsb2NhbGhvc3QwCgYIKoZIzj0EAwIDRwAw\n"
    "RAIgTXW9MITQSwzqbNTxUUdt9DcB+8pPUTbWZpiXcA26GMYCIBiYw+DSFMLHmkHF\n"
    "+5U3NXW3gVCLN9ntD5DAx8LTG8sB\n"
    "-----END CERTIFICATE-----\n";

static const char *s_tls_key =
    "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEIAVdo8UAScxG7jiuNY2UZESNX/KPH8qJ0u0gOMMsAzYWoAoGCCqGSM49\n"
    "AwEHoUQDQgAEqN6BIhvgbk7ecmUcn8Da9Avkj/uDNERtqWJG9r/or26X4u9jR5Jl\n"
    "4hf5Gx17YJkq5/z3k6ogPDPpoAYWIw1/sw==\n"
    "-----END EC PRIVATE KEY-----\n";

#if MG_ARCH == MG_ARCH_WIN32
#define usleep(x) Sleep((x) / 1000)
#endif

static int s_signo;
void signal_handler(int signo) {
  s_signo = signo;
}

static size_t pcap_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  int res = pcap_inject((pcap_t *) ifp->driver_data, buf, len);
  if (res == PCAP_ERROR) {
    MG_ERROR(("pcap_inject: %d", res));
  }
  return res == PCAP_ERROR ? 0 : len;
}

static bool pcap_up(struct mg_tcpip_if *ifp) {
  return ifp->driver_data ? true : false;
}

static size_t pcap_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  size_t received = 0;
  struct pcap_pkthdr *hdr = NULL;
  const unsigned char *pkt = NULL;
  usleep(1000);  // Sleep 1 millisecond. This is to avoid 100% CPU
  if (pcap_next_ex((pcap_t *) ifp->driver_data, &hdr, &pkt) == 1) {
    received = hdr->len < len ? hdr->len : len;
    memcpy(buf, pkt, received);
  }
  return received;
}

static void fn2(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_DEBUG(("Got response (%d) %.*s...", (int) hm->message.len, 12,
              hm->message.buf));
    c->is_draining = 1;
  } else if (ev == MG_EV_CONNECT) {
    mg_printf(c, "GET %s HTTP/1.1\r\n\r\n", mg_url_uri((char *) c->fn_data));
  } else if (ev == MG_EV_CLOSE) {
    free(c->fn_data);
  }
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT && c->fn_data != NULL) {
    struct mg_tls_opts opts = {.cert = mg_str(s_tls_cert),
                               .key = mg_str(s_tls_key)};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/debug"), NULL)) {
      int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    } else if (mg_match(hm->uri, mg_str("/api/md5"), NULL)) {
      uint8_t buf[16];
      mg_md5_ctx ctx;
      mg_md5_init(&ctx);
      mg_md5_update(&ctx, (uint8_t *) hm->body.buf, hm->body.len);
      mg_md5_final(&ctx, buf);
      mg_http_reply(c, 200, NULL, "%M\r\n", mg_print_hex, sizeof(buf), buf);
    } else if (mg_match(hm->uri, mg_str("/api/url"), NULL)) {
      char *url = mg_json_get_str(hm->body, "$.url");
      if (url == NULL) {
        mg_http_reply(c, 200, NULL, "no url, rl %d\r\n", (int) c->recv.len);
      } else {
        mg_http_connect(c->mgr, url, fn2, url);
        mg_http_reply(c, 200, NULL, "ok\r\n");
      }
    } else {
      mg_http_reply(c, 200, NULL, "%.*s\r\n", (int) hm->message.len,
                    hm->message.buf);
    }
  }
  (void) ev_data;
}

static void mqtt_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    MG_INFO(("%lu CREATED", c->id));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT && c->fn_data != NULL) {
    struct mg_tls_opts opts = {.ca = mg_str(s_ca_cert),
                               .name = mg_url_host(MQTTS_URL)};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    MG_INFO(("%lu CONNECTED", c->id));
    struct mg_mqtt_opts opts = {.qos = 1, .topic = mg_str(MQTT_TOPIC)};
    mg_mqtt_sub(c, &opts);
    MG_INFO(("%lu SUBSCRIBED to %.*s", c->id, opts.topic.len, opts.topic.buf));
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, (int) mm->data.len,
             mm->data.buf, (int) mm->topic.len, mm->topic.buf));
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("%lu CLOSED", c->id));
  }
}

int main(int argc, char *argv[]) {
  const char *iface = "lo0";              // Network iface
  const char *mac = "02:00:01:02:03:77";  // MAC address
  const char *bpf = NULL;  // "host x.x.x.x or ether host ff:ff:ff:ff:ff:ff";
  char errbuf[PCAP_ERRBUF_SIZE] = "";

  // Parse options
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      iface = argv[++i];
    } else if (strcmp(argv[i], "-mac") == 0 && i + 1 < argc) {
      mac = argv[++i];
    } else if (strcmp(argv[i], "-bpf") == 0 && i + 1 < argc) {
      bpf = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
      mg_log_set(atoi(argv[++i]));
    } else {
      MG_ERROR(("unknown option %s", argv[i]));
      return EXIT_FAILURE;
    }
  }

  // Open network interface
  pcap_t *ph = pcap_open_live(iface, 0xffff, 1, 1, errbuf);
  if (ph == NULL) {
    pcap_if_t *devs, *d;
    MG_ERROR(("Failed to open interface %s. Available interfaces:", iface));
    if (pcap_findalldevs(&devs, errbuf) == 0) {
      for (d = devs; d != NULL; d = d->next) {
        MG_ERROR(("%s (%s)", d->name, d->description ? d->description : ""));
      }
      pcap_freealldevs(devs);
    }
    return EXIT_FAILURE;
  }
  pcap_setnonblock(ph, 1, errbuf);
  pcap_setdirection(ph, PCAP_D_IN);

  // Apply BPF to reduce noise. Let in only broadcasts and our own traffic
  if (bpf != NULL) {
    struct bpf_program bpfp;
    if (pcap_compile(ph, &bpfp, bpf, 1, 0)) MG_ERROR(("BPF compile failed\n"));
    pcap_setfilter(ph, &bpfp);
    pcap_freecode(&bpfp);
  }

  MG_INFO(("Opened interface %s", iface));
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  struct mg_mgr mgr;        // Event manager
  mg_mgr_init(&mgr);        // Initialise event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level

  struct mg_tcpip_driver driver = {.tx = pcap_tx, .up = pcap_up, .rx = pcap_rx};
  struct mg_tcpip_if mif = {.driver = &driver, .driver_data = ph};
  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mif.mac[0], &mif.mac[1],
         &mif.mac[2], &mif.mac[3], &mif.mac[4], &mif.mac[5]);
  mg_tcpip_init(&mgr, &mif);

  MG_INFO(("Init done, starting main loop"));
  mg_http_listen(&mgr, "http://0.0.0.0:8000", http_ev_handler, NULL);
  mg_http_listen(&mgr, "https://0.0.0.0:8443", http_ev_handler, "tls enabled");

  bool got_ip = false;

  while (s_signo == 0) {
    mg_mgr_poll(&mgr, 100);

    // Trigger MQTT connection when we receive IP address
    if (mif.state == MG_TCPIP_STATE_READY && got_ip == false) {
      struct mg_mqtt_opts opts = {.clean = true};
      // mg_mqtt_connect(&mgr, MQTT_URL, &opts, mqtt_ev_handler, NULL);
      mg_mqtt_connect(&mgr, MQTTS_URL, &opts, mqtt_ev_handler, "tls enabled");
      got_ip = true;
    }
  }

  mg_mgr_free(&mgr);
  pcap_close(ph);
  printf("Exiting on signal %d\n", s_signo);

  return 0;
}
