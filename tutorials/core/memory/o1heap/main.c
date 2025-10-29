// Copyright (c) 2025 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "o1heap.h"

#define POOL_SIZE (5 * MG_IO_SIZE)

O1HeapInstance *s_mem;

// memory allocation
void *mg_calloc(size_t count, size_t size) {
  size_t bytes = count * size;
  void *ptr = o1heapAllocate(s_mem, bytes);
  if (ptr != NULL) memset(ptr, 0, bytes);
  if (ptr != NULL) MG_DEBUG(("%lu bytes @%p", bytes, ptr));
  if (ptr == NULL) MG_ERROR(("Failed to allocate %lu bytes", bytes));
  return ptr;
}

void mg_free(void *ptr) {
  o1heapFree(s_mem, ptr);
  if (ptr != NULL) MG_DEBUG(("block @%p", ptr));
}

// HTTP request callback
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/hi"), NULL)) {
      mg_http_reply(c, 200, "Host: foo.com\r\n", "hi\n");
    } else if (mg_match(hm->uri, mg_str("/echo"), NULL)) {
      mg_http_reply(c, 200, "", "%.*s", (int) hm->body.len, hm->body.buf);
    } else if (mg_match(hm->uri, mg_str("/stats"), NULL)) {
      struct mg_connection *t;
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
      mg_http_printf_chunk(c, "ID PROTO TYPE      LOCAL           REMOTE\n");
      for (t = c->mgr->conns; t != NULL; t = t->next) {
        mg_http_printf_chunk(c, "%-3lu %4s %s %M %M\n", t->id,
                             t->is_udp ? "UDP" : "TCP",
                             t->is_listening  ? "LISTENING"
                             : t->is_accepted ? "ACCEPTED "
                                              : "CONNECTED",
                             mg_print_ip, &t->loc, mg_print_ip, &t->rem);
      }
      mg_http_printf_chunk(c, "");  // Don't forget the last empty chunk
    } else {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.root_dir = ".";
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
}

int main(void) {
  struct mg_mgr mgr;

  void *pool = malloc(POOL_SIZE + O1HEAP_ALIGNMENT);
  void *aligned = (void *) (((uintptr_t) pool) & ~(O1HEAP_ALIGNMENT - 1));
  O1HeapDiagnostics d;
  s_mem = o1heapInit(aligned, POOL_SIZE);
  d = o1heapGetDiagnostics(s_mem);
  MG_INFO(
      ("Created aligned %lu-byte pool @%p, out of a %lu-byte allocated system "
       "memory pool",
       d.capacity, s_mem, POOL_SIZE));

  mg_mgr_init(&mgr);        // Initialise event manager
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);  // Create listener
  for (;;) {                                                // Event loop
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
  return 0;
}
