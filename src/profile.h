#pragma once
#include "iobuf.h"

// Macros to record timestamped events that happens with a connection.
// They are saved into a c->prof IO buffer, each event is a name and a 32-bit
// timestamp in milliseconds since connection init time.
//
// Test (run in two separate terminals):
//   make -C examples/http-server/ CFLAGS_EXTRA=-DMG_ENABLE_PROFILE=1
//   curl localhost:8000
// Output:
//   1ea1f1e7 2 net.c:150:mg_close_conn      3 profile:                                                            
//   1ea1f1e8 2 net.c:150:mg_close_conn      1ea1f1e6 init                                                         
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_OPEN
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_ACCEPT 
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_READ
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_HTTP_MSG
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_WRITE
//   1ea1f1e8 2 net.c:150:mg_close_conn          1 EV_CLOSE
//
// Usage:
//   Enable profiling by setting MG_ENABLE_PROFILE=1
//   Invoke MG_PROF_ADD(c, "MY_EVENT_1") in the places you'd like to measure

#if MG_ENABLE_PROFILE
struct mg_profitem {
  const char *name;    // Event name
  uint32_t timestamp;  // Milliseconds since connection creation (MG_EV_OPEN)
};

#define MG_PROFILE_ALLOC_GRANULARITY 256  // Can save 32 items wih to realloc

// Adding a profile item to the c->prof. Must be as fast as possible.
// Reallocation of the c->prof iobuf is not desirable here, that's why we
// pre-allocate c->prof with MG_PROFILE_ALLOC_GRANULARITY.
// This macro just inits and copies 8 bytes, and calls mg_millis(),
// which should be fast enough.
#define MG_PROF_ADD(c, name_)                                             \
  do {                                                                    \
    struct mg_iobuf *io = &c->prof;                                       \
    uint32_t inittime = ((struct mg_profitem *) io->buf)->timestamp;      \
    struct mg_profitem item = {name_, (uint32_t) mg_millis() - inittime}; \
    mg_iobuf_add(io, io->len, &item, sizeof(item));                       \
  } while (0)

// Initialising profile for a new connection. Not time sensitive
#define MG_PROF_INIT(c)                                          \
  do {                                                           \
    struct mg_profitem first = {"init", (uint32_t) mg_millis()}; \
    mg_iobuf_init(&(c)->prof, 0, MG_PROFILE_ALLOC_GRANULARITY);  \
    mg_iobuf_add(&c->prof, c->prof.len, &first, sizeof(first));  \
  } while (0)

#define MG_PROF_FREE(c) mg_iobuf_free(&(c)->prof)

// Dumping the profile. Not time sensitive
#define MG_PROF_DUMP(c)                                            \
  do {                                                             \
    struct mg_iobuf *io = &c->prof;                                \
    struct mg_profitem *p = (struct mg_profitem *) io->buf;        \
    struct mg_profitem *e = &p[io->len / sizeof(*p)];              \
    MG_INFO(("%lu profile:", c->id));                              \
    while (p < e) {                                                \
      MG_INFO(("%5lx %s", (unsigned long) p->timestamp, p->name)); \
      p++;                                                         \
    }                                                              \
  } while (0)

#else
#define MG_PROF_INIT(c)
#define MG_PROF_FREE(c)
#define MG_PROF_ADD(c, name)
#define MG_PROF_DUMP(c)
#endif
