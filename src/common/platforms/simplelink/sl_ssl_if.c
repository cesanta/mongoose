/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_SIMPLELINK

#include "common/mg_mem.h"

#ifndef MG_SSL_IF_SIMPLELINK_SLFS_PREFIX
#define MG_SSL_IF_SIMPLELINK_SLFS_PREFIX "SL:"
#endif

#define MG_SSL_IF_SIMPLELINK_SLFS_PREFIX_LEN \
  (sizeof(MG_SSL_IF_SIMPLELINK_SLFS_PREFIX) - 1)

struct mg_ssl_if_ctx {
  char *ssl_cert;
  char *ssl_key;
  char *ssl_ca_cert;
  char *ssl_server_name;
};

void mg_ssl_if_init() {
}

enum mg_ssl_if_result mg_ssl_if_conn_init(
    struct mg_connection *nc, const struct mg_ssl_if_conn_params *params,
    const char **err_msg) {
  struct mg_ssl_if_ctx *ctx =
      (struct mg_ssl_if_ctx *) MG_CALLOC(1, sizeof(*ctx));
  if (ctx == NULL) {
    MG_SET_PTRPTR(err_msg, "Out of memory");
    return MG_SSL_ERROR;
  }
  nc->ssl_if_data = ctx;

  if (params->cert != NULL || params->key != NULL) {
    if (params->cert != NULL && params->key != NULL) {
      ctx->ssl_cert = strdup(params->cert);
      ctx->ssl_key = strdup(params->key);
    } else {
      MG_SET_PTRPTR(err_msg, "Both cert and key are required.");
      return MG_SSL_ERROR;
    }
  }
  if (params->ca_cert != NULL && strcmp(params->ca_cert, "*") != 0) {
    ctx->ssl_ca_cert = strdup(params->ca_cert);
  }
  /* TODO(rojer): cipher_suites. */
  if (params->server_name != NULL) {
    ctx->ssl_server_name = strdup(params->server_name);
  }
  return MG_SSL_OK;
}

enum mg_ssl_if_result mg_ssl_if_conn_accept(struct mg_connection *nc,
                                            struct mg_connection *lc) {
  /* SimpleLink does everything for us, nothing for us to do. */
  (void) nc;
  (void) lc;
  return MG_SSL_OK;
}

enum mg_ssl_if_result mg_ssl_if_handshake(struct mg_connection *nc) {
  /* SimpleLink has already performed the handshake, nothing to do. */
  return MG_SSL_OK;
}

int mg_ssl_if_read(struct mg_connection *nc, void *buf, size_t len) {
  /* SimpelLink handles TLS, so this is just a pass-through. */
  int n = nc->iface->vtable->tcp_recv(nc, buf, len);
  if (n == 0) nc->flags |= MG_F_WANT_READ;
  return n;
}

int mg_ssl_if_write(struct mg_connection *nc, const void *buf, size_t len) {
  /* SimpelLink handles TLS, so this is just a pass-through. */
  return nc->iface->vtable->tcp_send(nc, buf, len);
}

void mg_ssl_if_conn_close_notify(struct mg_connection *nc) {
  /* Nothing to do */
  (void) nc;
}

void mg_ssl_if_conn_free(struct mg_connection *nc) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  if (ctx == NULL) return;
  nc->ssl_if_data = NULL;
  MG_FREE(ctx->ssl_cert);
  MG_FREE(ctx->ssl_key);
  MG_FREE(ctx->ssl_ca_cert);
  MG_FREE(ctx->ssl_server_name);
  memset(ctx, 0, sizeof(*ctx));
  MG_FREE(ctx);
}

bool pem_to_der(const char *pem_file, const char *der_file) {
  bool ret = false;
  FILE *pf = NULL, *df = NULL;
  bool writing = false;
  pf = fopen(pem_file, "r");
  if (pf == NULL) goto clean;
  remove(der_file);
  fs_slfs_set_file_size(der_file + MG_SSL_IF_SIMPLELINK_SLFS_PREFIX_LEN, 2048);
  df = fopen(der_file, "w");
  fs_slfs_unset_file_flags(der_file + MG_SSL_IF_SIMPLELINK_SLFS_PREFIX_LEN);
  if (df == NULL) goto clean;
  while (1) {
    char pem_buf[70];
    char der_buf[48];
    if (!fgets(pem_buf, sizeof(pem_buf), pf)) break;
    if (writing) {
      if (strstr(pem_buf, "-----END ") != NULL) {
        ret = true;
        break;
      }
      int l = 0;
      while (!isspace((unsigned int) pem_buf[l])) l++;
      int der_len = 0;
      cs_base64_decode((const unsigned char *) pem_buf, sizeof(pem_buf),
                       der_buf, &der_len);
      if (der_len <= 0) break;
      if (fwrite(der_buf, 1, der_len, df) != der_len) break;
    } else if (strstr(pem_buf, "-----BEGIN ") != NULL) {
      writing = true;
    }
  }

clean:
  if (pf != NULL) fclose(pf);
  if (df != NULL) {
    fclose(df);
    if (!ret) remove(der_file);
  }
  return ret;
}

