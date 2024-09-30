#include "base64.h"
#include "config.h"
#include "printf.h"
#include "sha256.h"
#include "tls.h"
#include "tls_aes128.h"
#include "tls_chacha20.h"
#include "tls_uecc.h"
#include "tls_x25519.h"
#include "util.h"

#if MG_TLS == MG_TLS_BUILTIN

#define CHACHA20 1

/* TLS 1.3 Record Content Type (RFC8446 B.1) */
#define MG_TLS_CHANGE_CIPHER 20
#define MG_TLS_ALERT 21
#define MG_TLS_HANDSHAKE 22
#define MG_TLS_APP_DATA 23
#define MG_TLS_HEARTBEAT 24

/* TLS 1.3 Handshake Message Type (RFC8446 B.3) */
#define MG_TLS_CLIENT_HELLO 1
#define MG_TLS_SERVER_HELLO 2
#define MG_TLS_ENCRYPTED_EXTENSIONS 8
#define MG_TLS_CERTIFICATE 11
#define MG_TLS_CERTIFICATE_REQUEST 13
#define MG_TLS_CERTIFICATE_VERIFY 15
#define MG_TLS_FINISHED 20

// handshake is re-entrant, so we need to keep track of its state state names
// refer to RFC8446#A.1
enum mg_tls_hs_state {
  // Client state machine:
  MG_TLS_STATE_CLIENT_START,          // Send ClientHello
  MG_TLS_STATE_CLIENT_WAIT_SH,        // Wait for ServerHello
  MG_TLS_STATE_CLIENT_WAIT_EE,        // Wait for EncryptedExtensions
  MG_TLS_STATE_CLIENT_WAIT_CERT,      // Wait for Certificate
  MG_TLS_STATE_CLIENT_WAIT_CV,        // Wait for CertificateVerify
  MG_TLS_STATE_CLIENT_WAIT_FINISHED,  // Wait for Finished
  MG_TLS_STATE_CLIENT_CONNECTED,      // Done

  // Server state machine:
  MG_TLS_STATE_SERVER_START,       // Wait for ClientHello
  MG_TLS_STATE_SERVER_NEGOTIATED,  // Wait for Finished
  MG_TLS_STATE_SERVER_CONNECTED    // Done
};

// encryption keys for a TLS connection
struct tls_enc {
  uint32_t sseq;  // server sequence number, used in encryption
  uint32_t cseq;  // client sequence number, used in decryption
  // keys for AES encryption or ChaCha20
  uint8_t handshake_secret[32];
  uint8_t server_write_key[32];
  uint8_t server_write_iv[12];
  uint8_t server_finished_key[32];
  uint8_t client_write_key[32];
  uint8_t client_write_iv[12];
  uint8_t client_finished_key[32];
};

// per-connection TLS data
struct tls_data {
  enum mg_tls_hs_state state;  // keep track of connection handshake progress

  struct mg_iobuf send;  // For the receive path, we're reusing c->rtls
  size_t recv_offset;    // While c->rtls contains full records, reuse that
  size_t recv_len;       // buffer but point at individual decrypted messages

  uint8_t content_type;  // Last received record content type

  mg_sha256_ctx sha256;  // incremental SHA-256 hash for TLS handshake

  uint8_t random[32];      // client random from ClientHello
  uint8_t session_id[32];  // client session ID between the handshake states
  uint8_t x25519_cli[32];  // client X25519 key between the handshake states
  uint8_t x25519_sec[32];  // x25519 secret between the handshake states

  int skip_verification;   // perform checks on server certificate?
  int cert_requested;      // client received a CertificateRequest?
  struct mg_str cert_der;  // certificate in DER format
  uint8_t ec_key[32];      // EC private key
  char hostname[254];      // server hostname (client extension)

  uint8_t certhash[32];  // certificate message hash
  uint8_t pubkey[64];    // server EC public key to verify cert
  uint8_t sighash[32];   // server EC public key to verify cert

  struct tls_enc enc;
};

#define TLS_RECHDR_SIZE 5  // 1 byte type, 2 bytes version, 2 bytes length
#define TLS_MSGHDR_SIZE 4  // 1 byte type, 3 bytes length

#ifdef MG_TLS_SSLKEYLOGFILE
#include <stdio.h>
static void mg_ssl_key_log(const char *label, uint8_t client_random[32],
                           uint8_t *secret, size_t secretsz) {
  char *keylogfile = getenv("SSLKEYLOGFILE");
  size_t i;
  if (keylogfile != NULL) {
    MG_DEBUG(("Dumping key log into %s", keylogfile));
    FILE *f = fopen(keylogfile, "a");
    if (f != NULL) {
      fprintf(f, "%s ", label);
      for (i = 0; i < 32; i++) {
        fprintf(f, "%02x", client_random[i]);
      }
      fprintf(f, " ");
      for (i = 0; i < secretsz; i++) {
        fprintf(f, "%02x", secret[i]);
      }
      fprintf(f, "\n");
      fclose(f);
    } else {
      MG_ERROR(("Cannot open %s", keylogfile));
    }
  }
}
#endif

// for derived tls keys we need SHA256([0]*32)
static uint8_t zeros[32] = {0};
static uint8_t zeros_sha256_digest[32] = {
    0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4,
    0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b,
    0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55};

// helper to hexdump buffers inline
static void mg_tls_hexdump(const char *msg, uint8_t *buf, size_t bufsz) {
  MG_VERBOSE(("%s: %M", msg, mg_print_hex, bufsz, buf));
}

// helper utilities to parse ASN.1 DER
struct mg_der_tlv {
  uint8_t type;
  uint32_t len;
  uint8_t *value;
};

// parse DER into a TLV record
static int mg_der_to_tlv(uint8_t *der, size_t dersz, struct mg_der_tlv *tlv) {
  if (dersz < 2) {
    return -1;
  }
  tlv->type = der[0];
  tlv->len = der[1];
  tlv->value = der + 2;
  if (tlv->len > 0x7f) {
    uint32_t i, n = tlv->len - 0x80;
    tlv->len = 0;
    for (i = 0; i < n; i++) {
      tlv->len = (tlv->len << 8) | (der[2 + i]);
    }
    tlv->value = der + 2 + n;
  }
  if (der + dersz < tlv->value + tlv->len) {
    return -1;
  }
  return 0;
}

static int mg_der_find(uint8_t *der, size_t dersz, uint8_t *oid, size_t oidsz,
                       struct mg_der_tlv *tlv) {
  uint8_t *p, *end;
  struct mg_der_tlv child = {0, 0, NULL};
  if (mg_der_to_tlv(der, dersz, tlv) < 0) {
    return -1;                  // invalid DER
  } else if (tlv->type == 6) {  // found OID, check value
    return (tlv->len == oidsz && memcmp(tlv->value, oid, oidsz) == 0);
  } else if ((tlv->type & 0x20) == 0) {
    return 0;  // Primitive, but not OID: not found
  }
  // Constructed object: scan children
  p = tlv->value;
  end = tlv->value + tlv->len;
  while (end > p) {
    int r;
    mg_der_to_tlv(p, (size_t) (end - p), &child);
    r = mg_der_find(p, (size_t) (end - p), oid, oidsz, tlv);
    if (r < 0) return -1;  // error
    if (r > 0) return 1;   // found OID!
    p = child.value + child.len;
  }
  return 0;  // not found
}

