#pragma once

#include "arch.h"

// Helper functions to send JSON-RPC responses over WebSocket. Usage example:
// mg_ws_rpc_success(c, req, "{%m:%d}", MG_ESC("volume"), 42);
// mg_ws_rpc_error(c, req, "file open failed: %d", errno);
void mg_ws_rpc_success(struct mg_connection *c, struct mg_str req,
                       const char *fmt, ...);
void mg_ws_rpc_error(struct mg_connection *c, struct mg_str req,
                     const char *fmt, ...);

// Send a formatted message to all WebSocket clients. Usage example:
// mg_ws_broadcast(mgr, "{%m:%m,%m:%M}", MG_ESC("method"), MG_ESC("state"),
//                 MG_ESC("result"), mg_print_fields, fields, "");
void mg_ws_broadcast(struct mg_mgr *mgr, const char *fmt, ...);

// Interface for serialising C/C++ structures to JSON
// struct mg_field {
//   const char *name;  // "settings.wifi.ssid""
//   const char *type;  // "bool", "int", "double", "string"
//   void *data;        // Optional data pointer, can be used by get/set functions
//   void (*get)(const void *data, void *out);  // Getter function, optional
//   void (*set)(void *data, const void *val);  // Setter function, optional
// };

struct mg_struct;
struct mg_field {
  const char *name;
  const char *type;
  size_t offset;
  const struct mg_struct *sub;
};

struct mg_struct {
  const char *name;
  size_t size;
  struct mg_field *fields;
  void (*get)(void *obj);
  void (*set)(void *obj);
};

// %M printer function for serialising a single field into JSON. Usage example:
// mg_snprintf(buf, len, "%M", mg_print_one_field, &fields[1]);
size_t mg_print_one_field(mg_pfn_t out, void *arg, va_list *ap);

// %M printer function for serialising many fields into JSON. Usage example:
// mg_snprintf(buf, len, "%M", mg_print_fields, fields, "settings."]);
size_t mg_print_fields(mg_pfn_t out, void *arg, va_list *ap);

void mg_get_bool(const void *ctx, void *out);
void mg_get_int(const void *ctx, void *out);
void mg_get_double(const void *ctx, void *out);
void mg_get_string(const void *ctx, void *out);
void mg_set_bool(void *ctx, const void *val);
void mg_set_int(void *ctx, const void *val);
void mg_set_double(void *ctx, const void *val);
void mg_set_string(void *ctx, const void *val);
