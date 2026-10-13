// P-256 API, mg_p256_*(), used by built-in TLS, JWT and OTA. Implemented by
// BearSSL code below (MG_P256=MG_P256_BEAR) or tls_p256_cortex.c (LENNGREN)
// BearSSL code: https://bearssl.org commit 7bea48e. Shared bignum and ECDSA
// code is always compiled, it is also used by P-384, see tls_p384.c
//
// Copyright (c) 2016 Thomas Pornin <pornin@bolet.org>
// Copyright (c) 2017 Thomas Pornin <pornin@bolet.org>
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Mongoose changes: br_ / BR_ names are prefixed with mg_ / MG_, as are the
// constant-time helpers from inner.h (NOT, MUX, EQ, ...). Structs are not
// typedef-ed. ECDSA signing takes the caller's k instead of HMAC-DRBG, k is
// computed by mg_p256_sign() (RFC 6979). ECDSA functions take the curve id and
// public key directly
#pragma once

#include "arch.h"
#include "config.h"
#include "str.h"
#include "tls.h"

#if MG_TLS == MG_TLS_BUILTIN || defined(MG_OTA_PUBLIC_KEY)

#define MG_BR_EC_secp256r1 23  // Curve IDs, same as TLS named groups
#define MG_BR_EC_secp384r1 24
#define MG_BR_MAX_EC_SIZE 384  // Largest supported curve, bits

// From BearSSL inc/bearssl_ec.h: an EC implementation
struct mg_br_ec_impl {
  uint32_t supported_curves;  // Bit mask of curve IDs
  const unsigned char *(*generator)(int curve, size_t *len);
  const unsigned char *(*order)(int curve, size_t *len);
  size_t (*xoff)(int curve, size_t *len);
  uint32_t (*mul)(unsigned char *G, size_t Glen, const unsigned char *x,
                  size_t xlen, int curve);
  size_t (*mulgen)(unsigned char *R, const unsigned char *x, size_t xlen,
                   int curve);
  uint32_t (*muladd)(unsigned char *A, const unsigned char *B, size_t len,
                     const unsigned char *x, size_t xlen,
                     const unsigned char *y, size_t ylen, int curve);
};

// From BearSSL src/inner.h: constant-time helpers
static inline uint32_t MG_BR_NOT(uint32_t ctl) {
  return ctl ^ 1;
}
static inline uint32_t MG_BR_MUX(uint32_t ctl, uint32_t x, uint32_t y) {
  return y ^ (-ctl & (x ^ y));
}
static inline uint32_t MG_BR_EQ(uint32_t x, uint32_t y) {
  uint32_t q = x ^ y;
  return MG_BR_NOT((q | -q) >> 31);
}
static inline uint32_t MG_BR_NEQ(uint32_t x, uint32_t y) {
  uint32_t q = x ^ y;
  return (q | -q) >> 31;
}
static inline uint32_t MG_BR_GT(uint32_t x, uint32_t y) {
  uint32_t z = y - x;
  return (z ^ ((x ^ y) & (x ^ z))) >> 31;
}
#define MG_BR_GE(x, y) MG_BR_NOT(MG_BR_GT(y, x))
#define MG_BR_LT(x, y) MG_BR_GT(y, x)
static inline int32_t MG_BR_CMP(uint32_t x, uint32_t y) {
  return (int32_t) MG_BR_GT(x, y) | -(int32_t) MG_BR_GT(y, x);
}
static inline uint32_t MG_BR_BIT_LENGTH(uint32_t x) {
  uint32_t k, c;
  k = MG_BR_NEQ(x, 0);
  c = MG_BR_GT(x, 0xFFFF), x = MG_BR_MUX(c, x >> 16, x), k += c << 4;
  c = MG_BR_GT(x, 0x00FF), x = MG_BR_MUX(c, x >> 8, x), k += c << 3;
  c = MG_BR_GT(x, 0x000F), x = MG_BR_MUX(c, x >> 4, x), k += c << 2;
  c = MG_BR_GT(x, 0x0003), x = MG_BR_MUX(c, x >> 2, x), k += c << 1;
  k += MG_BR_GT(x, 0x0001);
  return k;
}
#define MG_BR_MUL31(x, y) ((uint64_t) (x) * (uint64_t) (y))
#define MG_BR_MUL31_lo(x, y) \
  (((uint32_t) (x) * (uint32_t) (y)) & (uint32_t) 0x7FFFFFFF)
