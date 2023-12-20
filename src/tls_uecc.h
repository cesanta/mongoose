#pragma once
#include "arch.h"

#define uECC_SUPPORTS_secp256r1 1
/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_H_
#define _UECC_H_

/* Platform selection options.
If uECC_PLATFORM is not defined, the code will try to guess it based on compiler
macros. Possible values for uECC_PLATFORM are defined below: */
#define uECC_arch_other 0
#define uECC_x86 1
#define uECC_x86_64 2
#define uECC_arm 3
#define uECC_arm_thumb 4
#define uECC_arm_thumb2 5
#define uECC_arm64 6
#define uECC_avr 7

/* If desired, you can define uECC_WORD_SIZE as appropriate for your platform
(1, 4, or 8 bytes). If uECC_WORD_SIZE is not explicitly defined then it will be
automatically set based on your platform. */

/* Optimization level; trade speed for code size.
   Larger values produce code that is faster but larger.
   Currently supported values are 0 - 4; 0 is unusably slow for most
   applications. Optimization level 4 currently only has an effect ARM platforms
   where more than one curve is enabled. */
#ifndef uECC_OPTIMIZATION_LEVEL
#define uECC_OPTIMIZATION_LEVEL 2
#endif

/* uECC_SQUARE_FUNC - If enabled (defined as nonzero), this will cause a
specific function to be used for (scalar) squaring instead of the generic
multiplication function. This can make things faster somewhat faster, but
increases the code size. */
#ifndef uECC_SQUARE_FUNC
#define uECC_SQUARE_FUNC 0
#endif

/* uECC_VLI_NATIVE_LITTLE_ENDIAN - If enabled (defined as nonzero), this will
switch to native little-endian format for *all* arrays passed in and out of the
public API. This includes public and private keys, shared secrets, signatures
and message hashes. Using this switch reduces the amount of call stack memory
used by uECC, since less intermediate translations are required. Note that this
will *only* work on native little-endian processors and it will treat the
uint8_t arrays passed into the public API as word arrays, therefore requiring
the provided byte arrays to be word aligned on architectures that do not support
unaligned accesses. IMPORTANT: Keys and signatures generated with
uECC_VLI_NATIVE_LITTLE_ENDIAN=1 are incompatible with keys and signatures
generated with uECC_VLI_NATIVE_LITTLE_ENDIAN=0; all parties must use the same
endianness. */
#ifndef uECC_VLI_NATIVE_LITTLE_ENDIAN
#define uECC_VLI_NATIVE_LITTLE_ENDIAN 0
#endif

/* Curve support selection. Set to 0 to remove that curve. */
#ifndef uECC_SUPPORTS_secp160r1
#define uECC_SUPPORTS_secp160r1 0
#endif
#ifndef uECC_SUPPORTS_secp192r1
#define uECC_SUPPORTS_secp192r1 0
#endif
#ifndef uECC_SUPPORTS_secp224r1
#define uECC_SUPPORTS_secp224r1 0
#endif
#ifndef uECC_SUPPORTS_secp256r1
#define uECC_SUPPORTS_secp256r1 1
#endif
#ifndef uECC_SUPPORTS_secp256k1
#define uECC_SUPPORTS_secp256k1 0
#endif

/* Specifies whether compressed point format is supported.
   Set to 0 to disable point compression/decompression functions. */
#ifndef uECC_SUPPORT_COMPRESSED_POINT
#define uECC_SUPPORT_COMPRESSED_POINT 1
#endif

struct uECC_Curve_t;
typedef const struct uECC_Curve_t *uECC_Curve;

