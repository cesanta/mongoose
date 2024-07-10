#pragma once
#include "arch.h"

#define MG_UECC_SUPPORTS_secp256r1 1
/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_H_
#define _UECC_H_

/* Platform selection options.
If MG_UECC_PLATFORM is not defined, the code will try to guess it based on
compiler macros. Possible values for MG_UECC_PLATFORM are defined below: */
#define mg_uecc_arch_other 0
#define mg_uecc_x86 1
#define mg_uecc_x86_64 2
#define mg_uecc_arm 3
#define mg_uecc_arm_thumb 4
#define mg_uecc_arm_thumb2 5
#define mg_uecc_arm64 6
#define mg_uecc_avr 7

/* If desired, you can define MG_UECC_WORD_SIZE as appropriate for your platform
(1, 4, or 8 bytes). If MG_UECC_WORD_SIZE is not explicitly defined then it will
be automatically set based on your platform. */

/* Optimization level; trade speed for code size.
   Larger values produce code that is faster but larger.
   Currently supported values are 0 - 4; 0 is unusably slow for most
   applications. Optimization level 4 currently only has an effect ARM platforms
   where more than one curve is enabled. */
#ifndef MG_UECC_OPTIMIZATION_LEVEL
#define MG_UECC_OPTIMIZATION_LEVEL 2
#endif

/* MG_UECC_SQUARE_FUNC - If enabled (defined as nonzero), this will cause a
specific function to be used for (scalar) squaring instead of the generic
multiplication function. This can make things faster somewhat faster, but
increases the code size. */
#ifndef MG_UECC_SQUARE_FUNC
#define MG_UECC_SQUARE_FUNC 0
#endif

/* MG_UECC_VLI_NATIVE_LITTLE_ENDIAN - If enabled (defined as nonzero), this will
switch to native little-endian format for *all* arrays passed in and out of the
public API. This includes public and private keys, shared secrets, signatures
and message hashes. Using this switch reduces the amount of call stack memory
used by uECC, since less intermediate translations are required. Note that this
will *only* work on native little-endian processors and it will treat the
uint8_t arrays passed into the public API as word arrays, therefore requiring
the provided byte arrays to be word aligned on architectures that do not support
unaligned accesses. IMPORTANT: Keys and signatures generated with
MG_UECC_VLI_NATIVE_LITTLE_ENDIAN=1 are incompatible with keys and signatures
generated with MG_UECC_VLI_NATIVE_LITTLE_ENDIAN=0; all parties must use the same
endianness. */
#ifndef MG_UECC_VLI_NATIVE_LITTLE_ENDIAN
#define MG_UECC_VLI_NATIVE_LITTLE_ENDIAN 0
#endif

/* Curve support selection. Set to 0 to remove that curve. */
#ifndef MG_UECC_SUPPORTS_secp160r1
#define MG_UECC_SUPPORTS_secp160r1 0
#endif
#ifndef MG_UECC_SUPPORTS_secp192r1
#define MG_UECC_SUPPORTS_secp192r1 0
#endif
#ifndef MG_UECC_SUPPORTS_secp224r1
#define MG_UECC_SUPPORTS_secp224r1 0
#endif
#ifndef MG_UECC_SUPPORTS_secp256r1
#define MG_UECC_SUPPORTS_secp256r1 1
#endif
#ifndef MG_UECC_SUPPORTS_secp256k1
#define MG_UECC_SUPPORTS_secp256k1 0
#endif

/* Specifies whether compressed point format is supported.
   Set to 0 to disable point compression/decompression functions. */
#ifndef MG_UECC_SUPPORT_COMPRESSED_POINT
#define MG_UECC_SUPPORT_COMPRESSED_POINT 1
#endif

struct MG_UECC_Curve_t;
typedef const struct MG_UECC_Curve_t *MG_UECC_Curve;

