#pragma once

#include "fmt.h"
#include "json.h"
#include "log.h"
#include "printf.h"
#include "str.h"
#include "ws.h"

#ifndef MG_DASH_MAX_USER_NAME
#define MG_DASH_MAX_USER_NAME 32
#endif

#ifndef MG_HTTP_ADDR
#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define MG_HTTP_ADDR "http://0.0.0.0:8000"
#define MG_HTTPS_ADDR "https://0.0.0.0:8443"
#define MG_MODBUS_ADDR "tcp://0.0.0.0:8502"
#else
#define MG_HTTP_ADDR "http://0.0.0.0:80"
#define MG_HTTPS_ADDR "http://0.0.0.0:443"
#define MG_MODBUS_ADDR "tcp://0.0.0.0:502"
#endif
#endif

#ifndef MG_DASH_MAX_SEND_BUF_SIZE
#define MG_DASH_MAX_SEND_BUF_SIZE (MG_IO_SIZE * 5)
#endif

// C type of a value bound to a dashboard field, see struct mg_field
enum mg_val_type {
  MG_VAL_INT,     // int
  MG_VAL_UINT64,  // uint64_t
  MG_VAL_BOOL,    // bool
  MG_VAL_DBL,     // double
  MG_VAL_STR,     // Zero-terminated string
  MG_VAL_RAW,     // Pre-formatted JSON value, copied verbatim
};

// Dashboard field: binds a JSON key to a C variable for read and/or write
struct mg_field {
  const char *name;       // JSON key
  enum mg_val_type type;  // C type of the value, see enum mg_val_type
  void *value;            // Pointer to the bound variable
  size_t value_size;  // sizeof(*value) for type checking; 0 makes it read-only
};

// Operation passed to a struct mg_field_set read/write/delete/add callback.
// MG_DASH_ADD: fields hold the values of a new element to append; *index is
// not set. Return true to accept and persist it, false to reject (e.g. a cap
// was reached)
enum mg_dash_op { MG_DASH_READ, MG_DASH_WRITE, MG_DASH_DELETE, MG_DASH_ADD };

struct mg_dash;

// Authenticated (or guest) dashboard user
struct mg_dash_user {
  struct mg_dash_user *next;         // Next user in the list
  char name[MG_DASH_MAX_USER_NAME];  // Username
  char token[21];        // Session token, sent to the client as a cookie
  int level;             // Access level, see struct mg_dash::authenticate
  uint64_t expire;       // Session expiration time, see mg_millis()
  struct mg_dash *dash;  // Dashboard this user belongs to
};

// Named group of related dashboard fields, e.g. "settings" or "metrics"
struct mg_field_set {
  const char *name;         // Set name, used as a JSON key
  struct mg_field *fields;  // Zero-terminated array of fields
  bool (*fn)(enum mg_dash_op op,
             struct mg_dash_user *u);  // Read/write/delete/add callback
  int *index;  // Non-NULL: array set. *index is set before fn(READ) call;
               // fn sets *index = -1 to signal end of iteration.
               // Size query: framework sets *index = -1 before fn(READ); fn
               // sets *index = total size
  bool (*get_dir)(const struct mg_dash_user *u, char *buf,
                  size_t len);  // Non-NULL: file array
  struct mg_field_set *next;    // Next set in the list
};

// Custom URI handler, registered with MG_DASH_REGISTER_CUSTOM_HANDLER()
struct mg_dash_custom_handler {
  struct mg_dash_custom_handler *next;  // Next handler in the list
  struct mg_str uri_pattern;            // URI to match, see mg_match()
  mg_event_handler_t handler;           // Event handler function
  void *handler_data;                   // Opaque data for the handler
};

// Dashboard instance: fields, users and handlers served over HTTP and WS
struct mg_dash {
  struct mg_field_set *sets;  // Field sets, see MG_DASH_ADD_FIELD_SET()
  struct mg_dash_custom_handler *custom_handlers;  // Custom URI handlers
  // Validates pass, fills in user, returns access level (<= 0: failure).
  // NULL: no login required, all clients get guest access
  int (*authenticate)(char *user, size_t userlen, const char *pass);
  int session_auto_expiration_seconds;  // Session lifetime, 0: 1 hour default
  struct mg_dash_user *guest;           // Used when authenticate == NULL
  struct mg_fs *upload_fs;  // FS for uploads/listing, default &mg_fs_posix
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

// Dashboard event handler. Pass to mg_http_listen() as the fn argument
void mg_dash_ev_handler(struct mg_connection *c, int ev, void *ev_data);

// Notify WebSocket clients that a field set changed: broadcasts the new
// array size if *set->index < 0, or a single element update otherwise
void mg_dash_send_change(struct mg_mgr *mgr, struct mg_field_set *);

// Default reader for file-backed array sets (set->get_dir != NULL)
// Usage example - this is a complete implemenation of the file manager:
// ```c
// static struct file {
//   int index;
//   char name[64];
//   size_t size;
//   uint64_t checksum;
// } s_file;
// static struct mg_field_set set_files;
//
// static bool get_dir(const struct mg_dash_user *u, char *buf, size_t len) {
//   (void) u;
//   mkdir("/tmp/dashboard", 0755);
//   mg_snprintf(buf, len, "%s", "/tmp/dashboard");
//   return true;
// }
//
// static struct mg_field fields_files[] = {
//     {"name", MG_VAL_STR, s_file.name, sizeof(s_file.name)},
//     {"size", MG_VAL_UINT64, &s_file.size, 0},
//     {"checksum", MG_VAL_UINT64, &s_file.checksum, sizeof(s_file.checksum)},
//     {NULL, MG_VAL_INT, NULL, 0},
// };
//
// static bool files_fn(enum mg_dash_op op, struct mg_dash_user *u) {
//   if (op == MG_DASH_WRITE) return s_uploads_enabled && u->level >= 7;
//   if (op != MG_DASH_READ) return false;
//   if (!mg_dash_dir_read(&set_files, u)) return false;
//   s_file.checksum = 0;  // Mock to show how to set custom fields
//   return true;
// }
//
// static struct mg_field_set set_files = {"files", fields_files, files_fn, &s_file.index, get_dir, NULL};
// ```
bool mg_dash_dir_read(struct mg_field_set *set, struct mg_dash_user *u);

// Helper forward declarations for Mongoose CMSIS pack modules
extern struct mg_mgr g_mgr;
extern void mg_dash_init(struct mg_mgr *);
extern void mg_dash_poll(struct mg_mgr *);
extern void mg_mqtt_init(struct mg_mgr *);
extern void mg_mqtt_poll(struct mg_mgr *);
