#ifndef TLS_X15519_H
#define TLS_X15519_H

#include "arch.h"

#define X25519_BYTES 32
extern const uint8_t X25519_BASE_POINT[X25519_BYTES];

int mg_tls_x25519(uint8_t out[X25519_BYTES], const uint8_t scalar[X25519_BYTES],
                  const uint8_t x1[X25519_BYTES], int clamp);


#endif /* TLS_X15519_H */
