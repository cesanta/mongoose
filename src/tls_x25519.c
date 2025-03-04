/**
 * Adapted from STROBE: https://strobe.sourceforge.io/
 * Copyright (c) 2015-2016 Cryptography Research, Inc.
 * Author: Mike Hamburg
 * License: MIT License
 */
#include "tls.h"
#include "tls_x25519.h"
#include "util.h"

#if MG_TLS == MG_TLS_BUILTIN

const uint8_t X25519_BASE_POINT[X25519_BYTES] = {9};

#define X25519_WBITS 32

typedef uint32_t limb_t;
typedef uint64_t dlimb_t;
typedef int64_t sdlimb_t;

#define NLIMBS (256 / X25519_WBITS)
typedef limb_t mg_fe[NLIMBS];

static limb_t umaal(limb_t *carry, limb_t acc, limb_t mand, limb_t mier) {
  dlimb_t tmp = (dlimb_t) mand * mier + acc + *carry;
  *carry = (limb_t) (tmp >> X25519_WBITS);
  return (limb_t) tmp;
}

// These functions are implemented in terms of umaal on ARM
static limb_t adc(limb_t *carry, limb_t acc, limb_t mand) {
  dlimb_t total = (dlimb_t) *carry + acc + mand;
  *carry = (limb_t) (total >> X25519_WBITS);
  return (limb_t) total;
}

static limb_t adc0(limb_t *carry, limb_t acc) {
  dlimb_t total = (dlimb_t) *carry + acc;
  *carry = (limb_t) (total >> X25519_WBITS);
  return (limb_t) total;
}

// - Precondition: carry is small.
// - Invariant: result of propagate is < 2^255 + 1 word
// - In particular, always less than 2p.
// - Also, output x >= min(x,19)
static void propagate(mg_fe x, limb_t over) {
  unsigned i;
  limb_t carry;
  over = x[NLIMBS - 1] >> (X25519_WBITS - 1) | over << 1;
  x[NLIMBS - 1] &= ~((limb_t) 1 << (X25519_WBITS - 1));

  carry = over * 19;
  for (i = 0; i < NLIMBS; i++) {
    x[i] = adc0(&carry, x[i]);
  }
}

static void add(mg_fe out, const mg_fe a, const mg_fe b) {
  unsigned i;
  limb_t carry = 0;
  for (i = 0; i < NLIMBS; i++) {
    out[i] = adc(&carry, a[i], b[i]);
  }
  propagate(out, carry);
}

static void sub(mg_fe out, const mg_fe a, const mg_fe b) {
  unsigned i;
  sdlimb_t carry = -38;
  for (i = 0; i < NLIMBS; i++) {
    carry = carry + a[i] - b[i];
    out[i] = (limb_t) carry;
    carry >>= X25519_WBITS;
  }
  propagate(out, (limb_t) (1 + carry));
}

// `b` can contain less than 8 limbs, thus we use `limb_t *` instead of `mg_fe`
// to avoid build warnings
static void mul(mg_fe out, const mg_fe a, const limb_t *b, unsigned nb) {
  limb_t accum[2 * NLIMBS] = {0};
  unsigned i, j;

  limb_t carry2;
  for (i = 0; i < nb; i++) {
    limb_t mand = b[i];
    carry2 = 0;
    for (j = 0; j < NLIMBS; j++) {
      limb_t tmp;                        // "a" may be misaligned
      memcpy(&tmp, &a[j], sizeof(tmp));  // So make an aligned copy
      accum[i + j] = umaal(&carry2, accum[i + j], mand, tmp);
    }
    accum[i + j] = carry2;
  }

  carry2 = 0;
  for (j = 0; j < NLIMBS; j++) {
    out[j] = umaal(&carry2, accum[j], 38, accum[j + NLIMBS]);
  }
  propagate(out, carry2);
}

static void sqr(mg_fe out, const mg_fe a) {
  mul(out, a, a, NLIMBS);
}
static void mul1(mg_fe out, const mg_fe a) {
  mul(out, a, out, NLIMBS);
}
static void sqr1(mg_fe a) {
  mul1(a, a);
}

static void condswap(limb_t a[2 * NLIMBS], limb_t b[2 * NLIMBS],
                     limb_t doswap) {
  unsigned i;
  for (i = 0; i < 2 * NLIMBS; i++) {
    limb_t xor_ab = (a[i] ^ b[i]) & doswap;
    a[i] ^= xor_ab;
    b[i] ^= xor_ab;
  }
}

// Canonicalize a field element x, reducing it to the least residue which is
// congruent to it mod 2^255-19
// - Precondition: x < 2^255 + 1 word
static limb_t canon(mg_fe x) {
  // First, add 19.
  unsigned i;
  limb_t carry0 = 19;
  limb_t res;
  sdlimb_t carry;
  for (i = 0; i < NLIMBS; i++) {
    x[i] = adc0(&carry0, x[i]);
  }
  propagate(x, carry0);

  // Here, 19 <= x2 < 2^255
  // - This is because we added 19, so before propagate it can't be less
  // than 19. After propagate, it still can't be less than 19, because if
  // propagate does anything it adds 19.
  // - We know that the high bit must be clear, because either the input was ~
  // 2^255 + one word + 19 (in which case it propagates to at most 2 words) or
  // it was < 2^255. So now, if we subtract 19, we will get back to something in
  // [0,2^255-19).
  carry = -19;
  res = 0;
  for (i = 0; i < NLIMBS; i++) {
    carry += x[i];
    res |= x[i] = (limb_t) carry;
    carry >>= X25519_WBITS;
  }
  return (limb_t) (((dlimb_t) res - 1) >> X25519_WBITS);
}

