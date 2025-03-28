#include "cyw.h"
#include "net.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_CYW) && MG_ENABLE_DRIVER_CYW


static struct mg_tcpip_if *s_ifp;
static bool s_link, s_auth, s_join;

static bool cyw_init(uint8_t *mac);
static void cyw_poll(void);

static bool mg_tcpip_driver_cyw_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_cyw_data *d =
      (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
  if (MG_BIG_ENDIAN) {
    MG_ERROR(("Big-endian host"));
    return false;
  }
  s_ifp = ifp;
  s_link = s_auth = s_join = false;
  if (!cyw_init(ifp->mac)) return false;

  if (d->apmode) {
    MG_DEBUG(("Starting AP '%s' (%u)", d->apssid, d->apchannel));
    return mg_wifi_ap_start(d->apssid, d->appass, d->apchannel);
  } else if (d->ssid != NULL && d->pass != NULL) {
    MG_DEBUG(("Connecting to '%s'", d->ssid));
    return mg_wifi_connect(d->ssid, d->pass);
  }
  return true;
}

static size_t mg_cyw_tx(unsigned int ifc, void *data, size_t len);
size_t mg_tcpip_driver_cyw_output(const void *buf, size_t len,
                                  struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_cyw_data *d =
      (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
  return mg_cyw_tx(d->apmode ? 1 : 0, (void *) buf, len) >= len ? len : 0;
}

static bool mg_tcpip_driver_cyw_poll(struct mg_tcpip_if *ifp, bool s1) {
  cyw_poll();
  if (!s1) return false;
  struct mg_tcpip_driver_cyw_data *d =
      (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
  return d->apmode ? s_link : s_link && s_auth && s_join;
}

struct mg_tcpip_driver mg_tcpip_driver_cyw = {mg_tcpip_driver_cyw_init,
                                              mg_tcpip_driver_cyw_output, NULL,
                                              mg_tcpip_driver_cyw_poll};

// - DS:
// https://www.mouser.com/datasheet/2/196/Infineon_CYW43439_DataSheet_v03_00_EN-3074791.pdf
// - WHD: https://github.com/Infineon/wifi-host-driver
//
//              |  e   <-- event data
//              |-----
//          net | vnd   <-- network (TCP/IP) | vendor header (Broadcom (bcm))
//         -----|-----
//  IOCTL | ETH | ETH  <-- IOCTL/IOVAR: chip control | ETH: Ethernet header
// -------|-----|-----
//   CDC  | BDC | BDC
// ------- ----- -----
//        SDPCM        <-- includes SDIO bus arbitration, not used in SPI
// -------------------
//    SPI   |  SDIO    <-- padded to 32-bit | 64-bytes
//
// - SDPCM has 3 channels (control, data, and asynchronous data)
// - SPI (and SDIO) has 4 "functions", F0 to F3, to access different
// blocks in the chip, like the SPI/SDIO controller, chip backplane, and 2 DMA
// I/Os; these are usually handled by SDPCM but we need to explicitly access
// the I/O controller and chip backplane during initialization

// Processor core firmware is loaded to TCM RAM, along with module-dependent
// (hardware design) NVRAM data, via the chip backplane access through the bus
// Once the chip has been initialized, information regarding regulatory
// constraints (CLM blob, “Country Locale Matrix”), is loaded as an IOVAR. This
// is tied to the module being certified, hence it is also module-dependent.
// - Result: chip firmware + module NVRAM data + module CLM blob

#pragma pack(push, 1)
// all little endian

struct cdc_hdr {
  uint32_t cmd;   // ioctl command value
  uint16_t olen;  // output buflen
  uint16_t ilen;  // input buflen (excludes header)
  uint32_t flags;
  uint32_t status;
};

struct bdc_hdr {
  uint8_t flags;     // Flags
  uint8_t priority;  // 802.1d Priority (low 3 bits)
  uint8_t flags2;
  uint8_t data_offset;  // Offset from end of BDC header to packet data, in
                        // 4-uint8_t words. Leaves room for optional headers.
};

struct sdpcm_sw_hdr {
  uint8_t sequence;           // Sequence number of pkt
  uint8_t channel_and_flags;  // IOCTL/IOVAR or User Data or Event
  uint8_t next_length;
  uint8_t header_length;  // Offset to BDC or CDC header
  uint8_t wireless_flow_control;
  uint8_t bus_data_credit;  // Credit from WLAN Chip
  uint8_t _reserved[2];
};

struct sdpcm_hdr {
  uint16_t len;
  uint16_t _len;  // ~len
  struct sdpcm_sw_hdr sw_hdr;
};

struct data_hdr {
  struct sdpcm_hdr sdpcm;
  uint8_t pad[2];
  struct bdc_hdr bdc;
};

// gSPI, DS 4.2.1 Fig.12, 2-bit field
#define CYW_SD_FUNC_BUS 0   // F0
#define CYW_SD_FUNC_CHIP 1  // F1
#define CYW_SD_FUNC_WLAN 2  // F2

#define CYW_SDPCM_CTRL_HDR 0
#define CYW_SDPCM_ASYNC_HDR 1
#define CYW_SDPCM_DATA_HDR 2

#pragma pack(pop)

static uint8_t s_tx_seqno;
static uint32_t txdata[2048 / 4], resp[2048 / 4];

static void cyw_handle_cdc(struct cdc_hdr *cdc, size_t len);
static void cyw_handle_bdc(struct bdc_hdr *bdc, size_t len);
static void cyw_handle_bdc_evnt(struct bdc_hdr *bdc, size_t len);

static size_t cyw_spi_poll(uint8_t *dest);

// High-level comm stuff

static void cyw_poll(void) {
  struct sdpcm_hdr *sdpcm = (struct sdpcm_hdr *) resp;
  unsigned int channel;
  if (cyw_spi_poll((uint8_t *) resp) == 0) return;  // BUS DEPENDENCY
  if ((sdpcm->len ^ sdpcm->_len) != 0xffff || sdpcm->len < sizeof(*sdpcm) ||
      sdpcm->len > 2048 - sizeof(*sdpcm))
    return;
  channel = sdpcm->sw_hdr.channel_and_flags & 0x0F;
  if (channel == CYW_SDPCM_CTRL_HDR) {
    if (sdpcm->len >= sizeof(*sdpcm) + sizeof(struct cdc_hdr)) {
      struct cdc_hdr *cdc =
          (struct cdc_hdr *) ((size_t) sdpcm + sdpcm->sw_hdr.header_length);
      size_t len = sdpcm->len - sdpcm->sw_hdr.header_length;
      cyw_handle_cdc(cdc, len);
    }
  } else if (channel == CYW_SDPCM_DATA_HDR) {
    if (sdpcm->len >= sizeof(*sdpcm) + sizeof(struct bdc_hdr)) {
      struct bdc_hdr *bdc =
          (struct bdc_hdr *) ((size_t) sdpcm + sdpcm->sw_hdr.header_length);
      size_t len = sdpcm->len - sdpcm->sw_hdr.header_length;
      cyw_handle_bdc(bdc, len);
    }
  } else if (channel == CYW_SDPCM_ASYNC_HDR) {
    struct bdc_hdr *bdc =
        (struct bdc_hdr *) ((size_t) sdpcm + sdpcm->sw_hdr.header_length);
    size_t len_ = sdpcm->len - sdpcm->sw_hdr.header_length;
    cyw_handle_bdc_evnt(bdc, len_);
  }  // else silently discard
}

// WLAN frame reception
static void cyw_handle_bdc(struct bdc_hdr *bdc, size_t len) {
  uint8_t *payload = (uint8_t *) &bdc[bdc->data_offset + 1];
  mg_tcpip_qwrite(payload, len - (payload - (uint8_t *) bdc), s_ifp);
}

static size_t cyw_bus_tx(uint32_t *data, uint16_t len);

// WLAN frame transmission
static size_t mg_cyw_tx(unsigned int ifc, void *data, size_t len) {
  struct data_hdr *hdr = (struct data_hdr *) txdata;
  uint16_t txlen = (uint16_t) (len + sizeof(*hdr));
  memset(txdata, 0, sizeof(*hdr));
  memcpy((uint8_t *) txdata + sizeof(*hdr), data, len);
  // TODO(): hdr->bdc.priority = map IP to TOS if supporting QoS/ToS
  hdr->bdc.flags = 2 << 4;  // BDC version 2
  hdr->bdc.flags2 = ifc;    // 0 -> STA, 1 -> AP
  // hdr->bdc.data_offset = 0; // actually zeroed above
  hdr->sdpcm.len = txlen;
  hdr->sdpcm._len = (uint16_t) ~txlen;
  hdr->sdpcm.sw_hdr.sequence = ++s_tx_seqno;
  hdr->sdpcm.sw_hdr.channel_and_flags = CYW_SDPCM_DATA_HDR,
  hdr->sdpcm.sw_hdr.header_length = offsetof(struct data_hdr, bdc);
  return cyw_bus_tx(txdata, txlen);
}

// WLAN event handling
#pragma pack(push, 1)
// all in network order

struct eth_hdr {  // TODO(scaprile) reuse 'eth' in net_builtin.c
  uint8_t dest[6];
  uint8_t src[6];
  uint16_t type;
};

struct bcm_vendor_hdr {
  uint16_t subtype;  // vendor specific: 0x8001
  uint16_t length;   // bytes following this field
  uint8_t version;   // 0
  uint8_t oui[3];    // vendor specific: 0x00 0x10 0x18
  uint16_t usr_subtype;
};

struct bcm_evnt_hdr {
  uint16_t version;  // 1: fields up to ifname; 2: as shown
  uint16_t flags;
  uint32_t event_type;
  uint32_t status;
  uint32_t reason;
  uint32_t auth_type;
  uint32_t datalen;
  uint8_t addr[6];  // Station address (if applicable)
  char ifname[16];
  uint8_t ifidx;
  uint8_t bss_cfg_idx;
};

struct evnt_msg {
  struct eth_hdr eth;
  //  struct vendor_hdr; but we only handle Broadcom (Wi-Fi processor) events
  struct bcm_vendor_hdr bcm;
  struct bcm_evnt_hdr event;
};

#pragma pack(pop)

struct scan_result;
static void cyw_handle_scan_result(uint32_t status, struct scan_result *data,
                                   size_t len);

static void cyw_handle_bdc_evnt(struct bdc_hdr *bdc, size_t len) {
  struct evnt_msg *msg = (struct evnt_msg *) &bdc[bdc->data_offset + 1];
  MG_VERBOSE(("%u bytes event", len));
  if (mg_log_level >= MG_LL_VERBOSE) mg_hexdump((void *) bdc, len);
  if (mg_ntohs(msg->eth.type) != 0x886C || msg->bcm.oui[0] != 0x00 ||
      msg->bcm.oui[1] != 0x10 || msg->bcm.oui[2] != 0x18)
    return;  // discard if not Broadcom
  if (mg_ntohl(msg->event.datalen) <=
      len - ((uint8_t *) msg - (uint8_t *) bdc)) {
    uint32_t event_type = mg_ntohl(msg->event.event_type);
    uint32_t status = mg_ntohl(msg->event.status);
    uint32_t reason = mg_ntohl(msg->event.reason);
    uint16_t flags = mg_ntohs(msg->event.flags);
    MG_DEBUG(("BCM event %lu %lu %lu %p", event_type, status, reason, flags));
    if (event_type == 16 && status == 0) {  // Link
      s_link = flags & 1;
    } else if (event_type == 46 && s_link) {  // PSK sup with link up
      if (status == 6) {                      // Keyed
      } else if ((status == 4 || status == 8 || status == 10) &&
                 reason == 15) {  // Wait M1/M3/G1
        MG_ERROR(("AUTH TIMEOUT"));
        s_auth = false;
      } else {
        MG_ERROR(("AUTH FAILED"));
        s_auth = false;
      }
    } else if (event_type == 3 && status != 6) {  // Auth (not unsolicited)
      if (status == 0) {                          // Success
        s_auth = true;
      } else {
        MG_ERROR(("AUTH TIMEOUT"));
        s_auth = false;
      }
    } else if (event_type == 1) {  // Join
      if (status == 0) {           // Success
        s_join = true;
      } else {
        MG_ERROR(("%s", status == 3 /* No networks */ ? "SSID NOT FOUND"
                                                      : "JOIN FAILED"));
        s_join = false;
        mg_tcpip_call(s_ifp, MG_TCPIP_EV_WIFI_CONNECT_ERR, &status);
      }
    } else if (event_type == 12 || event_type == 5) {  // Disassoc, Deauth
      s_auth = false;
    } else if (event_type == 69) {  // Scan result
      struct scan_result *data = (struct scan_result *) (&msg->event + 1);
      size_t dlen = mg_ntohl(msg->event.datalen);
      if (dlen > len - ((uint8_t *) data - (uint8_t *) bdc)) return;
      cyw_handle_scan_result(status, data, dlen);
    }
  }  // else silently discard
}

static bool cyw_ioctl_get_(unsigned int ifc, unsigned int cmd, void *data,
                           size_t len);
static bool cyw_ioctl_set_(unsigned int ifc, unsigned int cmd, void *data,
                           size_t len);
static bool cyw_ioctl_iovar_get_(unsigned int ifc, char *var, void *data,
                                 size_t len);
static bool cyw_ioctl_iovar_set_(unsigned int ifc, char *var, void *data,
                                 size_t len);
// clang-format off
// convenience: ioctl funcs on default ifc (0), as only AP needs ifc 1
static bool cyw_ioctl_get(unsigned int cmd, void *data, size_t len) { return cyw_ioctl_get_(0, cmd, data, len); }
static bool cyw_ioctl_set(unsigned int cmd, void *data, size_t len) { return cyw_ioctl_set_(0, cmd, data, len); }
static bool cyw_ioctl_iovar_get(char *var, void *data, size_t len) { return cyw_ioctl_iovar_get_(0, var, data, len); }
static bool cyw_ioctl_iovar_set(char *var, void *data, size_t len) { return cyw_ioctl_iovar_set_(0, var, data, len); }
// clang-format on

// Wi-Fi network stuff

// clang-format off
static bool cyw_wifi_connect(char *ssid, char *pass) {
  uint32_t sup_wpa[2] = {0, 1}; // bss index 0 = STA, not open
  static const uint32_t const eapver[2] = {0, (uint32_t) -1}, // accept AP version
                              tmo[2] = {0, 2500};
  uint32_t data[64/4 + 1]; // max pass length: 64 for WPA, 128 for WPA3 SAE
  unsigned int len;
  uint32_t val; 
  val = 4; // security type: 0 for none, 2 for WPA, 4 for WPA2/WPA3, 6 for mixed WPA/WPA2
  // sup_wpa[1] = 0 if not using security
  if (!(cyw_ioctl_set(134 /* SET_WSEC */, (uint8_t *)&val, sizeof(val))
        && cyw_ioctl_iovar_set("bsscfg:sup_wpa", (void *)sup_wpa, sizeof(sup_wpa))
        && cyw_ioctl_iovar_set("bsscfg:sup_wpa2_eapver", (void *)eapver, sizeof(eapver))
        && cyw_ioctl_iovar_set("bsscfg:sup_wpa_tmo", (void *)tmo, sizeof(tmo)))
     ) return false;
  mg_delayms(2); // allow radio firmware to be ready
  // skip if not using auth
  memset(data, 0, sizeof(data));
  len = strlen(pass);
  ((uint16_t *)data)[0] = (uint16_t) len;
  ((uint16_t *)data)[1] = 1; // indicates wireless security key, skip for WPA3 SAE
  memcpy((uint8_t *)data + 2 * sizeof(uint16_t), pass, len); // skip for WPA3 SAE
  if (!cyw_ioctl_set(268 /* SET_WSEC_PMK */, data, sizeof(data))) return false; // skip for WPA3 SAE, sizeof/2 if supporting SAE but using WPA
  // for WPA3 SAE: memcpy((uint8_t *)data + sizeof(uint16_t), pass, len); cyw_ioctl_iovar_set("sae_password", data, sizeof(data));
  // resume if not using auth
  val = 1; if (!cyw_ioctl_set(20 /* SET_INFRA */, (uint8_t *)&val, sizeof(val))) return false;
  val = 0; // auth type: 0 for open, 3 for SAE
  if (!cyw_ioctl_set(22 /* SET_AUTH */, (uint8_t *)&val, sizeof(val))) return false;
  val = 1; // MFP capable: 1 for yes, 0 for no; recommended to be set for WPA2+ (2 for 'required', WPA3)
  cyw_ioctl_iovar_set("mfp", (uint8_t *)&val, sizeof(val)); // Old chipsets do not support MFP
  val = 0x80; // auth type: 0 for none, 4 for WPA PSK, 0x80 for WPA2 PSK, 0x40000 for WPA3 SAE PSK
  if (!cyw_ioctl_set(165 /* SET_WPA_AUTH */, (uint8_t *)&val, sizeof(val))) return false;
  len = strlen(ssid);
  data[0] = (uint32_t) len;
  memcpy((uint8_t *)&data[1], ssid, len);
  if (!cyw_ioctl_set(26 /* SET_SSID */, data, len + sizeof(uint32_t))) return false;
  return true;
}

static bool cyw_wifi_disconnect(void) {
  return cyw_ioctl_set(52 /* DISASSOC */, NULL, 0);
}

// For AP functions, we use explicit ifc selection; both for clarity and maintenance, as some actions are performed on ifc 0, with or without a bss_index, and others are performed on ifc 1

static bool cyw_wifi_ap_start(char *ssid, char *pass, unsigned int channel) {
  uint32_t data[64/4 + 2]; // max pass length: 64 for WPA, 128 for WPA3 SAE
  unsigned int len;
  uint32_t val; 
  // CHIP DEPENDENCY
  // RPi set the AMPDU parameter for AP (window size = 2) *****************
  // val = 2 ; cyw_ioctl_iovar_set_(0, "ampdu_ba_wsize", (uint8_t *)&val, sizeof(val));
  // some chips might require to turn APSTA off and issue a SET_AP IOCTL
  len = strlen(ssid);
  data[0] = 1; // bss index 1 = AP
  data[1] = (uint32_t) len;
  memcpy((uint8_t *)&data[2], ssid, len);
  // TODO(scaprile): this takes some time to process, or requires a delay before doing it
  if (!cyw_ioctl_iovar_set_(0, "bsscfg:ssid", (uint8_t *)&data, len + 2 * sizeof(uint32_t))) return false;
  // TODO(scaprile): but sometimes this one takes some time to process
  val = (uint32_t) channel; if (!cyw_ioctl_set_(0, 30 /* SET_CHANNEL */, (uint8_t *)&val, sizeof(val))) return false;
  data[0] = 1; // bss index 1 = AP
  data[1] = 0x00400004; // security type: 0 for none, 0x00200002 for WPA, 0x00400004 for WPA2, 0x01000004 for WPA3, 0x01400004 for mixed WPA2/WPA3, 0x00400006 for mixed WPA/WPA2
  // NOTE(): WHD writes & 0xFF if WPS is not enabled (?)
  if (!cyw_ioctl_iovar_set_(0, "bsscfg:wsec", (uint8_t *)&data, 2 * sizeof(uint32_t))) return false;
  val = 1; // MFP capable: 1 for yes, 0 for no; recommended to be set for WPA2+ (2 for 'required', WPA3)
  cyw_ioctl_iovar_set_(1, "mfp", (uint8_t *)&val, sizeof(val)); // Old chipsets do not support MFP
  mg_delayms(2); // allow radio firmware to be ready
  // skip if not using auth
  // WPA, WPA2, mixed WPA/WPA2, mixed WPA2/WPA3
  // NOTE(): WHD does not set SAE password for shared WPA2/WPA3, same do we
  memset(data, 0, sizeof(data));
  len = strlen(pass);
  ((uint16_t *)data)[0] = (uint16_t) len; // skip for WPA3 SAE
  ((uint16_t *)data)[1] = 1; // indicates wireless security key, skip for WPA3 SAE
  memcpy((uint8_t *)data + 2 * sizeof(uint16_t), pass, len); // skip for WPA3 SAE
  if (!cyw_ioctl_set_(1, 268 /* SET_WSEC_PMK */, data, sizeof(data))) return false; // skip for WPA3 SAE, sizeof/2 if supporting SAE but using WPA
  /* for WPA3 SAE: 
    memcpy((uint8_t *)data + sizeof(uint16_t), pass, len);
    cyw_ioctl_iovar_set_(1, "sae_password", data, sizeof(data)); */
  /* for WPA3 or mixed WPA2/WPA3: 
    val = 5 ; cyw_ioctl_iovar_set_(1, "sae_max_pwe_loop", (uint8_t *)&val, sizeof(val));  // Some chipsets do not support this */
  // resume if not using auth

  data[0] = 1; // bss index 1 = AP
  data[1] = 0x80; // auth type: 0 for none, 4 for WPA PSK, 0x80 for WPA2 PSK, 0x40000 for WPA3 SAE PSK; ored if mixed
  if (!cyw_ioctl_iovar_set_(0, "bsscfg:wpa_auth", (uint8_t *)&data, 2 * sizeof(uint32_t))) return false;

  val = 1 /* auto */; if (!cyw_ioctl_set_(1, 110 /* SET_GMODE */, (uint8_t *)&val, sizeof(val))) return false;
  // Set multicast tx rate to 11Mbps, may fail in some chipsets, we are enforcing it
  val = 11000000 / 500000; if (!cyw_ioctl_iovar_set_(1, "2g_mrate", (uint8_t *)&val, sizeof(val))) return false;
  val = 1; if (!cyw_ioctl_set_(1, 78 /* SET_DTIMPRD */, (uint8_t *)&val, sizeof(val))) return false;
  data[0] = 1; // bss index 1 = AP
  data[1] = 1; // UP
  // TODO(scaprile): this takes a long time to process
  if (!cyw_ioctl_iovar_set_(0, "bss", (uint8_t *)&data, 2 * sizeof(uint32_t))) return false;
  return true;
}

static bool cyw_wifi_ap_stop(void) {
  uint32_t data[2];
  data[0] = 1; // bss index 1 = AP
  data[1] = 0; // DOWN
  if (!cyw_ioctl_iovar_set_(0, "bss", (uint8_t *)&data, 2 * sizeof(uint32_t))) return false;
  // DO WE NEED TO CLEAR CHANNEL ???
  // CHIP DEPENDENCY
  //val = 8 ; cyw_ioctl_iovar_set_(0, "ampdu_ba_wsize", (uint8_t *)&val, sizeof(val));
  return true;
}

// WLAN scan handling

#pragma pack(push, 1)
// in little endian

struct wifi_scan_opt {
    uint32_t version;
    uint16_t action;
    uint16_t _;
    uint32_t ssid_len;
    uint8_t ssid[32];
    uint8_t bssid[6];
    int8_t bss_type;
    int8_t scan_type;
    int32_t nprobes;
    int32_t active_time;
    int32_t passive_time;
    int32_t home_time;
    int32_t channel_num;
    uint16_t channel_list[1];
};
#pragma pack(pop)

static bool cyw_wifi_scan(void) {
  struct wifi_scan_opt opts;
  memset(&opts, 0, sizeof(opts));
  opts.version = 1;
  opts.action = 1; // start
  opts._ = 0;
  memset(opts.bssid, 0xff, sizeof(opts.bssid));
  opts.bss_type = 2; // any
  opts.nprobes = -1;
  opts.active_time = -1;
  opts.passive_time = -1;
  opts.home_time = -1;
  opts.channel_num = 0;
  opts.channel_list[0] = 0;
  return cyw_ioctl_iovar_set("escan", (uint8_t *)&opts, sizeof(opts));
}


#pragma pack(push, 1)
// in little endian

struct scan_bss {
    uint32_t version;              // version field
    uint32_t length;               // byte length of data in this record, starting at version and including IEs
    uint8_t BSSID[6];              // Unique 6-byte MAC address
    uint16_t beacon_period;        // Interval between two consecutive beacon frames. Units are Kusec
    uint16_t capability;           // Capability information
    uint8_t SSID_len;              // SSID length
    uint8_t SSID[32];              // Array to store SSID
    uint8_t reserved1[1];          // Reserved(padding)
    uint32_t rateset_count;        // Count of rates in this set
    uint8_t rateset_rates[16];     // rates in 500kbps units, higher bit set if basic
    uint16_t chanspec;             // Channel specification for basic service set
    uint16_t atim_window;          // Announcement traffic indication message window size. Units are Kusec
    uint8_t dtim_period;           // Delivery traffic indication message period
    uint8_t reserved2[1];          // Reserved(padding)
    int16_t RSSI;                  // receive signal strength (in dBm)
    int8_t phy_noise;              // noise (in dBm)
    uint8_t n_cap;                 // BSS is 802.11n Capable
    uint8_t reserved3[2];          // Reserved(padding)
    uint32_t nbss_cap;             // 802.11n BSS Capabilities (based on HT_CAP_*)
    uint8_t ctl_ch;                // 802.11n BSS control channel number
    uint8_t reserved4[3];          // Reserved(padding)
    uint32_t reserved32[1];        // Reserved for expansion of BSS properties
    uint8_t flags;                 // flags
    uint8_t vht_cap;               // BSS is vht capable
    uint8_t reserved5[2];          // Reserved(padding)
    uint8_t basic_mcs[16];         // 802.11N BSS required MCS set
    uint16_t ie_offset;            // offset at which IEs start, from beginning
    uint16_t reserved16[1];        // Reserved(padding)
    uint32_t ie_length;            // byte length of Information Elements
    int16_t SNR;                   // Average SNR during frame reception
};

struct scan_result {
    uint32_t buflen;
    uint32_t version;
    uint16_t sync_id;
    uint16_t bss_count;
    struct scan_bss bss[1];
};

#pragma pack(pop)

// CHIP DEPENDENCY
#define CYW_BSS_BANDMASK 0xc000
#define CYW_BSS_BAND2G 0
//

static void cyw_handle_scan_result(uint32_t status, struct scan_result *data, size_t len) {
  MG_VERBOSE(("scan event, status: %ld", status));
  if (status == 0) { // SUCCESS
    MG_VERBOSE(("scan complete"));
    mg_tcpip_call(s_ifp, MG_TCPIP_EV_WIFI_SCAN_END, NULL);
  } else if (status == 8) { // PARTIAL
    struct mg_wifi_scan_bss_data bss;
    struct scan_bss *sbss = data->bss;
    unsigned int band = sbss->chanspec & CYW_BSS_BANDMASK;
    if (data->version != 109 || data->bss_count != 1) {
      MG_ERROR(("Unsupported: %lu %u", data->version, data->bss_count));
      return;
    }
    if (sbss->length > len - offsetof(struct scan_result, bss) || sbss->SSID_len > sizeof(sbss->SSID) || sbss->ie_offset < sizeof(*sbss) || sbss->ie_offset > (sizeof(*sbss) + sbss->ie_length) || sbss->ie_offset + sbss->ie_length > sbss->length)
      return; // silently discard malformed data
    if (!(sbss->flags & MG_BIT(2))) return; // RSSI_ONCHANNEL, ignore off-channel results
    bss.SSID = mg_str_n(sbss->SSID, sbss->SSID_len);
    bss.BSSID = sbss->BSSID;
    bss.RSSI = (int8_t)sbss->RSSI;
    bss.has_n = sbss->n_cap != 0;
    bss.channel = bss.has_n ? sbss->ctl_ch : (uint8_t)(sbss->chanspec & 0xff); // n 40MHz vs a/b/g and 20MHz
    bss.band = band & CYW_BSS_BAND2G ? MG_WIFI_BAND_2G : MG_WIFI_BAND_5G;
    bss.security = (sbss->capability & MG_BIT(4) /* CAP_PRIVACY */) ? MG_WIFI_SECURITY_WEP : MG_WIFI_SECURITY_OPEN;
    { // travel IEs (Information Elements) in search of security definitions
      const uint8_t wot1[4] = {0x00, 0x50, 0xf2, 0x01}; // WPA_OUI_TYPE1                     
      uint8_t *ie = (uint8_t *)sbss + sbss->ie_offset;
      int bytes = (int) sbss->ie_length;
      while (bytes > 0 && ie[1] + 2 < bytes) { // ie[0] -> type, ie[1] -> bytes from ie[2]
        if (ie[0] == 48 /* IE_ID_RSN */) bss.security |= MG_WIFI_SECURITY_WPA2;
        if (ie[0] == 221 /* IE_ID_VENDOR_SPECIFIC */ && memcmp(&ie[2], wot1, 4) == 0)
          bss.security |= MG_WIFI_SECURITY_WPA;
        ie += ie[1] + 2;
        bytes -= ie[1] + 2;
      }
    }
    MG_VERBOSE(("BSS: %.*s (%u) (%M) %d dBm %u", bss.SSID.len, bss.SSID.buf, bss.channel, mg_print_mac, bss.BSSID, (int) bss.RSSI, bss.security));  
    mg_tcpip_call(s_ifp, MG_TCPIP_EV_WIFI_SCAN_RESULT, &bss);
  } else {
    MG_ERROR(("scan error"));
  } 
}
// clang-format on

// IOCTL stuff. All values read and written are in little endian format

static uint16_t s_ioctl_reqid;

// CDC handler for waiting loop
static uint8_t *s_ioctl_resp;
static bool s_ioctl_err;

static void cyw_handle_cdc(struct cdc_hdr *cdc, size_t len) {
  uint8_t *resp = (uint8_t *) cdc + sizeof(*cdc);
  MG_VERBOSE(("%u bytes CDC frame", len));
  if ((cdc->flags >> 16) != s_ioctl_reqid) return;
  if (cdc->flags & 1) {
    MG_ERROR(("IOCTL error: %ld", -cdc->status));
    s_ioctl_err = true;
    return;
  }
  if (mg_log_level >= MG_LL_VERBOSE) mg_hexdump((void *) cdc, len);
  MG_DEBUG(("IOCTL result: %02x %02x %02x %02x ...", resp[0], resp[1], resp[2],
            resp[3]));
  s_ioctl_resp = resp;
}
// NOTE(): alt no loop handler dispatching IOCTL response to current handler:
// static void *s_ioctl_hnd; *s_ioctl_hnd(ioctl, len);
// app is a state machine calling get/sets and advancing via these callbacks

#pragma pack(push, 1)
// all little endian

struct ctrl_hdr {
  struct sdpcm_hdr sdpcm;
  struct cdc_hdr cdc;
};

#pragma pack(pop)

// IOCTL command send
static void cyw_ioctl_send_cmd(unsigned int ifc, unsigned int cmd, bool set,
                               size_t len) {
  struct ctrl_hdr *hdr = (struct ctrl_hdr *) txdata;
  uint16_t txlen = (uint16_t) (len + sizeof(*hdr));
  memset(txdata, 0, sizeof(*hdr));
  hdr->cdc.cmd = cmd;
  hdr->cdc.olen = (uint16_t) len;
  // hdr->cdc.ilen = 0; // actually zeroed above
  hdr->cdc.flags = ((uint32_t) ++s_ioctl_reqid << 16) | ((ifc & 0xf) << 12) |
                   (set ? MG_BIT(1) : 0);
  hdr->sdpcm.len = txlen;
  hdr->sdpcm._len = (uint16_t) ~txlen;
  hdr->sdpcm.sw_hdr.sequence = ++s_tx_seqno;
  hdr->sdpcm.sw_hdr.channel_and_flags = CYW_SDPCM_CTRL_HDR;
  hdr->sdpcm.sw_hdr.header_length = offsetof(struct ctrl_hdr, cdc);
  cyw_bus_tx(txdata, txlen);
}

// just send respective commands, response handled via CDC handler
static void cyw_ioctl_send_get(unsigned int ifc, unsigned int cmd) {
  cyw_ioctl_send_cmd(ifc, cmd, false, 0);
}

static void cyw_ioctl_send_set(unsigned int ifc, unsigned int cmd, void *data,
                               size_t len) {
  if (data != NULL && len > 0)
    memcpy((uint8_t *) txdata + sizeof(struct ctrl_hdr), data, len);
  cyw_ioctl_send_cmd(ifc, cmd, true, (uint16_t) len);
}

static void cyw_ioctl_send_iovar_get(unsigned int ifc, char *var, size_t len) {
  unsigned int namelen = strlen(var) + 1;  // include '\0'
  // cmd = GET IOVAR, "set" the name...
  cyw_ioctl_send_set(ifc, 262, var, len > namelen ? len : namelen);
}

static void cyw_ioctl_send_iovar_set2(unsigned int ifc, char *var, void *data1,
                                      size_t len1, void *data2, size_t len2) {
  struct ctrl_hdr *hdr = (struct ctrl_hdr *) txdata;
  unsigned int namelen = strlen(var) + 1;  // include '\0'
  uint16_t txlen, payload_len = (uint16_t) (namelen + len1 + len2);
  memcpy((uint8_t *) txdata + sizeof(*hdr), var, namelen);
  memcpy((uint8_t *) txdata + namelen + sizeof(*hdr), data1, len1);
  if (data2 != NULL)
    memcpy((uint8_t *) txdata + namelen + sizeof(*hdr) + len1, data2, len2);
  txlen = (uint16_t) (payload_len + sizeof(*hdr));
  cyw_ioctl_send_cmd(ifc, 263, true, txlen);  // cmd = SET IOVAR
}

static void cyw_ioctl_send_iovar_set(unsigned int ifc, char *var, void *data,
                                     size_t len) {
  cyw_ioctl_send_iovar_set2(ifc, var, data, len, NULL, 0);
}

// wait for a response, meanwhile delivering received frames and events
static bool cyw_ioctl_wait(void) {
  unsigned int times = 6000;
  s_ioctl_resp = NULL;
  s_ioctl_err = false;
  while (s_ioctl_resp == NULL && !s_ioctl_err && times-- > 0)
    cyw_poll();  // TODO(scaprile): review wait/sleep strategy (this loop is executed only when initializing/acting on the chip)
  MG_DEBUG(("resp: %lp, err: %c, times: %d", s_ioctl_resp,
            s_ioctl_err ? '1' : '0', (int) times));
  return s_ioctl_resp != NULL;
}

static bool cyw_ioctl_waitdata(void *data, size_t len) {
  if (!cyw_ioctl_wait()) return false;
  memcpy(data, s_ioctl_resp, len);
  return true;
}

// send respective commands, wait for a response or timeout
static bool cyw_ioctl_get_(unsigned int ifc, unsigned int cmd, void *data,
                           size_t len) {
  cyw_ioctl_send_get(ifc, cmd);
  return cyw_ioctl_waitdata(data, len);
}
static bool cyw_ioctl_set_(unsigned int ifc, unsigned int cmd, void *data,
                           size_t len) {
  cyw_ioctl_send_set(ifc, cmd, data, len);
  return cyw_ioctl_wait();
}

static bool cyw_ioctl_iovar_get_(unsigned int ifc, char *var, void *data,
                                 size_t len) {
  cyw_ioctl_send_iovar_get(ifc, var, len);
  return cyw_ioctl_waitdata(data, len);
}
static bool cyw_ioctl_iovar_set2_(unsigned int ifc, char *var, void *data1,
                                  size_t len1, void *data2, size_t len2) {
  cyw_ioctl_send_iovar_set2(ifc, var, data1, len1, data2, len2);
  return cyw_ioctl_wait();
}
static bool cyw_ioctl_iovar_set_(unsigned int ifc, char *var, void *data,
                                 size_t len) {
  return cyw_ioctl_iovar_set2_(ifc, var, data, len, NULL, 0);
}

// CYW43 chipset specifics. All values read and written are in little endian
// format

#pragma pack(push, 1)
// all little endian

struct cyw_country {
  uint32_t a;
  int32_t rev;
  uint32_t c;
};

struct clm_hdr {
  uint16_t flag;
  uint16_t type;
  uint32_t len;
  uint32_t crc;
};

#pragma pack(pop)

// worlwide rev0, try rev 17 for 4343W
static const uint32_t country_code = 'X' + ('X' << 8) + (0 << 16);

static bool cyw_spi_init();

// clang-format off
static bool cyw_init(uint8_t *mac) {
  uint32_t val = 0;
  if (!cyw_spi_init()) return false; // BUS DEPENDENCY
  // BT-ENABLED DEPENDENCY
  // set Wi-Fi up
  val = 0 /* disable */; cyw_ioctl_iovar_set("bus:txglom", (uint8_t *)&val, sizeof(val));
  val = 1 /* on */; cyw_ioctl_iovar_set("apsta", (uint8_t *)&val, sizeof(val));
  // CHIP DEPENDENCY
  val = 8 ; cyw_ioctl_iovar_set("ampdu_ba_wsize", (uint8_t *)&val, sizeof(val));
  val = 4 ; cyw_ioctl_iovar_set("ampdu_mpdu", (uint8_t *)&val, sizeof(val));
  val = 0 /* 8K */; cyw_ioctl_iovar_set("ampdu_rx_factor", (uint8_t *)&val, sizeof(val));  
  //
  {
    struct cyw_country c;
    unsigned int rev = (unsigned int) (country_code >> 16) & 0xffff;
    c.c = c.a = country_code & 0xffff;
    c.rev = rev == 0 ? -1 : (int32_t) rev; // if rev is 0, set it to -1, the chip will use any NVRAM/OTP configured aggregate or default to rev 0
    cyw_ioctl_iovar_set("country", (void *)&c, sizeof(c));
  } // this takes some time to process
  { // so do some retries while enabling events of interest
    // we care for SET_SSID(0), JOIN(1), AUTH(3), DEAUTH(5), DISASSOC_IND(12), LINK(16), PSK_SUP(46), SCAN_RESULT(69); all < 128
    uint32_t data[128/8/4 + 1];
    data[0] = 0; // bss index: 0 = STA
    memset(&data[1], 0, 128/8); // mark all as not desired
    data[1] = MG_BIT(0) | MG_BIT(1) | MG_BIT(3) | MG_BIT(5) | MG_BIT(12) | MG_BIT(16); // events 0 to 31
    data[2] = MG_BIT(46 - 32); // events 32 to 63
    data[3] = MG_BIT(69 - 64); // events 64 to 95
    unsigned int times = 100;
    while (times --)
      if (cyw_ioctl_iovar_set("bsscfg:event_msgs", (uint8_t *)data, sizeof(data))) break;
    if (times == ~0) return false;
  }
  val = 0; if (!cyw_ioctl_set(64 /* SET_ANTDIV */, (uint8_t *)&val, sizeof(val))) return false;
  if (!cyw_ioctl_set(2 /* UP, interface up */, NULL, 0)) return false;
  // use PM2 power saving for max throughput
  val = 200 /* ms */; if (!cyw_ioctl_iovar_set("pm2_sleep_ret", (uint8_t *)&val, sizeof(val))) return false;
  // set beacon intervals to reduce power consumption while associated to an AP but idle
  val = 1; if (!cyw_ioctl_iovar_set("bcn_li_bcn", (uint8_t *)&val, sizeof(val))) return false;
  val = 1; if (!cyw_ioctl_iovar_set("bcn_li_dtim", (uint8_t *)&val, sizeof(val))) return false;
  val = 10; if (!cyw_ioctl_iovar_set("assoc_listen", (uint8_t *)&val, sizeof(val))) return false;
  val = 1 /* auto */; if (!cyw_ioctl_set(110 /* SET_GMODE */, (uint8_t *)&val, sizeof(val))) return false;
  val = 0 /* any */; if (!cyw_ioctl_set(142 /* SET_BAND */, (uint8_t *)&val, sizeof(val))) return false;
  if (mg_log_level >= MG_LL_DEBUG) {
    char text[256]; // this is huge, but we're just starting up
    if (cyw_ioctl_iovar_get("ver", (uint8_t *)text, sizeof(text))) {
      unsigned int len = strnlen(text, sizeof(text));
      MG_DEBUG(("Firmware:\n%.*s", len, text));
    }
    text[0] = '\0';
    if (cyw_ioctl_iovar_get("clmver", (uint8_t *)text, sizeof(text)) && text[0] != '\0') {
      unsigned int len = strnlen(text, sizeof(text));
      MG_DEBUG(("CLM:\n%.*s", len, text));
    }
  }
  {
    if(cyw_ioctl_iovar_get("cur_etheraddr", mac, 6)) {
      MG_DEBUG(("MAC: %M", mg_print_mac, mac));
    } else {
      MG_ERROR(("read MAC failed"));
    }
  }
  return true;
}
// clang-format on

static bool cyw_load_fwll(void *fwdata, size_t fwlen, void *nvramdata,
                          size_t nvramlen);

static bool cyw_load_firmware(struct mg_tcpip_driver_cyw_firmware *fw) {
  return cyw_load_fwll((void *) fw->code_addr, fw->code_len,
                       (void *) fw->nvram_addr, fw->nvram_len);
}

// clang-format off
static bool cyw_load_clmll(void *data, size_t len) {
  unsigned int sent = 0, offset = 0;
  struct clm_hdr hdr = {
      .flag = 1 << 12 /* DLOAD_HANDLER_VER */ | MG_BIT(1) /* DL_BEGIN */,
      .type = 2,
      .crc = 0};
  while (sent < len) {
    unsigned int bytes = len - sent;
    if (bytes > 1024) bytes = 1024;
    if (sent + bytes >= len) hdr.flag |= MG_BIT(2);  // DL_END;
    hdr.len = bytes;
    if (!cyw_ioctl_iovar_set2_(0, "clmload", (void *) &hdr, sizeof(hdr), (uint8_t *) data + offset, bytes))
      break;
    sent += bytes;
    offset += bytes;
    hdr.flag &= ~MG_BIT(1);  // DL_BEGIN
  }
  return sent >= len;
}
// clang-format on

static bool cyw_load_clm(struct mg_tcpip_driver_cyw_firmware *fw) {
  return cyw_load_clmll((void *) fw->clm_addr, fw->clm_len);
}

// CYW43 chip backplane specifics. All values read and written are in little
// endian format

// Access to chip backplane is done windowed in 32KB banks
// - addr = area base address + register offset
// - set the window address to addr & ~ADDRMSK
// - access addr & ADDRMSK for non-32-bit quantities
// - if accesing 32-bit quantities, do it on (addr & ADDRMSK) | ACCSS4B
#define CYW_CHIP_CHIPCOMMON 0x18000000
#define CYW_CHIP_BCKPLN_WINSZ 0x8000
#define CYW_CHIP_BCKPLN_ADDRMSK 0x7fff
#define CYW_CHIP_BCKPLN_ACCSS4B MG_BIT(15)
#define CYW_CHIP_BCKPLN_WRAPPOFF 0x100000
#define CYW_CHIP_BCKPLN_SPIMAX 64
#define CYW_CHIP_BCKPLN_SDIOMAX 1536

// CHIP DEPENDENCY
#define CYW_CHIP_ARMCORE_BASE (CYW_CHIP_CHIPCOMMON + 0x3000)
#define CYW_CHIP_SOCSRAM_BASE (CYW_CHIP_CHIPCOMMON + 0x4000)
#define CYW_CHIP_ARMCORE (CYW_CHIP_ARMCORE_BASE + CYW_CHIP_BCKPLN_WRAPPOFF)
#define CYW_CHIP_SOCSRAM (CYW_CHIP_SOCSRAM_BASE + CYW_CHIP_BCKPLN_WRAPPOFF)
#define CYW_CHIP_ATCMRAM_BASE 0
#define CYW_CHIP_RAM_SIZE 0x80000
//

#define CYW_CHIP_ADDRLOW 0x1000a
#define CYW_CHIP_ADDRMID 0x1000b
#define CYW_CHIP_ADDRHIGH 0x1000c
#define CYW_CHIP_SPIFRCTRL 0x1000d
#define CYW_CHIP_CLOCKCSR 0x1000e
#define CYW_CHIP_PULLUP 0x1000f
#define CYW_CHIP_WAKEUPCTL 0x1001e
#define CYW_CHIP_SLEEPCSR 0x1001f

#define CYW_CHIP_SOCSRAM_BANKXIDX 0x010
#define CYW_CHIP_SOCSRAM_BANKXPDA 0x044
#define CYW_CHIP_AI_IOCTRL 0x408
#define CYW_CHIP_AI_RESETCTRL 0x800

static bool cyw_spi_write(unsigned int f, uint32_t addr, void *data,
                          uint16_t len);
static void cyw_spi_read(unsigned int f, uint32_t addr, void *data,
                         uint16_t len);

// clang-format off
// set backplane window to requested area.
static void cyw_set_backplane_window(uint32_t addr) {
  uint32_t val;
  addr &= ~CYW_CHIP_BCKPLN_ADDRMSK;
  val = (addr >> 24) & 0xff; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_ADDRHIGH, &val, 1);
  val = (addr >> 16) & 0xff; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_ADDRMID, &val, 1);
  val = (addr >> 8) & 0xff; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_ADDRLOW, &val, 1);
}

