#pragma once
#include "fmt.h"
#include "json.h"

// In-flight JSON-RPC request. Populate and pass to mg_rpc_process() to
// dispatch, or receive one pre-filled in a handler registered with mg_rpc_add().
struct mg_rpc_req {
  struct mg_rpc **head;  // Head of the handler list; allows handlers to call other RPCs
  struct mg_rpc *rpc;    // Handler that matched this request; set by mg_rpc_process()
  mg_pfn_t pfn;          // Output function for the response (e.g. mg_pfn_iobuf)
  void *pfn_data;        // Argument passed to pfn (e.g. pointer to an mg_iobuf)
  void *req_data;        // Arbitrary caller data, not touched by the RPC layer
  struct mg_str frame;   // Raw JSON-RPC frame, e.g. {"id":1,"method":"add","params":[1,2]}
};

// A registered JSON-RPC method handler. Managed by mg_rpc_add/del; do not
// allocate directly. method is a glob pattern matched via mg_match().
struct mg_rpc {
  struct mg_rpc *next;              // Next handler in the linked list
  struct mg_str method;             // Method name glob pattern (heap-allocated copy)
  void (*fn)(struct mg_rpc_req *);  // Handler function called on a match
  void *fn_data;                    // Passed to fn as r->rpc->fn_data
};

// Allocates a handler entry and prepends it to *head. method_pattern is
// copied (mg_strdup). Glob patterns are supported, e.g. "get/*".
// To receive JSON-RPC responses ($.result / $.error), register with method=""
void mg_rpc_add(struct mg_rpc **head, struct mg_str method_pattern,
                void (*handler)(struct mg_rpc_req *), void *handler_data);

// Removes and frees all handlers in *head whose fn matches handler.
// Pass handler=NULL to remove every handler in the list.
void mg_rpc_del(struct mg_rpc **head, void (*handler)(struct mg_rpc_req *));

// Dispatches r->frame to the matching handler. Looks up "$.method" and calls
// the first handler whose pattern matches. If "$.result" or "$.error" is found
// instead (i.e. r->frame is a JSON-RPC response), dispatches to the "" handler.
// Sends a -32601 "not found" error if no handler matches, or -32700 on parse failure.
void mg_rpc_process(struct mg_rpc_req *);

// Sends a JSON-RPC success response: {"id":<id>,"result":<fmt>}.
// fmt is a printf-style format string written to r->pfn; NULL sends "null".
// Does nothing if the request frame has no "id" field (notification).
void mg_rpc_ok(struct mg_rpc_req *, const char *fmt, ...);
void mg_rpc_vok(struct mg_rpc_req *, const char *fmt, va_list *ap);

// Sends a JSON-RPC error response: {"id":<id>,"error":{"code":<code>,"message":<fmt>}}.
// fmt is a printf-style format string; NULL sends "null" as the message.
// The "id" field is omitted if the request had none; the error is always sent.
void mg_rpc_err(struct mg_rpc_req *, int code, const char *fmt, ...);
void mg_rpc_verr(struct mg_rpc_req *, int code, const char *fmt, va_list *);

// Built-in handler: responds with a JSON array of all registered method names.
// Register it as a method handler, e.g. mg_rpc_add(&head, mg_str("rpc.list"), mg_rpc_list, NULL);
void mg_rpc_list(struct mg_rpc_req *r);
