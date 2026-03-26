#include "st67w6.h"
#include "net.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_ST67W6) && \
    MG_ENABLE_DRIVER_ST67W6

static struct mg_tcpip_if *s_ifp;
static uint32_t s_ip, s_mask;
static bool s_link = false, s_connecting = false;

static void wifi_cb(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  struct mg_wifi_data *wifi =
      &((struct mg_tcpip_driver_st67w6_data *) ifp->driver_data)->wifi;
  if (wifi->apmode && ev == MG_TCPIP_EV_ST_CHG &&
      *(uint8_t *) ev_data == MG_TCPIP_STATE_UP) {
    MG_DEBUG(("Access Point started"));
    s_ip = ifp->ip, ifp->ip = wifi->apip;
    s_mask = ifp->mask, ifp->mask = wifi->apmask;
    ifp->enable_dhcp_client = false;
    ifp->enable_dhcp_server = true;
  }
}

static bool st67w6_init(uint8_t *mac);
static void st67w6_poll(bool is_at);

static bool mg_tcpip_driver_st67w6_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_st67w6_data *d =
      (struct mg_tcpip_driver_st67w6_data *) ifp->driver_data;
  struct mg_wifi_data *wifi = &d->wifi;
  if (MG_BIG_ENDIAN) {
    MG_ERROR(("Big-endian host"));
    return false;
  }
  if (d->is_ready == NULL) return false;
  s_ifp = ifp;
  s_ip = ifp->ip;
  s_mask = ifp->mask;
  s_link = false;
  ifp->pfn = wifi_cb;
  if (!st67w6_init(ifp->mac)) return false;

  if (d->send_queue.size == 0) d->send_queue.size = 8192;
  d->send_queue.buf = (char *) mg_calloc(1, d->send_queue.size);
  if (d->send_queue.buf == NULL) {
    MG_ERROR(("OOM"));
    return false;
  }

  if (wifi->apmode) {
    return mg_wifi_ap_start(wifi);
  } else if (wifi->ssid != NULL && wifi->pass != NULL) {
    return mg_wifi_connect(wifi);
  }
  return true;
}

// Decouple; module access depends on it being RDY. See st67w6_poll()
size_t mg_tcpip_driver_st67w6_output(const void *buf, size_t len,
                                     struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_st67w6_data *d =
      (struct mg_tcpip_driver_st67w6_data *) ifp->driver_data;
  char *p;
  if (mg_queue_book(&d->send_queue, &p, len) < len) return 0;
  memcpy(p, buf, len);
  mg_queue_add(&d->send_queue, len);
  return len;
}

static bool mg_tcpip_driver_st67w6_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct mg_tcpip_driver_st67w6_data *d =
      (struct mg_tcpip_driver_st67w6_data *) ifp->driver_data;
  if (d->is_ready == NULL) return false;
  st67w6_poll(false);
  if (!s1) return false;
  return s_link;
}

struct mg_tcpip_driver mg_tcpip_driver_st67w6 = {
    mg_tcpip_driver_st67w6_init, mg_tcpip_driver_st67w6_output, NULL,
    mg_tcpip_driver_st67w6_poll};

//  AT | STA | AP | HCI | OT
// --------------------------
//       framing             <-- includes rx stall indication
// --------------------------
//         SPI               <-- padded to 32-bit
//
// Transactions take place when the module signals RDY, 'tx' and 'write'
// functions actually just write to memory
// - AT: handles configuration and events (unsolicited +foo:). Responses may
// come as:
//   - 1 frame: just OK or ERROR
//   - 2 frames: 1 text line each, a response and OK
//   - 3 frames: 2 text lines: 1 frame with text and no CR/LF, 1 frame with
//   binary and CR/LF, 1 text line with OK
// - STA and AP contain plain Ethernet frames to/from the STA and AP networks,
// respectively
// - HCI: BLE stuff
// - OT: ?

#pragma pack(push, 1)
// little endian

