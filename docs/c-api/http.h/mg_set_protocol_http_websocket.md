---
title: "mg_set_protocol_http_websocket()"
decl_name: "mg_set_protocol_http_websocket"
symbol_kind: "func"
signature: |
  void mg_set_protocol_http_websocket(struct mg_connection *nc);
---

Attaches a built-in HTTP event handler to the given connection.
The user-defined event handler will receive following extra events:

- MG_EV_HTTP_REQUEST: HTTP request has arrived. Parsed HTTP request
 is passed as
  `struct http_message` through the handler's `void *ev_data` pointer.
- MG_EV_HTTP_REPLY: The HTTP reply has arrived. The parsed HTTP reply is
  passed as `struct http_message` through the handler's `void *ev_data`
  pointer.
- MG_EV_HTTP_CHUNK: The HTTP chunked-encoding chunk has arrived.
  The parsed HTTP reply is passed as `struct http_message` through the
  handler's `void *ev_data` pointer. `http_message::body` would contain
  incomplete, reassembled HTTP body.
  It will grow with every new chunk that arrives, and it can
  potentially consume a lot of memory. An event handler may process
  the body as chunks are coming, and signal Mongoose to delete processed
  body by setting `MG_F_DELETE_CHUNK` in `mg_connection::flags`. When
  the last zero chunk is received,
  Mongoose sends `MG_EV_HTTP_REPLY` event with
  full reassembled body (if handler did not signal to delete chunks) or
  with empty body (if handler did signal to delete chunks).
- MG_EV_WEBSOCKET_HANDSHAKE_REQUEST: server has received the WebSocket
  handshake request. `ev_data` contains parsed HTTP request.
- MG_EV_WEBSOCKET_HANDSHAKE_DONE: server has completed the WebSocket
  handshake. `ev_data` is `NULL`.
- MG_EV_WEBSOCKET_FRAME: new WebSocket frame has arrived. `ev_data` is
  `struct websocket_message *`

When compiled with MG_ENABLE_HTTP_STREAMING_MULTIPART, Mongoose parses
multipart requests and splits them into separate events:
- MG_EV_HTTP_MULTIPART_REQUEST: Start of the request.
  This event is sent before body is parsed. After this, the user
  should expect a sequence of PART_BEGIN/DATA/END requests.
  This is also the last time when headers and other request fields are
  accessible.
- MG_EV_HTTP_PART_BEGIN: Start of a part of a multipart message.
  Argument: mg_http_multipart_part with var_name and file_name set
  (if present). No data is passed in this message.
- MG_EV_HTTP_PART_DATA: new portion of data from the multipart message.
  Argument: mg_http_multipart_part. var_name and file_name are preserved,
  data is available in mg_http_multipart_part.data.
- MG_EV_HTTP_PART_END: End of the current part. var_name, file_name are
  the same, no data in the message. If status is 0, then the part is
  properly terminated with a boundary, status < 0 means that connection
  was terminated.
- MG_EV_HTTP_MULTIPART_REQUEST_END: End of the multipart request.
  Argument: mg_http_multipart_part, var_name and file_name are NULL,
  status = 0 means request was properly closed, < 0 means connection
  was terminated (note: in this case both PART_END and REQUEST_END are
  delivered). 

