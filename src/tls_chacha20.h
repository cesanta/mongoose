// portable8439 v1.0.1
// Source: https://github.com/DavyLandman/portable8439
// Licensed under CC0-1.0
// Contains poly1305-donna e6ad6e091d30d7f4ec2d4f978be1fcfcbce72781 (Public
// Domain)

#include "arch.h"
#include "config.h"
#include "tls.h"

#if MG_TLS == MG_TLS_BUILTIN
#ifndef __PORTABLE_8439_H
#define __PORTABLE_8439_H
#if defined(__cplusplus)
extern "C" {
#endif

// provide your own decl specificier like -DPORTABLE_8439_DECL=ICACHE_RAM_ATTR
#ifndef PORTABLE_8439_DECL
#define PORTABLE_8439_DECL
#endif

/*
 This library implements RFC 8439 a.k.a. ChaCha20-Poly1305 AEAD

 You can use this library to avoid attackers mutating or reusing your
 encrypted messages. This does assume you never reuse a nonce+key pair and,
 if possible, carefully pick your associated data.
*/

/* Make sure we are either nested in C++ or running in a C99+ compiler
#if !defined(__cplusplus) && !defined(_MSC_VER) && \
    (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#error "C99 or newer required"
#endif */

// #if CHAR_BIT > 8
// #    error "Systems without native octals not suppoted"
// #endif

#if defined(_MSC_VER) || defined(__cplusplus)
// add restrict support is possible
#if (defined(_MSC_VER) && _MSC_VER >= 1900) || defined(__clang__) || \
    defined(__GNUC__)
#define restrict __restrict
#else
#define restrict
#endif
#endif

#define RFC_8439_TAG_SIZE (16)
#define RFC_8439_KEY_SIZE (32)
#define RFC_8439_NONCE_SIZE (12)

/*
    Encrypt/Seal plain text bytes into a cipher text that can only be
    decrypted by knowing the key, nonce and associated data.

    input:
        - key: RFC_8439_KEY_SIZE bytes that all parties have agreed
            upon beforehand
        - nonce: RFC_8439_NONCE_SIZE bytes that should never be repeated
            for the same key. A counter or a pseudo-random value are fine.
        - ad: associated data to include with calculating the tag of the
            cipher text. Can be null for empty.
        - plain_text: data to be encrypted, pointer + size should not overlap
            with cipher_text pointer

    output:
        - cipher_text: encrypted plain_text with a tag appended. Make sure to
            allocate at least plain_text_size + RFC_8439_TAG_SIZE

    returns:
        - size of bytes written to cipher_text, can be -1 if overlapping
            pointers are passed for plain_text and cipher_text
*/
PORTABLE_8439_DECL size_t mg_chacha20_poly1305_encrypt(
    uint8_t *restrict cipher_text, const uint8_t key[RFC_8439_KEY_SIZE],
    const uint8_t nonce[RFC_8439_NONCE_SIZE], const uint8_t *restrict ad,
    size_t ad_size, const uint8_t *restrict plain_text, size_t plain_text_size);

/*
    Decrypt/unseal cipher text given the right key, nonce, and additional data.

    input:
        - key: RFC_8439_KEY_SIZE bytes that all parties have agreed
            upon beforehand
        - nonce: RFC_8439_NONCE_SIZE bytes that should never be repeated for
            the same key. A counter or a pseudo-random value are fine.
        - ad: associated data to include with calculating the tag of the
            cipher text. Can be null for empty.
        - cipher_text: encrypted message.

    output:
        - plain_text: data to be encrypted, pointer + size should not overlap
            with cipher_text pointer, leave at least enough room for
            cipher_text_size - RFC_8439_TAG_SIZE

    returns:
        - size of bytes written to plain_text, -1 signals either:
            - incorrect key/nonce/ad
            - corrupted cipher_text
            - overlapping pointers are passed for plain_text and cipher_text
*/
PORTABLE_8439_DECL size_t mg_chacha20_poly1305_decrypt(
    uint8_t *restrict plain_text, const uint8_t key[RFC_8439_KEY_SIZE],
    const uint8_t nonce[RFC_8439_NONCE_SIZE],
    const uint8_t *restrict cipher_text, size_t cipher_text_size);
#if defined(__cplusplus)
}
#endif
#endif
#endif
