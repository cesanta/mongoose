/******************************************************************************
 *
 * THIS SOURCE CODE IS HEREBY PLACED INTO THE PUBLIC DOMAIN FOR THE GOOD OF ALL
 *
 * This is a simple and straightforward implementation of the AES Rijndael
 * 128-bit block cipher designed by Vincent Rijmen and Joan Daemen. The focus
 * of this work was correctness & accuracy.  It is written in 'C' without any
 * particular focus upon optimization or speed. It should be endian (memory
 * byte order) neutral since the few places that care are handled explicitly.
 *
 * This implementation of Rijndael was created by Steven M. Gibson of GRC.com.
 *
 * It is intended for general purpose use, but was written in support of GRC's
 * reference implementation of the SQRL (Secure Quick Reliable Login) client.
 *
 * See:    http://csrc.nist.gov/archive/aes/rijndael/wsdindex.html
 *
 * NO COPYRIGHT IS CLAIMED IN THIS WORK, HOWEVER, NEITHER IS ANY WARRANTY MADE
 * REGARDING ITS FITNESS FOR ANY PARTICULAR PURPOSE. USE IT AT YOUR OWN RISK.
 *
 *******************************************************************************/

#include "arch.h"
#include "tls.h"
#if MG_TLS == MG_TLS_BUILTIN
/******************************************************************************/
#define AES_DECRYPTION 1  // whether AES decryption is supported
/******************************************************************************/

#define MG_ENCRYPT 1  // specify whether we're encrypting
#define MG_DECRYPT 0  // or decrypting

#include "tls_aes128.h"

/******************************************************************************
 *  AES_INIT_KEYGEN_TABLES : MUST be called once before any AES use
 ******************************************************************************/
static void aes_init_keygen_tables(void);

/******************************************************************************
 *  AES_SETKEY : called to expand the key for encryption or decryption
 ******************************************************************************/
static int aes_setkey(aes_context *ctx,  // pointer to context
                      int mode,          // 1 or 0 for Encrypt/Decrypt
                      const unsigned char *key,  // AES input key
                      unsigned int keysize);  // size in bytes (must be 16, 24, 32 for
                                      // 128, 192 or 256-bit keys respectively)
                                      // returns 0 for success

/******************************************************************************
 *  AES_CIPHER : called to encrypt or decrypt ONE 128-bit block of data
 ******************************************************************************/
static int aes_cipher(aes_context *ctx,       // pointer to context
                      const unsigned char input[16],  // 128-bit block to en/decipher
                      unsigned char output[16]);      // 128-bit output result block
                                              // returns 0 for success

/******************************************************************************
 *  GCM_CONTEXT : GCM context / holds keytables, instance data, and AES ctx
 ******************************************************************************/
typedef struct {
  int mode;             // cipher direction: encrypt/decrypt
  uint64_t len;         // cipher data length processed so far
  uint64_t add_len;     // total add data length
  uint64_t HL[16];      // precalculated lo-half HTable
  uint64_t HH[16];      // precalculated hi-half HTable
  unsigned char base_ectr[16];  // first counter-mode cipher output for tag
  unsigned char y[16];          // the current cipher-input IV|Counter value
  unsigned char buf[16];        // buf working value
  aes_context aes_ctx;  // cipher context used
} gcm_context;

/******************************************************************************
 *  GCM_SETKEY : sets the GCM (and AES) keying material for use
 ******************************************************************************/
static int gcm_setkey(
    gcm_context *ctx,   // caller-provided context ptr
    const unsigned char *key,   // pointer to cipher key
    const unsigned int keysize  // size in bytes (must be 16, 24, 32 for
                        // 128, 192 or 256-bit keys respectively)
);                      // returns 0 for success

/******************************************************************************
 *
 *  GCM_CRYPT_AND_TAG
 *
 *  This either encrypts or decrypts the user-provided data and, either
 *  way, generates an authentication tag of the requested length. It must be
 *  called with a GCM context whose key has already been set with GCM_SETKEY.
 *
 *  The user would typically call this explicitly to ENCRYPT a buffer of data
 *  and optional associated data, and produce its an authentication tag.
 *
 *  To reverse the process the user would typically call the companion
 *  GCM_AUTH_DECRYPT function to decrypt data and verify a user-provided
 *  authentication tag.  The GCM_AUTH_DECRYPT function calls this function
 *  to perform its decryption and tag generation, which it then compares.
 *
 ******************************************************************************/
static int gcm_crypt_and_tag(
    gcm_context *ctx,    // gcm context with key already setup
    int mode,            // cipher direction: MG_ENCRYPT (1) or MG_DECRYPT (0)
    const unsigned char *iv,     // pointer to the 12-byte initialization vector
    size_t iv_len,       // byte length if the IV. should always be 12
    const unsigned char *add,    // pointer to the non-ciphered additional data
    size_t add_len,      // byte length of the additional AEAD data
    const unsigned char *input,  // pointer to the cipher data source
    unsigned char *output,       // pointer to the cipher data destination
    size_t length,       // byte length of the cipher data
    unsigned char *tag,          // pointer to the tag to be generated
    size_t tag_len);     // byte length of the tag to be generated

/******************************************************************************
 *
 *  GCM_START
 *
 *  Given a user-provided GCM context, this initializes it, sets the encryption
 *  mode, and preprocesses the initialization vector and additional AEAD data.
 *
 ******************************************************************************/
static int gcm_start(
    gcm_context *ctx,  // pointer to user-provided GCM context
    int mode,          // MG_ENCRYPT (1) or MG_DECRYPT (0)
    const unsigned char *iv,   // pointer to initialization vector
    size_t iv_len,     // IV length in bytes (should == 12)
    const unsigned char *add,  // pointer to additional AEAD data (NULL if none)
    size_t add_len);   // length of additional AEAD data (bytes)

/******************************************************************************
 *
 *  GCM_UPDATE
 *
 *  This is called once or more to process bulk plaintext or ciphertext data.
 *  We give this some number of bytes of input and it returns the same number
 *  of output bytes. If called multiple times (which is fine) all but the final
 *  invocation MUST be called with length mod 16 == 0. (Only the final call can
 *  have a partial block length of < 128 bits.)
 *
 ******************************************************************************/
