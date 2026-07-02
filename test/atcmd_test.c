#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0
#define MG_ENABLE_DRIVER_ATCMD 1

#include "mongoose.c"

static int s_num_tests = 0;
static bool s_error = false;

#ifdef NO_ABORT
static int s_abort = 0;
#define ABORT() ++s_abort, s_error = true
#else
#ifdef NO_SLEEP_ABORT
#define ABORT() abort()
#else
#define ABORT()                       \
  sleep(2); /* 2s, GH print reason */ \
  abort();
#endif
#endif

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      fflush(stdout);                                           \
      ABORT();                                                  \
    }                                                           \
  } while (0)

struct uart_mock {
  uint8_t rx[2048];
  uint8_t tx[256];
  size_t rx_len;
  size_t rx_pos;
  size_t tx_len;
  int reset_count;
};

struct test_ctx {
  struct mg_tcpip_if ifp;
  struct mg_tcpip_driver_atcmd_data dd;
  struct uart_mock mock;
  char qbuf[1506];
};

static void mock_reset(void *arg) {
  struct uart_mock *mock = (struct uart_mock *) arg;
  mock->reset_count++;
}

static void mock_tx(void *arg, uint8_t b) {
  struct uart_mock *mock = (struct uart_mock *) arg;
  ASSERT(mock->tx_len < sizeof(mock->tx));
  mock->tx[mock->tx_len++] = b;
}

static int mock_rx(void *arg) {
  struct uart_mock *mock = (struct uart_mock *) arg;
  return mock->rx_pos < mock->rx_len ? mock->rx[mock->rx_pos++] : -1;
}

static void mock_rx_add(struct uart_mock *mock, const void *buf, size_t len) {
  ASSERT(mock->rx_len + len <= sizeof(mock->rx));
  memcpy(mock->rx + mock->rx_len, buf, len);
  mock->rx_len += len;
}

static void init_if(struct test_ctx *ctx) {
  memset(ctx, 0, sizeof(*ctx));
  ctx->ifp.driver_data = &ctx->dd;
  ASSERT(mg_atcmd_init(&ctx->ifp) == true);
  ASSERT(ctx->ifp.recv_queue.size <= sizeof(ctx->qbuf));
  ctx->ifp.recv_queue.buf = ctx->qbuf;
}

static void reset_if(struct test_ctx *ctx, const char **script,
                     bool no_byte_stuff) {
  memset(&ctx->dd, 0, sizeof(ctx->dd));
  memset(&ctx->mock, 0, sizeof(ctx->mock));
  ctx->ifp.driver_data = &ctx->dd;
  ctx->ifp.recv_queue.head = 0;
  ctx->ifp.recv_queue.tail = 0;
  ctx->dd.usart = &ctx->mock;
  ctx->dd.reset = mock_reset;
  ctx->dd.tx = mock_tx;
  ctx->dd.rx = mock_rx;
  ctx->dd.script = script;
  ctx->dd.no_byte_stuff = no_byte_stuff;
}

#define DASHBOARD(x) \
  printf("HEALTH_DASHBOARD\t\"%s\": %s,\n", x, s_error ? "false" : "true")

int main(void) {
  struct test_ctx ctx;
  init_if(&ctx);

  s_error = false;
  {  // Send AT command, receive OK
    static const char *script[] = {"AT\r\n", "\r\nOK\r\n", NULL};
    static const uint8_t at[] = {'A', 'T', '\r', '\n'};
    static const uint8_t ok[] = {'\r', '\n', 'O', 'K', '\r', '\n'};
    reset_if(&ctx, script, false);

    ASSERT(mg_atcmd_poll(&ctx.ifp, true) == false);
    ASSERT(ctx.mock.tx_len == sizeof(at));
    ASSERT(memcmp(ctx.mock.tx, at, sizeof(at)) == 0);
    ASSERT(ctx.dd.link == false);
    mock_rx_add(&ctx.mock, ok, sizeof(ok));
    ASSERT(mg_atcmd_poll(&ctx.ifp, true) == true);
    ASSERT(ctx.dd.link == true);
    ASSERT(ctx.dd.script_index == -1);
    ASSERT(ctx.mock.reset_count == 0);
  }
  DASHBOARD("atcmd");

  s_error = false;
  {  // HDLC byte stuffing and unstuffing
    static const uint8_t payload[] = {0x00, 0x01, 'A', 0x7d, 0x7e, 0x20};
    static const uint8_t frame[] = {0x7e, 0x7d, 0x20, 0x7d, 0x21, 'A',
                                    0x7d, 0x5d, 0x7d, 0x5e, 0x20, 0x7e};
    uint8_t buf[sizeof(payload)];
    reset_if(&ctx, NULL, false);
    ASSERT(mg_atcmd_poll(&ctx.ifp, true) == true);

    ASSERT(mg_atcmd_tx(payload, sizeof(payload), &ctx.ifp) == sizeof(payload));
    ASSERT(ctx.mock.tx_len == sizeof(frame));
    ASSERT(memcmp(ctx.mock.tx, frame, sizeof(frame)) == 0);

    mock_rx_add(&ctx.mock, frame, sizeof(frame));
    ASSERT(mg_atcmd_rx(buf, sizeof(buf), &ctx.ifp) == sizeof(payload));
    ASSERT(memcmp(buf, payload, sizeof(payload)) == 0);
  }
  {  // HDLC framing with byte stuffing disabled
    static const uint8_t payload[] = {0x00, 0x01, 'A', 0x7d, 0x20};
    static const uint8_t frame[] = {0x7e, 0x00, 0x01, 'A', 0x7d, 0x20, 0x7e};
    uint8_t buf[sizeof(payload)];
    reset_if(&ctx, NULL, true);
    ASSERT(mg_atcmd_poll(&ctx.ifp, true) == true);

    ASSERT(mg_atcmd_tx(payload, sizeof(payload), &ctx.ifp) == sizeof(payload));
    ASSERT(ctx.mock.tx_len == sizeof(frame));
    ASSERT(memcmp(ctx.mock.tx, frame, sizeof(frame)) == 0);

    mock_rx_add(&ctx.mock, frame, sizeof(frame));
    ASSERT(mg_atcmd_rx(buf, sizeof(buf), &ctx.ifp) == sizeof(payload));
    ASSERT(memcmp(buf, payload, sizeof(payload)) == 0);
  }
  {  // Oversized frame is rejected
    uint8_t buf[8];
    reset_if(&ctx, NULL, true);
    ASSERT(mg_atcmd_poll(&ctx.ifp, true) == true);

    mock_rx_add(&ctx.mock, "\x7e", 1);
    memset(ctx.mock.rx + ctx.mock.rx_len, 'A', sizeof(buf) + 1);
    ctx.mock.rx_len += sizeof(buf) + 1;
    mock_rx_add(&ctx.mock, "\x7e", 1);

    ASSERT(mg_atcmd_rx(buf, sizeof(buf), &ctx.ifp) == 0);
  }
  printf("HEALTH_DASHBOARD\t\"hdlc\": %s\n",
         s_error ? "false" : "true");

#ifdef NO_ABORT
  if (s_abort != 0) return EXIT_FAILURE;
#endif

  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return EXIT_SUCCESS;
}
