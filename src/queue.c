#include "queue.h"

// Every message in the queue is prepended by the message length (ML)
// ML is sizeof(size_t) in size
// Tail points to the message data
//
//    |------| ML | message1 |  ML | message2 |--- free space ---|
//    ^      ^                                ^                  ^
//   buf    tail                             head               len

size_t mg_queue_space(struct mg_queue *q, char **buf) {
  size_t ofs;
  if (q->head > 0 && q->tail >= q->head) {  // All messages read?
    q->head = 0;                            // Yes. Reset head first
    q->tail = 0;                            // Now reset the tail
  }
  ofs = q->head + sizeof(size_t);
  if (buf != NULL) *buf = q->buf + ofs;
  return ofs > q->len ? 0 : q->len - ofs;
}

size_t mg_queue_next(struct mg_queue *q, char **buf) {
  size_t len = MG_QUEUE_EMPTY;
  if (q->tail < q->head) memcpy(&len, &q->buf[q->tail], sizeof(len));
  if (buf != NULL) *buf = &q->buf[q->tail + sizeof(len)];
  return len;
}

void mg_queue_add(struct mg_queue *q, size_t len) {
  size_t head = q->head + len + (size_t) sizeof(head);  // New head
  if (head <= q->len) {                                 // Have space ?
    memcpy(q->buf + q->head, &len, sizeof(len));        // Yes. Store ML
    q->head = head;                                     // Advance head
  }
}

void mg_queue_del(struct mg_queue *q) {
  size_t len = mg_queue_next(q, NULL), tail = q->tail + len + sizeof(size_t);
  if (len != MG_QUEUE_EMPTY) q->tail = tail;
}
