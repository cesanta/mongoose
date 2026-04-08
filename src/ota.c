#include "ota.h"
#include "http.h"
#include "log.h"

#if MG_OTA != MG_OTA_NONE

enum { MG_OTA_STATUS_WAITING, MG_OTA_STATUS_SUCCESS, MG_OTA_STATUS_FAIL };
static int s_version_status;
static int s_ota_status;
static uint64_t s_start_time;

static struct mg_ota_metadata {
  char *version;
  char *url;
  size_t size;
  uint8_t sha256[32];
} s_ota_metadata;

static void free_ota_metadata(void) {
  if (s_ota_metadata.version) mg_free(s_ota_metadata.version);
  if (s_ota_metadata.url) mg_free(s_ota_metadata.url);
  memset(&s_ota_metadata, 0, sizeof(s_ota_metadata));
}

static int fetch_ota_metadata(struct mg_http_message *response) {
  double result;
  if (mg_http_status(response) != 200) goto fetch_failed;
  if (mg_json_get(response->body, "$", NULL) != 0) goto fetch_failed;
  free_ota_metadata();
  s_ota_metadata.version = mg_json_get_str(response->body, "$.version");
  if (s_ota_metadata.version == NULL) goto fetch_failed;
  s_ota_metadata.url = mg_json_get_str(response->body, "$.url");
  if (s_ota_metadata.url == NULL) goto fetch_failed;
  if (!mg_json_get_num(response->body, "$.size", &result) || result <= 0)
    goto fetch_failed;
  s_ota_metadata.size = (size_t) result;
  // TODO (robertc2000): parse and validate sha256
  MG_DEBUG(("Firmware version: %s, url: %s, size: %ld", s_ota_metadata.version,
            s_ota_metadata.url, s_ota_metadata.size));
  return MG_OTA_STATUS_SUCCESS;
fetch_failed:
  free_ota_metadata();
  return MG_OTA_STATUS_FAIL;
}

