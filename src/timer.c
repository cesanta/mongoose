// Copyright (c) Cesanta Software Limited
// All rights reserved

#include "timer.h"
#include "arch.h"

struct mg_timer *g_timers;

void mg_timer_init(struct mg_timer *t, int ms, int flags, void (*fn)(void *),
                   void *arg) {
  struct mg_timer tmp = {ms, flags, fn, arg, 0UL, g_timers};
  *t = tmp;
  g_timers = t;
  if (flags & MG_TIMER_RUN_NOW) fn(arg);
}

void mg_timer_free(struct mg_timer *t) {
  struct mg_timer **head = &g_timers;
  while (*head && *head != t) head = &(*head)->next;
  if (*head) *head = t->next;
}

void mg_timer_poll(unsigned long now_ms) {
  // If time goes back (wrapped around), reset timers
  struct mg_timer *t, *tmp;
  static unsigned long oldnow;  // Timestamp in a previous invocation
  if (oldnow > now_ms) {        // If it is wrapped, reset timers
    for (t = g_timers; t != NULL; t = t->next) t->expire = 0;
  }
  oldnow = now_ms;

  for (t = g_timers; t != NULL; t = tmp) {
    tmp = t->next;
    if (t->expire == 0) t->expire = now_ms + t->period_ms;
    if (t->expire > now_ms) continue;
    t->fn(t->arg);
    // Try to tick timers with the given period as accurate as possible,
    // even if this polling function is called with some random period.
    t->expire = now_ms - t->expire > (unsigned long) t->period_ms
                    ? now_ms + t->period_ms
                    : t->expire + t->period_ms;
    if (!(t->flags & MG_TIMER_REPEAT)) mg_timer_free(t);
  }
}