static int gcm_update(gcm_context *ctx,  // pointer to user-provided GCM context
                      size_t length,     // length, in bytes, of data to process
                      const unsigned char *input,  // pointer to source data
                      unsigned char *output);      // pointer to destination data

/******************************************************************************
 *
 *  GCM_FINISH
 *
 *  This is called once after all calls to GCM_UPDATE to finalize the GCM.
 *  It performs the final GHASH to produce the resulting authentication TAG.
 *
 ******************************************************************************/
static int gcm_finish(
    gcm_context *ctx,  // pointer to user-provided GCM context
    unsigned char *tag,        // ptr to tag buffer - NULL if tag_len = 0
    size_t tag_len);   // length, in bytes, of the tag-receiving buf

/******************************************************************************
 *
 *  GCM_ZERO_CTX
 *
 *  The GCM context contains both the GCM context and the AES context.
 *  This includes keying and key-related material which is security-
 *  sensitive, so it MUST be zeroed after use. This function does that.
 *
 ******************************************************************************/
static void gcm_zero_ctx(gcm_context *ctx);

/******************************************************************************
 *
 * THIS SOURCE CODE IS HEREBY PLACED INTO THE PUBLIC DOMAIN FOR THE GOOD OF ALL
 *
 * This is a simple and straightforward implementation of the AES Rijndael
 * 128-bit block cipher designed by Vincent Rijmen and Joan Daemen. The focus
 * of this work was correctness & accuracy.  It is written in 'C' without any
 * particular focus upon optimization or speed. It should be endian (memory
 * byte order) neutral since the few places that care are handled explicitly.
 *
 * This implementation of Rijndael was created by Steven M. Gibson of GRC.com.
 *
 * It is intended for general purpose use, but was written in support of GRC's
 * reference implementation of the SQRL (Secure Quick Reliable Login) client.
 *
 * See:    http://csrc.nist.gov/archive/aes/rijndael/wsdindex.html
 *
 * NO COPYRIGHT IS CLAIMED IN THIS WORK, HOWEVER, NEITHER IS ANY WARRANTY MADE
 * REGARDING ITS FITNESS FOR ANY PARTICULAR PURPOSE. USE IT AT YOUR OWN RISK.
 *
 *******************************************************************************/

static int aes_tables_inited = 0;  // run-once flag for performing key
                                   // expasion table generation (see below)
/*
 *  The following static local tables must be filled-in before the first use of
 *  the GCM or AES ciphers. They are used for the AES key expansion/scheduling
 *  and once built are read-only and thread safe. The "gcm_initialize" function
 *  must be called once during system initialization to populate these arrays
 *  for subsequent use by the AES key scheduler. If they have not been built
 *  before attempted use, an error will be returned to the caller.
 *
 *  NOTE: GCM Encryption/Decryption does NOT REQUIRE AES decryption. Since
 *  GCM uses AES in counter-mode, where the AES cipher output is XORed with
 *  the GCM input, we ONLY NEED AES encryption.  Thus, to save space AES
 *  decryption is typically disabled by setting AES_DECRYPTION to 0 in aes.h.
 */
// We always need our forward tables
static unsigned char FSb[256];     // Forward substitution box (FSb)
static uint32_t FT0[256];  // Forward key schedule assembly tables
static uint32_t FT1[256];
static uint32_t FT2[256];
static uint32_t FT3[256];

#if AES_DECRYPTION         // We ONLY need reverse for decryption
static unsigned char RSb[256];     // Reverse substitution box (RSb)
static uint32_t RT0[256];  // Reverse key schedule assembly tables
static uint32_t RT1[256];
static uint32_t RT2[256];
static uint32_t RT3[256];
#endif /* AES_DECRYPTION */

static uint32_t RCON[10];  // AES round constants

/*
 * Platform Endianness Neutralizing Load and Store Macro definitions
 * AES wants platform-neutral Little Endian (LE) byte ordering
 */
#define GET_UINT32_LE(n, b, i)                                               \
  {                                                                          \
    (n) = ((uint32_t) (b)[(i)]) | ((uint32_t) (b)[(i) + 1] << 8) |           \
          ((uint32_t) (b)[(i) + 2] << 16) | ((uint32_t) (b)[(i) + 3] << 24); \
  }

#define PUT_UINT32_LE(n, b, i)          \
  {                                     \
    (b)[(i)] = (unsigned char) ((n));           \
    (b)[(i) + 1] = (unsigned char) ((n) >> 8);  \
    (b)[(i) + 2] = (unsigned char) ((n) >> 16); \
    (b)[(i) + 3] = (unsigned char) ((n) >> 24); \
  }

/*
 *  AES forward and reverse encryption round processing macros
 */
#define AES_FROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3)          \
  {                                                         \
    X0 = *RK++ ^ FT0[(Y0) & 0xFF] ^ FT1[(Y1 >> 8) & 0xFF] ^ \
         FT2[(Y2 >> 16) & 0xFF] ^ FT3[(Y3 >> 24) & 0xFF];   \
                                                            \
    X1 = *RK++ ^ FT0[(Y1) & 0xFF] ^ FT1[(Y2 >> 8) & 0xFF] ^ \
         FT2[(Y3 >> 16) & 0xFF] ^ FT3[(Y0 >> 24) & 0xFF];   \
                                                            \
    X2 = *RK++ ^ FT0[(Y2) & 0xFF] ^ FT1[(Y3 >> 8) & 0xFF] ^ \
         FT2[(Y0 >> 16) & 0xFF] ^ FT3[(Y1 >> 24) & 0xFF];   \
                                                            \
    X3 = *RK++ ^ FT0[(Y3) & 0xFF] ^ FT1[(Y0 >> 8) & 0xFF] ^ \
         FT2[(Y1 >> 16) & 0xFF] ^ FT3[(Y2 >> 24) & 0xFF];   \
  }

