// portable8439 v1.0.1
// Source: https://github.com/DavyLandman/portable8439
// Licensed under CC0-1.0
// Contains poly1305-donna e6ad6e091d30d7f4ec2d4f978be1fcfcbce72781 (Public
// Domain)

#include "tls_chacha20.h"
#include "tls.h"

#if MG_TLS == MG_TLS_BUILTIN
// ******* BEGIN: chacha-portable/chacha-portable.h ********

#if !defined(__cplusplus) && !defined(_MSC_VER) && \
    (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#error "C99 or newer required"
#endif

#define CHACHA20_KEY_SIZE (32)
#define CHACHA20_NONCE_SIZE (12)

#if defined(_MSC_VER) || defined(__cplusplus)
// add restrict support
#if ((defined(_MSC_VER) && _MSC_VER >= 1900) && !defined(__cplusplus)) || \
    defined(__clang__) || defined(__GNUC__)
#define restrict __restrict
#else
#define restrict
#endif
#endif

// xor data with a ChaCha20 keystream as per RFC8439
static PORTABLE_8439_DECL void chacha20_xor_stream(
    uint8_t *restrict dest, const uint8_t *restrict source, size_t length,
    const uint8_t key[CHACHA20_KEY_SIZE],
    const uint8_t nonce[CHACHA20_NONCE_SIZE], uint32_t counter);

static PORTABLE_8439_DECL void rfc8439_keygen(
    uint8_t poly_key[32], const uint8_t key[CHACHA20_KEY_SIZE],
    const uint8_t nonce[CHACHA20_NONCE_SIZE]);

// ******* END:   chacha-portable/chacha-portable.h ********
// ******* BEGIN: poly1305-donna/poly1305-donna.h ********

#include <stddef.h>

typedef struct poly1305_context {
  size_t aligner;
  unsigned char opaque[136];
} poly1305_context;

static PORTABLE_8439_DECL void poly1305_init(poly1305_context *ctx,
                                             const unsigned char key[32]);
static PORTABLE_8439_DECL void poly1305_update(poly1305_context *ctx,
                                               const unsigned char *m,
                                               size_t bytes);
static PORTABLE_8439_DECL void poly1305_finish(poly1305_context *ctx,
                                               unsigned char mac[16]);

// ******* END:   poly1305-donna/poly1305-donna.h ********
// ******* BEGIN: chacha-portable.c ********

#include <assert.h>
#include <string.h>

// this is a fresh implementation of chacha20, based on the description in
// rfc8349 it's such a nice compact algorithm that it is easy to do. In
// relationship to other c implementation this implementation:
//  - pure c99
//  - big & little endian support
//  - safe for architectures that don't support unaligned reads
//
// Next to this, we try to be fast as possible without resorting inline
// assembly.

// based on https://sourceforge.net/p/predef/wiki/Endianness/
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && \
    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __HAVE_LITTLE_ENDIAN 1
#elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) ||                 \
    defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || \
    defined(__MIPSEL) || defined(__MIPSEL__) || defined(__XTENSA_EL__) || \
    defined(__AVR__) || defined(LITTLE_ENDIAN)
#define __HAVE_LITTLE_ENDIAN 1
#endif

#ifndef TEST_SLOW_PATH
#if defined(__HAVE_LITTLE_ENDIAN)
#define FAST_PATH
#endif
#endif

#define CHACHA20_STATE_WORDS (16)
#define CHACHA20_BLOCK_SIZE (CHACHA20_STATE_WORDS * sizeof(uint32_t))

#ifdef FAST_PATH
#define store_32_le(target, source) memcpy(&(target), source, sizeof(uint32_t))
#else
#define store_32_le(target, source)                                 \
  target = (uint32_t) (source)[0] | ((uint32_t) (source)[1]) << 8 | \
           ((uint32_t) (source)[2]) << 16 | ((uint32_t) (source)[3]) << 24
#endif

static void initialize_state(uint32_t state[CHACHA20_STATE_WORDS],
                             const uint8_t key[CHACHA20_KEY_SIZE],
                             const uint8_t nonce[CHACHA20_NONCE_SIZE],
                             uint32_t counter) {
#if 0
#ifdef static_assert
  static_assert(sizeof(uint32_t) == 4,
                "We don't support systems that do not conform to standard of "
                "uint32_t being exact 32bit wide");
#endif
#endif
  state[0] = 0x61707865;
  state[1] = 0x3320646e;
  state[2] = 0x79622d32;
  state[3] = 0x6b206574;
  store_32_le(state[4], key);
  store_32_le(state[5], key + 4);
  store_32_le(state[6], key + 8);
  store_32_le(state[7], key + 12);
  store_32_le(state[8], key + 16);
  store_32_le(state[9], key + 20);
  store_32_le(state[10], key + 24);
  store_32_le(state[11], key + 28);
  state[12] = counter;
  store_32_le(state[13], nonce);
  store_32_le(state[14], nonce + 4);
  store_32_le(state[15], nonce + 8);
}

#define increment_counter(state) (state)[12]++

// source: http://blog.regehr.org/archives/1063
#define rotl32a(x, n) ((x) << (n)) | ((x) >> (32 - (n)))

#define Qround(a, b, c, d) \
  a += b;                  \
  d ^= a;                  \
  d = rotl32a(d, 16);      \
  c += d;                  \
  b ^= c;                  \
  b = rotl32a(b, 12);      \
  a += b;                  \
  d ^= a;                  \
  d = rotl32a(d, 8);       \
  c += d;                  \
  b ^= c;                  \
  b = rotl32a(b, 7);

#define TIMES16(x)                                                          \
  x(0) x(1) x(2) x(3) x(4) x(5) x(6) x(7) x(8) x(9) x(10) x(11) x(12) x(13) \
      x(14) x(15)

static void core_block(const uint32_t *restrict start,
                       uint32_t *restrict output) {
  int i;
// instead of working on the output array,
// we let the compiler allocate 16 local variables on the stack
#define __LV(i) uint32_t __t##i = start[i];
  TIMES16(__LV)

#define __Q(a, b, c, d) Qround(__t##a, __t##b, __t##c, __t##d)

  for (i = 0; i < 10; i++) {
    __Q(0, 4, 8, 12);
    __Q(1, 5, 9, 13);
    __Q(2, 6, 10, 14);
    __Q(3, 7, 11, 15);
    __Q(0, 5, 10, 15);
    __Q(1, 6, 11, 12);
    __Q(2, 7, 8, 13);
    __Q(3, 4, 9, 14);
  }

#define __FIN(i) output[i] = start[i] + __t##i;
  TIMES16(__FIN)
}

#define U8(x) ((uint8_t) ((x) &0xFF))

#ifdef FAST_PATH
#define xor32_le(dst, src, pad)            \
  uint32_t __value;                        \
  memcpy(&__value, src, sizeof(uint32_t)); \
  __value ^= *(pad);                       \
  memcpy(dst, &__value, sizeof(uint32_t));
#else
#define xor32_le(dst, src, pad)           \
  (dst)[0] = (src)[0] ^ U8(*(pad));       \
  (dst)[1] = (src)[1] ^ U8(*(pad) >> 8);  \
  (dst)[2] = (src)[2] ^ U8(*(pad) >> 16); \
  (dst)[3] = (src)[3] ^ U8(*(pad) >> 24);
#endif

#define index8_32(a, ix) ((a) + ((ix) * sizeof(uint32_t)))

#define xor32_blocks(dest, source, pad, words)                    \
  for (i = 0; i < words; i++) {                                   \
    xor32_le(index8_32(dest, i), index8_32(source, i), (pad) + i) \
  }

static void xor_block(uint8_t *restrict dest, const uint8_t *restrict source,
                      const uint32_t *restrict pad, unsigned int chunk_size) {
  unsigned int i, full_blocks = chunk_size / (unsigned int) sizeof(uint32_t);
  // have to be carefull, we are going back from uint32 to uint8, so endianness
  // matters again
  xor32_blocks(dest, source, pad, full_blocks)

      dest += full_blocks * sizeof(uint32_t);
  source += full_blocks * sizeof(uint32_t);
  pad += full_blocks;

  switch (chunk_size % sizeof(uint32_t)) {
    case 1:
      dest[0] = source[0] ^ U8(*pad);
      break;
    case 2:
      dest[0] = source[0] ^ U8(*pad);
      dest[1] = source[1] ^ U8(*pad >> 8);
      break;
    case 3:
      dest[0] = source[0] ^ U8(*pad);
      dest[1] = source[1] ^ U8(*pad >> 8);
      dest[2] = source[2] ^ U8(*pad >> 16);
      break;
  }
}

static void chacha20_xor_stream(uint8_t *restrict dest,
                                const uint8_t *restrict source, size_t length,
                                const uint8_t key[CHACHA20_KEY_SIZE],
                                const uint8_t nonce[CHACHA20_NONCE_SIZE],
                                uint32_t counter) {
  uint32_t state[CHACHA20_STATE_WORDS];
  uint32_t pad[CHACHA20_STATE_WORDS];
  size_t i, b, last_block, full_blocks = length / CHACHA20_BLOCK_SIZE;
  initialize_state(state, key, nonce, counter);
  for (b = 0; b < full_blocks; b++) {
    core_block(state, pad);
    increment_counter(state);
    xor32_blocks(dest, source, pad, CHACHA20_STATE_WORDS) dest +=
        CHACHA20_BLOCK_SIZE;
    source += CHACHA20_BLOCK_SIZE;
  }
  last_block = length % CHACHA20_BLOCK_SIZE;
  if (last_block > 0) {
    core_block(state, pad);
    xor_block(dest, source, pad, (unsigned int) last_block);
  }
}

#ifdef FAST_PATH
#define serialize(poly_key, result) memcpy(poly_key, result, 32)
#else
#define store32_le(target, source)   \
  (target)[0] = U8(*(source));       \
  (target)[1] = U8(*(source) >> 8);  \
  (target)[2] = U8(*(source) >> 16); \
  (target)[3] = U8(*(source) >> 24);

#define serialize(poly_key, result)                 \
  for (i = 0; i < 32 / sizeof(uint32_t); i++) {     \
    store32_le(index8_32(poly_key, i), result + i); \
  }
#endif

static void rfc8439_keygen(uint8_t poly_key[32],
                           const uint8_t key[CHACHA20_KEY_SIZE],
                           const uint8_t nonce[CHACHA20_NONCE_SIZE]) {
  uint32_t state[CHACHA20_STATE_WORDS];
  uint32_t result[CHACHA20_STATE_WORDS];
  size_t i;
  initialize_state(state, key, nonce, 0);
  core_block(state, result);
  serialize(poly_key, result);
  (void) i;
}
// ******* END: chacha-portable.c ********
// ******* BEGIN: poly1305-donna.c ********

/* auto detect between 32bit / 64bit */
#if /* uint128 available on 64bit system*/                              \
    (defined(__SIZEOF_INT128__) &&                                      \
     defined(__LP64__))                       /* MSVC 64bit compiler */ \
    || (defined(_MSC_VER) && defined(_M_X64)) /* gcc >= 4.4 64bit */    \
    || (defined(__GNUC__) && defined(__LP64__) &&                       \
        ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 4))))