// Did we receive a full TLS record in the c->rtls buffer?
static bool mg_tls_got_record(struct mg_connection *c) {
  return c->rtls.len >= (size_t) TLS_RECHDR_SIZE &&
         c->rtls.len >=
             (size_t) (TLS_RECHDR_SIZE + MG_LOAD_BE16(c->rtls.buf + 3));
}

// Remove a single TLS record from the recv buffer
static void mg_tls_drop_record(struct mg_connection *c) {
  struct mg_iobuf *rio = &c->rtls;
  uint16_t n = MG_LOAD_BE16(rio->buf + 3) + TLS_RECHDR_SIZE;
  mg_iobuf_del(rio, 0, n);
}

// Remove a single TLS message from decrypted buffer, remove the wrapping
// record if it was the last message within a record
static void mg_tls_drop_message(struct mg_connection *c) {
  uint32_t len;
  struct tls_data *tls = (struct tls_data *) c->tls;
  unsigned char *recv_buf = &c->rtls.buf[tls->recv_offset];
  if (tls->recv_len == 0) return;
  len = MG_LOAD_BE24(recv_buf + 1) + TLS_MSGHDR_SIZE;
  if (tls->recv_len < len) {
    mg_error(c, "wrong size");
    return;
  }
  mg_sha256_update(&tls->sha256, recv_buf, len);
  tls->recv_offset += len;
  tls->recv_len -= len;
  if (tls->recv_len == 0) {
    mg_tls_drop_record(c);
  }
}

// TLS1.3 secret derivation based on the key label
static void mg_tls_derive_secret(const char *label, uint8_t *key, size_t keysz,
                                 uint8_t *data, size_t datasz, uint8_t *hash,
                                 size_t hashsz) {
  size_t labelsz = strlen(label);
  uint8_t secret[32];
  uint8_t packed[256] = {0, (uint8_t) hashsz, (uint8_t) labelsz};
  // TODO: assert lengths of label, key, data and hash
  if (labelsz > 0) memmove(packed + 3, label, labelsz);
  packed[3 + labelsz] = (uint8_t) datasz;
  if (datasz > 0) memmove(packed + labelsz + 4, data, datasz);
  packed[4 + labelsz + datasz] = 1;

  mg_hmac_sha256(secret, key, keysz, packed, 5 + labelsz + datasz);
  memmove(hash, secret, hashsz);
}

// at this point we have x25519 shared secret, we can generate a set of derived
// handshake encryption keys
static void mg_tls_generate_handshake_keys(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;

  mg_sha256_ctx sha256;
  uint8_t early_secret[32];
  uint8_t pre_extract_secret[32];
  uint8_t hello_hash[32];
  uint8_t server_hs_secret[32];
  uint8_t client_hs_secret[32];
#if CHACHA20
  const size_t keysz = 32;
#else
  const size_t keysz = 16;
#endif

  mg_hmac_sha256(early_secret, NULL, 0, zeros, sizeof(zeros));
  mg_tls_derive_secret("tls13 derived", early_secret, 32, zeros_sha256_digest,
                       32, pre_extract_secret, 32);
  mg_hmac_sha256(tls->enc.handshake_secret, pre_extract_secret,
                 sizeof(pre_extract_secret), tls->x25519_sec,
                 sizeof(tls->x25519_sec));
  mg_tls_hexdump("hs secret", tls->enc.handshake_secret, 32);

  // mg_sha256_final is not idempotent, need to copy sha256 context to calculate
  // the digest
  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(hello_hash, &sha256);

  mg_tls_hexdump("hello hash", hello_hash, 32);
  // derive keys needed for the rest of the handshake
  mg_tls_derive_secret("tls13 s hs traffic", tls->enc.handshake_secret, 32,
                       hello_hash, 32, server_hs_secret, 32);
  mg_tls_derive_secret("tls13 c hs traffic", tls->enc.handshake_secret, 32,
                       hello_hash, 32, client_hs_secret, 32);

  mg_tls_derive_secret("tls13 key", server_hs_secret, 32, NULL, 0,
                       tls->enc.server_write_key, keysz);
  mg_tls_derive_secret("tls13 iv", server_hs_secret, 32, NULL, 0,
                       tls->enc.server_write_iv, 12);
  mg_tls_derive_secret("tls13 finished", server_hs_secret, 32, NULL, 0,
                       tls->enc.server_finished_key, 32);

  mg_tls_derive_secret("tls13 key", client_hs_secret, 32, NULL, 0,
                       tls->enc.client_write_key, keysz);
  mg_tls_derive_secret("tls13 iv", client_hs_secret, 32, NULL, 0,
                       tls->enc.client_write_iv, 12);
  mg_tls_derive_secret("tls13 finished", client_hs_secret, 32, NULL, 0,
                       tls->enc.client_finished_key, 32);

  mg_tls_hexdump("s hs traffic", server_hs_secret, 32);
  mg_tls_hexdump("s key", tls->enc.server_write_key, keysz);
  mg_tls_hexdump("s iv", tls->enc.server_write_iv, 12);
  mg_tls_hexdump("s finished", tls->enc.server_finished_key, 32);
  mg_tls_hexdump("c hs traffic", client_hs_secret, 32);
  mg_tls_hexdump("c key", tls->enc.client_write_key, keysz);
  mg_tls_hexdump("c iv", tls->enc.client_write_iv, 12);
  mg_tls_hexdump("c finished", tls->enc.client_finished_key, 32);

#ifdef MG_TLS_SSLKEYLOGFILE
  mg_ssl_key_log("SERVER_HANDSHAKE_TRAFFIC_SECRET", tls->random,
                 server_hs_secret, 32);
  mg_ssl_key_log("CLIENT_HANDSHAKE_TRAFFIC_SECRET", tls->random,
                 client_hs_secret, 32);
#endif
}

