#include "tls.h"

#if MG_TLS == MG_TLS_BUILTIN

// handshake is re-entrant, so we need to keep track of its state
enum mg_tls_hs_state {
  MG_TLS_HS_CLIENT_HELLO,  // first, wait for ClientHello
  MG_TLS_HS_SERVER_HELLO,  // then, send all server handshake data at once
  MG_TLS_HS_CLIENT_CHANGE_CIPHER,  // finally wait for ClientChangeCipher
  MG_TLS_HS_CLIENT_FINISH,         // and ClientFinish (encrypted)
  MG_TLS_HS_DONE,  // finish handshake, start application data flow
};

// per-connection TLS data
struct tls_data {
  enum mg_tls_hs_state state;  // keep track of connection handshake progress

  struct mg_iobuf send; // For the receive path, we're reusing c->rtls

  mg_sha256_ctx sha256;  // incremental SHA-256 hash for TLS handshake

  uint32_t sseq;  // server sequence number, used in encryption
  uint32_t cseq;  // client sequence number, used in decryption

  uint8_t session_id[32];  // client session ID between the handshake states
  uint8_t x25519_cli[32];  // client X25519 key between the handshake states
  uint8_t x25519_sec[32];  // x25519 secret between the handshake states

  struct mg_str server_cert_der;  // server certificate in DER format
  uint8_t server_key[32];         // server EC private key

  // keys for AES encryption
  uint8_t handshake_secret[32];
  uint8_t server_write_key[16];
  uint8_t server_write_iv[12];
  uint8_t server_finished_key[32];
  uint8_t client_write_key[16];
  uint8_t client_write_iv[12];
  uint8_t client_finished_key[32];
};

#define MG_LOAD_BE16(p) ((uint16_t) ((MG_U8P(p)[0] << 8U) | MG_U8P(p)[1]))
#define TLS_HDR_SIZE 5  // 1 byte type, 2 bytes version, 2 bytes len

// for derived tls keys we need SHA256([0]*32)
static uint8_t zeros[32] = {0};
static uint8_t zeros_sha256_digest[32] =
    "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24"
    "\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55";

#define X25519_BYTES 32
const uint8_t X25519_BASE_POINT[X25519_BYTES] = {9};

#define X25519_WBITS 32

typedef uint32_t limb_t;
typedef uint64_t dlimb_t;
typedef int64_t sdlimb_t;
#define LIMB(x) (uint32_t)(x##ull), (uint32_t) ((x##ull) >> 32)

#define NLIMBS (256 / X25519_WBITS)
typedef limb_t fe[NLIMBS];

static limb_t umaal(limb_t *carry, limb_t acc, limb_t mand, limb_t mier) {
  dlimb_t tmp = (dlimb_t) mand * mier + acc + *carry;
  *carry = (limb_t) (tmp >> X25519_WBITS);
  return (limb_t) tmp;
}

// These functions are implemented in terms of umaal on ARM
static limb_t adc(limb_t *carry, limb_t acc, limb_t mand) {
  dlimb_t total = (dlimb_t) *carry + acc + mand;
  *carry = (limb_t) (total >> X25519_WBITS);
  return (limb_t) total;
}

static limb_t adc0(limb_t *carry, limb_t acc) {
  dlimb_t total = (dlimb_t) *carry + acc;
  *carry = (limb_t) (total >> X25519_WBITS);
  return (limb_t) total;
}

// - Precondition: carry is small.
// - Invariant: result of propagate is < 2^255 + 1 word
// - In particular, always less than 2p.
// - Also, output x >= min(x,19)
static void propagate(fe x, limb_t over) {
  unsigned i;
  limb_t carry;
  over = x[NLIMBS - 1] >> (X25519_WBITS - 1) | over << 1;
  x[NLIMBS - 1] &= ~((limb_t) 1 << (X25519_WBITS - 1));

  carry = over * 19;
  for (i = 0; i < NLIMBS; i++) {
    x[i] = adc0(&carry, x[i]);
  }
}

static void add(fe out, const fe a, const fe b) {
  unsigned i;
  limb_t carry = 0;
  for (i = 0; i < NLIMBS; i++) {
    out[i] = adc(&carry, a[i], b[i]);
  }
  propagate(out, carry);
}

static void sub(fe out, const fe a, const fe b) {
  unsigned i;
  sdlimb_t carry = -38;
  for (i = 0; i < NLIMBS; i++) {
    carry = carry + a[i] - b[i];
    out[i] = (limb_t) carry;
    carry >>= X25519_WBITS;
  }
  propagate(out, (limb_t) (1 + carry));
}