static bool cyw_core_reset(uint32_t core_base, bool check) {
  uint32_t val = 0;
  // core disabled after chip reset
  cyw_set_backplane_window(core_base); // set backplane window for requested area; we do know offsets fall within that window
  // possible CHIP DEPENDENCY: AI_RESETSTATUS check and wait (instead of these cool reads) to ensure backplane operations end
  cyw_spi_read(CYW_SD_FUNC_CHIP, (core_base + CYW_CHIP_AI_IOCTRL) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 1); // ensure backplane operations end
  val = MG_BIT(1) | MG_BIT(0) /* SICF_FGC | SICF_CLOCK_EN */; cyw_spi_write(CYW_SD_FUNC_CHIP, (core_base + CYW_CHIP_AI_IOCTRL) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 1); // reset
  cyw_spi_read(CYW_SD_FUNC_CHIP, (core_base + CYW_CHIP_AI_IOCTRL) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 1); // ensure backplane operations end
  val = 0x00; cyw_spi_write(CYW_SD_FUNC_CHIP, (core_base + CYW_CHIP_AI_RESETCTRL) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 1); // release reset
  mg_delayms(1);
  val = MG_BIT(0) /* SICF_CLOCK_EN */; cyw_spi_write(CYW_SD_FUNC_CHIP, (core_base + CYW_CHIP_AI_IOCTRL) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 1);
  cyw_spi_read(CYW_SD_FUNC_CHIP, (core_base + CYW_CHIP_AI_IOCTRL) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 1); // ensure backplane operations end
  mg_delayms(1);

  if (check) {
    // Verify only clock is enabled
    cyw_spi_read(CYW_SD_FUNC_CHIP, (core_base + CYW_CHIP_AI_IOCTRL) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 1);
    if ((val & (MG_BIT(1) | MG_BIT(0)) /* SICF_FGC | SICF_CLOCK_EN) */) != MG_BIT(0)) return false;
    // Verify it is not in reset state
    cyw_spi_read(CYW_SD_FUNC_CHIP, (core_base + CYW_CHIP_AI_RESETCTRL) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 1);
    if (val & MG_BIT(0)) return false; // AIRC_RESET
  }
  return true;
}

