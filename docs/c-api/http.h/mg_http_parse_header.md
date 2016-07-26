---
title: "mg_http_parse_header()"
decl_name: "mg_http_parse_header"
symbol_kind: "func"
signature: |
  int mg_http_parse_header(struct mg_str *hdr, const char *var_name, char *buf,
                           size_t buf_size);
---

Parses the HTTP header `hdr`. Finds variable `var_name` and stores its value
in the buffer `buf`, `buf_size`. Returns 0 if variable not found, non-zero
otherwise.

This function is supposed to parse cookies, authentication headers, etc.
Example (error handling omitted):

    char user[20];
    struct mg_str *hdr = mg_get_http_header(hm, "Authorization");
    mg_http_parse_header(hdr, "username", user, sizeof(user));

Returns the length of the variable's value. If buffer is not large enough,
or variable not found, 0 is returned. 