static void s_version_fn(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_str host = mg_url_host((char *) c->fn_data);
  struct mg_http_message *hm;
  static int fetch_status;

  if (ev == MG_EV_POLL) {
    if (s_start_time + 5 * 1000 < mg_millis()) {
      mg_error(c, "Connection timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    mg_printf(c,
              "GET %s HTTP/1.1\r\n"
              "Host: %.*s\r\n"
              "Connection: close\r\n\r\n",
              mg_url_uri((char *) c->fn_data), (int) host.len, host.buf);
    fetch_status = MG_OTA_STATUS_WAITING;
  } else if (ev == MG_EV_HTTP_MSG) {
    hm = (struct mg_http_message *) ev_data;
    fetch_status = fetch_ota_metadata(hm);
  } else if (ev == MG_EV_ERROR) {
    MG_ERROR(("%lu Connection error", c->id));
    s_version_status = MG_OTA_STATUS_FAIL;
  } else if (ev == MG_EV_CLOSE) {
    MG_DEBUG(("%lu Connection closed %lu", c->id, mg_millis() - s_start_time));
    s_version_status = fetch_status;
  }
}

static void s_firmware_fn(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_str host = mg_url_host(s_ota_metadata.url);
  struct mg_http_message hm;
  static size_t ofs = 0;
  static bool ota_begun = false;
  size_t alignment = 512, drop, len;
  int n, status;
  char *buf;
  (void) ev_data;

  if (ev == MG_EV_POLL) {
    if (s_start_time + 120 * 1000 < mg_millis()) {
      mg_error(c, "Connection timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    mg_printf(c,
              "GET %s HTTP/1.1\r\n"
              "Host: %.*s\r\n"
              "Connection: close\r\n\r\n",
              mg_url_uri(s_ota_metadata.url), (int) host.len, host.buf);
    ofs = 0;
    ota_begun = false;
  } else if (ev == MG_EV_READ) {
    if (s_ota_status == MG_OTA_STATUS_FAIL) return;
    if (!ota_begun) {  // First chunk, parse headers, OTA begin
      n = mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
      if (n < 0) {
        mg_error(c, "Bad HTTP response");
        return;
      }
      if (n == 0) return;
      status = mg_http_status(&hm);
      if (status != 200 || hm.body.len != s_ota_metadata.size) {
        mg_error(c, "Bad HTTP response (%d), body length: %lu", status,
                 hm.body.len);
        return;
      }
      MG_DEBUG(("Beginning OTA (%ld bytes)", s_ota_metadata.size));
      if (!mg_ota_begin(s_ota_metadata.size)) {
        s_ota_status = MG_OTA_STATUS_FAIL;
        mg_error(c, "mg_ota_begin(%lu) failed",
                 (unsigned long) s_ota_metadata.size);
        return;
      }
      ota_begun = true;
      len = c->recv.len - (size_t) n;
      buf = (char *) c->recv.buf + n;
      drop = (size_t) n;
    } else {  // Header parsed, incoming chunks
      len = c->recv.len;
      buf = (char *) c->recv.buf;
      drop = 0;
    }
    // OTA write
    size_t aligned = (ofs + len < s_ota_metadata.size)
                         ? aligned = MG_ROUND_DOWN(len, alignment)
                         : len;
    if (aligned == 0) return;
    if (!mg_ota_write(buf, aligned)) {
      mg_error(c, "mg_ota_write(%lu) @%lu failed", (unsigned long) aligned,
               (unsigned long) ofs);
      return;
    }
    ofs += aligned;
    mg_iobuf_del(&c->recv, 0, aligned + drop);
    MG_DEBUG(("Wrote %lu/%lu bytes", (unsigned long) ofs,
              (unsigned long) s_ota_metadata.size));
  } else if (ev == MG_EV_ERROR) {
    MG_ERROR(("%lu Connection error", c->id));
    s_ota_status = MG_OTA_STATUS_FAIL;
  } else if (ev == MG_EV_CLOSE) {  // OTA end
    MG_DEBUG(("Connection closing, downloaded %ld/%ld bytes", ofs,
              s_ota_metadata.size));
    if (s_ota_status != MG_OTA_STATUS_FAIL && ota_begun) {
      if (ofs != s_ota_metadata.size) {
        s_ota_status = MG_OTA_STATUS_FAIL;
        MG_ERROR(("Firmware size mismatch: got %lu expected %lu",
                  (unsigned long) ofs, (unsigned long) s_ota_metadata.size));
      } else if (!mg_ota_end()) {
        s_ota_status = MG_OTA_STATUS_FAIL;
        MG_ERROR(("mg_ota_end() failed"));
      } else {
        MG_DEBUG(("mg_ota_end() successful"));
        s_ota_status = MG_OTA_STATUS_SUCCESS;
      }
    }
    ota_begun = false;
    ofs = 0;
  }
}

void mg_ota_url_check(struct mg_mgr *mgr, const char *current_version,
                      const char *metadata_url,
                      void (*fn)(const char *status)) {
  s_version_status = MG_OTA_STATUS_WAITING;
  s_ota_status = MG_OTA_STATUS_WAITING;
  s_start_time = mg_millis();
  MG_DEBUG(("Connecting to %s to retrieve metadata", metadata_url));
  if (!mg_http_connect(mgr, metadata_url, s_version_fn,
                       (void *) metadata_url)) {
    if (fn) fn("Failed to connect");
    return;
  }
  while (s_version_status == MG_OTA_STATUS_WAITING) mg_mgr_poll(mgr, 10);
  if (s_version_status == MG_OTA_STATUS_SUCCESS) {
    if (strcmp(s_ota_metadata.version, current_version) == 0) {
      if (fn) fn("Same version");
      free_ota_metadata();
      return;
    }
  } else {
    if (fn) fn("Version retrieving error");
    free_ota_metadata();
    return;
  }
  if (fn) fn("Pulling firmware");
  s_start_time = mg_millis();
  MG_DEBUG(("Connecting to %s to download firmware", s_ota_metadata.url));
  if (!mg_connect(mgr, s_ota_metadata.url, s_firmware_fn, NULL)) {
    if (fn) fn("Failed to connect");
    free_ota_metadata();
    return;
  }
  while (s_ota_status == MG_OTA_STATUS_WAITING) mg_mgr_poll(mgr, 10);
  if (s_ota_status == MG_OTA_STATUS_FAIL && fn) fn("OTA fail");
  free_ota_metadata();
}

#endif
