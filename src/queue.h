#pragma once

#include "arch.h"    // For size_t
#include "config.h"  // For MG_ENABLE_ATOMIC

#if MG_ENABLE_ATOMIC
#include <stdatomic.h>
#elif !defined(_Atomic)
#define _Atomic
#endif

// Single producer, single consumer non-blocking queue
//
// Producer:
//    void *buf;
//    while (mg_queue_space(q, &buf) < len) WAIT();  // Wait for free space
//    memcpy(buf, data, len);  // Copy data to the queue
//    mg_queue_add(q, len);    // Advance q->head
//
// Consumer:
//    void *buf;
//    while ((len = mg_queue_next(q, &buf)) == MG_QUEUE_EMPTY) WAIT();
//    mg_hexdump(buf, len);    // Handle message
//    mg_queue_del(q);         // Delete message
//
struct mg_queue {
  char *buf;
  size_t len;
  volatile _Atomic size_t tail;
  volatile _Atomic size_t head;
};

#define MG_QUEUE_EMPTY ((size_t) ~0ul)  // Next message size when queue is empty

void mg_queue_add(struct mg_queue *, size_t len);       // Advance head
void mg_queue_del(struct mg_queue *);                   // Advance tail
size_t mg_queue_space(struct mg_queue *, char **);      // Get free space
size_t mg_queue_next(struct mg_queue *, char **);       // Get next message size
