---
title: HTTP events
---

As discussed in the overview, `mg_set_protocol_http_websocket()` function
parses incoming data, treats it as HTTP or WebSocket, and triggers high-level
HTTP or WebSocket events. Here is a list of events specific to HTTP.

- MG_EV_HTTP_REQUEST: An HTTP request has arrived. Parsed request
 is passed as
  `struct http_message` through the handler's `void *ev_data` pointer.
- MG_EV_HTTP_REPLY: An HTTP reply has arrived. Parsed reply is
  passed as `struct http_message` through the handler's `void *ev_data`
  pointer.
- MG_EV_HTTP_MULTIPART_REQUEST: A multipart POST request has arrived.
  This event is sent before body is parsed. After this, the user
  should expect a sequence of MG_EV_HTTP_PART_BEGIN/DATA/END requests.
  This is also the last time when headers and other request fields are
  accessible.
- MG_EV_HTTP_CHUNK: An HTTP chunked-encoding chunk has arrived.
  The parsed HTTP reply is passed as `struct http_message` through the
  handler's `void *ev_data` pointer. `http_message::body` would contain
  incomplete, reassembled HTTP body.
  It will grow with every new chunk that arrives, and it can
  potentially consume a lot of memory. The event handler may process
  the body as chunks are coming, and signal Mongoose to delete processed
  body by setting `MG_F_DELETE_CHUNK` in `mg_connection::flags`. When
  the last zero chunk is received,
  Mongoose sends `MG_EV_HTTP_REPLY` event with
  full reassembled body (if handler did not signal to delete chunks) or
  with empty body (if handler did signal to delete chunks).
- MG_EV_HTTP_PART_BEGIN: a new part of multipart message is started,
  extra parameters are passed in mg_http_multipart_part
- MG_EV_HTTP_PART_DATA: a new portion of data from the multiparted message
  no additional headers are available, only data and data size
- MG_EV_HTTP_PART_END: a final boundary received, analogue to maybe used to
  find the end of packet
  Note: Mongoose should be compiled with MG_ENABLE_HTTP_STREAMING_MULTIPART
  to enable multipart events.
