#pragma once

#include "arch.h"

struct mg_timer {
  int64_t period_ms;        // Timer period in milliseconds
  int64_t expire;           // Expiration timestamp in milliseconds
  unsigned flags;           // Possible flags values below
#define MG_TIMER_REPEAT 1   // Call function periodically, otherwise run once
#define MG_TIMER_RUN_NOW 2  // Call immediately when timer is set
  void (*fn)(void *);       // Function to call
  void *arg;                // Function argument
  struct mg_timer *next;    // Linkage in g_timers list
};

extern struct mg_timer *g_timers;  // Global list of timers

void mg_timer_init(struct mg_timer *, int64_t, unsigned, void (*)(void *),
                   void *);
void mg_timer_free(struct mg_timer *);
void mg_timer_poll(int64_t current_time_ms);