#ifdef __cplusplus
extern "C" {
#endif

#if MG_UECC_SUPPORTS_secp160r1
MG_UECC_Curve mg_uecc_secp160r1(void);
#endif
#if MG_UECC_SUPPORTS_secp192r1
MG_UECC_Curve mg_uecc_secp192r1(void);
#endif
#if MG_UECC_SUPPORTS_secp224r1
MG_UECC_Curve mg_uecc_secp224r1(void);
#endif
#if MG_UECC_SUPPORTS_secp256r1
MG_UECC_Curve mg_uecc_secp256r1(void);
#endif
#if MG_UECC_SUPPORTS_secp256k1
MG_UECC_Curve mg_uecc_secp256k1(void);
#endif

/* MG_UECC_RNG_Function type
The RNG function should fill 'size' random bytes into 'dest'. It should return 1
if 'dest' was filled with random data, or 0 if the random data could not be
generated. The filled-in values should be either truly random, or from a
cryptographically-secure PRNG.

A correctly functioning RNG function must be set (using mg_uecc_set_rng())
before calling mg_uecc_make_key() or mg_uecc_sign().

Setting a correctly functioning RNG function improves the resistance to
side-channel attacks for mg_uecc_shared_secret() and
mg_uecc_sign_deterministic().

A correct RNG function is set by default when building for Windows, Linux, or OS
X. If you are building on another POSIX-compliant system that supports
/dev/random or /dev/urandom, you can define MG_UECC_POSIX to use the predefined
RNG. For embedded platforms there is no predefined RNG function; you must
provide your own.
*/
typedef int (*MG_UECC_RNG_Function)(uint8_t *dest, unsigned size);

/* mg_uecc_set_rng() function.
Set the function that will be used to generate random bytes. The RNG function
should return 1 if the random data was generated, or 0 if the random data could
not be generated.

On platforms where there is no predefined RNG function (eg embedded platforms),
this must be called before mg_uecc_make_key() or mg_uecc_sign() are used.

Inputs:
    rng_function - The function that will be used to generate random bytes.
*/
void mg_uecc_set_rng(MG_UECC_RNG_Function rng_function);

/* mg_uecc_get_rng() function.

Returns the function that will be used to generate random bytes.
*/
MG_UECC_RNG_Function mg_uecc_get_rng(void);

/* mg_uecc_curve_private_key_size() function.

Returns the size of a private key for the curve in bytes.
*/
int mg_uecc_curve_private_key_size(MG_UECC_Curve curve);

/* mg_uecc_curve_public_key_size() function.

Returns the size of a public key for the curve in bytes.
*/
int mg_uecc_curve_public_key_size(MG_UECC_Curve curve);

/* mg_uecc_make_key() function.
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
int mg_uecc_make_key(uint8_t *public_key, uint8_t *private_key,
                     MG_UECC_Curve curve);

/* mg_uecc_shared_secret() function.
Compute a shared secret given your secret key and someone else's public key. If
the public key is not from a trusted source and has not been previously
verified, you should verify it first using mg_uecc_valid_public_key(). Note: It
is recommended that you hash the result of mg_uecc_shared_secret() before using
it for symmetric encryption or HMAC.

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
int mg_uecc_shared_secret(const uint8_t *public_key, const uint8_t *private_key,
                          uint8_t *secret, MG_UECC_Curve curve);

#if MG_UECC_SUPPORT_COMPRESSED_POINT
/* mg_uecc_compress() function.
Compress a public key.

Inputs:
    public_key - The public key to compress.

Outputs:
    compressed - Will be filled in with the compressed public key. Must be at
least (curve size + 1) bytes long; for example, if the curve is secp256r1,
                 compressed must be 33 bytes long.
*/
void mg_uecc_compress(const uint8_t *public_key, uint8_t *compressed,
                      MG_UECC_Curve curve);

/* mg_uecc_decompress() function.
Decompress a compressed public key.

Inputs:
    compressed - The compressed public key.

Outputs:
    public_key - Will be filled in with the decompressed public key.
*/
void mg_uecc_decompress(const uint8_t *compressed, uint8_t *public_key,
                        MG_UECC_Curve curve);
#endif /* MG_UECC_SUPPORT_COMPRESSED_POINT */

/* mg_uecc_valid_public_key() function.
Check to see if a public key is valid.

Note that you are not required to check for a valid public key before using any
other uECC functions. However, you may wish to avoid spending CPU time computing
a shared secret or verifying a signature using an invalid public key.

Inputs:
    public_key - The public key to check.

Returns 1 if the public key is valid, 0 if it is invalid.
*/
int mg_uecc_valid_public_key(const uint8_t *public_key, MG_UECC_Curve curve);

/* mg_uecc_compute_public_key() function.
Compute the corresponding public key for a private key.

Inputs:
    private_key - The private key to compute the public key for

Outputs:
    public_key - Will be filled in with the corresponding public key

Returns 1 if the key was computed successfully, 0 if an error occurred.
*/
int mg_uecc_compute_public_key(const uint8_t *private_key, uint8_t *public_key,
                               MG_UECC_Curve curve);

/* mg_uecc_sign() function.
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
int mg_uecc_sign(const uint8_t *private_key, const uint8_t *message_hash,
                 unsigned hash_size, uint8_t *signature, MG_UECC_Curve curve);

/* MG_UECC_HashContext structure.
This is used to pass in an arbitrary hash function to
mg_uecc_sign_deterministic(). The structure will be used for multiple hash
computations; each time a new hash is computed, init_hash() will be called,
followed by one or more calls to update_hash(), and finally a call to
finish_hash() to produce the resulting hash.

The intention is that you will create a structure that includes
MG_UECC_HashContext followed by any hash-specific data. For example:

typedef struct SHA256_HashContext {
    MG_UECC_HashContext uECC;
    SHA256_CTX ctx;
} SHA256_HashContext;

void init_SHA256(MG_UECC_HashContext *base) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Init(&context->ctx);
}

void update_SHA256(MG_UECC_HashContext *base,
                   const uint8_t *message,
                   unsigned message_size) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Update(&context->ctx, message, message_size);
}

void finish_SHA256(MG_UECC_HashContext *base, uint8_t *hash_result) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Final(hash_result, &context->ctx);
}

... when signing ...
{
    uint8_t tmp[32 + 32 + 64];
    SHA256_HashContext ctx = {{&init_SHA256, &update_SHA256, &finish_SHA256, 64,
32, tmp}}; mg_uecc_sign_deterministic(key, message_hash, &ctx.uECC, signature);
}
*/
typedef struct MG_UECC_HashContext {
  void (*init_hash)(const struct MG_UECC_HashContext *context);
  void (*update_hash)(const struct MG_UECC_HashContext *context,
                      const uint8_t *message, unsigned message_size);
  void (*finish_hash)(const struct MG_UECC_HashContext *context,
                      uint8_t *hash_result);
  unsigned
      block_size; /* Hash function block size in bytes, eg 64 for SHA-256. */
  unsigned
      result_size; /* Hash function result size in bytes, eg 32 for SHA-256. */
  uint8_t *tmp;    /* Must point to a buffer of at least (2 * result_size +
                      block_size) bytes. */
} MG_UECC_HashContext;

/* mg_uecc_sign_deterministic() function.
Generate an ECDSA signature for a given hash value, using a deterministic
algorithm (see RFC 6979). You do not need to set the RNG using mg_uecc_set_rng()
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
int mg_uecc_sign_deterministic(const uint8_t *private_key,
                               const uint8_t *message_hash, unsigned hash_size,
                               const MG_UECC_HashContext *hash_context,
                               uint8_t *signature, MG_UECC_Curve curve);

/* mg_uecc_verify() function.
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
int mg_uecc_verify(const uint8_t *public_key, const uint8_t *message_hash,
                   unsigned hash_size, const uint8_t *signature,
                   MG_UECC_Curve curve);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _UECC_H_ */

/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_TYPES_H_
#define _UECC_TYPES_H_

#ifndef MG_UECC_PLATFORM
#if defined(__AVR__) && __AVR__
#define MG_UECC_PLATFORM mg_uecc_avr
#elif defined(__thumb2__) || \
    defined(_M_ARMT) /* I think MSVC only supports Thumb-2 targets */
#define MG_UECC_PLATFORM mg_uecc_arm_thumb2
#elif defined(__thumb__)
#define MG_UECC_PLATFORM mg_uecc_arm_thumb
#elif defined(__arm__) || defined(_M_ARM)
#define MG_UECC_PLATFORM mg_uecc_arm
#elif defined(__aarch64__)
#define MG_UECC_PLATFORM mg_uecc_arm64
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_) || \
    defined(__I86__)