static void mg_tls_generate_application_keys(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  uint8_t hash[32];
  uint8_t premaster_secret[32];
  uint8_t master_secret[32];
  uint8_t server_secret[32];
  uint8_t client_secret[32];
#if CHACHA20
  const size_t keysz = 32;
#else
  const size_t keysz = 16;
#endif

  mg_sha256_ctx sha256;
  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(hash, &sha256);

  mg_tls_derive_secret("tls13 derived", tls->enc.handshake_secret, 32,
                       zeros_sha256_digest, 32, premaster_secret, 32);
  mg_hmac_sha256(master_secret, premaster_secret, 32, zeros, 32);

  mg_tls_derive_secret("tls13 s ap traffic", master_secret, 32, hash, 32,
                       server_secret, 32);
  mg_tls_derive_secret("tls13 key", server_secret, 32, NULL, 0,
                       tls->enc.server_write_key, keysz);
  mg_tls_derive_secret("tls13 iv", server_secret, 32, NULL, 0,
                       tls->enc.server_write_iv, 12);
  mg_tls_derive_secret("tls13 c ap traffic", master_secret, 32, hash, 32,
                       client_secret, 32);
  mg_tls_derive_secret("tls13 key", client_secret, 32, NULL, 0,
                       tls->enc.client_write_key, keysz);
  mg_tls_derive_secret("tls13 iv", client_secret, 32, NULL, 0,
                       tls->enc.client_write_iv, 12);

  mg_tls_hexdump("s ap traffic", server_secret, 32);
  mg_tls_hexdump("s key", tls->enc.server_write_key, keysz);
  mg_tls_hexdump("s iv", tls->enc.server_write_iv, 12);
  mg_tls_hexdump("s finished", tls->enc.server_finished_key, 32);
  mg_tls_hexdump("c ap traffic", client_secret, 32);
  mg_tls_hexdump("c key", tls->enc.client_write_key, keysz);
  mg_tls_hexdump("c iv", tls->enc.client_write_iv, 12);
  mg_tls_hexdump("c finished", tls->enc.client_finished_key, 32);
  tls->enc.sseq = tls->enc.cseq = 0;

#ifdef MG_TLS_SSLKEYLOGFILE
  mg_ssl_key_log("SERVER_TRAFFIC_SECRET_0", tls->random, server_secret, 32);
  mg_ssl_key_log("CLIENT_TRAFFIC_SECRET_0", tls->random, client_secret, 32);
#endif
}

// AES GCM encryption of the message + put encoded data into the write buffer
static void mg_tls_encrypt(struct mg_connection *c, const uint8_t *msg,
                           size_t msgsz, uint8_t msgtype) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  struct mg_iobuf *wio = &tls->send;
  uint8_t *outmsg;
  uint8_t *tag;
  size_t encsz = msgsz + 16 + 1;
  uint8_t hdr[5] = {MG_TLS_APP_DATA, 0x03, 0x03,
                    (uint8_t) ((encsz >> 8) & 0xff), (uint8_t) (encsz & 0xff)};
  uint8_t associated_data[5] = {MG_TLS_APP_DATA, 0x03, 0x03,
                                (uint8_t) ((encsz >> 8) & 0xff),
                                (uint8_t) (encsz & 0xff)};
  uint8_t nonce[12];

  uint32_t seq = c->is_client ? tls->enc.cseq : tls->enc.sseq;
  uint8_t *key =
      c->is_client ? tls->enc.client_write_key : tls->enc.server_write_key;
  uint8_t *iv =
      c->is_client ? tls->enc.client_write_iv : tls->enc.server_write_iv;

#if !CHACHA20
  mg_gcm_initialize();
#endif

  memmove(nonce, iv, sizeof(nonce));
  nonce[8] ^= (uint8_t) ((seq >> 24) & 255U);
  nonce[9] ^= (uint8_t) ((seq >> 16) & 255U);
  nonce[10] ^= (uint8_t) ((seq >> 8) & 255U);
  nonce[11] ^= (uint8_t) ((seq) &255U);

  mg_iobuf_add(wio, wio->len, hdr, sizeof(hdr));
  mg_iobuf_resize(wio, wio->len + encsz);
  outmsg = wio->buf + wio->len;
  tag = wio->buf + wio->len + msgsz + 1;
  memmove(outmsg, msg, msgsz);
  outmsg[msgsz] = msgtype;
#if CHACHA20
  (void) tag;  // tag is only used in aes gcm
  {
    size_t maxlen = MG_IO_SIZE > 16384 ? 16384 : MG_IO_SIZE;
    uint8_t *enc = (uint8_t *) calloc(1, maxlen + 256 + 1);
    if (enc == NULL) {
      mg_error(c, "TLS OOM");
      return;
    } else {
      size_t n = mg_chacha20_poly1305_encrypt(enc, key, nonce, associated_data,
                                              sizeof(associated_data), outmsg,
                                              msgsz + 1);
      memmove(outmsg, enc, n);
      free(enc);
    }
  }
#else
  mg_aes_gcm_encrypt(outmsg, outmsg, msgsz + 1, key, 16, nonce, sizeof(nonce),
                     associated_data, sizeof(associated_data), tag, 16);
#endif
  c->is_client ? tls->enc.cseq++ : tls->enc.sseq++;
  wio->len += encsz;
}

// read an encrypted record, decrypt it in place
static int mg_tls_recv_record(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  struct mg_iobuf *rio = &c->rtls;
  uint16_t msgsz;
  uint8_t *msg;
  uint8_t nonce[12];
  int r;

  uint32_t seq = c->is_client ? tls->enc.sseq : tls->enc.cseq;
  uint8_t *key =
      c->is_client ? tls->enc.server_write_key : tls->enc.client_write_key;
  uint8_t *iv =
      c->is_client ? tls->enc.server_write_iv : tls->enc.client_write_iv;

  if (tls->recv_len > 0) {
    return 0; /* some data from previous record is still present */
  }
  for (;;) {
    if (!mg_tls_got_record(c)) {
      return MG_IO_WAIT;
    }
    if (rio->buf[0] == MG_TLS_APP_DATA) {
      break;
    } else if (rio->buf[0] ==
               MG_TLS_CHANGE_CIPHER) {  // Skip ChangeCipher messages
      mg_tls_drop_record(c);
    } else if (rio->buf[0] == MG_TLS_ALERT) {  // Skip Alerts
      MG_INFO(("TLS ALERT packet received"));
      mg_tls_drop_record(c);
    } else {
      mg_error(c, "unexpected packet");
      return -1;
    }
  }

#if !CHACHA20
  mg_gcm_initialize();
#endif

  msgsz = MG_LOAD_BE16(rio->buf + 3);
  msg = rio->buf + 5;
  memmove(nonce, iv, sizeof(nonce));
  nonce[8] ^= (uint8_t) ((seq >> 24) & 255U);
  nonce[9] ^= (uint8_t) ((seq >> 16) & 255U);
  nonce[10] ^= (uint8_t) ((seq >> 8) & 255U);
  nonce[11] ^= (uint8_t) ((seq) &255U);
#if CHACHA20
  {
    uint8_t *dec = (uint8_t *) calloc(1, msgsz);
    size_t n;
    if (dec == NULL) {
      mg_error(c, "TLS OOM");
      return -1;
    }
    n = mg_chacha20_poly1305_decrypt(dec, key, nonce, msg, msgsz);
    memmove(msg, dec, n);
    free(dec);
  }
#else
  if (msgsz < 16) {
    mg_error(c, "wrong size");
    return -1;
  }
  mg_aes_gcm_decrypt(msg, msg, msgsz - 16, key, 16, nonce, sizeof(nonce));
#endif
  r = msgsz - 16 - 1;
  tls->content_type = msg[msgsz - 16 - 1];
  tls->recv_offset = (size_t) msg - (size_t) rio->buf;
  tls->recv_len = msgsz - 16 - 1;
  c->is_client ? tls->enc.sseq++ : tls->enc.cseq++;
  return r;
}

