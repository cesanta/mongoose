// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "net.h"

// Authenticated user.
// A user can be authenticated by:
//   - a name:pass pair, passed in a header Authorization: Basic .....
//   - an access_token, passed in a header Cookie: access_token=....
// When a user is shown a login screen, she enters a user:pass. If successful,
// a server responds with a http-only access_token cookie set.
struct user {
  const char *name, *pass, *access_token;
};

// Event log entry
struct event {
  int type, prio;
  unsigned long timestamp;
  const char *text;
};

// Connected Devices
struct device {
  char *dev_name;
  char *mac;
  char *ip_addr;
  int speed;
  char *connected_to;
  int lease_time_left;
  char *last_seen;
};

static struct device s_devices[] = {{.dev_name = "espressif",
                                     .mac = "02:11:22:33:44:55",
                                     .ip_addr = "192.168.1.1/24",
                                     .speed = 1000,
                                     .connected_to = "Ethernet",
                                     .lease_time_left = 1000,
                                     .last_seen = "13h20m ago"},

                                    {.dev_name = "windows11",
                                     .mac = "01:22:11:44:33:55",
                                     .ip_addr = "192.168.1.2/24",
                                     .speed = 200,
                                     .connected_to = "Wifi 2.4 GHz",
                                     .lease_time_left = 4141,
                                     .last_seen = "23s ago"},

                                    {.dev_name = "iRobot-2",
                                     .mac = "01:22:11:44:33:42",
                                     .ip_addr = "192.168.1.3/24",
                                     .speed = 600,
                                     .connected_to = "Wifi 5GHz",
                                     .lease_time_left = 1141,
                                     .last_seen = "20m ago"}};

// DHCP configuration
struct dhcp {
  bool enabled;
  uint8_t address_begin;
  uint8_t address_end;
  unsigned long lease_time_sec;
};

static struct dhcp s_dhcp = {true, 10, 250, 86400};

// Mocked events
static struct event s_events[] = {
    {.type = 0, .prio = 0, .text = "here goes event 1"},
    {.type = 1, .prio = 2, .text = "event 2..."},
    {.type = 2, .prio = 1, .text = "another event"},
    {.type = 1, .prio = 1, .text = "something happened!"},
    {.type = 2, .prio = 0, .text = "once more..."},
    {.type = 2, .prio = 0, .text = "more again..."},
    {.type = 1, .prio = 1, .text = "oops. it happened again"},
};

static const char *s_json_header =
    "Content-Type: application/json\r\n"
    "Cache-Control: no-cache\r\n";
static uint64_t s_boot_timestamp = 0;  // Updated by SNTP

// Certificate generation procedure:
// openssl ecparam -name prime256v1 -genkey -noout -out key.pem
// openssl req -new -key key.pem -x509 -nodes -days 3650 -out cert.pem
static const char *s_tls_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBCTCBsAIJAK9wbIDkHnAoMAoGCCqGSM49BAMCMA0xCzAJBgNVBAYTAklFMB4X\n"
    "DTIzMDEyOTIxMjEzOFoXDTMzMDEyNjIxMjEzOFowDTELMAkGA1UEBhMCSUUwWTAT\n"
    "BgcqhkjOPQIBBggqhkjOPQMBBwNCAARzSQS5OHd17lUeNI+6kp9WYu0cxuEIi/JT\n"
    "jphbCmdJD1cUvhmzM9/phvJT9ka10Z9toZhgnBq0o0xfTQ4jC1vwMAoGCCqGSM49\n"
    "BAMCA0gAMEUCIQCe0T2E0GOiVe9KwvIEPeX1J1J0T7TNacgR0Ya33HV9VgIgNvdn\n"
    "aEWiBp1xshs4iz6WbpxrS1IHucrqkZuJLfNZGZI=\n"
    "-----END CERTIFICATE-----\n";

static const char *s_tls_key =
    "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEICBz3HOkQLPBDtdknqC7k1PNsWj6HfhyNB5MenfjmqiooAoGCCqGSM49\n"
    "AwEHoUQDQgAEc0kEuTh3de5VHjSPupKfVmLtHMbhCIvyU46YWwpnSQ9XFL4ZszPf\n"
    "6YbyU/ZGtdGfbaGYYJwatKNMX00OIwtb8A==\n"
    "-----END EC PRIVATE KEY-----\n";

static int event_next(int no, struct event *e) {
  if (no < 0 || no >= (int) (sizeof(s_events) / sizeof(s_events[0]))) return 0;
  *e = s_events[no];
  return no + 1;
}

