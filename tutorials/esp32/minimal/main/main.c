// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include <stdlib.h>
#include "esp_core_dump.h"
#include "esp_err.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "mongoose.h"
#include "sdkconfig.h"

#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!

#ifndef HTTP_URL
#define HTTP_URL "http://0.0.0.0:80"
#endif

#ifndef CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH
#define CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH 0
#endif

#ifndef ESP_PARTITION_SUBTYPE_DATA_COREDUMP
#define ESP_PARTITION_SUBTYPE_DATA_COREDUMP 0x03
#endif

struct crash_download {
  const esp_partition_t *partition;
  size_t offset;
  size_t size;
  size_t sent;
};

static void crash_json(struct mg_connection *c) {
#if CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH
  esp_core_dump_summary_t summary;
  char reason[200] = "";
  size_t address = 0, size = 0;
  esp_err_t err = esp_core_dump_image_get(&address, &size);
  esp_err_t sr = err == ESP_OK ? esp_core_dump_get_summary(&summary) : err;
  esp_err_t rr = err == ESP_OK ? esp_core_dump_get_panic_reason(
                                     reason, sizeof(reason)) : err;

  mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                "{%m:%s,%m:%d,%m:%u,%m:%u,%m:%m,%m:%m,%m:%d,%m:%d,%m:%m}\n",
                MG_ESC("present"), err == ESP_OK ? "true" : "false",
                MG_ESC("reset_reason"), (int) esp_reset_reason(),
                MG_ESC("address"), (unsigned) address, MG_ESC("size"),
                (unsigned) size, MG_ESC("panic_reason"),
                MG_ESC(rr == ESP_OK ? reason : ""), MG_ESC("task"),
                MG_ESC(sr == ESP_OK ? summary.exc_task : ""), MG_ESC("pc"),
                sr == ESP_OK ? (int) summary.exc_pc : 0, MG_ESC("error"),
                (int) err, MG_ESC("binary"), MG_ESC("/api/crash.bin"));
#else
  mg_http_reply(c, 501, "Content-Type: application/json\r\n",
                "{%m:false,%m:%d,%m:%m}\n", MG_ESC("present"),
                MG_ESC("reset_reason"), (int) esp_reset_reason(),
                MG_ESC("error"), MG_ESC("coredump to flash is disabled"));
#endif
}

static void crash_send(struct mg_connection *c) {
#if CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH
  struct crash_download *d = (struct crash_download *) c->fn_data;
  while (d != NULL && d->sent < d->size && c->send.len < 2048) {
    uint8_t buf[512];
    size_t left = d->size - d->sent;
    size_t len = left > sizeof(buf) ? sizeof(buf) : left;
    esp_err_t err = esp_partition_read(d->partition, d->offset + d->sent, buf,
                                       len);
    if (err != ESP_OK) {
      MG_ERROR(("esp_partition_read: %s", esp_err_to_name(err)));
      c->is_draining = 1;
      return;
    }
    mg_send(c, buf, len);
    d->sent += len;
  }
  if (d != NULL && d->sent >= d->size) {
    free(d);
    c->fn_data = NULL;
    c->is_draining = 1;
  }
#else
  (void) c;
#endif
}

static void crash_bin(struct mg_connection *c) {
#if CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH
  const esp_partition_t *p = esp_partition_find_first(
      ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_COREDUMP, NULL);
  struct crash_download *d;
  size_t address = 0, size = 0, offset = 0;
  esp_err_t err = esp_core_dump_image_get(&address, &size);

  if (err != ESP_OK) {
    mg_http_reply(c, 404, "Content-Type: application/json\r\n",
                  "{%m:%m}\n", MG_ESC("error"), MG_ESC(esp_err_to_name(err)));
  } else if (p == NULL || address < p->address ||
             address + size > p->address + p->size) {
    mg_http_reply(c, 500, "Content-Type: application/json\r\n",
                  "{%m:%m}\n", MG_ESC("error"),
                  MG_ESC("invalid coredump partition"));
  } else if ((d = (struct crash_download *) calloc(1, sizeof(*d))) == NULL) {
    mg_http_reply(c, 500, "Content-Type: application/json\r\n",
                  "{%m:%m}\n", MG_ESC("error"), MG_ESC("out of memory"));
  } else {
    offset = address - p->address;
    d->partition = p;
    d->offset = offset;
    d->size = size;
    c->fn_data = d;
    mg_printf(c,
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: application/octet-stream\r\n"
              "Content-Length: %u\r\n"
              "Content-Disposition: attachment; filename=\"esp32-coredump."
              "bin\"\r\n"
              "\r\n",
              (unsigned) size);
    crash_send(c);
  }
#else
  mg_http_reply(c, 501, "Content-Type: application/json\r\n",
                "{%m:%m}\n", MG_ESC("error"),
                MG_ESC("coredump to flash is disabled"));
#endif
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/crash.bin"), NULL)) {
      crash_bin(c);
    } else if (mg_match(hm->uri, mg_str("/api/crash"), NULL)) {
      crash_json(c);
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose\n");
    }
  } else if (ev == MG_EV_POLL && c->fn_data != NULL) {
    crash_send(c);
  } else if (ev == MG_EV_CLOSE && c->fn_data != NULL) {
    free(c->fn_data);
    c->fn_data = NULL;
  }
}

void app_main(void) {
  void wifi_init(const char *ssid, const char *pass);
  wifi_init(WIFI_SSID, WIFI_PASS);

  struct mg_mgr mgr;
  mg_log_set(MG_LL_INFO);
  mg_mgr_init(&mgr);
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", HTTP_URL));
  mg_http_listen(&mgr, HTTP_URL, http_ev_handler, NULL);

  for (;;) mg_mgr_poll(&mgr, 1000);
}
