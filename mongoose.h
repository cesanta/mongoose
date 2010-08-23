// Copyright (c) 2004-2010 Sergey Lyubka
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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct mg_context;     // Handle for the HTTP service itself
struct mg_connection;  // Handle for the individual connection


// This structure contains full information about the HTTP request.
// It is passed to the user-specified callback function as a parameter.
struct mg_request_info {
  char *request_method;  // "GET", "POST", etc
  char *uri;             // URL-decoded URI
  char *http_version;    // E.g. "1.0", "1.1"
  char *query_string;    // \0 - terminated
  char *remote_user;     // Authenticated user
  char *log_message;     // Mongoose error log message
  long remote_ip;        // Client's IP address
  int remote_port;       // Client's port
  int status_code;       // HTTP status code
  int is_ssl;            // 1 if SSL-ed, 0 if not
  int num_headers;       // Number of headers
  struct mg_header {
    char *name;          // HTTP header name
    char *value;         // HTTP header value
  } http_headers[64];    // Maximum 64 headers
};

// User-defined handler function. It must return MG_SUCCESS or MG_ERROR.
//
// If handler returns MG_SUCCESS, that means that handler has processed the
// request by sending appropriate HTTP reply to the client. Mongoose treats
// the request as served.
//
// If callback returns MG_ERROR, that means that callback has not processed
// the request. Handler must not send any data to the client in this case.
// Mongoose proceeds with request handling as if nothing happened.
//
// NOTE: ssl_password_handler must have the following prototype:
//      int (*)(char *, int, int, void *)
// Refer to OpenSSL documentation for more details.
enum mg_error_t {
  MG_ERROR,
  MG_SUCCESS,
  MG_NOT_FOUND,
  MG_BUFFER_TOO_SMALL
};

typedef enum mg_error_t (*mg_callback_t)(struct mg_connection *,
    const struct mg_request_info *);

// This structure describes Mongoose configuration.
struct mg_config {
  char *document_root;
  char *index_files;
  char *ssl_certificate;
  char *listening_ports;
  char *cgi_extensions;
  char *cgi_interpreter;
  char *cgi_environment;
  char *ssi_extensions;
  char *auth_domain;
  char *protect;
  char *global_passwords_file;
  char *put_delete_passwords_file;
  char *access_log_file;
  char *error_log_file;
  char *acl;
  char *uid;
  char *mime_types;
  char *enable_directory_listing;
  char *num_threads;

  mg_callback_t new_request_handler;
  mg_callback_t http_error_handler;
  mg_callback_t event_log_handler;
  mg_callback_t ssl_password_handler;
};


// Start the web server.
//
// This must be the first function called by the application.
// It creates a serving thread, and returns a context structure that
// can be used to stop the server.
// After calling mg_start(), configuration data must not be changed.
struct mg_context *mg_start(struct mg_config *);


// Stop the web server.
//
// Must be called last, when an application wants to stop the web server and
// release all associated resources. This function blocks until all Mongoose
// threads are stopped. Context pointer becomes invalid.
void mg_stop(struct mg_context *);


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
//  MG_ERROR, MG_SUCCESS
enum mg_error_t mg_modify_passwords_file(struct mg_context *ctx, 
    const char *file_name, const char *user, const char *password);

// Send data to the client.
int mg_write(struct mg_connection *, const void *buf, size_t len);


// Send data to the browser using printf() semantics.
//
// Works exactly like mg_write(), but allows to do message formatting.
// Note that mg_printf() uses internal buffer of size IO_BUF_SIZE
// (8 Kb by default) as temporary message storage for formatting. Do not
// print data that is bigger than that, otherwise it will be truncated.
int mg_printf(struct mg_connection *, const char *fmt, ...);


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
// Either request_info->query_string or read POST data can be scanned.
// mg_get_qsvar() is convenience method to get variable from the query string.
// Destination buffer is guaranteed to be '\0' - terminated. In case of
// failure, dst[0] == '\0'.
//
// Return:
//  MG_SUCCESS    Variable value was successfully copied in the buffer.
//  MG_NOT_FOUND  Requested variable not found.
//  MG_BUFFER_TOO_SMALL  Destination buffer is too small to hold the value.
enum mg_error_t mg_get_var(const char *data, size_t data_len,
    const char *var_name, char *buf, size_t buf_len);
enum mg_error_t mg_get_qsvar(const struct mg_request_info *,
    const char *var_name, char *buf, size_t buf_len);

// Fetch value of certain cookie variable into the destination buffer.
//
// Destination buffer is guaranteed to be '\0' - terminated. In case of
// failure, dst[0] == '\0'. Note that RFC allows many occurences of the same
// parameter. This function returns only first occurance.
//
// Return:
//  MG_SUCCESS    Cookie parameter was successfully copied in the buffer.
//  MG_NOT_FOUND  Either "Cookie:" header is not present at all, or the
//                requested parameter is not found.
//  MG_BUFFER_TOO_SMALL  Destination buffer is too small to hold the value.
enum mg_error_t mg_get_cookie(const struct mg_connection *,
    const char *cookie_name, char *buf, size_t buf_len);


// Return Mongoose version.
const char *mg_version(void);


// MD5 hash given strings.
// Buffer 'buf' must be 33 bytes long. Varargs is a NULL terminated list of
// asciiz strings. When function returns, buf will contain human-readable
// MD5 hash. Example:
//   char buf[33];
//   mg_md5(buf, "aa", "bb", NULL);
void mg_md5(char *buf, ...);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MONGOOSE_HEADER_INCLUDED