// `b` can contain less than 8 limbs, thus we use `limb_t *` instead of `fe`
// to avoid build warnings
static void mul(fe out, const fe a, const limb_t *b, unsigned nb) {
  limb_t accum[2 * NLIMBS] = {0};
  unsigned i, j;

  limb_t carry2;
  for (i = 0; i < nb; i++) {
    limb_t mand = b[i];
    carry2 = 0;
    for (j = 0; j < NLIMBS; j++) {
      accum[i + j] = umaal(&carry2, accum[i + j], mand, a[j]);
    }
    accum[i + j] = carry2;
  }

  carry2 = 0;
  for (j = 0; j < NLIMBS; j++) {
    out[j] = umaal(&carry2, accum[j], 38, accum[j + NLIMBS]);
  }
  propagate(out, carry2);
}

static void sqr(fe out, const fe a) {
  mul(out, a, a, NLIMBS);
}
static void mul1(fe out, const fe a) {
  mul(out, a, out, NLIMBS);
}
static void sqr1(fe a) {
  mul1(a, a);
}

static void condswap(limb_t a[2 * NLIMBS], limb_t b[2 * NLIMBS],
                     limb_t doswap) {
  unsigned i;
  for (i = 0; i < 2 * NLIMBS; i++) {
    limb_t xor = (a[i] ^ b[i]) & doswap;
    a[i] ^= xor;
    b[i] ^= xor;
  }
}

// Canonicalize a field element x, reducing it to the least residue which is
// congruent to it mod 2^255-19
// - Precondition: x < 2^255 + 1 word
static limb_t canon(fe x) {
  // First, add 19.
  unsigned i;
  limb_t carry0 = 19;
  limb_t res;
  sdlimb_t carry;
  for (i = 0; i < NLIMBS; i++) {
    x[i] = adc0(&carry0, x[i]);
  }
  propagate(x, carry0);

  // Here, 19 <= x2 < 2^255
  // - This is because we added 19, so before propagate it can't be less
  // than 19. After propagate, it still can't be less than 19, because if
  // propagate does anything it adds 19.
  // - We know that the high bit must be clear, because either the input was ~
  // 2^255 + one word + 19 (in which case it propagates to at most 2 words) or
  // it was < 2^255. So now, if we subtract 19, we will get back to something in
  // [0,2^255-19).
  carry = -19;
  res = 0;
  for (i = 0; i < NLIMBS; i++) {
    carry += x[i];
    res |= x[i] = (limb_t) carry;
    carry >>= X25519_WBITS;
  }
  return (limb_t) (((dlimb_t) res - 1) >> X25519_WBITS);
}

static const limb_t a24[1] = {121665};

static void ladder_part1(fe xs[5]) {
  limb_t *x2 = xs[0], *z2 = xs[1], *x3 = xs[2], *z3 = xs[3], *t1 = xs[4];
  add(t1, x2, z2);                                 // t1 = A
  sub(z2, x2, z2);                                 // z2 = B
  add(x2, x3, z3);                                 // x2 = C
  sub(z3, x3, z3);                                 // z3 = D
  mul1(z3, t1);                                    // z3 = DA
  mul1(x2, z2);                                    // x3 = BC
  add(x3, z3, x2);                                 // x3 = DA+CB
  sub(z3, z3, x2);                                 // z3 = DA-CB
  sqr1(t1);                                        // t1 = AA
  sqr1(z2);                                        // z2 = BB
  sub(x2, t1, z2);                                 // x2 = E = AA-BB
  mul(z2, x2, a24, sizeof(a24) / sizeof(a24[0]));  // z2 = E*a24
  add(z2, z2, t1);                                 // z2 = E*a24 + AA
}

static void ladder_part2(fe xs[5], const fe x1) {
  limb_t *x2 = xs[0], *z2 = xs[1], *x3 = xs[2], *z3 = xs[3], *t1 = xs[4];
  sqr1(z3);         // z3 = (DA-CB)^2
  mul1(z3, x1);     // z3 = x1 * (DA-CB)^2
  sqr1(x3);         // x3 = (DA+CB)^2
  mul1(z2, x2);     // z2 = AA*(E*a24+AA)
  sub(x2, t1, x2);  // x2 = BB again
  mul1(x2, t1);     // x2 = AA*BB
}