#ifdef __cplusplus
extern "C" {
#endif

#if uECC_SUPPORTS_secp160r1
uECC_Curve uECC_secp160r1(void);
#endif
#if uECC_SUPPORTS_secp192r1
uECC_Curve uECC_secp192r1(void);
#endif
#if uECC_SUPPORTS_secp224r1
uECC_Curve uECC_secp224r1(void);
#endif
#if uECC_SUPPORTS_secp256r1
uECC_Curve uECC_secp256r1(void);
#endif
#if uECC_SUPPORTS_secp256k1
uECC_Curve uECC_secp256k1(void);
#endif

/* uECC_RNG_Function type
The RNG function should fill 'size' random bytes into 'dest'. It should return 1
if 'dest' was filled with random data, or 0 if the random data could not be
generated. The filled-in values should be either truly random, or from a
cryptographically-secure PRNG.

A correctly functioning RNG function must be set (using uECC_set_rng()) before
calling uECC_make_key() or uECC_sign().

Setting a correctly functioning RNG function improves the resistance to
side-channel attacks for uECC_shared_secret() and uECC_sign_deterministic().

A correct RNG function is set by default when building for Windows, Linux, or OS
X. If you are building on another POSIX-compliant system that supports
/dev/random or /dev/urandom, you can define uECC_POSIX to use the predefined
RNG. For embedded platforms there is no predefined RNG function; you must
provide your own.
*/
typedef int (*uECC_RNG_Function)(uint8_t *dest, unsigned size);

/* uECC_set_rng() function.
Set the function that will be used to generate random bytes. The RNG function
should return 1 if the random data was generated, or 0 if the random data could
not be generated.

On platforms where there is no predefined RNG function (eg embedded platforms),
this must be called before uECC_make_key() or uECC_sign() are used.

Inputs:
    rng_function - The function that will be used to generate random bytes.
*/
void uECC_set_rng(uECC_RNG_Function rng_function);

/* uECC_get_rng() function.

Returns the function that will be used to generate random bytes.
*/
uECC_RNG_Function uECC_get_rng(void);

/* uECC_curve_private_key_size() function.

Returns the size of a private key for the curve in bytes.
*/
int uECC_curve_private_key_size(uECC_Curve curve);

/* uECC_curve_public_key_size() function.

Returns the size of a public key for the curve in bytes.
*/
int uECC_curve_public_key_size(uECC_Curve curve);

/* uECC_make_key() function.
Create a public/private key pair.

Outputs:
    public_key  - Will be filled in with the public key. Must be at least 2 *
the curve size (in bytes) long. For example, if the curve is secp256r1,
public_key must be 64 bytes long. private_key - Will be filled in with the
private key. Must be as long as the curve order; this is typically the same as
the curve size, except for secp160r1. For example, if the curve is secp256r1,
private_key must be 32 bytes long.

                  For secp160r1, private_key must be 21 bytes long! Note that
the first byte will almost always be 0 (there is about a 1 in 2^80 chance of it
being non-zero).

Returns 1 if the key pair was generated successfully, 0 if an error occurred.
*/
int uECC_make_key(uint8_t *public_key, uint8_t *private_key, uECC_Curve curve);

/* uECC_shared_secret() function.
Compute a shared secret given your secret key and someone else's public key. If
the public key is not from a trusted source and has not been previously
verified, you should verify it first using uECC_valid_public_key(). Note: It is
recommended that you hash the result of uECC_shared_secret() before using it for
symmetric encryption or HMAC.

Inputs:
    public_key  - The public key of the remote party.
    private_key - Your private key.

Outputs:
    secret - Will be filled in with the shared secret value. Must be the same
size as the curve size; for example, if the curve is secp256r1, secret must be
32 bytes long.

Returns 1 if the shared secret was generated successfully, 0 if an error
occurred.
*/
int uECC_shared_secret(const uint8_t *public_key, const uint8_t *private_key,
                       uint8_t *secret, uECC_Curve curve);

#if uECC_SUPPORT_COMPRESSED_POINT
/* uECC_compress() function.
Compress a public key.

Inputs:
    public_key - The public key to compress.

Outputs:
    compressed - Will be filled in with the compressed public key. Must be at
least (curve size + 1) bytes long; for example, if the curve is secp256r1,
                 compressed must be 33 bytes long.
*/
void uECC_compress(const uint8_t *public_key, uint8_t *compressed,
                   uECC_Curve curve);

/* uECC_decompress() function.
Decompress a compressed public key.

Inputs:
    compressed - The compressed public key.

Outputs:
    public_key - Will be filled in with the decompressed public key.
*/
void uECC_decompress(const uint8_t *compressed, uint8_t *public_key,
                     uECC_Curve curve);
#endif /* uECC_SUPPORT_COMPRESSED_POINT */

/* uECC_valid_public_key() function.
Check to see if a public key is valid.

Note that you are not required to check for a valid public key before using any
other uECC functions. However, you may wish to avoid spending CPU time computing
a shared secret or verifying a signature using an invalid public key.

Inputs:
    public_key - The public key to check.

Returns 1 if the public key is valid, 0 if it is invalid.
*/
int uECC_valid_public_key(const uint8_t *public_key, uECC_Curve curve);

/* uECC_compute_public_key() function.
Compute the corresponding public key for a private key.

Inputs:
    private_key - The private key to compute the public key for

Outputs:
    public_key - Will be filled in with the corresponding public key

Returns 1 if the key was computed successfully, 0 if an error occurred.
*/
int uECC_compute_public_key(const uint8_t *private_key, uint8_t *public_key,
                            uECC_Curve curve);

/* uECC_sign() function.
Generate an ECDSA signature for a given hash value.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and
pass it in to this function along with your private key.

Inputs:
    private_key  - Your private key.
    message_hash - The hash of the message to sign.
    hash_size    - The size of message_hash in bytes.

Outputs:
    signature - Will be filled in with the signature value. Must be at least 2 *
curve size long. For example, if the curve is secp256r1, signature must be 64
bytes long.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int uECC_sign(const uint8_t *private_key, const uint8_t *message_hash,
              unsigned hash_size, uint8_t *signature, uECC_Curve curve);

/* uECC_HashContext structure.
This is used to pass in an arbitrary hash function to uECC_sign_deterministic().
The structure will be used for multiple hash computations; each time a new hash
is computed, init_hash() will be called, followed by one or more calls to
update_hash(), and finally a call to finish_hash() to produce the resulting
hash.

The intention is that you will create a structure that includes uECC_HashContext
followed by any hash-specific data. For example:

typedef struct SHA256_HashContext {
    uECC_HashContext uECC;
    SHA256_CTX ctx;
} SHA256_HashContext;

void init_SHA256(uECC_HashContext *base) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Init(&context->ctx);
}

void update_SHA256(uECC_HashContext *base,
                   const uint8_t *message,
                   unsigned message_size) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Update(&context->ctx, message, message_size);
}

void finish_SHA256(uECC_HashContext *base, uint8_t *hash_result) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Final(hash_result, &context->ctx);
}

... when signing ...
{
    uint8_t tmp[32 + 32 + 64];
    SHA256_HashContext ctx = {{&init_SHA256, &update_SHA256, &finish_SHA256, 64,
32, tmp}}; uECC_sign_deterministic(key, message_hash, &ctx.uECC, signature);
}
*/
typedef struct uECC_HashContext {
  void (*init_hash)(const struct uECC_HashContext *context);
  void (*update_hash)(const struct uECC_HashContext *context,
                      const uint8_t *message, unsigned message_size);
  void (*finish_hash)(const struct uECC_HashContext *context,
                      uint8_t *hash_result);
  unsigned
      block_size; /* Hash function block size in bytes, eg 64 for SHA-256. */
  unsigned
      result_size; /* Hash function result size in bytes, eg 32 for SHA-256. */
  uint8_t *tmp;    /* Must point to a buffer of at least (2 * result_size +
                      block_size) bytes. */
} uECC_HashContext;

/* uECC_sign_deterministic() function.
Generate an ECDSA signature for a given hash value, using a deterministic
algorithm (see RFC 6979). You do not need to set the RNG using uECC_set_rng()
before calling this function; however, if the RNG is defined it will improve
resistance to side-channel attacks.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and
pass it to this function along with your private key and a hash context. Note
that the message_hash does not need to be computed with the same hash function
used by hash_context.

Inputs:
    private_key  - Your private key.
    message_hash - The hash of the message to sign.
    hash_size    - The size of message_hash in bytes.
    hash_context - A hash context to use.

Outputs:
    signature - Will be filled in with the signature value.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int uECC_sign_deterministic(const uint8_t *private_key,
                            const uint8_t *message_hash, unsigned hash_size,
                            const uECC_HashContext *hash_context,
                            uint8_t *signature, uECC_Curve curve);

/* uECC_verify() function.
Verify an ECDSA signature.

Usage: Compute the hash of the signed data using the same hash as the signer and
pass it to this function along with the signer's public key and the signature
values (r and s).

Inputs:
    public_key   - The signer's public key.
    message_hash - The hash of the signed data.
    hash_size    - The size of message_hash in bytes.
    signature    - The signature value.

Returns 1 if the signature is valid, 0 if it is invalid.
*/
int uECC_verify(const uint8_t *public_key, const uint8_t *message_hash,
                unsigned hash_size, const uint8_t *signature, uECC_Curve curve);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _UECC_H_ */

/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_VLI_H_
#define _UECC_VLI_H_

//#include "types.h"
//#include "uECC.h"

/* Functions for raw large-integer manipulation. These are only available
   if uECC.c is compiled with uECC_ENABLE_VLI_API defined to 1. */
#ifndef uECC_ENABLE_VLI_API
#define uECC_ENABLE_VLI_API 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if uECC_ENABLE_VLI_API

void uECC_vli_clear(uECC_word_t *vli, wordcount_t num_words);

/* Constant-time comparison to zero - secure way to compare long integers */
/* Returns 1 if vli == 0, 0 otherwise. */
uECC_word_t uECC_vli_isZero(const uECC_word_t *vli, wordcount_t num_words);

/* Returns nonzero if bit 'bit' of vli is set. */
uECC_word_t uECC_vli_testBit(const uECC_word_t *vli, bitcount_t bit);

/* Counts the number of bits required to represent vli. */
bitcount_t uECC_vli_numBits(const uECC_word_t *vli,
                            const wordcount_t max_words);

/* Sets dest = src. */
void uECC_vli_set(uECC_word_t *dest, const uECC_word_t *src,
                  wordcount_t num_words);

/* Constant-time comparison function - secure way to compare long integers */
/* Returns one if left == right, zero otherwise */
uECC_word_t uECC_vli_equal(const uECC_word_t *left, const uECC_word_t *right,
                           wordcount_t num_words);

/* Constant-time comparison function - secure way to compare long integers */
/* Returns sign of left - right, in constant time. */
cmpresult_t uECC_vli_cmp(const uECC_word_t *left, const uECC_word_t *right,
                         wordcount_t num_words);

/* Computes vli = vli >> 1. */
void uECC_vli_rshift1(uECC_word_t *vli, wordcount_t num_words);

/* Computes result = left + right, returning carry. Can modify in place. */
uECC_word_t uECC_vli_add(uECC_word_t *result, const uECC_word_t *left,
                         const uECC_word_t *right, wordcount_t num_words);

/* Computes result = left - right, returning borrow. Can modify in place. */
uECC_word_t uECC_vli_sub(uECC_word_t *result, const uECC_word_t *left,
                         const uECC_word_t *right, wordcount_t num_words);

/* Computes result = left * right. Result must be 2 * num_words long. */
void uECC_vli_mult(uECC_word_t *result, const uECC_word_t *left,
                   const uECC_word_t *right, wordcount_t num_words);

/* Computes result = left^2. Result must be 2 * num_words long. */
void uECC_vli_square(uECC_word_t *result, const uECC_word_t *left,
                     wordcount_t num_words);

/* Computes result = (left + right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap
   mod. */
void uECC_vli_modAdd(uECC_word_t *result, const uECC_word_t *left,
                     const uECC_word_t *right, const uECC_word_t *mod,
                     wordcount_t num_words);

/* Computes result = (left - right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap
   mod. */
void uECC_vli_modSub(uECC_word_t *result, const uECC_word_t *left,
                     const uECC_word_t *right, const uECC_word_t *mod,
                     wordcount_t num_words);

/* Computes result = product % mod, where product is 2N words long.
   Currently only designed to work for mod == curve->p or curve_n. */
void uECC_vli_mmod(uECC_word_t *result, uECC_word_t *product,
                   const uECC_word_t *mod, wordcount_t num_words);

/* Calculates result = product (mod curve->p), where product is up to
   2 * curve->num_words long. */
void uECC_vli_mmod_fast(uECC_word_t *result, uECC_word_t *product,
                        uECC_Curve curve);

/* Computes result = (left * right) % mod.
   Currently only designed to work for mod == curve->p or curve_n. */
void uECC_vli_modMult(uECC_word_t *result, const uECC_word_t *left,
                      const uECC_word_t *right, const uECC_word_t *mod,
                      wordcount_t num_words);

/* Computes result = (left * right) % curve->p. */
void uECC_vli_modMult_fast(uECC_word_t *result, const uECC_word_t *left,
                           const uECC_word_t *right, uECC_Curve curve);

/* Computes result = left^2 % mod.
   Currently only designed to work for mod == curve->p or curve_n. */
void uECC_vli_modSquare(uECC_word_t *result, const uECC_word_t *left,
                        const uECC_word_t *mod, wordcount_t num_words);

/* Computes result = left^2 % curve->p. */
void uECC_vli_modSquare_fast(uECC_word_t *result, const uECC_word_t *left,
                             uECC_Curve curve);

/* Computes result = (1 / input) % mod.*/
void uECC_vli_modInv(uECC_word_t *result, const uECC_word_t *input,
                     const uECC_word_t *mod, wordcount_t num_words);

#if uECC_SUPPORT_COMPRESSED_POINT
/* Calculates a = sqrt(a) (mod curve->p) */
void uECC_vli_mod_sqrt(uECC_word_t *a, uECC_Curve curve);
#endif

/* Converts an integer in uECC native format to big-endian bytes. */
void uECC_vli_nativeToBytes(uint8_t *bytes, int num_bytes,
                            const uECC_word_t *native);
/* Converts big-endian bytes to an integer in uECC native format. */
void uECC_vli_bytesToNative(uECC_word_t *native, const uint8_t *bytes,
                            int num_bytes);

unsigned uECC_curve_num_words(uECC_Curve curve);
unsigned uECC_curve_num_bytes(uECC_Curve curve);
unsigned uECC_curve_num_bits(uECC_Curve curve);
unsigned uECC_curve_num_n_words(uECC_Curve curve);
unsigned uECC_curve_num_n_bytes(uECC_Curve curve);
unsigned uECC_curve_num_n_bits(uECC_Curve curve);

const uECC_word_t *uECC_curve_p(uECC_Curve curve);
const uECC_word_t *uECC_curve_n(uECC_Curve curve);
const uECC_word_t *uECC_curve_G(uECC_Curve curve);
const uECC_word_t *uECC_curve_b(uECC_Curve curve);

int uECC_valid_point(const uECC_word_t *point, uECC_Curve curve);

/* Multiplies a point by a scalar. Points are represented by the X coordinate
   followed by the Y coordinate in the same array, both coordinates are
   curve->num_words long. Note that scalar must be curve->num_n_words long (NOT
   curve->num_words). */
void uECC_point_mult(uECC_word_t *result, const uECC_word_t *point,
                     const uECC_word_t *scalar, uECC_Curve curve);

/* Generates a random integer in the range 0 < random < top.
   Both random and top have num_words words. */
int uECC_generate_random_int(uECC_word_t *random, const uECC_word_t *top,
                             wordcount_t num_words);

#endif /* uECC_ENABLE_VLI_API */

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _UECC_VLI_H_ */

/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_TYPES_H_
#define _UECC_TYPES_H_

#ifndef uECC_PLATFORM
#if defined(__AVR__) && __AVR__
#define uECC_PLATFORM uECC_avr
#elif defined(__thumb2__) || \
    defined(_M_ARMT) /* I think MSVC only supports Thumb-2 targets */
#define uECC_PLATFORM uECC_arm_thumb2
#elif defined(__thumb__)
#define uECC_PLATFORM uECC_arm_thumb
#elif defined(__arm__) || defined(_M_ARM)
#define uECC_PLATFORM uECC_arm
#elif defined(__aarch64__)
#define uECC_PLATFORM uECC_arm64
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_) || \
    defined(__I86__)