struct spi_hdr {
  uint16_t magic;
  uint16_t len;
  uint8_t vflags;  // version :2, rx_stall :1, flags :5
  uint8_t type;
  uint16_t reserved;
};

#define ST67W6_SPI_TYPE_AT 0
#define ST67W6_SPI_TYPE_STA 1
#define ST67W6_SPI_TYPE_AP 2
#define ST67W6_SPI_TYPE_HCI 3
#define ST67W6_SPI_TYPE_OT 4

#pragma pack(pop)

static uint32_t txdata[2048 / 4], rxdata[2048 / 4];
static uint8_t at_resp[300];

static bool s_at_ok, s_at_err;
static size_t s_at_resp_len;
static void st67w6_handle_wifi_evnt(char *, size_t len);
static void st67w6_handle_scan_result(char *, size_t len);

static size_t st67w6_spi_poll(uint8_t *write, uint8_t *read);
static void st67w6_write(unsigned int f, void *data, uint16_t len);
static void st67w6_update_hash_table(void);

// High-level comm stuff

static void st67w6_poll(bool is_at) {
  struct mg_tcpip_driver_st67w6_data *d =
      (struct mg_tcpip_driver_st67w6_data *) s_ifp->driver_data;
  struct spi_hdr *h = (struct spi_hdr *) rxdata;
  unsigned int type;
  uint8_t *txsource = (uint8_t *) txdata;
  if (s_ifp->update_mac_hash_table) {
    // first call to _poll() is after _init(), so this is safe
    st67w6_update_hash_table();
    s_ifp->update_mac_hash_table = false;
  }
  if (!is_at) {  // send outstanding WLAN frames in the queue
    char *buf;
    size_t len;
    // NOTE(): have traffic-dependent queues or queue traffic type with data
    if ((len = mg_queue_next(&d->send_queue, &buf)) > 0) {
      st67w6_write(d->wifi.apmode ? ST67W6_SPI_TYPE_AP : ST67W6_SPI_TYPE_STA,
                   buf, (uint16_t) len);
      mg_queue_del(&d->send_queue, len);
    } else {  // nothing to send
      txsource = NULL;
    }
  }
  if (st67w6_spi_poll(txsource, (uint8_t *) rxdata) == 0) return;
  if (h->len == 0) return;
  type = h->type;
  if (type == ST67W6_SPI_TYPE_AT) {
    char *p = (char *) (h + 1);
    size_t len = h->len;
    if (len > 7 && strncmp(p, "+CWLAP:", 7) == 0) {  // scan result
      st67w6_handle_scan_result(p + 7, len - 7);
    } else if (len > 4 && strncmp(p, "+CW:", 4) == 0) {
      st67w6_handle_wifi_evnt(p + 4, len - 4);
    } else {
      MG_VERBOSE(("AT partial: %.*s", (int) len, p));
      if (s_at_resp_len + len >= sizeof(at_resp))
        s_at_resp_len = 0;  // truncate response, error will be caught later
      memcpy(at_resp + s_at_resp_len, p, len);
      s_at_resp_len += len;
      if (mg_match(mg_str_n((char *) at_resp, s_at_resp_len),
                   mg_str_n("*ERROR*", 7), NULL)) {
        s_at_err = true;
      } else if (mg_match(mg_str_n((char *) at_resp, s_at_resp_len),
                          mg_str_n("*OK*", 4), NULL)) {
        s_at_ok = true;
      }
    }
  } else if (type == ST67W6_SPI_TYPE_STA || type == ST67W6_SPI_TYPE_AP) {
    // WLAN frame reception
    mg_tcpip_qwrite(h + 1, h->len, s_ifp);
  }  // else silently discard
}

// WLAN event handling