static void x25519_core(fe xs[5], const uint8_t scalar[X25519_BYTES],
                        const uint8_t *x1, int clamp) {
  int i;
  limb_t swap = 0;
  limb_t *x2 = xs[0], *x3 = xs[2], *z3 = xs[3];
  memset(xs, 0, 4 * sizeof(fe));
  x2[0] = z3[0] = 1;
  memcpy(x3, x1, sizeof(fe));

  for (i = 255; i >= 0; i--) {
    uint8_t bytei = scalar[i / 8];
    limb_t doswap;
    if (clamp) {
      if (i / 8 == 0) {
        bytei &= (uint8_t) ~7U;
      } else if (i / 8 == X25519_BYTES - 1) {
        bytei &= 0x7F;
        bytei |= 0x40;
      }
    }
    doswap = 0 - (limb_t) ((bytei >> (i % 8)) & 1);
    condswap(x2, x3, swap ^ doswap);
    swap = doswap;

    ladder_part1(xs);
    ladder_part2(xs, (const limb_t *) x1);
  }
  condswap(x2, x3, swap);
}

static int x25519(uint8_t out[X25519_BYTES], const uint8_t scalar[X25519_BYTES],
                  const uint8_t x1[X25519_BYTES], int clamp) {
  int i, ret;
  fe xs[5];
  limb_t *x2, *z2, *z3, *prev;
  static const struct {
    uint8_t a, c, n;
  } steps[13] = {{2, 1, 1},  {2, 1, 1},  {4, 2, 3},  {2, 4, 6},  {3, 1, 1},
                 {3, 2, 12}, {4, 3, 25}, {2, 3, 25}, {2, 4, 50}, {3, 2, 125},
                 {3, 1, 2},  {3, 1, 2},  {3, 1, 1}};
  x25519_core(xs, scalar, x1, clamp);

  // Precomputed inversion chain
  x2 = xs[0];
  z2 = xs[1];
  z3 = xs[3];

  prev = z2;
  for (i = 0; i < 13; i++) {
    int j;
    limb_t *a = xs[steps[i].a];
    for (j = steps[i].n; j > 0; j--) {
      sqr(a, prev);
      prev = a;
    }
    mul1(a, xs[steps[i].c]);
  }

  // Here prev = z3
  // x2 /= z2
  mul((limb_t *) out, x2, z3, NLIMBS);
  ret = (int) canon((limb_t *) out);
  if (!clamp) ret = 0;
  return ret;
}

// helper to hexdump buffers inline
static void mg_tls_hexdump(const char *msg, uint8_t *buf, size_t bufsz) {
  char p[512];
  MG_VERBOSE(("%s: %s", msg, mg_hex(buf, bufsz, p)));
}

// TLS1.3 secret derivation based on the key label
static void mg_tls_derive_secret(const char *label, uint8_t *key, size_t keysz,
                                 uint8_t *data, size_t datasz, uint8_t *hash,
                                 size_t hashsz) {
  size_t labelsz = strlen(label);
  uint8_t secret[32];
  uint8_t packed[256] = {0, (uint8_t) hashsz, (uint8_t) labelsz};
  // TODO: assert lengths of label, key, data and hash
  memmove(packed + 3, label, labelsz);
  packed[3 + labelsz] = (uint8_t) datasz;
  memmove(packed + labelsz + 4, data, datasz);
  packed[4 + labelsz + datasz] = 1;

  mg_hmac_sha256(secret, key, keysz, packed, 5 + labelsz + datasz);
  memmove(hash, secret, hashsz);
}

// Did we receive a full TLS message in the c->rtls buffer?
static bool mg_tls_got_msg(struct mg_connection *c) {
  return c->rtls.len >= (size_t) TLS_HDR_SIZE &&
         c->rtls.len >= (size_t) (TLS_HDR_SIZE + MG_LOAD_BE16(c->rtls.buf + 3));
}

// Remove a single TLS record from the recv buffer
static void mg_tls_drop_packet(struct mg_iobuf *rio) {
  uint16_t n = MG_LOAD_BE16(rio->buf + 3) + TLS_HDR_SIZE;
  mg_iobuf_del(rio, 0, n);
}