static const limb_t a24[1] = {121665};

static void ladder_part1(mg_fe xs[5]) {
  limb_t *x2 = xs[0], *z2 = xs[1], *x3 = xs[2], *z3 = xs[3], *t1 = xs[4];
  add(t1, x2, z2);                                 // t1 = A
  sub(z2, x2, z2);                                 // z2 = B
  add(x2, x3, z3);                                 // x2 = C
  sub(z3, x3, z3);                                 // z3 = D
  mul1(z3, t1);                                    // z3 = DA
  mul1(x2, z2);                                    // x3 = BC
  add(x3, z3, x2);                                 // x3 = DA+CB
  sub(z3, z3, x2);                                 // z3 = DA-CB
  sqr1(t1);                                        // t1 = AA
  sqr1(z2);                                        // z2 = BB
  sub(x2, t1, z2);                                 // x2 = E = AA-BB
  mul(z2, x2, a24, sizeof(a24) / sizeof(a24[0]));  // z2 = E*a24
  add(z2, z2, t1);                                 // z2 = E*a24 + AA
}

static void ladder_part2(mg_fe xs[5], const mg_fe x1) {
  limb_t *x2 = xs[0], *z2 = xs[1], *x3 = xs[2], *z3 = xs[3], *t1 = xs[4];
  sqr1(z3);         // z3 = (DA-CB)^2
  mul1(z3, x1);     // z3 = x1 * (DA-CB)^2
  sqr1(x3);         // x3 = (DA+CB)^2
  mul1(z2, x2);     // z2 = AA*(E*a24+AA)
  sub(x2, t1, x2);  // x2 = BB again
  mul1(x2, t1);     // x2 = AA*BB
}

static void x25519_core(mg_fe xs[5], const uint8_t scalar[X25519_BYTES],
                        const uint8_t *x1, int clamp) {
  int i;
  mg_fe x1_limbs;
  limb_t swap = 0;
  limb_t *x2 = xs[0], *x3 = xs[2], *z3 = xs[3];
  memset(xs, 0, 4 * sizeof(mg_fe));
  x2[0] = z3[0] = 1;
  for (i = 0; i < NLIMBS; i++) {
    x3[i] = x1_limbs[i] =
        MG_U32(x1[i * 4 + 3], x1[i * 4 + 2], x1[i * 4 + 1], x1[i * 4]);
  }

  for (i = 255; i >= 0; i--) {
    uint8_t bytei = scalar[i / 8];
    limb_t doswap;
    if (clamp) {
      if (i / 8 == 0) {
        bytei &= (uint8_t) ~7U;
      } else if (i / 8 == X25519_BYTES - 1) {
        bytei &= 0x7F;
        bytei |= 0x40;
      }
    }
    doswap = 0 - (limb_t) ((bytei >> (i % 8)) & 1);
    condswap(x2, x3, swap ^ doswap);
    swap = doswap;

    ladder_part1(xs);
    ladder_part2(xs, (const limb_t *) x1_limbs);
  }
  condswap(x2, x3, swap);
}

int mg_tls_x25519(uint8_t out[X25519_BYTES], const uint8_t scalar[X25519_BYTES],
                  const uint8_t x1[X25519_BYTES], int clamp) {
  int i, ret;
  mg_fe xs[5], out_limbs;
  limb_t *x2, *z2, *z3, *prev;
  static const struct {
    uint8_t a, c, n;
  } steps[13] = {{2, 1, 1},  {2, 1, 1},  {4, 2, 3},  {2, 4, 6},  {3, 1, 1},
                 {3, 2, 12}, {4, 3, 25}, {2, 3, 25}, {2, 4, 50}, {3, 2, 125},
                 {3, 1, 2},  {3, 1, 2},  {3, 1, 1}};
  x25519_core(xs, scalar, x1, clamp);

  // Precomputed inversion chain
  x2 = xs[0];
  z2 = xs[1];
  z3 = xs[3];

  prev = z2;
  for (i = 0; i < 13; i++) {
    int j;
    limb_t *a = xs[steps[i].a];
    for (j = steps[i].n; j > 0; j--) {
      sqr(a, prev);
      prev = a;
    }
    mul1(a, xs[steps[i].c]);
  }

  // Here prev = z3
  // x2 /= z2
  mul(out_limbs, x2, z3, NLIMBS);
  ret = (int) canon(out_limbs);
  if (!clamp) ret = 0;
  for (i = 0; i < NLIMBS; i++) {
    uint32_t n = out_limbs[i];
    out[i * 4] = (uint8_t) (n & 0xff);
    out[i * 4 + 1] = (uint8_t) ((n >> 8) & 0xff);
    out[i * 4 + 2] = (uint8_t) ((n >> 16) & 0xff);
    out[i * 4 + 3] = (uint8_t) ((n >> 24) & 0xff);
  }
  return ret;
}

#endif