static void mg_tls_calc_cert_verify_hash(struct mg_connection *c,
                                         uint8_t hash[32], int is_client) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  uint8_t server_context[34] = "TLS 1.3, server CertificateVerify";
  uint8_t client_context[34] = "TLS 1.3, client CertificateVerify";
  uint8_t sig_content[130];
  mg_sha256_ctx sha256;

  memset(sig_content, 0x20, 64);
  if (is_client) {
    memmove(sig_content + 64, client_context, sizeof(client_context));
  } else {
    memmove(sig_content + 64, server_context, sizeof(server_context));
  }

  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(sig_content + 98, &sha256);

  mg_sha256_init(&sha256);
  mg_sha256_update(&sha256, sig_content, sizeof(sig_content));
  mg_sha256_final(hash, &sha256);
}

// read and parse ClientHello record
static int mg_tls_server_recv_hello(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  struct mg_iobuf *rio = &c->rtls;
  uint8_t session_id_len;
  uint16_t j;
  uint16_t cipher_suites_len;
  uint16_t ext_len;
  uint8_t *ext;
  uint16_t msgsz;

  if (!mg_tls_got_record(c)) {
    return MG_IO_WAIT;
  }
  if (rio->buf[0] != MG_TLS_HANDSHAKE || rio->buf[5] != MG_TLS_CLIENT_HELLO) {
    mg_error(c, "not a client hello packet");
    return -1;
  }
  msgsz = MG_LOAD_BE16(rio->buf + 3);
  mg_sha256_update(&tls->sha256, rio->buf + 5, msgsz);
  // store client random
  memmove(tls->random, rio->buf + 11, sizeof(tls->random));
  // store session_id
  session_id_len = rio->buf[43];
  if (session_id_len == sizeof(tls->session_id)) {
    memmove(tls->session_id, rio->buf + 44, session_id_len);
  } else if (session_id_len != 0) {
    MG_INFO(("bad session id len"));
  }
  cipher_suites_len = MG_LOAD_BE16(rio->buf + 44 + session_id_len);
  if (cipher_suites_len > (rio->len - 46 - session_id_len)) goto fail;
  ext_len = MG_LOAD_BE16(rio->buf + 48 + session_id_len + cipher_suites_len);
  ext = rio->buf + 50 + session_id_len + cipher_suites_len;
  if (ext_len > (rio->len - 50 - session_id_len - cipher_suites_len)) goto fail;
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
    key_exchange_len = MG_LOAD_BE16(ext + j + 4);
    key_exchange = ext + j + 6;
    if (key_exchange_len >
        rio->len - (uint16_t) ((size_t) key_exchange - (size_t) rio->buf) - 2)
      goto fail;
    for (k = 0; k < key_exchange_len;) {
      uint16_t m = MG_LOAD_BE16(key_exchange + k + 2);
      if (m > (key_exchange_len - k - 4)) goto fail;
      if (m == 32 && key_exchange[k] == 0x00 && key_exchange[k + 1] == 0x1d) {
        memmove(tls->x25519_cli, key_exchange + k + 4, m);
        mg_tls_drop_record(c);
        return 0;
      }
      k += (uint16_t) (m + 4);
    }
    j += (uint16_t) (n + 4);
  }
fail:
  mg_error(c, "bad client hello");
  return -1;
}

#define PLACEHOLDER_8B 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'
#define PLACEHOLDER_16B PLACEHOLDER_8B, PLACEHOLDER_8B
#define PLACEHOLDER_32B PLACEHOLDER_16B, PLACEHOLDER_16B

// put ServerHello record into wio buffer
static void mg_tls_server_send_hello(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  struct mg_iobuf *wio = &tls->send;

  // clang-format off
  uint8_t msg_server_hello[122] = {
      // server hello, tls 1.2
      0x02, 0x00, 0x00, 0x76, 0x03, 0x03,
      // random (32 bytes)
      PLACEHOLDER_32B,
      // session ID length + session ID (32 bytes)
      0x20, PLACEHOLDER_32B,
#if defined(CHACHA20) && CHACHA20
      // TLS_CHACHA20_POLY1305_SHA256 + no compression
      0x13, 0x03, 0x00,
#else
      // TLS_AES_128_GCM_SHA256 + no compression
      0x13, 0x01, 0x00,
#endif
      // extensions + keyshare
      0x00, 0x2e, 0x00, 0x33, 0x00, 0x24, 0x00, 0x1d, 0x00, 0x20,
      // x25519 keyshare
      PLACEHOLDER_32B,
      // supported versions (tls1.3 == 0x304)
      0x00, 0x2b, 0x00, 0x02, 0x03, 0x04};
  // clang-format on

  // calculate keyshare
  uint8_t x25519_pub[X25519_BYTES];
  uint8_t x25519_prv[X25519_BYTES];
  if (!mg_random(x25519_prv, sizeof(x25519_prv))) mg_error(c, "RNG"); 
  mg_tls_x25519(x25519_pub, x25519_prv, X25519_BASE_POINT, 1);
  mg_tls_x25519(tls->x25519_sec, x25519_prv, tls->x25519_cli, 1);
  mg_tls_hexdump("s x25519 sec", tls->x25519_sec, sizeof(tls->x25519_sec));

  // fill in the gaps: random + session ID + keyshare
  memmove(msg_server_hello + 6, tls->random, sizeof(tls->random));
  memmove(msg_server_hello + 39, tls->session_id, sizeof(tls->session_id));
  memmove(msg_server_hello + 84, x25519_pub, sizeof(x25519_pub));

  // server hello message
  mg_iobuf_add(wio, wio->len, "\x16\x03\x03\x00\x7a", 5);
  mg_iobuf_add(wio, wio->len, msg_server_hello, sizeof(msg_server_hello));
  mg_sha256_update(&tls->sha256, msg_server_hello, sizeof(msg_server_hello));

  // change cipher message
  mg_iobuf_add(wio, wio->len, "\x14\x03\x03\x00\x01\x01", 6);
}

static void mg_tls_server_send_ext(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  // server extensions
  uint8_t ext[6] = {0x08, 0, 0, 2, 0, 0};
  mg_sha256_update(&tls->sha256, ext, sizeof(ext));
  mg_tls_encrypt(c, ext, sizeof(ext), MG_TLS_HANDSHAKE);
}