#define __GUESS64
#else
#define __GUESS32
#endif

#if defined(POLY1305_8BIT)
/*
        poly1305 implementation using 8 bit * 8 bit = 16 bit multiplication and
32 bit addition

        based on the public domain reference version in supercop by djb
static */

#if defined(_MSC_VER) && _MSC_VER < 1700
#define POLY1305_NOINLINE
#elif defined(_MSC_VER)
#define POLY1305_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define POLY1305_NOINLINE __attribute__((noinline))
#else
#define POLY1305_NOINLINE
#endif

#define poly1305_block_size 16

/* 17 + sizeof(size_t) + 51*sizeof(unsigned char) */
typedef struct poly1305_state_internal_t {
  unsigned char buffer[poly1305_block_size];
  size_t leftover;
  unsigned char h[17];
  unsigned char r[17];
  unsigned char pad[17];
  unsigned char final;
} poly1305_state_internal_t;

static void poly1305_init(poly1305_context *ctx, const unsigned char key[32]) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;
  size_t i;

  st->leftover = 0;

  /* h = 0 */
  for (i = 0; i < 17; i++) st->h[i] = 0;

  /* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
  st->r[0] = key[0] & 0xff;
  st->r[1] = key[1] & 0xff;
  st->r[2] = key[2] & 0xff;
  st->r[3] = key[3] & 0x0f;
  st->r[4] = key[4] & 0xfc;
  st->r[5] = key[5] & 0xff;
  st->r[6] = key[6] & 0xff;
  st->r[7] = key[7] & 0x0f;
  st->r[8] = key[8] & 0xfc;
  st->r[9] = key[9] & 0xff;
  st->r[10] = key[10] & 0xff;
  st->r[11] = key[11] & 0x0f;
  st->r[12] = key[12] & 0xfc;
  st->r[13] = key[13] & 0xff;
  st->r[14] = key[14] & 0xff;
  st->r[15] = key[15] & 0x0f;
  st->r[16] = 0;

  /* save pad for later */
  for (i = 0; i < 16; i++) st->pad[i] = key[i + 16];
  st->pad[16] = 0;

  st->final = 0;
}

