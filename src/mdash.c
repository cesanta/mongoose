#include "mdash.h"

#if MG_ENABLE_MDASH

#include "base64.h"
#include "health.h"
#include "log.h"
#include "ota.h"
#include "printf.h"
#include "rpc.h"
#include "sntp.h"
#include "tls.h"
#include "url.h"
#include "ws.h"

static const char *mg_mdash_ca_pem =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n";

static struct mg_connection *mg_mdash_conn = NULL;
// static struct mg_str mg_mdash_key;
// static void mg_connection *mg_mdash_conn = NULL;

static void mg_mdash_fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_CONNECT) {
    // c->is_hexdumping = true;
    if (mg_url_is_ssl(MG_MDASH_URL)) {
      struct mg_str host = mg_url_host(MG_MDASH_URL);
      struct mg_tls_opts opts = {.ca = mg_str(mg_mdash_ca_pem), .name = host};
      mg_tls_init(c, &opts);
    }
  }
  if (ev == MG_EV_WS_OPEN) {
    MG_INFO(("Connected to %s", MG_MDASH_URL));
    // s_mDashState = MDASH_STATE_CLOUD_CONNECTED;
    // mDashTriggerEvent(s_mDashState, NULL);
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_iobuf io = {0, 0, 0, 512};
    struct mg_ws_message *wm = ev_data;
    struct mg_str s = wm->data;
    while (s.len > 0 && isspace((uint8_t) s.buf[s.len - 1])) s.len--;
    MG_INFO(("RPC: %.*s", (int) s.len, s.buf));
    struct mg_rpc_req r = {.head = &c->mgr->rpcs,
                           .rpc = NULL,
                           .pfn = mg_pfn_iobuf,
                           .pfn_data = &io,
                           .req_data = NULL,
                           .frame = s};
    mg_rpc_process(&r);
    if (io.buf != NULL) {
      MG_INFO(("     %s", io.buf));
      mg_ws_send(c, (const char *) io.buf, io.len, WEBSOCKET_OP_TEXT);
    } else {
      MG_DEBUG(("%s", "RPC unhandled"));
    }
    mg_iobuf_free(&io);
  } else if (ev == MG_EV_CLOSE) {
    mg_mdash_conn = NULL;
    MG_INFO(("disconnected, ram %lu", 0));
  }
  (void) c;
}

static void mg_mdash_rpc_get_info(struct mg_rpc_req *r) {
  mg_rpc_ok(r, "{%m:%m,%m:%lu,%m:%m,%m:\"mws.%d\"}", MG_ESC("fw_version"),
            MG_ESC(MG_MDASH_FIRMWARE_VERSION), MG_ESC("uptime"),
            (uint64_t) (mg_now() / 1000), MG_ESC("reboot_reason"),
            MG_ESC(mg_health_reason_str(mg_health_reason())), MG_ESC("arch"),
            MG_ARCH);
}

static void mg_mdash_rpc_ota_begin(struct mg_rpc_req *r) {
  size_t total = (size_t) mg_json_get_long(r->frame, "$.params.size", -1);
  if (mg_ota_begin(total)) {
    mg_rpc_ok(r, "true");
  } else {
    mg_rpc_err(r, 500, "%m", MG_ESC("failed"));
  }
}

static void mg_mdash_rpc_ota_write(struct mg_rpc_req *r) {
  int len = 0, ofs = 0;
  if ((ofs = mg_json_get(r->frame, "$.params.data", &len)) <= 0 ||
      r->frame.buf[ofs] != '"') {
    mg_rpc_err(r, 500, "%m", MG_ESC("data required"));
  } else {
    struct mg_str s = mg_str_n(&r->frame.buf[ofs] + 1, (size_t) (len - 2));
    size_t n = mg_base64_decode(s.buf, s.len, s.buf, s.len); // Decode in-place
    if (n == 0) {
      mg_rpc_ok(r, "%m", MG_ESC("finished"));
      mg_ota_end();
    } else if (mg_ota_write(s.buf, s.len)) {
      mg_rpc_ok(r, "{%m:%d}", MG_ESC("queued"), n);
    } else {
      mg_rpc_err(r, 500, "%m", MG_ESC("write failed"));
      mg_ota_end();
    }
    // mDashNotify("State.Set", "{%m:{%m:%d}}", MG_ESC("ota"),
    // MG_ESC("percent"), (int) (s_ota_written * 100 / s_ota_size));
  }
}

static void mg_mdash_rpc_ota_end(struct mg_rpc_req *r) {
  if (mg_ota_end()) {
    mg_rpc_ok(r, "true");
  } else {
    mg_rpc_err(r, 500, "%m", MG_ESC("failed"));
  }
  // mDashNotify("State.Set", "{%m:{%m:null}}", MG_ESC("ota"),
  // MG_ESC("percent"));
}

void mg_mdash_init(struct mg_mgr *mgr) {
  MG_INFO(("Initialising mdash agent, key [%s]", MG_MDASH_KEY));
  mg_rpc_add(&mgr->rpcs, mg_str("RPC.List"), mg_rpc_list, NULL);
  mg_rpc_add(&mgr->rpcs, mg_str("Sys.GetInfo"), mg_mdash_rpc_get_info, NULL);
  mg_rpc_add(&mgr->rpcs, mg_str("OTA.Begin"), mg_mdash_rpc_ota_begin, NULL);
  mg_rpc_add(&mgr->rpcs, mg_str("OTA.Write"), mg_mdash_rpc_ota_write, NULL);
  mg_rpc_add(&mgr->rpcs, mg_str("OTA.End"), mg_mdash_rpc_ota_end, NULL);
}

void mg_mdash_poll(struct mg_mgr *mgr) {
  static uint64_t timer = 0;
  if (mg_mdash_conn == NULL && mg_timer_expired(&timer, 1000, mg_millis())) {
    MG_INFO(("Connecting to %s", MG_MDASH_URL));
    mg_mdash_conn = mg_ws_connect(mgr, MG_MDASH_URL, mg_mdash_fn, NULL,
                                  "Origin: %s\r\n"
                                  "Authorization: Bearer %s\r\n",
                                  MG_MDASH_URL, MG_MDASH_KEY);
  }
}

#else
void mg_mdash_init(struct mg_mgr *mgr) {
  (void) mgr;
}
void mg_mdash_poll(struct mg_mgr *mgr) {
  (void) mgr;
}
#endif