#if MG_ENABLE_FILESYSTEM && defined(MG_FS_SLFS)
/* If the file's extension is .pem, convert it to DER format and put on SLFS. */
static char *sl_pem2der(const char *pem_file) {
  const char *pem_ext = strstr(pem_file, ".pem");
  if (pem_ext == NULL || *(pem_ext + 4) != '\0') {
    return strdup(pem_file);
  }
  char *der_file = NULL;
  /* DER file must be located on SLFS, add prefix. */
  int l = mg_asprintf(&der_file, 0, MG_SSL_IF_SIMPLELINK_SLFS_PREFIX "%.*s.der",
                      (int) (pem_ext - pem_file), pem_file);
  if (der_file == NULL) return NULL;
  bool result = false;
  cs_stat_t st;
  if (mg_stat(der_file, &st) != 0) {
    result = pem_to_der(pem_file, der_file);
    LOG(LL_DEBUG, ("%s -> %s = %d", pem_file, der_file, result));
  } else {
    /* File exists, assume it's already been converted. */
    result = true;
  }
  if (result) {
    /* Strip the SL: prefix we added since NWP does not expect it. */
    memmove(der_file, der_file + MG_SSL_IF_SIMPLELINK_SLFS_PREFIX_LEN,
            l - 2 /* including \0 */);
  } else {
    MG_FREE(der_file);
    der_file = NULL;
  }
  return der_file;
}
#else
static char *sl_pem2der(const char *pem_file) {
  return strdup(pem_file);
}
#endif

int sl_set_ssl_opts(int sock, struct mg_connection *nc) {
  int err;
  const struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  DBG(("%p ssl ctx: %p", nc, ctx));

  if (ctx == NULL) return 0;
  DBG(("%p %s,%s,%s,%s", nc, (ctx->ssl_cert ? ctx->ssl_cert : "-"),
       (ctx->ssl_key ? ctx->ssl_cert : "-"),
       (ctx->ssl_ca_cert ? ctx->ssl_ca_cert : "-"),
       (ctx->ssl_server_name ? ctx->ssl_server_name : "-")));
  if (ctx->ssl_cert != NULL && ctx->ssl_key != NULL) {
    char *ssl_cert = sl_pem2der(ctx->ssl_cert), *ssl_key = NULL;
    if (ssl_cert != NULL) {
      err = sl_SetSockOpt(sock, SL_SOL_SOCKET,
                          SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME, ssl_cert,
                          strlen(ssl_cert));
      MG_FREE(ssl_cert);
      LOG(LL_DEBUG, ("CERTIFICATE_FILE_NAME %s -> %d", ssl_cert, err));
      ssl_key = sl_pem2der(ctx->ssl_key);
      if (ssl_key != NULL) {
        err = sl_SetSockOpt(sock, SL_SOL_SOCKET,
                            SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME, ssl_key,
                            strlen(ssl_key));
        MG_FREE(ssl_key);
        LOG(LL_DEBUG, ("PRIVATE_KEY_FILE_NAME %s -> %d", ssl_key, err));
      } else {
        err = -1;
      }
    } else {
      err = -1;
    }
    if (err != 0) return err;
  }
  if (ctx->ssl_ca_cert != NULL) {
    if (ctx->ssl_ca_cert[0] != '\0') {
      char *ssl_ca_cert = sl_pem2der(ctx->ssl_ca_cert);
      if (ssl_ca_cert != NULL) {
        err =
            sl_SetSockOpt(sock, SL_SOL_SOCKET, SL_SO_SECURE_FILES_CA_FILE_NAME,
                          ssl_ca_cert, strlen(ssl_ca_cert));
        LOG(LL_DEBUG, ("CA_FILE_NAME %s -> %d", ssl_ca_cert, err));
      } else {
        err = -1;
      }
      MG_FREE(ssl_ca_cert);
      if (err != 0) return err;
    }
  }
  if (ctx->ssl_server_name != NULL) {
    err = sl_SetSockOpt(sock, SL_SOL_SOCKET,
                        SL_SO_SECURE_DOMAIN_NAME_VERIFICATION,
                        ctx->ssl_server_name, strlen(ctx->ssl_server_name));
    DBG(("DOMAIN_NAME_VERIFICATION %s -> %d", ctx->ssl_server_name, err));
    /* Domain name verificationw as added in a NWP service pack, older
     * versions return SL_ERROR_BSD_ENOPROTOOPT. There isn't much we can do
     * about it,
     * so we ignore the error. */
    if (err != 0 && err != SL_ERROR_BSD_ENOPROTOOPT) return err;
  }
  return 0;
}

#endif /* MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_SIMPLELINK */