static void poly1305_add(unsigned char h[17], const unsigned char c[17]) {
  unsigned short u;
  unsigned int i;
  for (u = 0, i = 0; i < 17; i++) {
    u += (unsigned short) h[i] + (unsigned short) c[i];
    h[i] = (unsigned char) u & 0xff;
    u >>= 8;
  }
}

static void poly1305_squeeze(unsigned char h[17], unsigned long hr[17]) {
  unsigned long u;
  unsigned int i;
  u = 0;
  for (i = 0; i < 16; i++) {
    u += hr[i];
    h[i] = (unsigned char) u & 0xff;
    u >>= 8;
  }
  u += hr[16];
  h[16] = (unsigned char) u & 0x03;
  u >>= 2;
  u += (u << 2); /* u *= 5; */
  for (i = 0; i < 16; i++) {
    u += h[i];
    h[i] = (unsigned char) u & 0xff;
    u >>= 8;
  }
  h[16] += (unsigned char) u;
}

static void poly1305_freeze(unsigned char h[17]) {
  const unsigned char minusp[17] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0xfc};
  unsigned char horig[17], negative;
  unsigned int i;

  /* compute h + -p */
  for (i = 0; i < 17; i++) horig[i] = h[i];
  poly1305_add(h, minusp);

  /* select h if h < p, or h + -p if h >= p */
  negative = -(h[16] >> 7);
  for (i = 0; i < 17; i++) h[i] ^= negative & (horig[i] ^ h[i]);
}

static void poly1305_blocks(poly1305_state_internal_t *st,
                            const unsigned char *m, size_t bytes) {
  const unsigned char hibit = st->final ^ 1; /* 1 << 128 */

  while (bytes >= poly1305_block_size) {
    unsigned long hr[17], u;
    unsigned char c[17];
    unsigned int i, j;

    /* h += m */
    for (i = 0; i < 16; i++) c[i] = m[i];
    c[16] = hibit;
    poly1305_add(st->h, c);

    /* h *= r */
    for (i = 0; i < 17; i++) {
      u = 0;
      for (j = 0; j <= i; j++) {
        u += (unsigned short) st->h[j] * st->r[i - j];
      }
      for (j = i + 1; j < 17; j++) {
        unsigned long v = (unsigned short) st->h[j] * st->r[i + 17 - j];
        v = ((v << 8) + (v << 6)); /* v *= (5 << 6); */
        u += v;
      }
      hr[i] = u;
    }

    /* (partial) h %= p */
    poly1305_squeeze(st->h, hr);

    m += poly1305_block_size;
    bytes -= poly1305_block_size;
  }
}

static POLY1305_NOINLINE void poly1305_finish(poly1305_context *ctx,
                                              unsigned char mac[16]) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;
  size_t i;

  /* process the remaining block */
  if (st->leftover) {
    size_t i = st->leftover;
    st->buffer[i++] = 1;
    for (; i < poly1305_block_size; i++) st->buffer[i] = 0;
    st->final = 1;
    poly1305_blocks(st, st->buffer, poly1305_block_size);
  }

  /* fully reduce h */
  poly1305_freeze(st->h);

  /* h = (h + pad) % (1 << 128) */
  poly1305_add(st->h, st->pad);
  for (i = 0; i < 16; i++) mac[i] = st->h[i];

  /* zero out the state */
  for (i = 0; i < 17; i++) st->h[i] = 0;
  for (i = 0; i < 17; i++) st->r[i] = 0;
  for (i = 0; i < 17; i++) st->pad[i] = 0;
}
#elif defined(POLY1305_16BIT)
/*
        poly1305 implementation using 16 bit * 16 bit = 32 bit multiplication
and 32 bit addition static */

#if defined(_MSC_VER) && _MSC_VER < 1700
#define POLY1305_NOINLINE
#elif defined(_MSC_VER)
#define POLY1305_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define POLY1305_NOINLINE __attribute__((noinline))
#else
#define POLY1305_NOINLINE
#endif

#define poly1305_block_size 16

/* 17 + sizeof(size_t) + 18*sizeof(unsigned short) */
typedef struct poly1305_state_internal_t {
  unsigned char buffer[poly1305_block_size];
  size_t leftover;
  unsigned short r[10];
  unsigned short h[10];
  unsigned short pad[8];
  unsigned char final;
} poly1305_state_internal_t;

/* interpret two 8 bit unsigned integers as a 16 bit unsigned integer in little
 * endian */
static unsigned short U8TO16(const unsigned char *p) {
  return (((unsigned short) (p[0] & 0xff)) |
          ((unsigned short) (p[1] & 0xff) << 8));
}

/* store a 16 bit unsigned integer as two 8 bit unsigned integers in little
 * endian */
static void U16TO8(unsigned char *p, unsigned short v) {
  p[0] = (v) &0xff;
  p[1] = (v >> 8) & 0xff;
}