#define MG_UECC_PLATFORM mg_uecc_x86
#elif defined(__amd64__) || defined(_M_X64)
#define MG_UECC_PLATFORM mg_uecc_x86_64
#else
#define MG_UECC_PLATFORM mg_uecc_arch_other
#endif
#endif

#ifndef MG_UECC_ARM_USE_UMAAL
#if (MG_UECC_PLATFORM == mg_uecc_arm) && (__ARM_ARCH >= 6)
#define MG_UECC_ARM_USE_UMAAL 1
#elif (MG_UECC_PLATFORM == mg_uecc_arm_thumb2) && (__ARM_ARCH >= 6) && \
    (!defined(__ARM_ARCH_7M__) || !__ARM_ARCH_7M__)
#define MG_UECC_ARM_USE_UMAAL 1
#else
#define MG_UECC_ARM_USE_UMAAL 0
#endif
#endif

#ifndef MG_UECC_WORD_SIZE
#if MG_UECC_PLATFORM == mg_uecc_avr
#define MG_UECC_WORD_SIZE 1
#elif (MG_UECC_PLATFORM == mg_uecc_x86_64 || MG_UECC_PLATFORM == mg_uecc_arm64)
#define MG_UECC_WORD_SIZE 8
#else
#define MG_UECC_WORD_SIZE 4
#endif
#endif

