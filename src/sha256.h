// https://github.com/B-Con/crypto-algorithms
// Author:     Brad Conte (brad AT bradconte.com)
// Disclaimer: This code is presented "as is" without any guarantees.
// Details:    Defines the API for the corresponding SHA1 implementation.
// Copyright:  public domain

#pragma once

#include "arch.h"

typedef struct {
  uint32_t state[8];
  uint64_t bits;
  uint32_t len;
  unsigned char buffer[64];
} mg_sha256_ctx;


void mg_sha256_init(mg_sha256_ctx *);
void mg_sha256_update(mg_sha256_ctx *, const unsigned char *data, size_t len);
void mg_sha256_final(unsigned char digest[32], mg_sha256_ctx *);
void mg_sha256(uint8_t dst[32], uint8_t *data, size_t datasz);
void mg_hmac_sha256(uint8_t dst[32], uint8_t *key, size_t keysz, uint8_t *data,
                    size_t datasz);

typedef struct {
    uint64_t state[8];
    uint8_t buffer[128];
    uint64_t bitlen[2];
    uint32_t datalen;
} mg_sha384_ctx;
void mg_sha384_init(mg_sha384_ctx *ctx);
void mg_sha384_update(mg_sha384_ctx *ctx, const uint8_t *data, size_t len);
void mg_sha384_final(uint8_t digest[48], mg_sha384_ctx *ctx);
void mg_sha384(uint8_t dst[48], uint8_t *data, size_t datasz);

