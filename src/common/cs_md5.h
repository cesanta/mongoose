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

#ifndef CS_COMMON_MD5_H_
#define CS_COMMON_MD5_H_

#include "common/platform.h"

#ifndef CS_DISABLE_MD5
#define CS_DISABLE_MD5 0
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
  uint32_t buf[4];
  uint32_t bits[2];
  unsigned char in[64];
} cs_md5_ctx;

void cs_md5_init(cs_md5_ctx *c);
void cs_md5_update(cs_md5_ctx *c, const unsigned char *data, size_t len);
void cs_md5_final(unsigned char *md, cs_md5_ctx *c);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_COMMON_MD5_H_ */
