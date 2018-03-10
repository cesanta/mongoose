---
title: "mg_http_parse_header2()"
decl_name: "mg_http_parse_header2"
symbol_kind: "func"
signature: |
  int mg_http_parse_header2(struct mg_str *hdr, const char *var_name, char **buf,
                            size_t buf_size);
---

Parses the HTTP header `hdr`. Finds variable `var_name` and stores its value
in the buffer `*buf`, `buf_size`. If the buffer size is not enough,
allocates a buffer of required size and writes it to `*buf`, similar to
asprintf(). The caller should always check whether the buffer was updated,
and free it if so.

This function is supposed to parse cookies, authentication headers, etc.
Example (error handling omitted):

    char user_buf[20];
    char *user = user_buf;
    struct mg_str *hdr = mg_get_http_header(hm, "Authorization");
    mg_http_parse_header2(hdr, "username", &user, sizeof(user_buf));
    // ... do something useful with user
    if (user != user_buf) {
      free(user);
    }

Returns the length of the variable's value. If variable is not found, 0 is
returned. 