#define AES_RROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3)          \
  {                                                         \
    X0 = *RK++ ^ RT0[(Y0) & 0xFF] ^ RT1[(Y3 >> 8) & 0xFF] ^ \
         RT2[(Y2 >> 16) & 0xFF] ^ RT3[(Y1 >> 24) & 0xFF];   \
                                                            \
    X1 = *RK++ ^ RT0[(Y1) & 0xFF] ^ RT1[(Y0 >> 8) & 0xFF] ^ \
         RT2[(Y3 >> 16) & 0xFF] ^ RT3[(Y2 >> 24) & 0xFF];   \
                                                            \
    X2 = *RK++ ^ RT0[(Y2) & 0xFF] ^ RT1[(Y1 >> 8) & 0xFF] ^ \
         RT2[(Y0 >> 16) & 0xFF] ^ RT3[(Y3 >> 24) & 0xFF];   \
                                                            \
    X3 = *RK++ ^ RT0[(Y3) & 0xFF] ^ RT1[(Y2 >> 8) & 0xFF] ^ \
         RT2[(Y1 >> 16) & 0xFF] ^ RT3[(Y0 >> 24) & 0xFF];   \
  }

/*
 *  These macros improve the readability of the key
 *  generation initialization code by collapsing
 *  repetitive common operations into logical pieces.
 */
#define ROTL8(x) ((x << 8) & 0xFFFFFFFF) | (x >> 24)
#define XTIME(x) ((x << 1) ^ ((x & 0x80) ? 0x1B : 0x00))
#define MUL(x, y) ((x && y) ? pow[(log[x] + log[y]) % 255] : 0)
#define MIX(x, y)                     \
  {                                   \
    y = ((y << 1) | (y >> 7)) & 0xFF; \
    x ^= y;                           \
  }
#define CPY128     \
  {                \
    *RK++ = *SK++; \
    *RK++ = *SK++; \
    *RK++ = *SK++; \
    *RK++ = *SK++; \
  }

/******************************************************************************
 *
 *  AES_INIT_KEYGEN_TABLES
 *
 *  Fills the AES key expansion tables allocated above with their static
 *  data. This is not "per key" data, but static system-wide read-only
 *  table data. THIS FUNCTION IS NOT THREAD SAFE. It must be called once
 *  at system initialization to setup the tables for all subsequent use.
 *
 ******************************************************************************/
void aes_init_keygen_tables(void) {
  int i, x, y, z;  // general purpose iteration and computation locals
  int pow[256];
  int log[256];

  if (aes_tables_inited) return;

  // fill the 'pow' and 'log' tables over GF(2^8)
  for (i = 0, x = 1; i < 256; i++) {
    pow[i] = x;
    log[x] = i;
    x = (x ^ XTIME(x)) & 0xFF;
  }
  // compute the round constants
  for (i = 0, x = 1; i < 10; i++) {
    RCON[i] = (uint32_t) x;
    x = XTIME(x) & 0xFF;
  }
  // fill the forward and reverse substitution boxes
  FSb[0x00] = 0x63;
#if AES_DECRYPTION  // whether AES decryption is supported
  RSb[0x63] = 0x00;
#endif /* AES_DECRYPTION */

  for (i = 1; i < 256; i++) {
    x = y = pow[255 - log[i]];
    MIX(x, y);
    MIX(x, y);
    MIX(x, y);
    MIX(x, y);
    FSb[i] = (unsigned char) (x ^= 0x63);
#if AES_DECRYPTION  // whether AES decryption is supported
    RSb[x] = (unsigned char) i;
#endif /* AES_DECRYPTION */
  }
  // generate the forward and reverse key expansion tables
  for (i = 0; i < 256; i++) {
    x = FSb[i];
    y = XTIME(x) & 0xFF;
    z = (y ^ x) & 0xFF;

    FT0[i] = ((uint32_t) y) ^ ((uint32_t) x << 8) ^ ((uint32_t) x << 16) ^
             ((uint32_t) z << 24);

    FT1[i] = ROTL8(FT0[i]);
    FT2[i] = ROTL8(FT1[i]);
    FT3[i] = ROTL8(FT2[i]);

#if AES_DECRYPTION  // whether AES decryption is supported
    x = RSb[i];

    RT0[i] = ((uint32_t) MUL(0x0E, x)) ^ ((uint32_t) MUL(0x09, x) << 8) ^
             ((uint32_t) MUL(0x0D, x) << 16) ^ ((uint32_t) MUL(0x0B, x) << 24);

    RT1[i] = ROTL8(RT0[i]);
    RT2[i] = ROTL8(RT1[i]);
    RT3[i] = ROTL8(RT2[i]);
#endif /* AES_DECRYPTION */
  }
  aes_tables_inited = 1;  // flag that the tables have been generated
}  // to permit subsequent use of the AES cipher

/******************************************************************************
 *
 *  AES_SET_ENCRYPTION_KEY
 *
 *  This is called by 'aes_setkey' when we're establishing a key for
 *  subsequent encryption.  We give it a pointer to the encryption
 *  context, a pointer to the key, and the key's length in bytes.
 *  Valid lengths are: 16, 24 or 32 bytes (128, 192, 256 bits).
 *
 ******************************************************************************/