// SNTP connection event handler. When we get a response from an SNTP server,
// adjust s_boot_timestamp. We'll get a valid time from that point on
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  uint64_t *expiration_time = (uint64_t *) c->data;
  if (ev == MG_EV_OPEN) {
    *expiration_time = mg_millis() + 3000;  // Store expiration time in 3s
  } else if (ev == MG_EV_SNTP_TIME) {
    uint64_t t = *(uint64_t *) ev_data;
    s_boot_timestamp = t - mg_millis();
    c->is_closing = 1;
  } else if (ev == MG_EV_POLL) {
    if (mg_millis() > *expiration_time) c->is_closing = 1;
  }
  (void) fn_data;
}

static void timer_sntp_fn(void *param) {  // SNTP timer function. Sync up time
  mg_sntp_connect(param, "udp://time.google.com:123", sfn, NULL);
}

// Parse HTTP requests, return authenticated user or NULL
static struct user *authenticate(struct mg_http_message *hm) {
  // In production, make passwords strong and tokens randomly generated
  // In this example, user list is kept in RAM. In production, it can
  // be backed by file, database, or some other method.
  static struct user users[] = {
      {"admin", "admin", "admin_token"},
      {"user1", "user1", "user1_token"},
      {"user2", "user2", "user2_token"},
      {NULL, NULL, NULL},
  };
  char user[64], pass[64];
  struct user *u, *result = NULL;
  mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
  MG_INFO(("user [%s] pass [%s]", user, pass));

  if (user[0] != '\0' && pass[0] != '\0') {
    // Both user and password is set, search by user/password
    for (u = users; result == NULL && u->name != NULL; u++)
      if (strcmp(user, u->name) == 0 && strcmp(pass, u->pass) == 0) result = u;
  } else if (user[0] == '\0') {
    // Only password is set, search by token
    for (u = users; result == NULL && u->name != NULL; u++)
      if (strcmp(pass, u->access_token) == 0) result = u;
  }
  return result;
}

static void handle_login(struct mg_connection *c, struct user *u) {
  char cookie[256];
  mg_snprintf(cookie, sizeof(cookie),
              "Set-Cookie: access_token=%s;Path=/;"
              "HttpOnly;SameSite=Lax;Max-Age=%d\r\n",
              u->access_token, 3600 * 24);
  mg_http_reply(c, 200, cookie, "{%m:%m}", MG_ESC("user"), MG_ESC(u->name));
}

static void handle_logout(struct mg_connection *c) {
  mg_http_reply(c, 200,
                "Set-Cookie: access_token=; Path=/; "
                "Expires=Thu, 01 Jan 1970 00:00:00 UTC; "
                "Secure; HttpOnly; Max-Age=0; \r\n",
                "true\n");
}

static void handle_debug(struct mg_connection *c, struct mg_http_message *hm) {
  int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
  mg_log_set(level);
  mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
}

static size_t print_int_arr(void (*out)(char, void *), void *ptr, va_list *ap) {
  size_t len = 0, num = va_arg(*ap, size_t);  // Number of items in the array
  int *arr = va_arg(*ap, int *);              // Array ptr
  for (size_t i = 0; i < num; i++) {
    len += mg_xprintf(out, ptr, "%s%d", i == 0 ? "" : ",", arr[i]);
  }
  return len;
}

static void handle_stats_get(struct mg_connection *c) {
  int us[] = {124, 123, 7, 7, 8, 8, 125, 125, 124, 125, 134, 145};
  int ds[] = {430, 310, 56, 57, 68, 450, 470, 410, 420, 480, 490, 455};
  mg_http_reply(
      c, 200, s_json_header, "{%m:%d,%m:%lu,%m:%lu,%m:[%M],%m:[%M]}",
      MG_ESC("connected_devices"), 17,                                        //
      MG_ESC("uploaded"), 2187,                                               //
      MG_ESC("downloaded"), 17934,                                            //
      MG_ESC("upload_speed"), print_int_arr, sizeof(us) / sizeof(us[0]), us,  //
      MG_ESC("download_speed"), print_int_arr, sizeof(ds) / sizeof(ds[0]), ds);
}

static size_t print_events(void (*out)(char, void *), void *ptr, va_list *ap) {
  size_t len = 0;
  struct event e;
  int no = 0;
  while ((no = event_next(no, &e)) != 0) {
    len += mg_xprintf(out, ptr, "%s{%m:%lu,%m:%d,%m:%d,%m:%m}",  //
                      len == 0 ? "" : ",",                       //
                      MG_ESC("time"), e.timestamp,               //
                      MG_ESC("type"), e.type,                    //
                      MG_ESC("prio"), e.prio,                    //
                      MG_ESC("text"), MG_ESC(e.text));
  }
  (void) ap;
  return len;
}

static void handle_events_get(struct mg_connection *c) {
  mg_http_reply(c, 200, s_json_header, "[%M]", print_events);
}