static void cyw_socram_init(void) {
  uint32_t val;
  // CHIP DEPENDENCY: disable remap for SRAM_3
  cyw_set_backplane_window(CYW_CHIP_SOCSRAM_BASE); // set backplane window for requested area; we do know offsets fall within that window
  val = 0x03; cyw_spi_write(CYW_SD_FUNC_CHIP, ((CYW_CHIP_SOCSRAM_BASE + CYW_CHIP_SOCSRAM_BANKXIDX) & CYW_CHIP_BCKPLN_ADDRMSK) | CYW_CHIP_BCKPLN_ACCSS4B, &val, sizeof(val));
  val = 0x00; cyw_spi_write(CYW_SD_FUNC_CHIP, ((CYW_CHIP_SOCSRAM_BASE + CYW_CHIP_SOCSRAM_BANKXPDA) & CYW_CHIP_BCKPLN_ADDRMSK) | CYW_CHIP_BCKPLN_ACCSS4B, &val, sizeof(val));
}

// transfer is fractioned in bus-to-backplane-size units within backplane windows
static void cyw_load_data(uint32_t dest, void *data, size_t len) {
  size_t sent = 0, offset = 0;
  uint32_t last_addr = ~0;
  while (sent < len)  {
    size_t bytes = len - sent, avail;
    uint32_t addr = dest + offset;
    if (addr - last_addr >= CYW_CHIP_BCKPLN_WINSZ || last_addr == ~0) {
      cyw_set_backplane_window(addr); // set backplane window for requested area
      last_addr = addr & ~CYW_CHIP_BCKPLN_ADDRMSK;
    }
    addr &= CYW_CHIP_BCKPLN_ADDRMSK;
    avail = CYW_CHIP_BCKPLN_WINSZ - (unsigned int) addr;
    if (bytes > avail) bytes = avail;
    // BUS DEPENDENCY: max bus to backplane transfer size
    if (bytes > CYW_CHIP_BCKPLN_SPIMAX) bytes = CYW_CHIP_BCKPLN_SPIMAX;
    cyw_spi_write(CYW_SD_FUNC_CHIP, addr | CYW_CHIP_BCKPLN_ACCSS4B, (uint8_t *)data + offset, bytes);
    sent += bytes;
    offset += bytes;
  }
}