// read and parse ClientHello record
static int mg_tls_client_hello(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  struct mg_iobuf *rio = &c->rtls;
  uint8_t session_id_len;
  uint16_t j;
  uint16_t cipher_suites_len;
  uint16_t ext_len;
  uint8_t *ext;

  if (!mg_tls_got_msg(c)) {
    return MG_IO_WAIT;
  }
  if (rio->buf[0] != 0x16 || rio->buf[5] != 0x01) {
    mg_error(c, "not a hello packet");
    return -1;
  }
  mg_sha256_update(&tls->sha256, rio->buf + 5, rio->len - 5);
  session_id_len = rio->buf[43];
  if (session_id_len == sizeof(tls->session_id)) {
    memmove(tls->session_id, rio->buf + 44, session_id_len);
  } else if (session_id_len != 0) {
    MG_INFO(("bad session id len"));
  }
  cipher_suites_len = MG_LOAD_BE16(rio->buf + 44 + session_id_len);
  ext_len = MG_LOAD_BE16(rio->buf + 48 + session_id_len + cipher_suites_len);
  ext = rio->buf + 50 + session_id_len + cipher_suites_len;
  for (j = 0; j < ext_len;) {
    uint16_t k;
    uint16_t key_exchange_len;
    uint8_t *key_exchange;
    uint16_t n = MG_LOAD_BE16(ext + j + 2);
    if (ext[j] != 0x00 ||
        ext[j + 1] != 0x33) {  // not a key share extension, ignore
      j += (uint16_t) (n + 4);
      continue;
    }
    key_exchange_len = MG_LOAD_BE16(ext + j + 5);
    key_exchange = ext + j + 6;
    for (k = 0; k < key_exchange_len;) {
      uint16_t m = MG_LOAD_BE16(key_exchange + k + 2);
      if (m == 32 && key_exchange[k] == 0x00 && key_exchange[k + 1] == 0x1d) {
        memmove(tls->x25519_cli, key_exchange + k + 4, m);
        mg_tls_drop_packet(rio);
        return 0;
      }
      k += (uint16_t) (m + 4);
    }
    j += (uint16_t) (n + 4);
  }
  mg_error(c, "bad client hello");
  return -1;
}

// put ServerHello record into wio buffer
static void mg_tls_server_hello(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  struct mg_iobuf *wio = &tls->send;

  uint8_t msg_server_hello[122] =
      // server hello, tls 1.2
      "\x02\x00\x00\x76\x03\x03"
      // random (32 bytes)
      "\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe"
      "\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe"
      // session ID length + session ID (32 bytes)
      "\x20"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#if defined(CHACHA20) && CHACHA20
      // TLS_CHACHA20_POLY1305_SHA256 + no compression
      "\x13\x03\x00"
#else
      // TLS_AES_128_GCM_SHA256 + no compression
      "\x13\x01\x00"
#endif
      // extensions + keyshare
      "\x00\x2e\x00\x33\x00\x24\x00\x1d\x00\x20"
      // x25519 keyshare
      "\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab"
      "\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab\xab"
      // supported versions (tls1.3 == 0x304)
      "\x00\x2b\x00\x02\x03\x04";

  // calculate keyshare
  uint8_t x25519_pub[X25519_BYTES];
  uint8_t x25519_prv[X25519_BYTES];
  mg_random(x25519_prv, sizeof(x25519_prv));
  x25519(x25519_pub, x25519_prv, X25519_BASE_POINT, 1);
  x25519(tls->x25519_sec, x25519_prv, tls->x25519_cli, 1);
  mg_tls_hexdump("x25519 sec", tls->x25519_sec, sizeof(tls->x25519_sec));

  // fill in the gaps: session ID + keyshare
  memmove(msg_server_hello + 39, tls->session_id, sizeof(tls->session_id));
  memmove(msg_server_hello + 84, x25519_pub, sizeof(x25519_pub));

  // server hello message
  mg_iobuf_add(wio, wio->len, "\x16\x03\x03\x00\x7a", 5);
  mg_iobuf_add(wio, wio->len, msg_server_hello, sizeof(msg_server_hello));
  mg_sha256_update(&tls->sha256, msg_server_hello, sizeof(msg_server_hello));

  // change cipher message
  mg_iobuf_add(wio, wio->len, "\x14\x03\x03\x00\x01\x01", 6);
}

