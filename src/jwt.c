#include "jwt.h"
#include "base64.h"
#include "json.h"
#include "log.h"
#include "printf.h"
#include "sha256.h"
#include "util.h"

#if MG_TLS == MG_TLS_BUILTIN
#include "tls_uecc.h"
#endif

static size_t jwt_split(struct mg_str jwt, struct mg_str *h, struct mg_str *p,
                        struct mg_str *s) {
  size_t i, a = jwt.len, b = jwt.len;
  for (i = 0; i < jwt.len; i++) {
    if (jwt.buf[i] == '.') {
      if (a == jwt.len) a = i;
      if (a != i) b = i;
      if (b != jwt.len) break;
    }
  }
  if (a == jwt.len || b == jwt.len || b + 1 >= jwt.len) return 0;
  *h = mg_str_n(jwt.buf, a);
  *p = mg_str_n(jwt.buf + a + 1, b - a - 1);
  *s = mg_str_n(jwt.buf + b + 1, jwt.len - b - 1);
  return b;
}

static bool jwt_alg(struct mg_str h, const char *alg) {
  char got[16];
  size_t len = h.len * 3 / 4 + 2;
  char *buf = (char *) mg_calloc(1, len);
  bool ok;
  if (buf == NULL) return false;
  ok = mg_base64url_decode(h.buf, h.len, buf, len) > 0 &&
       mg_json_unescape(mg_str(buf), "$.alg", got, sizeof(got)) > 0 &&
       strcmp(got, alg) == 0;
  mg_free(buf);
  return ok;
}

static size_t jwt_header_payload(const char *alg, const struct mg_jwt_opts *opts,
                                 char *buf, size_t len) {
  char *header, *kid = NULL;
  size_t n, m;
  if (opts->kid.len > 0) {
    kid = mg_mprintf(",%m:%m", MG_ESC("kid"), mg_print_esc,
                     (int) opts->kid.len, opts->kid.buf);
    if (kid == NULL) return 0;
  }
  header = mg_mprintf("{%m:%m,%m:%m%.*s%.*s%.*s}", MG_ESC("alg"),
                      MG_ESC(alg), MG_ESC("typ"), MG_ESC("JWT"),
                      kid == NULL ? 0 : (int) strlen(kid),
                      kid == NULL ? "" : kid,
                      opts->header.len > 0 ? 1 : 0, ",",
                      (int) opts->header.len, opts->header.buf);
  mg_free(kid);
  if (header == NULL) return 0;
  n = mg_base64url_encode((uint8_t *) header, strlen(header), buf, len);
  mg_free(header);
  if (n == 0 || n + 1 >= len) return 0;
  buf[n++] = '.';
  m = mg_base64url_encode((uint8_t *) opts->claims.buf, opts->claims.len,
                          buf + n, len - n);
  return m == 0 ? 0 : n + m;
}

size_t mg_jwt_sign_hs256(const struct mg_jwt_opts *opts, char *buf,
                         size_t len) {
  uint8_t sig[32];
  size_t n = jwt_header_payload("HS256", opts, buf, len);
  size_t m;
  if (n == 0 || n + 1 >= len) return 0;
  mg_hmac_sha256(sig, (uint8_t *) opts->secret.buf, opts->secret.len,
                 (uint8_t *) buf, n);
  buf[n++] = '.';
  m = mg_base64url_encode(sig, sizeof(sig), buf + n, len - n);
  return m == 0 ? 0 : n + m;
}

size_t mg_jwt_verify_hs256(struct mg_str jwt, const struct mg_jwt_opts *opts,
                           char *buf, size_t len) {
  struct mg_str h, p, s;
  uint8_t sig[33], digest[32];
  size_t n = jwt_split(jwt, &h, &p, &s);
  if (n == 0 || !jwt_alg(h, "HS256")) return 0;
  if (mg_base64url_decode(s.buf, s.len, (char *) sig, sizeof(sig)) !=
      sizeof(digest))
    return 0;
  mg_hmac_sha256(digest, (uint8_t *) opts->secret.buf, opts->secret.len,
                 (uint8_t *) jwt.buf, n);
  if (!mg_memeq(sig, digest, sizeof(digest))) return 0;
  return mg_base64url_decode(p.buf, p.len, buf, len);
}

#if MG_TLS == MG_TLS_BUILTIN
typedef struct {
  MG_UECC_HashContext uECC;
  mg_sha256_ctx sha256;
} jwt_hash_ctx;

static void jwt_hash_init(const MG_UECC_HashContext *ctx) {
  jwt_hash_ctx *c = (jwt_hash_ctx *) ctx;
  mg_sha256_init(&c->sha256);
}

static void jwt_hash_update(const MG_UECC_HashContext *ctx, const uint8_t *msg,
                            unsigned len) {
  jwt_hash_ctx *c = (jwt_hash_ctx *) ctx;
  mg_sha256_update(&c->sha256, msg, len);
}

static void jwt_hash_finish(const MG_UECC_HashContext *ctx, uint8_t *hash) {
  jwt_hash_ctx *c = (jwt_hash_ctx *) ctx;
  mg_sha256_final(hash, &c->sha256);
}

size_t mg_jwt_sign_es256(const struct mg_jwt_opts *opts, char *buf,
                         size_t len) {
  uint8_t hash[32], sig[64], tmp[2 * 32 + 64];
  jwt_hash_ctx ctx = {
      {jwt_hash_init, jwt_hash_update, jwt_hash_finish, 64, 32, tmp},
      {{0}, 0, 0, {0}}};
  size_t n = jwt_header_payload("ES256", opts, buf, len);
  size_t m;
  if (n == 0 || n + 1 >= len) return 0;
  mg_sha256(hash, (uint8_t *) buf, n);
  if (mg_uecc_sign_deterministic(opts->private_key, hash, sizeof(hash),
                                 &ctx.uECC, sig, mg_uecc_secp256r1()) != 1)
    return 0;
  buf[n++] = '.';
  m = mg_base64url_encode(sig, sizeof(sig), buf + n, len - n);
  return m == 0 ? 0 : n + m;
}

size_t mg_jwt_verify_es256(struct mg_str jwt, const struct mg_jwt_opts *opts,
                           char *buf, size_t len) {
  struct mg_str h, p, s;
  uint8_t sig[65], hash[32];
  size_t n = jwt_split(jwt, &h, &p, &s);
  if (n == 0 || !jwt_alg(h, "ES256")) return 0;
  if (mg_base64url_decode(s.buf, s.len, (char *) sig, sizeof(sig)) != 64)
    return 0;
  mg_sha256(hash, (uint8_t *) jwt.buf, n);
  if (mg_uecc_verify(opts->public_key, hash, sizeof(hash), sig,
                     mg_uecc_secp256r1()) != 1)
    return 0;
  return mg_base64url_decode(p.buf, p.len, buf, len);
}
#else
size_t mg_jwt_sign_es256(const struct mg_jwt_opts *opts, char *buf,
                         size_t len) {
  (void) opts, (void) buf, (void) len;
  MG_ERROR(("JWT ES256 requires built-in TLS"));
  return 0;
}

size_t mg_jwt_verify_es256(struct mg_str jwt, const struct mg_jwt_opts *opts,
                           char *buf, size_t len) {
  (void) jwt, (void) opts, (void) buf, (void) len;
  return mg_jwt_sign_es256(NULL, NULL, 0);
}
#endif
