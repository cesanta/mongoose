#include "timer.h"
#include "arch.h"

#define MG_TIMER_CALLED 4

void mg_timer_init(struct mg_timer **head, struct mg_timer *t, uint64_t ms,
                   unsigned flags, void (*fn)(void *), void *arg) {
  t->id = 0, t->period_ms = ms, t->expire = 0;
  t->flags = flags, t->fn = fn, t->arg = arg, t->next = *head;
  *head = t;
}

void mg_timer_free(struct mg_timer **head, struct mg_timer *t) {
  while (*head && *head != t) head = &(*head)->next;
  if (*head) *head = t->next;
}

// t: expiration time, prd: period, now: current time. Return true if expired
bool mg_timer_expired(uint64_t *t, uint64_t prd, uint64_t now) {
  if (now + prd < *t) *t = 0;                    // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd;                   // Firt poll? Set expiration
  if (*t > now) return false;                    // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd;  // Next expiration time
  return true;                                   // Expired, return true
}

void mg_timer_poll(struct mg_timer **head, uint64_t now_ms) {
  struct mg_timer *t, *tmp;
  for (t = *head; t != NULL; t = tmp) {
    bool once = t->expire == 0 && (t->flags & MG_TIMER_RUN_NOW) &&
                !(t->flags & MG_TIMER_CALLED);  // Handle MG_TIMER_NOW only once
    bool expired = mg_timer_expired(&t->expire, t->period_ms, now_ms);
    tmp = t->next;
    if (!once && !expired) continue;
    if ((t->flags & MG_TIMER_REPEAT) || !(t->flags & MG_TIMER_CALLED)) {
      t->fn(t->arg);
    }
    t->flags |= MG_TIMER_CALLED;
  }
}
