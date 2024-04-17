// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "net.h"

// Device settings
struct device_settings {
  uint32_t magic;
  int log_level;
  bool mqtt_enabled;
  char mqtt_server_url[64];
  char mqtt_topic_tx[16];
  char mqtt_topic_rx[16];
};

struct conndata {
  uint64_t expiration_time;  // Modbus request timeout
  unsigned long id;          // Connection ID waiting for the Modbus response
};

static struct device_settings s_settings;
static const char *s_json_header =
    "Content-Type: application/json\r\n"
    "Cache-Control: no-cache\r\n";
static uint64_t s_boot_timestamp = 0;  // Updated by SNTP

static void set_default_settings(struct device_settings *s) {
  s->magic = SETTINGS_MAGIC;
  s->log_level = MG_LL_DEBUG;
  mg_snprintf(s->mqtt_server_url, sizeof(s->mqtt_server_url), "%s",
              "mqtt://broker.hivemq.com:1883");
  mg_snprintf(s->mqtt_topic_tx, sizeof(s->mqtt_topic_tx), "%s", "modbus1/tx");
  mg_snprintf(s->mqtt_topic_rx, sizeof(s->mqtt_topic_rx), "%s", "modbus1/rx");
}

// This is for newlib and TLS (mbedTLS)
uint64_t mg_now(void) {
  return mg_millis() + s_boot_timestamp;
}

// SNTP connection event handler. When we get a response from an SNTP server,
// adjust s_boot_timestamp. We'll get a valid time from that point on
static void sfn(struct mg_connection *c, int ev, void *ev_data) {
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
}

// SNTP timer function. Sync up time
static void timer_sntp_fn(void *param) {
  mg_sntp_connect(param, "udp://time.google.com:123", sfn, NULL);
}

static void setfromjson(struct mg_str json, const char *jsonpath, char *buf,
                        size_t len) {
  char *val = mg_json_get_str(json, jsonpath);
  if (val != NULL) mg_snprintf(buf, len, "%s", val);
  free(val);
}

static void handle_settings_set(struct mg_connection *c, struct mg_str body) {
  struct device_settings settings;
  memset(&settings, 0, sizeof(settings));
  set_default_settings(&settings);
  mg_json_get_bool(body, "$.mqtt_enabled", &settings.mqtt_enabled);
  settings.log_level = mg_json_get_long(body, "$.log_level", MG_LL_INFO);
  setfromjson(body, "$.mqtt_server_url", settings.mqtt_server_url,
              sizeof(settings.mqtt_server_url));
  setfromjson(body, "$.mqtt_topic_rx", settings.mqtt_topic_rx,
              sizeof(settings.mqtt_topic_rx));
  setfromjson(body, "$.mqtt_topic_tx", settings.mqtt_topic_tx,
              sizeof(settings.mqtt_topic_tx));

  s_settings = settings;
  bool ok = web_save_settings(&s_settings, sizeof(s_settings));
  mg_http_reply(c, 200, s_json_header,
                "{%m:%s,%m:%m}",                          //
                MG_ESC("status"), ok ? "true" : "false",  //
                MG_ESC("message"), MG_ESC(ok ? "Success" : "Failed"));
}

static void handle_settings_get(struct mg_connection *c) {
  mg_http_reply(c, 200, s_json_header,
                "{%m:%s,%m:%d,%m:%m,%m:%m,%m:%m}\n",  //
                MG_ESC("mqtt_enabled"),
                s_settings.mqtt_enabled ? "true" : "false",  //
                MG_ESC("log_level"), s_settings.log_level,   //
                MG_ESC("mqtt_server_url"), MG_ESC(s_settings.mqtt_server_url),
                MG_ESC("mqtt_topic_rx"), MG_ESC(s_settings.mqtt_topic_rx),
                MG_ESC("mqtt_topic_tx"), MG_ESC(s_settings.mqtt_topic_tx));
}

// Modbus handler function
static void mfn(struct mg_connection *c, int ev, void *ev_data) {
  struct conndata *cd = (struct conndata *) c->data;
  if (ev == MG_EV_READ) {
    MG_INFO(("%lu RECEIVED %lu", c->id, c->recv.len));
    if (c->recv.len < 8) return;  // Less than minimum length, buffer more
    uint16_t len = mg_ntohs(*(uint16_t *) &c->recv.buf[4]);  // PDU length
    if (c->recv.len < len + 4U) return;  // Partial frame, buffer more
    // Notify parent connection
    for (struct mg_connection *t = c->mgr->conns; t != NULL; t = t->next) {
      if (t->id == cd->id) mg_call(t, MG_EV_USER, &c->recv);
    }
    c->is_closing = 1;
  } else if (MG_EV_POLL) {
    // MG_INFO(("%lu closing tmout %llu", c->id, cd->expiration_time));
    if (cd->expiration_time > 0 && cd->expiration_time < mg_millis()) {
      c->is_closing = 1;
    }
  }
  (void) ev_data;
}