static int aes_set_encryption_key(aes_context *ctx, const unsigned char *key,
                                  unsigned int keysize) {
  unsigned int i;                  // general purpose iteration local
  uint32_t *RK = ctx->rk;  // initialize our RoundKey buffer pointer

  for (i = 0; i < (keysize >> 2); i++) {
    GET_UINT32_LE(RK[i], key, i << 2);
  }

  switch (ctx->rounds) {
    case 10:
      for (i = 0; i < 10; i++, RK += 4) {
        RK[4] = RK[0] ^ RCON[i] ^ ((uint32_t) FSb[(RK[3] >> 8) & 0xFF]) ^
                ((uint32_t) FSb[(RK[3] >> 16) & 0xFF] << 8) ^
                ((uint32_t) FSb[(RK[3] >> 24) & 0xFF] << 16) ^
                ((uint32_t) FSb[(RK[3]) & 0xFF] << 24);

        RK[5] = RK[1] ^ RK[4];
        RK[6] = RK[2] ^ RK[5];
        RK[7] = RK[3] ^ RK[6];
      }
      break;

    case 12:
      for (i = 0; i < 8; i++, RK += 6) {
        RK[6] = RK[0] ^ RCON[i] ^ ((uint32_t) FSb[(RK[5] >> 8) & 0xFF]) ^
                ((uint32_t) FSb[(RK[5] >> 16) & 0xFF] << 8) ^
                ((uint32_t) FSb[(RK[5] >> 24) & 0xFF] << 16) ^
                ((uint32_t) FSb[(RK[5]) & 0xFF] << 24);

        RK[7] = RK[1] ^ RK[6];
        RK[8] = RK[2] ^ RK[7];
        RK[9] = RK[3] ^ RK[8];
        RK[10] = RK[4] ^ RK[9];
        RK[11] = RK[5] ^ RK[10];
      }
      break;

    case 14:
      for (i = 0; i < 7; i++, RK += 8) {
        RK[8] = RK[0] ^ RCON[i] ^ ((uint32_t) FSb[(RK[7] >> 8) & 0xFF]) ^
                ((uint32_t) FSb[(RK[7] >> 16) & 0xFF] << 8) ^
                ((uint32_t) FSb[(RK[7] >> 24) & 0xFF] << 16) ^
                ((uint32_t) FSb[(RK[7]) & 0xFF] << 24);

        RK[9] = RK[1] ^ RK[8];
        RK[10] = RK[2] ^ RK[9];
        RK[11] = RK[3] ^ RK[10];

        RK[12] = RK[4] ^ ((uint32_t) FSb[(RK[11]) & 0xFF]) ^
                 ((uint32_t) FSb[(RK[11] >> 8) & 0xFF] << 8) ^
                 ((uint32_t) FSb[(RK[11] >> 16) & 0xFF] << 16) ^
                 ((uint32_t) FSb[(RK[11] >> 24) & 0xFF] << 24);

        RK[13] = RK[5] ^ RK[12];
        RK[14] = RK[6] ^ RK[13];
        RK[15] = RK[7] ^ RK[14];
      }
      break;

    default:
      return -1;
  }
  return (0);
}

#if AES_DECRYPTION  // whether AES decryption is supported

/******************************************************************************
 *
 *  AES_SET_DECRYPTION_KEY
 *
 *  This is called by 'aes_setkey' when we're establishing a
 *  key for subsequent decryption.  We give it a pointer to
 *  the encryption context, a pointer to the key, and the key's
 *  length in bits. Valid lengths are: 128, 192, or 256 bits.
 *
 ******************************************************************************/
static int aes_set_decryption_key(aes_context *ctx, const unsigned char *key,
                                  unsigned int keysize) {
  int i, j;
  aes_context cty;         // a calling aes context for set_encryption_key
  uint32_t *RK = ctx->rk;  // initialize our RoundKey buffer pointer
  uint32_t *SK;
  int ret;

  cty.rounds = ctx->rounds;  // initialize our local aes context
  cty.rk = cty.buf;          // round count and key buf pointer

  if ((ret = aes_set_encryption_key(&cty, key, keysize)) != 0) return (ret);

  SK = cty.rk + cty.rounds * 4;

  CPY128  // copy a 128-bit block from *SK to *RK

      for (i = ctx->rounds - 1, SK -= 8; i > 0; i--, SK -= 8) {
    for (j = 0; j < 4; j++, SK++) {
      *RK++ = RT0[FSb[(*SK) & 0xFF]] ^ RT1[FSb[(*SK >> 8) & 0xFF]] ^
              RT2[FSb[(*SK >> 16) & 0xFF]] ^ RT3[FSb[(*SK >> 24) & 0xFF]];
    }
  }
  CPY128  // copy a 128-bit block from *SK to *RK
      memset(&cty, 0, sizeof(aes_context));  // clear local aes context
  return (0);
}

#endif /* AES_DECRYPTION */

/******************************************************************************
 *
 *  AES_SETKEY
 *
 *  Invoked to establish the key schedule for subsequent encryption/decryption
 *
 ******************************************************************************/
static int aes_setkey(aes_context *ctx,  // AES context provided by our caller
                      int mode,          // ENCRYPT or DECRYPT flag
                      const unsigned char *key,  // pointer to the key
                      unsigned int keysize)      // key length in bytes
{
  // since table initialization is not thread safe, we could either add
  // system-specific mutexes and init the AES key generation tables on
  // demand, or ask the developer to simply call "gcm_initialize" once during
  // application startup before threading begins. That's what we choose.
  if (!aes_tables_inited) return (-1);  // fail the call when not inited.

  ctx->mode = mode;    // capture the key type we're creating
  ctx->rk = ctx->buf;  // initialize our round key pointer

  switch (keysize)  // set the rounds count based upon the keysize
  {
    case 16:
      ctx->rounds = 10;
      break;  // 16-byte, 128-bit key
    case 24:
      ctx->rounds = 12;
      break;  // 24-byte, 192-bit key
    case 32:
      ctx->rounds = 14;
      break;  // 32-byte, 256-bit key
    default:
      return (-1);
  }

#if AES_DECRYPTION
  if (mode == MG_DECRYPT)  // expand our key for encryption or decryption
    return (aes_set_decryption_key(ctx, key, keysize));
  else /* MG_ENCRYPT */
#endif /* AES_DECRYPTION */
    return (aes_set_encryption_key(ctx, key, keysize));
}

/******************************************************************************
 *
 *  AES_CIPHER
 *
 *  Perform AES encryption and decryption.
 *  The AES context will have been setup with the encryption mode
 *  and all keying information appropriate for the task.
 *
 ******************************************************************************/