static void handle_devices_get(struct mg_connection *c) {
  char test_json[1024];
  int nr_devs = sizeof(s_devices) / sizeof(struct device);
  memset(test_json, 0, sizeof(test_json));
  test_json[0] = '[';
  for (int i = 0; i < nr_devs; i++) {
    size_t current_length = strlen(test_json);

    mg_snprintf(
        test_json + current_length, sizeof(test_json) - current_length,
        "{%m:\"%s\",%m:\"%s\", %m:\"%s\", %m:%d,%m:\"%s\",%m:%d,%m:\"%s\"}",  //
        MG_ESC("dev_name"), s_devices[i].dev_name,                            //
        MG_ESC("mac"), s_devices[i].mac,                                      //
        MG_ESC("ip"), s_devices[i].ip_addr,                                   //
        MG_ESC("speed"), s_devices[i].speed,                                  //
        MG_ESC("connected_to"), s_devices[i].connected_to,                    //
        MG_ESC("lease_time_left"), s_devices[i].lease_time_left,              //
        MG_ESC("last_seen"), s_devices[i].last_seen);

    if (i < nr_devs - 1) {
      strncat(test_json, ",", sizeof(test_json) - strlen(test_json) - 1);
    }
  }

  strncat(test_json, "]", sizeof(test_json) - strlen(test_json) - 1);
  mg_http_reply(c, 200, s_json_header, "%s", test_json);
}

static void handle_dhcp_set(struct mg_connection *c, struct mg_str body) {
  struct dhcp dhcp;
  memset(&dhcp, 0, sizeof(dhcp));
  mg_json_get_bool(body, "$.enabled", &dhcp.enabled);
  dhcp.address_begin = mg_json_get_long(body, "$.address_begin", 0);
  dhcp.address_end = mg_json_get_long(body, "$.address_end", 0);
  dhcp.lease_time_sec = mg_json_get_long(body, "$.lease_time_sec", 0);
  s_dhcp = dhcp;  // Save to the device flash, too
  bool ok = true;
  mg_http_reply(c, 200, s_json_header,
                "{%m:%s,%m:%m}",                          //
                MG_ESC("status"), ok ? "true" : "false",  //
                MG_ESC("message"), MG_ESC(ok ? "Success" : "Failed"));
}

static void handle_dhcp_get(struct mg_connection *c) {
  mg_http_reply(c, 200, s_json_header, "{%m:%s,%m:%hhu,%m:%hhu,%m:%lu}",  //
                MG_ESC("enabled"), s_dhcp.enabled ? "true" : "false",     //
                MG_ESC("address_begin"), s_dhcp.address_begin,            //
                MG_ESC("address_end"), s_dhcp.address_end,                //
                MG_ESC("lease_time_sec"), s_dhcp.lease_time_sec);
}

// HTTP request handler function
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_ACCEPT) {
    if (fn_data != NULL) {  // TLS
      struct mg_tls_opts opts = {0};
      opts.cert = mg_str(s_tls_cert);
      opts.key = mg_str(s_tls_key);
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct user *u = authenticate(hm);

    if (mg_http_match_uri(hm, "/api/#") && u == NULL) {
      mg_http_reply(c, 403, "", "Not Authorised\n");
    } else if (mg_http_match_uri(hm, "/api/login")) {
      handle_login(c, u);
    } else if (mg_http_match_uri(hm, "/api/logout")) {
      handle_logout(c);
    } else if (mg_http_match_uri(hm, "/api/debug")) {
      handle_debug(c, hm);
    } else if (mg_http_match_uri(hm, "/api/stats/get")) {
      handle_stats_get(c);
    } else if (mg_http_match_uri(hm, "/api/events/get")) {
      handle_events_get(c);
    } else if (mg_http_match_uri(hm, "/api/devices/get")) {
      handle_devices_get(c);
    } else if (mg_http_match_uri(hm, "/api/dhcp/get")) {
      handle_dhcp_get(c);
    } else if (mg_http_match_uri(hm, "/api/dhcp/set")) {
      handle_dhcp_set(c, hm->body);
    } else {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
#if MG_ENABLE_PACKED_FS
      opts.root_dir = "/web_root";
      opts.fs = &mg_fs_packed;
#else
      opts.root_dir = "web_root";
#endif
      mg_http_serve_dir(c, ev_data, &opts);
    }
    MG_DEBUG(("%lu %.*s %.*s -> %.*s", c->id, (int) hm->method.len,
              hm->method.ptr, (int) hm->uri.len, hm->uri.ptr, (int) 3,
              &c->send.buf[9]));
  }
  (void) fn_data;
}

void web_init(struct mg_mgr *mgr) {
  mg_http_listen(mgr, HTTP_URL, fn, NULL);
  mg_http_listen(mgr, HTTPS_URL, fn, (void *) 1);

  // mg_timer_add(c->mgr, 1000, MG_TIMER_REPEAT, timer_mqtt_fn, c->mgr);
  mg_timer_add(mgr, 3600 * 1000, MG_TIMER_RUN_NOW | MG_TIMER_REPEAT,
               timer_sntp_fn, mgr);
}
