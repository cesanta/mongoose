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

#include "common/cs_dbg.h"
#include "common/test_main.h"
#include "common/test_util.h"

#if defined(_MSC_VER) && _MSC_VER >= 1900
#include <crtdbg.h>

int __cdecl CrtDbgHook(int nReportType, char *szMsg, int *pnRet) {
  (void) nReportType;
  (void) szMsg;
  (void) pnRet;

  fprintf(stderr, "CRT debug hook: type: %d, msg: %s\n", nReportType, szMsg);
  /* Return true - Abort,Retry,Ignore dialog will *not* be displayed */
  return 1;
}
#endif

#ifndef __cdecl
#define __cdecl
#endif

int g_num_tests = 0;
int g_num_checks = 0;
const char *g_argv_0 = NULL;

int __cdecl main(int argc, char *argv[]) {
  const char *fail_msg;
  const char *filter = argc > 1 ? argv[1] : "";
  char *seed_str = getenv("TEST_SEED");
  double started;
  int seed = 0;

  if (seed_str != NULL) {
    seed = atoi(seed_str);
  } else {
    seed = (int) time(NULL);
  }
  printf("seed: %d\n", seed);
  srand(seed);

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
#if defined(_MSC_VER) && _MSC_VER >= 1900
  /* NOTE: not available on wine/vc6 */
  _CrtSetReportHook2(_CRT_RPTHOOK_INSTALL, CrtDbgHook);
#endif

  g_argv_0 = argv[0];

  tests_setup();

  started = cs_time();
  fail_msg = tests_run(filter);
  printf("%s, ran %d tests (%d checks) in %.3lfs\n", fail_msg ? "FAIL" : "PASS",
         g_num_tests, g_num_checks, cs_time() - started);

  tests_teardown();

  if (fail_msg != NULL) {
    /* Prevent leak analyzer from running: there will be "leaks" because of
     * premature return from the test, and in this case we don't care. */
    _exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
