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
// version extraction. Non-static so mg_mgr_init() can stash its address in
// mgr->userdata: that store is what actually keeps -Wl,--gc-sections from
// stripping it on builds that never poll for OTAs (an attribute alone does
// not - the linker drops unreferenced sections regardless of "used"/"retain")
const char mg_fw_version[] = "MG_VERSION:" MG_OTA_FIRMWARE_VERSION;

static struct mg_ota_state {
  char my_version[MG_OTA_MAX_VERSION_LEN];
  char json_url[MG_OTA_MAX_URL_LEN];
  char version[MG_OTA_MAX_VERSION_LEN];
  char url[MG_OTA_MAX_URL_LEN];
  size_t size;
  uint8_t sha256[32];
  void (*fn)(const char *status);
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
              "GET %s%sarch=%d&version=%s&id=%s&interval=%d HTTP/1.1\r\n"
              "Host: %.*s\r\n"
              "Connection: close\r\n\r\n",
              uri, sep, MG_ARCH, s_ota->my_version, id,
              MG_OTA_PULL_INTERVAL_SECONDS, host.len, host.buf);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    double result;
    MG_DEBUG(("Got metadata:\n%.*s", hm->body.len, hm->body.buf));
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
    } else if (strcmp(s_ota->version, s_ota->my_version) == 0) {
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

void mg_ota_url_check(struct mg_mgr *mgr, const char *my_version,
                      const char *json_url, void (*fn)(const char *status)) {
  if (fn == NULL) fn = status_fn;
  if (s_ota != NULL) {
    fn("OTA already in progress");
  } else if ((s_ota = (struct mg_ota_state *) mg_calloc(1, sizeof(*s_ota))) ==
             NULL) {
    fn("Out of memory");
  } else {
    struct mg_connection *c;
    mg_snprintf(s_ota->my_version, sizeof(s_ota->my_version), "%s", my_version);
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
  if (MG_OTA_URL != NULL &&
      mg_timer_expired(&t, MG_OTA_PULL_INTERVAL_SECONDS * 1000, mg_millis())) {
    mg_ota_url_check(mgr, MG_OTA_FIRMWARE_VERSION, MG_OTA_URL,
                     MG_OTA_STATUS_FN);
  }
}
