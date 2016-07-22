---
title: "mg_set_protocol_http_websocket()"
decl_name: "mg_set_protocol_http_websocket"
symbol_kind: "func"
signature: |
  void mg_set_protocol_http_websocket(struct mg_connection *nc);
---

Attach a built-in HTTP event handler to a given connection.
The user-defined event handler will receive the following extra events:

- MG_EV_HTTP_REQUEST: HTTP request has arrived. Parsed HTTP request
 is passed as
  `struct http_message` through the handler's `void *ev_data` pointer.
- MG_EV_HTTP_MULTIPART_REQUEST: A multipart POST request has been received.
  This event is sent before the body is parsed. After this the user
  should expect a sequence of MG_EV_HTTP_PART_BEGIN/DATA/END requests.
  This is also the last time when headers and other request fields are
  accessible.
- MG_EV_HTTP_REPLY: HTTP reply has arrived. The parsed HTTP reply is passed as
  `struct http_message` through the handler's `void *ev_data` pointer.
- MG_EV_HTTP_CHUNK: HTTP chunked-encoding chunk has arrived.
  The parsed HTTP reply is passed as `struct http_message` through the
  handler's `void *ev_data` pointer. `http_message::body` would contain
  incomplete, reassembled HTTP body.
  It will grow with every new chunk arrived and could 
  potentially consume a lot of memory. An event handler may process
  the body as chunks are coming in, and signal Mongoose to delete processed
  body by setting `MG_F_DELETE_CHUNK` in `mg_connection::flags`. When
  the last zero chunk is received,
  Mongoose sends an `MG_EV_HTTP_REPLY` event with
  the full reassembled body (if handler did not signal to delete chunks) or
  with the empty body (if handler did signal to delete chunks).
- MG_EV_WEBSOCKET_HANDSHAKE_REQUEST: server has received WebSocket handshake
  request. `ev_data` contains parsed HTTP request.
- MG_EV_WEBSOCKET_HANDSHAKE_DONE: server has completed WebSocket handshake.
  `ev_data` is `NULL`.
- MG_EV_WEBSOCKET_FRAME: new WebSocket frame has arrived. `ev_data` is
  `struct websocket_message *`
- MG_EV_HTTP_PART_BEGIN: new part of multipart message is started. 
  Extra parameters are passed in mg_http_multipart_part
- MG_EV_HTTP_PART_DATA: new portion of data from multiparted message has arrived.
  No additional headers are available, only data and data size.
- MG_EV_HTTP_PART_END: final boundary received. Marks the end of the multipart message. 
  Note: Mongoose should be compiled with MG_ENABLE_HTTP_STREAMING_MULTIPART
  to enable MG_EV_HTTP_MULTIPART_REQUEST, MG_EV_HTTP_REQUEST_END,
  MG_EV_HTTP_REQUEST_CANCEL, MG_EV_HTTP_PART_BEGIN, MG_EV_HTTP_PART_DATA,
  MG_EV_HTTP_PART_END constants 