static int aes_cipher(aes_context *ctx, const unsigned char input[16],
                      unsigned char output[16]) {
  int i;
  uint32_t *RK, X0, X1, X2, X3, Y0, Y1, Y2, Y3;  // general purpose locals

  RK = ctx->rk;

  GET_UINT32_LE(X0, input, 0);
  X0 ^= *RK++;  // load our 128-bit
  GET_UINT32_LE(X1, input, 4);
  X1 ^= *RK++;  // input buffer in a storage
  GET_UINT32_LE(X2, input, 8);
  X2 ^= *RK++;  // memory endian-neutral way
  GET_UINT32_LE(X3, input, 12);
  X3 ^= *RK++;

#if AES_DECRYPTION  // whether AES decryption is supported

  if (ctx->mode == MG_DECRYPT) {
    for (i = (ctx->rounds >> 1) - 1; i > 0; i--) {
      AES_RROUND(Y0, Y1, Y2, Y3, X0, X1, X2, X3);
      AES_RROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3);
    }

    AES_RROUND(Y0, Y1, Y2, Y3, X0, X1, X2, X3);

    X0 = *RK++ ^ ((uint32_t) RSb[(Y0) & 0xFF]) ^
         ((uint32_t) RSb[(Y3 >> 8) & 0xFF] << 8) ^
         ((uint32_t) RSb[(Y2 >> 16) & 0xFF] << 16) ^
         ((uint32_t) RSb[(Y1 >> 24) & 0xFF] << 24);

    X1 = *RK++ ^ ((uint32_t) RSb[(Y1) & 0xFF]) ^
         ((uint32_t) RSb[(Y0 >> 8) & 0xFF] << 8) ^
         ((uint32_t) RSb[(Y3 >> 16) & 0xFF] << 16) ^
         ((uint32_t) RSb[(Y2 >> 24) & 0xFF] << 24);

    X2 = *RK++ ^ ((uint32_t) RSb[(Y2) & 0xFF]) ^
         ((uint32_t) RSb[(Y1 >> 8) & 0xFF] << 8) ^
         ((uint32_t) RSb[(Y0 >> 16) & 0xFF] << 16) ^
         ((uint32_t) RSb[(Y3 >> 24) & 0xFF] << 24);

    X3 = *RK++ ^ ((uint32_t) RSb[(Y3) & 0xFF]) ^
         ((uint32_t) RSb[(Y2 >> 8) & 0xFF] << 8) ^
         ((uint32_t) RSb[(Y1 >> 16) & 0xFF] << 16) ^
         ((uint32_t) RSb[(Y0 >> 24) & 0xFF] << 24);
  } else /* MG_ENCRYPT */
  {
#endif /* AES_DECRYPTION */

    for (i = (ctx->rounds >> 1) - 1; i > 0; i--) {
      AES_FROUND(Y0, Y1, Y2, Y3, X0, X1, X2, X3);
      AES_FROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3);
    }

    AES_FROUND(Y0, Y1, Y2, Y3, X0, X1, X2, X3);

    X0 = *RK++ ^ ((uint32_t) FSb[(Y0) & 0xFF]) ^
         ((uint32_t) FSb[(Y1 >> 8) & 0xFF] << 8) ^
         ((uint32_t) FSb[(Y2 >> 16) & 0xFF] << 16) ^
         ((uint32_t) FSb[(Y3 >> 24) & 0xFF] << 24);

    X1 = *RK++ ^ ((uint32_t) FSb[(Y1) & 0xFF]) ^
         ((uint32_t) FSb[(Y2 >> 8) & 0xFF] << 8) ^
         ((uint32_t) FSb[(Y3 >> 16) & 0xFF] << 16) ^
         ((uint32_t) FSb[(Y0 >> 24) & 0xFF] << 24);

    X2 = *RK++ ^ ((uint32_t) FSb[(Y2) & 0xFF]) ^
         ((uint32_t) FSb[(Y3 >> 8) & 0xFF] << 8) ^
         ((uint32_t) FSb[(Y0 >> 16) & 0xFF] << 16) ^
         ((uint32_t) FSb[(Y1 >> 24) & 0xFF] << 24);

    X3 = *RK++ ^ ((uint32_t) FSb[(Y3) & 0xFF]) ^
         ((uint32_t) FSb[(Y0 >> 8) & 0xFF] << 8) ^
         ((uint32_t) FSb[(Y1 >> 16) & 0xFF] << 16) ^
         ((uint32_t) FSb[(Y2 >> 24) & 0xFF] << 24);

#if AES_DECRYPTION  // whether AES decryption is supported
  }
#endif /* AES_DECRYPTION */

  PUT_UINT32_LE(X0, output, 0);
  PUT_UINT32_LE(X1, output, 4);
  PUT_UINT32_LE(X2, output, 8);
  PUT_UINT32_LE(X3, output, 12);

  return (0);
}
/* end of aes.c */
/******************************************************************************
 *
 * THIS SOURCE CODE IS HEREBY PLACED INTO THE PUBLIC DOMAIN FOR THE GOOD OF ALL
 *
 * This is a simple and straightforward implementation of AES-GCM authenticated
 * encryption. The focus of this work was correctness & accuracy. It is written
 * in straight 'C' without any particular focus upon optimization or speed. It
 * should be endian (memory byte order) neutral since the few places that care
 * are handled explicitly.
 *
 * This implementation of AES-GCM was created by Steven M. Gibson of GRC.com.
 *
 * It is intended for general purpose use, but was written in support of GRC's
 * reference implementation of the SQRL (Secure Quick Reliable Login) client.
 *
 * See:    http://csrc.nist.gov/publications/nistpubs/800-38D/SP-800-38D.pdf
 *         http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/
 *         gcm/gcm-revised-spec.pdf
 *
 * NO COPYRIGHT IS CLAIMED IN THIS WORK, HOWEVER, NEITHER IS ANY WARRANTY MADE
 * REGARDING ITS FITNESS FOR ANY PARTICULAR PURPOSE. USE IT AT YOUR OWN RISK.
 *
 *******************************************************************************/

/******************************************************************************
 *                      ==== IMPLEMENTATION WARNING ====
 *
 *  This code was developed for use within SQRL's fixed environmnent. Thus, it
 *  is somewhat less "general purpose" than it would be if it were designed as
 *  a general purpose AES-GCM library. Specifically, it bothers with almost NO
 *  error checking on parameter limits, buffer bounds, etc. It assumes that it
 *  is being invoked by its author or by someone who understands the values it
 *  expects to receive. Its behavior will be undefined otherwise.
 *
 *  All functions that might fail are defined to return 'ints' to indicate a
 *  problem. Most do not do so now. But this allows for error propagation out
 *  of internal functions if robust error checking should ever be desired.
 *
 ******************************************************************************/

