#pragma once
#include "fmt.h"
#include "json.h"

// JSON-RPC request descriptor
struct mg_rpc_req {
  void **head;          // List of all RPC handlers
  mg_pfn_t pfn;         // Response printing function
  void *pfn_data;       // Response printing function data
  void *handler_data;   // Endpoint handler data
  void *process_data;   // Arbitrary user data
  struct mg_str frame;  // Request, e.g. {"id":1,"method":"add","params":[1,2]}
};

void mg_rpc_add(void **head, struct mg_str method_pattern,
                void (*handler)(struct mg_rpc_req *), void *handler_data);
void mg_rpc_free(void **head);
void mg_rpc_process(struct mg_rpc_req *);

// Helper functions to print result or error frame
void mg_rpc_ok(struct mg_rpc_req *, const char *fmt, ...);
void mg_rpc_vok(struct mg_rpc_req *, const char *fmt, va_list *ap);
void mg_rpc_err(struct mg_rpc_req *, int code, const char *fmt, ...);
void mg_rpc_verr(struct mg_rpc_req *, int code, const char *fmt, va_list *);
void mg_rpc_list(struct mg_rpc_req *r);