static void poly1305_init(poly1305_context *ctx, const unsigned char key[32]) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;
  unsigned short t0, t1, t2, t3, t4, t5, t6, t7;
  size_t i;

  /* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
  t0 = U8TO16(&key[0]);
  st->r[0] = (t0) &0x1fff;
  t1 = U8TO16(&key[2]);
  st->r[1] = ((t0 >> 13) | (t1 << 3)) & 0x1fff;
  t2 = U8TO16(&key[4]);
  st->r[2] = ((t1 >> 10) | (t2 << 6)) & 0x1f03;
  t3 = U8TO16(&key[6]);
  st->r[3] = ((t2 >> 7) | (t3 << 9)) & 0x1fff;
  t4 = U8TO16(&key[8]);
  st->r[4] = ((t3 >> 4) | (t4 << 12)) & 0x00ff;
  st->r[5] = ((t4 >> 1)) & 0x1ffe;
  t5 = U8TO16(&key[10]);
  st->r[6] = ((t4 >> 14) | (t5 << 2)) & 0x1fff;
  t6 = U8TO16(&key[12]);
  st->r[7] = ((t5 >> 11) | (t6 << 5)) & 0x1f81;
  t7 = U8TO16(&key[14]);
  st->r[8] = ((t6 >> 8) | (t7 << 8)) & 0x1fff;
  st->r[9] = ((t7 >> 5)) & 0x007f;

  /* h = 0 */
  for (i = 0; i < 10; i++) st->h[i] = 0;

  /* save pad for later */
  for (i = 0; i < 8; i++) st->pad[i] = U8TO16(&key[16 + (2 * i)]);

  st->leftover = 0;
  st->final = 0;
}

static void poly1305_blocks(poly1305_state_internal_t *st,
                            const unsigned char *m, size_t bytes) {
  const unsigned short hibit = (st->final) ? 0 : (1 << 11); /* 1 << 128 */
  unsigned short t0, t1, t2, t3, t4, t5, t6, t7;
  unsigned long d[10];
  unsigned long c;

  while (bytes >= poly1305_block_size) {
    size_t i, j;

    /* h += m[i] */
    t0 = U8TO16(&m[0]);
    st->h[0] += (t0) &0x1fff;
    t1 = U8TO16(&m[2]);
    st->h[1] += ((t0 >> 13) | (t1 << 3)) & 0x1fff;
    t2 = U8TO16(&m[4]);
    st->h[2] += ((t1 >> 10) | (t2 << 6)) & 0x1fff;
    t3 = U8TO16(&m[6]);
    st->h[3] += ((t2 >> 7) | (t3 << 9)) & 0x1fff;
    t4 = U8TO16(&m[8]);
    st->h[4] += ((t3 >> 4) | (t4 << 12)) & 0x1fff;
    st->h[5] += ((t4 >> 1)) & 0x1fff;
    t5 = U8TO16(&m[10]);
    st->h[6] += ((t4 >> 14) | (t5 << 2)) & 0x1fff;
    t6 = U8TO16(&m[12]);
    st->h[7] += ((t5 >> 11) | (t6 << 5)) & 0x1fff;
    t7 = U8TO16(&m[14]);
    st->h[8] += ((t6 >> 8) | (t7 << 8)) & 0x1fff;
    st->h[9] += ((t7 >> 5)) | hibit;

    /* h *= r, (partial) h %= p */
    for (i = 0, c = 0; i < 10; i++) {
      d[i] = c;
      for (j = 0; j < 10; j++) {
        d[i] += (unsigned long) st->h[j] *
                ((j <= i) ? st->r[i - j] : (5 * st->r[i + 10 - j]));
        /* Sum(h[i] * r[i] * 5) will overflow slightly above 6 products with an
         * unclamped r, so carry at 5 */
        if (j == 4) {
          c = (d[i] >> 13);
          d[i] &= 0x1fff;
        }
      }
      c += (d[i] >> 13);
      d[i] &= 0x1fff;
    }
    c = ((c << 2) + c); /* c *= 5 */
    c += d[0];
    d[0] = ((unsigned short) c & 0x1fff);
    c = (c >> 13);
    d[1] += c;

    for (i = 0; i < 10; i++) st->h[i] = (unsigned short) d[i];

    m += poly1305_block_size;
    bytes -= poly1305_block_size;
  }
}

static POLY1305_NOINLINE void poly1305_finish(poly1305_context *ctx,
                                              unsigned char mac[16]) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;
  unsigned short c;
  unsigned short g[10];
  unsigned short mask;
  unsigned long f;
  size_t i;

  /* process the remaining block */
  if (st->leftover) {
    size_t i = st->leftover;
    st->buffer[i++] = 1;
    for (; i < poly1305_block_size; i++) st->buffer[i] = 0;
    st->final = 1;
    poly1305_blocks(st, st->buffer, poly1305_block_size);
  }

  /* fully carry h */
  c = st->h[1] >> 13;
  st->h[1] &= 0x1fff;
  for (i = 2; i < 10; i++) {
    st->h[i] += c;
    c = st->h[i] >> 13;
    st->h[i] &= 0x1fff;
  }
  st->h[0] += (c * 5);
  c = st->h[0] >> 13;
  st->h[0] &= 0x1fff;
  st->h[1] += c;
  c = st->h[1] >> 13;
  st->h[1] &= 0x1fff;
  st->h[2] += c;

  /* compute h + -p */
  g[0] = st->h[0] + 5;
  c = g[0] >> 13;
  g[0] &= 0x1fff;
  for (i = 1; i < 10; i++) {
    g[i] = st->h[i] + c;
    c = g[i] >> 13;
    g[i] &= 0x1fff;
  }

  /* select h if h < p, or h + -p if h >= p */
  mask = (c ^ 1) - 1;
  for (i = 0; i < 10; i++) g[i] &= mask;
  mask = ~mask;
  for (i = 0; i < 10; i++) st->h[i] = (st->h[i] & mask) | g[i];

  /* h = h % (2^128) */
  st->h[0] = ((st->h[0]) | (st->h[1] << 13)) & 0xffff;
  st->h[1] = ((st->h[1] >> 3) | (st->h[2] << 10)) & 0xffff;
  st->h[2] = ((st->h[2] >> 6) | (st->h[3] << 7)) & 0xffff;
  st->h[3] = ((st->h[3] >> 9) | (st->h[4] << 4)) & 0xffff;
  st->h[4] = ((st->h[4] >> 12) | (st->h[5] << 1) | (st->h[6] << 14)) & 0xffff;
  st->h[5] = ((st->h[6] >> 2) | (st->h[7] << 11)) & 0xffff;
  st->h[6] = ((st->h[7] >> 5) | (st->h[8] << 8)) & 0xffff;
  st->h[7] = ((st->h[8] >> 8) | (st->h[9] << 5)) & 0xffff;

  /* mac = (h + pad) % (2^128) */
  f = (unsigned long) st->h[0] + st->pad[0];
  st->h[0] = (unsigned short) f;
  for (i = 1; i < 8; i++) {
    f = (unsigned long) st->h[i] + st->pad[i] + (f >> 16);
    st->h[i] = (unsigned short) f;
  }

  for (i = 0; i < 8; i++) U16TO8(mac + (i * 2), st->h[i]);

  /* zero out the state */
  for (i = 0; i < 10; i++) st->h[i] = 0;
  for (i = 0; i < 10; i++) st->r[i] = 0;
  for (i = 0; i < 8; i++) st->pad[i] = 0;
}
#elif defined(POLY1305_32BIT) || \
    (!defined(POLY1305_64BIT) && defined(__GUESS32))
