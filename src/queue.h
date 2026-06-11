#pragma once

#include "arch.h"

// Lock-free, single-producer / single-consumer (SPSC) message queue.
// Safe to write from an ISR and read from the main loop without locking,
// as long as only one context writes and one context reads at a time.
// Each message is stored with an internal 32-bit length prefix; the buf
// pointers returned by mg_queue_book() and mg_queue_next() already skip it.
//
// Producer pattern:
//   char buf[1024];
//   if (mg_queue_book(&q, &buf, len) >= len) {
//     memcpy(buf, data, len);
//     mg_queue_add(&q, len);
//   }
//
// Consumer pattern:
//   char *buf;
//   size_t len = mg_queue_next(&q, &buf);
//   if (len > 0) { process(buf, len); mg_queue_del(&q, len); }

struct mg_queue {
  char *buf;            // Caller-supplied backing buffer
  size_t size;          // Size of buf in bytes
  volatile size_t tail; // Read offset; updated by the consumer
  volatile size_t head; // Write offset; updated by the producer
};

// Initialises q to use the caller-supplied buf/size. buf must remain valid
// for the lifetime of the queue.
void mg_queue_init(struct mg_queue *q, char *buf, size_t size);

// Reserves space for a message of len bytes. Sets *buf to the write location.
// Returns the number of bytes available (>= len means the reservation
// succeeded). Does NOT commit the message; call mg_queue_add() after writing.
size_t mg_queue_book(struct mg_queue *q, char **buf, size_t len);

// Commits the most recently reserved message of len bytes. Must be called
// with the same len passed to the preceding mg_queue_book() call, after
// the data has been written into the buffer it returned.
void mg_queue_add(struct mg_queue *q, size_t len);

// Returns the length of the oldest unconsumed message and sets *buf to its
// start. Returns 0 if the queue is empty. Does NOT remove the message;
// call mg_queue_del() after processing to advance the read position.
size_t mg_queue_next(struct mg_queue *q, char **buf);

// Removes the oldest message of len bytes. Must be called with the same len
// returned by the preceding mg_queue_next() call.
void mg_queue_del(struct mg_queue *q, size_t len);