static void send8(struct mg_connection *c, uint8_t val) {
  mg_send(c, &val, sizeof(val));
}

static void send16(struct mg_connection *c, uint16_t val) {
  send8(c, (uint8_t) (val >> 8) & 255);
  send8(c, (uint8_t) (val & 255));
}

static struct mg_connection *start_modbus_request(struct mg_mgr *mgr,
                                                  struct mg_str json,
                                                  unsigned long cid) {
  struct mg_connection *c = NULL;
  char *url = mg_json_get_str(json, "$.url");
  long timeout = mg_json_get_long(json, "$.timeout", 750);
  uint8_t id = (uint8_t) mg_json_get_long(json, "$.id", 1);
  uint16_t reg = (uint16_t) mg_json_get_long(json, "$.reg", 1);
  uint8_t func = (uint8_t) mg_json_get_long(json, "$.func", 0);
  uint16_t nregs = (uint16_t) mg_json_get_long(json, "$.nregs", 1);
  MG_INFO(("%lu REQUEST: %.*s", cid, json.len, json.buf));
  if (func == 0) {
    MG_ERROR(("Set func to a valid modbus function code"));
  } else if ((c = mg_connect(mgr, url, mfn, NULL)) == NULL) {
    MG_ERROR(("Failed to start modbus connection at %M", MG_ESC(url)));
  } else {
    send16(c, 1);  // TID. Use random?
    send16(c, 0);  // Protocol identifier: 0 (modbus)
    uint16_t *lp = (uint16_t *) &c->send.buf[c->send.len];
    send16(c, 0);  // Length: to be set later
    size_t len = c->send.len;

    send8(c, id);    // Client ID
    send8(c, func);  // Function

    if (func == 3 || func == 4 || func == 6 || func == 16) {
      send16(c, reg);    // Start register
      send16(c, nregs);  // Number of registers

      if (func == 16) {                   // Fill in register values to write
        send8(c, (uint8_t) (nregs * 2));  // Send number of bytes
        for (uint16_t i = 0; i < nregs; i++) {
          char path[20];
          mg_snprintf(path, sizeof(path), "$.values[%hu]", i);
          uint16_t r = (uint16_t) mg_json_get_long(json, path, 0);
          send16(c, r);
        }
      }
    } else if (func == 43) {
      send8(c, 14);  // MEI
      send8(c, 1);   // Basic device ID: 1
      send8(c, 0);   // Object ID: 0 (vendor name)
    }

    *lp = mg_htons((uint16_t) (c->send.len - len));  // Set length field
    mg_hexdump(c->send.buf, c->send.len);
    MG_INFO(("%lu SENDING %lu", c->id, c->send.len));

    struct conndata *cd = (struct conndata *) c->data;
    cd->id = cid;  // Store parent connection ID
    cd->expiration_time = mg_millis() + timeout;
  }
  free(url);
  return c;
}

static void handle_modbus_exec(struct mg_connection *c, struct mg_str body) {
  struct mg_connection *mc = start_modbus_request(c->mgr, body, c->id);
  if (mc == NULL) {
    mg_http_reply(c, 200, s_json_header, "false\n");
  } else {
    struct conndata *cd = (struct conndata *) c->data;
    cd->expiration_time = mg_millis() + 1500;
  }
}

