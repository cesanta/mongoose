#define MG_ENABLE_SOCKET 1 // 0 for true MIP tests - 1 for BSD sockets
#define MG_ENABLE_LINES 1
#define MG_ENABLE_MIP 0    // 1 for true MIP tests - 0 for BSD sockets
#define MG_ENABLE_PACKED_FS 0

#include <assert.h>
#include "mongoose.c"

#include "driver_mock.c"

// ********************** Environment **********************

#define FETCH_BUF_SIZE (256 * 1024)

// ********************** Utilities **********************

// Assertion
static int s_num_tests = 0;

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      abort();                                                  \
    }                                                           \
  } while (0)

// ********************** Tests **********************
// MIP Mongoose TCP-IP stack tests

// *** Queue ***


#if MG_ENABLE_MIP == 1
static void test_queue(void) {
  static uint8_t
      buf[sizeof(size_t) + sizeof(uint16_t) + 3];  // fit 1 element but not 2
  uint16_t val = 1234;
  static struct queue q = {buf, sizeof(buf), 0, 0};

  // Write to an empty queue, and read back
  assert(q_avail(&q) == 0);
  assert(q_write(&q, &val, sizeof(val)) == true);
  assert(q_avail(&q) == sizeof(val));
  assert(q.head > q.tail);
  // Only one element may fit
  assert(q_write(&q, &val, sizeof(val)) == false);
  val = 0;
  assert(q_read(&q, &val) == sizeof(val));
  assert(val == 1234);
  assert(q_avail(&q) == 0);

  // Second write - wrap over the buffer boundary
  assert(q_write(&q, &val, sizeof(val)) == true);
  assert(q_avail(&q) == sizeof(val));
  assert(q.head < q.tail);
  // Only one element may fit
  assert(q_write(&q, &val, sizeof(val)) == false);
  val = 0;
  assert(q_read(&q, &val) == sizeof(val));
  assert(val == 1234);
  assert(q_avail(&q) == 0);
}
#endif

// *** States ***

#if MG_ENABLE_MIP == 1
static void test_statechange(void) {
  uint8_t tx[1540];
  struct mip_if iface;
  memset(&iface, 0, sizeof(iface));
  iface.ip = mg_htonl(0x01020304);
  iface.state = MIP_STATE_READY;
  iface.tx.buf = tx, iface.tx.len = sizeof(tx);
  iface.driver = &mip_driver_mock;
  onstatechange(&iface);
}
#endif

// *** Simple HTTP fetch ***
// Note OS TCP-IP stack
// jfs 3/11/22

// static void f_http_fetch(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
// static void test_http_fetch(void);

// Worker
static void f_http_fetch(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
      int *fn_data_int_ptr = (int *) fn_data;
      // MG_INFO(("%d", ev));
      if (ev == MG_EV_CONNECT) {
        // c->is_hexdumping = 1;
        mg_printf(c, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n",
                  c->rem.is_ip6 ? "" : "/robots.txt",
                  c->rem.is_ip6 ? "ipv6.google.com" : "cesanta.com");
      } else if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        // MG_INFO(("-->[%.*s]", (int) hm->message.len, hm->message.ptr));
        // ASSERT(mg_vcmp(&hm->method, "HTTP/1.1") == 0);
        // ASSERT(mg_vcmp(&hm->uri, "301") == 0);
        *fn_data_int_ptr = mg_http_status(hm);
      } else if (ev == MG_EV_CLOSE) {
        if (*fn_data_int_ptr == 0) *fn_data_int_ptr = 888;
      } else if (ev == MG_EV_ERROR) {
        if (*fn_data_int_ptr == 0) *fn_data_int_ptr = 777;
      }
}

// HTTP fetch (ref unit_test.c#L986-L996)
// Using OS's TCP/IP stack.
static void test_http_fetch(void) {
    struct mg_mgr mgr;
    struct mg_connection *c;
    int i, ok = 0;
    mg_mgr_init(&mgr);
    c = mg_http_connect(&mgr, "http://cesanta.com", f_http_fetch, &ok);
    ASSERT(c != NULL);		// Assertion on initialisation
    for (i = 0; i < 500 && ok <= 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(ok == 301);		  // Assertion on DNS lookup
  c->is_closing = 1;
  mg_mgr_poll(&mgr, 0);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL); // Assertion on deconstruction
}

// ***  ***

// ******************* main() *******************
// Performing MIP tests
//
// Notes:
// test_http_fetch() doesn't support MIP stack yet
// Compilation environment for http_fetch() disable
// test_queue() test_statechange()

int main(void) {
#if MG_ENABLE_MIP == 1
  test_queue();
  test_statechange();
#endif
  test_http_fetch();
  printf("SUCCESS\n");
  return 0;
}