// - Do not call any AT functions here, otherwise revise st67w6_at_wait()
// - The module likes to send ERROR along with other events
static void st67w6_handle_wifi_evnt(char *p, size_t len) {
  struct mg_str data[2];
  MG_VERBOSE(("event: %.*s", (int) len, p));
  if (len > 9 && strncmp(p, "CONNECTED", 9) == 0) {
    s_link = true;
    s_connecting = false;
  } else if (len > 12 && strncmp(p, "DISCONNECTED", 12) == 0) {
    s_link = false;
    s_connecting = false;  // should not be needed
  } else if (s_connecting && len > 6 &&
             mg_match(mg_str_n(p, len), mg_str_n("ERROR,*\r\n*", 10), data)) {
    size_t reason = 0;
    bool ok = mg_to_size_t(data[0], &reason);
    s_connecting = false;
    MG_ERROR(("CONNECT FAILED"));
    mg_tcpip_call(s_ifp, MG_TCPIP_EV_WIFI_CONNECT_ERR, ok ? &reason : NULL);
  } else if (len > 9 && strncmp(p, "SCAN_DONE", 9) == 0) {
    MG_VERBOSE(("scan complete"));
    mg_tcpip_call(s_ifp, MG_TCPIP_EV_WIFI_SCAN_END, NULL);
  }  // else silently discard: CONNECTING; STA_CONNECTED,"MAC";
     // STA_DISCONNECTED,"MAC"; DIST_STA_IP,"MAC","IP"
}

static bool st67w6_at_cmd(char *cmd, size_t len);

// Wi-Fi network stuff

static bool st67w6_wifi_connect(char *ssid, char *pass) {
  char cmd[90];  // ssid + pass + AT
  size_t cmd_len;
  if (!st67w6_at_cmd("AT+CWMODE=1,0\r\n", 15)) return false;
  cmd_len = mg_snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\",,0\r\n", ssid,
                        pass);  // takes >700ms
  if (!st67w6_at_cmd(cmd, cmd_len)) return false;
  st67w6_at_cmd("AT+CWRECONNCFG=0,0\r\n", 20);  // disregard error, connecting
  s_connecting = true;
  return true;
}

static bool st67w6_wifi_disconnect(void) {
  s_connecting = false;
  if (!st67w6_at_cmd("AT+CWQAP=0\r\n", 12)) return false;  // takes >800ms
  return st67w6_at_cmd("AT+CWMODE=0,0\r\n", 15);           // takes >550ms
}

static bool st67w6_wifi_ap_start(char *ssid, char *pass, unsigned int channel) {
  char cmd[90];  // ssid + pass + AT
  size_t cmd_len;
  if (!st67w6_at_cmd("AT+CWMODE=2,0\r\n", 15)) return false;  // takes >800ms
  cmd_len = mg_snprintf(
      cmd, sizeof(cmd), "AT+CWSAP=\"%s\",\"%s\",%u,3,2,0\r\n", ssid, pass,
      channel);  // 3: WPA2_PSK; 2: max stations  // takes >350ms
  s_link = true;
  return st67w6_at_cmd(cmd, cmd_len);
}

static bool st67w6_wifi_ap_stop(void) {
  s_link = false;
  return st67w6_at_cmd("AT+CWMODE=0,0\r\n", 15);  // takes >550ms
}

// WLAN scan handling

// +CWLAP:(security,"SSID",RSSI,"BSSID",channel,cipher,proto,wps)\r\n
// security: OPEN, WEP, WPA, WPA2, WPA-WPA2, WPA-EAP, WPA3-SAE, WPA2-WPA3-SAE
// cipher: NONE, WEP, AES/CCMP, TKIP, TKIP and AES/CCMP
// proto: 4-bit bitmap AX,N,G,B; all set from right to left

static bool st67w6_wifi_scan(void) {
  return st67w6_at_cmd("AT+CWLAPOPT=1,1695,-100,255,50\r\n", 32) &&
         st67w6_at_cmd("AT+CWLAP=0,,,0\r\n", 16);
}