static void mg_tls_server_send_cert(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  // server DER certificate (empty)
  size_t n = tls->cert_der.len;
  uint8_t *cert = (uint8_t *) calloc(1, 13 + n);
  if (cert == NULL) {
    mg_error(c, "tls cert oom");
    return;
  }
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
  memmove(cert + 11, tls->cert_der.buf, n);
  cert[11 + n] = cert[12 + n] = 0;  // certificate extensions (none)
  mg_sha256_update(&tls->sha256, cert, 13 + n);
  mg_tls_encrypt(c, cert, 13 + n, MG_TLS_HANDSHAKE);
  free(cert);
}

// type adapter between uECC hash context and our sha256 implementation
typedef struct SHA256_HashContext {
  MG_UECC_HashContext uECC;
  mg_sha256_ctx ctx;
} SHA256_HashContext;

static void init_SHA256(const MG_UECC_HashContext *base) {
  SHA256_HashContext *c = (SHA256_HashContext *) base;
  mg_sha256_init(&c->ctx);
}

static void update_SHA256(const MG_UECC_HashContext *base,
                          const uint8_t *message, unsigned message_size) {
  SHA256_HashContext *c = (SHA256_HashContext *) base;
  mg_sha256_update(&c->ctx, message, message_size);
}
static void finish_SHA256(const MG_UECC_HashContext *base,
                          uint8_t *hash_result) {
  SHA256_HashContext *c = (SHA256_HashContext *) base;
  mg_sha256_final(hash_result, &c->ctx);
}

static void mg_tls_send_cert_verify(struct mg_connection *c, int is_client) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  // server certificate verify packet
  uint8_t verify[82] = {0x0f, 0x00, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00};
  size_t sigsz, verifysz = 0;
  uint8_t hash[32] = {0}, tmp[2 * 32 + 64] = {0};
  struct SHA256_HashContext ctx = {
      {&init_SHA256, &update_SHA256, &finish_SHA256, 64, 32, tmp},
      {{0}, 0, 0, {0}}};
  int neg1, neg2;
  uint8_t sig[64] = {0};

  mg_tls_calc_cert_verify_hash(c, (uint8_t *) hash, is_client);

  mg_uecc_sign_deterministic(tls->ec_key, hash, sizeof(hash), &ctx.uECC, sig,
                             mg_uecc_secp256r1());

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

  mg_sha256_update(&tls->sha256, verify, verifysz);
  mg_tls_encrypt(c, verify, verifysz, MG_TLS_HANDSHAKE);
}

static void mg_tls_server_send_finish(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  struct mg_iobuf *wio = &tls->send;
  mg_sha256_ctx sha256;
  uint8_t hash[32];
  uint8_t finish[36] = {0x14, 0, 0, 32};
  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(hash, &sha256);
  mg_hmac_sha256(finish + 4, tls->enc.server_finished_key, 32, hash, 32);
  mg_tls_encrypt(c, finish, sizeof(finish), MG_TLS_HANDSHAKE);
  mg_io_send(c, wio->buf, wio->len);
  wio->len = 0;

  mg_sha256_update(&tls->sha256, finish, sizeof(finish));
}

static int mg_tls_server_recv_finish(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  unsigned char *recv_buf;
  // we have to backup sha256 value to restore it later, since Finished record
  // is exceptional and is not supposed to be added to the rolling hash
  // calculation.
  mg_sha256_ctx sha256 = tls->sha256;
  if (mg_tls_recv_record(c) < 0) {
    return -1;
  }
  recv_buf = &c->rtls.buf[tls->recv_offset];
  if (recv_buf[0] != MG_TLS_FINISHED) {
    mg_error(c, "expected Finish but got msg 0x%02x", recv_buf[0]);
    return -1;
  }
  mg_tls_drop_message(c);

  // restore hash
  tls->sha256 = sha256;
  return 0;
}

static void mg_tls_client_send_hello(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  struct mg_iobuf *wio = &tls->send;

  uint8_t x25519_pub[X25519_BYTES];

  // the only signature algorithm we actually support
  uint8_t secp256r1_sig_algs[8] = {
      0x00, 0x0d, 0x00, 0x04, 0x00, 0x02, 0x04, 0x03,
  };
  // all popular signature algorithms (if we don't care about verification)
  uint8_t all_sig_algs[34] = {
      0x00, 0x0d, 0x00, 0x1e, 0x00, 0x1c, 0x04, 0x03, 0x05, 0x03, 0x06, 0x03,
      0x08, 0x07, 0x08, 0x08, 0x08, 0x09, 0x08, 0x0a, 0x08, 0x0b, 0x08, 0x04,
      0x08, 0x05, 0x08, 0x06, 0x04, 0x01, 0x05, 0x01, 0x06, 0x01};
  uint8_t server_name_ext[9] = {0x00, 0x00, 0x00, 0xfe, 0x00,
                                0xfe, 0x00, 0x00, 0xfe};

  // clang-format off
  uint8_t msg_client_hello[145] = {
      // TLS Client Hello header reported as TLS1.2 (5)
      0x16, 0x03, 0x03, 0x00, 0xfe,
      // client hello, tls 1.2 (6)
      0x01, 0x00, 0x00, 0x8c, 0x03, 0x03,
      // random (32 bytes)
      PLACEHOLDER_32B,
      // session ID length + session ID (32 bytes)
      0x20, PLACEHOLDER_32B, 0x00,
      0x02,  // size = 2 bytes
#if defined(CHACHA20) && CHACHA20
      // TLS_CHACHA20_POLY1305_SHA256
      0x13, 0x03,
#else
      // TLS_AES_128_GCM_SHA256
      0x13, 0x01,
#endif
      // no compression
      0x01, 0x00,
      // extensions + keyshare
      0x00, 0xfe,
      // x25519 keyshare
      0x00, 0x33, 0x00, 0x26, 0x00, 0x24, 0x00, 0x1d, 0x00, 0x20,
      PLACEHOLDER_32B,
      // supported groups (x25519)
      0x00, 0x0a, 0x00, 0x04, 0x00, 0x02, 0x00, 0x1d,
      // supported versions (tls1.3 == 0x304)
      0x00, 0x2b, 0x00, 0x03, 0x02, 0x03, 0x04,
      // session ticket (none)
      0x00, 0x23, 0x00, 0x00, // 144 bytes till here
	};
  // clang-format on
  const char *hostname = tls->hostname;
  size_t hostnamesz = strlen(tls->hostname);
  size_t hostname_extsz = hostnamesz ? hostnamesz + 9 : 0;
  uint8_t *sig_alg = tls->skip_verification ? all_sig_algs : secp256r1_sig_algs;
  size_t sig_alg_sz = tls->skip_verification ? sizeof(all_sig_algs)
                                             : sizeof(secp256r1_sig_algs);

  // patch ClientHello with correct hostname ext length (if any)
  MG_STORE_BE16(msg_client_hello + 3,
                hostname_extsz + 183 - 9 - 34 + sig_alg_sz);
  MG_STORE_BE16(msg_client_hello + 7,
                hostname_extsz + 179 - 9 - 34 + sig_alg_sz);
  MG_STORE_BE16(msg_client_hello + 82,
                hostname_extsz + 104 - 9 - 34 + sig_alg_sz);

  if (hostnamesz > 0) {
    MG_STORE_BE16(server_name_ext + 2, hostnamesz + 5);
    MG_STORE_BE16(server_name_ext + 4, hostnamesz + 3);
    MG_STORE_BE16(server_name_ext + 7, hostnamesz);
  }

  // calculate keyshare
  if (!mg_random(tls->x25519_cli, sizeof(tls->x25519_cli))) mg_error(c, "RNG");
  mg_tls_x25519(x25519_pub, tls->x25519_cli, X25519_BASE_POINT, 1);

  // fill in the gaps: random + session ID + keyshare
  if (!mg_random(tls->session_id, sizeof(tls->session_id))) mg_error(c, "RNG");
  if (!mg_random(tls->random, sizeof(tls->random))) mg_error(c, "RNG");
  memmove(msg_client_hello + 11, tls->random, sizeof(tls->random));
  memmove(msg_client_hello + 44, tls->session_id, sizeof(tls->session_id));
  memmove(msg_client_hello + 94, x25519_pub, sizeof(x25519_pub));

  // client hello message
  mg_iobuf_add(wio, wio->len, msg_client_hello, sizeof(msg_client_hello));
  mg_sha256_update(&tls->sha256, msg_client_hello + 5,
                   sizeof(msg_client_hello) - 5);
  mg_iobuf_add(wio, wio->len, sig_alg, sig_alg_sz);
  mg_sha256_update(&tls->sha256, sig_alg, sig_alg_sz);
  if (hostnamesz > 0) {
    mg_iobuf_add(wio, wio->len, server_name_ext, sizeof(server_name_ext));
    mg_iobuf_add(wio, wio->len, hostname, hostnamesz);
    mg_sha256_update(&tls->sha256, server_name_ext, sizeof(server_name_ext));
    mg_sha256_update(&tls->sha256, (uint8_t *) hostname, hostnamesz);
  }

  // change cipher message
  mg_iobuf_add(wio, wio->len, (const char *) "\x14\x03\x03\x00\x01\x01", 6);
  mg_io_send(c, wio->buf, wio->len);
  wio->len = 0;
}