// at this point we have x25519 shared secret, we can generate a set of derived
// handshake encryption keys
static void mg_tls_generate_handshake_keys(struct mg_connection *c) {
  struct tls_data *tls = c->tls;

  mg_sha256_ctx sha256;
  uint8_t early_secret[32];
  uint8_t pre_extract_secret[32];
  uint8_t hello_hash[32];
  uint8_t server_hs_secret[32];
  uint8_t client_hs_secret[32];

  mg_hmac_sha256(early_secret, NULL, 0, zeros, sizeof(zeros));
  mg_tls_derive_secret("tls13 derived", early_secret, 32, zeros_sha256_digest,
                       32, pre_extract_secret, 32);
  mg_hmac_sha256(tls->handshake_secret, pre_extract_secret,
                 sizeof(pre_extract_secret), tls->x25519_sec,
                 sizeof(tls->x25519_sec));
  mg_tls_hexdump("hs secret", tls->handshake_secret, 32);

  // mg_sha256_final is not idempotent, need to copy sha256 context to calculate
  // the digest
  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(hello_hash, &sha256);

  // derive keys needed for the rest of the handshake
  mg_tls_derive_secret("tls13 s hs traffic", tls->handshake_secret, 32,
                       hello_hash, 32, server_hs_secret, 32);
  mg_tls_derive_secret("tls13 key", server_hs_secret, 32, NULL, 0,
                       tls->server_write_key, 16);
  mg_tls_derive_secret("tls13 iv", server_hs_secret, 32, NULL, 0,
                       tls->server_write_iv, 12);
  mg_tls_derive_secret("tls13 finished", server_hs_secret, 32, NULL, 0,
                       tls->server_finished_key, 32);
  mg_tls_hexdump("s hs traffic", server_hs_secret, 32);

  mg_tls_derive_secret("tls13 c hs traffic", tls->handshake_secret, 32,
                       hello_hash, 32, client_hs_secret, 32);
  mg_tls_derive_secret("tls13 key", client_hs_secret, 32, NULL, 0,
                       tls->client_write_key, 16);
  mg_tls_derive_secret("tls13 iv", client_hs_secret, 32, NULL, 0,
                       tls->client_write_iv, 12);
  mg_tls_derive_secret("tls13 finished", client_hs_secret, 32, NULL, 0,
                       tls->client_finished_key, 32);
}

// AES GCM encryption of the message + put encoded data into the write buffer
static void mg_tls_encrypt(struct mg_connection *c, const uint8_t *msg,
                           size_t msgsz, uint8_t msgtype) {
  struct tls_data *tls = c->tls;
  struct mg_iobuf *wio = &tls->send;
  uint8_t *outmsg;
  uint8_t *tag;
  size_t encsz = msgsz + 16 + 1;
  uint8_t hdr[5] = {0x17, 0x03, 0x03, (encsz >> 8) & 0xff, encsz & 0xff};
  uint8_t associated_data[5] = {0x17, 0x03, 0x03, (encsz >> 8) & 0xff,
                                encsz & 0xff};
  uint8_t nonce[12];
  memmove(nonce, tls->server_write_iv, sizeof(tls->server_write_iv));
  nonce[8] ^= (uint8_t) ((tls->sseq >> 24) & 255U);
  nonce[9] ^= (uint8_t) ((tls->sseq >> 16) & 255U);
  nonce[10] ^= (uint8_t) ((tls->sseq >> 8) & 255U);
  nonce[11] ^= (uint8_t) ((tls->sseq) & 255U);

  gcm_initialize();
  mg_iobuf_add(wio, wio->len, hdr, sizeof(hdr));
  mg_iobuf_resize(wio, wio->len + encsz);
  outmsg = wio->buf + wio->len;
  tag = wio->buf + wio->len + msgsz + 1;
  memmove(outmsg, msg, msgsz);
  outmsg[msgsz] = msgtype;
  aes_gcm_encrypt(outmsg, outmsg, msgsz + 1, tls->server_write_key,
                  sizeof(tls->server_write_key), nonce, sizeof(nonce),
                  associated_data, sizeof(associated_data), tag, 16);
  wio->len += encsz;
  tls->sseq++;
}

// read an encrypted message, decrypt it into read buffer (AES GCM)
static int mg_tls_recv_decrypt(struct mg_connection *c, void *buf,
                               size_t bufsz) {
  struct tls_data *tls = c->tls;
  struct mg_iobuf *rio = &c->rtls;
  // struct mg_iobuf *rio = &tls->recv;
  uint16_t msgsz;
  uint8_t *msg;
  uint8_t nonce[12];
  int r;
  for (;;) {
    if (!mg_tls_got_msg(c)) {
      return MG_IO_WAIT;
    }
    if (rio->buf[0] == 0x17) {
      break;
    } else if (rio->buf[0] == 0x15) {
      MG_INFO(("TLS ALERT packet received"));  // TODO: drop packet?
    } else {
      mg_error(c, "unexpected packet");
      return -1;
    }
  }
  msgsz = MG_LOAD_BE16(rio->buf + 3);
  msg = rio->buf + 5;
  memmove(nonce, tls->client_write_iv, sizeof(tls->client_write_iv));
  nonce[8] ^= (uint8_t) ((tls->cseq >> 24) & 255U);
  nonce[9] ^= (uint8_t) ((tls->cseq >> 16) & 255U);
  nonce[10] ^= (uint8_t) ((tls->cseq >> 8) & 255U);
  nonce[11] ^= (uint8_t) ((tls->cseq) & 255U);
  aes_gcm_decrypt(msg, msg, msgsz - 16, tls->client_write_key,
                  sizeof(tls->client_write_key), nonce, sizeof(nonce));
  r = msgsz - 16 - 1;
  if (msg[r] == 0x17) {
    if (bufsz > 0) {
      memmove(buf, msg, msgsz - 16);
    }
  } else {
    r = 0;
  }
  tls->cseq++;
  mg_tls_drop_packet(rio);
  return r;
}

