#include "mongoose.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  /* We want enough data */
  if (size < 5) {
      return 0;
  }

  /* allocate a null-terminated string that we can use */
  char *null_terminated = malloc(size+1);
  memcpy(null_terminated, data, size);
  null_terminated[size] = '\0';

  /* http parsing */
  struct http_message req;
  memset(&req, 0x0, sizeof(struct http_message));
  mg_parse_http((const char *) data, size, &req, 0);
  if( mg_parse_http((const char *) data, size, &req, 1) != NULL) {
    mg_get_http_header(&req, null_terminated);
  }

  /* check URI parsing */
  unsigned int port;
  struct mg_str scheme, user_info, host, path, query, fragment;
  struct mg_str uri = MG_MK_STR_N(null_terminated, size);
  struct mg_str uri_out;
  uri_out.p = NULL;

  if (mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment) == 0)
  {
    mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out);
    if (uri_out.p != NULL) {
        free(uri_out.p);
    }
  }

  /* check mg_normalize_uri_path */
  struct mg_str mgs;
  char *dup = malloc(size+1);
  memcpy(dup, null_terminated, size+1);

  mgs.p = null_terminated;
  mgs.len = size;
  mg_normalize_uri_path(&mgs, &mgs);
  free(dup);

  /* some http parsing */
  struct mg_str body;
  body.p = null_terminated;
  body.len = size;
  char buf[256];
  mg_get_http_var(&body, "key", buf, sizeof(buf));

  /* DNS parsing */
  struct mg_dns_message dns_msg;
  mg_parse_dns(null_terminated, size, &dns_msg);

  char filename[100];
  const char *chunk;
  size_t chunk_len;
  char var_num[100];
  mg_parse_multipart(null_terminated, size, var_num, 
          sizeof(var_num), filename, 100, &chunk, &chunk_len);

  /* cleanup */
  free(null_terminated);

  return 0;
}

