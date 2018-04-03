/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CS_COMMON_TEST_UTIL_H_
#define CS_COMMON_TEST_UTIL_H_

#include <string.h>
#include <stdio.h>

#include "common/cs_time.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int g_num_tests;
extern int g_num_checks;

#ifdef MG_TEST_ABORT_ON_FAIL
#define MG_TEST_ABORT abort()
#else
#define MG_TEST_ABORT
#endif

int _assert_streq(const char *actual, const char *expected);
int _assert_streq_nz(const char *actual, const char *expected);
void _strfail(const char *a, const char *e, int len);

#define FAIL(str, line)                                              \
  do {                                                               \
    printf("%s:%d:1 [%s] (in %s)\n", __FILE__, line, str, __func__); \
    MG_TEST_ABORT;                                                   \
    return str;                                                      \
  } while (0)

#define ASSERT(expr)                    \
  do {                                  \
    g_num_checks++;                     \
    if (!(expr)) FAIL(#expr, __LINE__); \
  } while (0)
#define ASSERT_TRUE(expr) ASSERT(expr)
#define ASSERT_FALSE(expr) ASSERT(!(expr))

/*
 * Run test function, use its name as the test name to print
 */
#define RUN_TEST(test) RUN_TEST_WNAME(test, #test)

/*
 * Run test function, use the provided name as the test name to print
 */
#define RUN_TEST_WNAME(test, test_name)            \
  do {                                             \
    const char *msg = NULL;                        \
    if (strstr(test_name, filter)) {               \
      double elapsed = cs_time();                  \
      msg = test();                                \
      elapsed = cs_time() - elapsed;               \
      printf("  [%.3f] %s\n", elapsed, test_name); \
      fflush(stdout);                              \
      g_num_tests++;                               \
    }                                              \
    if (msg) return msg;                           \
  } while (0)

/* VC6 doesn't have long long. */
#if (defined(_MSC_VER) && _MSC_VER <= 1200)
#define LONG_TYPE int64_t
#else
#define LONG_TYPE long long
#endif
#define AS_DOUBLE(d) (double)(d)

#define RANGE_CHECK(x) (fabs(x) > 4503599627370496.0 /* 2 << 51 */)

/*
 * Numeric equality assertion.
 * Args are cast to double to ensure aruemnts are only evaluated once.
 * This will work fine for all integer types except large 64-bit values.
 */
#define ASSERT_EQ(actual, expected)                         \
  do {                                                      \
    const double ad = AS_DOUBLE(actual);                    \
    const double ed = AS_DOUBLE(expected);                  \
    if (RANGE_CHECK(ad) || RANGE_CHECK(ed)) {               \
      FAIL("loss of precision, use ASSERT_EQ64", __LINE__); \
    }                                                       \
    g_num_checks++;                                         \
    if (!(ad == ed)) {                                      \
      printf("%lf != %lf\n", ad, ed);                       \
      FAIL(#actual " != " #expected, __LINE__);             \
    }                                                       \
  } while (0)

#define ASSERT_EQ64(actual, expected)            \
  do {                                           \
    const LONG_TYPE a64 = (LONG_TYPE)(actual);   \
    const LONG_TYPE e64 = (LONG_TYPE)(expected); \
    g_num_checks++;                              \
    if (!(a64 == e64)) {                         \
      printf("%lld != %lld\n", a64, e64);        \
      FAIL(#actual " != " #expected, __LINE__);  \
    }                                            \
  } while (0)

#define ASSERT_NE(actual, expected)                         \
  do {                                                      \
    const double ad = AS_DOUBLE(actual);                    \
    const double ed = AS_DOUBLE(expected);                  \
    g_num_checks++;                                         \
    if (RANGE_CHECK(ad) || RANGE_CHECK(ed)) {               \
      FAIL("loss of precision, use ASSERT_NE64", __LINE__); \
    }                                                       \
    if (!(ad != ed)) {                                      \
      printf("%lf == %lf\n", ad, ed);                       \
      FAIL(#actual " == " #expected, __LINE__);             \
    }                                                       \
  } while (0)

#define ASSERT_NE64(actual, expected)            \
  do {                                           \
    const LONG_TYPE a64 = (LONG_TYPE)(actual);   \
    const LONG_TYPE e64 = (LONG_TYPE)(expected); \
    g_num_checks++;                              \
    if (!(a64 != e64)) {                         \
      printf("%lld == %lld\n", a64, e64);        \
      FAIL(#actual " == " #expected, __LINE__);  \
    }                                            \
  } while (0)

/* "Less than" assertion. */
#define ASSERT_LT(a, b)                                  \
  do {                                                   \
    const double ad = AS_DOUBLE(a);                      \
    const double bd = AS_DOUBLE(b);                      \
    g_num_checks++;                                      \
    if (RANGE_CHECK(ad) || RANGE_CHECK(bd)) {            \
      FAIL("loss of precision in comparison", __LINE__); \
    }                                                    \
    if (!(ad < bd)) {                                    \
      printf("%lf >= %lf\n", ad, bd);                    \
      FAIL(#a " < " #b, __LINE__);                       \
    }                                                    \
  } while (0)

/* "Greater than" assertion. */
#define ASSERT_GT(a, b)               \
  do {                                \
    const double ad = AS_DOUBLE(a);   \
    const double bd = AS_DOUBLE(b);   \
    g_num_checks++;                   \
    if (!(ad > bd)) {                 \
      printf("%lf <= %lf\n", ad, bd); \
      FAIL(#a " > " #b, __LINE__);    \
    }                                 \
  } while (0)

/* Assert that actual == expected, where both are NUL-terminated. */
#define ASSERT_STREQ(actual, expected)                            \
  do {                                                            \
    g_num_checks++;                                               \
    if (!_assert_streq((actual), (expected))) {                   \
      FAIL("ASSERT_STREQ(" #actual ", " #expected ")", __LINE__); \
    }                                                             \
  } while (0)

/* Assert that actual == expected, where both are pointers */
#define ASSERT_PTREQ(actual, expected)                            \
  do {                                                            \
    const void *ap = (const void *) (actual);                     \
    const void *ep = (const void *) (expected);                   \
    g_num_checks++;                                               \
    if (!(ap == ep)) {                                            \
      printf("%p != %p\n", ap, ep);                               \
      FAIL("ASSERT_PTREQ(" #actual ", " #expected ")", __LINE__); \
    }                                                             \
  } while (0)

/* Assert that actual != expected, where both are pointers */
#define ASSERT_PTRNE(actual, expected)                            \
  do {                                                            \
    const void *ap = (const void *) (actual);                     \
    const void *ep = (const void *) (expected);                   \
    g_num_checks++;                                               \
    if (!(ap != ep)) {                                            \
      printf("%p == %p\n", ap, ep);                               \
      FAIL("ASSERT_PTRNE(" #actual ", " #expected ")", __LINE__); \
    }                                                             \
  } while (0)

/* Same as STREQ, but only expected is NUL-terminated. */
#define ASSERT_STREQ_NZ(actual, expected)                            \
  do {                                                               \
    g_num_checks++;                                                  \
    if (!_assert_streq_nz(actual, expected)) {                       \
      FAIL("ASSERT_STREQ_NZ(" #actual ", " #expected ")", __LINE__); \
    }                                                                \
  } while (0)

#define ASSERT_MG_STREQ(actual, expected)                            \
  do {                                                               \
    g_num_checks++;                                                  \
    if ((actual).len != strlen(expected) ||                          \
        memcmp((actual).p, expected, (actual).len) != 0) {           \
      printf("'%.*s' (%d) != '%s'\n", (int)(actual).len, (actual).p, \
             (int)(actual).len, expected);                           \
      FAIL("ASSERT_MG_STREQ(" #actual ", " #expected ")", __LINE__); \
    }                                                                \
  } while (0)

#define CHECK_CALL(call)                                                  \
  do {                                                                    \
    const char *r = (call);                                               \
    if (r != NULL) {                                                      \
      fprintf(stderr, "... %s:%d in %s\n", __FILE__, __LINE__, __func__); \
      return r;                                                           \
    }                                                                     \
  } while (0)

#ifndef MG_ENABLE_POLL_UNTIL
#define MG_ENABLE_POLL_UNTIL 0
#endif

#if MG_ENABLE_POLL_UNTIL
#include "mongoose.h"
int c_str_ne(void *a, void *b);
int c_int_ne(void *a, void *b);
int c_int_eq(void *a, void *b);
void poll_until(struct mg_mgr *mgr, double timeout, int (*cond)(void *, void *),
                void *cond_arg1, void *cond_arg2);
#endif

#ifdef __cplusplus
}
#endif

#endif /* CS_COMMON_TEST_UTIL_H_ */
