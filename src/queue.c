#include "queue.h"
#include "util.h"

#if defined(__GNUC__) || defined(__clang__)
#define MG_MEMORY_BARRIER() __sync_synchronize()
#elif defined(_MSC_VER) && _MSC_VER >= 1700
#define MG_MEMORY_BARRIER() MemoryBarrier()
#elif !defined(MG_MEMORY_BARRIER)
#define MG_MEMORY_BARRIER()
#endif

// Every message in a queue is prepended by a 32-bit message length (ML).
// If ML is 0, then it is the end, and reader must wrap to the beginning.
//
//  Queue when q->tail <= q->head:
//  |----- free -----| ML | message1 | ML | message2 |  ----- free ------|
//  ^                ^                               ^                   ^
// buf              tail                            head                len
//
//  Queue when q->tail > q->head:
//  | ML | message2 |----- free ------| ML | message1 | 0 |---- free ----|
//  ^               ^                 ^                                  ^
// buf             head              tail                               len

void mg_queue_init(struct mg_queue *q, char *buf, size_t size) {
  q->size = size;
  q->buf = buf;
  q->head = q->tail = 0;
}

static size_t mg_queue_read_len(struct mg_queue *q) {
  uint32_t n = 0;
  MG_MEMORY_BARRIER();
  memcpy(&n, q->buf + q->tail, sizeof(n));
  assert(q->tail + n + sizeof(n) <= q->size);
  return n;
}

static void mg_queue_write_len(struct mg_queue *q, size_t len) {
  uint32_t n = (uint32_t) len;
  memcpy(q->buf + q->head, &n, sizeof(n));
  MG_MEMORY_BARRIER();
}

size_t mg_queue_book(struct mg_queue *q, char **buf, size_t len) {
  size_t space = 0, hs = sizeof(uint32_t) * 2;  // *2 is for the 0 marker
  if (q->head >= q->tail && q->head + len + hs <= q->size) {
    space = q->size - q->head - hs;  // There is enough space
  } else if (q->head >= q->tail && q->tail > hs) {
    mg_queue_write_len(q, 0);  // Not enough space ahead
    q->head = 0;               // Wrap head to the beginning
  }
  if (q->head + hs + len < q->tail) space = q->tail - q->head - hs;
  if (buf != NULL) *buf = q->buf + q->head + sizeof(uint32_t);
  return space;
}

size_t mg_queue_next(struct mg_queue *q, char **buf) {
  size_t len = 0;
  if (q->tail != q->head) {
    len = mg_queue_read_len(q);
    if (len == 0) {  // Zero (head wrapped) ?
      q->tail = 0;   // Reset tail to the start
      if (q->head > q->tail) len = mg_queue_read_len(q);  // Read again
    }
  }
  if (buf != NULL) *buf = q->buf + q->tail + sizeof(uint32_t);
  assert(q->tail + len <= q->size);
  return len;
}

void mg_queue_add(struct mg_queue *q, size_t len) {
  assert(len > 0);
  mg_queue_write_len(q, len);
  assert(q->head + sizeof(uint32_t) * 2 + len <= q->size);
  q->head += len + sizeof(uint32_t);
}

void mg_queue_del(struct mg_queue *q, size_t len) {
  q->tail += len + sizeof(uint32_t);
  assert(q->tail + sizeof(uint32_t) <= q->size);
}
