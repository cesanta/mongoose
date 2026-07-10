#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define FRAME_SIZE 1540
#define FRAME_QUEUE_DEPTH 32
#define SHM_NAME "/mongoose-shm"

// Keep this SHM ABI semaphore-free. Some S390/Linux headers pull fcntl/socket
// declarations through <semaphore.h>, colliding with Mongoose BSD prototypes.
// Each direction is single-producer/single-consumer and polled, so a simple
// ring is enough for this test path.
struct shm_frame {
  size_t len;
  uint8_t buf[FRAME_SIZE];
};

struct shm_queue {
  volatile size_t head;
  volatile size_t tail;
  struct shm_frame frames[FRAME_QUEUE_DEPTH];
};

struct shm_link {
  struct shm_queue to_tap;
  struct shm_queue from_tap;
};

static inline bool shm_queue_push(struct shm_queue *q, const void *buf,
                                  size_t len) {
  size_t tail = q->tail, next = (tail + 1) % FRAME_QUEUE_DEPTH;
  struct shm_frame *f;
  if (len > sizeof(q->frames[0].buf) || next == q->head) return false;
  f = &q->frames[tail];
  memcpy(f->buf, buf, len);
  f->len = len;
  __sync_synchronize();
  q->tail = next;
  return true;
}

static inline size_t shm_queue_pop(struct shm_queue *q, void *buf, size_t len) {
  size_t head = q->head, n;
  struct shm_frame *f;
  if (head == q->tail) return 0;
  __sync_synchronize();
  f = &q->frames[head];
  n = f->len;
  if (n > len) {
    q->head = (head + 1) % FRAME_QUEUE_DEPTH;
    return 0;
  }
  memcpy(buf, f->buf, n);
  __sync_synchronize();
  q->head = (head + 1) % FRAME_QUEUE_DEPTH;
  return n;
}
