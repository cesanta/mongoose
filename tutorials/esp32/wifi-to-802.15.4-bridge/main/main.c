// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
// IPv4 Wi-Fi station <-> Mongoose Ethernet-over-802.15.4 radio bridge.
//
// LAN / DHCP server <-- Wi-Fi --> ESP32-C6 <-- 802.15.4 --> radio nodes
//
// The radio side is Mongoose's esp32 driver acting as the bridge for the
// radio nodes, like tutorials/nxp/frdm-rw612/ethernet-to-802.15.4-bridge.
// The radio driver reads LAN frames from the Wi-Fi interface's recv_queue and
// sends frames to the LAN with that interface's driver tx(). Here the LAN
// interface is a thin wrapper around the Wi-Fi station.
//
// A Wi-Fi station can only send with its own MAC address, so unlike Ethernet
// the frames are translated, see translate(): upstream, node MACs are replaced
// with the station MAC; downstream, the node MAC is restored by IP address.
// Each node gets its own LAN IP from DHCP. The bridge gets one too, for its
// own Mongoose: GET / on port 80 returns the bridge's status as JSON.
//
// Threads: wifi_input() runs in the Wi-Fi task and only queues a copy. All
// the rest, radio, translation and Wi-Fi transmit, runs in app_main's loop.
#include "driver/gpio.h"
#include "esp_coexist.h"
#include "esp_event.h"
#include "esp_ieee802154.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_net_stack.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "freertos/FreeRTOS.h"
#include "lwip/esp_netif_net_stack.h"
#include "mongoose.h"
#include "nvs_flash.h"

#if !CONFIG_IDF_TARGET_ESP32C6
#error This tutorial requires ESP32-C6
#endif

// Override with WIFI_NAME and WIFI_PASS environment variables at build time
#ifndef WIFI_NAME
#define WIFI_NAME "WIFI_NAME"
#endif
#ifndef WIFI_PASS
#define WIFI_PASS "WIFI_PASS"
#endif

#define FRAME_SIZE 1514                     // Ethernet header + 1500, no FCS
#define NODE_COUNT 16                       // Radio nodes the bridge tracks
#define NODE_TIMEOUT_MS (30UL * 60 * 1000)  // Forget a node idle this long

// A radio node, learned from its frames to Wi-Fi
struct node {
  uint64_t expires;       // Expiry, milliseconds since boot; zero: unused
  uint8_t mac[6], ip[4];  // The node's own MAC, and its LAN IPv4 address
};

static uint8_t s_mac[6];                 // Wi-Fi station MAC, used by all nodes
static struct node s_nodes[NODE_COUNT];  // Radio nodes, see translate()
static esp_netif_t *s_netif;             // Wi-Fi station, raw Ethernet I/O
static volatile bool s_wifi_up;          // Associated with the access point
static struct mg_tcpip_if s_wifi;        // The radio's LAN interface
static struct mg_tcpip_if s_radio;       // 802.15.4 radio interface
static struct mg_tcpip_if s_local;       // The bridge's own IP, for the status

// Cumulative totals. Up: radio -> Wi-Fi; down: Wi-Fi -> radio; bridge: Wi-Fi
// -> the bridge's own IP
static unsigned s_frames_up, s_frames_down, s_frames_local, s_wifi_drops;
static unsigned s_frames_bridge, s_radio_sent, s_radio_received;

// Is a an IPv4 address worth learning: not 0.x, loopback or multicast
static bool is_host_ip(const uint8_t *a) {
  return a[0] != 0 && a[0] != 127 && a[0] < 224;
}

