// https://github.com/B-Con/crypto-algorithms
// Author:     Brad Conte (brad AT bradconte.com)
// Disclaimer: This code is presented "as is" without any guarantees.
// Details:    Defines the API for the corresponding SHA1 implementation.
// Copyright:  public domain

#include "sha256.h"

#define ror(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define ch(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define maj(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define ep0(x) (ror(x, 2) ^ ror(x, 13) ^ ror(x, 22))
#define ep1(x) (ror(x, 6) ^ ror(x, 11) ^ ror(x, 25))
#define sig0(x) (ror(x, 7) ^ ror(x, 18) ^ ((x) >> 3))
#define sig1(x) (ror(x, 17) ^ ror(x, 19) ^ ((x) >> 10))

static const uint32_t mg_sha256_k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void mg_sha256_init(mg_sha256_ctx *ctx) {
  ctx->len = 0;
  ctx->bits = 0;
  ctx->state[0] = 0x6a09e667;
  ctx->state[1] = 0xbb67ae85;
  ctx->state[2] = 0x3c6ef372;
  ctx->state[3] = 0xa54ff53a;
  ctx->state[4] = 0x510e527f;
  ctx->state[5] = 0x9b05688c;
  ctx->state[6] = 0x1f83d9ab;
  ctx->state[7] = 0x5be0cd19;
}

static void mg_sha256_chunk(mg_sha256_ctx *ctx) {
  int i, j;
  uint32_t a, b, c, d, e, f, g, h;
  uint32_t m[64];
  for (i = 0, j = 0; i < 16; ++i, j += 4)
    m[i] = (uint32_t) (((uint32_t) ctx->buffer[j] << 24) |
                       ((uint32_t) ctx->buffer[j + 1] << 16) |
                       ((uint32_t) ctx->buffer[j + 2] << 8) |
                       ((uint32_t) ctx->buffer[j + 3]));
  for (; i < 64; ++i)
    m[i] = sig1(m[i - 2]) + m[i - 7] + sig0(m[i - 15]) + m[i - 16];

  a = ctx->state[0];
  b = ctx->state[1];
  c = ctx->state[2];
  d = ctx->state[3];
  e = ctx->state[4];
  f = ctx->state[5];
  g = ctx->state[6];
  h = ctx->state[7];

  for (i = 0; i < 64; ++i) {
    uint32_t t1 = h + ep1(e) + ch(e, f, g) + mg_sha256_k[i] + m[i];
    uint32_t t2 = ep0(a) + maj(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
  ctx->state[4] += e;
  ctx->state[5] += f;
  ctx->state[6] += g;
  ctx->state[7] += h;
}

void mg_sha256_update(mg_sha256_ctx *ctx, const unsigned char *data,
                      size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    ctx->buffer[ctx->len] = data[i];
    if ((++ctx->len) == 64) {
      mg_sha256_chunk(ctx);
      ctx->bits += 512;
      ctx->len = 0;
    }
  }
}

// TODO: make final reusable (remove side effects)
void mg_sha256_final(unsigned char digest[32], mg_sha256_ctx *ctx) {
  uint32_t i = ctx->len;
  if (i < 56) {
    ctx->buffer[i++] = 0x80;
    while (i < 56) {
      ctx->buffer[i++] = 0x00;
    }
  } else {
    ctx->buffer[i++] = 0x80;
    while (i < 64) {
      ctx->buffer[i++] = 0x00;
    }
    mg_sha256_chunk(ctx);
    memset(ctx->buffer, 0, 56);
  }

  ctx->bits += ctx->len * 8;
  ctx->buffer[63] = (uint8_t) ((ctx->bits) & 0xff);
  ctx->buffer[62] = (uint8_t) ((ctx->bits >> 8) & 0xff);
  ctx->buffer[61] = (uint8_t) ((ctx->bits >> 16) & 0xff);
  ctx->buffer[60] = (uint8_t) ((ctx->bits >> 24) & 0xff);
  ctx->buffer[59] = (uint8_t) ((ctx->bits >> 32) & 0xff);
  ctx->buffer[58] = (uint8_t) ((ctx->bits >> 40) & 0xff);
  ctx->buffer[57] = (uint8_t) ((ctx->bits >> 48) & 0xff);
  ctx->buffer[56] = (uint8_t) ((ctx->bits >> 56) & 0xff);
  mg_sha256_chunk(ctx);

  for (i = 0; i < 4; ++i) {
    digest[i] = (uint8_t) ((ctx->state[0] >> (24 - i * 8)) & 0xff);
    digest[i + 4] = (uint8_t) ((ctx->state[1] >> (24 - i * 8)) & 0xff);
    digest[i + 8] = (uint8_t) ((ctx->state[2] >> (24 - i * 8)) & 0xff);
    digest[i + 12] = (uint8_t) ((ctx->state[3] >> (24 - i * 8)) & 0xff);
    digest[i + 16] = (uint8_t) ((ctx->state[4] >> (24 - i * 8)) & 0xff);
    digest[i + 20] = (uint8_t) ((ctx->state[5] >> (24 - i * 8)) & 0xff);
    digest[i + 24] = (uint8_t) ((ctx->state[6] >> (24 - i * 8)) & 0xff);
    digest[i + 28] = (uint8_t) ((ctx->state[7] >> (24 - i * 8)) & 0xff);
  }
}

void mg_hmac_sha256(uint8_t dst[32], uint8_t *key, size_t keysz, uint8_t *data,
                    size_t datasz) {
  mg_sha256_ctx ctx;
  uint8_t k[64] = {0};
  uint8_t o_pad[64], i_pad[64];
  unsigned int i;
  memset(i_pad, 0x36, sizeof(i_pad));
  memset(o_pad, 0x5c, sizeof(o_pad));
  if (keysz < 64) {
    if (keysz > 0) memmove(k, key, keysz);
  } else {
    mg_sha256_init(&ctx);
    mg_sha256_update(&ctx, key, keysz);
    mg_sha256_final(k, &ctx);
  }
  for (i = 0; i < sizeof(k); i++) {
    i_pad[i] ^= k[i];
    o_pad[i] ^= k[i];
  }
  mg_sha256_init(&ctx);
  mg_sha256_update(&ctx, i_pad, sizeof(i_pad));
  mg_sha256_update(&ctx, data, datasz);
  mg_sha256_final(dst, &ctx);
  mg_sha256_init(&ctx);
  mg_sha256_update(&ctx, o_pad, sizeof(o_pad));
  mg_sha256_update(&ctx, dst, 32);
  mg_sha256_final(dst, &ctx);
}