/*
        poly1305 implementation using 32 bit * 32 bit = 64 bit multiplication
and 64 bit addition static */

#if defined(_MSC_VER) && _MSC_VER < 1700
#define POLY1305_NOINLINE
#elif defined(_MSC_VER)
#define POLY1305_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define POLY1305_NOINLINE __attribute__((noinline))
#else
#define POLY1305_NOINLINE
#endif

#define poly1305_block_size 16

/* 17 + sizeof(size_t) + 14*sizeof(unsigned long) */
typedef struct poly1305_state_internal_t {
  unsigned long r[5];
  unsigned long h[5];
  unsigned long pad[4];
  size_t leftover;
  unsigned char buffer[poly1305_block_size];
  unsigned char final;
} poly1305_state_internal_t;

/* interpret four 8 bit unsigned integers as a 32 bit unsigned integer in little
 * endian */
static unsigned long U8TO32(const unsigned char *p) {
  return (((unsigned long) (p[0] & 0xff)) |
          ((unsigned long) (p[1] & 0xff) << 8) |
          ((unsigned long) (p[2] & 0xff) << 16) |
          ((unsigned long) (p[3] & 0xff) << 24));
}

/* store a 32 bit unsigned integer as four 8 bit unsigned integers in little
 * endian */
static void U32TO8(unsigned char *p, unsigned long v) {
  p[0] = (unsigned char) ((v) &0xff);
  p[1] = (unsigned char) ((v >> 8) & 0xff);
  p[2] = (unsigned char) ((v >> 16) & 0xff);
  p[3] = (unsigned char) ((v >> 24) & 0xff);
}

static void poly1305_init(poly1305_context *ctx, const unsigned char key[32]) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;

  /* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
  st->r[0] = (U8TO32(&key[0])) & 0x3ffffff;
  st->r[1] = (U8TO32(&key[3]) >> 2) & 0x3ffff03;
  st->r[2] = (U8TO32(&key[6]) >> 4) & 0x3ffc0ff;
  st->r[3] = (U8TO32(&key[9]) >> 6) & 0x3f03fff;
  st->r[4] = (U8TO32(&key[12]) >> 8) & 0x00fffff;

  /* h = 0 */
  st->h[0] = 0;
  st->h[1] = 0;
  st->h[2] = 0;
  st->h[3] = 0;
  st->h[4] = 0;

  /* save pad for later */
  st->pad[0] = U8TO32(&key[16]);
  st->pad[1] = U8TO32(&key[20]);
  st->pad[2] = U8TO32(&key[24]);
  st->pad[3] = U8TO32(&key[28]);

  st->leftover = 0;
  st->final = 0;
}

static void poly1305_blocks(poly1305_state_internal_t *st,
                            const unsigned char *m, size_t bytes) {
  const unsigned long hibit = (st->final) ? 0 : (1UL << 24); /* 1 << 128 */
  unsigned long r0, r1, r2, r3, r4;
  unsigned long s1, s2, s3, s4;
  unsigned long h0, h1, h2, h3, h4;
  uint64_t d0, d1, d2, d3, d4;
  unsigned long c;

  r0 = st->r[0];
  r1 = st->r[1];
  r2 = st->r[2];
  r3 = st->r[3];
  r4 = st->r[4];

  s1 = r1 * 5;
  s2 = r2 * 5;
  s3 = r3 * 5;
  s4 = r4 * 5;

  h0 = st->h[0];
  h1 = st->h[1];
  h2 = st->h[2];
  h3 = st->h[3];
  h4 = st->h[4];

  while (bytes >= poly1305_block_size) {
    /* h += m[i] */
    h0 += (U8TO32(m + 0)) & 0x3ffffff;
    h1 += (U8TO32(m + 3) >> 2) & 0x3ffffff;
    h2 += (U8TO32(m + 6) >> 4) & 0x3ffffff;
    h3 += (U8TO32(m + 9) >> 6) & 0x3ffffff;
    h4 += (U8TO32(m + 12) >> 8) | hibit;

    /* h *= r */
    d0 = ((uint64_t) h0 * r0) + ((uint64_t) h1 * s4) + ((uint64_t) h2 * s3) +
         ((uint64_t) h3 * s2) + ((uint64_t) h4 * s1);
    d1 = ((uint64_t) h0 * r1) + ((uint64_t) h1 * r0) + ((uint64_t) h2 * s4) +
         ((uint64_t) h3 * s3) + ((uint64_t) h4 * s2);
    d2 = ((uint64_t) h0 * r2) + ((uint64_t) h1 * r1) + ((uint64_t) h2 * r0) +
         ((uint64_t) h3 * s4) + ((uint64_t) h4 * s3);
    d3 = ((uint64_t) h0 * r3) + ((uint64_t) h1 * r2) + ((uint64_t) h2 * r1) +
         ((uint64_t) h3 * r0) + ((uint64_t) h4 * s4);
    d4 = ((uint64_t) h0 * r4) + ((uint64_t) h1 * r3) + ((uint64_t) h2 * r2) +
         ((uint64_t) h3 * r1) + ((uint64_t) h4 * r0);

    /* (partial) h %= p */
    c = (unsigned long) (d0 >> 26);
    h0 = (unsigned long) d0 & 0x3ffffff;
    d1 += c;
    c = (unsigned long) (d1 >> 26);
    h1 = (unsigned long) d1 & 0x3ffffff;
    d2 += c;
    c = (unsigned long) (d2 >> 26);
    h2 = (unsigned long) d2 & 0x3ffffff;
    d3 += c;
    c = (unsigned long) (d3 >> 26);
    h3 = (unsigned long) d3 & 0x3ffffff;
    d4 += c;
    c = (unsigned long) (d4 >> 26);
    h4 = (unsigned long) d4 & 0x3ffffff;
    h0 += c * 5;
    c = (h0 >> 26);
    h0 = h0 & 0x3ffffff;
    h1 += c;

    m += poly1305_block_size;
    bytes -= poly1305_block_size;
  }

  st->h[0] = h0;
  st->h[1] = h1;
  st->h[2] = h2;
  st->h[3] = h3;
  st->h[4] = h4;
}

