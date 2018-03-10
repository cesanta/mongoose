---
title: "mg_parse_multipart()"
decl_name: "mg_parse_multipart"
symbol_kind: "func"
signature: |
  size_t mg_parse_multipart(const char *buf, size_t buf_len, char *var_name,
                            size_t var_name_len, char *file_name,
                            size_t file_name_len, const char **chunk,
                            size_t *chunk_len);
---

Parses the buffer `buf`, `buf_len` that contains multipart form data chunks.
Stores the chunk name in a `var_name`, `var_name_len` buffer.
If a chunk is an uploaded file, then `file_name`, `file_name_len` is
filled with an uploaded file name. `chunk`, `chunk_len`
points to the chunk data.

Return: number of bytes to skip to the next chunk or 0 if there are
        no more chunks.

Usage example:

```c
   static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
     switch(ev) {
       case MG_EV_HTTP_REQUEST: {
         struct http_message *hm = (struct http_message *) ev_data;
         char var_name[100], file_name[100];
         const char *chunk;
         size_t chunk_len, n1, n2;

         n1 = n2 = 0;
         while ((n2 = mg_parse_multipart(hm->body.p + n1,
                                         hm->body.len - n1,
                                         var_name, sizeof(var_name),
                                         file_name, sizeof(file_name),
                                         &chunk, &chunk_len)) > 0) {
           printf("var: %s, file_name: %s, size: %d, chunk: [%.*s]\n",
                  var_name, file_name, (int) chunk_len,
                  (int) chunk_len, chunk);
           n1 += n2;
         }
       }
       break;
``` 