#if (MG_UECC_WORD_SIZE != 1) && (MG_UECC_WORD_SIZE != 4) && \
    (MG_UECC_WORD_SIZE != 8)
#error "Unsupported value for MG_UECC_WORD_SIZE"
#endif

#if ((MG_UECC_PLATFORM == mg_uecc_avr) && (MG_UECC_WORD_SIZE != 1))
#pragma message("MG_UECC_WORD_SIZE must be 1 for AVR")
#undef MG_UECC_WORD_SIZE
#define MG_UECC_WORD_SIZE 1
#endif

#if ((MG_UECC_PLATFORM == mg_uecc_arm ||         \
      MG_UECC_PLATFORM == mg_uecc_arm_thumb ||   \
      MG_UECC_PLATFORM == mg_uecc_arm_thumb2) && \
     (MG_UECC_WORD_SIZE != 4))
#pragma message("MG_UECC_WORD_SIZE must be 4 for ARM")
#undef MG_UECC_WORD_SIZE
#define MG_UECC_WORD_SIZE 4
#endif

typedef int8_t wordcount_t;
typedef int16_t bitcount_t;
typedef int8_t cmpresult_t;

#if (MG_UECC_WORD_SIZE == 1)

typedef uint8_t mg_uecc_word_t;
typedef uint16_t mg_uecc_dword_t;

#define HIGH_BIT_SET 0x80
#define MG_UECC_WORD_BITS 8
#define MG_UECC_WORD_BITS_SHIFT 3
#define MG_UECC_WORD_BITS_MASK 0x07

#elif (MG_UECC_WORD_SIZE == 4)

typedef uint32_t mg_uecc_word_t;
typedef uint64_t mg_uecc_dword_t;

