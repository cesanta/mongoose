#include "ota.h"
#include "http.h"
#include "json.h"
#include "log.h"
#include "printf.h"
#include "url.h"
#include "util.h"

#ifndef MG_OTA_MAX_URL_LEN
#define MG_OTA_MAX_URL_LEN 256
#endif

// Scannable version tag embedded in every firmware binary, for server-side
// version extraction. The version string starts after the 11-char "MG_VERSION:" prefix.
static const char mg_fw_version[] = "MG_VERSION:" MG_OTA_FIRMWARE_VERSION;

static bool s_autocommit_ok;  // True after OTA server confirms "same version"

static struct mg_ota_state {
  char json_url[MG_OTA_MAX_URL_LEN];
  char version[MG_OTA_MAX_VERSION_LEN];
  char url[MG_OTA_MAX_URL_LEN];
  size_t size;
  uint8_t sha256[32];
  void (*fn)(const char *error_message);
} *s_ota;

static void s_firmware_fn(struct mg_connection *c, int ev, void *ev_data);

#if MG_ENABLE_CUSTOM_DEVICE_ID
#else
void mg_ota_device_id(char *buf, size_t len) {
#if MG_ARCH == MG_ARCH_CUBE && defined(UID_BASE)
  mg_snprintf(buf, len, "%M", mg_print_hex, 12, (uint8_t *) UID_BASE);
#else
  mg_snprintf(buf, len, "%d", 0);
#endif
}
#endif

static void s_version_fn(struct mg_connection *c, int ev, void *ev_data) {
  uint64_t expiration = *(uint64_t *) c->data;
  if (ev == MG_EV_POLL) {
    if (mg_millis() > expiration) mg_error(c, "Metadata timeout");
  } else if (ev == MG_EV_CONNECT) {
    char id[33];
    struct mg_str host = mg_url_host(s_ota->json_url);
    const char *uri = mg_url_uri(s_ota->json_url);
    const char *sep = strchr(uri, '?') == NULL ? "?" : "&";
    mg_ota_device_id(id, sizeof(id));
    id[sizeof(id) - 1] = '\0';
    mg_printf(c,
              "GET %s%sarch=%d&version=%s&id=%s&interval=%d&boot=%d HTTP/1.1\r\n"
              "Host: %.*s\r\n"
              "Connection: close\r\n\r\n",
              uri, sep, MG_ARCH, mg_fw_version + 11, id,
              MG_OTA_PULL_INTERVAL_SECONDS, MG_OTA_STATE_GET(), host.len, host.buf);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    double result;
    MG_DEBUG(("Got metadata: %.*s", hm->body.len, hm->body.buf));
    if (mg_http_status(hm) != 200 || mg_json_get(hm->body, "$", NULL) != 0 ||
        !mg_json_unescape(hm->body, "$.version", s_ota->version,
                          sizeof(s_ota->version)) ||
        !mg_json_unescape(hm->body, "$.url", s_ota->url, sizeof(s_ota->url)) ||
        !mg_json_get_num(hm->body, "$.size", &result) || result <= 0) {
      char buf[100];
      mg_snprintf(buf, sizeof(buf), "Bad metadata: %.*s", hm->body.len,
                  hm->body.buf);
      s_ota->fn(buf);
      mg_free(s_ota);
      s_ota = NULL;
    } else if (strcmp(s_ota->version, mg_fw_version + 11) == 0) {
      s_autocommit_ok = true;
      s_ota->fn("Same version");
      mg_free(s_ota);
      s_ota = NULL;
    } else {
      struct mg_connection *fc;
      s_ota->size = (size_t) result;
      // TODO (robertc2000): parse and validate sha256
      MG_DEBUG(("Firmware version: %s, url: %s, size: %ld", s_ota->version,
                s_ota->url, s_ota->size));
      fc = mg_http_connect(c->mgr, s_ota->url, s_firmware_fn, NULL);
      if (fc == NULL) {
        s_ota->fn("Failed to connect");
        mg_free(s_ota);
        s_ota = NULL;
      } else {
        *(uint64_t *) fc->data = mg_millis() + 300 * 1000;  // Set expiration
      }
    }
    c->is_closing = 1;
  } else if (ev == MG_EV_ERROR) {
    s_ota->fn((char *) ev_data);
    mg_free(s_ota);
    s_ota = NULL;
  }
}