// CHIP DEPENDENCY: no SOCSRAM base address; start address in fwdata image (Cortex-R4 chips)
static bool cyw_load_fwll(void *fwdata, size_t fwlen, void *nvramdata, size_t nvramlen) {
  uint32_t val = ((~(nvramlen / 4) & 0xffff) << 16) | (nvramlen / 4); // ~len len in 32-bit words
  cyw_core_reset(CYW_CHIP_SOCSRAM, false);  // cores were disabled at chip reset
  cyw_socram_init();
  cyw_load_data(CYW_CHIP_ATCMRAM_BASE, fwdata, fwlen);
  mg_delayms(5); // ************ CHECK IF THIS IS ACTUALLY NEEDED
  // Load NVRAM and place 'length ~length' at the end; end of chip RAM
  { 
    const uint32_t start = CYW_CHIP_RAM_SIZE - 4 - nvramlen;
    cyw_load_data(start, nvramdata, nvramlen); // nvramlen must be a multiple of 4
    // RAM_SIZE is a multiple of WINSZ, so the place for len ~len will be at the end of the window
    cyw_spi_write(CYW_SD_FUNC_CHIP, (CYW_CHIP_BCKPLN_WINSZ - 4) | CYW_CHIP_BCKPLN_ACCSS4B, &val, sizeof(val));
  }
  // Reset ARM core and check it starts
  if (!cyw_core_reset(CYW_CHIP_ARMCORE, true)) return false;
  return true;
}
// clang-format on