#define uECC_PLATFORM uECC_x86
#elif defined(__amd64__) || defined(_M_X64)
#define uECC_PLATFORM uECC_x86_64
#else
#define uECC_PLATFORM uECC_arch_other
#endif
#endif

#ifndef uECC_ARM_USE_UMAAL
#if (uECC_PLATFORM == uECC_arm) && (__ARM_ARCH >= 6)
#define uECC_ARM_USE_UMAAL 1
#elif (uECC_PLATFORM == uECC_arm_thumb2) && (__ARM_ARCH >= 6) && \
    (!defined(__ARM_ARCH_7M__) || !__ARM_ARCH_7M__)
#define uECC_ARM_USE_UMAAL 1
#else
#define uECC_ARM_USE_UMAAL 0
#endif
#endif

#ifndef uECC_WORD_SIZE
#if uECC_PLATFORM == uECC_avr
#define uECC_WORD_SIZE 1
#elif (uECC_PLATFORM == uECC_x86_64 || uECC_PLATFORM == uECC_arm64)
#define uECC_WORD_SIZE 8
#else
#define uECC_WORD_SIZE 4
#endif
#endif

#if (uECC_WORD_SIZE != 1) && (uECC_WORD_SIZE != 4) && (uECC_WORD_SIZE != 8)
#error "Unsupported value for uECC_WORD_SIZE"
#endif

