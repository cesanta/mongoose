#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_LINES 1
#define MG_ENABLE_MIP 1
#define MG_ENABLE_PACKED_FS 0

#include <assert.h>
#include "mongoose.c"

#include "driver_mock.c"

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

int main(void) {
  test_queue();
  test_statechange();
  printf("SUCCESS\n");
  return 0;
}