// Translate an Ethernet frame in place and update s_nodes. Returns false if
// the frame must be dropped. IPv4 and ARP only; IPs and ports stay intact.
// To Wi-Fi: learn the sending node and replace its MAC with the station MAC.
// DHCP keeps the node's chaddr, so each node gets its own LAN IP.
// To radio: find the node by destination IP, or by DHCP chaddr, and restore
// its MAC. Broadcasts pass, unknown unicasts drop
static bool translate(uint8_t *f, size_t len, bool to_radio, uint64_t now) {
  uint8_t *ip = NULL, *a = NULL, *bootp = NULL, *udp = NULL;
  struct node *n = NULL, *free_node = NULL;
  const uint8_t *addr;
  bool by_mac;
  size_t i;
  if (len < 14 || len > FRAME_SIZE || memcmp(f + 6, s_mac, 6) == 0) {
    return false;
  }
  if (!to_radio && ((f[6] & 1) || memcmp(f + 6, "\0\0\0\0\0\0", 6) == 0)) {
    return false;
  }
  if (f[12] == 8 && f[13] == 6) {  // ARP for Ethernet/IPv4, request or reply
    a = f + 14;
    if (len < 42 || memcmp(a, "\x00\x01\x08\x00\x06\x04", 6) != 0 ||
        a[6] != 0 || (a[7] != 1 && a[7] != 2) ||
        (!to_radio && memcmp(a + 8, f + 6, 6) != 0)) {
      return false;
    }
  } else if (f[12] == 8 && f[13] == 0 && len >= 34) {  // IPv4, with options
    size_t hlen, total;
    ip = f + 14;
    hlen = (ip[0] & 15) * 4U, total = ip[2] * 256U + ip[3];
    if ((ip[0] >> 4) != 4 || hlen < 20 || total < hlen || total > len - 14) {
      return false;
    }
    // DHCP: UDP ports 67/68, BOOTP and the magic cookie. Only complete,
    // unfragmented packets
    udp = ip + hlen;
    if (ip[9] == 17 && !(ip[6] & 0x3f) && ip[7] == 0 && total >= hlen + 248) {
      uint8_t *p = udp + 8;
      size_t ulen = udp[4] * 256U + udp[5];
      if (udp[0] == 0 && udp[1] == (to_radio ? 67 : 68) && udp[2] == 0 &&
          udp[3] == (to_radio ? 68 : 67) && ulen >= 248 &&
          ulen <= total - hlen && p[0] == (to_radio ? 2 : 1) && p[1] == 1 &&
          p[2] == 6 && memcmp(p + 236, "\x63\x82\x53\x63", 4) == 0) {
        bootp = p;
      }
    }
  } else {
    return false;
  }

  // The node: to Wi-Fi, by source MAC; to radio, by DHCP chaddr, else by
  // destination IP. Expire old nodes and find a free slot in the same pass
  by_mac = !to_radio || bootp != NULL;
  addr = !to_radio       ? f + 6
         : bootp != NULL ? bootp + 28
         : ip != NULL    ? ip + 16
                         : a + 24;
  for (i = 0; i < NODE_COUNT; i++) {
    struct node *p = &s_nodes[i];
    if (p->expires != 0 && now > p->expires) p->expires = 0;
    if (p->expires == 0) {
      if (free_node == NULL) free_node = p;
    } else if (n == NULL && (by_mac || is_host_ip(addr)) &&
               memcmp(by_mac ? p->mac : p->ip, addr, by_mac ? 6 : 4) == 0) {
      n = p;
    }
  }

  if (!to_radio) {
    if (bootp != NULL && memcmp(bootp + 28, f + 6, 6) != 0) return false;
    if (n == NULL) {
      if ((n = free_node) == NULL) return false;  // Table full
      memset(n, 0, sizeof(*n));
      memcpy(n->mac, f + 6, 6);
    }
    n->expires = now + NODE_TIMEOUT_MS;
    addr = ip != NULL ? ip + 12 : a + 14;  // Sender IP
    if (is_host_ip(addr)) memcpy(n->ip, addr, 4);
    if (a != NULL) memcpy(a + 8, s_mac, 6);  // ARP sender MAC
    if (bootp != NULL) {
      bootp[10] |= 0x80;    // Ask for broadcast replies: no node IP yet
      udp[6] = udp[7] = 0;  // No UDP checksum, IPv4 allows that
    }
    memcpy(f + 6, s_mac, 6);
    return true;
  }

  if (bootp != NULL) {
    if (n == NULL) return false;  // Another LAN client's DHCP reply
    if (is_host_ip(bootp + 16)) memcpy(n->ip, bootp + 16, 4);  // yiaddr
  }
  if (n == NULL) return (f[0] & 1) != 0;  // Broadcast passes, unicast drops
  memcpy(f, n->mac, 6);
  if (a != NULL && a[7] == 2) memcpy(a + 18, n->mac, 6);  // ARP reply target
  return true;
}

// Is mac a radio node, learned by translate()
static bool is_node(const uint8_t *mac, uint64_t now) {
  size_t i;
  for (i = 0; i < NODE_COUNT; i++) {
    if (s_nodes[i].expires > now && memcmp(s_nodes[i].mac, mac, 6) == 0) {
      return true;
    }
  }
  return false;
}