/* Calculating the "GHASH"
 *
 * There are many ways of calculating the so-called GHASH in software, each with
 * a traditional size vs performance tradeoff.  The GHASH (Galois field hash) is
 * an intriguing construction which takes two 128-bit strings (also the cipher's
 * block size and the fundamental operation size for the system) and hashes them
 * into a third 128-bit result.
 *
 * Many implementation solutions have been worked out that use large precomputed
 * table lookups in place of more time consuming bit fiddling, and this approach
 * can be scaled easily upward or downward as needed to change the time/space
 * tradeoff. It's been studied extensively and there's a solid body of theory
 * and practice.  For example, without using any lookup tables an implementation
 * might obtain 119 cycles per byte throughput, whereas using a simple, though
 * large, key-specific 64 kbyte 8-bit lookup table the performance jumps to 13
 * cycles per byte.
 *
 * And Intel's processors have, since 2010, included an instruction which does
 * the entire 128x128->128 bit job in just several 64x64->128 bit pieces.
 *
 * Since SQRL is interactive, and only processing a few 128-bit blocks, I've
 * settled upon a relatively slower but appealing small-table compromise which
 * folds a bunch of not only time consuming but also bit twiddling into a simple
 * 16-entry table which is attributed to Victor Shoup's 1996 work while at
 * Bellcore: "On Fast and Provably Secure MessageAuthentication Based on
 * Universal Hashing."  See: http://www.shoup.net/papers/macs.pdf
 * See, also section 4.1 of the "gcm-revised-spec" cited above.
 */

/*
 *  This 16-entry table of pre-computed constants is used by the
 *  GHASH multiplier to improve over a strictly table-free but
 *  significantly slower 128x128 bit multiple within GF(2^128).
 */
static const uint64_t last4[16] = {
    0x0000, 0x1c20, 0x3840, 0x2460, 0x7080, 0x6ca0, 0x48c0, 0x54e0,
    0xe100, 0xfd20, 0xd940, 0xc560, 0x9180, 0x8da0, 0xa9c0, 0xb5e0};

/*
 * Platform Endianness Neutralizing Load and Store Macro definitions
 * GCM wants platform-neutral Big Endian (BE) byte ordering
 */
#define GET_UINT32_BE(n, b, i)                                            \
  {                                                                       \
    (n) = ((uint32_t) (b)[(i)] << 24) | ((uint32_t) (b)[(i) + 1] << 16) | \
          ((uint32_t) (b)[(i) + 2] << 8) | ((uint32_t) (b)[(i) + 3]);     \
  }

#define PUT_UINT32_BE(n, b, i)          \
  {                                     \
    (b)[(i)] = (unsigned char) ((n) >> 24);     \
    (b)[(i) + 1] = (unsigned char) ((n) >> 16); \
    (b)[(i) + 2] = (unsigned char) ((n) >> 8);  \
    (b)[(i) + 3] = (unsigned char) ((n));       \
  }

/******************************************************************************
 *
 *  GCM_INITIALIZE
 *
 *  Must be called once to initialize the GCM library.
 *
 *  At present, this only calls the AES keygen table generator, which expands
 *  the AES keying tables for use. This is NOT A THREAD-SAFE function, so it
 *  MUST be called during system initialization before a multi-threading
 *  environment is running.
 *
 ******************************************************************************/
int mg_gcm_initialize(void) {
  aes_init_keygen_tables();
  return (0);
}

/******************************************************************************
 *
 *  GCM_MULT
 *
 *  Performs a GHASH operation on the 128-bit input vector 'x', setting
 *  the 128-bit output vector to 'x' times H using our precomputed tables.
 *  'x' and 'output' are seen as elements of GCM's GF(2^128) Galois field.
 *
 ******************************************************************************/
static void gcm_mult(gcm_context *ctx,   // pointer to established context
                     const unsigned char x[16],  // pointer to 128-bit input vector
                     unsigned char output[16])   // pointer to 128-bit output vector
{
  int i;
  unsigned char lo, hi, rem;
  uint64_t zh, zl;

  lo = (unsigned char) (x[15] & 0x0f);
  hi = (unsigned char) (x[15] >> 4);
  zh = ctx->HH[lo];
  zl = ctx->HL[lo];

  for (i = 15; i >= 0; i--) {
    lo = (unsigned char) (x[i] & 0x0f);
    hi = (unsigned char) (x[i] >> 4);

    if (i != 15) {
      rem = (unsigned char) (zl & 0x0f);
      zl = (zh << 60) | (zl >> 4);
      zh = (zh >> 4);
      zh ^= (uint64_t) last4[rem] << 48;
      zh ^= ctx->HH[lo];
      zl ^= ctx->HL[lo];
    }
    rem = (unsigned char) (zl & 0x0f);
    zl = (zh << 60) | (zl >> 4);
    zh = (zh >> 4);
    zh ^= (uint64_t) last4[rem] << 48;
    zh ^= ctx->HH[hi];
    zl ^= ctx->HL[hi];
  }
  PUT_UINT32_BE(zh >> 32, output, 0);
  PUT_UINT32_BE(zh, output, 4);
  PUT_UINT32_BE(zl >> 32, output, 8);
  PUT_UINT32_BE(zl, output, 12);
}

/******************************************************************************
 *
 *  GCM_SETKEY
 *
 *  This is called to set the AES-GCM key. It initializes the AES key
 *  and populates the gcm context's pre-calculated HTables.
 *
 ******************************************************************************/