static int mg_tls_client_recv_hello(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  struct mg_iobuf *rio = &c->rtls;
  uint16_t msgsz;
  uint8_t *ext;
  uint16_t ext_len;
  int j;

  if (!mg_tls_got_record(c)) {
    return MG_IO_WAIT;
  }
  if (rio->buf[0] != MG_TLS_HANDSHAKE || rio->buf[5] != MG_TLS_SERVER_HELLO) {
    if (rio->buf[0] == MG_TLS_ALERT && rio->len >= 7) {
      mg_error(c, "tls alert %d", rio->buf[6]);
      return -1;
    }
    MG_INFO(("got packet type 0x%02x/0x%02x", rio->buf[0], rio->buf[5]));
    mg_error(c, "not a server hello packet");
    return -1;
  }

  msgsz = MG_LOAD_BE16(rio->buf + 3);
  mg_sha256_update(&tls->sha256, rio->buf + 5, msgsz);

  ext_len = MG_LOAD_BE16(rio->buf + 5 + 39 + 32 + 3);
  ext = rio->buf + 5 + 39 + 32 + 3 + 2;
  if (ext_len > (rio->len - (5 + 39 + 32 + 3 + 2))) goto fail;

  for (j = 0; j < ext_len;) {
    uint16_t ext_type = MG_LOAD_BE16(ext + j);
    uint16_t ext_len2 = MG_LOAD_BE16(ext + j + 2);
    uint16_t group;
    uint8_t *key_exchange;
    uint16_t key_exchange_len;
    if (ext_len2 > (ext_len - j - 4)) goto fail;
    if (ext_type != 0x0033) {  // not a key share extension, ignore
      j += (uint16_t) (ext_len2 + 4);
      continue;
    }
    group = MG_LOAD_BE16(ext + j + 4);
    if (group != 0x001d) {
      mg_error(c, "bad key exchange group");
      return -1;
    }
    key_exchange_len = MG_LOAD_BE16(ext + j + 6);
    key_exchange = ext + j + 8;
    if (key_exchange_len != 32) {
      mg_error(c, "bad key exchange length");
      return -1;
    }
    mg_tls_x25519(tls->x25519_sec, tls->x25519_cli, key_exchange, 1);
    mg_tls_hexdump("c x25519 sec", tls->x25519_sec, 32);
    mg_tls_drop_record(c);
    /* generate handshake keys */
    mg_tls_generate_handshake_keys(c);
    return 0;
  }
fail:
  mg_error(c, "bad client hello");
  return -1;
}

static int mg_tls_client_recv_ext(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  unsigned char *recv_buf;
  if (mg_tls_recv_record(c) < 0) {
    return -1;
  }
  recv_buf = &c->rtls.buf[tls->recv_offset];
  if (recv_buf[0] != MG_TLS_ENCRYPTED_EXTENSIONS) {
    mg_error(c, "expected server extensions but got msg 0x%02x", recv_buf[0]);
    return -1;
  }
  mg_tls_drop_message(c);
  return 0;
}