// CYW43 SPI bus specifics

#define CYW_BUS_SPI_BUSCTRL 0x00     // 4 regs, 0 to 3
#define CYW_BUS_SPI_INT 0x04         // 2 regs, 4 to 5
#define CYW_BUS_SPI_INTEN 0x06       // 16-bit register
#define CYW_BUS_SPI_STATUS 0x08      // 32-bit register
#define CYW_BUS_SPI_TEST 0x14        // 32-bit register
#define CYW_BUS_SPI_RESPDLY_F1 0x1d  // 8-bit register, F1: chip

#define CYW_BUS_STS_LEN(x) ((x >> 9) & 0x7ff)

// clang-format off
static size_t cyw_spi_poll(uint8_t *response) {
  size_t len;
  uint32_t res;
  // SPI poll
  cyw_spi_read(CYW_SD_FUNC_BUS, CYW_BUS_SPI_STATUS, &res, sizeof(res));
  if (res == ~0 || !(res & MG_BIT(8) /* packet available */ )) return 0;
  len = CYW_BUS_STS_LEN(res);
  if (len == 0) { // just ack IRQ
    uint16_t val = 1;
    cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_SPIFRCTRL, &val, 1);
    cyw_spi_read(CYW_SD_FUNC_BUS, CYW_BUS_SPI_INT, &val, sizeof(val));
    cyw_spi_write(CYW_SD_FUNC_BUS, CYW_BUS_SPI_INT, &val, sizeof(val));
    return 0;
  }
  cyw_spi_read(CYW_SD_FUNC_WLAN, 0,  response, len);
  return len;
}