static void mg_tls_server_extensions(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  // server extensions
  uint8_t ext[6] = {0x08, 0, 0, 2, 0, 0};
  mg_sha256_update(&tls->sha256, ext, sizeof(ext));
  mg_tls_encrypt(c, ext, sizeof(ext), 0x16);
}

static void mg_tls_server_cert(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  // server DER certificate (empty)
  size_t n = tls->server_cert_der.len;
  uint8_t *cert = calloc(1, 13 + n);             // FIXME: free
  cert[0] = 0x0b;                                // handshake header
  cert[1] = (uint8_t) (((n + 9) >> 16) & 255U);  // 3 bytes: payload length
  cert[2] = (uint8_t) (((n + 9) >> 8) & 255U);
  cert[3] = (uint8_t) ((n + 9) & 255U);
  cert[4] = 0;                                   // request context
  cert[5] = (uint8_t) (((n + 5) >> 16) & 255U);  // 3 bytes: cert (s) length
  cert[6] = (uint8_t) (((n + 5) >> 8) & 255U);
  cert[7] = (uint8_t) ((n + 5) & 255U);
  cert[8] =
      (uint8_t) (((n) >> 16) & 255U);  // 3 bytes: first (and only) cert len
  cert[9] = (uint8_t) (((n) >> 8) & 255U);
  cert[10] = (uint8_t) (n & 255U);
  // bytes 11+ are certificate in DER format
  memmove(cert + 11, tls->server_cert_der.ptr, n);
  cert[11 + n] = cert[12 + n] = 0;  // certificate extensions (none)
  mg_sha256_update(&tls->sha256, cert, 13 + n);
  mg_tls_encrypt(c, cert, 13 + n, 0x16);
}

// type adapter between uECC hash context and our sha256 implementation
typedef struct SHA256_HashContext {
  uECC_HashContext uECC;
  mg_sha256_ctx ctx;
} SHA256_HashContext;

static void init_SHA256(const uECC_HashContext *base) {
  SHA256_HashContext *c = (SHA256_HashContext *) base;
  mg_sha256_init(&c->ctx);
}

static void update_SHA256(const uECC_HashContext *base, const uint8_t *message,
                          unsigned message_size) {
  SHA256_HashContext *c = (SHA256_HashContext *) base;
  mg_sha256_update(&c->ctx, message, message_size);
}
static void finish_SHA256(const uECC_HashContext *base, uint8_t *hash_result) {
  SHA256_HashContext *c = (SHA256_HashContext *) base;
  mg_sha256_final(hash_result, &c->ctx);
}

static void mg_tls_server_verify_ecdsa(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  // server certificate verify packet
  uint8_t verify[82] = {0x0f, 0x00, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00};
  size_t sigsz, verifysz = 0;
  uint8_t hash[32] = {0}, tmp[2 * 32 + 64] = {0};
  struct SHA256_HashContext ctx = {
      {&init_SHA256, &update_SHA256, &finish_SHA256, 64, 32, tmp},
      {{0}, 0, 0, {0}}};
  int neg1, neg2;
  uint8_t sig[64], sig_content[130] = {
                       "                                "
                       "                                "
                       "TLS 1.3, server CertificateVerify\0"};
  mg_sha256_ctx sha256;
  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(sig_content + 98, &sha256);

  mg_sha256_init(&sha256);
  mg_sha256_update(&sha256, sig_content, sizeof(sig_content));
  mg_sha256_final(hash, &sha256);

  uECC_sign_deterministic(tls->server_key, hash, sizeof(hash), &ctx.uECC, sig,
                          uECC_secp256r1());

  neg1 = !!(sig[0] & 0x80);
  neg2 = !!(sig[32] & 0x80);
  verify[8] = 0x30;  // ASN.1 SEQUENCE
  verify[9] = (uint8_t) (68 + neg1 + neg2);
  verify[10] = 0x02;  // ASN.1 INTEGER
  verify[11] = (uint8_t) (32 + neg1);
  memmove(verify + 12 + neg1, sig, 32);
  verify[12 + 32 + neg1] = 0x02;  // ASN.1 INTEGER
  verify[13 + 32 + neg1] = (uint8_t) (32 + neg2);
  memmove(verify + 14 + 32 + neg1 + neg2, sig + 32, 32);

  sigsz = (size_t) (70 + neg1 + neg2);
  verifysz = 8U + sigsz;
  verify[3] = (uint8_t) (sigsz + 4);
  verify[7] = (uint8_t) sigsz;

  mg_tls_hexdump("verify", verify, verifysz);

  mg_sha256_update(&tls->sha256, verify, verifysz);
  mg_tls_encrypt(c, verify, verifysz, 0x16);
}

