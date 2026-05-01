#pragma once

#include "fmt.h"
#include "json.h"
#include "log.h"
#include "printf.h"
#include "str.h"
#include "ws.h"

enum mg_val_type {
  MG_VAL_INT,
  MG_VAL_BOOL,
  MG_VAL_DBL,
  MG_VAL_STR,
  MG_VAL_RAW,
};

struct mg_field {
  const char *name;
  enum mg_val_type type;
  void *value;
  size_t value_size;
};

struct mg_field_set {
  const char *name;
  struct mg_field *fields;
  void (*reader)(void);
  void (*writer)(void);
  int read_level;
  int write_level;
  struct mg_field_set *next;
};

struct mg_dash_custom_handler {
  struct mg_dash_custom_handler *next;
  struct mg_str uri_pattern;
  mg_event_handler_t handler;
  void *handler_data;
};

struct mg_dash_file {
  struct mg_dash_file *next;
  char *name;
  size_t size;
};

struct mg_dash {
  struct mg_field_set *sets;
  struct mg_dash_custom_handler *custom_handlers;
  //struct mg_dash_file *files;
};

#define MG_DASH_ADD_FIELD_SET(dash_, set_) \
  do {                                     \
    (set_)->next = (dash_)->sets;          \
    (dash_)->sets = (set_);                \
  } while (0)

#define MG_DASH_REGISTER_CUSTOM_HANDLER(dash_, uri_, fn_, data_) \
  do {                                                           \
    static struct mg_dash_custom_handler ch_;                    \
    ch_.next = (dash_)->custom_handlers;                         \
    ch_.uri_pattern = mg_str(uri_);                              \
    ch_.handler = (fn_);                                         \
    ch_.handler_data = (data_);                                  \
    (dash_)->custom_handlers = &ch_;                             \
  } while (0)

#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define MG_HTTP_ADDR "http://0.0.0.0:8000"
#define MG_HTTPS_ADDR "https://0.0.0.0:8443"
#define MG_MODBUS_ADDR "tcp://0.0.0.0:8502"
#else
#define MG_HTTP_ADDR "http://0.0.0.0:80"
#define MG_HTTPS_ADDR "http://0.0.0.0:443"
#define MG_MODBUS_ADDR "tcp://0.0.0.0:502"
#endif

#ifndef MG_DASH_MAX_SEND_BUF_SIZE
#define MG_DASH_MAX_SEND_BUF_SIZE (MG_IO_SIZE * 5)
#endif

// File manager
extern struct mg_dash_file *mg_dash_files;
void mg_dash_file_add(struct mg_str name, size_t size);
void mg_dash_file_del(struct mg_str name);

void mg_dash_ev_handler(struct mg_connection *c, int ev, void *ev_data);
void mg_dash_send_change(struct mg_mgr *mgr, struct mg_field_set *);

// Helper forward declarations for Mongoose CMSIS pack modules
extern struct mg_mgr g_mgr;
extern void mg_dash_init(struct mg_mgr *);
extern void mg_dash_poll(struct mg_mgr *);
extern void mg_mqtt_init(struct mg_mgr *);
extern void mg_mqtt_poll(struct mg_mgr *);
