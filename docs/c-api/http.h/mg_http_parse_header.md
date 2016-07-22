---
title: "mg_http_parse_header()"
decl_name: "mg_http_parse_header"
symbol_kind: "func"
signature: |
  int mg_http_parse_header(struct mg_str *hdr, const char *var_name, char *buf,
                           size_t buf_size);
---

Parse HTTP header `hdr`. Find variable `var_name` and store it's value
in the buffer `buf`, `buf_size`. Return 0 if variable not found, non-zero
otherwise.

This function is supposed to parse
cookies, authentication headers, etcetera. Example (error handling omitted):

    char user[20];
    struct mg_str *hdr = mg_get_http_header(hm, "Authorization");
    mg_http_parse_header(hdr, "username", user, sizeof(user));

Return length of the variable's value. If buffer is not large enough,
or variable not found, 0 is returned. 

