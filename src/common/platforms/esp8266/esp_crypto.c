/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "mongoose/mongoose.h"

#ifdef RTOS_SDK
#include "esp_libc.h"
#else
#include "osapi.h"
#endif

extern int sha1_vector(size_t num_msgs, const uint8_t *msgs[],
                       const size_t *msg_lens, uint8_t *digest);

extern int md5_vector(size_t num_msgs, const uint8_t *msgs[],
                      const size_t *msg_lens, uint8_t *digest);

/* For digest auth. */
void mg_hash_md5_v(size_t num_msgs, const uint8_t *msgs[],
                   const size_t *msg_lens, uint8_t *digest) {
  (void) md5_vector(num_msgs, msgs, msg_lens, digest);
}

/* For WebSocket handshake. */
void mg_hash_sha1_v(size_t num_msgs, const uint8_t *msgs[],
                    const size_t *msg_lens, uint8_t *digest) {
  (void) sha1_vector(num_msgs, msgs, msg_lens, digest);
}

#if MG_ENABLE_SSL

#include "mbedtls/aes.h"
#include "mbedtls/sha256.h"

#define AES_PRIV_NR_POS (4 * 15)

/*
 * Crypto functions in ROM/SDK.
 * They come from wpa_supplicant, you can find them here https://w1.fi/cgit/
 *
 * Note that ROM version of the key setup function is older, does not take the
 * number of bits argument and only supports AES-128. This prototype doesn't
 * suit it, but since the difference is in the last aegument, it doesn't matter.
 */

extern void rijndaelKeySetupDec(void *ctx, const uint8_t *key, int bits);
extern int rijndaelKeySetupEnc(void *ctx, const uint8_t *key, int bits);
void aes_encrypt(void *ctx, const uint8_t *plain, uint8_t *crypt);
void aes_decrypt(void *ctx, const uint8_t *crypt, uint8_t *plain);

/*
 * AES that comes with wpa_supplicant allocates its own AES context in
 * aes_{encrypt,decrypt}_init. Ideally, we'd take that pointer and store it in
 * our mbedtls_aes_context, but then a lot of space would be wasted.
 * We do not call _init and go directly to key setup functions and poke number
 * of rounds into the right place too. This is a bit hacky, but works fine.
 * There is also a difference between older function in ROM and the one coming
 * with SDK which is newer: the older one actually takes two arguments, not 3.
 * But it doesn't matter, extra argument doesn't hurt and this works with both.
 */
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits) {
  if (keybits != 128) return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
  ((uint32_t *) ctx)[AES_PRIV_NR_POS] = 10;
  rijndaelKeySetupEnc(ctx, key, 128);
  return 0;
}

int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits) {
  if (keybits != 128) return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
  ((uint32_t *) ctx)[AES_PRIV_NR_POS] = 10;
  rijndaelKeySetupDec(ctx, key, 128);
  return 0;
}

int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16]) {
  aes_encrypt(ctx, input, output);
  return 0;
}

int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16]) {
  aes_decrypt(ctx, input, output);
  return 0;
}

/* os_get_random uses hardware RNG, so it's cool. */
int mg_ssl_if_mbed_random(void *ctx, unsigned char *buf, size_t len) {
  os_get_random(buf, len);
  (void) ctx;
  return 0;
}

/* For CryptoAuthLib host crypto. */
int atcac_sw_sha2_256(const uint8_t *data, size_t data_size,
                      uint8_t digest[32]) {
  mbedtls_sha256(data, data_size, digest, false /* is_224 */);
  return 0;
}

#endif /* MG_ENABLE_SSL */
