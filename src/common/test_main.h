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
