---
title: "struct mg_http_multipart_part"
decl_name: "struct mg_http_multipart_part"
symbol_kind: "struct"
signature: |
  struct mg_http_multipart_part {
    const char *file_name;
    const char *var_name;
    struct mg_str data;
    int status; /* <0 on error */
    void *user_data;
  };
---

HTTP multipart part 

