# Handling file uploads

In order to handle file uploads, use the following HTML snippet:

```HTML
<form method="POST" action="/upload" enctype="multipart/form-data">
  <input type="file" name="file">
  <input type="submit" value="Upload">
</form>
```

Uploaded files will be sent to the `/upload` endpoint via the `POST` request.
HTTP body will contain multipart-encoded buffer with the file contents.

To save the uploaded file, use this code snippet:

```c
struct mg_str cb(struct mg_connection *c, struct mg_str file_name) {
  // Return the same filename. Do not actually do this except in test!
  // fname is user-controlled and needs to be sanitized.
  return file_name;
}

void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  switch (ev) {
    ...
    case MG_EV_HTTP_PART_BEGIN:
    case MG_EV_HTTP_PART_DATA:
    case MG_EV_HTTP_PART_END:
      mg_file_upload_handler(c, ev, ev_data, cb);
      break;
  }
}
```
