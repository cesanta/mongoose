#include <assert.h>
#include "mongoose.c"

static void test_queue(void) {
  static uint8_t buf[sizeof(size_t) + sizeof(uint16_t) + 3 ]; // fit 1 element but not 2
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

int main(void) {
  test_queue();
  printf("SUCCESS\n");
  return 0;
}
