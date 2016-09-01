---
title: "mg_file_upload_handler()"
decl_name: "mg_file_upload_handler"
symbol_kind: "func"
signature: |
  void mg_file_upload_handler(struct mg_connection *nc, int ev, void *ev_data,
                              mg_fu_fname_fn local_name_fn);
---

File upload handler.
This handler can be used to implement file uploads with minimum code.
This handler will process MG_EV_HTTP_PART_* events and store file data into
a local file.
`local_name_fn` will be invoked with whatever name was provided by the client
and will expect the name of the local file to open. A return value of NULL
will abort file upload (client will get a "403 Forbidden" response). If
non-null, the returned string must be heap-allocated and will be freed by
the caller.
Exception: it is ok to return the same string verbatim.

Example:

```c
struct mg_str upload_fname(struct mg_connection *nc, struct mg_str fname) {
  // Just return the same filename. Do not actually do this except in test!
  // fname is user-controlled and needs to be sanitized.
  return fname;
}
void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    ...
    case MG_EV_HTTP_PART_BEGIN:
    case MG_EV_HTTP_PART_DATA:
    case MG_EV_HTTP_PART_END:
      mg_file_upload_handler(nc, ev, ev_data, upload_fname);
      break;
  }
}
``` 

