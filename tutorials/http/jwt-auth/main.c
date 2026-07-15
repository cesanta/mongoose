// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// HTTP server example. It issues and verifies JWT Bearer tokens.

#include "mongoose.h"

static const char *s_listen_on = "http://0.0.0.0:8000";
static const char *s_secret = "change-me";
enum { TOKEN_LEN = 1024, CLAIMS_LEN = 256, JWT_LEN = 1024 };
static const char *s_ec_private_key =
    "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEIAVdo8UAScxG7jiuNY2UZESNX/KPH8qJ0u0gOMMsAzYWoAoGCCqGSM49\n"
    "AwEHoUQDQgAEqN6BIhvgbk7ecmUcn8Da9Avkj/uDNERtqWJG9r/or26X4u9jR5Jl\n"
    "4hf5Gx17YJkq5/z3k6ogPDPpoAYWIw1/sw==\n"
    "-----END EC PRIVATE KEY-----\n";

#if MG_TLS == MG_TLS_BUILTIN
static size_t verify_es256(struct mg_str token, char *claims, size_t len) {
  uint8_t key[32], pub[64];
  struct mg_jwt_opts opts = {.public_key = pub};
  // This keeps the example self-contained. A real verifier would usually keep
  // only the issuer public key, selected by kid, not the signing private key.
  if (mg_uecc_parse_private_key(mg_str(s_ec_private_key), key, sizeof(key)) !=
          32 ||
      mg_uecc_compute_public_key(key, pub, mg_uecc_secp256r1()) != 1) {
    return 0;
  }
  return mg_jwt_verify_es256(token, &opts, claims, len);
}
#endif

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    char user[1], *token, *claims, *jwt;
    token = (char *) mg_calloc(1, TOKEN_LEN);
    claims = (char *) mg_calloc(1, CLAIMS_LEN);
    jwt = (char *) mg_calloc(1, JWT_LEN);
    if (token == NULL || claims == NULL || jwt == NULL) {
      mg_http_reply(c, 500, "", "OOM\n");
      mg_free(token);
      mg_free(claims);
      mg_free(jwt);
      return;
    }
    mg_http_creds(hm, user, sizeof(user), token, TOKEN_LEN);

    if (mg_match(hm->uri, mg_str("/login-hs256"), NULL)) {
      struct mg_jwt_opts opts = {.kid = mg_str("hmac-key-1"),
                                 .secret = mg_str(s_secret)};
      size_t n;
      mg_snprintf(claims, CLAIMS_LEN, "{%m:%m,%m:%m}", MG_ESC("sub"),
                  MG_ESC("admin"), MG_ESC("scope"),
                  MG_ESC("dash:read dash:write"));
      opts.claims = mg_str(claims);
      n = mg_jwt_sign_hs256(&opts, jwt, JWT_LEN);
      mg_http_reply(c, n == 0 ? 500 : 200, "Content-Type: application/json\r\n",
                    "{%m:%m}\n", MG_ESC("token"), MG_ESC(n == 0 ? "" : jwt));

#if MG_TLS == MG_TLS_BUILTIN
    } else if (mg_match(hm->uri, mg_str("/login-es256"), NULL)) {
      uint8_t key[32];
      struct mg_jwt_opts opts = {.kid = mg_str("ec-key-1"),
                                 .private_key = key};
      size_t keysz, n;
      keysz =
          mg_uecc_parse_private_key(mg_str(s_ec_private_key), key, sizeof(key));
      // ES256 uses a raw 64-byte r || s signature. mg_jwt_sign_es256() signs it
      // deterministically; applications can also hash the signing input and
      // call mg_uecc_sign() directly if they need random-nonce ECDSA.
      mg_snprintf(claims, CLAIMS_LEN, "{%m:%m,%m:%m}", MG_ESC("sub"),
                  MG_ESC("admin"), MG_ESC("scope"), MG_ESC("dash:read"));
      opts.claims = mg_str(claims);
      n = keysz == 32 ? mg_jwt_sign_es256(&opts, jwt, JWT_LEN) : 0;
      mg_http_reply(c, n == 0 ? 500 : 200, "Content-Type: application/json\r\n",
                    "{%m:%m}\n", MG_ESC("token"), MG_ESC(n == 0 ? "" : jwt));
#endif

    } else {
      struct mg_jwt_opts opts = {.secret = mg_str(s_secret)};
      if (mg_jwt_verify_hs256(mg_str(token), &opts, claims, CLAIMS_LEN) > 0) {
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                      claims);
#if MG_TLS == MG_TLS_BUILTIN
      } else if (verify_es256(mg_str(token), claims, CLAIMS_LEN) > 0) {
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                      claims);
#endif
      } else {
        mg_http_reply(c, 401, "WWW-Authenticate: Bearer\r\n", "Unauthorized\n");
      }
    }
    mg_free(token);
    mg_free(claims);
    mg_free(jwt);
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_log_set(MG_LL_DEBUG);
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  return 0;
}