static int gcm_setkey(
    gcm_context *ctx,    // pointer to caller-provided gcm context
    const unsigned char *key,    // pointer to the AES encryption key
    const unsigned int keysize)  // size in bytes (must be 16, 24, 32 for
                         // 128, 192 or 256-bit keys respectively)
{
  int ret, i, j;
  uint64_t hi, lo;
  uint64_t vl, vh;
  unsigned char h[16];

  memset(ctx, 0, sizeof(gcm_context));  // zero caller-provided GCM context
  memset(h, 0, 16);                     // initialize the block to encrypt

  // encrypt the null 128-bit block to generate a key-based value
  // which is then used to initialize our GHASH lookup tables
  if ((ret = aes_setkey(&ctx->aes_ctx, MG_ENCRYPT, key, keysize)) != 0)
    return (ret);
  if ((ret = aes_cipher(&ctx->aes_ctx, h, h)) != 0) return (ret);

  GET_UINT32_BE(hi, h, 0);  // pack h as two 64-bit ints, big-endian
  GET_UINT32_BE(lo, h, 4);
  vh = (uint64_t) hi << 32 | lo;

  GET_UINT32_BE(hi, h, 8);
  GET_UINT32_BE(lo, h, 12);
  vl = (uint64_t) hi << 32 | lo;

  ctx->HL[8] = vl;  // 8 = 1000 corresponds to 1 in GF(2^128)
  ctx->HH[8] = vh;
  ctx->HH[0] = 0;  // 0 corresponds to 0 in GF(2^128)
  ctx->HL[0] = 0;

  for (i = 4; i > 0; i >>= 1) {
    uint32_t T = (uint32_t) (vl & 1) * 0xe1000000U;
    vl = (vh << 63) | (vl >> 1);
    vh = (vh >> 1) ^ ((uint64_t) T << 32);
    ctx->HL[i] = vl;
    ctx->HH[i] = vh;
  }
  for (i = 2; i < 16; i <<= 1) {
    uint64_t *HiL = ctx->HL + i, *HiH = ctx->HH + i;
    vh = *HiH;
    vl = *HiL;
    for (j = 1; j < i; j++) {
      HiH[j] = vh ^ ctx->HH[j];
      HiL[j] = vl ^ ctx->HL[j];
    }
  }
  return (0);
}

/******************************************************************************
 *
 *    GCM processing occurs four phases: SETKEY, START, UPDATE and FINISH.
 *
 *  SETKEY:
 *
 *   START: Sets the Encryption/Decryption mode.
 *          Accepts the initialization vector and additional data.
 *
 *  UPDATE: Encrypts or decrypts the plaintext or ciphertext.
 *
 *  FINISH: Performs a final GHASH to generate the authentication tag.
 *
 ******************************************************************************
 *
 *  GCM_START
 *
 *  Given a user-provided GCM context, this initializes it, sets the encryption
 *  mode, and preprocesses the initialization vector and additional AEAD data.
 *
 ******************************************************************************/
int gcm_start(gcm_context *ctx,  // pointer to user-provided GCM context
              int mode,          // GCM_ENCRYPT or GCM_DECRYPT
              const unsigned char *iv,   // pointer to initialization vector
              size_t iv_len,     // IV length in bytes (should == 12)
              const unsigned char *add,  // ptr to additional AEAD data (NULL if none)
              size_t add_len)    // length of additional AEAD data (bytes)
{
  int ret;             // our error return if the AES encrypt fails
  unsigned char work_buf[16];  // XOR source built from provided IV if len != 16
  const unsigned char *p;      // general purpose array pointer
  size_t use_len;      // byte count to process, up to 16 bytes
  size_t i;            // local loop iterator

  // since the context might be reused under the same key
  // we zero the working buffers for this next new process
  memset(ctx->y, 0x00, sizeof(ctx->y));
  memset(ctx->buf, 0x00, sizeof(ctx->buf));
  ctx->len = 0;
  ctx->add_len = 0;

  ctx->mode = mode;                // set the GCM encryption/decryption mode
  ctx->aes_ctx.mode = MG_ENCRYPT;  // GCM *always* runs AES in ENCRYPTION mode

  if (iv_len == 12) {            // GCM natively uses a 12-byte, 96-bit IV
    memcpy(ctx->y, iv, iv_len);  // copy the IV to the top of the 'y' buff
    ctx->y[15] = 1;              // start "counting" from 1 (not 0)
  } else  // if we don't have a 12-byte IV, we GHASH whatever we've been given
  {
    memset(work_buf, 0x00, 16);               // clear the working buffer
    PUT_UINT32_BE(iv_len * 8, work_buf, 12);  // place the IV into buffer

    p = iv;
    while (iv_len > 0) {
      use_len = (iv_len < 16) ? iv_len : 16;
      for (i = 0; i < use_len; i++) ctx->y[i] ^= p[i];
      gcm_mult(ctx, ctx->y, ctx->y);
      iv_len -= use_len;
      p += use_len;
    }
    for (i = 0; i < 16; i++) ctx->y[i] ^= work_buf[i];
    gcm_mult(ctx, ctx->y, ctx->y);
  }
  if ((ret = aes_cipher(&ctx->aes_ctx, ctx->y, ctx->base_ectr)) != 0)
    return (ret);

  ctx->add_len = add_len;
  p = add;
  while (add_len > 0) {
    use_len = (add_len < 16) ? add_len : 16;
    for (i = 0; i < use_len; i++) ctx->buf[i] ^= p[i];
    gcm_mult(ctx, ctx->buf, ctx->buf);
    add_len -= use_len;
    p += use_len;
  }
  return (0);
}

/******************************************************************************
 *
 *  GCM_UPDATE
 *
 *  This is called once or more to process bulk plaintext or ciphertext data.
 *  We give this some number of bytes of input and it returns the same number
 *  of output bytes. If called multiple times (which is fine) all but the final
 *  invocation MUST be called with length mod 16 == 0. (Only the final call can
 *  have a partial block length of < 128 bits.)
 *
 ******************************************************************************/
int gcm_update(gcm_context *ctx,    // pointer to user-provided GCM context
               size_t length,       // length, in bytes, of data to process
               const unsigned char *input,  // pointer to source data
               unsigned char *output)       // pointer to destination data
{
  int ret;         // our error return if the AES encrypt fails
  unsigned char ectr[16];  // counter-mode cipher output for XORing
  size_t use_len;  // byte count to process, up to 16 bytes
  size_t i;        // local loop iterator

  ctx->len += length;  // bump the GCM context's running length count

  while (length > 0) {
    // clamp the length to process at 16 bytes
    use_len = (length < 16) ? length : 16;

    // increment the context's 128-bit IV||Counter 'y' vector
    for (i = 16; i > 12; i--)
      if (++ctx->y[i - 1] != 0) break;

    // encrypt the context's 'y' vector under the established key
    if ((ret = aes_cipher(&ctx->aes_ctx, ctx->y, ectr)) != 0) return (ret);

    // encrypt or decrypt the input to the output
    if (ctx->mode == MG_ENCRYPT) {
      for (i = 0; i < use_len; i++) {
        // XOR the cipher's ouptut vector (ectr) with our input
        output[i] = (unsigned char) (ectr[i] ^ input[i]);
        // now we mix in our data into the authentication hash.
        // if we're ENcrypting we XOR in the post-XOR (output)
        // results, but if we're DEcrypting we XOR in the input
        // data
        ctx->buf[i] ^= output[i];
      }
    } else {
      for (i = 0; i < use_len; i++) {
        // but if we're DEcrypting we XOR in the input data first,
        // i.e. before saving to ouput data, otherwise if the input
        // and output buffer are the same (inplace decryption) we
        // would not get the correct auth tag

        ctx->buf[i] ^= input[i];

        // XOR the cipher's ouptut vector (ectr) with our input
        output[i] = (unsigned char) (ectr[i] ^ input[i]);
      }
    }
    gcm_mult(ctx, ctx->buf, ctx->buf);  // perform a GHASH operation

    length -= use_len;  // drop the remaining byte count to process
    input += use_len;   // bump our input pointer forward
    output += use_len;  // bump our output pointer forward
  }
  return (0);
}