static int mg_tls_client_recv_cert(struct mg_connection *c) {
  uint8_t *cert;
  uint32_t certsz;
  struct mg_der_tlv oid, pubkey, seq, subj;
  int subj_match = 0;
  struct tls_data *tls = (struct tls_data *) c->tls;
  unsigned char *recv_buf;
  if (mg_tls_recv_record(c) < 0) {
    return -1;
  }
  recv_buf = &c->rtls.buf[tls->recv_offset];
  if (recv_buf[0] == MG_TLS_CERTIFICATE_REQUEST) {
    MG_VERBOSE(("got certificate request"));
    mg_tls_drop_message(c);
    tls->cert_requested = 1;
    return -1;
  }
  if (recv_buf[0] != MG_TLS_CERTIFICATE) {
    mg_error(c, "expected server certificate but got msg 0x%02x", recv_buf[0]);
    return -1;
  }
  if (tls->skip_verification) {
    mg_tls_drop_message(c);
    return 0;
  }

  if (tls->recv_len < 11) {
    mg_error(c, "certificate list too short");
    return -1;
  }

  cert = recv_buf + 11;
  certsz = MG_LOAD_BE24(recv_buf + 8);
  if (certsz > tls->recv_len - 11) {
    mg_error(c, "certificate too long: %d vs %d", certsz, tls->recv_len - 11);
    return -1;
  }

  do {
    // secp256r1 public key
    if (mg_der_find(cert, certsz,
                    (uint8_t *) "\x2A\x86\x48\xCE\x3D\x03\x01\x07", 8,
                    &oid) < 0) {
      mg_error(c, "certificate secp256r1 public key OID not found");
      return -1;
    }
    if (mg_der_to_tlv(oid.value + oid.len,
                      (size_t) (cert + certsz - (oid.value + oid.len)),
                      &pubkey) < 0) {
      mg_error(c, "certificate secp256r1 public key not found");
      return -1;
    }

    // expect BIT STRING, unpadded, uncompressed: [0]+[4]+32+32 content bytes
    if (pubkey.type != 3 || pubkey.len != 66 || pubkey.value[0] != 0 ||
        pubkey.value[1] != 4) {
      mg_error(c, "unsupported public key bitstring encoding");
      return -1;
    }
    memmove(tls->pubkey, pubkey.value + 2, pubkey.len - 2);
  } while (0);

  // Subject Alternative Names
  do {
    if (mg_der_find(cert, certsz, (uint8_t *) "\x55\x1d\x11", 3, &oid) < 0) {
      mg_error(c, "certificate does not contain subject alternative names");
      return -1;
    }
    if (mg_der_to_tlv(oid.value + oid.len,
                      (size_t) (cert + certsz - (oid.value + oid.len)),
                      &seq) < 0) {
      mg_error(c, "certificate subject alternative names not found");
      return -1;
    }
    if (mg_der_to_tlv(seq.value, seq.len, &seq) < 0) {
      mg_error(
          c,
          "certificate subject alternative names is not a constructed object");
      return -1;
    }
    MG_VERBOSE(("verify hostname %s", tls->hostname));
    while (seq.len > 0) {
      if (mg_der_to_tlv(seq.value, seq.len, &subj) < 0) {
        mg_error(c, "bad subject alternative name");
        return -1;
      }
      MG_VERBOSE(("subj=%.*s", subj.len, subj.value));
      if (mg_match(mg_str((const char *) tls->hostname),
                   mg_str_n((const char *) subj.value, subj.len), NULL)) {
        subj_match = 1;
        break;
      }
      seq.len = (uint32_t) (seq.value + seq.len - (subj.value + subj.len));
      seq.value = subj.value + subj.len;
    }
    if (!subj_match) {
      mg_error(c, "certificate did not match the hostname");
      return -1;
    }
  } while (0);

  mg_tls_drop_message(c);
  mg_tls_calc_cert_verify_hash(c, tls->sighash, 0);
  return 0;
}

static int mg_tls_client_recv_cert_verify(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  unsigned char *recv_buf;
  if (mg_tls_recv_record(c) < 0) {
    return -1;
  }
  recv_buf = &c->rtls.buf[tls->recv_offset];
  if (recv_buf[0] != MG_TLS_CERTIFICATE_VERIFY) {
    mg_error(c, "expected server certificate verify but got msg 0x%02x", recv_buf[0]);
    return -1;
  }
  // Ignore CertificateVerify is strict checks are not required
  if (tls->skip_verification) {
    mg_tls_drop_message(c);
    return 0;
  }

  // Extract certificate signature and verify it using pubkey and sighash
  do {
    uint8_t sig[64];
    struct mg_der_tlv seq, a, b;
    if (mg_der_to_tlv(recv_buf + 8, tls->recv_len - 8, &seq) < 0) {
      mg_error(c, "verification message is not an ASN.1 DER sequence");
      return -1;
    }
    if (mg_der_to_tlv(seq.value, seq.len, &a) < 0) {
      mg_error(c, "missing first part of the signature");
      return -1;
    }
    if (mg_der_to_tlv(a.value + a.len, seq.len - a.len, &b) < 0) {
      mg_error(c, "missing second part of the signature");
      return -1;
    }
    // Integers may be padded with zeroes
    if (a.len > 32) {
      a.value = a.value + (a.len - 32);
      a.len = 32;
    }
    if (b.len > 32) {
      b.value = b.value + (b.len - 32);
      b.len = 32;
    }

    memmove(sig, a.value, a.len);
    memmove(sig + 32, b.value, b.len);

    if (mg_uecc_verify(tls->pubkey, tls->sighash, sizeof(tls->sighash), sig,
                       mg_uecc_secp256r1()) != 1) {
      mg_error(c, "failed to verify certificate");
      return -1;
    }
  } while (0);

  mg_tls_drop_message(c);
  return 0;
}

static int mg_tls_client_recv_finish(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  unsigned char *recv_buf;
  if (mg_tls_recv_record(c) < 0) {
    return -1;
  }
  recv_buf = &c->rtls.buf[tls->recv_offset];
  if (recv_buf[0] != MG_TLS_FINISHED) {
    mg_error(c, "expected server finished but got msg 0x%02x", recv_buf[0]);
    return -1;
  }
  mg_tls_drop_message(c);
  return 0;
}

static void mg_tls_client_send_finish(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  struct mg_iobuf *wio = &tls->send;
  mg_sha256_ctx sha256;
  uint8_t hash[32];
  uint8_t finish[36] = {0x14, 0, 0, 32};
  memmove(&sha256, &tls->sha256, sizeof(mg_sha256_ctx));
  mg_sha256_final(hash, &sha256);
  mg_hmac_sha256(finish + 4, tls->enc.client_finished_key, 32, hash, 32);
  mg_tls_encrypt(c, finish, sizeof(finish), MG_TLS_HANDSHAKE);
  mg_io_send(c, wio->buf, wio->len);
  wio->len = 0;
}

static void mg_tls_client_handshake(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  switch (tls->state) {
    case MG_TLS_STATE_CLIENT_START:
      mg_tls_client_send_hello(c);
      tls->state = MG_TLS_STATE_CLIENT_WAIT_SH;
      // Fallthrough
    case MG_TLS_STATE_CLIENT_WAIT_SH:
      if (mg_tls_client_recv_hello(c) < 0) {
        break;
      }
      tls->state = MG_TLS_STATE_CLIENT_WAIT_EE;
      // Fallthrough
    case MG_TLS_STATE_CLIENT_WAIT_EE:
      if (mg_tls_client_recv_ext(c) < 0) {
        break;
      }
      tls->state = MG_TLS_STATE_CLIENT_WAIT_CERT;
      // Fallthrough
    case MG_TLS_STATE_CLIENT_WAIT_CERT:
      if (mg_tls_client_recv_cert(c) < 0) {
        break;
      }
      tls->state = MG_TLS_STATE_CLIENT_WAIT_CV;
      // Fallthrough
    case MG_TLS_STATE_CLIENT_WAIT_CV:
      if (mg_tls_client_recv_cert_verify(c) < 0) {
        break;
      }
      tls->state = MG_TLS_STATE_CLIENT_WAIT_FINISHED;
      // Fallthrough
    case MG_TLS_STATE_CLIENT_WAIT_FINISHED:
      if (mg_tls_client_recv_finish(c) < 0) {
        break;
      }
      if (tls->cert_requested) {
        /* for mTLS we should generate application keys at this point
         * but then restore handshake keys and continue with
         * the rest of the handshake */
        struct tls_enc app_keys;
        struct tls_enc hs_keys = tls->enc;
        mg_tls_generate_application_keys(c);
        app_keys = tls->enc;
        tls->enc = hs_keys;
        mg_tls_server_send_cert(c);
        mg_tls_send_cert_verify(c, 1);
        mg_tls_client_send_finish(c);
        tls->enc = app_keys;
      } else {
        mg_tls_client_send_finish(c);
        mg_tls_generate_application_keys(c);
      }
      tls->state = MG_TLS_STATE_CLIENT_CONNECTED;
      c->is_tls_hs = 0;
      break;
    default:
      mg_error(c, "unexpected client state: %d", tls->state);
      break;
  }
}