#define MG_BR_CCOPY mg_br_ccopy
void mg_br_ccopy(uint32_t ctl, void *dst, const void *src, size_t len);
uint32_t mg_br_divrem(uint32_t hi, uint32_t lo, uint32_t d, uint32_t *r);
static inline uint32_t mg_br_rem(uint32_t hi, uint32_t lo, uint32_t d) {
  uint32_t r;
  mg_br_divrem(hi, lo, d, &r);
  return r;
}
static inline uint32_t mg_br_div(uint32_t hi, uint32_t lo, uint32_t d) {
  uint32_t r;
  return mg_br_divrem(hi, lo, d, &r);
}
static inline void mg_br_enc32be(void *dst, uint32_t x) {
  unsigned char *buf = (unsigned char *) dst;
  buf[0] = (unsigned char) (x >> 24), buf[1] = (unsigned char) (x >> 16);
  buf[2] = (unsigned char) (x >> 8), buf[3] = (unsigned char) x;
}
static inline void mg_br_i31_zero(uint32_t *x, uint32_t bit_len) {
  *x++ = bit_len;
  memset(x, 0, ((bit_len + 31) >> 5) * sizeof *x);
}

// Big integers, "i31" format, see BearSSL src/inner.h
uint32_t mg_br_i31_add(uint32_t *a, const uint32_t *b, uint32_t ctl);
uint32_t mg_br_i31_sub(uint32_t *a, const uint32_t *b, uint32_t ctl);
uint32_t mg_br_i31_bit_length(uint32_t *x, size_t xlen);
uint32_t mg_br_i31_decode_mod(uint32_t *x, const void *src, size_t len,
                              const uint32_t *m);
void mg_br_i31_decode(uint32_t *x, const void *src, size_t len);
void mg_br_i31_encode(void *dst, size_t len, const uint32_t *x);
void mg_br_i31_from_monty(uint32_t *x, const uint32_t *m, uint32_t m0i);
void mg_br_i31_to_monty(uint32_t *x, const uint32_t *m);
uint32_t mg_br_i31_iszero(const uint32_t *x);
void mg_br_i31_modpow(uint32_t *x, const unsigned char *e, size_t elen,
                      const uint32_t *m, uint32_t m0i, uint32_t *t1,
                      uint32_t *t2);
void mg_br_i31_montymul(uint32_t *d, const uint32_t *x, const uint32_t *y,
                        const uint32_t *m, uint32_t m0i);
void mg_br_i31_muladd_small(uint32_t *x, uint32_t z, const uint32_t *m);
uint32_t mg_br_i31_ninv31(uint32_t x);
void mg_br_i31_rshift(uint32_t *x, int count);

// ECDSA, see BearSSL src/ec/ecdsa_i31_*.c. q is an uncompressed point
void mg_br_ecdsa_i31_bits2int(uint32_t *x, const void *src, size_t len,
                              uint32_t ebitlen);
uint32_t mg_br_ecdsa_i31_vrfy_raw(const struct mg_br_ec_impl *impl, int curve,
                                  const void *hash, size_t hash_len,
                                  const unsigned char *q, size_t qlen,
                                  const void *sig, size_t sig_len);
size_t mg_br_ecdsa_i31_sign_raw(const struct mg_br_ec_impl *impl, int curve,
                                const void *hash, size_t hash_len,
                                const unsigned char *x, size_t xlen,
                                const unsigned char *k, void *sig);

// P-256 (secp256r1) API. Keys and signatures are big endian byte strings,
// a public key is X || Y, a signature is r || s. Functions return 1 on
// success, 0 on failure

// Generate a random key pair, using mg_random()
int mg_p256_make_key(uint8_t public_key[64], uint8_t private_key[32]);

// Compute public key from a private key. Return 0 if private key is invalid
int mg_p256_compute_public_key(const uint8_t private_key[32],
                               uint8_t public_key[64]);

// ECDH. Validates the peer public key, returns 0 if it is not on the curve
int mg_p256_shared_secret(const uint8_t public_key[64],
                          const uint8_t private_key[32], uint8_t secret[32]);

// ECDSA sign with a deterministic k, RFC 6979 with HMAC-SHA256
int mg_p256_sign(const uint8_t private_key[32], const uint8_t *hash,
                 size_t hash_size, uint8_t signature[64]);

// ECDSA verify. Validates the public key
int mg_p256_verify(const uint8_t public_key[64], const uint8_t *hash,
                   size_t hash_size, const uint8_t signature[64]);

// Internal: ECDSA sign with a given k, 32 bytes big endian, in [1, n-1].
// Provided by the selected implementation, used by mg_p256_sign()
int mg_p256_sign_with_k(const uint8_t private_key[32], const uint8_t *hash,
                        size_t hash_size, const uint8_t k[32],
                        uint8_t signature[64]);
#endif  // MG_TLS == MG_TLS_BUILTIN || defined(MG_OTA_PUBLIC_KEY)