// BUS DEPENDENCY: name is generic but function is bus dependent
static size_t cyw_bus_tx(uint32_t *data, uint16_t len) {
  while (len & 3) data[len++] = 0; // SPI 32-bit padding (SDIO->64-byte)
  return cyw_spi_write(CYW_SD_FUNC_WLAN, 0, data, len) ? len: 0;
}

// this can be integrated in lowest level SPI read/write _driver_ functions
// (those calling hal SPI transaction functions), though is only used at start
uint32_t sw16_2(uint32_t data) {
  return ((uint32_t)mg_htons((uint16_t)(data >> 16)) << 16) + mg_htons((uint16_t)data);
}

// DS 4.2.2 Table 6: signal we're working in 16-bit mode
#define CYW_SD_16bMODE MG_BIT(2) // arbitrary bit out of the FUNC space

static bool cyw_spi_init() {
  struct mg_tcpip_driver_cyw_data *d =
      (struct mg_tcpip_driver_cyw_data *) s_ifp->driver_data;
  uint32_t val = 0;
  // DS 4.2.3 Boot-Up Sequence; WHD: other chips might require more effort
  unsigned int times = 51;
  while (times--) {
    cyw_spi_read(CYW_SD_FUNC_BUS | CYW_SD_16bMODE, CYW_BUS_SPI_TEST, &val, sizeof(val));
    if (sw16_2(val) == 0xFEEDBEAD) break;
    mg_delayms(1);
  }
  if (times == ~0) return false;
  // DS 4.2.3 Table 6. Chip starts in 16-bit little-endian mode.
  // Configure SPI and switch to 32-bit big-endian mode:
  // - High-speed mode: d->hs true
  // - IRQ POLARITY high
  // - SPI RESPONSE DELAY 4 bytes time [not in DS] TODO(scaprile): logic ana
  // - Status not sent after command, IRQ with status
  val = sw16_2(0x000204a3 | (d->hs ? MG_BIT(4) : 0)); // 4 reg content
  cyw_spi_write(CYW_SD_FUNC_BUS | CYW_SD_16bMODE, CYW_BUS_SPI_BUSCTRL, &val, sizeof(val));
  mg_tcpip_call(s_ifp, MG_TCPIP_EV_DRIVER, NULL);
  cyw_spi_read(CYW_SD_FUNC_BUS, CYW_BUS_SPI_TEST, &val, sizeof(val));
  if (val != 0xFEEDBEAD) return false;
  val = 4; cyw_spi_write(CYW_SD_FUNC_BUS, CYW_BUS_SPI_RESPDLY_F1, &val, 1);
  val = 0x99; // clear error bits DATA_UNAVAILABLE, COMMAND_ERROR, DATA_ERROR, F1_OVERFLOW
  cyw_spi_write(CYW_SD_FUNC_BUS, CYW_BUS_SPI_INT, &val, 1);
  val = 0x00be; // Enable IRQs F2_F3_FIFO_RD_UNDERFLOW, F2_F3_FIFO_WR_OVERFLOW, COMMAND_ERROR, DATA_ERROR, F2_PACKET_AVAILABLE, F1_OVERFLOW
  // BT-ENABLED DEPENDENCY: add F1_INTR (bit 13)
  cyw_spi_write(CYW_SD_FUNC_BUS, CYW_BUS_SPI_INTEN, &val, sizeof(uint16_t));

  // chip backplane is ready, initialize it
  // request ALP (Active Low Power) clock
  val = MG_BIT(3) /* ALP_REQ */; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_CLOCKCSR, &val, 1);
  // BT-ENABLED DEPENDENCY
  times = 10;
  while (times--) {
    cyw_spi_read(CYW_SD_FUNC_CHIP, CYW_CHIP_CLOCKCSR, &val, 1);
    if (val & MG_BIT(6)) break; // ALP_AVAIL
    mg_delayms(1);
  }
  if (times == ~0) return false;
  // clear request
  val = 0; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_CLOCKCSR, &val, 1);
  cyw_set_backplane_window(CYW_CHIP_CHIPCOMMON); // set backplane window to start of CHIPCOMMON area
  cyw_spi_read(CYW_SD_FUNC_CHIP, (CYW_CHIP_CHIPCOMMON + 0x00) & CYW_CHIP_BCKPLN_ADDRMSK, &val, 2);
  MG_INFO(("WLAN chip is CYW%u", *((uint16_t *)&val)));

  // Load firmware (code and NVRAM)
  if (!cyw_load_firmware(d->fw)) return false;

  // Wait for High Throughput (HT) clock ready
  times = 50;
  while (times--) {
    cyw_spi_read(CYW_SD_FUNC_CHIP, CYW_CHIP_CLOCKCSR, &val, 1);
    if (val & MG_BIT(7)) break; // HT_AVAIL
    mg_delayms(1);
  }
  if (times == ~0) return false;
  // Wait for backplane ready
  times = 1000;
  while (times--) {
    cyw_spi_read(CYW_SD_FUNC_BUS, CYW_BUS_SPI_STATUS, &val, sizeof(val));
    if (val & MG_BIT(5)) break; // F2_RX_READY
    mg_delayms(1);
  }
  if (times == ~0) return false;

  // CHIP DEPENDENCY
  // Enable save / restore
  // Configure WakeupCtrl, set HT_AVAIL in CLOCK_CSR
  cyw_spi_read(CYW_SD_FUNC_CHIP, CYW_CHIP_WAKEUPCTL, &val, 1);
  val |= MG_BIT(1) /* WAKE_TILL_HT_AVAIL */; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_WAKEUPCTL, &val, 1);
