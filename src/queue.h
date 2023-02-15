#pragma once

#include "arch.h"  // For size_t

// Single producer, single consumer non-blocking queue
//
// Producer:
//    char *buf;
//    while (mg_queue_book(q, &buf) < len) WAIT();  // Wait for space
//    memcpy(buf, my_data, len);   // Copy data to the queue
//    mg_queue_add(q, len);
//
// Consumer:
//    char *buf;
//    while ((len = mg_queue_get(q, &buf)) == 0) WAIT();
//    mg_hexdump(buf, len); // Handle message
//    mg_queue_del(q, len);
//
struct mg_queue {
  char *buf;
  size_t size;
  volatile size_t tail;
  volatile size_t head;
};

void mg_queue_init(struct mg_queue *, char *, size_t);        // Init queue
size_t mg_queue_book(struct mg_queue *, char **buf, size_t);  // Reserve space
void mg_queue_add(struct mg_queue *, size_t);                 // Add new message
size_t mg_queue_next(struct mg_queue *, char **);  // Get oldest message
void mg_queue_del(struct mg_queue *, size_t);      // Delete oldest message
