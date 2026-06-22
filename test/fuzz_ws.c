// https://llvm.org/docs/LibFuzzer.html
// WebSocket frame-parser fuzzer: drives the ws_process() RX state machine in
// src/ws.c, which the generic fuzz.c harness never reaches (it only exercises
// HTTP/DNS/MQTT/SNTP and the TCP/IP stack, never a WS-upgraded connection).

#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_LOG 0
#define MG_ENABLE_LINES 1
#define MG_ENABLE_TCPIP 1

#include "mongoose.c"

#include "driver_mock.c"

#ifdef __cplusplus
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *, size_t);
#else
int LLVMFuzzerTestOneInput(const uint8_t *, size_t);
#endif

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  (void) c;
  (void) ev;
  (void) ev_data;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  struct mg_tcpip_if mif = {.ip = 1,
                            .mask = 255,
                            .gw = 1,
                            .driver = &mg_tcpip_driver_mock,
                            .state = MG_TCPIP_STATE_READY,
                            .gw_ready = true};
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_tcpip_init(&mgr, &mif);

  // mg_ws_connect() creates a client connection and installs mg_ws_cb as its
  // protocol handler. We skip the real handshake and feed the fuzz bytes
  // straight into the WS read path so ws_process() parses attacker frames.
  struct mg_connection *c =
      mg_ws_connect(&mgr, "ws://localhost:12345", fn, NULL, NULL);
  if (c != NULL) {
    c->pfn = mg_ws_cb;
    c->pfn_data = NULL;
    mg_iobuf_add(&c->recv, 0, data, size);
    c->pfn(c, MG_EV_READ, NULL);
  }

  mg_mgr_free(&mgr);
  return 0;
}

#if defined(MAIN)
int main(int argc, char *argv[]) {
  int res = EXIT_FAILURE;
  if (argc > 1) {
    struct mg_str data = mg_file_read(&mg_fs_posix, argv[1]);
    if (data.buf != NULL) {
      LLVMFuzzerTestOneInput((uint8_t *) data.buf, data.len);
      res = EXIT_SUCCESS;
    }
    free(data.buf);
  }
  return res;
}
#endif