static void st67w6_handle_scan_result(char *data, size_t len) {
  struct mg_wifi_scan_bss_data bss;
  struct mg_str fields[2];
  char mac[6];
  uint8_t val;
  unsigned int i;
  MG_VERBOSE(("scan result event: %.*s", (int) len, data));
  ++data, --len;  // skip '('
  if (!mg_span(mg_str_n(data, len), &fields[0], &fields[1], ',') ||
      !mg_str_to_num(fields[0], 10, &val, 1))
    return;
  bss.security =
      (val == 0) ? MG_WIFI_SECURITY_OPEN : (uint8_t) MG_WIFI_SECURITY_WEP;
  if (val == 2 || val == 4) bss.security |= MG_WIFI_SECURITY_WPA;
  if (val == 3 || val == 4 || val == 7) bss.security |= MG_WIFI_SECURITY_WPA2;
  if (val == 6 || val == 7) bss.security |= MG_WIFI_SECURITY_WPA3;
  if (val == 5) bss.security |= MG_WIFI_SECURITY_WPA_ENTERPRISE;
  if (!mg_span(fields[1], &fields[0], &fields[1], ',')) return;
  bss.SSID.buf = fields[0].buf + 1, bss.SSID.len = fields[0].len - 2;
  if (!mg_span(fields[1], &fields[0], &fields[1], ',')) return;
  while (fields[0].buf[0] == ' ') ++fields[0].buf, --fields[0].len;
  if (fields[0].buf[0] != '-') return;  // positive RSSI would be great
  ++fields[0].buf, --fields[0].len;
  if (!mg_str_to_num(fields[0], 10, &val, 1)) return;
  bss.RSSI = (int8_t) - (int8_t) val;
  if (!mg_span(fields[1], &fields[0], &fields[1], ',')) return;
  if (fields[0].len < 19) return;
  ++fields[0].buf, --fields[0].len;  // skip '"'
  for (i = 0; i < 6; i++) {
    struct mg_str str;
    str.buf = fields[0].buf + 3 * i;
    str.len = 2;
    if (!mg_str_to_num(str, 16, &mac[i], 1)) return;
  }
  bss.BSSID = mac;
  if (!mg_span(fields[1], &fields[0], &fields[1], ',') ||
      !mg_str_to_num(fields[0], 10, &bss.channel, 1))
    return;
  if (!mg_span(fields[1], &fields[0], &fields[1], ',') ||
      !mg_str_to_num(fields[0], 10, &val, 1))
    return;
  // ignore cypher
  if (!mg_span(fields[1], &fields[0], &fields[1], ',') ||
      !mg_str_to_num(fields[0], 10, &val, 1))
    return;
  bss.has_n = (val & 4) != 0;
  bss.has_ax = (val & 8) != 0;
  bss.band = MG_WIFI_BAND_2G;  // NOT INFORMED with default options, no docs
  MG_VERBOSE(("BSS: %.*s (%u) (%M) %d dBm %u", bss.SSID.len, bss.SSID.buf,
              bss.channel, mg_print_mac, bss.BSSID, (int) bss.RSSI,
              bss.security));
  mg_tcpip_call(s_ifp, MG_TCPIP_EV_WIFI_SCAN_RESULT, &bss);
}

// AT stuff

static inline bool delayms(unsigned int ms) {
  mg_delayms(ms);
  return true;
}

// send AT command, wait for a response or timeout, meanwhile delivering
// received frames and events

static bool st67w6_at_cmd(char *cmd, size_t len) {
  bool is_at = true;
  unsigned int times = 1000;
  s_at_resp_len = 0;
  st67w6_write(ST67W6_SPI_TYPE_AT, cmd, (uint16_t) len);
  s_at_ok = false, s_at_err = false;
  do {  // AT response processing does not call any other AT function
    st67w6_poll(is_at);  // otherwise we can't allow them to pile up here
    is_at = false;       // avoid repeating, allow queued WLAN frames to be sent
    // network frames will be pushed to the queue so that is safe
  } while (!s_at_ok && !s_at_err && times-- > 0 && delayms(1));
  MG_VERBOSE(("AT response:\n%.*s", s_at_resp_len, at_resp));
  MG_VERBOSE(("ok: %c, err: %c, times: %d", s_at_ok ? '1' : '0',
              s_at_err ? '1' : '0', (int) times));
  return s_at_ok;
}

