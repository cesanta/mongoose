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

void mg_sha256(uint8_t dst[32], uint8_t *data, size_t datasz) {
  mg_sha256_ctx ctx;
  mg_sha256_init(&ctx);
  mg_sha256_update(&ctx, data, datasz);
  mg_sha256_final(dst, &ctx);
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

#define rotr64(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
#define ep064(x) (rotr64(x, 28) ^ rotr64(x, 34) ^ rotr64(x, 39))
#define ep164(x) (rotr64(x, 14) ^ rotr64(x, 18) ^ rotr64(x, 41))
#define sig064(x) (rotr64(x, 1) ^ rotr64(x, 8) ^ ((x) >> 7))
#define sig164(x) (rotr64(x, 19) ^ rotr64(x, 61) ^ ((x) >> 6))

static const uint64_t mg_sha256_k2[80] = {
#if defined(__DCC__)
    0x428a2f98d728ae22ull, 0x7137449123ef65cdull, 0xb5c0fbcfec4d3b2full,
    0xe9b5dba58189dbbcull, 0x3956c25bf348b538ull, 0x59f111f1b605d019ull,
    0x923f82a4af194f9bull, 0xab1c5ed5da6d8118ull, 0xd807aa98a3030242ull,
    0x12835b0145706fbeull, 0x243185be4ee4b28cull, 0x550c7dc3d5ffb4e2ull,
    0x72be5d74f27b896full, 0x80deb1fe3b1696b1ull, 0x9bdc06a725c71235ull,
    0xc19bf174cf692694ull, 0xe49b69c19ef14ad2ull, 0xefbe4786384f25e3ull,
    0x0fc19dc68b8cd5b5ull, 0x240ca1cc77ac9c65ull, 0x2de92c6f592b0275ull,
    0x4a7484aa6ea6e483ull, 0x5cb0a9dcbd41fbd4ull, 0x76f988da831153b5ull,
    0x983e5152ee66dfabull, 0xa831c66d2db43210ull, 0xb00327c898fb213full,
    0xbf597fc7beef0ee4ull, 0xc6e00bf33da88fc2ull, 0xd5a79147930aa725ull,
    0x06ca6351e003826full, 0x142929670a0e6e70ull, 0x27b70a8546d22ffcull,
    0x2e1b21385c26c926ull, 0x4d2c6dfc5ac42aedull, 0x53380d139d95b3dfull,
    0x650a73548baf63deull, 0x766a0abb3c77b2a8ull, 0x81c2c92e47edaee6ull,
    0x92722c851482353bull, 0xa2bfe8a14cf10364ull, 0xa81a664bbc423001ull,
    0xc24b8b70d0f89791ull, 0xc76c51a30654be30ull, 0xd192e819d6ef5218ull,
    0xd69906245565a910ull, 0xf40e35855771202aull, 0x106aa07032bbd1b8ull,
    0x19a4c116b8d2d0c8ull, 0x1e376c085141ab53ull, 0x2748774cdf8eeb99ull,
    0x34b0bcb5e19b48a8ull, 0x391c0cb3c5c95a63ull, 0x4ed8aa4ae3418acbull,
    0x5b9cca4f7763e373ull, 0x682e6ff3d6b2b8a3ull, 0x748f82ee5defb2fcull,
    0x78a5636f43172f60ull, 0x84c87814a1f0ab72ull, 0x8cc702081a6439ecull,
    0x90befffa23631e28ull, 0xa4506cebde82bde9ull, 0xbef9a3f7b2c67915ull,
    0xc67178f2e372532bull, 0xca273eceea26619cull, 0xd186b8c721c0c207ull,
    0xeada7dd6cde0eb1eull, 0xf57d4f7fee6ed178ull, 0x06f067aa72176fbaull,
    0x0a637dc5a2c898a6ull, 0x113f9804bef90daeull, 0x1b710b35131c471bull,
    0x28db77f523047d84ull, 0x32caab7b40c72493ull, 0x3c9ebe0a15c9bebcull,
    0x431d67c49c100d4cull, 0x4cc5d4becb3e42b6ull, 0x597f299cfc657e2aull,
    0x5fcb6fab3ad6faecull, 0x6c44198c4a475817ull
#else
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
    0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
    0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
    0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
    0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
    0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
    0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
    0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
    0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
    0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
    0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
    0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
    0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
    0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
    0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
    0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
    0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
    0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
    0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
    0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
    0x5fcb6fab3ad6faec, 0x6c44198c4a475817
#endif
};

static void mg_sha384_transform(mg_sha384_ctx *ctx, const uint8_t data[]) {
  uint64_t m[80];
  uint64_t a, b, c, d, e, f, g, h;
  int i, j;

  for (i = 0, j = 0; i < 16; ++i, j += 8)
    m[i] = ((uint64_t) data[j] << 56) | ((uint64_t) data[j + 1] << 48) |
           ((uint64_t) data[j + 2] << 40) | ((uint64_t) data[j + 3] << 32) |
           ((uint64_t) data[j + 4] << 24) | ((uint64_t) data[j + 5] << 16) |
           ((uint64_t) data[j + 6] << 8) | ((uint64_t) data[j + 7]);
  for (; i < 80; ++i)
    m[i] = sig164(m[i - 2]) + m[i - 7] + sig064(m[i - 15]) + m[i - 16];

  a = ctx->state[0];
  b = ctx->state[1];
  c = ctx->state[2];
  d = ctx->state[3];
  e = ctx->state[4];
  f = ctx->state[5];
  g = ctx->state[6];
  h = ctx->state[7];

  for (i = 0; i < 80; ++i) {
    uint64_t t1 = h + ep164(e) + ch(e, f, g) + mg_sha256_k2[i] + m[i];
    uint64_t t2 = ep064(a) + maj(a, b, c);
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

void mg_sha384_init(mg_sha384_ctx *ctx) {
  ctx->datalen = 0;
  ctx->bitlen[0] = 0;
  ctx->bitlen[1] = 0;
#if defined(__DCC__)
  ctx->state[0] = 0xcbbb9d5dc1059ed8ull;
  ctx->state[1] = 0x629a292a367cd507ull;
  ctx->state[2] = 0x9159015a3070dd17ull;
  ctx->state[3] = 0x152fecd8f70e5939ull;
  ctx->state[4] = 0x67332667ffc00b31ull;
  ctx->state[5] = 0x8eb44a8768581511ull;
  ctx->state[6] = 0xdb0c2e0d64f98fa7ull;
  ctx->state[7] = 0x47b5481dbefa4fa4ull;
#else
  ctx->state[0] = 0xcbbb9d5dc1059ed8;
  ctx->state[1] = 0x629a292a367cd507;
  ctx->state[2] = 0x9159015a3070dd17;
  ctx->state[3] = 0x152fecd8f70e5939;
  ctx->state[4] = 0x67332667ffc00b31;
  ctx->state[5] = 0x8eb44a8768581511;
  ctx->state[6] = 0xdb0c2e0d64f98fa7;
  ctx->state[7] = 0x47b5481dbefa4fa4;
#endif
}

void mg_sha384_update(mg_sha384_ctx *ctx, const uint8_t *data, size_t len) {
  size_t i;
  for (i = 0; i < len; ++i) {
    ctx->buffer[ctx->datalen] = data[i];
    ctx->datalen++;
    if (ctx->datalen == 128) {
      mg_sha384_transform(ctx, ctx->buffer);
      ctx->bitlen[1] += 1024;
      if (ctx->bitlen[1] < 1024) ctx->bitlen[0]++;
      ctx->datalen = 0;
    }
  }
}

void mg_sha384_final(uint8_t hash[48], mg_sha384_ctx *ctx) {
  size_t i = ctx->datalen;

  if (ctx->datalen < 112) {
    ctx->buffer[i++] = 0x80;
    while (i < 112) ctx->buffer[i++] = 0x00;
  } else {
    ctx->buffer[i++] = 0x80;
    while (i < 128) ctx->buffer[i++] = 0x00;
    mg_sha384_transform(ctx, ctx->buffer);
    memset(ctx->buffer, 0, 112);
  }

  ctx->bitlen[1] += ctx->datalen * 8;
  if (ctx->bitlen[1] < ctx->datalen * 8) ctx->bitlen[0]++;
  ctx->buffer[127] = (uint8_t) (ctx->bitlen[1]);
  ctx->buffer[126] = (uint8_t) (ctx->bitlen[1] >> 8);
  ctx->buffer[125] = (uint8_t) (ctx->bitlen[1] >> 16);
  ctx->buffer[124] = (uint8_t) (ctx->bitlen[1] >> 24);
  ctx->buffer[123] = (uint8_t) (ctx->bitlen[1] >> 32);
  ctx->buffer[122] = (uint8_t) (ctx->bitlen[1] >> 40);
  ctx->buffer[121] = (uint8_t) (ctx->bitlen[1] >> 48);
  ctx->buffer[120] = (uint8_t) (ctx->bitlen[1] >> 56);
  ctx->buffer[119] = (uint8_t) (ctx->bitlen[0]);
  ctx->buffer[118] = (uint8_t) (ctx->bitlen[0] >> 8);
  ctx->buffer[117] = (uint8_t) (ctx->bitlen[0] >> 16);
  ctx->buffer[116] = (uint8_t) (ctx->bitlen[0] >> 24);
  ctx->buffer[115] = (uint8_t) (ctx->bitlen[0] >> 32);
  ctx->buffer[114] = (uint8_t) (ctx->bitlen[0] >> 40);
  ctx->buffer[113] = (uint8_t) (ctx->bitlen[0] >> 48);
  ctx->buffer[112] = (uint8_t) (ctx->bitlen[0] >> 56);
  mg_sha384_transform(ctx, ctx->buffer);

  for (i = 0; i < 6; ++i) {
    hash[i * 8] = (uint8_t) ((ctx->state[i] >> 56) & 0xff);
    hash[i * 8 + 1] = (uint8_t) ((ctx->state[i] >> 48) & 0xff);
    hash[i * 8 + 2] = (uint8_t) ((ctx->state[i] >> 40) & 0xff);
    hash[i * 8 + 3] = (uint8_t) ((ctx->state[i] >> 32) & 0xff);
    hash[i * 8 + 4] = (uint8_t) ((ctx->state[i] >> 24) & 0xff);
    hash[i * 8 + 5] = (uint8_t) ((ctx->state[i] >> 16) & 0xff);
    hash[i * 8 + 6] = (uint8_t) ((ctx->state[i] >> 8) & 0xff);
    hash[i * 8 + 7] = (uint8_t) (ctx->state[i] & 0xff);
  }
}

void mg_sha384(uint8_t dst[48], uint8_t *data, size_t datasz) {
  mg_sha384_ctx ctx;
  mg_sha384_init(&ctx);
  mg_sha384_update(&ctx, data, datasz);
  mg_sha384_final(dst, &ctx);
}