#define HIGH_BIT_SET 0x80000000
#define MG_UECC_WORD_BITS 32
#define MG_UECC_WORD_BITS_SHIFT 5
#define MG_UECC_WORD_BITS_MASK 0x01F

#elif (MG_UECC_WORD_SIZE == 8)

typedef uint64_t mg_uecc_word_t;

#define HIGH_BIT_SET 0x8000000000000000U
#define MG_UECC_WORD_BITS 64
#define MG_UECC_WORD_BITS_SHIFT 6
#define MG_UECC_WORD_BITS_MASK 0x03F

#endif /* MG_UECC_WORD_SIZE */

#endif /* _UECC_TYPES_H_ */

/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_VLI_H_
#define _UECC_VLI_H_

// #include "types.h"
// #include "uECC.h"

/* Functions for raw large-integer manipulation. These are only available
   if uECC.c is compiled with MG_UECC_ENABLE_VLI_API defined to 1. */
#ifndef MG_UECC_ENABLE_VLI_API
#define MG_UECC_ENABLE_VLI_API 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if MG_UECC_ENABLE_VLI_API

void mg_uecc_vli_clear(mg_uecc_word_t *vli, wordcount_t num_words);

/* Constant-time comparison to zero - secure way to compare long integers */
/* Returns 1 if vli == 0, 0 otherwise. */
mg_uecc_word_t mg_uecc_vli_isZero(const mg_uecc_word_t *vli,
                                  wordcount_t num_words);

/* Returns nonzero if bit 'bit' of vli is set. */
mg_uecc_word_t mg_uecc_vli_testBit(const mg_uecc_word_t *vli, bitcount_t bit);

/* Counts the number of bits required to represent vli. */
bitcount_t mg_uecc_vli_numBits(const mg_uecc_word_t *vli,
                               const wordcount_t max_words);

/* Sets dest = src. */
void mg_uecc_vli_set(mg_uecc_word_t *dest, const mg_uecc_word_t *src,
                     wordcount_t num_words);

/* Constant-time comparison function - secure way to compare long integers */
/* Returns one if left == right, zero otherwise */
mg_uecc_word_t mg_uecc_vli_equal(const mg_uecc_word_t *left,
                                 const mg_uecc_word_t *right,
                                 wordcount_t num_words);

/* Constant-time comparison function - secure way to compare long integers */
/* Returns sign of left - right, in constant time. */
cmpresult_t mg_uecc_vli_cmp(const mg_uecc_word_t *left,
                            const mg_uecc_word_t *right, wordcount_t num_words);

/* Computes vli = vli >> 1. */
void mg_uecc_vli_rshift1(mg_uecc_word_t *vli, wordcount_t num_words);

/* Computes result = left + right, returning carry. Can modify in place. */
mg_uecc_word_t mg_uecc_vli_add(mg_uecc_word_t *result,
                               const mg_uecc_word_t *left,
                               const mg_uecc_word_t *right,
                               wordcount_t num_words);

/* Computes result = left - right, returning borrow. Can modify in place. */
mg_uecc_word_t mg_uecc_vli_sub(mg_uecc_word_t *result,
                               const mg_uecc_word_t *left,
                               const mg_uecc_word_t *right,
                               wordcount_t num_words);

/* Computes result = left * right. Result must be 2 * num_words long. */
void mg_uecc_vli_mult(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                      const mg_uecc_word_t *right, wordcount_t num_words);

/* Computes result = left^2. Result must be 2 * num_words long. */
void mg_uecc_vli_square(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                        wordcount_t num_words);

/* Computes result = (left + right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap
   mod. */
void mg_uecc_vli_modAdd(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                        const mg_uecc_word_t *right, const mg_uecc_word_t *mod,
                        wordcount_t num_words);

/* Computes result = (left - right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap
   mod. */