// Radio events: count radio frames, and give each its Wi-Fi coexistence
// priority before it goes out. Wi-Fi and 802.15.4 share one antenna. The
// bridge's unicast frames give a node its turn, and the node answers right
// after: send those at high priority, so the radio keeps the antenna until
// the answer is in. The broadcast beacons go at low priority, and Wi-Fi gets
// the antenna between turns. ESP-IDF applies the priority when the next
// transmission or reception starts
static void radio_fn(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  if (ev == MG_TCPIP_EV_PHY_RX) s_radio_received++;
  if (ev == MG_TCPIP_EV_PHY_TX) {
    struct mg_str *f = (struct mg_str *) ev_data;  // MAC header: dst at 5..6
    const uint8_t *frame = (const uint8_t *) f->buf;
    bool bcast = f->len < 7 || (frame[5] == 0xff && frame[6] == 0xff);
    ieee802154_coex_event_t want = bcast ? IEEE802154_LOW : IEEE802154_HIGH;
    esp_ieee802154_coex_config_t coex = esp_ieee802154_get_coex_config();
    s_radio_sent++;
    if (coex.txrx != want) {
      coex.txrx = want;
      esp_ieee802154_set_coex_config(coex);
    }
  }
  (void) ifp;
}

// ESP-NETIF custom stack input: Ethernet frames from Wi-Fi, before lwIP.
// Runs in the Wi-Fi task. The queue has one producer, this function, and
// one consumer, the radio driver in app_main, so it needs no lock
static esp_err_t wifi_input(void *netif, void *buf, size_t len, void *eb) {
  char *dst;
  if (s_wifi_up && len >= 14 && len <= FRAME_SIZE &&
      mg_queue_book(&s_wifi.recv_queue, &dst, len) >= len) {
    memcpy(dst, buf, len);
    mg_queue_add(&s_wifi.recv_queue, len);
  } else {
    s_wifi_drops++;
  }
  esp_netif_free_rx_buffer(esp_netif_get_handle_from_netif_impl(netif), eb);
  return ESP_OK;
}

// Send an Ethernet frame over Wi-Fi. Returns len, or 0 on failure
static size_t wifi_send(void *buf, size_t len) {
  return s_wifi_up && esp_netif_transmit(s_netif, buf, len) == ESP_OK ? len : 0;
}

// LAN driver tx: the radio driver hands over an Ethernet frame from a radio
// node. A unicast to another node goes back to the radio, untranslated.
// Anything else is translated and sent over Wi-Fi
static size_t wifi_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  uint8_t *frame = (uint8_t *) buf;  // The radio's receive buffer, writable
  uint64_t now = mg_millis();
  (void) ifp;
  if (len >= 14 && (frame[0] & 1) == 0 && is_node(frame, now)) {
    if (mg_tcpip_driver_esp32.tx(buf, len, &s_radio) == 0) return 0;
    s_frames_local++;
    return len;
  }
  if (!translate(frame, len, false, now) || wifi_send(frame, len) == 0) {
    return 0;
  }
  s_frames_up++;
  return len;
}

// Is a Wi-Fi frame for the bridge itself: an ARP or IPv4 packet to its IP, or
// a DHCP reply to its MAC, before it has an IP
static bool for_bridge(const uint8_t *f, size_t len) {
  uint32_t ip = s_local.ip;
  size_t hlen = len > 14 ? (size_t) (f[14] & 15) * 4 : 0;
  const uint8_t *udp = f + 14 + hlen;
  if (len >= 42 && f[12] == 8 && f[13] == 6) {  // ARP: target IP at 38
    return ip != 0 && memcmp(f + 38, &ip, 4) == 0;
  }
  if (len < 34 || f[12] != 8 || f[13] != 0) return false;  // IPv4 only
  if (ip != 0 && memcmp(f + 30, &ip, 4) == 0) return true;  // Destination
  return hlen >= 20 && len >= 14 + hlen + 8 + 34 && f[23] == 17 &&
         udp[0] == 0 && udp[1] == 67 && udp[2] == 0 && udp[3] == 68 &&
         memcmp(udp + 8 + 28, s_local.mac, 6) == 0;  // DHCP chaddr
}

// Hand a Wi-Fi frame to the bridge's own TCP/IP stack
static void to_bridge(const void *buf, size_t len) {
  char *p;
  if (mg_queue_book(&s_local.recv_queue, &p, len) >= len) {
    memcpy(p, buf, len);
    mg_queue_add(&s_local.recv_queue, len);
    s_frames_bridge++;
  }
}