static void mg_tls_server_finish(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  struct mg_iobuf *wio = &tls->send;
  mg_sha256_ctx sha256;
  uint8_t hash[32];
  uint8_t finish[36] = {0x14, 0, 0, 32};
  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(hash, &sha256);
  mg_hmac_sha256(finish + 4, tls->server_finished_key, 32, hash, 32);
  mg_tls_hexdump("hash", hash, sizeof(hash));
  mg_tls_hexdump("key", tls->server_finished_key,
                 sizeof(tls->server_finished_key));
  mg_tls_encrypt(c, finish, sizeof(finish), 0x16);
  mg_io_send(c, wio->buf, wio->len);
  wio->len = 0;

  mg_sha256_update(&tls->sha256, finish, sizeof(finish));
}

static int mg_tls_client_change_cipher(struct mg_connection *c) {
  // struct tls_data *tls = c->tls;
  struct mg_iobuf *rio = &c->rtls;
  for (;;) {
    if (!mg_tls_got_msg(c)) {
      return MG_IO_WAIT;
    }
    if (rio->buf[0] == 0x14) {  // got a ChangeCipher record
      break;
    } else if (rio->buf[0] == 0x15) {  // skip Alert records
      MG_DEBUG(("TLS ALERT packet received"));
      mg_tls_drop_packet(rio);
    } else {
      mg_error(c, "unexpected packet");
      return MG_IO_ERR;
    }
  }
  // consume ChangeCipher packet
  mg_tls_drop_packet(rio);
  return 0;
}

static int mg_tls_client_finish(struct mg_connection *c) {
  uint8_t tmp[2048];
  int n = mg_tls_recv_decrypt(c, tmp, sizeof(tmp));
  if (n < 0) {
    return -1;
  }
  // TODO: make sure it's a ClientFinish record
  return 0;
}

static void mg_tls_generate_application_keys(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  uint8_t hash[32];
  uint8_t premaster_secret[32];
  uint8_t master_secret[32];
  uint8_t server_secret[32];
  uint8_t client_secret[32];

  mg_sha256_ctx sha256;
  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(hash, &sha256);

  mg_tls_derive_secret("tls13 derived", tls->handshake_secret, 32,
                       zeros_sha256_digest, 32, premaster_secret, 32);
  mg_hmac_sha256(master_secret, premaster_secret, 32, zeros, 32);

  mg_tls_derive_secret("tls13 s ap traffic", master_secret, 32, hash, 32,
                       server_secret, 32);
  mg_tls_derive_secret("tls13 key", server_secret, 32, NULL, 0,
                       tls->server_write_key, 16);
  mg_tls_derive_secret("tls13 iv", server_secret, 32, NULL, 0,
                       tls->server_write_iv, 12);
  mg_tls_derive_secret("tls13 c ap traffic", master_secret, 32, hash, 32,
                       client_secret, 32);
  mg_tls_derive_secret("tls13 key", client_secret, 32, NULL, 0,
                       tls->client_write_key, 16);
  mg_tls_derive_secret("tls13 iv", client_secret, 32, NULL, 0,
                       tls->client_write_iv, 12);

  tls->sseq = tls->cseq = 0;
}