void mg_uecc_vli_modSub(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                        const mg_uecc_word_t *right, const mg_uecc_word_t *mod,
                        wordcount_t num_words);

/* Computes result = product % mod, where product is 2N words long.
   Currently only designed to work for mod == curve->p or curve_n. */
void mg_uecc_vli_mmod(mg_uecc_word_t *result, mg_uecc_word_t *product,
                      const mg_uecc_word_t *mod, wordcount_t num_words);

/* Calculates result = product (mod curve->p), where product is up to
   2 * curve->num_words long. */
void mg_uecc_vli_mmod_fast(mg_uecc_word_t *result, mg_uecc_word_t *product,
                           MG_UECC_Curve curve);

/* Computes result = (left * right) % mod.
   Currently only designed to work for mod == curve->p or curve_n. */
void mg_uecc_vli_modMult(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                         const mg_uecc_word_t *right, const mg_uecc_word_t *mod,
                         wordcount_t num_words);

/* Computes result = (left * right) % curve->p. */
void mg_uecc_vli_modMult_fast(mg_uecc_word_t *result,
                              const mg_uecc_word_t *left,
                              const mg_uecc_word_t *right, MG_UECC_Curve curve);

/* Computes result = left^2 % mod.
   Currently only designed to work for mod == curve->p or curve_n. */
void mg_uecc_vli_modSquare(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                           const mg_uecc_word_t *mod, wordcount_t num_words);

/* Computes result = left^2 % curve->p. */
void mg_uecc_vli_modSquare_fast(mg_uecc_word_t *result,
                                const mg_uecc_word_t *left,
                                MG_UECC_Curve curve);

/* Computes result = (1 / input) % mod.*/
void mg_uecc_vli_modInv(mg_uecc_word_t *result, const mg_uecc_word_t *input,
                        const mg_uecc_word_t *mod, wordcount_t num_words);

#if MG_UECC_SUPPORT_COMPRESSED_POINT
/* Calculates a = sqrt(a) (mod curve->p) */
void mg_uecc_vli_mod_sqrt(mg_uecc_word_t *a, MG_UECC_Curve curve);
#endif

/* Converts an integer in uECC native format to big-endian bytes. */
void mg_uecc_vli_nativeToBytes(uint8_t *bytes, int num_bytes,
                               const mg_uecc_word_t *native);
/* Converts big-endian bytes to an integer in uECC native format. */
void mg_uecc_vli_bytesToNative(mg_uecc_word_t *native, const uint8_t *bytes,
                               int num_bytes);

unsigned mg_uecc_curve_num_words(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_bytes(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_bits(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_n_words(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_n_bytes(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_n_bits(MG_UECC_Curve curve);

const mg_uecc_word_t *mg_uecc_curve_p(MG_UECC_Curve curve);
const mg_uecc_word_t *mg_uecc_curve_n(MG_UECC_Curve curve);
const mg_uecc_word_t *mg_uecc_curve_G(MG_UECC_Curve curve);
const mg_uecc_word_t *mg_uecc_curve_b(MG_UECC_Curve curve);

int mg_uecc_valid_point(const mg_uecc_word_t *point, MG_UECC_Curve curve);

/* Multiplies a point by a scalar. Points are represented by the X coordinate
   followed by the Y coordinate in the same array, both coordinates are
   curve->num_words long. Note that scalar must be curve->num_n_words long (NOT
   curve->num_words). */
void mg_uecc_point_mult(mg_uecc_word_t *result, const mg_uecc_word_t *point,
                        const mg_uecc_word_t *scalar, MG_UECC_Curve curve);

/* Generates a random integer in the range 0 < random < top.
   Both random and top have num_words words. */
int mg_uecc_generate_random_int(mg_uecc_word_t *random,
                                const mg_uecc_word_t *top,
                                wordcount_t num_words);

#endif /* MG_UECC_ENABLE_VLI_API */

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _UECC_VLI_H_ */

// End of uecc BSD-2
