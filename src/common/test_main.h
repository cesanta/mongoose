/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_TEST_MAIN_H_
#define CS_COMMON_TEST_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

void tests_setup(void);
const char *tests_run(const char *filter);
void tests_teardown(void);
extern const char *g_argv_0;

#ifdef __cplusplus
}
#endif

#endif /* CS_COMMON_TEST_MAIN_H_ */