static void status_fn(const char *errmsg) {
  if (errmsg) MG_ERROR(("OTA failed: %s", errmsg));
}

static void status_fn_2(struct mg_connection *c, const char *errmsg) {
  if (s_ota) s_ota->fn(errmsg);
  mg_free(s_ota);
  s_ota = NULL;
  mg_http_reply(c, errmsg ? 500 : 200, "", "%s\n", errmsg ? errmsg : "ok");
}

static void s_firmware_fn(struct mg_connection *c, int ev, void *ev_data) {
  uint64_t expiration = *(uint64_t *) c->data;
  if (ev == MG_EV_POLL) {
    if (mg_millis() > expiration) mg_error(c, "OTA timeout");
  } else if (ev == MG_EV_CONNECT) {
    struct mg_str host = mg_url_host(s_ota->url);
    mg_printf(c,
              "GET %s HTTP/1.1\r\n"
              "Host: %.*s\r\n"
              "Connection: close\r\n\r\n",
              mg_url_uri(s_ota->url), (int) host.len, host.buf);
  } else if (ev == MG_EV_HTTP_HDRS) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    int status = mg_http_status(hm);
    if (status != 200 || hm->body.len != s_ota->size) {
      mg_error(c, "Bad HTTP response: status %d, size %lu vs %lu", status,
               (unsigned long) hm->body.len, (unsigned long) s_ota->size);
    } else {
      MG_DEBUG(("Beginning OTA (%lu bytes)", (unsigned long) s_ota->size));
      mg_http_start_ota(c, hm, status_fn_2);
    }
  } else if (ev == MG_EV_ERROR) {
    s_ota->fn((char *) ev_data);
    mg_free(s_ota);
    s_ota = NULL;
  }
}

void mg_ota_url_check(struct mg_mgr *mgr, const char *json_url,
                      void (*fn)(const char *error_message)) {
  if (fn == NULL) fn = status_fn;
  if (s_ota != NULL) {
    fn("OTA already in progress");
  } else if ((s_ota = (struct mg_ota_state *) mg_calloc(1, sizeof(*s_ota))) ==
             NULL) {
    fn("Out of memory");
  } else {
    struct mg_connection *c;
    mg_snprintf(s_ota->json_url, sizeof(s_ota->json_url), "%s", json_url);
    MG_DEBUG(("Connecting to %s", json_url));
    c = mg_http_connect(mgr, s_ota->json_url, s_version_fn, NULL);
    if (c == NULL) {
      mg_free(s_ota);
      s_ota = NULL;
      fn("Failed to connect");
    } else {
      s_ota->fn = fn;
      *(uint64_t *) c->data = mg_millis() + 5 * 1000;  // Set expiration
    }
  }
}

void mg_ota_poll(struct mg_mgr *mgr) {
  static uint64_t t = 5000;  // Fire first time 5 sec after boot

  static uint64_t feed_timer;  // Advances by 500ms per tick; tracks elapsed time
  if (MG_OTA_STATE_GET() == MG_OTA_TESTING &&
      mg_timer_expired(&feed_timer, 500, mg_millis())) {
    if (feed_timer < (uint64_t) MG_OTA_ROLLBACK_TIMEOUT_SECONDS * 1000) {
      MG_OTA_ROLLBACK_TIMER_FEED();  // Feed watchdog every 500ms
    } else if (s_autocommit_ok) {
      MG_INFO(("Auto-committing firmware"));
      MG_OTA_STATE_SET(MG_OTA_CONFIRMED);  // Stop feeding → IWDG resets cleanly
    } else {
      MG_INFO(("No commit confirmation, rolling back"));
      // Stop feeding → IWDG fires → resets into MG_OTA_FAILED → rollback
    }
  }

  if (MG_OTA_URL != NULL &&
      mg_timer_expired(&t, MG_OTA_PULL_INTERVAL_SECONDS * 1000, mg_millis())) {
    mg_ota_url_check(mgr, MG_OTA_URL, MG_OTA_STATUS_FN);
  }
}
