#pragma once

#include "arch.h"
#include "str.h"
#include "tls.h"

struct mg_jwt_opts {
  struct mg_str claims;  // JSON payload
  struct mg_str header;  // Extra header members, no braces; alg/typ are set
  struct mg_str kid;     // Key ID protected header
  struct mg_str secret;  // HS256 secret
  const uint8_t *private_key;  // ES256 private key (MG_TLS_BUILTIN only)
  const uint8_t *public_key;   // ES256 public key (MG_TLS_BUILTIN only)
};

// Signs JSON claims as a compact JWT. Returns JWT length, or 0 on error.
size_t mg_jwt_sign_hs256(const struct mg_jwt_opts *opts, char *buf,
                         size_t len);

// Verifies JWT and decodes JSON claims into buf. Returns claims length, or 0.
size_t mg_jwt_verify_hs256(struct mg_str jwt, const struct mg_jwt_opts *opts,
                           char *buf, size_t len);

size_t mg_jwt_sign_es256(const struct mg_jwt_opts *opts, char *buf,
                         size_t len);
size_t mg_jwt_verify_es256(struct mg_str jwt, const struct mg_jwt_opts *opts,
                           char *buf, size_t len);
