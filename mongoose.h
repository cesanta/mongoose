// Copyright (c) 2004-2012 Sergey Lyubka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef MONGOOSE_HEADER_INCLUDED
#define  MONGOOSE_HEADER_INCLUDED

#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct mg_context;     // Handle for the HTTP service itself
struct mg_connection;  // Handle for the individual connection


// This structure contains information about the HTTP request.
struct mg_request_info {
  const char *request_method; // "GET", "POST", etc
  const char *uri;            // URL-decoded URI
  const char *http_version;   // E.g. "1.0", "1.1"
  const char *query_string;   // URL part after '?', not including '?', or NULL
  const char *remote_user;    // Authenticated user, or NULL if no auth used
  long remote_ip;             // Client's IP address
  int remote_port;            // Client's port
  int is_ssl;                 // 1 if SSL-ed, 0 if not
  int num_headers;            // Number of headers
  struct mg_header {
    const char *name;         // HTTP header name
    const char *value;        // HTTP header value
  } http_headers[64];         // Maximum 64 headers
  void *user_data;            // User data pointer passed to mg_start()
  void *ev_data;              // Event-specific data pointer
};


// Various events on which user-defined callback functions are called by Mongoose.
// MG_NEW_REQUEST
  // New HTTP request has arrived from the client.
  // If callback returns non-zero, Mongoose stops handling current request.
typedef int (*mg_new_request_f)(struct mg_connection *conn);

//  MG_REQUEST_COMPLETE
  // Mongoose has finished handling the request.
typedef void (*mg_request_complete_f)(struct mg_connection *conn, int http_status_code);

//  MG_HTTP_ERROR
  // HTTP error must be returned to the client.
  // If callback returns non-zero, Mongoose stops handling error.
typedef int (*mg_http_error_f)(struct mg_connection *conn, int http_status_code);

//  MG_EVENT_LOG
  // Mongoose logs a message.
  // If callback returns non-zero, Mongoose stops handling that event.
typedef int (*mg_event_log_f)(struct mg_connection *conn, const char *log_message);

//  MG_INIT_SSL
  // SSL initialization, sent before certificate setup.
  // If callback returns non-zero, Mongoose does not set up certificates.
  // SSL_CTX is defined elsewhere(???)
typedef struct ssl_ctx_st SSL_CTX;
typedef int (*mg_init_ssl_f)(SSL_CTX *ssl_context);

//  MG_WEBSOCKET_CONNECT
  // Sent on HTTP connect, before websocket handshake.
  // If user callback returns zero, then mongoose proceeds
  // with handshake, otherwise it closes the connection.
typedef int (*mg_websocket_connect_f)(struct mg_connection *conn);

//  MG_WEBSOCKET_READY
  // Handshake has been successfully completed.
typedef void (*mg_websocket_ready_f)(struct mg_connection *conn);

//  MG_WEBSOCKET_MESSAGE
  // Incoming message from the client, data could be read with mg_read().
  // If user callback returns non-zero, mongoose closes the websocket.
typedef int (*mg_websocket_message_f)(struct mg_connection *conn);

//  MG_WEBSOCKET_CLOSE,
  // Client has closed the connection.
typedef void (*mg_websocket_close_f)(struct mg_connection *conn);

//  MG_OPEN_FILE
  // Mongoose tries to open file.
  // If callback returns non-zero, Mongoose will not try to open it, but
  // will use the returned value as a pointer to the file data. This allows
  // for example to serve files from memory.
  // Upon return, callee must set data_len to the length of the data.
  //
  //   if (strcmp(file_name, "foo.txt") == 0) {
  //     *data_len = 4;
  //     return "data";
  //   }
  //   return NULL;
  //
  // Note that this even is sent multiple times during one request. Each
  // time mongoose tries to open or stat the file, this event is sent, e.g.
  // for opening .htpasswd file, stat-ting requested file, opening requested
  // file, etc.
typedef const char * (*mg_open_file_f)(struct mg_connection *conn,
                                       const char *filename, long long *data_len);

