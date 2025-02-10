#ifndef TLS_RSA_H
#define TLS_RSA_H

#include "arch.h"
int mg_rsa_mod_pow(const uint8_t *mod, size_t modsz, const uint8_t *exp, size_t expsz, const uint8_t *msg, size_t msgsz, uint8_t *out, size_t outsz);

#endif // TLS_RSA_H