// Radio driver tx, called with frames from the Wi-Fi interface's recv_queue:
// restore the node's MAC, then queue the frame for the radio. Frames for the
// bridge itself go to its own stack; broadcasts go to both
static size_t radio_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  uint8_t *frame = (uint8_t *) buf;  // Points into the recv_queue, writable
  bool bcast = len >= 14 && (frame[0] & 1);
  if (bcast || for_bridge(frame, len)) {
    to_bridge(buf, len);
    if (!bcast) return len;
  }
  if (!translate(frame, len, true, mg_millis())) return 0;
  len = mg_tcpip_driver_esp32.tx(buf, len, ifp);
  if (len != 0) s_frames_down++;
  return len;
}

// The bridge's own interface sends straight to Wi-Fi, under the station MAC
static size_t local_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  (void) ifp;
  return wifi_send((void *) buf, len);
}

// The bridge's own interface is up while Wi-Fi is associated
static bool local_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp, (void) s1;
  return s_wifi_up;
}

// Print the node table as a JSON array, for %M
static size_t print_nodes(mg_pfn_t out, void *arg, va_list *ap) {
  uint64_t now = mg_millis();
  size_t i, n = mg_xprintf(out, arg, "[");
  const char *comma = "";
  (void) ap;
  for (i = 0; i < NODE_COUNT; i++) {
    const struct node *p = &s_nodes[i];
    uint32_t ip;
    if (p->expires <= now) continue;
    memcpy(&ip, p->ip, sizeof(ip));
    n += mg_xprintf(out, arg, "%s{\"mac\":\"%M\",\"ip\":\"%M\",\"idle_ms\":%llu}",
                    comma, mg_print_mac, p->mac, mg_print_ip4, &ip,
                    (unsigned long long) (now + NODE_TIMEOUT_MS - p->expires));
    comma = ",";
  }
  return n + mg_xprintf(out, arg, "]");
}

// GET /: the bridge's status, as JSON
static void http_ev(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    wifi_ap_record_t ap = {0};
    bool assoc = esp_wifi_sta_get_ap_info(&ap) == ESP_OK;
    mg_http_reply(
        c, 200, "Content-Type: application/json\r\n",
        "{\"uptime_ms\":%llu,\"free_heap\":%lu,\"ip\":\"%M\","
        "\"wifi\":{\"up\":%s,\"ssid\":%m,\"rssi\":%d,\"channel\":%u,"
        "\"rx_drops\":%u},"
        "\"radio\":{\"channel\":%d,\"frames_sent\":%u,\"frames_received\":%u,"
        "\"rx_drops\":%u,\"tx_errors\":%u},"
        "\"frames\":{\"up\":%u,\"down\":%u,\"node_to_node\":%u,\"bridge\":%u},"
        "\"nodes\":%M}\n",
        (unsigned long long) mg_millis(),
        (unsigned long) esp_get_free_heap_size(), mg_print_ip4, &s_local.ip,
        s_wifi_up ? "true" : "false",
        MG_ESC(assoc ? (const char *) ap.ssid : ""), assoc ? ap.rssi : 0,
        assoc ? ap.primary : 0, s_wifi_drops, MG_RADIO_CHANNEL,
        s_radio_sent, s_radio_received, (unsigned) s_radio.ndrop,
        (unsigned) s_radio.nerr, s_frames_up, s_frames_down, s_frames_local,
        s_frames_bridge, print_nodes);
  }
  (void) ev_data;
}

// Wi-Fi association state; the bridge's own IP comes from Mongoose's DHCP.
// app_main retries the connection, so this handler only tracks state
static void wifi_event(void *arg, esp_event_base_t base, int32_t id,
                       void *data) {
  if (id == WIFI_EVENT_STA_CONNECTED) {
    s_wifi_up = true;
    MG_INFO(("Wi-Fi connected"));
  } else if (id == WIFI_EVENT_STA_DISCONNECTED) {
    const wifi_event_sta_disconnected_t *e = data;
    s_wifi_up = false;
    MG_INFO(("Wi-Fi disconnected, reason %u", e->reason));
  }
  (void) arg, (void) base;
}

// XIAO RF switch: GPIO3 low powers it, GPIO14 selects the antenna
static void antenna_init(void) {
#ifdef CONFIG_BRIDGE_XIAO_ESP32C6
  ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_3, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_3, 0));
  vTaskDelay(pdMS_TO_TICKS(100));
  ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT));
#ifdef CONFIG_BRIDGE_XIAO_EXTERNAL_ANTENNA
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_14, 1));
#else
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_14, 0));
#endif
#endif
}