//  MG_INIT_LUA,
  // Mongoose initializes Lua server page. Sent only if Lua support is enabled.
struct lua_State;
typedef void (*mg_init_lua_f)(struct mg_connection *conn, struct lua_State *L);


//  MG_UPLOAD
  // Mongoose has uploaded file to a temporary directory.
typedef void (*mg_upload_f)(struct mg_connection *conn, const char *filename);


struct mg_callbacks {
    mg_new_request_f        new_request;
    mg_request_complete_f   request_complete;
    mg_http_error_f         http_error;
    mg_event_log_f          event_log;
    mg_init_ssl_f           init_ssl;
    mg_websocket_connect_f  websocket_connect;
    mg_websocket_ready_f    websocket_ready;
    mg_websocket_message_f  websocket_message;
    mg_websocket_close_f    websocket_close;
    mg_open_file_f          open_file;
    mg_init_lua_f           init_lua;
    mg_upload_f             upload;
};

// Start web server.
//
// Parameters:
//   callback: user defined event handling function or NULL.
//   options: NULL terminated list of option_name, option_value pairs that
//            specify Mongoose configuration parameters.
//
// Side-effects: on UNIX, ignores SIGCHLD and SIGPIPE signals. If custom
//    processing is required for these, signal handlers must be set up
//    after calling mg_start().
//
//
// Example:
//   const char *options[] = {
//     "document_root", "/var/www",
//     "listening_ports", "80,443s",
//     NULL
//   };
//   struct mg_callbacks callbacks;
//   memset(&callbacks, 0, sizeof(mg_callbacks));
//   callbacks.my_new_request = my_new_request;
//   struct mg_context *ctx = mg_start(&callbacks, NULL, options);
//
// Please refer to http://code.google.com/p/mongoose/wiki/MongooseManual
// for the list of valid option and their possible values.
//
// Return:
//   web server context, or NULL on error.
struct mg_context *mg_start(struct mg_callbacks *callback, void *user_data,
                            const char **options);


// Stop the web server.
//
// Must be called last, when an application wants to stop the web server and
// release all associated resources. This function blocks until all Mongoose
// threads are stopped. Context pointer becomes invalid.
void mg_stop(struct mg_context *);


// Get the value of particular configuration parameter.
// The value returned is read-only. Mongoose does not allow changing
// configuration at run time.
// If given parameter name is not valid, NULL is returned. For valid
// names, return value is guaranteed to be non-NULL. If parameter is not
// set, zero-length string is returned.
const char *mg_get_option(const struct mg_context *ctx, const char *name);


// Return array of strings that represent valid configuration options.
// For each option, a short name, long name, and default value is returned.
// Array is NULL terminated.
const char **mg_get_valid_option_names(void);


// Add, edit or delete the entry in the passwords file.
//
// This function allows an application to manipulate .htpasswd files on the
// fly by adding, deleting and changing user records. This is one of the
// several ways of implementing authentication on the server side. For another,
// cookie-based way please refer to the examples/chat.c in the source tree.
//
// If password is not NULL, entry is added (or modified if already exists).
// If password is NULL, entry is deleted.
//
// Return:
//   1 on success, 0 on error.
int mg_modify_passwords_file(const char *passwords_file_name,
                             const char *domain,
                             const char *user,
                             const char *password);


// Return information associated with the request.
struct mg_request_info *mg_get_request_info(struct mg_connection *);


// Send data to the client.
// Return:
//  0   when the connection has been closed
//  -1  on error
//  number of bytes written on success
int mg_write(struct mg_connection *, const void *buf, size_t len);


// Send data to the browser using printf() semantics.
//
// Works exactly like mg_write(), but allows to do message formatting.
// Below are the macros for enabling compiler-specific checks for
// printf-like arguments.

#undef PRINTF_FORMAT_STRING
#if _MSC_VER >= 1400
#include <sal.h>
#if _MSC_VER > 1400
#define PRINTF_FORMAT_STRING(s) _Printf_format_string_ s
#else
#define PRINTF_FORMAT_STRING(s) __format_string s
#endif
#else
#define PRINTF_FORMAT_STRING(s) s
#endif

