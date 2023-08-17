// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// SNTP example using MIP and pcap driver

#include <pcap.h>
#include "mongoose.h"

#define MQTT_URL "mqtt://broker.hivemq.com:1883"  // MQTT broker to connect to
#define MQTT_TOPIC "t/123"                        // Topic to subscribe to

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

static void fn2(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_DEBUG(("Got response (%d) %.*s...", (int) hm->message.len, 12,
              hm->message.ptr));
    c->is_draining = 1;
  } else if (ev == MG_EV_CONNECT) {
    mg_printf(c, "GET %s HTTP/1.1\r\n\r\n", mg_url_uri((char *) fn_data));
  } else if (ev == MG_EV_CLOSE) {
    free(fn_data);
  }
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/debug")) {
      int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    } else if (mg_http_match_uri(hm, "/api/url")) {
      char *url = mg_json_get_str(hm->body, "$.url");
      if (url == NULL) {
        mg_http_reply(c, 200, NULL, "no url, rl %d\r\n", (int) c->recv.len);
      } else {
        mg_http_connect(c->mgr, url, fn2, url);
        mg_http_reply(c, 200, NULL, "ok\r\n");
      }
    } else {
      mg_http_reply(c, 200, NULL, "%.*s\r\n", (int) hm->message.len,
                    hm->message.ptr);
    }
  }
  (void) ev_data, (void) fn_data;
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
  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, &mgr);

  while (s_signo == 0) mg_mgr_poll(&mgr, 100);  // Infinite event loop

  mg_mgr_free(&mgr);
  pcap_close(ph);
  printf("Exiting on signal %d\n", s_signo);

  return 0;
}
