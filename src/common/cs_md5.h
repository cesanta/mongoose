/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
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