#ifdef __GNUC__
#define PRINTF_ARGS(x, y) __attribute__((format(printf, x, y)))
#else
#define PRINTF_ARGS(x, y)
#endif

int mg_printf(struct mg_connection *,
              PRINTF_FORMAT_STRING(const char *fmt), ...) PRINTF_ARGS(2, 3);


// Send contents of the entire file together with HTTP headers.
void mg_send_file(struct mg_connection *conn, const char *path);


// Read data from the remote end, return number of bytes read.
int mg_read(struct mg_connection *, void *buf, size_t len);


// Get the value of particular HTTP header.
//
// This is a helper function. It traverses request_info->http_headers array,
// and if the header is present in the array, returns its value. If it is
// not present, NULL is returned.
const char *mg_get_header(const struct mg_connection *, const char *name);


// Get a value of particular form variable.
//
// Parameters:
//   data: pointer to form-uri-encoded buffer. This could be either POST data,
//         or request_info.query_string.
//   data_len: length of the encoded data.
//   var_name: variable name to decode from the buffer
//   dst: destination buffer for the decoded variable
//   dst_len: length of the destination buffer
//
// Return:
//   On success, length of the decoded variable.
//   On error:
//      -1 (variable not found).
//      -2 (destination buffer is NULL, zero length or too small to hold the decoded variable).
//
// Destination buffer is guaranteed to be '\0' - terminated if it is not
// NULL or zero length.
int mg_get_var(const char *data, size_t data_len,
               const char *var_name, char *dst, size_t dst_len);

// Fetch value of certain cookie variable into the destination buffer.
//
// Destination buffer is guaranteed to be '\0' - terminated. In case of
// failure, dst[0] == '\0'. Note that RFC allows many occurrences of the same
// parameter. This function returns only first occurrence.
//
// Return:
//   On success, value length.
//   On error:
//      -1 (either "Cookie:" header is not present at all or the requested parameter is not found).
//      -2 (destination buffer is NULL, zero length or too small to hold the value).
int mg_get_cookie(const struct mg_connection *,
                  const char *cookie_name, char *buf, size_t buf_len);


// Connect to the remote web server.
// Return:
//   On success, valid pointer to the new connection
//   On error, NULL
struct mg_connection *mg_connect(struct mg_context *ctx,
                                 const char *host, int port, int use_ssl);


// Close the connection opened by mg_connect().
void mg_close_connection(struct mg_connection *conn);


// Download given URL to a given file.
//   url: URL to download
//   path: file name where to save the data
//   request_info: pointer to a structure that will hold parsed reply headers
//   buf, bul_len: a buffer for the reply headers
// Return:
//   On error, NULL
//   On success, opened file stream to the downloaded contents. The stream
//   is positioned to the end of the file. It is the user's responsibility
//   to fclose() the opened file stream.
FILE *mg_fetch(struct mg_context *ctx, const char *url, const char *path,
               char *buf, size_t buf_len, struct mg_request_info *request_info);


// File upload functionality. Each uploaded file gets saved into a temporary
// file and MG_UPLOAD event is sent.
// Return number of uploaded files.
int mg_upload(struct mg_connection *conn, const char *destination_dir);


// Convenience function -- create detached thread.
// Return: 0 on success, non-0 on error.
typedef void * (*mg_thread_func_t)(void *);
int mg_start_thread(mg_thread_func_t f, void *p);


// Return builtin mime type for the given file name.
// For unrecognized extensions, "text/plain" is returned.
const char *mg_get_builtin_mime_type(const char *file_name);


// Return Mongoose version.
const char *mg_version(void);


// MD5 hash given strings.
// Buffer 'buf' must be 33 bytes long. Varargs is a NULL terminated list of
// ASCIIz strings. When function returns, buf will contain human-readable
// MD5 hash. Example:
//   char buf[33];
//   mg_md5(buf, "aa", "bb", NULL);
void mg_md5(char buf[33], ...);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MONGOOSE_HEADER_INCLUDED
