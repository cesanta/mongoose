/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "common/test_util.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <sys/time.h>
#else
#include <windows.h>
#endif

int num_tests = 0;

static char *_escape(const char *s, size_t n) {
  size_t i, j;
  char *res = (char *) malloc(n * 4 + 1);
  for (i = j = 0; s[i] != '\0' && i < n; i++) {
    if (!iscntrl((int) s[i])) {
      res[j++] = s[i];
    } else {
      j += sprintf(res + j, "\\x%02x", s[i]);
    }
  }
  res[j] = '\0';
  return res;
}

void _strfail(const char *a, const char *e, int len) {
  char *ae, *ee;
  if (len < 0) {
    len = strlen(a);
    if (strlen(e) > (size_t) len) len = strlen(e);
  }
  ae = _escape(a, len);
  ee = _escape(e, len);
  printf("Expected: %s\nActual  : %s\n", ee, ae);
  free(ae);
  free(ee);
}

int _assert_streq(const char *actual, const char *expected) {
  if (strcmp(actual, expected) != 0) {
    _strfail(actual, expected, -1);
    return 0;
  }
  return 1;
}

int _assert_streq_nz(const char *actual, const char *expected) {
  size_t n = strlen(expected);
  if (strncmp(actual, expected, n) != 0) {
    _strfail(actual, expected, n);
    return 0;
  }
  return 1;
}

#if MG_ENABLE_POLL_UNTIL
int c_str_ne(void *a, void *b) {
  int r = strcmp((const char *) a, (const char *) b);
  return r;
}

int c_int_ne(void *a, void *b) {
  return *((int *) a) != (intptr_t) b;
}

int c_int_eq(void *a, void *b) {
  return *((int *) a) == (intptr_t) b;
}

void poll_until(struct mg_mgr *mgr, double timeout, int (*cond)(void *, void *),
                void *cond_arg1, void *cond_arg2) {
  int i;
  double start = cs_time();
  while (cs_time() - start < timeout) {
    mg_mgr_poll(mgr, 2);
    if (cond != NULL && cond(cond_arg1, cond_arg2)) {
      /* A few more cycles to test for overshoots. */
      for (i = 0; i < 5; i++) {
        mg_mgr_poll(mgr, 2);
      }
      return;
    }
  }
}
#endif