/******************************************************************************
 *
 *  GCM_FINISH
 *
 *  This is called once after all calls to GCM_UPDATE to finalize the GCM.
 *  It performs the final GHASH to produce the resulting authentication TAG.
 *
 ******************************************************************************/
int gcm_finish(gcm_context *ctx,  // pointer to user-provided GCM context
               unsigned char *tag,        // pointer to buffer which receives the tag
               size_t tag_len)    // length, in bytes, of the tag-receiving buf
{
  unsigned char work_buf[16];
  uint64_t orig_len = ctx->len * 8;
  uint64_t orig_add_len = ctx->add_len * 8;
  size_t i;

  if (tag_len != 0) memcpy(tag, ctx->base_ectr, tag_len);

  if (orig_len || orig_add_len) {
    memset(work_buf, 0x00, 16);

    PUT_UINT32_BE((orig_add_len >> 32), work_buf, 0);
    PUT_UINT32_BE((orig_add_len), work_buf, 4);
    PUT_UINT32_BE((orig_len >> 32), work_buf, 8);
    PUT_UINT32_BE((orig_len), work_buf, 12);

    for (i = 0; i < 16; i++) ctx->buf[i] ^= work_buf[i];
    gcm_mult(ctx, ctx->buf, ctx->buf);
    for (i = 0; i < tag_len; i++) tag[i] ^= ctx->buf[i];
  }
  return (0);
}

/******************************************************************************
 *
 *  GCM_CRYPT_AND_TAG
 *
 *  This either encrypts or decrypts the user-provided data and, either
 *  way, generates an authentication tag of the requested length. It must be
 *  called with a GCM context whose key has already been set with GCM_SETKEY.
 *
 *  The user would typically call this explicitly to ENCRYPT a buffer of data
 *  and optional associated data, and produce its an authentication tag.
 *
 *  To reverse the process the user would typically call the companion
 *  GCM_AUTH_DECRYPT function to decrypt data and verify a user-provided
 *  authentication tag.  The GCM_AUTH_DECRYPT function calls this function
 *  to perform its decryption and tag generation, which it then compares.
 *
 ******************************************************************************/
int gcm_crypt_and_tag(
    gcm_context *ctx,    // gcm context with key already setup
    int mode,            // cipher direction: GCM_ENCRYPT or GCM_DECRYPT
    const unsigned char *iv,     // pointer to the 12-byte initialization vector
    size_t iv_len,       // byte length if the IV. should always be 12
    const unsigned char *add,    // pointer to the non-ciphered additional data
    size_t add_len,      // byte length of the additional AEAD data
    const unsigned char *input,  // pointer to the cipher data source
    unsigned char *output,       // pointer to the cipher data destination
    size_t length,       // byte length of the cipher data
    unsigned char *tag,          // pointer to the tag to be generated
    size_t tag_len)      // byte length of the tag to be generated
{                        /*
                            assuming that the caller has already invoked gcm_setkey to
                            prepare the gcm context with the keying material, we simply
                            invoke each of the three GCM sub-functions in turn...
                         */
  gcm_start(ctx, mode, iv, iv_len, add, add_len);
  gcm_update(ctx, length, input, output);
  gcm_finish(ctx, tag, tag_len);
  return (0);
}

/******************************************************************************
 *
 *  GCM_ZERO_CTX
 *
 *  The GCM context contains both the GCM context and the AES context.
 *  This includes keying and key-related material which is security-
 *  sensitive, so it MUST be zeroed after use. This function does that.
 *
 ******************************************************************************/
void gcm_zero_ctx(gcm_context *ctx) {
  // zero the context originally provided to us
  memset(ctx, 0, sizeof(gcm_context));
}
//
//  aes-gcm.c
//  Pods
//
//  Created by Markus Kosmal on 20/11/14.
//
//

int mg_aes_gcm_encrypt(unsigned char *output,  //
                       const unsigned char *input, size_t input_length,
                       const unsigned char *key, const size_t key_len,
                       const unsigned char *iv, const size_t iv_len,
                       unsigned char *aead, size_t aead_len, unsigned char *tag,
                       const size_t tag_len) {
  int ret = 0;      // our return value
  gcm_context ctx;  // includes the AES context structure

  gcm_setkey(&ctx, key, (unsigned int) key_len);

  ret = gcm_crypt_and_tag(&ctx, MG_ENCRYPT, iv, iv_len, aead, aead_len, input,
                          output, input_length, tag, tag_len);

  gcm_zero_ctx(&ctx);

  return (ret);
}

int mg_aes_gcm_decrypt(unsigned char *output, const unsigned char *input,
                       size_t input_length, const unsigned char *key,
                       const size_t key_len, const unsigned char *iv,
                       const size_t iv_len) {
  int ret = 0;      // our return value
  gcm_context ctx;  // includes the AES context structure

  size_t tag_len = 0;
  unsigned char *tag_buf = NULL;

  gcm_setkey(&ctx, key, (unsigned int) key_len);

  ret = gcm_crypt_and_tag(&ctx, MG_DECRYPT, iv, iv_len, NULL, 0, input, output,
                          input_length, tag_buf, tag_len);

  gcm_zero_ctx(&ctx);

  return (ret);
}
#endif
// End of aes128 PD
