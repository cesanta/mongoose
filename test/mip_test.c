#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_LINES 1
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_PACKED_FS 0

#include "mongoose.c"
#include "driver_mock.c"

static int s_num_tests = 0;

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      abort();                                                  \
    }                                                           \
  } while (0)

static void test_statechange(void) {
  char tx[1540];
  struct mg_tcpip_if iface;
  memset(&iface, 0, sizeof(iface));
  iface.ip = mg_htonl(0x01020304);
  iface.state = MG_TCPIP_STATE_READY;
  iface.tx.ptr = tx, iface.tx.len = sizeof(tx);
  iface.driver = &mg_tcpip_driver_mock;
  onstatechange(&iface);
}

static void ph(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_POLL) ++(*(int *) fn_data);
  (void) c, (void) ev_data;
}

static void test_poll(void) {
  int count = 0, i;
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  struct mg_tcpip_if mif;
  memset(&mif, 0, sizeof(mif));
  mif.driver = &mg_tcpip_driver_mock;
  mg_tcpip_init(&mgr, &mif);
  mg_http_listen(&mgr, "http://127.0.0.1:12346", ph, &count);
  for (i = 0; i < 10; i++) mg_mgr_poll(&mgr, 0);
  ASSERT(count == 10);
  mg_tcpip_free(&mif);
  mg_mgr_free(&mgr);
}

int main(void) {
  test_statechange();
  test_poll();
  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return 0;
}