#if 0
  // Set BRCM_CARDCAP to CMD_NODEC. NOTE(): This is probably only necessary for SDIO, not SPI
  val = MG_BIT(3); cyw_spi_write(CYW_SD_FUNC_BUS, 0xf0 /* SDIOD_CCCR_BRCM_CARDCAP */, &val, 1);
#endif
  // Force HT request to chip backplane
  val = MG_BIT(1) /* FORCE_HT */; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_CLOCKCSR, &val, 1);
  // Enable Keep SDIO On (KSO)
  cyw_spi_read(CYW_SD_FUNC_CHIP, CYW_CHIP_SLEEPCSR, &val, 1);
  if (!(val & MG_BIT(0))) {
      val |= MG_BIT(0); cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_SLEEPCSR, &val, 1);
  }
  // The SPI bus can be configured for sleep (KSO controls wlan block sleep)
  cyw_spi_read(CYW_SD_FUNC_BUS, CYW_BUS_SPI_BUSCTRL, &val, sizeof(val));
  val &= ~MG_BIT(7) /* WAKE_UP */; cyw_spi_write(CYW_SD_FUNC_BUS, CYW_BUS_SPI_BUSCTRL, &val, sizeof(val));
  // Set SPI bus sleep
  val = 0x0f; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_PULLUP, &val, 1);

  // Clear pullups. NOTE(): ?
  val = 0x00; cyw_spi_write(CYW_SD_FUNC_CHIP, CYW_CHIP_PULLUP, &val, 1);
  cyw_spi_read(CYW_SD_FUNC_CHIP, CYW_CHIP_PULLUP, &val, 1);
  // Clear possible data unavailable error
  cyw_spi_read(CYW_SD_FUNC_BUS, CYW_BUS_SPI_INTEN, &val, sizeof(uint16_t));
  if (val & MG_BIT(0)) cyw_spi_write(CYW_SD_FUNC_BUS, CYW_BUS_SPI_INTEN, &val, sizeof(uint16_t));

  // Load CLM blob
  if (!cyw_load_clm(d->fw)) return false;

  return true;
}
// clang-format on

