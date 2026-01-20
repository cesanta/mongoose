#ifndef TLS_RSA_H
#define TLS_RSA_H

#include "arch.h"
int mg_rsa_mod_pow(const uint8_t *mod, size_t modsz, const uint8_t *exp, size_t expsz, const uint8_t *msg, size_t msgsz, uint8_t *out, size_t outsz);
int mg_rsa_crt_sign(const uint8_t *em, size_t em_len,
                    const uint8_t *dP, size_t dP_len,
                    const uint8_t *dQ, size_t dQ_len,
                    const uint8_t *p, size_t p_len,
                    const uint8_t *q, size_t q_len,
                    const uint8_t *qInv, size_t qInv_len,
                    uint8_t *signature, size_t sig_len);
#endif // TLS_RSA_H