static POLY1305_NOINLINE void poly1305_finish(poly1305_context *ctx,
                                              unsigned char mac[16]) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;
  unsigned long h0, h1, h2, h3, h4, c;
  unsigned long g0, g1, g2, g3, g4;
  uint64_t f;
  unsigned long mask;

  /* process the remaining block */
  if (st->leftover) {
    size_t i = st->leftover;
    st->buffer[i++] = 1;
    for (; i < poly1305_block_size; i++) st->buffer[i] = 0;
    st->final = 1;
    poly1305_blocks(st, st->buffer, poly1305_block_size);
  }

  /* fully carry h */
  h0 = st->h[0];
  h1 = st->h[1];
  h2 = st->h[2];
  h3 = st->h[3];
  h4 = st->h[4];

  c = h1 >> 26;
  h1 = h1 & 0x3ffffff;
  h2 += c;
  c = h2 >> 26;
  h2 = h2 & 0x3ffffff;
  h3 += c;
  c = h3 >> 26;
  h3 = h3 & 0x3ffffff;
  h4 += c;
  c = h4 >> 26;
  h4 = h4 & 0x3ffffff;
  h0 += c * 5;
  c = h0 >> 26;
  h0 = h0 & 0x3ffffff;
  h1 += c;

  /* compute h + -p */
  g0 = h0 + 5;
  c = g0 >> 26;
  g0 &= 0x3ffffff;
  g1 = h1 + c;
  c = g1 >> 26;
  g1 &= 0x3ffffff;
  g2 = h2 + c;
  c = g2 >> 26;
  g2 &= 0x3ffffff;
  g3 = h3 + c;
  c = g3 >> 26;
  g3 &= 0x3ffffff;
  g4 = h4 + c - (1UL << 26);

  /* select h if h < p, or h + -p if h >= p */
  mask = (g4 >> ((sizeof(unsigned long) * 8) - 1)) - 1;
  g0 &= mask;
  g1 &= mask;
  g2 &= mask;
  g3 &= mask;
  g4 &= mask;
  mask = ~mask;
  h0 = (h0 & mask) | g0;
  h1 = (h1 & mask) | g1;
  h2 = (h2 & mask) | g2;
  h3 = (h3 & mask) | g3;
  h4 = (h4 & mask) | g4;

  /* h = h % (2^128) */
  h0 = ((h0) | (h1 << 26)) & 0xffffffff;
  h1 = ((h1 >> 6) | (h2 << 20)) & 0xffffffff;
  h2 = ((h2 >> 12) | (h3 << 14)) & 0xffffffff;
  h3 = ((h3 >> 18) | (h4 << 8)) & 0xffffffff;

  /* mac = (h + pad) % (2^128) */
  f = (uint64_t) h0 + st->pad[0];
  h0 = (unsigned long) f;
  f = (uint64_t) h1 + st->pad[1] + (f >> 32);
  h1 = (unsigned long) f;
  f = (uint64_t) h2 + st->pad[2] + (f >> 32);
  h2 = (unsigned long) f;
  f = (uint64_t) h3 + st->pad[3] + (f >> 32);
  h3 = (unsigned long) f;

  U32TO8(mac + 0, h0);
  U32TO8(mac + 4, h1);
  U32TO8(mac + 8, h2);
  U32TO8(mac + 12, h3);

  /* zero out the state */
  st->h[0] = 0;
  st->h[1] = 0;
  st->h[2] = 0;
  st->h[3] = 0;
  st->h[4] = 0;
  st->r[0] = 0;
  st->r[1] = 0;
  st->r[2] = 0;
  st->r[3] = 0;
  st->r[4] = 0;
  st->pad[0] = 0;
  st->pad[1] = 0;
  st->pad[2] = 0;
  st->pad[3] = 0;
}

#else
/*
        poly1305 implementation using 64 bit * 64 bit = 128 bit multiplication
and 128 bit addition static */

#if defined(_MSC_VER)

typedef struct uint128_t {
  uint64_t lo;
  uint64_t hi;
} uint128_t;

#define MUL128(out, x, y) out.lo = _umul128((x), (y), &out.hi)
#define ADD(out, in)                \
  {                                 \
    uint64_t t = out.lo;            \
    out.lo += in.lo;                \
    out.hi += (out.lo < t) + in.hi; \
  }
#define ADDLO(out, in)      \
  {                         \
    uint64_t t = out.lo;    \
    out.lo += in;           \
    out.hi += (out.lo < t); \
  }
#define SHR(in, shift) (__shiftright128(in.lo, in.hi, (shift)))
#define LO(in) (in.lo)

#if defined(_MSC_VER) && _MSC_VER < 1700
#define POLY1305_NOINLINE
#else
#define POLY1305_NOINLINE __declspec(noinline)
#endif
#elif defined(__GNUC__)
#if defined(__SIZEOF_INT128__)
// Get rid of GCC warning "ISO C does not support '__int128' types"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
typedef unsigned __int128 uint128_t;
#pragma GCC diagnostic pop
#else
typedef unsigned uint128_t __attribute__((mode(TI)));
#endif

#define MUL128(out, x, y) out = ((uint128_t) x * y)
#define ADD(out, in) out += in
#define ADDLO(out, in) out += in
#define SHR(in, shift) (uint64_t)(in >> (shift))
#define LO(in) (uint64_t)(in)

#define POLY1305_NOINLINE __attribute__((noinline))
#endif

#define poly1305_block_size 16

/* 17 + sizeof(size_t) + 8*sizeof(uint64_t) */
typedef struct poly1305_state_internal_t {
  uint64_t r[3];
  uint64_t h[3];
  uint64_t pad[2];
  size_t leftover;
  unsigned char buffer[poly1305_block_size];
  unsigned char final;
} poly1305_state_internal_t;

/* interpret eight 8 bit unsigned integers as a 64 bit unsigned integer in
 * little endian */