static bool st67w6_spi_init(void);
bool mg_to_size_t(struct mg_str str, size_t *val);

static bool st67w6_init(uint8_t *mac) {
  //  struct mg_tcpip_driver_st67w6_data *d = (struct
  //  mg_tcpip_driver_st67w6_data *) s_ifp->driver_data;
  struct mg_str data[3];
  size_t val;
  bool is_b = false;
  if (!st67w6_spi_init()) return false;
  if (!st67w6_at_cmd("AT\r\n", 4)) return false;
  if (!st67w6_at_cmd("AT+CWNETMODE?\r\n", 15) ||
      !mg_match(mg_str_n((char *) at_resp, s_at_resp_len),
                mg_str_n("*:*\r\n*", 6), data) ||
      !mg_to_size_t(data[1], &val))
    return false;
  if (val != 0) {
    MG_ERROR(("Wrong firmware, T02 is needed"));
    return false;
  }
  // set clock, who cares ???
  if (!st67w6_at_cmd("AT+GET_CLOCK\r\n", 14)) return false;
  MG_DEBUG(("%.*s", s_at_resp_len, at_resp));  // TODO(scaprile): --> VERBOSE
  // BT-ENABLED DEPENDENCY

  // MODULE DEPENDENCY
  if (!st67w6_at_cmd("AT+EFUSE-R=24,\"0x100\"\r\n", 23) ||
      !mg_match(mg_str_n((char *) at_resp, s_at_resp_len), mg_str_n("*,*", 3),
                data))
    return false;
  if (data[1].buf[0] == 'C' && data[1].buf[1] == '6') is_b = true;
  MG_DEBUG(("WLAN module is %sB type", is_b ? "" : "not"));  // --> VERBOSE
  if (is_b) {
    // Disable the antenna diversity pin
    if (!st67w6_at_cmd("AT+IORST=0\r\n", 12)) return false;
    // Apparently they intend to disable some antenna ...
    if (!st67w6_at_cmd("AT+CWANTENABLE?\r\n", 17)) return false;
    MG_DEBUG(("%.*s", s_at_resp_len, at_resp));  // --> VERBOSE
  }

  // Do not set wake-up pin (AT+SLWKIO)
  // Disable power save mode
  // NOTE(scaprile): (no response if in hibernate mode, though I guess we
  // wouldn't have reached this point in that case either)
  if (!st67w6_at_cmd("AT+PWR=0\r\n", 12)) return false;
  // set Wi-Fi
  // set country code
  if (!st67w6_at_cmd("AT+CWCOUNTRY=0,\"00\"\r\n", 21)) return false;
#if 0
	// set DTIM
  if (!st67w6_at_cmd("AT+SLWKDTIM=1\r\n", 15)) return false;
#endif
  // Read only default MAC, ignore set bit count (data[6] & 0x3F). Custom MACs
  // reside at @0x64 and 0x70
  if (st67w6_at_cmd("AT+EFUSE-R=7,\"0x014\"\r\n", 22) &&
      mg_match(mg_str_n((char *) at_resp, s_at_resp_len), mg_str_n("*,*", 3),
               data)) {
    int i;
    for (i = 0; i < 6; i++) mac[i] = data[1].buf[5 - i];
    MG_DEBUG(("MAC: %M", mg_print_mac, mac));
  } else {
    MG_ERROR(("read MAC failed"));
  }
  return true;
}

static void st67w6_update_hash_table(void) {
  // TODO(): read database, rebuild hash table
  //  uint32_t val = 0;
  //  val = 1;
  //  st67w6_at_iovar_set2_(0, "mcast_list", (uint8_t *) &val, sizeof(val),
  //  (uint8_t *) mcast_addr, sizeof(mcast_addr)); mg_delayms(50);
}

// SPI specifics

