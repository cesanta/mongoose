#include "timer.h"
#include "arch.h"

#define MG_TIMER_CALLED 4

void mg_timer_init(struct mg_timer **head, struct mg_timer *t, uint64_t ms,
                   unsigned flags, void (*fn)(void *), void *arg) {
  t->id = 0, t->period_ms = ms, t->start = 0;
  t->flags = flags, t->fn = fn, t->arg = arg, t->next = *head;
  *head = t;
}

void mg_timer_free(struct mg_timer **head, struct mg_timer *t) {
  while (*head && *head != t) head = &(*head)->next;
  if (*head) *head = t->next;
}

// t: start time, prd: period, now: current time. Return true if expired
bool mg_timer_expired(uint64_t *t, uint64_t prd, uint64_t now) {
  if (*t == 0) *t = now;             // First poll? Setup
  if (now < *t)
    *t = now;  // Handle non-monotonic time (misses 1 fire on wraparound)
  if (now - *t < prd) return false;  // Not expired yet, return
  *t = *t + prd;                     // Calculate next time, keep period
  return true;                       // Expired, return true
}

void mg_timer_poll(struct mg_timer **head, uint64_t now_ms) {
  struct mg_timer *t, *tmp;
  for (t = *head; t != NULL; t = tmp) {
    bool once = t->start == 0 && (t->flags & MG_TIMER_RUN_NOW) &&
                !(t->flags & MG_TIMER_CALLED);  // Handle MG_TIMER_RUN_NOW only once
    bool expired = mg_timer_expired(&t->start, t->period_ms, now_ms);
    tmp = t->next;
    if (!once && !expired) continue;
    if ((t->flags & MG_TIMER_REPEAT) || !(t->flags & MG_TIMER_CALLED)) {
      t->fn(t->arg);
    }
    t->flags |= MG_TIMER_CALLED;
  }
}