static uint64_t U8TO64(const unsigned char *p) {
  return (((uint64_t) (p[0] & 0xff)) | ((uint64_t) (p[1] & 0xff) << 8) |
          ((uint64_t) (p[2] & 0xff) << 16) | ((uint64_t) (p[3] & 0xff) << 24) |
          ((uint64_t) (p[4] & 0xff) << 32) | ((uint64_t) (p[5] & 0xff) << 40) |
          ((uint64_t) (p[6] & 0xff) << 48) | ((uint64_t) (p[7] & 0xff) << 56));
}

/* store a 64 bit unsigned integer as eight 8 bit unsigned integers in little
 * endian */
static void U64TO8(unsigned char *p, uint64_t v) {
  p[0] = (unsigned char) ((v) &0xff);
  p[1] = (unsigned char) ((v >> 8) & 0xff);
  p[2] = (unsigned char) ((v >> 16) & 0xff);
  p[3] = (unsigned char) ((v >> 24) & 0xff);
  p[4] = (unsigned char) ((v >> 32) & 0xff);
  p[5] = (unsigned char) ((v >> 40) & 0xff);
  p[6] = (unsigned char) ((v >> 48) & 0xff);
  p[7] = (unsigned char) ((v >> 56) & 0xff);
}

static void poly1305_init(poly1305_context *ctx, const unsigned char key[32]) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;
  uint64_t t0, t1;

  /* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
  t0 = U8TO64(&key[0]);
  t1 = U8TO64(&key[8]);

  st->r[0] = (t0) &0xffc0fffffff;
  st->r[1] = ((t0 >> 44) | (t1 << 20)) & 0xfffffc0ffff;
  st->r[2] = ((t1 >> 24)) & 0x00ffffffc0f;

  /* h = 0 */
  st->h[0] = 0;
  st->h[1] = 0;
  st->h[2] = 0;

  /* save pad for later */
  st->pad[0] = U8TO64(&key[16]);
  st->pad[1] = U8TO64(&key[24]);

  st->leftover = 0;
  st->final = 0;
}

static void poly1305_blocks(poly1305_state_internal_t *st,
                            const unsigned char *m, size_t bytes) {
  const uint64_t hibit = (st->final) ? 0 : ((uint64_t) 1 << 40); /* 1 << 128 */
  uint64_t r0, r1, r2;
  uint64_t s1, s2;
  uint64_t h0, h1, h2;
  uint64_t c;
  uint128_t d0, d1, d2, d;

  r0 = st->r[0];
  r1 = st->r[1];
  r2 = st->r[2];

  h0 = st->h[0];
  h1 = st->h[1];
  h2 = st->h[2];

  s1 = r1 * (5 << 2);
  s2 = r2 * (5 << 2);

  while (bytes >= poly1305_block_size) {
    uint64_t t0, t1;

    /* h += m[i] */
    t0 = U8TO64(&m[0]);
    t1 = U8TO64(&m[8]);

    h0 += ((t0) &0xfffffffffff);
    h1 += (((t0 >> 44) | (t1 << 20)) & 0xfffffffffff);
    h2 += (((t1 >> 24)) & 0x3ffffffffff) | hibit;

    /* h *= r */
    MUL128(d0, h0, r0);
    MUL128(d, h1, s2);
    ADD(d0, d);
    MUL128(d, h2, s1);
    ADD(d0, d);
    MUL128(d1, h0, r1);
    MUL128(d, h1, r0);
    ADD(d1, d);
    MUL128(d, h2, s2);
    ADD(d1, d);
    MUL128(d2, h0, r2);
    MUL128(d, h1, r1);
    ADD(d2, d);
    MUL128(d, h2, r0);
    ADD(d2, d);

    /* (partial) h %= p */
    c = SHR(d0, 44);
    h0 = LO(d0) & 0xfffffffffff;
    ADDLO(d1, c);
    c = SHR(d1, 44);
    h1 = LO(d1) & 0xfffffffffff;
    ADDLO(d2, c);
    c = SHR(d2, 42);
    h2 = LO(d2) & 0x3ffffffffff;
    h0 += c * 5;
    c = (h0 >> 44);
    h0 = h0 & 0xfffffffffff;
    h1 += c;

    m += poly1305_block_size;
    bytes -= poly1305_block_size;
  }

  st->h[0] = h0;
  st->h[1] = h1;
  st->h[2] = h2;
}

static POLY1305_NOINLINE void poly1305_finish(poly1305_context *ctx,
                                              unsigned char mac[16]) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;
  uint64_t h0, h1, h2, c;
  uint64_t g0, g1, g2;
  uint64_t t0, t1;

  /* process the remaining block */
  if (st->leftover) {
    size_t i = st->leftover;
    st->buffer[i] = 1;
    for (i = i + 1; i < poly1305_block_size; i++) st->buffer[i] = 0;
    st->final = 1;
    poly1305_blocks(st, st->buffer, poly1305_block_size);
  }

  /* fully carry h */
  h0 = st->h[0];
  h1 = st->h[1];
  h2 = st->h[2];

  c = (h1 >> 44);
  h1 &= 0xfffffffffff;
  h2 += c;
  c = (h2 >> 42);
  h2 &= 0x3ffffffffff;
  h0 += c * 5;
  c = (h0 >> 44);
  h0 &= 0xfffffffffff;
  h1 += c;
  c = (h1 >> 44);
  h1 &= 0xfffffffffff;
  h2 += c;
  c = (h2 >> 42);
  h2 &= 0x3ffffffffff;
  h0 += c * 5;
  c = (h0 >> 44);
  h0 &= 0xfffffffffff;
  h1 += c;

  /* compute h + -p */
  g0 = h0 + 5;
  c = (g0 >> 44);
  g0 &= 0xfffffffffff;
  g1 = h1 + c;
  c = (g1 >> 44);
  g1 &= 0xfffffffffff;
  g2 = h2 + c - ((uint64_t) 1 << 42);

  /* select h if h < p, or h + -p if h >= p */
  c = (g2 >> ((sizeof(uint64_t) * 8) - 1)) - 1;
  g0 &= c;
  g1 &= c;
  g2 &= c;
  c = ~c;
  h0 = (h0 & c) | g0;
  h1 = (h1 & c) | g1;
  h2 = (h2 & c) | g2;

  /* h = (h + pad) */
  t0 = st->pad[0];
  t1 = st->pad[1];

  h0 += ((t0) &0xfffffffffff);
  c = (h0 >> 44);
  h0 &= 0xfffffffffff;
  h1 += (((t0 >> 44) | (t1 << 20)) & 0xfffffffffff) + c;
  c = (h1 >> 44);
  h1 &= 0xfffffffffff;
  h2 += (((t1 >> 24)) & 0x3ffffffffff) + c;
  h2 &= 0x3ffffffffff;

  /* mac = h % (2^128) */
  h0 = ((h0) | (h1 << 44));
  h1 = ((h1 >> 20) | (h2 << 24));

  U64TO8(&mac[0], h0);
  U64TO8(&mac[8], h1);

  /* zero out the state */
  st->h[0] = 0;
  st->h[1] = 0;
  st->h[2] = 0;
  st->r[0] = 0;
  st->r[1] = 0;
  st->r[2] = 0;
  st->pad[0] = 0;
  st->pad[1] = 0;
}

