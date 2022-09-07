// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// SNTP example using MIP and pcap driver

#include <pcap.h>
#include "mongoose.h"

#define MQTT_URL "mqtt://broker.hivemq.com:1883"  // MQTT broker to connect to
#define MQTT_TOPIC "t/123"                        // Topic to subscribe to

static int s_signo;
void signal_handler(int signo) {
  s_signo = signo;
}

static size_t pcap_tx(const void *buf, size_t len, void *userdata) {
  int res = pcap_inject((pcap_t *) userdata, buf, len);
  if (res == PCAP_ERROR) {
    MG_ERROR(("pcap_inject: %d", res));
  }
  MG_INFO(("TX %lu", len));
  // mg_hexdump(buf, len);
  return res == PCAP_ERROR ? 0 : len;
}

static bool pcap_up(void *userdata) {
  return userdata ? true : false;
}

static size_t pcap_rx(void *buf, size_t len, void *userdata) {
  size_t received = 0;
  struct pcap_pkthdr *hdr = NULL;
  const unsigned char *pkt = NULL;
  if (pcap_next_ex((pcap_t *) userdata, &hdr, &pkt) == 1) {
    received = hdr->len < len ? hdr->len : len;
    memcpy(buf, pkt, received);
    MG_INFO(("RX %lu", received));
    // mg_hexdump(buf, received);
  }
  return received;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) mg_http_reply(c, 200, NULL, "hi\n");
  (void) fn_data, (void) ev_data;
}

// MQTT event handler function
static void mfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_CONNECT && mg_url_is_ssl(MQTT_URL)) {
    struct mg_tls_opts opts = {.ca = "ca.pem",
                               .srvname = mg_url_host(MQTT_URL)};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_MQTT_OPEN) {
    c->is_hexdumping = 1;
    mg_mqtt_sub(c, mg_str(MQTT_TOPIC), 2);
  } else if (ev == MG_EV_MQTT_MSG) {
    struct mg_mqtt_message *mm = ev_data;
    MG_INFO(("%.*s", (int) mm->data.len, mm->data.ptr));
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_DEBUG(("cmd %d qos %d", mm->cmd, mm->qos));
  } else if (ev == MG_EV_CLOSE) {
  }
  (void) fn_data;
}

int main(int argc, char *argv[]) {
  const char *iface = "lo0";              // Network iface
  const char *mac = "00:00:01:02:03:77";  // MAC address
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
  mg_log_set(MG_LL_DEBUG);  // Set debug log level

  struct mip_cfg c = {.ip = 0, .mask = 0, .gw = 0};
  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &c.mac[0], &c.mac[1], &c.mac[2],
         &c.mac[3], &c.mac[4], &c.mac[5]);

  struct mip_driver driver = {.tx = pcap_tx, .up = pcap_up, .rx = pcap_rx};
  mip_init(&mgr, &c, &driver, ph);
  MG_INFO(("Init done, starting main loop"));

  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);

  struct mg_mqtt_opts opts = {0};
  // mg_mqtt_connect(&mgr, MQTT_URL, &opts, mfn, NULL);

  while (s_signo == 0) mg_mgr_poll(&mgr, 1);  // Infinite event loop

  mg_mgr_free(&mgr);
  pcap_close(ph);
  printf("Exiting on signal %d\n", s_signo);

  return 0;
}