static void mg_tls_server_handshake(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  switch (tls->state) {
    case MG_TLS_STATE_SERVER_START:
      if (mg_tls_server_recv_hello(c) < 0) {
        return;
      }
      mg_tls_server_send_hello(c);
      mg_tls_generate_handshake_keys(c);
      mg_tls_server_send_ext(c);
      mg_tls_server_send_cert(c);
      mg_tls_send_cert_verify(c, 0);
      mg_tls_server_send_finish(c);
      tls->state = MG_TLS_STATE_SERVER_NEGOTIATED;
      // fallthrough
    case MG_TLS_STATE_SERVER_NEGOTIATED:
      if (mg_tls_server_recv_finish(c) < 0) {
        return;
      }
      mg_tls_generate_application_keys(c);
      tls->state = MG_TLS_STATE_SERVER_CONNECTED;
      c->is_tls_hs = 0;
      return;
    default:
      mg_error(c, "unexpected server state: %d", tls->state);
      break;
  }
}

void mg_tls_handshake(struct mg_connection *c) {
  if (c->is_client) {
    mg_tls_client_handshake(c);
  } else {
    mg_tls_server_handshake(c);
  }
}

static int mg_parse_pem(const struct mg_str pem, const struct mg_str label,
                        struct mg_str *der) {
  size_t n = 0, m = 0;
  char *s;
  const char *c;
  struct mg_str caps[6];  // number of wildcards + 1
  if (!mg_match(pem, mg_str("#-----BEGIN #-----#-----END #-----#"), caps)) {
    *der = mg_strdup(pem);
    return 0;
  }
  if (mg_strcmp(caps[1], label) != 0 || mg_strcmp(caps[3], label) != 0) {
    return -1;  // bad label
  }
  if ((s = (char *) calloc(1, caps[2].len)) == NULL) {
    return -1;
  }

  for (c = caps[2].buf; c < caps[2].buf + caps[2].len; c++) {
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
  der->buf = s;
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

  tls->state =
      c->is_client ? MG_TLS_STATE_CLIENT_START : MG_TLS_STATE_SERVER_START;

  tls->skip_verification = opts->skip_verification;
  //tls->send.align = MG_IO_SIZE;

  c->tls = tls;
  c->is_tls = c->is_tls_hs = 1;
  mg_sha256_init(&tls->sha256);

  // save hostname (client extension)
  if (opts->name.len > 0) {
    if (opts->name.len >= sizeof(tls->hostname) - 1) {
      mg_error(c, "hostname too long");
      return;
    }
    strncpy((char *) tls->hostname, opts->name.buf, sizeof(tls->hostname) - 1);
    tls->hostname[opts->name.len] = 0;
  }

  if (opts->cert.buf == NULL) {
    MG_VERBOSE(("no certificate provided"));
    return;
  }

  // parse PEM or DER certificate
  if (mg_parse_pem(opts->cert, mg_str_s("CERTIFICATE"), &tls->cert_der) < 0) {
    MG_ERROR(("Failed to load certificate"));
    return;
  }

  // parse PEM or DER EC key
  if (opts->key.buf == NULL) {
    mg_error(c, "certificate provided without a private key");
    return;
  }

  if (mg_parse_pem(opts->key, mg_str_s("EC PRIVATE KEY"), &key) == 0) {
    if (key.len < 39) {
      MG_ERROR(("EC private key too short"));
      return;
    }
    // expect ASN.1 SEQUENCE=[INTEGER=1, BITSTRING of 32 bytes, ...]
    // 30 nn 02 01 01 04 20 [key] ...
    if (key.buf[0] != 0x30 || (key.buf[1] & 0x80) != 0) {
      MG_ERROR(("EC private key: ASN.1 bad sequence"));
      return;
    }
    if (memcmp(key.buf + 2, "\x02\x01\x01\x04\x20", 5) != 0) {
      MG_ERROR(("EC private key: ASN.1 bad data"));
    }
    memmove(tls->ec_key, key.buf + 7, 32);
    free((void *) key.buf);
  } else if (mg_parse_pem(opts->key, mg_str_s("PRIVATE KEY"), &key) == 0) {
    mg_error(c, "PKCS8 private key format is not supported");
  } else {
    mg_error(c, "expected EC PRIVATE KEY or PRIVATE KEY");
  }
}

void mg_tls_free(struct mg_connection *c) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  if (tls != NULL) {
    mg_iobuf_free(&tls->send);
    free((void *) tls->cert_der.buf);
  }
  free(c->tls);
  c->tls = NULL;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct tls_data *tls = (struct tls_data *) c->tls;
  long n = MG_IO_WAIT;
  if (len > MG_IO_SIZE) len = MG_IO_SIZE;
  if (len > 16384) len = 16384;
  mg_tls_encrypt(c, (const uint8_t *) buf, len, MG_TLS_APP_DATA);
  while (tls->send.len > 0 &&
         (n = mg_io_send(c, tls->send.buf, tls->send.len)) > 0) {
    mg_iobuf_del(&tls->send, 0, (size_t) n);
  }
  if (n == MG_IO_ERR || n == MG_IO_WAIT) return n;
  return (long) len;
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  int r = 0;
  struct tls_data *tls = (struct tls_data *) c->tls;
  unsigned char *recv_buf;
  size_t minlen;

  r = mg_tls_recv_record(c);
  if (r < 0) {
    return r;
  }
  recv_buf = &c->rtls.buf[tls->recv_offset];

  if (tls->content_type != MG_TLS_APP_DATA) {
    tls->recv_len = 0;
    mg_tls_drop_record(c);
    return MG_IO_WAIT;
  }
  minlen = len < tls->recv_len ? len : tls->recv_len;
  memmove(buf, recv_buf, minlen);
  tls->recv_offset += minlen;
  tls->recv_len -= minlen;
  if (tls->recv_len == 0) {
    mg_tls_drop_record(c);
  }
  return (long) minlen;
}

size_t mg_tls_pending(struct mg_connection *c) {
  return mg_tls_got_record(c) ? 1 : 0;
}

void mg_tls_ctx_init(struct mg_mgr *mgr) {
  (void) mgr;
}

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  (void) mgr;
}
#endif