#if ((uECC_PLATFORM == uECC_avr) && (uECC_WORD_SIZE != 1))
#pragma message("uECC_WORD_SIZE must be 1 for AVR")
#undef uECC_WORD_SIZE
#define uECC_WORD_SIZE 1
#endif

#if ((uECC_PLATFORM == uECC_arm || uECC_PLATFORM == uECC_arm_thumb || \
      uECC_PLATFORM == uECC_arm_thumb2) &&                            \
     (uECC_WORD_SIZE != 4))
#pragma message("uECC_WORD_SIZE must be 4 for ARM")
#undef uECC_WORD_SIZE
#define uECC_WORD_SIZE 4
#endif

typedef int8_t wordcount_t;
typedef int16_t bitcount_t;
typedef int8_t cmpresult_t;

#if (uECC_WORD_SIZE == 1)

typedef uint8_t uECC_word_t;
typedef uint16_t uECC_dword_t;

#define HIGH_BIT_SET 0x80
#define uECC_WORD_BITS 8
#define uECC_WORD_BITS_SHIFT 3
#define uECC_WORD_BITS_MASK 0x07

#elif (uECC_WORD_SIZE == 4)

typedef uint32_t uECC_word_t;
typedef uint64_t uECC_dword_t;

#define HIGH_BIT_SET 0x80000000
#define uECC_WORD_BITS 32
#define uECC_WORD_BITS_SHIFT 5
#define uECC_WORD_BITS_MASK 0x01F

#elif (uECC_WORD_SIZE == 8)

typedef uint64_t uECC_word_t;

#define HIGH_BIT_SET 0x8000000000000000U
#define uECC_WORD_BITS 64
#define uECC_WORD_BITS_SHIFT 6
#define uECC_WORD_BITS_MASK 0x03F

#endif /* uECC_WORD_SIZE */

#endif /* _UECC_TYPES_H_ */
// End of uecc BSD-2