// gSPI, DS 4.2.1 Fig.12
#define CYW_SD_LEN(x) ((x) &0x7FF)             // bits 0-10
#define CYW_SD_ADDR(x) (((x) &0x1FFFF) << 11)  // bits 11-27,
#define CYW_SD_FUNC(x) (((x) &3) << 28)        // bits 28-29
#define CYW_SD_INC MG_BIT(30)
#define CYW_SD_WR MG_BIT(31)

static bool cyw_spi_write(unsigned int f, uint32_t addr, void *data,
                          uint16_t len) {
  struct mg_tcpip_driver_cyw_data *d =
      (struct mg_tcpip_driver_cyw_data *) s_ifp->driver_data;
  struct mg_tcpip_spi_ *s = (struct mg_tcpip_spi_ *) d->spi;
  uint32_t hdr = CYW_SD_WR | CYW_SD_INC | CYW_SD_FUNC(f) | CYW_SD_ADDR(addr) |
                 CYW_SD_LEN(len);  // gSPI header
  // TODO(scaprile): check spin in between and timeout values, return false
  if (f == CYW_SD_FUNC_WLAN) {
    uint32_t val = 0;
    while ((val & MG_BIT(5)) != MG_BIT(5))  // F2 rx ready (FIFO ready)
      cyw_spi_read(CYW_SD_FUNC_BUS, CYW_BUS_SPI_STATUS, &val, sizeof(val));
  }
  if (f & CYW_SD_16bMODE)
    hdr = sw16_2(hdr);  // swap half-words in 16-bit little-endian mode

  s->begin(NULL);
  s->txn(NULL, (uint8_t *) &hdr, NULL, sizeof(hdr));
  if (len <= 4) {
    uint32_t pad = 0;
    memcpy(&pad, data, len);
    s->txn(NULL, (uint8_t *) &pad, NULL, sizeof(pad));
  } else {
    s->txn(NULL, (uint8_t *) data, NULL, len);
  }
  s->end(NULL);
  return true;
}

// will write 32-bit aligned quantities to data if f == CYW_SD_FUNC_WLAN
static void cyw_spi_read(unsigned int f, uint32_t addr, void *data,
                         uint16_t len) {
  struct mg_tcpip_driver_cyw_data *d =
      (struct mg_tcpip_driver_cyw_data *) s_ifp->driver_data;
  struct mg_tcpip_spi_ *s = (struct mg_tcpip_spi_ *) d->spi;
  uint32_t padding =
      f == CYW_SD_FUNC_CHIP
          ? 4
          : 0;  // add padding to chip backplane reads as a response delay
  uint32_t hdr = CYW_SD_INC | CYW_SD_FUNC(f) | CYW_SD_ADDR(addr) |
                 CYW_SD_LEN(len + padding);  // gSPI header
  if (f == CYW_SD_FUNC_WLAN && (len & 3))
    len = (len + 4) & ~3;  // align WLAN transfers to 32-bit
  if (f & CYW_SD_16bMODE)
    hdr = sw16_2(hdr);  // swap half-words in 16-bit little-endian mode

  s->begin(NULL);
  s->txn(NULL, (uint8_t *) &hdr, NULL, sizeof(hdr));
  if (f == CYW_SD_FUNC_CHIP) {
    uint32_t pad;
    s->txn(NULL, NULL, (uint8_t *) &pad, 4);  // read padding back and discard
  }
  s->txn(NULL, NULL, (uint8_t *) data, len);
  s->end(NULL);
}

// Mongoose Wi-Fi API functions

bool mg_wifi_scan(void) {
  return cyw_wifi_scan();
}

bool mg_wifi_connect(char *ssid, char *pass) {
  return cyw_wifi_connect(ssid, pass);
}

bool mg_wifi_disconnect(void) {
  return cyw_wifi_disconnect();
}

bool mg_wifi_ap_start(char *ssid, char *pass, unsigned int channel) {
  return cyw_wifi_ap_start(ssid, pass, channel);
}

bool mg_wifi_ap_stop(void) {
  return cyw_wifi_ap_stop();
}

#endif