void mg_tls_handshake(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  switch (tls->state) {
    case MG_TLS_HS_CLIENT_HELLO:
      if (mg_tls_client_hello(c) < 0) {
        return;
      }
      tls->state = MG_TLS_HS_SERVER_HELLO;
      // fallthrough
    case MG_TLS_HS_SERVER_HELLO:
      mg_tls_server_hello(c);
      mg_tls_generate_handshake_keys(c);
      mg_tls_server_extensions(c);
      mg_tls_server_cert(c);
      mg_tls_server_verify_ecdsa(c);
      mg_tls_server_finish(c);
      tls->state = MG_TLS_HS_CLIENT_CHANGE_CIPHER;
      // fallthrough
    case MG_TLS_HS_CLIENT_CHANGE_CIPHER:
      if (mg_tls_client_change_cipher(c) < 0) {
        return;
      }
      tls->state = MG_TLS_HS_CLIENT_FINISH;
      // fallthrough
    case MG_TLS_HS_CLIENT_FINISH:
      if (mg_tls_client_finish(c) < 0) {
        return;
      }
      mg_tls_generate_application_keys(c);
      tls->state = MG_TLS_HS_DONE;
      // fallthrough
    case MG_TLS_HS_DONE: c->is_tls_hs = 0; return;
  }
}

static int mg_parse_pem(const struct mg_str pem, const struct mg_str label,
                        struct mg_str *der) {
  size_t n = 0, m = 0;
  char *s;
  const char *c;
  struct mg_str caps[5];
  if (!mg_match(pem, mg_str("#-----BEGIN #-----#-----END #-----#"), caps)) {
    *der = mg_strdup(pem);
    return 0;
  }
  if (mg_strcmp(caps[1], label) != 0 || mg_strcmp(caps[3], label) != 0) {
    return -1;  // bad label
  }
  if ((s = calloc(1, caps[2].len)) == NULL) {
    return -1;
  }

  for (c = caps[2].ptr; c < caps[2].ptr + caps[2].len; c++) {
    if (*c == ' ' || *c == '\n' || *c == '\r' || *c == '\t') {
      continue;
    }
    s[n++] = *c;
  }
  m = mg_base64_decode(s, n, s, n);
  if (m == 0) {
    free(s);
    return -1;
  }
  der->ptr = s;
  der->len = m;
  return 0;
}

void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  struct mg_str key;
  struct tls_data *tls = (struct tls_data *) calloc(1, sizeof(struct tls_data));
  if (tls == NULL) {
    mg_error(c, "tls oom");
    return;
  }
  // parse PEM or DER EC key
  if (opts->key.ptr == NULL ||
      mg_parse_pem(opts->key, mg_str_s("EC PRIVATE KEY"), &key) < 0) {
    MG_ERROR(("Failed to load EC private key"));
    return;
  }
  if (key.len < 39) {
    MG_ERROR(("EC private key too short"));
    return;
  }
  // expect ASN.1 SEQUENCE=[INTEGER=1, BITSTRING of 32 bytes, ...]
  // 30 nn 02 01 01 04 20 [key] ...
  if (key.ptr[0] != 0x30 || (key.ptr[1] & 0x80) != 0) {
    MG_ERROR(("EC private key: ASN.1 bad sequence"));
    return;
  }
  if (memcmp(key.ptr + 2, "\x02\x01\x01\x04\x20", 5) != 0) {
    MG_ERROR(("EC private key: ASN.1 bad data"));
  }
  memmove(tls->server_key, key.ptr + 7, 32);
  free((void *) key.ptr);

  // parse PEM or DER certificate
  if (mg_parse_pem(opts->cert, mg_str_s("CERTIFICATE"), &tls->server_cert_der) <
      0) {
    MG_ERROR(("Failed to load certificate"));
    return;
  }

  // tls->send.align = tls->recv.align = MG_IO_SIZE;
  tls->send.align = MG_IO_SIZE;
  c->tls = tls;
  c->is_tls = c->is_tls_hs = 1;
  mg_sha256_init(&tls->sha256);
}

void mg_tls_free(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  if (tls != NULL) {
    mg_iobuf_free(&tls->send);
    free((void *) tls->server_cert_der.ptr);
  }
  free(c->tls);
  c->tls = NULL;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct tls_data *tls = c->tls;
  long n = MG_IO_WAIT;
  if (len > MG_IO_SIZE) len = MG_IO_SIZE;
  mg_tls_encrypt(c, buf, len, 0x17);
  while (tls->send.len > 0 &&
         (n = mg_io_send(c, tls->send.buf, tls->send.len)) > 0) {
    mg_iobuf_del(&tls->send, 0, (size_t) n);
  }
  if (n == MG_IO_ERR || n == MG_IO_WAIT) return n;
  return (long) len;
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  return mg_tls_recv_decrypt(c, buf, len);
}

size_t mg_tls_pending(struct mg_connection *c) {
  return mg_tls_got_msg(c) ? 1 : 0;
}

void mg_tls_ctx_init(struct mg_mgr *mgr) {
  (void) mgr;
}

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  (void) mgr;
}
#endif