// Wi-Fi station whose received frames go to wifi_input(), not to lwIP. The
// bridge's own IP is Mongoose's, see s_local
static void wifi_init(void) {
  esp_netif_inherent_config_t base = ESP_NETIF_INHERENT_DEFAULT_WIFI_STA();
  const esp_netif_netstack_config_t stack = {
      .lwip = {.init_fn = wlanif_init_sta, .input_fn = wifi_input}};
  const esp_netif_config_t cfg = {.base = &base, .stack = &stack};
  wifi_init_config_t init = WIFI_INIT_CONFIG_DEFAULT();
  wifi_config_t wifi = {
      .sta = {.ssid = WIFI_NAME,
              .password = WIFI_PASS,
              .scan_method = WIFI_ALL_CHANNEL_SCAN,
              .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
              .threshold = {.rssi = -127, .authmode = WIFI_AUTH_OPEN}}};
  esp_err_t err = nvs_flash_init();  // Wi-Fi needs NVS, even with RAM storage
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  base.flags = ESP_NETIF_FLAG_AUTOUP;  // No IP, DHCP, GARP or IPv6
  s_netif = esp_netif_new(&cfg);
  if (s_netif == NULL) ESP_ERROR_CHECK(ESP_ERR_NO_MEM);
  ESP_ERROR_CHECK(esp_netif_attach_wifi_station(s_netif));
  ESP_ERROR_CHECK(esp_wifi_set_default_wifi_sta_handlers());
  ESP_ERROR_CHECK(esp_wifi_init(&init));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             wifi_event, NULL));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi));
  ESP_ERROR_CHECK(esp_wifi_get_mac(WIFI_IF_STA, s_mac));
}

void app_main(void) {
  static struct mg_tcpip_driver wifi_driver;
  static struct mg_tcpip_driver radio_driver;
  static struct mg_tcpip_driver local_driver;
  static struct mg_mgr mgr;
  static char wifi_queue[8192];
  uint64_t retry_timer = 0, stats_timer = 0;

  antenna_init();
  wifi_init();

  // The radio uses a locally administered variant of the station MAC
  memcpy(s_radio.mac, s_mac, 6);
  s_radio.mac[0] = (uint8_t) ((s_radio.mac[0] | 2) & 0xfe);
  s_radio.mac[5] ^= 0x80;
  radio_driver = mg_tcpip_driver_esp32;
  radio_driver.tx = radio_tx;
  s_radio.driver = &radio_driver;
  s_radio.fn = radio_fn;  // Send at coexistence priority
  if (!s_radio.driver->init(&s_radio)) ESP_ERROR_CHECK(ESP_FAIL);

  // The LAN interface: the radio driver only uses its recv_queue and tx
  wifi_driver.tx = wifi_tx;
  s_wifi.driver = &wifi_driver;
  memcpy(s_wifi.mac, s_mac, 6);
  mg_queue_init(&s_wifi.recv_queue, wifi_queue, sizeof(wifi_queue));
  mg_radio_bridge(&s_radio, &s_wifi);

  // The bridge's own interface, under the station MAC, with an IP from DHCP:
  // GET / on port 80 returns the bridge's status
  local_driver.tx = local_tx;
  local_driver.poll = local_poll;
  s_local.driver = &local_driver;
  memcpy(s_local.mac, s_mac, 6);
  mg_snprintf(s_local.dhcp_name, sizeof(s_local.dhcp_name), "radio-bridge");
  mg_mgr_init(&mgr);
  mg_tcpip_init(&mgr, &s_local);
  mg_http_listen(&mgr, "http://0.0.0.0:80", http_ev, NULL);

  // Start the radio before Wi-Fi, then enable coexistence. Wi-Fi power
  // saving gives the arbiter time slots for 802.15.4
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_coex_wifi_i154_enable());
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MAX_MODEM));
  ESP_ERROR_CHECK(esp_wifi_connect());
  MG_INFO(
      ("Wi-Fi/radio bridge, SSID %s, channel %u", WIFI_NAME, MG_RADIO_CHANNEL));

  // Busy loop: radio turns are timed in microseconds, a FreeRTOS tick is much
  // longer. Wi-Fi tasks have higher priority and preempt it. The idle task
  // starves, so sdkconfig.defaults turns off the idle task watchdog check
  for (;;) {
    uint64_t now = mg_millis();
    s_radio.driver->poll(&s_radio, false);
    mg_mgr_poll(&mgr, 0);
    if (!s_wifi_up && mg_timer_expired(&retry_timer, 5000, now)) {
      (void) esp_wifi_connect();
    }
    if (mg_timer_expired(&stats_timer, 5000, now)) {
      MG_INFO(("Frames up/down/node-to-node %u/%u/%u, Wi-Fi RX drops %u",
               s_frames_up, s_frames_down, s_frames_local, s_wifi_drops));
    }
  }
}