#define ST67W6_SPI_MAGIC 0x55AA
// #define ST67W6_SPI_VERSION(x), IS_STALL, FLAGS(x), ...

static const uint8_t idlehdr[sizeof(struct spi_hdr)] = {0xaa, 0x55, 0, 0,
                                                        0,    0,    0, 0};

static void st67w6_write(unsigned int f, void *data, uint16_t len) {
  struct spi_hdr *h = (struct spi_hdr *) txdata;
  h->magic = ST67W6_SPI_MAGIC;
  h->type = (uint8_t) f;
  h->vflags = 0;
  h->len = len;
  h->reserved = 0;
  memmove(h + 1, data, len);
}

static size_t st67w6_spi_poll(uint8_t *write, uint8_t *read) {
  struct mg_tcpip_driver_st67w6_data *d =
      (struct mg_tcpip_driver_st67w6_data *) s_ifp->driver_data;
  struct spi_hdr *th, *rh = (struct spi_hdr *) read;
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) d->spi;
  size_t padded;
  unsigned int times;

  th = (write != NULL) ? (struct spi_hdr *) write : (struct spi_hdr *) idlehdr;
  s->begin(s->spi);
  times = 50;
  while (times--) {
    if (d->is_ready()) break;
    if (times == 0) {
      MG_ERROR(("RDY TIMEOUT"));
      s->end(s->spi);
      return 0;
    }
    mg_delayms(1);
  }

  padded = (th->len + 3) & ~3;
  s->txn(s->spi, (uint8_t *) th, (uint8_t *) rh, sizeof(*th) + padded);
  if (rh->magic == ST67W6_SPI_MAGIC && rh->len > padded) {
    size_t remaining_padded = (rh->len - padded + 3) & ~3;
    if (remaining_padded > (2048 - sizeof(*rh) - padded))
      remaining_padded = 2048 - sizeof(*rh) - padded;
    s->txn(s->spi, NULL, read + sizeof(*rh) + padded, remaining_padded);
  }
  times = 50;
  while (times--) {
    if (!d->is_ready()) break;
    if (times == 0) {
      MG_ERROR(("!RDY TIMEOUT"));
      break;
    }
    mg_delayms(1);
  }
  s->end(s->spi);
  return (size_t) rh->len;
}

static bool st67w6_spi_init(void) {
  struct mg_tcpip_driver_st67w6_data *d =
      (struct mg_tcpip_driver_st67w6_data *) s_ifp->driver_data;
  size_t len;
  unsigned int times = 1000;
  while (times--) {
    if (d->is_ready()) break;
    if (times == 0) return false;
    mg_delayms(1);
  }
  if (((len = st67w6_spi_poll(NULL, (uint8_t *) rxdata)) == 0) ||
      !mg_match(mg_str_n(((char *) rxdata) + sizeof(struct spi_hdr), len),
                mg_str_n("*ready*", 7), NULL))
    return false;
  return true;
}

// Mongoose Wi-Fi API functions

bool mg_wifi_scan(void) {
  return st67w6_wifi_scan();
}

bool mg_wifi_connect(struct mg_wifi_data *wifi) {
  s_ifp->ip = s_ip;
  s_ifp->mask = s_mask;
  if (s_ifp->ip == 0) s_ifp->enable_dhcp_client = true;
  s_ifp->enable_dhcp_server = false;
  MG_DEBUG(("Connecting to '%s'", wifi->ssid));
  return st67w6_wifi_connect(wifi->ssid, wifi->pass);
}

bool mg_wifi_disconnect(void) {
  return st67w6_wifi_disconnect();
}

bool mg_wifi_ap_start(struct mg_wifi_data *wifi) {
  MG_DEBUG(("Starting AP '%s' (%u)", wifi->apssid, wifi->apchannel));
  return st67w6_wifi_ap_start(wifi->apssid, wifi->appass, wifi->apchannel);
}

bool mg_wifi_ap_stop(void) {
  return st67w6_wifi_ap_stop();
}

#endif
