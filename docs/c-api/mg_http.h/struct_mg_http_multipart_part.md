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
    /*
     * User handler can indicate how much of the data was consumed
     * by setting this variable. By default, it is assumed that all
     * data has been consumed by the handler.
     * If not all data was consumed, user's handler will be invoked again later
     * with the remainder.
     */
    size_t num_data_consumed;
  };
---

HTTP multipart part 