#endif

static void poly1305_update(poly1305_context *ctx, const unsigned char *m,
                            size_t bytes) {
  poly1305_state_internal_t *st = (poly1305_state_internal_t *) ctx;
  size_t i;

  /* handle leftover */
  if (st->leftover) {
    size_t want = (poly1305_block_size - st->leftover);
    if (want > bytes) want = bytes;
    for (i = 0; i < want; i++) st->buffer[st->leftover + i] = m[i];
    bytes -= want;
    m += want;
    st->leftover += want;
    if (st->leftover < poly1305_block_size) return;
    poly1305_blocks(st, st->buffer, poly1305_block_size);
    st->leftover = 0;
  }

  /* process full blocks */
  if (bytes >= poly1305_block_size) {
    size_t want = (bytes & (size_t) ~(poly1305_block_size - 1));
    poly1305_blocks(st, m, want);
    m += want;
    bytes -= want;
  }

  /* store leftover */
  if (bytes) {
    for (i = 0; i < bytes; i++) st->buffer[st->leftover + i] = m[i];
    st->leftover += bytes;
  }
}

// ******* END: poly1305-donna.c ********
// ******* BEGIN: portable8439.c ********

#define __CHACHA20_BLOCK_SIZE (64)
#define __POLY1305_KEY_SIZE (32)

static PORTABLE_8439_DECL uint8_t __ZEROES[16] = {0};
static PORTABLE_8439_DECL void pad_if_needed(poly1305_context *ctx,
                                             size_t size) {
  size_t padding = size % 16;
  if (padding != 0) {
    poly1305_update(ctx, __ZEROES, 16 - padding);
  }
}

#define __u8(v) ((uint8_t) ((v) &0xFF))

// TODO: make this depending on the unaligned/native read size possible
static PORTABLE_8439_DECL void write_64bit_int(poly1305_context *ctx,
                                               uint64_t value) {
  uint8_t result[8];
  result[0] = __u8(value);
  result[1] = __u8(value >> 8);
  result[2] = __u8(value >> 16);
  result[3] = __u8(value >> 24);
  result[4] = __u8(value >> 32);
  result[5] = __u8(value >> 40);
  result[6] = __u8(value >> 48);
  result[7] = __u8(value >> 56);
  poly1305_update(ctx, result, 8);
}

static PORTABLE_8439_DECL void poly1305_calculate_mac(
    uint8_t *mac, const uint8_t *cipher_text, size_t cipher_text_size,
    const uint8_t key[RFC_8439_KEY_SIZE],
    const uint8_t nonce[RFC_8439_NONCE_SIZE], const uint8_t *ad,
    size_t ad_size) {
  // init poly key (section 2.6)
  uint8_t poly_key[__POLY1305_KEY_SIZE] = {0};
  poly1305_context poly_ctx;
  rfc8439_keygen(poly_key, key, nonce);
  // start poly1305 mac
  poly1305_init(&poly_ctx, poly_key);

  if (ad != NULL && ad_size > 0) {
    // write AD if present
    poly1305_update(&poly_ctx, ad, ad_size);
    pad_if_needed(&poly_ctx, ad_size);
  }

  // now write the cipher text
  poly1305_update(&poly_ctx, cipher_text, cipher_text_size);
  pad_if_needed(&poly_ctx, cipher_text_size);

  // write sizes
  write_64bit_int(&poly_ctx, ad_size);
  write_64bit_int(&poly_ctx, cipher_text_size);

  // calculate MAC
  poly1305_finish(&poly_ctx, mac);
}

#define MG_PM(p) ((size_t) (p))

// pointers overlap if the smaller either ahead of the end,
// or its end is before the start of the other
//
// s_size should be smaller or equal to b_size
#define MG_OVERLAPPING(s, s_size, b, b_size) \
  (MG_PM(s) < MG_PM((b) + (b_size))) && (MG_PM(b) < MG_PM((s) + (s_size)))

PORTABLE_8439_DECL size_t mg_chacha20_poly1305_encrypt(
    uint8_t *restrict cipher_text, const uint8_t key[RFC_8439_KEY_SIZE],
    const uint8_t nonce[RFC_8439_NONCE_SIZE], const uint8_t *restrict ad,
    size_t ad_size, const uint8_t *restrict plain_text,
    size_t plain_text_size) {
  size_t new_size = plain_text_size + RFC_8439_TAG_SIZE;
  if (MG_OVERLAPPING(plain_text, plain_text_size, cipher_text, new_size)) {
    return (size_t) -1;
  }
  chacha20_xor_stream(cipher_text, plain_text, plain_text_size, key, nonce, 1);
  poly1305_calculate_mac(cipher_text + plain_text_size, cipher_text,
                         plain_text_size, key, nonce, ad, ad_size);
  return new_size;
}

PORTABLE_8439_DECL size_t mg_chacha20_poly1305_decrypt(
    uint8_t *restrict plain_text, const uint8_t key[RFC_8439_KEY_SIZE],
    const uint8_t nonce[RFC_8439_NONCE_SIZE],
    const uint8_t *restrict cipher_text, size_t cipher_text_size) {
  // first we calculate the mac and see if it lines up, only then do we decrypt
  size_t actual_size = cipher_text_size - RFC_8439_TAG_SIZE;
  if (MG_OVERLAPPING(plain_text, actual_size, cipher_text, cipher_text_size)) {
    return (size_t) -1;
  }

  chacha20_xor_stream(plain_text, cipher_text, actual_size, key, nonce, 1);
  return actual_size;
}
// ******* END:   portable8439.c ********
#endif  // MG_TLS == MG_TLS_BUILTIN
