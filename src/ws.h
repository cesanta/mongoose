#pragma once

// WebSocket frame opcodes, used in mg_ws_send() op parameter and
// in the low 4 bits of mg_ws_message.flags.
#define WEBSOCKET_OP_CONTINUE 0  // Continuation frame (fragmented message)
#define WEBSOCKET_OP_TEXT 1      // Text (UTF-8) data frame
#define WEBSOCKET_OP_BINARY 2    // Binary data frame
#define WEBSOCKET_OP_CLOSE 8     // Connection close control frame
#define WEBSOCKET_OP_PING 9      // Ping control frame (auto-replied with PONG)
#define WEBSOCKET_OP_PONG 10     // Pong control frame

#include "http.h"

// WebSocket message delivered as ev_data in MG_EV_WS_MSG and MG_EV_WS_CTL.
// data is a zero-copy slice into the receive buffer; valid until the next
// mg_mgr_poll() call.
struct mg_ws_message {
  struct mg_str data;  // Message payload
  uint8_t flags;       // First byte of the WS frame header: FIN (bit7) | opcode (bits3:0)
};

// Opens an outbound WebSocket connection.
//
// Returns:
//   Client connection, or NULL on error.
// Example:
//   mg_ws_connect(&mgr, "ws://localhost:8000/websocket", fn, NULL, NULL);
// Full examples:
//   tutorials/websocket/websocket-client, tutorials/mqtt/mqtt-over-ws-client
// Related APIs:
//   mg_ws_send(), mg_ws_printf(), mg_tls_init(), mg_ws_upgrade()
// Notes:
//   url may use ws:// or wss://. Sends the HTTP Upgrade request immediately.
//   fmt is a printf-style string for extra HTTP request headers; each header
//   must end with "\r\n". The user-supplied fn event handler receives
//   MG_EV_WS_OPEN on handshake success, MG_EV_WS_MSG for messages, and
//   MG_EV_WS_CTL for control frames.
struct mg_connection *mg_ws_connect(struct mg_mgr *, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...);

// Upgrades a server-side HTTP connection to WebSocket.
//
// Example:
//   if (mg_match(hm->uri, mg_str("/websocket"), NULL)) {
//     mg_ws_upgrade(c, hm, NULL);
//   }
// Full examples:
//   tutorials/websocket/websocket-server,
//   tutorials/websocket/json-rpc-over-websocket, tutorials/core/timers
// Related APIs:
//   mg_http_listen(), mg_match(), mg_ws_send(), mg_ws_printf()
// Notes:
//   Call from an MG_EV_HTTP_MSG handler and pass that event's hm. fmt is a
//   printf-style string for extra response headers; each header must end with
//   "\r\n". Fires MG_EV_WS_OPEN immediately on success. Sends HTTP 426 and
//   drains the connection if the request lacks Sec-WebSocket-Key.
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *,
                   const char *fmt, ...);

// Sends one WebSocket frame.
//
// Returns:
//   Number of bytes appended to c->send; 0 if the frame header cannot be
//   appended.
// Example:
//   mg_ws_send(c, "hello", 5, WEBSOCKET_OP_TEXT);
// Full examples:
//   tutorials/websocket/websocket-server, tutorials/websocket/websocket-client,
//   tutorials/core/timers
// Related APIs:
//   mg_ws_connect(), mg_ws_upgrade(), mg_ws_printf(), mg_ws_wrap()
// Notes:
//   op is one of WEBSOCKET_OP_*. Client connections are automatically masked
//   per RFC 6455. On OOM, the return value can be smaller than header + len.
//   Data is appended to c->send and sent by a later mg_mgr_poll() call.
size_t mg_ws_send(struct mg_connection *, const void *buf, size_t len, int op);

// Wraps the last len bytes already in c->send with a WebSocket frame header
// and opcode op. Used internally by mg_ws_printf(); call it directly when you
// have written data into c->send manually and need to frame it.
// Returns c->send.len (total buffer size after the operation).
size_t mg_ws_wrap(struct mg_connection *, size_t len, int op);

// Formats a WebSocket message using printf-style fmt and sends it with
// opcode op (WEBSOCKET_OP_TEXT or WEBSOCKET_OP_BINARY).
// Returns the number of payload bytes written, or 0 on OOM.
size_t mg_ws_printf(struct mg_connection *c, int op, const char *fmt, ...);
size_t mg_ws_vprintf(struct mg_connection *c, int op, const char *fmt,
                     va_list *);
