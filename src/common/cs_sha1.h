
#ifndef CS_COMMON_SHA1_H_
#define CS_COMMON_SHA1_H_

#ifndef CS_DISABLE_SHA1
#define CS_DISABLE_SHA1 0
#endif

#if !CS_DISABLE_SHA1

#include "common/platform.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
  uint32_t state[5];
  uint32_t count[2];
  unsigned char buffer[64];
} cs_sha1_ctx;

void cs_sha1_init(cs_sha1_ctx *);
void cs_sha1_update(cs_sha1_ctx *, const unsigned char *data, uint32_t len);
void cs_sha1_final(unsigned char digest[20], cs_sha1_ctx *);
void cs_hmac_sha1(const unsigned char *key, size_t key_len,
                  const unsigned char *text, size_t text_len,
                  unsigned char out[20]);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_DISABLE_SHA1 */

#endif /* CS_COMMON_SHA1_H_ */