// Print modbus response
static size_t print_mb_resp(void (*out)(char, void *), void *ptr, va_list *ap) {
  int func = va_arg(*ap, int);
  size_t len = va_arg(*ap, size_t);
  uint8_t *buf = va_arg(*ap, uint8_t *);
  size_t n = 0;
  if (func == 3 || func == 4) {
    // For "multiple" read responses, print registers
    n += mg_xprintf(out, ptr, "[");
    for (size_t ofs = 1; ofs < len; ofs += 2) {
      uint16_t reg = mg_ntohs(*(uint8_t *) &buf[ofs]);
      n += mg_xprintf(out, ptr, "%s%hu", ofs == 1 ? "" : ",", reg);
    }
    n += mg_xprintf(out, ptr, "]");
  } else if (func == 43) {
    n += mg_xprintf(out, ptr, "{");
    if (len >= 6) {
      n += mg_xprintf(out, ptr, "%m:%d", MG_ESC("mei"), buf[0]);
      n += mg_xprintf(out, ptr, ",%m:%d", MG_ESC("devid"), buf[1]);
      n += mg_xprintf(out, ptr, ",%m:%d", MG_ESC("conformity"), buf[2]);
      n += mg_xprintf(out, ptr, ",%m:%d", MG_ESC("more"), buf[3]);
      n += mg_xprintf(out, ptr, ",%m:%d", MG_ESC("next"), buf[4]);
      n += mg_xprintf(out, ptr, ",%m:[", MG_ESC("objects"));
      size_t ofs = 6;
      while (ofs < len) {
        if (ofs > 6) n += mg_xprintf(out, ptr, ",");
        n += mg_xprintf(out, ptr, "{");
        n += mg_xprintf(out, ptr, "%m:%d", MG_ESC("id"), buf[ofs++]);
        if (ofs < len) {
          uint8_t olen = buf[ofs++];
          if (olen > (uint8_t) (len - ofs)) olen = (uint8_t) (len - ofs);
          n += mg_xprintf(out, ptr, ",%m:%m", MG_ESC("value"), mg_print_esc,
                          olen, &buf[ofs]);
          ofs += olen;
        }
        n += mg_xprintf(out, ptr, "}");
      }
      n += mg_xprintf(out, ptr, "]");
    }
    n += mg_xprintf(out, ptr, "}");
  } else {
    n += mg_xprintf(out, ptr, "null");
  }
  return n;
}

// HTTP request handler function
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  struct conndata *cd = (struct conndata *) c->data;
  if (ev == MG_EV_ACCEPT) {
    if (c->fn_data != NULL) {  // TLS listener!
      struct mg_tls_opts opts = {0};
      opts.cert = mg_unpacked("/certs/server_cert.pem");
      opts.key = mg_unpacked("/certs/server_key.pem");
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    if (mg_match(hm->uri, mg_str("/api/settings/get"), NULL)) {
      handle_settings_get(c);
    } else if (mg_match(hm->uri, mg_str("/api/settings/set"), NULL)) {
      handle_settings_set(c, hm->body);
    } else if (mg_match(hm->uri, mg_str("/api/settings/set"), NULL)) {
      handle_settings_set(c, hm->body);
    } else if (mg_match(hm->uri, mg_str("/api/modbus/exec"), NULL)) {
      handle_modbus_exec(c, hm->body);
    } else if (mg_match(hm->uri, mg_str("/api/device/reset"), NULL)) {
      mg_timer_add(c->mgr, 500, 0, (void (*)(void *)) mg_device_reset, NULL);
      mg_http_reply(c, 200, s_json_header, "true\n");
    } else {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
      opts.root_dir = "web_root";  // On workstations, use filesystem
#else
      opts.root_dir = "/web_root";  // On embedded, use packed files
      opts.fs = &mg_fs_packed;
#endif
      mg_http_serve_dir(c, ev_data, &opts);
    }
    MG_DEBUG(("%lu %.*s %.*s", c->id, (int) hm->method.len, hm->method.buf,
              (int) hm->uri.len, hm->uri.buf));
  } else if (ev == MG_EV_POLL) {
    if (cd->expiration_time > 0 && cd->expiration_time < mg_millis()) {
      cd->expiration_time = 0;
      mg_http_reply(c, 200, s_json_header, "false\n");
    }
  } else if (ev == MG_EV_USER) {
    cd->expiration_time = 0;  // Cleanup timeout setting
    struct mg_iobuf *io = ev_data;
    uint8_t f = io->buf[7];
    mg_http_reply(c, 200, s_json_header, "{%m:%s,%m:%m,%m:%M}\n",         //
                  MG_ESC("success"), "true",                              //
                  MG_ESC("raw"), mg_print_hex, io->len - 7, io->buf + 7,  //
                  MG_ESC("data"), print_mb_resp, f, io->len - 8, io->buf + 8);
  }
}

void web_init(struct mg_mgr *mgr) {
  set_default_settings(&s_settings);
  web_load_settings(&s_settings, sizeof(s_settings));

  mg_http_listen(mgr, HTTP_URL, fn, NULL);
  mg_http_listen(mgr, HTTPS_URL, fn, (void *) 1);
  mg_timer_add(mgr, 10 * 60 * 1000, MG_TIMER_RUN_NOW | MG_TIMER_REPEAT,
               timer_sntp_fn, mgr);
}
