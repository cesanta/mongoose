// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// SNTP example using MIP and pcap driver

#include <pcap.h>
#include "mongoose.h"

static struct mg_connection *s_sntp_conn = NULL;

static int s_signo;
void signal_handler(int signo) {
  s_signo = signo;
}

// SNTP client callback
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_SNTP_TIME) {
    int64_t t = *(int64_t *) ev_data;
    MG_INFO(("Got SNTP time: %lld ms from epoch", t));
  } else if (ev == MG_EV_CLOSE) {
    s_sntp_conn = NULL;
  }
  (void) fn_data, (void) c;
}

// Called every 5 seconds. Increase that for production case.
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  if (s_sntp_conn == NULL) s_sntp_conn = mg_sntp_connect(mgr, NULL, sfn, NULL);
  if (s_sntp_conn != NULL) mg_sntp_request(s_sntp_conn);
}

static int fail(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  return EXIT_FAILURE;
}

static size_t pcap_tx(const void *data, size_t len, void *userdata) {
  int res = pcap_inject((pcap_t *) userdata, data, len);
  if (res == PCAP_ERROR) {
    MG_ERROR(("pcap_inject: %d", res));
  }
  return len;
}

static bool pcap_status(void *userdata) {
  return userdata ? true : false;
}

static size_t pcap_rx(void *buf, size_t len, void *userdata) {
  size_t received = 0;
  struct pcap_pkthdr *hdr = NULL;
  const unsigned char *pkt = NULL;
  if (pcap_next_ex((pcap_t *) userdata, &hdr, &pkt) == 1) {
    received = hdr->len < len ? hdr->len : len;
    memcpy(buf, pkt, received);
  }
  return received;
}

int main(int argc, char *argv[]) {
  const char *iface = "lo0";              // Network iface
  const char *mac = "aa:bb:cc:01:02:03";  // MAC address
  const char *bpf = NULL;  // "host x.x.x.x or ether host ff:ff:ff:ff:ff:ff";
  char errbuf[PCAP_ERRBUF_SIZE] = "";

  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager
  mg_log_set("3");    // Set debug log level
  mg_timer_add(&mgr, 3000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);

  // Parse options
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      iface = argv[++i];
    } else if (strcmp(argv[i], "-mac") == 0 && i + 1 < argc) {
      mac = argv[++i];
    } else if (strcmp(argv[i], "-bpf") == 0 && i + 1 < argc) {
      bpf = argv[++i];
    } else {
      return fail("unknown option %s", argv[i]);
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
    fail("pcap_open_live: %s\n", errbuf);
  }
  pcap_setnonblock(ph, 1, errbuf);
  // Apply BPF to reduce noise. Let in only broadcasts and our own traffic
  if (bpf != NULL) {
    struct bpf_program bpfp;
    if (pcap_compile(ph, &bpfp, bpf, 1, 0)) fail("compile \n");
    pcap_setfilter(ph, &bpfp);
    pcap_freecode(&bpfp);
  }
  MG_INFO(("Opened interface %s\n", iface));

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  struct mip_ipcfg ipcfg = {.ip = 0, .mask = 0, .gw = 0};
  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ipcfg.mac[0], &ipcfg.mac[1],
         &ipcfg.mac[2], &ipcfg.mac[3], &ipcfg.mac[4], &ipcfg.mac[5]);

  struct mip_driver driver = {
      .data = ph, .tx = pcap_tx, .status = pcap_status, .rx = pcap_rx};

  mip_init(&mgr, &ipcfg, &driver);
  MG_INFO(("Init done, starting main loop"));

  while (s_signo == 0) mg_mgr_poll(&mgr, 1);  // Infinite event loop

  mg_mgr_free(&mgr);
  pcap_close(ph);
  printf("Exiting on signal %d\n", s_signo);

  return 0;
}
