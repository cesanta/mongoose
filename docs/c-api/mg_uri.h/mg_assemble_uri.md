---
title: "mg_assemble_uri()"
decl_name: "mg_assemble_uri"
symbol_kind: "func"
signature: |
  int mg_assemble_uri(const struct mg_str *scheme, const struct mg_str *user_info,
                      const struct mg_str *host, unsigned int port,
                      const struct mg_str *path, const struct mg_str *query,
                      const struct mg_str *fragment, int normalize_path,
                      struct mg_str *uri);
---

Assemble URI from parts. Any of the inputs can be NULL or zero-length mg_str.

If normalize_path is true, path is normalized by resolving relative refs.

Result is a heap-allocated string (uri->p must be free()d after use).

Returns 0 on success, -1 on error. 

