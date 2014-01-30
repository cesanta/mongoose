// Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>
// Copyright (c) 2013-2014 Cesanta Software Limited
// All rights reserved
//
// This library is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this library under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this library under a commercial
// license, as set out in <http://cesanta.com/>.

#undef UNICODE                  // Use ANSI WinAPI functions
#undef _UNICODE                 // Use multibyte encoding on Windows
#define _MBCS                   // Use multibyte encoding on Windows
#define _INTEGRAL_MAX_BITS 64   // Enable _stati64() on Windows
#define _CRT_SECURE_NO_WARNINGS // Disable deprecation warning in VS2005+
#undef WIN32_LEAN_AND_MEAN      // Let windows.h always include winsock2.h
#define _XOPEN_SOURCE 600       // For flockfile() on Linux
#define __STDC_FORMAT_MACROS    // <inttypes.h> wants this for C++
#define __STDC_LIMIT_MACROS     // C++ wants that for INT64_MAX
#define _LARGEFILE_SOURCE       // Enable fseeko() and ftello() functions
#define _FILE_OFFSET_BITS 64    // Enable 64-bit file offsets

#ifdef _MSC_VER
#pragma warning (disable : 4127)  // FD_SET() emits warning, disable it
#pragma warning (disable : 4204)  // missing c99 support
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>    // For _beginthread
#include <io.h>         // For _lseeki64
#include <direct.h>     // For _mkdir
typedef int socklen_t;
#if !defined(__MINGW32__) || !defined(_PID_T_) || defined(_NO_OLDNAMES)
typedef HANDLE pid_t;
#endif
typedef SOCKET sock_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned __int64 uint64_t;
typedef __int64   int64_t;
typedef CRITICAL_SECTION mutex_t;
typedef struct _stati64 file_stat_t;
#pragma comment(lib, "ws2_32.lib")
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define INT64_FMT  "I64d"
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#define mutex_init(x) InitializeCriticalSection(x)
#define mutex_destroy(x) DeleteCriticalSection(x)
#define mutex_lock(x) EnterCriticalSection(x)
#define mutex_unlock(x) LeaveCriticalSection(x)
#define get_thread_id() ((unsigned long) GetCurrentThreadId())
#ifndef S_ISDIR
#define S_ISDIR(x) ((x) & _S_IFDIR)
#endif
#define sleep(x) Sleep((x) * 1000)
#define stat(x, y) mg_stat((x), (y))
#define fopen(x, y) mg_fopen((x), (y))
#define open(x, y) mg_open((x), (y))
#define lseek(x, y, z) _lseeki64((x), (y), (z))
#define mkdir(x, y) _mkdir(x)
#define to64(x) _atoi64(x)
#define flockfile(x)
#define funlockfile(x)
#ifndef va_copy
#define va_copy(x,y) x = y
#endif // MINGW #defines va_copy
#ifndef __func__
#define STRX(x) #x
#define STR(x) STRX(x)
#define __func__ __FILE__ ":" STR(__LINE__)
#endif
#else
#include <dirent.h>
#include <inttypes.h>
#include <pthread.h>
#include <pwd.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>  // For inet_pton() when MONGOOSE_USE_IPV6 is defined
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#define closesocket(x) close(x)
typedef int sock_t;
typedef pthread_mutex_t mutex_t;
typedef struct stat file_stat_t;
#define mutex_init(x) pthread_mutex_init(x, NULL)
#define mutex_destroy(x) pthread_mutex_destroy(x)
#define mutex_lock(x) pthread_mutex_lock(x)
#define mutex_unlock(x) pthread_mutex_unlock(x)
#define get_thread_id() ((unsigned long) pthread_self())
#define INVALID_SOCKET ((sock_t) -1)
#define INT64_FMT PRId64
#define to64(x) strtoll(x, NULL, 10)
#define __cdecl
#define O_BINARY 0
#endif

#ifdef MONGOOSE_USE_SSL
#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <openssl/ssl.h>
#endif

#include "mongoose.h"

struct ll { struct ll *prev, *next; };
#define LINKED_LIST_INIT(N)  ((N)->next = (N)->prev = (N))
#define LINKED_LIST_DECLARE_AND_INIT(H)  struct ll H = { &H, &H }
#define LINKED_LIST_ENTRY(P,T,N)  ((T *)((char *)(P) - offsetof(T, N)))
#define LINKED_LIST_IS_EMPTY(N)  ((N)->next == (N))
#define LINKED_LIST_FOREACH(H,N,T) \
  for (N = (H)->next, T = (N)->next; N != (H); N = (T), T = (N)->next)
#define LINKED_LIST_ADD_TO_FRONT(H,N) do { ((H)->next)->prev = (N); \
  (N)->next = ((H)->next);  (N)->prev = (H); (H)->next = (N); } while (0)
#define LINKED_LIST_ADD_TO_TAIL(H,N) do { ((H)->prev)->next = (N); \
  (N)->prev = ((H)->prev); (N)->next = (H); (H)->prev = (N); } while (0)
#define LINKED_LIST_REMOVE(N) do { ((N)->next)->prev = ((N)->prev); \
  ((N)->prev)->next = ((N)->next); LINKED_LIST_INIT(N); } while (0)

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define MAX_REQUEST_SIZE 16384
#define IOBUF_SIZE 8192
#define MAX_PATH_SIZE 8192
#define LUA_SCRIPT_PATTERN "**.lp$"
#define DEFAULT_CGI_PATTERN "**.cgi$|**.pl$|**.php$"
#define CGI_ENVIRONMENT_SIZE 4096
#define MAX_CGI_ENVIR_VARS 64
#define ENV_EXPORT_TO_CGI "MONGOOSE_CGI"
#define PASSWORDS_FILE_NAME ".htpasswd"

#ifndef MONGOOSE_USE_WEBSOCKET_PING_INTERVAL
#define MONGOOSE_USE_WEBSOCKET_PING_INTERVAL 5
#endif

// Extra HTTP headers to send in every static file reply
#if !defined(MONGOOSE_USE_EXTRA_HTTP_HEADERS)
#define MONGOOSE_USE_EXTRA_HTTP_HEADERS ""
#endif

#ifndef MONGOOSE_USE_POST_SIZE_LIMIT
#define MONGOOSE_USE_POST_SIZE_LIMIT 0
#endif

#ifndef MONGOOSE_USE_IDLE_TIMEOUT_SECONDS
#define MONGOOSE_USE_IDLE_TIMEOUT_SECONDS 30
#endif

#ifdef MONGOOSE_ENABLE_DEBUG
#define DBG(x) do { printf("%-20s ", __func__); printf x; putchar('\n'); \
  fflush(stdout); } while(0)
#else
#define DBG(x)
#endif

#ifdef MONGOOSE_NO_FILESYSTEM
#define MONGOOSE_NO_AUTH
#define MONGOOSE_NO_CGI
#define MONGOOSE_NO_DAV
#define MONGOOSE_NO_DIRECTORY_LISTING
#define MONGOOSE_NO_LOGGING
#endif

union socket_address {
  struct sockaddr sa;
  struct sockaddr_in sin;
#ifdef MONGOOSE_USE_IPV6
  struct sockaddr_in6 sin6;
#endif
};

struct vec {
  const char *ptr;
  int len;
};

// For directory listing and WevDAV support
struct dir_entry {
  struct connection *conn;
  char *file_name;
  file_stat_t st;
};

// NOTE(lsm): this enum shoulds be in sync with the config_options.
enum {
  ACCESS_CONTROL_LIST,
#ifndef MONGOOSE_NO_FILESYSTEM
  ACCESS_LOG_FILE,
#ifndef MONGOOSE_NO_AUTH
  AUTH_DOMAIN,
#endif
#ifndef MONGOOSE_NO_CGI
  CGI_INTERPRETER,
  CGI_PATTERN,
#endif
#ifndef MONGOOSE_NO_DAV
  DAV_AUTH_FILE,
#endif
  DOCUMENT_ROOT,
#ifndef MONGOOSE_NO_DIRECTORY_LISTING
  ENABLE_DIRECTORY_LISTING,
#endif
#endif
  EXTRA_MIME_TYPES,
#if !defined(MONGOOSE_NO_FILESYSTEM) && !defined(MONGOOSE_NO_AUTH)
  GLOBAL_AUTH_FILE,
#endif
  HIDE_FILES_PATTERN,
#ifndef MONGOOSE_NO_FILESYSTEM
  INDEX_FILES,
#endif
  LISTENING_PORT,
#ifndef _WIN32
  RUN_AS_USER,
#endif
#ifdef MONGOOSE_USE_SSL
  SSL_CERTIFICATE,
#endif
  URL_REWRITES,
  NUM_OPTIONS
};

static const char *static_config_options[] = {
  "access_control_list", NULL,
#ifndef MONGOOSE_NO_FILESYSTEM
  "access_log_file", NULL,
#ifndef MONGOOSE_NO_AUTH
  "auth_domain", "mydomain.com",
#endif
#ifndef MONGOOSE_NO_CGI
  "cgi_interpreter", NULL,
  "cgi_pattern", DEFAULT_CGI_PATTERN,
#endif
#ifndef MONGOOSE_NO_DAV
  "dav_auth_file", NULL,
#endif
  "document_root",  NULL,
#ifndef MONGOOSE_NO_DIRECTORY_LISTING
  "enable_directory_listing", "yes",
#endif
#endif
  "extra_mime_types", NULL,
#if !defined(MONGOOSE_NO_FILESYSTEM) && !defined(MONGOOSE_NO_AUTH)
  "global_auth_file", NULL,
#endif
  "hide_files_patterns", NULL,
#ifndef MONGOOSE_NO_FILESYSTEM
  "index_files","index.html,index.htm,index.cgi,index.php,index.lp",
#endif
  "listening_port", NULL,
#ifndef _WIN32
  "run_as_user", NULL,
#endif
#ifdef MONGOOSE_USE_SSL
  "ssl_certificate", NULL,
#endif
  "url_rewrites", NULL,
  NULL
};

struct mg_server {
  sock_t listening_sock;
  union socket_address lsa;   // Listening socket address
  struct ll active_connections;
  mg_handler_t request_handler;
  mg_handler_t error_handler;
  mg_handler_t auth_handler;
  char *config_options[NUM_OPTIONS];
  char local_ip[48];
  void *server_data;
#ifdef MONGOOSE_USE_SSL
  SSL_CTX *ssl_ctx;            // Server SSL context
  SSL_CTX *client_ssl_ctx;     // Client SSL context
#endif
  sock_t ctl[2];    // Control socketpair. Used to wake up from select() call
};

// Expandable IO buffer
struct iobuf {
  char *buf;    // Buffer that holds the data
  int size;     // Buffer size
  int len;      // Number of bytes currently in a buffer
};

// Local endpoint representation
union endpoint {
  int fd;                   // Opened regular local file
  sock_t cgi_sock;          // CGI socket
  void *ssl;                // SSL descriptor
};

enum endpoint_type { EP_NONE, EP_FILE, EP_CGI, EP_USER, EP_PUT, EP_CLIENT };
enum connection_flags {
  CONN_CLOSE = 1,           // Connection must be closed at the end of the poll
  CONN_SPOOL_DONE = 2,        // All data has been buffered for sending
  CONN_SSL_HANDS_SHAKEN = 4,  // SSL handshake has completed. Only for SSL
  CONN_HEADERS_SENT = 8,      // User callback has sent HTTP headers
  CONN_BUFFER = 16,           // CGI only. Holds data send until CGI prints
                              // all HTTP headers
  CONN_CONNECTING = 32,       // HTTP client is doing non-blocking connect()
  CONN_LONG_RUNNING = 64      // Long-running URI handlers
};

struct connection {
  struct mg_connection mg_conn;   // XXX: Must be first
  struct ll link;                 // Linkage to server->active_connections
  struct mg_server *server;
  sock_t client_sock;             // Connected client
  struct iobuf local_iobuf;
  struct iobuf remote_iobuf;
  union endpoint endpoint;
  enum endpoint_type endpoint_type;
  time_t birth_time;
  time_t last_activity_time;
  char *path_info;
  char *request;
  int64_t num_bytes_sent; // Total number of bytes sent
  int64_t cl;             // Reply content length, for Range support
  int request_len;  // Request length, including last \r\n after last header
  int flags;        // CONN_* flags: CONN_CLOSE, CONN_SPOOL_DONE, etc
  mg_handler_t handler;  // Callback for HTTP client
#ifdef MONGOOSE_USE_SSL
  SSL *ssl;        // SSL descriptor
#endif
};

static void open_local_endpoint(struct connection *conn, int skip_user);
static void close_local_endpoint(struct connection *conn);

static const struct {
  const char *extension;
  size_t ext_len;
  const char *mime_type;
} static_builtin_mime_types[] = {
  {".html", 5, "text/html"},
  {".htm", 4, "text/html"},
  {".shtm", 5, "text/html"},
  {".shtml", 6, "text/html"},
  {".css", 4, "text/css"},
  {".js",  3, "application/x-javascript"},
  {".ico", 4, "image/x-icon"},
  {".gif", 4, "image/gif"},
  {".jpg", 4, "image/jpeg"},
  {".jpeg", 5, "image/jpeg"},
  {".png", 4, "image/png"},
  {".svg", 4, "image/svg+xml"},
  {".txt", 4, "text/plain"},
  {".torrent", 8, "application/x-bittorrent"},
  {".wav", 4, "audio/x-wav"},
  {".mp3", 4, "audio/x-mp3"},
  {".mid", 4, "audio/mid"},
  {".m3u", 4, "audio/x-mpegurl"},
  {".ogg", 4, "application/ogg"},
  {".ram", 4, "audio/x-pn-realaudio"},
  {".xml", 4, "text/xml"},
  {".json",  5, "text/json"},
  {".xslt", 5, "application/xml"},
  {".xsl", 4, "application/xml"},
  {".ra",  3, "audio/x-pn-realaudio"},
  {".doc", 4, "application/msword"},
  {".exe", 4, "application/octet-stream"},
  {".zip", 4, "application/x-zip-compressed"},
  {".xls", 4, "application/excel"},
  {".tgz", 4, "application/x-tar-gz"},
  {".tar", 4, "application/x-tar"},
  {".gz",  3, "application/x-gunzip"},
  {".arj", 4, "application/x-arj-compressed"},
  {".rar", 4, "application/x-arj-compressed"},
  {".rtf", 4, "application/rtf"},
  {".pdf", 4, "application/pdf"},
  {".swf", 4, "application/x-shockwave-flash"},
  {".mpg", 4, "video/mpeg"},
  {".webm", 5, "video/webm"},
  {".mpeg", 5, "video/mpeg"},
  {".mov", 4, "video/quicktime"},
  {".mp4", 4, "video/mp4"},
  {".m4v", 4, "video/x-m4v"},
  {".asf", 4, "video/x-ms-asf"},
  {".avi", 4, "video/x-msvideo"},
  {".bmp", 4, "image/bmp"},
  {".ttf", 4, "application/x-font-ttf"},
  {NULL,  0, NULL}
};

#ifndef MONGOOSE_NO_THREADS
void *mg_start_thread(void *(*f)(void *), void *p) {
#ifdef _WIN32
  return (void *) _beginthread((void (__cdecl *)(void *)) f, 0, p);
#else
  pthread_t thread_id = (pthread_t) 0;
  pthread_attr_t attr;

  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if MONGOOSE_USE_STACK_SIZE > 1
  (void) pthread_attr_setstacksize(&attr, MONGOOSE_USE_STACK_SIZE);
#endif

  pthread_create(&thread_id, &attr, f, p);
  pthread_attr_destroy(&attr);

  return (void *) thread_id;
#endif
}
#endif  // MONGOOSE_NO_THREADS

#ifdef _WIN32
// Encode 'path' which is assumed UTF-8 string, into UNICODE string.
// wbuf and wbuf_len is a target buffer and its length.
static void to_wchar(const char *path, wchar_t *wbuf, size_t wbuf_len) {
  char buf[MAX_PATH_SIZE * 2], buf2[MAX_PATH_SIZE * 2], *p;

  strncpy(buf, path, sizeof(buf));
  buf[sizeof(buf) - 1] = '\0';

  // Trim trailing slashes
  p = buf + strlen(buf) - 1;
  while (p > buf && p[0] == '\\' || p[0] == '/') *p-- = '\0';
  //change_slashes_to_backslashes(buf);

  // Convert to Unicode and back. If doubly-converted string does not
  // match the original, something is fishy, reject.
  memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int) wbuf_len);
  WideCharToMultiByte(CP_UTF8, 0, wbuf, (int) wbuf_len, buf2, sizeof(buf2),
                      NULL, NULL);
  if (strcmp(buf, buf2) != 0) {
    wbuf[0] = L'\0';
  }
}

static int mg_stat(const char *path, file_stat_t *st) {
  wchar_t wpath[MAX_PATH_SIZE];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  DBG(("[%ls] -> %d", wpath, _wstati64(wpath, st)));
  return _wstati64(wpath, st);
}

static FILE *mg_fopen(const char *path, const char *mode) {
  wchar_t wpath[MAX_PATH_SIZE], wmode[10];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  to_wchar(mode, wmode, ARRAY_SIZE(wmode));
  return _wfopen(wpath, wmode);
}

static int mg_open(const char *path, int flag) {
  wchar_t wpath[MAX_PATH_SIZE];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  return _wopen(wpath, flag);
}
#endif

static void set_close_on_exec(int fd) {
#ifdef _WIN32
  (void) SetHandleInformation((HANDLE) fd, HANDLE_FLAG_INHERIT, 0);
#else
  fcntl(fd, F_SETFD, FD_CLOEXEC);
#endif
}

static void set_non_blocking_mode(sock_t sock) {
#ifdef _WIN32
  unsigned long on = 1;
  ioctlsocket(sock, FIONBIO, &on);
#else
  int flags = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif
}

// A helper function for traversing a comma separated list of values.
// It returns a list pointer shifted to the next value, or NULL if the end
// of the list found.
// Value is stored in val vector. If value has form "x=y", then eq_val
// vector is initialized to point to the "y" part, and val vector length
// is adjusted to point only to "x".
static const char *next_option(const char *list, struct vec *val,
                               struct vec *eq_val) {
  if (list == NULL || *list == '\0') {
    // End of the list
    list = NULL;
  } else {
    val->ptr = list;
    if ((list = strchr(val->ptr, ',')) != NULL) {
      // Comma found. Store length and shift the list ptr
      val->len = list - val->ptr;
      list++;
    } else {
      // This value is the last one
      list = val->ptr + strlen(val->ptr);
      val->len = list - val->ptr;
    }

    if (eq_val != NULL) {
      // Value has form "x=y", adjust pointers and lengths
      // so that val points to "x", and eq_val points to "y".
      eq_val->len = 0;
      eq_val->ptr = (const char *) memchr(val->ptr, '=', val->len);
      if (eq_val->ptr != NULL) {
        eq_val->ptr++;  // Skip over '=' character
        eq_val->len = val->ptr + val->len - eq_val->ptr;
        val->len = (eq_val->ptr - val->ptr) - 1;
      }
    }
  }

  return list;
}

static int spool(struct iobuf *io, const void *buf, int len) {
  static const double mult = 1.2;
  char *p = NULL;
  int new_len = 0;

  assert(io->len >= 0);
  assert(io->len <= io->size);

  //DBG(("1. %d %d %d", len, io->len, io->size));
  if (len <= 0) {
  } else if ((new_len = io->len + len) < io->size) {
    memcpy(io->buf + io->len, buf, len);
    io->len = new_len;
  } else if ((p = (char *) realloc(io->buf, (int) (new_len * mult))) != NULL) {
    io->buf = p;
    memcpy(io->buf + io->len, buf, len);
    io->len = new_len;
    io->size = (int) (new_len * mult);
  } else {
    len = 0;
  }
  //DBG(("%d %d %d", len, io->len, io->size));

  return len;
}

// Like snprintf(), but never returns negative value, or a value
// that is larger than a supplied buffer.
static int mg_vsnprintf(char *buf, size_t buflen, const char *fmt, va_list ap) {
  int n;
  if (buflen < 1) return 0;
  n = vsnprintf(buf, buflen, fmt, ap);
  if (n < 0) {
    n = 0;
  } else if (n >= (int) buflen) {
    n = (int) buflen - 1;
  }
  buf[n] = '\0';
  return n;
}

static int mg_snprintf(char *buf, size_t buflen, const char *fmt, ...) {
  va_list ap;
  int n;
  va_start(ap, fmt);
  n = mg_vsnprintf(buf, buflen, fmt, ap);
  va_end(ap);
  return n;
}

// Check whether full request is buffered. Return:
//   -1  if request is malformed
//    0  if request is not yet fully buffered
//   >0  actual request length, including last \r\n\r\n
static int get_request_len(const char *s, int buf_len) {
  const unsigned char *buf = (unsigned char *) s;
  int i;

  for (i = 0; i < buf_len; i++) {
    // Control characters are not allowed but >=128 are.
    // Abort scan as soon as one malformed character is found.
    if (!isprint(buf[i]) && buf[i] != '\r' && buf[i] != '\n' && buf[i] < 128) {
      return -1;
    } else if (buf[i] == '\n' && i + 1 < buf_len && buf[i + 1] == '\n') {
      return i + 2;
    } else if (buf[i] == '\n' && i + 2 < buf_len && buf[i + 1] == '\r' &&
               buf[i + 2] == '\n') {
      return i + 3;
    }
  }

  return 0;
}

// Skip the characters until one of the delimiters characters found.
// 0-terminate resulting word. Skip the rest of the delimiters if any.
// Advance pointer to buffer to the next word. Return found 0-terminated word.
static char *skip(char **buf, const char *delimiters) {
  char *p, *begin_word, *end_word, *end_delimiters;

  begin_word = *buf;
  end_word = begin_word + strcspn(begin_word, delimiters);
  end_delimiters = end_word + strspn(end_word, delimiters);

  for (p = end_word; p < end_delimiters; p++) {
    *p = '\0';
  }

  *buf = end_delimiters;

  return begin_word;
}

// Parse HTTP headers from the given buffer, advance buffer to the point
// where parsing stopped.
static void parse_http_headers(char **buf, struct mg_connection *ri) {
  size_t i;

  for (i = 0; i < ARRAY_SIZE(ri->http_headers); i++) {
    ri->http_headers[i].name = skip(buf, ": ");
    ri->http_headers[i].value = skip(buf, "\r\n");
    if (ri->http_headers[i].name[0] == '\0')
      break;
    ri->num_headers = i + 1;
  }
}

static const char *status_code_to_str(int status_code) {
  switch (status_code) {
    case 200: return "OK";
    case 201: return "Created";
    case 204: return "No Content";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 304: return "Not Modified";
    case 400: return "Bad Request";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 409: return "Conflict";
    case 411: return "Length Required";
    case 413: return "Request Entity Too Large";
    case 415: return "Unsupported Media Type";
    case 423: return "Locked";
    case 500: return "Server Error";
    case 501: return "Not Implemented";
    default:  return "Server Error";
  }
}

static void send_http_error(struct connection *conn, int code,
                            const char *fmt, ...) {
  const char *message = status_code_to_str(code);
  const char *rewrites = conn->server->config_options[URL_REWRITES];
  char headers[200], body[200];
  struct vec a, b;
  va_list ap;
  int body_len, headers_len, match_code;

  conn->mg_conn.status_code = code;

  // Invoke error handler if it is set
  if (conn->server->error_handler != NULL &&
      conn->server->error_handler(&conn->mg_conn) == MG_ERROR_PROCESSED) {
    close_local_endpoint(conn);
    return;
  }

  // Handle error code rewrites
  while ((rewrites = next_option(rewrites, &a, &b)) != NULL) {
    if ((match_code = atoi(a.ptr)) > 0 && match_code == code) {
      conn->mg_conn.status_code = 302;
      mg_printf(&conn->mg_conn, "HTTP/1.1 %d Moved\r\n"
                "Location: %.*s?code=%d&orig_uri=%s\r\n\r\n",
                conn->mg_conn.status_code, b.len, b.ptr, code,
                conn->mg_conn.uri);
      close_local_endpoint(conn);
      return;
    }
  }

  body_len = mg_snprintf(body, sizeof(body), "%d %s\n", code, message);
  if (fmt != NULL) {
    va_start(ap, fmt);
    body_len += mg_vsnprintf(body + body_len, sizeof(body) - body_len, fmt, ap);
    va_end(ap);
  }
  if (code >= 300 && code <= 399) {
    // 3xx errors do not have body
    body_len = 0;
  }
  headers_len = mg_snprintf(headers, sizeof(headers),
                            "HTTP/1.1 %d %s\r\nContent-Length: %d\r\n"
                            "Content-Type: text/plain\r\n\r\n",
                            code, message, body_len);
  spool(&conn->remote_iobuf, headers, headers_len);
  spool(&conn->remote_iobuf, body, body_len);
  close_local_endpoint(conn);  // This will write to the log file
}

// Print message to buffer. If buffer is large enough to hold the message,
// return buffer. If buffer is to small, allocate large enough buffer on heap,
// and return allocated buffer.
static int alloc_vprintf(char **buf, size_t size, const char *fmt, va_list ap) {
  va_list ap_copy;
  int len;

  // Windows is not standard-compliant, and vsnprintf() returns -1 if
  // buffer is too small. Also, older versions of msvcrt.dll do not have
  // _vscprintf().  However, if size is 0, vsnprintf() behaves correctly.
  // Therefore, we make two passes: on first pass, get required message length.
  // On second pass, actually print the message.
  va_copy(ap_copy, ap);
  len = vsnprintf(NULL, 0, fmt, ap_copy);

  if (len > (int) size &&
      (size = len + 1) > 0 &&
      (*buf = (char *) malloc(size)) == NULL) {
    len = -1;  // Allocation failed, mark failure
  } else {
    va_copy(ap_copy, ap);
    vsnprintf(*buf, size, fmt, ap_copy);
  }

  return len;
}

static void write_chunk(struct connection *conn, const char *buf, int len) {
  char chunk_size[50];
  int n = mg_snprintf(chunk_size, sizeof(chunk_size), "%X\r\n", len);
  spool(&conn->remote_iobuf, chunk_size, n);
  spool(&conn->remote_iobuf, buf, len);
  spool(&conn->remote_iobuf, "\r\n", 2);
}

int mg_vprintf(struct mg_connection *conn, const char *fmt, va_list ap,
               int chunked) {
  char mem[IOBUF_SIZE], *buf = mem;
  int len;

  if ((len = alloc_vprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
    if (chunked) {
      write_chunk((struct connection *) conn, buf, len);
    } else {
      len = mg_write(conn, buf, (size_t) len);
    }
  }
  if (buf != mem && buf != NULL) {
    free(buf);
  }

  return len;
}

int mg_printf(struct mg_connection *conn, const char *fmt, ...) {
  int len;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vprintf(conn, fmt, ap, 0);
  va_end(ap);
  return len;
}

static int mg_socketpair(sock_t sp[2]) {
  struct sockaddr_in sa;
  sock_t sock, ret = -1;
  socklen_t len = sizeof(sa);

  sp[0] = sp[1] = INVALID_SOCKET;

  (void) memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(0);
  sa.sin_addr.s_addr = htonl(0x7f000001);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET &&
      !bind(sock, (struct sockaddr *) &sa, len) &&
      !listen(sock, 1) &&
      !getsockname(sock, (struct sockaddr *) &sa, &len) &&
      (sp[0] = socket(AF_INET, SOCK_STREAM, 6)) != -1 &&
      !connect(sp[0], (struct sockaddr *) &sa, len) &&
      (sp[1] = accept(sock,(struct sockaddr *) &sa, &len)) != INVALID_SOCKET) {
    set_close_on_exec(sp[0]);
    set_close_on_exec(sp[1]);
    ret = 0;
  } else {
    if (sp[0] != INVALID_SOCKET) closesocket(sp[0]);
    if (sp[1] != INVALID_SOCKET) closesocket(sp[1]);
    sp[0] = sp[1] = INVALID_SOCKET;
  }
  closesocket(sock);

  return ret;
}

static int is_error(int n) {
  return n == 0 ||
    (n < 0 && errno != EINTR && errno != EINPROGRESS &&
     errno != EAGAIN && errno != EWOULDBLOCK
#ifdef _WIN32
     && WSAGetLastError() != WSAEINTR && WSAGetLastError() != WSAEWOULDBLOCK
#endif
    );
}

static void discard_leading_iobuf_bytes(struct iobuf *io, int n) {
  if (n >= 0 && n <= io->len) {
    memmove(io->buf, io->buf + n, io->len - n);
    io->len -= n;
  }
}

#ifndef MONGOOSE_NO_CGI
#ifdef _WIN32
struct threadparam {
  sock_t s;
  HANDLE hPipe;
};

static int wait_until_ready(sock_t sock, int for_read) {
  fd_set set;
  FD_ZERO(&set);
  FD_SET(sock, &set);
  select(sock + 1, for_read ? &set : 0, for_read ? 0 : &set, 0, 0);
  return 1;
}

static void *push_to_stdin(void *arg) {
  struct threadparam *tp = arg;
  int n, sent, stop = 0;
  DWORD k;
  char buf[IOBUF_SIZE];

  while (!stop && wait_until_ready(tp->s, 1) &&
         (n = recv(tp->s, buf, sizeof(buf), 0)) > 0) {
    if (n == -1 && GetLastError() == WSAEWOULDBLOCK) continue;
    for (sent = 0; !stop && sent < n; sent += k) {
      if (!WriteFile(tp->hPipe, buf + sent, n - sent, &k, 0)) stop = 1;
    }
  }
  DBG(("%s", "FORWARED EVERYTHING TO CGI"));
  CloseHandle(tp->hPipe);
  free(tp);
  _endthread();
  return NULL;
}

static void *pull_from_stdout(void *arg) {
  struct threadparam *tp = arg;
  int k, stop = 0;
  DWORD n, sent;
  char buf[IOBUF_SIZE];

  while (!stop && ReadFile(tp->hPipe, buf, sizeof(buf), &n, NULL)) {
    for (sent = 0; !stop && sent < n; sent += k) {
      if (wait_until_ready(tp->s, 0) &&
          (k = send(tp->s, buf + sent, n - sent, 0)) <= 0) stop = 1;
    }
  }
  DBG(("%s", "EOF FROM CGI"));
  CloseHandle(tp->hPipe);
  shutdown(tp->s, 2);  // Without this, IO thread may get truncated data
  closesocket(tp->s);
  free(tp);
  _endthread();
  return NULL;
}

static void spawn_stdio_thread(sock_t sock, HANDLE hPipe,
                               void *(*func)(void *)) {
  struct threadparam *tp = malloc(sizeof(*tp));
  if (tp != NULL) {
    tp->s = sock;
    tp->hPipe = hPipe;
    mg_start_thread(func, tp);
  }
}

static void abs_path(const char *utf8_path, char *abs_path, size_t len) {
  wchar_t buf[MAX_PATH_SIZE], buf2[MAX_PATH_SIZE];
  to_wchar(utf8_path, buf, ARRAY_SIZE(buf));
  GetFullPathNameW(buf, ARRAY_SIZE(buf2), buf2, NULL);
  WideCharToMultiByte(CP_UTF8, 0, buf2, wcslen(buf2) + 1, abs_path, len, 0, 0);
}

static pid_t start_process(char *interp, const char *cmd, const char *env,
                           const char *envp[], const char *dir, sock_t sock) {
  STARTUPINFOW si = {0};
  PROCESS_INFORMATION pi = {0};
  HANDLE a[2], b[2], me = GetCurrentProcess();
  wchar_t wcmd[MAX_PATH_SIZE], full_dir[MAX_PATH_SIZE];
  char buf[MAX_PATH_SIZE], buf4[MAX_PATH_SIZE], buf5[MAX_PATH_SIZE],
       cmdline[MAX_PATH_SIZE], *p;
  DWORD flags = DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS;
  FILE *fp;

  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;
  si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

  CreatePipe(&a[0], &a[1], NULL, 0);
  CreatePipe(&b[0], &b[1], NULL, 0);
  DuplicateHandle(me, a[0], me, &si.hStdInput, 0, TRUE, flags);
  DuplicateHandle(me, b[1], me, &si.hStdOutput, 0, TRUE, flags);

  if (interp == NULL && (fp = fopen(cmd, "r")) != NULL) {
    buf[0] = buf[1] = '\0';
    fgets(buf, sizeof(buf), fp);
    buf[sizeof(buf) - 1] = '\0';
    if (buf[0] == '#' && buf[1] == '!') {
      interp = buf + 2;
      for (p = interp + strlen(interp);
           isspace(* (uint8_t *) p) && p > interp; p--) *p = '\0';
    }
    fclose(fp);
  }

  if (interp != NULL) {
    abs_path(interp, buf4, ARRAY_SIZE(buf4));
    interp = buf4;
  }
  abs_path(dir, buf5, ARRAY_SIZE(buf5));
  to_wchar(dir, full_dir, ARRAY_SIZE(full_dir));
  mg_snprintf(cmdline, sizeof(cmdline), "%s%s\"%s\"",
              interp ? interp : "", interp ? " " : "", cmd);
  to_wchar(cmdline, wcmd, ARRAY_SIZE(wcmd));

  if (CreateProcessW(NULL, wcmd, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP,
                     (void *) env, full_dir, &si, &pi) != 0) {
    spawn_stdio_thread(sock, a[1], push_to_stdin);
    spawn_stdio_thread(sock, b[0], pull_from_stdout);
  } else {
    CloseHandle(a[1]);
    CloseHandle(b[0]);
    closesocket(sock);
  }
  DBG(("CGI command: [%ls] -> %p", wcmd, pi.hProcess));

  CloseHandle(si.hStdOutput);
  CloseHandle(si.hStdInput);
  CloseHandle(a[0]);
  CloseHandle(b[1]);
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  return pi.hProcess;
}
#else
static pid_t start_process(const char *interp, const char *cmd, const char *env,
                           const char *envp[], const char *dir, sock_t sock) {
  char buf[500];
  pid_t pid = fork();
  (void) env;

  if (pid == 0) {
    (void) chdir(dir);
    (void) dup2(sock, 0);
    (void) dup2(sock, 1);
    closesocket(sock);

    // After exec, all signal handlers are restored to their default values,
    // with one exception of SIGCHLD. According to POSIX.1-2001 and Linux's
    // implementation, SIGCHLD's handler will leave unchanged after exec
    // if it was set to be ignored. Restore it to default action.
    signal(SIGCHLD, SIG_DFL);

    if (interp == NULL) {
      execle(cmd, cmd, NULL, envp);
    } else {
      execle(interp, interp, cmd, NULL, envp);
    }
    snprintf(buf, sizeof(buf), "Status: 500\r\n\r\n"
             "500 Server Error: %s%s%s: %s", interp == NULL ? "" : interp,
             interp == NULL ? "" : " ", cmd, strerror(errno));
    send(1, buf, strlen(buf), 0);
    exit(EXIT_FAILURE);  // exec call failed
  }

  return pid;
}
#endif  // _WIN32

// This structure helps to create an environment for the spawned CGI program.
// Environment is an array of "VARIABLE=VALUE\0" ASCIIZ strings,
// last element must be NULL.
// However, on Windows there is a requirement that all these VARIABLE=VALUE\0
// strings must reside in a contiguous buffer. The end of the buffer is
// marked by two '\0' characters.
// We satisfy both worlds: we create an envp array (which is vars), all
// entries are actually pointers inside buf.
struct cgi_env_block {
  struct mg_connection *conn;
  char buf[CGI_ENVIRONMENT_SIZE];       // Environment buffer
  const char *vars[MAX_CGI_ENVIR_VARS]; // char *envp[]
  int len;                              // Space taken
  int nvars;                            // Number of variables in envp[]
};

// Append VARIABLE=VALUE\0 string to the buffer, and add a respective
// pointer into the vars array.
static char *addenv(struct cgi_env_block *block, const char *fmt, ...) {
  int n, space;
  char *added;
  va_list ap;

  // Calculate how much space is left in the buffer
  space = sizeof(block->buf) - block->len - 2;
  assert(space >= 0);

  // Make a pointer to the free space int the buffer
  added = block->buf + block->len;

  // Copy VARIABLE=VALUE\0 string into the free space
  va_start(ap, fmt);
  n = mg_vsnprintf(added, (size_t) space, fmt, ap);
  va_end(ap);

  // Make sure we do not overflow buffer and the envp array
  if (n > 0 && n + 1 < space &&
      block->nvars < (int) ARRAY_SIZE(block->vars) - 2) {
    // Append a pointer to the added string into the envp array
    block->vars[block->nvars++] = added;
    // Bump up used length counter. Include \0 terminator
    block->len += n + 1;
  }

  return added;
}

static void addenv2(struct cgi_env_block *blk, const char *name) {
  const char *s;
  if ((s = getenv(name)) != NULL) addenv(blk, "%s=%s", name, s);
}

static void prepare_cgi_environment(struct connection *conn,
                                    const char *prog,
                                    struct cgi_env_block *blk) {
  struct mg_connection *ri = &conn->mg_conn;
  const char *s, *slash;
  char *p, **opts = conn->server->config_options;
  int  i;

  blk->len = blk->nvars = 0;
  blk->conn = ri;

  addenv(blk, "SERVER_NAME=%s", opts[AUTH_DOMAIN]);
  addenv(blk, "SERVER_ROOT=%s", opts[DOCUMENT_ROOT]);
  addenv(blk, "DOCUMENT_ROOT=%s", opts[DOCUMENT_ROOT]);
  addenv(blk, "SERVER_SOFTWARE=%s/%s", "Mongoose", MONGOOSE_VERSION);

  // Prepare the environment block
  addenv(blk, "%s", "GATEWAY_INTERFACE=CGI/1.1");
  addenv(blk, "%s", "SERVER_PROTOCOL=HTTP/1.1");
  addenv(blk, "%s", "REDIRECT_STATUS=200"); // For PHP

  // TODO(lsm): fix this for IPv6 case
  //addenv(blk, "SERVER_PORT=%d", ri->remote_port);

  addenv(blk, "REQUEST_METHOD=%s", ri->request_method);
  addenv(blk, "REMOTE_ADDR=%s", ri->remote_ip);
  addenv(blk, "REMOTE_PORT=%d", ri->remote_port);
  addenv(blk, "REQUEST_URI=%s%s%s", ri->uri,
         ri->query_string == NULL ? "" : "?",
         ri->query_string == NULL ? "" : ri->query_string);

  // SCRIPT_NAME
  if (conn->path_info != NULL) {
    addenv(blk, "SCRIPT_NAME=%.*s",
           (int) (strlen(ri->uri) - strlen(conn->path_info)), ri->uri);
    addenv(blk, "PATH_INFO=%s", conn->path_info);
  } else {
    s = strrchr(prog, '/');
    slash = strrchr(ri->uri, '/');
    addenv(blk, "SCRIPT_NAME=%.*s%s",
           slash == NULL ? 0 : (int) (slash - ri->uri), ri->uri,
           s == NULL ? prog : s);
  }

  addenv(blk, "SCRIPT_FILENAME=%s", prog);
  addenv(blk, "PATH_TRANSLATED=%s", prog);
#ifdef MONGOOSE_USE_SSL
  addenv(blk, "HTTPS=%s", conn->ssl != NULL ? "on" : "off");
#else
  addenv(blk, "HTTPS=%s", "off");
#endif

  if ((s = mg_get_header(ri, "Content-Type")) != NULL)
    addenv(blk, "CONTENT_TYPE=%s", s);

  if (ri->query_string != NULL)
    addenv(blk, "QUERY_STRING=%s", ri->query_string);

  if ((s = mg_get_header(ri, "Content-Length")) != NULL)
    addenv(blk, "CONTENT_LENGTH=%s", s);

  addenv2(blk, "PATH");
  addenv2(blk, "PERLLIB");
  addenv2(blk, ENV_EXPORT_TO_CGI);

#if defined(_WIN32)
  addenv2(blk, "COMSPEC");
  addenv2(blk, "SYSTEMROOT");
  addenv2(blk, "SystemDrive");
  addenv2(blk, "ProgramFiles");
  addenv2(blk, "ProgramFiles(x86)");
  addenv2(blk, "CommonProgramFiles(x86)");
#else
  addenv2(blk, "LD_LIBRARY_PATH");
#endif // _WIN32

  // Add all headers as HTTP_* variables
  for (i = 0; i < ri->num_headers; i++) {
    p = addenv(blk, "HTTP_%s=%s",
        ri->http_headers[i].name, ri->http_headers[i].value);

    // Convert variable name into uppercase, and change - to _
    for (; *p != '=' && *p != '\0'; p++) {
      if (*p == '-')
        *p = '_';
      *p = (char) toupper(* (unsigned char *) p);
    }
  }

  blk->vars[blk->nvars++] = NULL;
  blk->buf[blk->len++] = '\0';

  assert(blk->nvars < (int) ARRAY_SIZE(blk->vars));
  assert(blk->len > 0);
  assert(blk->len < (int) sizeof(blk->buf));
}

static const char cgi_status[] = "HTTP/1.1 200 OK\r\n";

static void open_cgi_endpoint(struct connection *conn, const char *prog) {
  struct cgi_env_block blk;
  char dir[MAX_PATH_SIZE], *p;
  sock_t fds[2];

  prepare_cgi_environment(conn, prog, &blk);
  // CGI must be executed in its own directory. 'dir' must point to the
  // directory containing executable program, 'p' must point to the
  // executable program name relative to 'dir'.
  if ((p = strrchr(prog, '/')) == NULL) {
    mg_snprintf(dir, sizeof(dir), "%s", ".");
  } else {
    mg_snprintf(dir, sizeof(dir), "%.*s", (int) (p - prog), prog);
  }

  // Try to create socketpair in a loop until success. mg_socketpair()
  // can be interrupted by a signal and fail.
  // TODO(lsm): use sigaction to restart interrupted syscall
  do {
    mg_socketpair(fds);
  } while (fds[0] == INVALID_SOCKET);

  if (start_process(conn->server->config_options[CGI_INTERPRETER],
                    prog, blk.buf, blk.vars, dir, fds[1]) > 0) {
    conn->endpoint_type = EP_CGI;
    conn->endpoint.cgi_sock = fds[0];
    spool(&conn->remote_iobuf, cgi_status, sizeof(cgi_status) - 1);
    conn->mg_conn.status_code = 200;
    conn->flags |= CONN_BUFFER;
  } else {
    closesocket(fds[0]);
    send_http_error(conn, 500, "start_process(%s) failed", prog);
  }

#ifndef _WIN32
  closesocket(fds[1]);  // On Windows, CGI stdio thread closes that socket
#endif
}

static void read_from_cgi(struct connection *conn) {
  struct iobuf *io = &conn->remote_iobuf;
  char buf[IOBUF_SIZE], buf2[sizeof(buf)], *s = buf2;
  const char *status = "500";
  struct mg_connection c;
  int len, s_len = sizeof(cgi_status) - 1,
      n = recv(conn->endpoint.cgi_sock, buf, sizeof(buf), 0);

  DBG(("%p %d", conn, n));
  if (is_error(n)) {
    close_local_endpoint(conn);
  } else if (n > 0) {
    spool(&conn->remote_iobuf, buf, n);
    if (conn->flags & CONN_BUFFER) {
      len = get_request_len(io->buf + s_len, io->len - s_len);
      if (len == 0) return;
      if (len > 0) {
        memset(&c, 0, sizeof(c));
        memcpy(buf2, io->buf + s_len, len);
        buf2[len - 1] = '\0';
        parse_http_headers(&s, &c);
        if (mg_get_header(&c, "Location") != NULL) {
          status = "302";
        } else if ((status = (char *) mg_get_header(&c, "Status")) == NULL) {
          status = "200";
        }
      }
      memcpy(io->buf + 9, status, 3);
      conn->mg_conn.status_code = atoi(status);
      conn->flags &= ~CONN_BUFFER;
    }
  }
}

static void forward_post_data(struct connection *conn) {
  struct iobuf *io = &conn->local_iobuf;
  int n = send(conn->endpoint.cgi_sock, io->buf, io->len, 0);
  discard_leading_iobuf_bytes(io, n);
}
#endif  // !MONGOOSE_NO_CGI

// 'sa' must be an initialized address to bind to
static sock_t open_listening_socket(union socket_address *sa) {
  socklen_t len = sizeof(*sa);
  sock_t on = 1, sock = INVALID_SOCKET;

  if ((sock = socket(sa->sa.sa_family, SOCK_STREAM, 6)) != INVALID_SOCKET &&
      !setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *) &on, sizeof(on)) &&
      !bind(sock, &sa->sa, sa->sa.sa_family == AF_INET ?
            sizeof(sa->sin) : sizeof(sa->sa)) &&
      !listen(sock, SOMAXCONN)) {
    set_non_blocking_mode(sock);
    // In case port was set to 0, get the real port number
    (void) getsockname(sock, &sa->sa, &len);
  } else if (sock != INVALID_SOCKET) {
    closesocket(sock);
    sock = INVALID_SOCKET;
  }

  return sock;
}

static char *mg_strdup(const char *str) {
  char *copy = (char *) malloc(strlen(str) + 1);
  if (copy != NULL) {
    strcpy(copy, str);
  }
  return copy;
}

static int isbyte(int n) {
  return n >= 0 && n <= 255;
}

static int parse_net(const char *spec, uint32_t *net, uint32_t *mask) {
  int n, a, b, c, d, slash = 32, len = 0;

  if ((sscanf(spec, "%d.%d.%d.%d/%d%n", &a, &b, &c, &d, &slash, &n) == 5 ||
      sscanf(spec, "%d.%d.%d.%d%n", &a, &b, &c, &d, &n) == 4) &&
      isbyte(a) && isbyte(b) && isbyte(c) && isbyte(d) &&
      slash >= 0 && slash < 33) {
    len = n;
    *net = ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | d;
    *mask = slash ? 0xffffffffU << (32 - slash) : 0;
  }

  return len;
}

// Verify given socket address against the ACL.
// Return -1 if ACL is malformed, 0 if address is disallowed, 1 if allowed.
static int check_acl(const char *acl, uint32_t remote_ip) {
  int allowed, flag;
  uint32_t net, mask;
  struct vec vec;

  // If any ACL is set, deny by default
  allowed = acl == NULL ? '+' : '-';

  while ((acl = next_option(acl, &vec, NULL)) != NULL) {
    flag = vec.ptr[0];
    if ((flag != '+' && flag != '-') ||
        parse_net(&vec.ptr[1], &net, &mask) == 0) {
      return -1;
    }

    if (net == (remote_ip & mask)) {
      allowed = flag;
    }
  }

  return allowed == '+';
}

static void sockaddr_to_string(char *buf, size_t len,
                               const union socket_address *usa) {
  buf[0] = '\0';
#if defined(MONGOOSE_USE_IPV6)
  inet_ntop(usa->sa.sa_family, usa->sa.sa_family == AF_INET ?
            (void *) &usa->sin.sin_addr :
            (void *) &usa->sin6.sin6_addr, buf, len);
#elif defined(_WIN32)
  // Only Windoze Vista (and newer) have inet_ntop()
  strncpy(buf, inet_ntoa(usa->sin.sin_addr), len);
#else
  inet_ntop(usa->sa.sa_family, (void *) &usa->sin.sin_addr, buf, len);
#endif
}

static struct connection *accept_new_connection(struct mg_server *server) {
  union socket_address sa;
  socklen_t len = sizeof(sa);
  sock_t sock = INVALID_SOCKET;
  struct connection *conn = NULL;

  // NOTE(lsm): on Windows, sock is always > FD_SETSIZE
  if ((sock = accept(server->listening_sock, &sa.sa, &len)) == INVALID_SOCKET) {
  } else if (!check_acl(server->config_options[ACCESS_CONTROL_LIST],
                        ntohl(* (uint32_t *) &sa.sin.sin_addr))) {
    // NOTE(lsm): check_acl doesn't work for IPv6
    closesocket(sock);
  } else if ((conn = (struct connection *) calloc(1, sizeof(*conn))) == NULL) {
    closesocket(sock);
#ifdef MONGOOSE_USE_SSL
  } else if (server->ssl_ctx != NULL &&
             ((conn->ssl = SSL_new(server->ssl_ctx)) == NULL ||
              SSL_set_fd(conn->ssl, sock) != 1)) {
    DBG(("SSL error"));
    closesocket(sock);
    free(conn);
    conn = NULL;
#endif
  } else {
    set_close_on_exec(sock);
    set_non_blocking_mode(sock);
    conn->server = server;
    conn->client_sock = sock;
    sockaddr_to_string(conn->mg_conn.remote_ip,
                       sizeof(conn->mg_conn.remote_ip), &sa);
    conn->mg_conn.remote_port = ntohs(sa.sin.sin_port);
    conn->mg_conn.server_param = server->server_data;
    conn->mg_conn.local_ip = server->local_ip;
    conn->mg_conn.local_port = ntohs(server->lsa.sin.sin_port);
    LINKED_LIST_ADD_TO_FRONT(&server->active_connections, &conn->link);
    DBG(("added conn %p", conn));
  }

  return conn;
}

static void close_conn(struct connection *conn) {
  LINKED_LIST_REMOVE(&conn->link);
  closesocket(conn->client_sock);
  close_local_endpoint(conn);
  DBG(("%p %d %d", conn, conn->flags, conn->endpoint_type));
  free(conn->request);            // It's OK to free(NULL), ditto below
  free(conn->path_info);
  free(conn->remote_iobuf.buf);
  free(conn->local_iobuf.buf);
#ifdef MONGOOSE_USE_SSL
  if (conn->ssl != NULL) SSL_free(conn->ssl);
#endif
  free(conn);
}

// Protect against directory disclosure attack by removing '..',
// excessive '/' and '\' characters
static void remove_double_dots_and_double_slashes(char *s) {
  char *p = s;

  while (*s != '\0') {
    *p++ = *s++;
    if (s[-1] == '/' || s[-1] == '\\') {
      // Skip all following slashes, backslashes and double-dots
      while (s[0] != '\0') {
        if (s[0] == '/' || s[0] == '\\') { s++; }
        else if (s[0] == '.' && s[1] == '.') { s += 2; }
        else { break; }
      }
    }
  }
  *p = '\0';
}

int mg_url_decode(const char *src, int src_len, char *dst,
                  int dst_len, int is_form_url_encoded) {
  int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

  for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
    if (src[i] == '%' && i < src_len - 2 &&
        isxdigit(* (const unsigned char *) (src + i + 1)) &&
        isxdigit(* (const unsigned char *) (src + i + 2))) {
      a = tolower(* (const unsigned char *) (src + i + 1));
      b = tolower(* (const unsigned char *) (src + i + 2));
      dst[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
      i += 2;
    } else if (is_form_url_encoded && src[i] == '+') {
      dst[j] = ' ';
    } else {
      dst[j] = src[i];
    }
  }

  dst[j] = '\0'; // Null-terminate the destination

  return i >= src_len ? j : -1;
}

static int is_valid_http_method(const char *method) {
  return !strcmp(method, "GET") || !strcmp(method, "POST") ||
    !strcmp(method, "HEAD") || !strcmp(method, "CONNECT") ||
    !strcmp(method, "PUT") || !strcmp(method, "DELETE") ||
    !strcmp(method, "OPTIONS") || !strcmp(method, "PROPFIND")
    || !strcmp(method, "MKCOL");
}

// Parse HTTP request, fill in mg_request structure.
// This function modifies the buffer by NUL-terminating
// HTTP request components, header names and header values.
// Note that len must point to the last \n of HTTP headers.
static int parse_http_message(char *buf, int len, struct mg_connection *ri) {
  int is_request, n;

  // Reset the connection. Make sure that we don't touch fields that are
  // set elsewhere: remote_ip, remote_port, server_param
  ri->request_method = ri->uri = ri->http_version = ri->query_string = NULL;
  ri->num_headers = ri->status_code = ri->is_websocket = ri->content_len = 0;

  buf[len - 1] = '\0';

  // RFC says that all initial whitespaces should be ingored
  while (*buf != '\0' && isspace(* (unsigned char *) buf)) {
    buf++;
  }
  ri->request_method = skip(&buf, " ");
  ri->uri = skip(&buf, " ");
  ri->http_version = skip(&buf, "\r\n");

  // HTTP message could be either HTTP request or HTTP response, e.g.
  // "GET / HTTP/1.0 ...." or  "HTTP/1.0 200 OK ..."
  is_request = is_valid_http_method(ri->request_method);
  if ((is_request && memcmp(ri->http_version, "HTTP/", 5) != 0) ||
      (!is_request && memcmp(ri->request_method, "HTTP/", 5) != 0)) {
    len = -1;
  } else {
    if (is_request) {
      ri->http_version += 5;
    }
    parse_http_headers(&buf, ri);

    if ((ri->query_string = strchr(ri->uri, '?')) != NULL) {
      *(char *) ri->query_string++ = '\0';
    }
    n = (int) strlen(ri->uri);
    mg_url_decode(ri->uri, n, (char *) ri->uri, n + 1, 0);
    remove_double_dots_and_double_slashes((char *) ri->uri);
  }

  return len;
}

static int lowercase(const char *s) {
  return tolower(* (const unsigned char *) s);
}

static int mg_strcasecmp(const char *s1, const char *s2) {
  int diff;

  do {
    diff = lowercase(s1++) - lowercase(s2++);
  } while (diff == 0 && s1[-1] != '\0');

  return diff;
}

static int mg_strncasecmp(const char *s1, const char *s2, size_t len) {
  int diff = 0;

  if (len > 0)
    do {
      diff = lowercase(s1++) - lowercase(s2++);
    } while (diff == 0 && s1[-1] != '\0' && --len > 0);

  return diff;
}

// Return HTTP header value, or NULL if not found.
const char *mg_get_header(const struct mg_connection *ri, const char *s) {
  int i;

  for (i = 0; i < ri->num_headers; i++)
    if (!mg_strcasecmp(s, ri->http_headers[i].name))
      return ri->http_headers[i].value;

  return NULL;
}

#ifndef MONGOOSE_NO_FILESYSTEM
// Perform case-insensitive match of string against pattern
static int match_prefix(const char *pattern, int pattern_len, const char *str) {
  const char *or_str;
  int len, res, i = 0, j = 0;

  if ((or_str = (const char *) memchr(pattern, '|', pattern_len)) != NULL) {
    res = match_prefix(pattern, or_str - pattern, str);
    return res > 0 ? res :
        match_prefix(or_str + 1, (pattern + pattern_len) - (or_str + 1), str);
  }

  for (; i < pattern_len; i++, j++) {
    if (pattern[i] == '?' && str[j] != '\0') {
      continue;
    } else if (pattern[i] == '$') {
      return str[j] == '\0' ? j : -1;
    } else if (pattern[i] == '*') {
      i++;
      if (pattern[i] == '*') {
        i++;
        len = (int) strlen(str + j);
      } else {
        len = (int) strcspn(str + j, "/");
      }
      if (i == pattern_len) {
        return j + len;
      }
      do {
        res = match_prefix(pattern + i, pattern_len - i, str + j + len);
      } while (res == -1 && len-- > 0);
      return res == -1 ? -1 : j + res + len;
    } else if (lowercase(&pattern[i]) != lowercase(&str[j])) {
      return -1;
    }
  }
  return j;
}

static int must_hide_file(struct connection *conn, const char *path) {
  const char *pw_pattern = "**" PASSWORDS_FILE_NAME "$";
  const char *pattern = conn->server->config_options[HIDE_FILES_PATTERN];
  return match_prefix(pw_pattern, strlen(pw_pattern), path) > 0 ||
    (pattern != NULL && match_prefix(pattern, strlen(pattern), path) > 0);
}

// Return 1 if real file has been found, 0 otherwise
static int convert_uri_to_file_name(struct connection *conn, char *buf,
                                    size_t buf_len, file_stat_t *st) {
  struct vec a, b;
  const char *rewrites = conn->server->config_options[URL_REWRITES];
  const char *root = conn->server->config_options[DOCUMENT_ROOT];
#ifndef MONGOOSE_NO_CGI
  const char *cgi_pat = conn->server->config_options[CGI_PATTERN];
  char *p;
#endif
  const char *uri = conn->mg_conn.uri;
  int match_len;

  // No filesystem access
  if (root == NULL) return 0;

  // Handle URL rewrites
  mg_snprintf(buf, buf_len, "%s%s", root, uri);
  while ((rewrites = next_option(rewrites, &a, &b)) != NULL) {
    if ((match_len = match_prefix(a.ptr, a.len, uri)) > 0) {
      mg_snprintf(buf, buf_len, "%.*s%s", (int) b.len, b.ptr, uri + match_len);
      break;
    }
  }

  if (stat(buf, st) == 0) return 1;

#ifndef MONGOOSE_NO_CGI
  // Support PATH_INFO for CGI scripts.
  for (p = buf + strlen(root) + 2; *p != '\0'; p++) {
    if (*p == '/') {
      *p = '\0';
      if (match_prefix(cgi_pat, strlen(cgi_pat), buf) > 0 && !stat(buf, st)) {
      DBG(("!!!! [%s]", buf));
        *p = '/';
        conn->path_info = mg_strdup(p);
        *p = '\0';
        return 1;
      }
      *p = '/';
    }
  }
#endif

  return 0;
}
#endif  // MONGOOSE_NO_FILESYSTEM

static int should_keep_alive(const struct mg_connection *conn) {
  const char *method = conn->request_method;
  const char *http_version = conn->http_version;
  const char *header = mg_get_header(conn, "Connection");
  return method != NULL && (!strcmp(method, "GET") ||
        ((struct connection *) conn)->endpoint_type == EP_USER) &&
    ((header != NULL && !mg_strcasecmp(header, "keep-alive")) ||
     (header == NULL && http_version && !strcmp(http_version, "1.1")));
}

int mg_write(struct mg_connection *c, const void *buf, int len) {
  return spool(&((struct connection *) c)->remote_iobuf, buf, len);
}

void mg_send_status(struct mg_connection *c, int status) {
  if (c->status_code == 0) {
    c->status_code = status;
    mg_printf(c, "HTTP/1.1 %d %s\r\n", status, status_code_to_str(status));
  }
}

void mg_send_header(struct mg_connection *c, const char *name, const char *v) {
  if (c->status_code == 0) {
    c->status_code = 200;
    mg_printf(c, "HTTP/1.1 %d %s\r\n", 200, status_code_to_str(200));
  }
  mg_printf(c, "%s: %s\r\n", name, v);
}

static void terminate_headers(struct mg_connection *c) {
  struct connection *conn = (struct connection *) c;
  if (!(conn->flags & CONN_HEADERS_SENT)) {
    mg_send_header(c, "Transfer-Encoding", "chunked");
    mg_write(c, "\r\n", 2);
    conn->flags |= CONN_HEADERS_SENT;
  }
}

void mg_send_data(struct mg_connection *c, const void *data, int data_len) {
  terminate_headers(c);
  write_chunk((struct connection *) c, (const char *) data, data_len);
}

void mg_printf_data(struct mg_connection *c, const char *fmt, ...) {
  va_list ap;

  terminate_headers(c);

  va_start(ap, fmt);
  mg_vprintf(c, fmt, ap, 1);
  va_end(ap);
}

#if !defined(NO_WEBSOCKET) || !defined(MONGOOSE_NO_AUTH)
static int is_big_endian(void) {
  static const int n = 1;
  return ((char *) &n)[0] == 0;
}
#endif

#ifndef MONGOOSE_NO_WEBSOCKET
// START OF SHA-1 code
// Copyright(c) By Steve Reid <steve@edmweb.com>
#define SHA1HANDSOFF
#if defined(__sun)
#include "solarisfixes.h"
#endif

union char64long16 { unsigned char c[64]; uint32_t l[16]; };

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

static uint32_t blk0(union char64long16 *block, int i) {
  // Forrest: SHA expect BIG_ENDIAN, swap if LITTLE_ENDIAN
  if (!is_big_endian()) {
    block->l[i] = (rol(block->l[i], 24) & 0xFF00FF00) |
      (rol(block->l[i], 8) & 0x00FF00FF);
  }
  return block->l[i];
}

#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(block, i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;

static void SHA1Transform(uint32_t state[5], const unsigned char buffer[64]) {
  uint32_t a, b, c, d, e;
  union char64long16 block[1];

  memcpy(block, buffer, 64);
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];
  R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
  R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
  R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
  R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
  R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
  R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
  R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
  R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
  R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
  R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
  R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
  R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
  R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
  R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
  R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
  R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
  R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
  R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
  R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
  R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
  // Erase working structures. The order of operations is important,
  // used to ensure that compiler doesn't optimize those out.
  memset(block, 0, sizeof(block));
  a = b = c = d = e = 0;
  (void) a; (void) b; (void) c; (void) d; (void) e;
}

static void SHA1Init(SHA1_CTX* context) {
  context->state[0] = 0x67452301;
  context->state[1] = 0xEFCDAB89;
  context->state[2] = 0x98BADCFE;
  context->state[3] = 0x10325476;
  context->state[4] = 0xC3D2E1F0;
  context->count[0] = context->count[1] = 0;
}

static void SHA1Update(SHA1_CTX* context, const unsigned char* data,
                       uint32_t len) {
  uint32_t i, j;

  j = context->count[0];
  if ((context->count[0] += len << 3) < j)
    context->count[1]++;
  context->count[1] += (len>>29);
  j = (j >> 3) & 63;
  if ((j + len) > 63) {
    memcpy(&context->buffer[j], data, (i = 64-j));
    SHA1Transform(context->state, context->buffer);
    for ( ; i + 63 < len; i += 64) {
      SHA1Transform(context->state, &data[i]);
    }
    j = 0;
  }
  else i = 0;
  memcpy(&context->buffer[j], &data[i], len - i);
}

static void SHA1Final(unsigned char digest[20], SHA1_CTX* context) {
  unsigned i;
  unsigned char finalcount[8], c;

  for (i = 0; i < 8; i++) {
    finalcount[i] = (unsigned char)((context->count[(i >= 4 ? 0 : 1)]
                                     >> ((3-(i & 3)) * 8) ) & 255);
  }
  c = 0200;
  SHA1Update(context, &c, 1);
  while ((context->count[0] & 504) != 448) {
    c = 0000;
    SHA1Update(context, &c, 1);
  }
  SHA1Update(context, finalcount, 8);
  for (i = 0; i < 20; i++) {
    digest[i] = (unsigned char)
      ((context->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
  }
  memset(context, '\0', sizeof(*context));
  memset(&finalcount, '\0', sizeof(finalcount));
}
// END OF SHA1 CODE

static void base64_encode(const unsigned char *src, int src_len, char *dst) {
  static const char *b64 =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int i, j, a, b, c;

  for (i = j = 0; i < src_len; i += 3) {
    a = src[i];
    b = i + 1 >= src_len ? 0 : src[i + 1];
    c = i + 2 >= src_len ? 0 : src[i + 2];

    dst[j++] = b64[a >> 2];
    dst[j++] = b64[((a & 3) << 4) | (b >> 4)];
    if (i + 1 < src_len) {
      dst[j++] = b64[(b & 15) << 2 | (c >> 6)];
    }
    if (i + 2 < src_len) {
      dst[j++] = b64[c & 63];
    }
  }
  while (j % 4 != 0) {
    dst[j++] = '=';
  }
  dst[j++] = '\0';
}

static void send_websocket_handshake(struct mg_connection *conn,
                                     const char *key) {
  static const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  char buf[500], sha[20], b64_sha[sizeof(sha) * 2];
  SHA1_CTX sha_ctx;

  mg_snprintf(buf, sizeof(buf), "%s%s", key, magic);
  SHA1Init(&sha_ctx);
  SHA1Update(&sha_ctx, (unsigned char *) buf, strlen(buf));
  SHA1Final((unsigned char *) sha, &sha_ctx);
  base64_encode((unsigned char *) sha, sizeof(sha), b64_sha);
  mg_snprintf(buf, sizeof(buf), "%s%s%s",
              "HTTP/1.1 101 Switching Protocols\r\n"
              "Upgrade: websocket\r\n"
              "Connection: Upgrade\r\n"
              "Sec-WebSocket-Accept: ", b64_sha, "\r\n\r\n");

  mg_write(conn, buf, strlen(buf));
}

static int deliver_websocket_frame(struct connection *conn) {
  // Having buf unsigned char * is important, as it is used below in arithmetic
  unsigned char *buf = (unsigned char *) conn->local_iobuf.buf;
  int i, len, buf_len = conn->local_iobuf.len, frame_len = 0,
      mask_len = 0, header_len = 0, data_len = 0, buffered = 0;

  if (buf_len >= 2) {
    len = buf[1] & 127;
    mask_len = buf[1] & 128 ? 4 : 0;
    if (len < 126 && buf_len >= mask_len) {
      data_len = len;
      header_len = 2 + mask_len;
    } else if (len == 126 && buf_len >= 4 + mask_len) {
      header_len = 4 + mask_len;
      data_len = ((((int) buf[2]) << 8) + buf[3]);
    } else if (buf_len >= 10 + mask_len) {
      header_len = 10 + mask_len;
      data_len = (int) (((uint64_t) htonl(* (uint32_t *) &buf[2])) << 32) +
        htonl(* (uint32_t *) &buf[6]);
    }
  }

  frame_len = header_len + data_len;
  buffered = frame_len > 0 && frame_len <= buf_len;

  if (buffered) {
    conn->mg_conn.content_len = data_len;
    conn->mg_conn.content = (char *) buf + header_len;
    conn->mg_conn.wsbits = buf[0];

    // Apply mask if necessary
    if (mask_len > 0) {
      for (i = 0; i < data_len; i++) {
        buf[i + header_len] ^= (buf + header_len - mask_len)[i % 4];
      }
    }

    // Call the handler and remove frame from the iobuf
    if (conn->server->request_handler(&conn->mg_conn) == MG_CLIENT_CLOSE) {
      conn->flags |= CONN_SPOOL_DONE;
    }
    discard_leading_iobuf_bytes(&conn->local_iobuf, frame_len);
  }

  return buffered;
}

int mg_websocket_write(struct mg_connection* conn, int opcode,
                       const char *data, size_t data_len) {
    unsigned char *copy;
    size_t copy_len = 0;
    int retval = -1;

    if ((copy = (unsigned char *) malloc(data_len + 10)) == NULL) {
      return -1;
    }

    copy[0] = 0x80 + (opcode & 0x0f);

    // Frame format: http://tools.ietf.org/html/rfc6455#section-5.2
    if (data_len < 126) {
      // Inline 7-bit length field
      copy[1] = data_len;
      memcpy(copy + 2, data, data_len);
      copy_len = 2 + data_len;
    } else if (data_len <= 0xFFFF) {
      // 16-bit length field
      copy[1] = 126;
      * (uint16_t *) (copy + 2) = (uint16_t) htons((uint16_t) data_len);
      memcpy(copy + 4, data, data_len);
      copy_len = 4 + data_len;
    } else {
      // 64-bit length field
      copy[1] = 127;
      * (uint32_t *) (copy + 2) = (uint32_t)
        htonl((uint32_t) ((uint64_t) data_len >> 32));
      * (uint32_t *) (copy + 6) = (uint32_t) htonl(data_len & 0xffffffff);
      memcpy(copy + 10, data, data_len);
      copy_len = 10 + data_len;
    }

    if (copy_len > 0) {
      retval = mg_write(conn, copy, copy_len);
    }
    free(copy);

    return retval;
}

static void send_websocket_handshake_if_requested(struct mg_connection *conn) {
  const char *ver = mg_get_header(conn, "Sec-WebSocket-Version"),
        *key = mg_get_header(conn, "Sec-WebSocket-Key");
  if (ver != NULL && key != NULL) {
    conn->is_websocket = 1;
    send_websocket_handshake(conn, key);
  }
}

static void ping_idle_websocket_connection(struct connection *conn, time_t t) {
  if (t - conn->last_activity_time > MONGOOSE_USE_WEBSOCKET_PING_INTERVAL) {
    mg_websocket_write(&conn->mg_conn, 0x9, "", 0);
  }
}
#else
#define ping_idle_websocket_connection(conn, t)
#endif // !MONGOOSE_NO_WEBSOCKET

static void write_terminating_chunk(struct connection *conn) {
  mg_write(&conn->mg_conn, "0\r\n\r\n", 5);
}

static int call_request_handler(struct connection *conn) {
  int result;
  conn->mg_conn.content = conn->local_iobuf.buf;
  switch ((result = conn->server->request_handler(&conn->mg_conn))) {
    case MG_REQUEST_CALL_AGAIN: conn->flags |= CONN_LONG_RUNNING; break;
    case MG_REQUEST_NOT_PROCESSED: break;
    default:
      if (conn->flags & CONN_HEADERS_SENT) {
        write_terminating_chunk(conn);
      }
      close_local_endpoint(conn);
      break;
  }
  return result;
}

static void callback_http_client_on_connect(struct connection *conn) {
  int ok = 1, ret;
  socklen_t len = sizeof(ok);

  conn->flags &= ~CONN_CONNECTING;
  ret = getsockopt(conn->client_sock, SOL_SOCKET, SO_ERROR, (char *) &ok, &len);
#ifdef MONGOOSE_USE_SSL
  if (ret == 0 && ok == 0 && conn->ssl != NULL) {
    int res = SSL_connect(conn->ssl), ssl_err = SSL_get_error(conn->ssl, res);
    //DBG(("%p res %d %d", conn, res, ssl_err));
    if (res == 1) {
      conn->flags = CONN_SSL_HANDS_SHAKEN;
    } else if (res == 0 || ssl_err == 2 || ssl_err == 3) {
      conn->flags |= CONN_CONNECTING;
      return; // Call us again
    } else {
      ok = 1;
    }
  }
#endif
  conn->mg_conn.status_code =
    (ret == 0 && ok == 0) ? MG_CONNECT_SUCCESS : MG_CONNECT_FAILURE;
  if (conn->handler(&conn->mg_conn) || ok != 0) {
    conn->flags |= CONN_CLOSE;
  }
}

static void write_to_socket(struct connection *conn) {
  struct iobuf *io = &conn->remote_iobuf;
  int n = 0;

  if (conn->endpoint_type == EP_CLIENT && conn->flags & CONN_CONNECTING) {
    callback_http_client_on_connect(conn);
    return;
  }

#ifdef MONGOOSE_USE_SSL
  if (conn->ssl != NULL) {
    n = SSL_write(conn->ssl, io->buf, io->len);
  } else
#endif
  { n = send(conn->client_sock, io->buf, io->len, 0); }

  DBG(("%p Written %d of %d(%d): [%.*s ...]",
       conn, n, io->len, io->size, io->len < 40 ? io->len : 40, io->buf));

  if (is_error(n)) {
    conn->flags |= CONN_CLOSE;
  } else if (n > 0) {
    discard_leading_iobuf_bytes(io, n);
    conn->num_bytes_sent += n;
  }

  if (io->len == 0 && conn->flags & CONN_SPOOL_DONE) {
    conn->flags |= CONN_CLOSE;
  }
}

const char *mg_get_mime_type(const char *path, const char *default_mime_type) {
  const char *ext;
  size_t i, path_len;

  path_len = strlen(path);

  for (i = 0; static_builtin_mime_types[i].extension != NULL; i++) {
    ext = path + (path_len - static_builtin_mime_types[i].ext_len);
    if (path_len > static_builtin_mime_types[i].ext_len &&
        mg_strcasecmp(ext, static_builtin_mime_types[i].extension) == 0) {
      return static_builtin_mime_types[i].mime_type;
    }
  }

  return default_mime_type;
}

#ifndef MONGOOSE_NO_FILESYSTEM
// Convert month to the month number. Return -1 on error, or month number
static int get_month_index(const char *s) {
  static const char *month_names[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  int i;

  for (i = 0; i < (int) ARRAY_SIZE(month_names); i++)
    if (!strcmp(s, month_names[i]))
      return i;

  return -1;
}

static int num_leap_years(int year) {
  return year / 4 - year / 100 + year / 400;
}

// Parse UTC date-time string, and return the corresponding time_t value.
static time_t parse_date_string(const char *datetime) {
  static const unsigned short days_before_month[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
  };
  char month_str[32];
  int second, minute, hour, day, month, year, leap_days, days;
  time_t result = (time_t) 0;

  if (((sscanf(datetime, "%d/%3s/%d %d:%d:%d",
               &day, month_str, &year, &hour, &minute, &second) == 6) ||
       (sscanf(datetime, "%d %3s %d %d:%d:%d",
               &day, month_str, &year, &hour, &minute, &second) == 6) ||
       (sscanf(datetime, "%*3s, %d %3s %d %d:%d:%d",
               &day, month_str, &year, &hour, &minute, &second) == 6) ||
       (sscanf(datetime, "%d-%3s-%d %d:%d:%d",
               &day, month_str, &year, &hour, &minute, &second) == 6)) &&
      year > 1970 &&
      (month = get_month_index(month_str)) != -1) {
    leap_days = num_leap_years(year) - num_leap_years(1970);
    year -= 1970;
    days = year * 365 + days_before_month[month] + (day - 1) + leap_days;
    result = days * 24 * 3600 + hour * 3600 + minute * 60 + second;
  }

  return result;
}

// Look at the "path" extension and figure what mime type it has.
// Store mime type in the vector.
static void get_mime_type(const struct mg_server *server, const char *path,
                          struct vec *vec) {
  struct vec ext_vec, mime_vec;
  const char *list, *ext;
  size_t path_len;

  path_len = strlen(path);

  // Scan user-defined mime types first, in case user wants to
  // override default mime types.
  list = server->config_options[EXTRA_MIME_TYPES];
  while ((list = next_option(list, &ext_vec, &mime_vec)) != NULL) {
    // ext now points to the path suffix
    ext = path + path_len - ext_vec.len;
    if (mg_strncasecmp(ext, ext_vec.ptr, ext_vec.len) == 0) {
      *vec = mime_vec;
      return;
    }
  }

  vec->ptr = mg_get_mime_type(path, "text/plain");
  vec->len = strlen(vec->ptr);
}

static const char *suggest_connection_header(const struct mg_connection *conn) {
  return should_keep_alive(conn) ? "keep-alive" : "close";
}

static void construct_etag(char *buf, size_t buf_len, const file_stat_t *st) {
  mg_snprintf(buf, buf_len, "\"%lx.%" INT64_FMT "\"",
              (unsigned long) st->st_mtime, (int64_t) st->st_size);
}

// Return True if we should reply 304 Not Modified.
static int is_not_modified(const struct connection *conn,
                           const file_stat_t *stp) {
  char etag[64];
  const char *ims = mg_get_header(&conn->mg_conn, "If-Modified-Since");
  const char *inm = mg_get_header(&conn->mg_conn, "If-None-Match");
  construct_etag(etag, sizeof(etag), stp);
  return (inm != NULL && !mg_strcasecmp(etag, inm)) ||
    (ims != NULL && stp->st_mtime <= parse_date_string(ims));
}

// For given directory path, substitute it to valid index file.
// Return 0 if index file has been found, -1 if not found.
// If the file is found, it's stats is returned in stp.
static int find_index_file(struct connection *conn, char *path,
                           size_t path_len, file_stat_t *stp) {
  const char *list = conn->server->config_options[INDEX_FILES];
  file_stat_t st;
  struct vec filename_vec;
  size_t n = strlen(path), found = 0;

  // The 'path' given to us points to the directory. Remove all trailing
  // directory separator characters from the end of the path, and
  // then append single directory separator character.
  while (n > 0 && path[n - 1] == '/') {
    n--;
  }
  path[n] = '/';

  // Traverse index files list. For each entry, append it to the given
  // path and see if the file exists. If it exists, break the loop
  while ((list = next_option(list, &filename_vec, NULL)) != NULL) {

    // Ignore too long entries that may overflow path buffer
    if (filename_vec.len > (int) (path_len - (n + 2)))
      continue;

    // Prepare full path to the index file
    strncpy(path + n + 1, filename_vec.ptr, filename_vec.len);
    path[n + 1 + filename_vec.len] = '\0';

    //DBG(("[%s]", path));

    // Does it exist?
    if (!stat(path, &st)) {
      // Yes it does, break the loop
      *stp = st;
      found = 1;
      break;
    }
  }

  // If no index file exists, restore directory path
  if (!found) {
    path[n] = '\0';
  }

  return found;
}

static int parse_range_header(const char *header, int64_t *a, int64_t *b) {
  return sscanf(header, "bytes=%" INT64_FMT "-%" INT64_FMT, a, b);
}

static void gmt_time_string(char *buf, size_t buf_len, time_t *t) {
  strftime(buf, buf_len, "%a, %d %b %Y %H:%M:%S GMT", gmtime(t));
}

static void open_file_endpoint(struct connection *conn, const char *path,
                               file_stat_t *st) {
  char date[64], lm[64], etag[64], range[64], headers[500];
  const char *msg = "OK", *hdr;
  time_t curtime = time(NULL);
  int64_t r1, r2;
  struct vec mime_vec;
  int n;

  conn->endpoint_type = EP_FILE;
  set_close_on_exec(conn->endpoint.fd);
  conn->mg_conn.status_code = 200;

  get_mime_type(conn->server, path, &mime_vec);
  conn->cl = st->st_size;
  range[0] = '\0';

  // If Range: header specified, act accordingly
  r1 = r2 = 0;
  hdr = mg_get_header(&conn->mg_conn, "Range");
  if (hdr != NULL && (n = parse_range_header(hdr, &r1, &r2)) > 0 &&
      r1 >= 0 && r2 >= 0) {
    conn->mg_conn.status_code = 206;
    conn->cl = n == 2 ? (r2 > conn->cl ? conn->cl : r2) - r1 + 1: conn->cl - r1;
    mg_snprintf(range, sizeof(range), "Content-Range: bytes "
                "%" INT64_FMT "-%" INT64_FMT "/%" INT64_FMT "\r\n",
                r1, r1 + conn->cl - 1, (int64_t) st->st_size);
    msg = "Partial Content";
    lseek(conn->endpoint.fd, r1, SEEK_SET);
  }

  // Prepare Etag, Date, Last-Modified headers. Must be in UTC, according to
  // http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3
  gmt_time_string(date, sizeof(date), &curtime);
  gmt_time_string(lm, sizeof(lm), &st->st_mtime);
  construct_etag(etag, sizeof(etag), st);

  n = mg_snprintf(headers, sizeof(headers),
                  "HTTP/1.1 %d %s\r\n"
                  "Date: %s\r\n"
                  "Last-Modified: %s\r\n"
                  "Etag: %s\r\n"
                  "Content-Type: %.*s\r\n"
                  "Content-Length: %" INT64_FMT "\r\n"
                  "Connection: %s\r\n"
                  "Accept-Ranges: bytes\r\n"
                  "%s%s\r\n",
                  conn->mg_conn.status_code, msg, date, lm, etag,
                  (int) mime_vec.len, mime_vec.ptr, conn->cl,
                  suggest_connection_header(&conn->mg_conn),
                  range, MONGOOSE_USE_EXTRA_HTTP_HEADERS);
  spool(&conn->remote_iobuf, headers, n);

  if (!strcmp(conn->mg_conn.request_method, "HEAD")) {
    conn->flags |= CONN_SPOOL_DONE;
    close(conn->endpoint.fd);
    conn->endpoint_type = EP_NONE;
  }
}

#endif  // MONGOOSE_NO_FILESYSTEM

static void call_request_handler_if_data_is_buffered(struct connection *conn) {
  struct iobuf *loc = &conn->local_iobuf;
  struct mg_connection *c = &conn->mg_conn;

#ifndef MONGOOSE_NO_WEBSOCKET
  if (conn->mg_conn.is_websocket) {
    do { } while (deliver_websocket_frame(conn));
  } else
#endif
  if ((size_t) loc->len >= c->content_len &&
      call_request_handler(conn) == MG_REQUEST_NOT_PROCESSED) {
    open_local_endpoint(conn, 1);
  }
}

#if !defined(MONGOOSE_NO_DIRECTORY_LISTING) || !defined(MONGOOSE_NO_DAV)

#ifdef _WIN32
struct dirent {
  char d_name[MAX_PATH_SIZE];
};

typedef struct DIR {
  HANDLE   handle;
  WIN32_FIND_DATAW info;
  struct dirent result;
} DIR;

// Implementation of POSIX opendir/closedir/readdir for Windows.
static DIR *opendir(const char *name) {
  DIR *dir = NULL;
  wchar_t wpath[MAX_PATH_SIZE];
  DWORD attrs;

  if (name == NULL) {
    SetLastError(ERROR_BAD_ARGUMENTS);
  } else if ((dir = (DIR *) malloc(sizeof(*dir))) == NULL) {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  } else {
    to_wchar(name, wpath, ARRAY_SIZE(wpath));
    attrs = GetFileAttributesW(wpath);
    if (attrs != 0xFFFFFFFF &&
        ((attrs & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)) {
      (void) wcscat(wpath, L"\\*");
      dir->handle = FindFirstFileW(wpath, &dir->info);
      dir->result.d_name[0] = '\0';
    } else {
      free(dir);
      dir = NULL;
    }
  }

  return dir;
}

static int closedir(DIR *dir) {
  int result = 0;

  if (dir != NULL) {
    if (dir->handle != INVALID_HANDLE_VALUE)
      result = FindClose(dir->handle) ? 0 : -1;

    free(dir);
  } else {
    result = -1;
    SetLastError(ERROR_BAD_ARGUMENTS);
  }

  return result;
}

static struct dirent *readdir(DIR *dir) {
  struct dirent *result = 0;

  if (dir) {
    if (dir->handle != INVALID_HANDLE_VALUE) {
      result = &dir->result;
      (void) WideCharToMultiByte(CP_UTF8, 0,
          dir->info.cFileName, -1, result->d_name,
          sizeof(result->d_name), NULL, NULL);

      if (!FindNextFileW(dir->handle, &dir->info)) {
        (void) FindClose(dir->handle);
        dir->handle = INVALID_HANDLE_VALUE;
      }

    } else {
      SetLastError(ERROR_FILE_NOT_FOUND);
    }
  } else {
    SetLastError(ERROR_BAD_ARGUMENTS);
  }

  return result;
}
#endif // _WIN32  POSIX opendir/closedir/readdir implementation

static int scan_directory(struct connection *conn, const char *dir,
                          struct dir_entry **arr) {
  char path[MAX_PATH_SIZE];
  struct dir_entry *p;
  struct dirent *dp;
  int arr_size = 0, arr_ind = 0, inc = 100;
  DIR *dirp;

  *arr = NULL;
  if ((dirp = (opendir(dir))) == NULL) return 0;

  while ((dp = readdir(dirp)) != NULL) {
    // Do not show current dir and hidden files
    if (!strcmp(dp->d_name, ".") ||
        !strcmp(dp->d_name, "..") ||
        must_hide_file(conn, dp->d_name)) {
      continue;
    }
    mg_snprintf(path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);

    // Resize the array if nesessary
    if (arr_ind >= arr_size) {
      if ((p = (struct dir_entry *)
           realloc(*arr, (inc + arr_size) * sizeof(**arr))) != NULL) {
        // Memset new chunk to zero, otherwize st_mtime will have garbage which
        // can make strftime() segfault, see
        // http://code.google.com/p/mongoose/issues/detail?id=79
        memset(p + arr_size, 0, sizeof(**arr) * inc);

        *arr = p;
        arr_size += inc;
      }
    }

    if (arr_ind < arr_size) {
      (*arr)[arr_ind].conn = conn;
      (*arr)[arr_ind].file_name = strdup(dp->d_name);
      stat(path, &(*arr)[arr_ind].st);
      arr_ind++;
    }
  }
  closedir(dirp);

  return arr_ind;
}

static void mg_url_encode(const char *src, char *dst, size_t dst_len) {
  static const char *dont_escape = "._-$,;~()";
  static const char *hex = "0123456789abcdef";
  const char *end = dst + dst_len - 1;

  for (; *src != '\0' && dst < end; src++, dst++) {
    if (isalnum(*(const unsigned char *) src) ||
        strchr(dont_escape, * (const unsigned char *) src) != NULL) {
      *dst = *src;
    } else if (dst + 2 < end) {
      dst[0] = '%';
      dst[1] = hex[(* (const unsigned char *) src) >> 4];
      dst[2] = hex[(* (const unsigned char *) src) & 0xf];
      dst += 2;
    }
  }

  *dst = '\0';
}
#endif  // !NO_DIRECTORY_LISTING || !MONGOOSE_NO_DAV

#ifndef MONGOOSE_NO_DIRECTORY_LISTING

static void print_dir_entry(const struct dir_entry *de) {
  char size[64], mod[64], href[MAX_PATH_SIZE * 3], chunk[MAX_PATH_SIZE * 4];
  int64_t fsize = de->st.st_size;
  int is_dir = S_ISDIR(de->st.st_mode), n;
  const char *slash = is_dir ? "/" : "";

  if (is_dir) {
    mg_snprintf(size, sizeof(size), "%s", "[DIRECTORY]");
  } else {
     // We use (signed) cast below because MSVC 6 compiler cannot
     // convert unsigned __int64 to double.
    if (fsize < 1024) {
      mg_snprintf(size, sizeof(size), "%d", (int) fsize);
    } else if (fsize < 0x100000) {
      mg_snprintf(size, sizeof(size), "%.1fk", (double) fsize / 1024.0);
    } else if (fsize < 0x40000000) {
      mg_snprintf(size, sizeof(size), "%.1fM", (double) fsize / 1048576);
    } else {
      mg_snprintf(size, sizeof(size), "%.1fG", (double) fsize / 1073741824);
    }
  }
  strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M", localtime(&de->st.st_mtime));
  mg_url_encode(de->file_name, href, sizeof(href));
  n = mg_snprintf(chunk, sizeof(chunk),
                  "<tr><td><a href=\"%s%s%s\">%s%s</a></td>"
                  "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
                  de->conn->mg_conn.uri, href, slash, de->file_name, slash,
                  mod, size);
  write_chunk((struct connection *) de->conn, chunk, n);
}

// Sort directory entries by size, or name, or modification time.
// On windows, __cdecl specification is needed in case if project is built
// with __stdcall convention. qsort always requires __cdels callback.
static int __cdecl compare_dir_entries(const void *p1, const void *p2) {
  const struct dir_entry *a = (const struct dir_entry *) p1,
        *b = (const struct dir_entry *) p2;
  const char *qs = a->conn->mg_conn.query_string ?
    a->conn->mg_conn.query_string : "na";
  int cmp_result = 0;

  if (S_ISDIR(a->st.st_mode) && !S_ISDIR(b->st.st_mode)) {
    return -1;  // Always put directories on top
  } else if (!S_ISDIR(a->st.st_mode) && S_ISDIR(b->st.st_mode)) {
    return 1;   // Always put directories on top
  } else if (*qs == 'n') {
    cmp_result = strcmp(a->file_name, b->file_name);
  } else if (*qs == 's') {
    cmp_result = a->st.st_size == b->st.st_size ? 0 :
      a->st.st_size > b->st.st_size ? 1 : -1;
  } else if (*qs == 'd') {
    cmp_result = a->st.st_mtime == b->st.st_mtime ? 0 :
      a->st.st_mtime > b->st.st_mtime ? 1 : -1;
  }

  return qs[1] == 'd' ? -cmp_result : cmp_result;
}

static void send_directory_listing(struct connection *conn, const char *dir) {
  char buf[2000];
  struct dir_entry *arr = NULL;
  int i, num_entries, sort_direction = conn->mg_conn.query_string != NULL &&
    conn->mg_conn.query_string[1] == 'd' ? 'a' : 'd';

  conn->mg_conn.status_code = 200;
  mg_snprintf(buf, sizeof(buf), "%s",
              "HTTP/1.1 200 OK\r\n"
              "Transfer-Encoding: Chunked\r\n"
              "Content-Type: text/html; charset=utf-8\r\n\r\n");
  spool(&conn->remote_iobuf, buf, strlen(buf));

  mg_snprintf(buf, sizeof(buf),
              "<html><head><title>Index of %s</title>"
              "<style>th {text-align: left;}</style></head>"
              "<body><h1>Index of %s</h1><pre><table cellpadding=\"0\">"
              "<tr><th><a href=\"?n%c\">Name</a></th>"
              "<th><a href=\"?d%c\">Modified</a></th>"
              "<th><a href=\"?s%c\">Size</a></th></tr>"
              "<tr><td colspan=\"3\"><hr></td></tr>",
              conn->mg_conn.uri, conn->mg_conn.uri,
              sort_direction, sort_direction, sort_direction);
  write_chunk(conn, buf, strlen(buf));

  num_entries = scan_directory(conn, dir, &arr);
  qsort(arr, num_entries, sizeof(arr[0]), compare_dir_entries);
  for (i = 0; i < num_entries; i++) {
    print_dir_entry(&arr[i]);
    free(arr[i].file_name);
  }
  free(arr);

  write_terminating_chunk(conn);
  close_local_endpoint(conn);
}
#endif  // MONGOOSE_NO_DIRECTORY_LISTING

#ifndef MONGOOSE_NO_DAV
static void print_props(struct connection *conn, const char *uri,
                        file_stat_t *stp) {
  char mtime[64], buf[MAX_PATH_SIZE + 200];

  gmt_time_string(mtime, sizeof(mtime), &stp->st_mtime);
  mg_snprintf(buf, sizeof(buf),
      "<d:response>"
       "<d:href>%s</d:href>"
       "<d:propstat>"
        "<d:prop>"
         "<d:resourcetype>%s</d:resourcetype>"
         "<d:getcontentlength>%" INT64_FMT "</d:getcontentlength>"
         "<d:getlastmodified>%s</d:getlastmodified>"
        "</d:prop>"
        "<d:status>HTTP/1.1 200 OK</d:status>"
       "</d:propstat>"
      "</d:response>\n",
      uri, S_ISDIR(stp->st_mode) ? "<d:collection/>" : "",
      (int64_t) stp->st_size, mtime);
  spool(&conn->remote_iobuf, buf, strlen(buf));
}

static void handle_propfind(struct connection *conn, const char *path,
                            file_stat_t *stp) {
  static const char header[] = "HTTP/1.1 207 Multi-Status\r\n"
    "Connection: close\r\n"
    "Content-Type: text/xml; charset=utf-8\r\n\r\n"
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<d:multistatus xmlns:d='DAV:'>\n";
  static const char footer[] = "</d:multistatus>";
  const char *depth = mg_get_header(&conn->mg_conn, "Depth"),
        *list_dir = conn->server->config_options[ENABLE_DIRECTORY_LISTING];

  conn->mg_conn.status_code = 207;
  spool(&conn->remote_iobuf, header, sizeof(header) - 1);

  // Print properties for the requested resource itself
  print_props(conn, conn->mg_conn.uri, stp);

  // If it is a directory, print directory entries too if Depth is not 0
  if (S_ISDIR(stp->st_mode) && !mg_strcasecmp(list_dir, "yes") &&
      (depth == NULL || strcmp(depth, "0") != 0)) {
    struct dir_entry *arr = NULL;
    int i, num_entries = scan_directory(conn, path, &arr);

    for (i = 0; i < num_entries; i++) {
      char buf[MAX_PATH_SIZE], buf2[sizeof(buf) * 3];
      struct dir_entry *de = &arr[i];

      mg_snprintf(buf, sizeof(buf), "%s%s", de->conn->mg_conn.uri,
                  de->file_name);
      mg_url_encode(buf, buf2, sizeof(buf2) - 1);
      print_props(conn, buf, &de->st);
    }
  }

  spool(&conn->remote_iobuf, footer, sizeof(footer) - 1);
  close_local_endpoint(conn);
}

static void handle_mkcol(struct connection *conn, const char *path) {
  int status_code = 500;

  if (conn->mg_conn.content_len > 0) {
    status_code = 415;
  } else if (!mkdir(path, 0755)) {
    status_code = 201;
  } else if (errno == EEXIST) {
    status_code = 405;
  } else if (errno == EACCES) {
    status_code = 403;
  } else if (errno == ENOENT) {
    status_code = 409;
  }
  send_http_error(conn, status_code, NULL);
}

static int remove_directory(const char *dir) {
  char path[MAX_PATH_SIZE];
  struct dirent *dp;
  file_stat_t st;
  DIR *dirp;

  if ((dirp = opendir(dir)) == NULL) return 0;

  while ((dp = readdir(dirp)) != NULL) {
    if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) continue;
    mg_snprintf(path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);
    stat(path, &st);
    if (S_ISDIR(st.st_mode)) {
      remove_directory(path);
    } else {
      remove(path);
    }
  }
  closedir(dirp);
  rmdir(dir);

  return 1;
}

static void handle_delete(struct connection *conn, const char *path) {
  file_stat_t st;

  if (stat(path, &st) != 0) {
    send_http_error(conn, 404, NULL);
  } else if (S_ISDIR(st.st_mode)) {
    remove_directory(path);
    send_http_error(conn, 204, NULL);
  } else if (!remove(path) == 0) {
    send_http_error(conn, 204, NULL);
  } else {
    send_http_error(conn, 423, NULL);
  }
}

// For a given PUT path, create all intermediate subdirectories
// for given path. Return 0 if the path itself is a directory,
// or -1 on error, 1 if OK.
static int put_dir(const char *path) {
  char buf[MAX_PATH_SIZE];
  const char *s, *p;
  file_stat_t st;

  // Create intermediate directories if they do not exist
  for (s = p = path + 1; (p = strchr(s, '/')) != NULL; s = ++p) {
    if (p - path >= (int) sizeof(buf)) return -1; // Buffer overflow
    memcpy(buf, path, p - path);
    buf[p - path] = '\0';
    if (stat(buf, &st) != 0 && mkdir(buf, 0755) != 0) return -1;
    if (p[1] == '\0') return 0;  // Path is a directory itself
  }

  return 1;
}

static void handle_put(struct connection *conn, const char *path) {
  file_stat_t st;
  const char *range, *cl_hdr = mg_get_header(&conn->mg_conn, "Content-Length");
  int64_t r1, r2;
  int rc;

  conn->mg_conn.status_code = !stat(path, &st) ? 200 : 201;
  if ((rc = put_dir(path)) == 0) {
    mg_printf(&conn->mg_conn, "HTTP/1.1 %d OK\r\n\r\n",
              conn->mg_conn.status_code);
    close_local_endpoint(conn);
  } else if (rc == -1) {
    send_http_error(conn, 500, "put_dir: %s", strerror(errno));
  } else if (cl_hdr == NULL) {
    send_http_error(conn, 411, NULL);
#ifdef _WIN32
    //On Windows, open() is a macro with 2 params
  } else if ((conn->endpoint.fd =
              open(path, O_RDWR | O_CREAT | O_TRUNC)) < 0) {
#else
  } else if ((conn->endpoint.fd =
              open(path, O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
#endif
    send_http_error(conn, 500, "open(%s): %s", path, strerror(errno));
  } else {
    DBG(("PUT [%s] %d", path, conn->local_iobuf.len));
    conn->endpoint_type = EP_PUT;
    set_close_on_exec(conn->endpoint.fd);
    range = mg_get_header(&conn->mg_conn, "Content-Range");
    conn->cl = to64(cl_hdr);
    r1 = r2 = 0;
    if (range != NULL && parse_range_header(range, &r1, &r2) > 0) {
      conn->mg_conn.status_code = 206;
      lseek(conn->endpoint.fd, r1, SEEK_SET);
      conn->cl = r2 > r1 ? r2 - r1 + 1: conn->cl - r1;
    }
    mg_printf(&conn->mg_conn, "HTTP/1.1 %d OK\r\nContent-Length: 0\r\n\r\n",
              conn->mg_conn.status_code);
  }
}

static void forward_put_data(struct connection *conn) {
  struct iobuf *io = &conn->local_iobuf;
  int n = write(conn->endpoint.fd, io->buf, io->len);
  if (n > 0) {
    discard_leading_iobuf_bytes(io, n);
    conn->cl -= n;
    if (conn->cl <= 0) {
      close_local_endpoint(conn);
    }
  }
}
#endif //  MONGOOSE_NO_DAV

static void send_options(struct connection *conn) {
  static const char reply[] = "HTTP/1.1 200 OK\r\nAllow: GET, POST, HEAD, "
    "CONNECT, PUT, DELETE, OPTIONS, PROPFIND, MKCOL\r\nDAV: 1\r\n\r\n";
  spool(&conn->remote_iobuf, reply, sizeof(reply) - 1);
  conn->flags |= CONN_SPOOL_DONE;
}

#ifndef MONGOOSE_NO_AUTH
void mg_send_digest_auth_request(struct mg_connection *c) {
  struct connection *conn = (struct connection *) c;
  c->status_code = 401;
  mg_printf(c,
            "HTTP/1.1 401 Unauthorized\r\n"
            "WWW-Authenticate: Digest qop=\"auth\", "
            "realm=\"%s\", nonce=\"%lu\"\r\n\r\n",
            conn->server->config_options[AUTH_DOMAIN],
            (unsigned long) time(NULL));
  close_local_endpoint(conn);
}

// Use the global passwords file, if specified by auth_gpass option,
// or search for .htpasswd in the requested directory.
static FILE *open_auth_file(struct connection *conn, const char *path) {
  char name[MAX_PATH_SIZE];
  const char *p, *gpass = conn->server->config_options[GLOBAL_AUTH_FILE];
  file_stat_t st;
  FILE *fp = NULL;

  if (gpass != NULL) {
    // Use global passwords file
    fp = fopen(gpass, "r");
  } else if (!stat(path, &st) && S_ISDIR(st.st_mode)) {
    mg_snprintf(name, sizeof(name), "%s%c%s", path, '/', PASSWORDS_FILE_NAME);
    fp = fopen(name, "r");
  } else {
    // Try to find .htpasswd in requested directory.
    if ((p = strrchr(path, '/')) == NULL) p = path;
    mg_snprintf(name, sizeof(name), "%.*s%c%s",
                (int) (p - path), path, '/', PASSWORDS_FILE_NAME);
    fp = fopen(name, "r");
  }

  return fp;
}

#if !defined(HAVE_MD5) && !defined(MONGOOSE_NO_AUTH)
typedef struct MD5Context {
  uint32_t buf[4];
  uint32_t bits[2];
  unsigned char in[64];
} MD5_CTX;

static void byteReverse(unsigned char *buf, unsigned longs) {
  uint32_t t;

  // Forrest: MD5 expect LITTLE_ENDIAN, swap if BIG_ENDIAN
  if (is_big_endian()) {
    do {
      t = (uint32_t) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
        ((unsigned) buf[1] << 8 | buf[0]);
      * (uint32_t *) buf = t;
      buf += 4;
    } while (--longs);
  }
}

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f, w, x, y, z, data, s) \
  ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

// Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
// initialization constants.
static void MD5Init(MD5_CTX *ctx) {
  ctx->buf[0] = 0x67452301;
  ctx->buf[1] = 0xefcdab89;
  ctx->buf[2] = 0x98badcfe;
  ctx->buf[3] = 0x10325476;

  ctx->bits[0] = 0;
  ctx->bits[1] = 0;
}

static void MD5Transform(uint32_t buf[4], uint32_t const in[16]) {
  register uint32_t a, b, c, d;

  a = buf[0];
  b = buf[1];
  c = buf[2];
  d = buf[3];

  MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
  MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
  MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
  MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
  MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
  MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
  MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
  MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
  MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
  MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
  MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
  MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
  MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
  MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
  MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
  MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

  MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
  MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
  MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
  MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
  MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
  MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
  MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
  MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
  MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
  MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
  MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
  MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
  MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
  MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
  MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
  MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

  MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
  MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
  MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
  MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
  MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
  MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
  MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
  MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
  MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
  MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
  MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
  MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
  MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
  MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
  MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
  MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

  MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
  MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
  MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
  MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
  MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
  MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
  MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
  MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
  MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
  MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
  MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
  MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
  MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
  MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
  MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
  MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}

static void MD5Update(MD5_CTX *ctx, unsigned char const *buf, unsigned len) {
  uint32_t t;

  t = ctx->bits[0];
  if ((ctx->bits[0] = t + ((uint32_t) len << 3)) < t)
    ctx->bits[1]++;
  ctx->bits[1] += len >> 29;

  t = (t >> 3) & 0x3f;

  if (t) {
    unsigned char *p = (unsigned char *) ctx->in + t;

    t = 64 - t;
    if (len < t) {
      memcpy(p, buf, len);
      return;
    }
    memcpy(p, buf, t);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32_t *) ctx->in);
    buf += t;
    len -= t;
  }

  while (len >= 64) {
    memcpy(ctx->in, buf, 64);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32_t *) ctx->in);
    buf += 64;
    len -= 64;
  }

  memcpy(ctx->in, buf, len);
}

static void MD5Final(unsigned char digest[16], MD5_CTX *ctx) {
  unsigned count;
  unsigned char *p;
  uint32_t *a;

  count = (ctx->bits[0] >> 3) & 0x3F;

  p = ctx->in + count;
  *p++ = 0x80;
  count = 64 - 1 - count;
  if (count < 8) {
    memset(p, 0, count);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32_t *) ctx->in);
    memset(ctx->in, 0, 56);
  } else {
    memset(p, 0, count - 8);
  }
  byteReverse(ctx->in, 14);

  a = (uint32_t *)ctx->in;
  a[14] = ctx->bits[0];
  a[15] = ctx->bits[1];

  MD5Transform(ctx->buf, (uint32_t *) ctx->in);
  byteReverse((unsigned char *) ctx->buf, 4);
  memcpy(digest, ctx->buf, 16);
  memset((char *) ctx, 0, sizeof(*ctx));
}
#endif // !HAVE_MD5



// Stringify binary data. Output buffer must be twice as big as input,
// because each byte takes 2 bytes in string representation
static void bin2str(char *to, const unsigned char *p, size_t len) {
  static const char *hex = "0123456789abcdef";

  for (; len--; p++) {
    *to++ = hex[p[0] >> 4];
    *to++ = hex[p[0] & 0x0f];
  }
  *to = '\0';
}

// Return stringified MD5 hash for list of strings. Buffer must be 33 bytes.
char *mg_md5(char buf[33], ...) {
  unsigned char hash[16];
  const char *p;
  va_list ap;
  MD5_CTX ctx;

  MD5Init(&ctx);

  va_start(ap, buf);
  while ((p = va_arg(ap, const char *)) != NULL) {
    MD5Update(&ctx, (const unsigned char *) p, (unsigned) strlen(p));
  }
  va_end(ap);

  MD5Final(hash, &ctx);
  bin2str(buf, hash, sizeof(hash));
  return buf;
}

// Check the user's password, return 1 if OK
static int check_password(const char *method, const char *ha1, const char *uri,
                          const char *nonce, const char *nc, const char *cnonce,
                          const char *qop, const char *response) {
  char ha2[32 + 1], expected_response[32 + 1];

#if 0
  // Check for authentication timeout
  if ((unsigned long) time(NULL) - (unsigned long) to64(nonce) > 3600) {
    return 0;
  }
#endif

  mg_md5(ha2, method, ":", uri, NULL);
  mg_md5(expected_response, ha1, ":", nonce, ":", nc,
      ":", cnonce, ":", qop, ":", ha2, NULL);

  return mg_strcasecmp(response, expected_response) == 0 ?
    MG_AUTH_OK : MG_AUTH_FAIL;
}


// Authorize against the opened passwords file. Return 1 if authorized.
int mg_authorize_digest(struct mg_connection *c, FILE *fp) {
  struct connection *conn = (struct connection *) c;
  const char *hdr;
  char line[256], f_user[256], ha1[256], f_domain[256], user[100], nonce[100],
       uri[MAX_REQUEST_SIZE], cnonce[100], resp[100], qop[100], nc[100];

  if (c == NULL || fp == NULL) return 0;
  if ((hdr = mg_get_header(c, "Authorization")) == NULL ||
      mg_strncasecmp(hdr, "Digest ", 7) != 0) return 0;
  if (!mg_parse_header(hdr, "username", user, sizeof(user))) return 0;
  if (!mg_parse_header(hdr, "cnonce", cnonce, sizeof(cnonce))) return 0;
  if (!mg_parse_header(hdr, "response", resp, sizeof(resp))) return 0;
  if (!mg_parse_header(hdr, "uri", uri, sizeof(uri))) return 0;
  if (!mg_parse_header(hdr, "qop", qop, sizeof(qop))) return 0;
  if (!mg_parse_header(hdr, "nc", nc, sizeof(nc))) return 0;
  if (!mg_parse_header(hdr, "nonce", nonce, sizeof(nonce))) return 0;

  while (fgets(line, sizeof(line), fp) != NULL) {
    if (sscanf(line, "%[^:]:%[^:]:%s", f_user, f_domain, ha1) == 3 &&
        !strcmp(user, f_user) &&
        // NOTE(lsm): due to a bug in MSIE, we do not compare URIs
        !strcmp(conn->server->config_options[AUTH_DOMAIN], f_domain))
      return check_password(c->request_method, ha1, uri,
                            nonce, nc, cnonce, qop, resp);
  }
  return MG_AUTH_FAIL;
}


// Return 1 if request is authorised, 0 otherwise.
static int is_authorized(struct connection *conn, const char *path) {
  FILE *fp;
  int authorized = MG_AUTH_OK;

  if ((fp = open_auth_file(conn, path)) != NULL) {
    authorized = mg_authorize_digest(&conn->mg_conn, fp);
    fclose(fp);
  }

  return authorized;
}

static int is_authorized_for_dav(struct connection *conn) {
  const char *auth_file = conn->server->config_options[DAV_AUTH_FILE];
  FILE *fp;
  int authorized = MG_AUTH_FAIL;

  if (auth_file != NULL && (fp = fopen(auth_file, "r")) != NULL) {
    authorized = mg_authorize_digest(&conn->mg_conn, fp);
    fclose(fp);
  }

  return authorized;
}

static int is_dav_mutation(const struct connection *conn) {
  const char *s = conn->mg_conn.request_method;
  return s && (!strcmp(s, "PUT") || !strcmp(s, "DELETE") ||
               !strcmp(s, "MKCOL"));
}
#endif // MONGOOSE_NO_AUTH

int parse_header(const char *str, int str_len, const char *var_name, char *buf,
                 size_t buf_size) {
  int ch = ' ', len = 0, n = strlen(var_name);
  const char *p, *end = str + str_len, *s = NULL;

  if (buf != NULL && buf_size > 0) buf[0] = '\0';

  // Find where variable starts
  for (s = str; s != NULL && s + n < end; s++) {
    if ((s == str || s[-1] == ' ' || s[-1] == ',') && s[n] == '=' &&
        !memcmp(s, var_name, n)) break;
  }

  if (s != NULL && &s[n + 1] < end) {
    s += n + 1;
    if (*s == '"' || *s == '\'') ch = *s++;
    p = s;
    while (p < end && p[0] != ch && p[0] != ',' && len < (int) buf_size) {
      if (p[0] == '\\' && p[1] == ch) p++;
      buf[len++] = *p++;
    }
    if (len >= (int) buf_size || (ch != ' ' && *p != ch)) {
      len = 0;
    } else {
      if (len > 0 && s[len - 1] == ',') len--;
      if (len > 0 && s[len - 1] == ';') len--;
      buf[len] = '\0';
    }
  }

  return len;
}

int mg_parse_header(const char *s, const char *var_name, char *buf,
                    size_t buf_size) {
  return parse_header(s, s == NULL ? 0 : strlen(s), var_name, buf, buf_size);
}

#ifdef MONGOOSE_USE_LUA
#include "lua_5.2.1.h"

#ifdef _WIN32
static void *mmap(void *addr, int64_t len, int prot, int flags, int fd,
                  int offset) {
  HANDLE fh = (HANDLE) _get_osfhandle(fd);
  HANDLE mh = CreateFileMapping(fh, 0, PAGE_READONLY, 0, 0, 0);
  void *p = MapViewOfFile(mh, FILE_MAP_READ, 0, 0, (size_t) len);
  CloseHandle(mh);
  return p;
}
#define munmap(x, y)  UnmapViewOfFile(x)
#define MAP_FAILED NULL
#define MAP_PRIVATE 0
#define PROT_READ 0
#else
#include <sys/mman.h>
#endif

static void reg_string(struct lua_State *L, const char *name, const char *val) {
  lua_pushstring(L, name);
  lua_pushstring(L, val);
  lua_rawset(L, -3);
}

static void reg_int(struct lua_State *L, const char *name, int val) {
  lua_pushstring(L, name);
  lua_pushinteger(L, val);
  lua_rawset(L, -3);
}

static void reg_function(struct lua_State *L, const char *name,
                         lua_CFunction func, struct mg_connection *conn) {
  lua_pushstring(L, name);
  lua_pushlightuserdata(L, conn);
  lua_pushcclosure(L, func, 1);
  lua_rawset(L, -3);
}

static int lua_write(lua_State *L) {
  int i, num_args;
  const char *str;
  size_t size;
  struct mg_connection *conn = (struct mg_connection *)
    lua_touserdata(L, lua_upvalueindex(1));

  num_args = lua_gettop(L);
  for (i = 1; i <= num_args; i++) {
    if (lua_isstring(L, i)) {
      str = lua_tolstring(L, i, &size);
      mg_write(conn, str, size);
    }
  }

  return 0;
}

static int lsp_sock_close(lua_State *L) {
  if (lua_gettop(L) > 0 && lua_istable(L, -1)) {
    lua_getfield(L, -1, "sock");
    closesocket((sock_t) lua_tonumber(L, -1));
  } else {
    return luaL_error(L, "invalid :close() call");
  }
  return 1;
}

static int lsp_sock_recv(lua_State *L) {
  char buf[2000];
  int n;

  if (lua_gettop(L) > 0 && lua_istable(L, -1)) {
    lua_getfield(L, -1, "sock");
    n = recv((sock_t) lua_tonumber(L, -1), buf, sizeof(buf), 0);
    if (n <= 0) {
      lua_pushnil(L);
    } else {
      lua_pushlstring(L, buf, n);
    }
  } else {
    return luaL_error(L, "invalid :close() call");
  }
  return 1;
}

static int lsp_sock_send(lua_State *L) {
  const char *buf;
  size_t len, sent = 0;
  int n, sock;

  if (lua_gettop(L) > 1 && lua_istable(L, -2) && lua_isstring(L, -1)) {
    buf = lua_tolstring(L, -1, &len);
    lua_getfield(L, -2, "sock");
    sock = (int) lua_tonumber(L, -1);
    while (sent < len) {
      if ((n = send(sock, buf + sent, len - sent, 0)) <= 0) break;
      sent += n;
    }
    lua_pushnumber(L, sent);
  } else {
    return luaL_error(L, "invalid :close() call");
  }
  return 1;
}

static const struct luaL_Reg luasocket_methods[] = {
  {"close", lsp_sock_close},
  {"send", lsp_sock_send},
  {"recv", lsp_sock_recv},
  {NULL, NULL}
};

static sock_t conn2(const char *host, int port) {
  struct sockaddr_in sin;
  struct hostent *he = NULL;
  sock_t sock = INVALID_SOCKET;

  if (host != NULL &&
      (he = gethostbyname(host)) != NULL &&
    (sock = socket(PF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET) {
    set_close_on_exec(sock);
    sin.sin_family = AF_INET;
    sin.sin_port = htons((uint16_t) port);
    sin.sin_addr = * (struct in_addr *) he->h_addr_list[0];
    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) != 0) {
      closesocket(sock);
      sock = INVALID_SOCKET;
    }
  }
  return sock;
}

static int lsp_connect(lua_State *L) {
  sock_t sock;

  if (lua_isstring(L, -2) && lua_isnumber(L, -1)) {
    sock = conn2(lua_tostring(L, -2), (int) lua_tonumber(L, -1));
    if (sock == INVALID_SOCKET) {
      lua_pushnil(L);
    } else {
      lua_newtable(L);
      reg_int(L, "sock", sock);
      reg_string(L, "host", lua_tostring(L, -4));
      luaL_getmetatable(L, "luasocket");
      lua_setmetatable(L, -2);
    }
  } else {
    return luaL_error(L, "connect(host,port): invalid parameter given.");
  }
  return 1;
}

static void prepare_lua_environment(struct mg_connection *ri, lua_State *L) {
  extern void luaL_openlibs(lua_State *);
  int i;

  luaL_openlibs(L);
#ifdef MONGOOSE_USE_LUA_SQLITE3
  { extern int luaopen_lsqlite3(lua_State *); luaopen_lsqlite3(L); }
#endif

  luaL_newmetatable(L, "luasocket");
  lua_pushliteral(L, "__index");
  luaL_newlib(L, luasocket_methods);
  lua_rawset(L, -3);
  lua_pop(L, 1);
  lua_register(L, "connect", lsp_connect);

  if (ri == NULL) return;

  // Register mg module
  lua_newtable(L);
  reg_function(L, "write", lua_write, ri);

  // Export request_info
  lua_pushstring(L, "request_info");
  lua_newtable(L);
  reg_string(L, "request_method", ri->request_method);
  reg_string(L, "uri", ri->uri);
  reg_string(L, "http_version", ri->http_version);
  reg_string(L, "query_string", ri->query_string);
  reg_string(L, "remote_ip", ri->remote_ip);
  reg_int(L, "remote_port", ri->remote_port);
  lua_pushstring(L, "content");
  lua_pushlstring(L, ri->content == NULL ? "" : ri->content, 0);
  lua_rawset(L, -3);
  reg_int(L, "content_len", ri->content_len);
  reg_int(L, "num_headers", ri->num_headers);
  lua_pushstring(L, "http_headers");
  lua_newtable(L);
  for (i = 0; i < ri->num_headers; i++) {
    reg_string(L, ri->http_headers[i].name, ri->http_headers[i].value);
  }
  lua_rawset(L, -3);
  lua_rawset(L, -3);

  lua_setglobal(L, "mg");

  // Register default mg.onerror function
  (void) luaL_dostring(L, "mg.onerror = function(e) mg.write('\\nLua "
                       "error:\\n', debug.traceback(e, 1)) end");
}

static int lua_error_handler(lua_State *L) {
  const char *error_msg =  lua_isstring(L, -1) ?  lua_tostring(L, -1) : "?\n";

  lua_getglobal(L, "mg");
  if (!lua_isnil(L, -1)) {
    lua_getfield(L, -1, "write");   // call mg.write()
    lua_pushstring(L, error_msg);
    lua_pushliteral(L, "\n");
    lua_call(L, 2, 0);
    (void) luaL_dostring(L, "mg.write(debug.traceback(), '\\n')");
  } else {
    printf("Lua error: [%s]\n", error_msg);
    (void) luaL_dostring(L, "print(debug.traceback(), '\\n')");
  }
  // TODO(lsm): leave the stack balanced

  return 0;
}

static void lsp(struct connection *conn, const char *p, int len, lua_State *L) {
  int i, j, pos = 0;

  for (i = 0; i < len; i++) {
    if (p[i] == '<' && p[i + 1] == '?') {
      for (j = i + 1; j < len ; j++) {
        if (p[j] == '?' && p[j + 1] == '>') {
          mg_write(&conn->mg_conn, p + pos, i - pos);
          if (luaL_loadbuffer(L, p + (i + 2), j - (i + 2), "") == LUA_OK) {
            lua_pcall(L, 0, LUA_MULTRET, 0);
          }
          pos = j + 2;
          i = pos - 1;
          break;
        }
      }
    }
  }
  if (i > pos) mg_write(&conn->mg_conn, p + pos, i - pos);
}

static void handle_lsp_request(struct connection *conn, const char *path,
                               file_stat_t *st) {
  void *p = NULL;
  lua_State *L = NULL;
  FILE *fp = NULL;

  if ((fp = fopen(path, "r")) == NULL ||
      (p = mmap(NULL, st->st_size, PROT_READ, MAP_PRIVATE,
                fileno(fp), 0)) == MAP_FAILED ||
      (L = luaL_newstate()) == NULL) {
    send_http_error(conn, 500, "mmap(%s): %s", path, strerror(errno));
  } else {
    // We're not sending HTTP headers here, Lua page must do it.
    prepare_lua_environment(&conn->mg_conn, L);
    lua_pushcclosure(L, &lua_error_handler, 0);
    lua_pushglobaltable(L);
    lsp(conn, p, (int) st->st_size, L);
    close_local_endpoint(conn);
  }

  if (L != NULL) lua_close(L);
  if (p != NULL) munmap(p, st->st_size);
  if (fp != NULL) fclose(fp);
}
#endif // MONGOOSE_USE_LUA

static void open_local_endpoint(struct connection *conn, int skip_user) {
#ifndef MONGOOSE_NO_FILESYSTEM
  static const char lua_pat[] = LUA_SCRIPT_PATTERN;
  file_stat_t st;
  char path[MAX_PATH_SIZE];
  int exists = 0, is_directory = 0;
#ifndef MONGOOSE_NO_CGI
  const char *cgi_pat = conn->server->config_options[CGI_PATTERN];
#else
  const char *cgi_pat = DEFAULT_CGI_PATTERN;
#endif
#ifndef MONGOOSE_NO_DIRECTORY_LISTING
  const char *dir_lst = conn->server->config_options[ENABLE_DIRECTORY_LISTING];
#else
  const char *dir_lst = "yes";
#endif
#endif

#ifndef MONGOOSE_NO_AUTH
  // Call auth handler
  if (conn->server->auth_handler != NULL &&
      conn->server->auth_handler(&conn->mg_conn) == MG_AUTH_FAIL) {
    mg_send_digest_auth_request(&conn->mg_conn);
    return;
  }
#endif

  // Call URI handler if one is registered for this URI
  if (skip_user == 0 && conn->server->request_handler != NULL) {
    conn->endpoint_type = EP_USER;
#if MONGOOSE_USE_POST_SIZE_LIMIT > 1
    {
      const char *cl = mg_get_header(&conn->mg_conn, "Content-Length");
      if (!strcmp(conn->mg_conn.request_method, "POST") &&
          (cl == NULL || to64(cl) > MONGOOSE_USE_POST_SIZE_LIMIT)) {
        send_http_error(conn, 500, "POST size > %zu",
                        (size_t) MONGOOSE_USE_POST_SIZE_LIMIT);
      }
    }
#endif
    return;
  }

#ifdef MONGOOSE_NO_FILESYSTEM
  if (!strcmp(conn->mg_conn.request_method, "OPTIONS")) {
    send_options(conn);
  } else {
    send_http_error(conn, 404, NULL);
  }
#else
  exists = convert_uri_to_file_name(conn, path, sizeof(path), &st);
  is_directory = S_ISDIR(st.st_mode);

  if (!strcmp(conn->mg_conn.request_method, "OPTIONS")) {
    send_options(conn);
  } else if (conn->server->config_options[DOCUMENT_ROOT] == NULL) {
    send_http_error(conn, 404, NULL);
#ifndef MONGOOSE_NO_AUTH
  } else if ((!is_dav_mutation(conn) && !is_authorized(conn, path)) ||
             (is_dav_mutation(conn) && !is_authorized_for_dav(conn))) {
    mg_send_digest_auth_request(&conn->mg_conn);
    close_local_endpoint(conn);
#endif
#ifndef MONGOOSE_NO_DAV
  } else if (!strcmp(conn->mg_conn.request_method, "PROPFIND")) {
    handle_propfind(conn, path, &st);
  } else if (!strcmp(conn->mg_conn.request_method, "MKCOL")) {
    handle_mkcol(conn, path);
  } else if (!strcmp(conn->mg_conn.request_method, "DELETE")) {
    handle_delete(conn, path);
  } else if (!strcmp(conn->mg_conn.request_method, "PUT")) {
    handle_put(conn, path);
#endif
  } else if (!exists || must_hide_file(conn, path)) {
    send_http_error(conn, 404, NULL);
  } else if (is_directory &&
             conn->mg_conn.uri[strlen(conn->mg_conn.uri) - 1] != '/') {
    conn->mg_conn.status_code = 301;
    mg_printf(&conn->mg_conn, "HTTP/1.1 301 Moved Permanently\r\n"
              "Location: %s/\r\n\r\n", conn->mg_conn.uri);
    close_local_endpoint(conn);
  } else if (is_directory && !find_index_file(conn, path, sizeof(path), &st)) {
    if (!mg_strcasecmp(dir_lst, "yes")) {
#ifndef MONGOOSE_NO_DIRECTORY_LISTING
      send_directory_listing(conn, path);
#else
      send_http_error(conn, 501, NULL);
#endif
    } else {
      send_http_error(conn, 403, NULL);
    }
  } else if (match_prefix(lua_pat, sizeof(lua_pat) - 1, path) > 0) {
#ifdef MONGOOSE_USE_LUA
    handle_lsp_request(conn, path, &st);
#else
    send_http_error(conn, 501, NULL);
#endif
  } else if (match_prefix(cgi_pat, strlen(cgi_pat), path) > 0) {
#if !defined(MONGOOSE_NO_CGI)
    open_cgi_endpoint(conn, path);
#else
    send_http_error(conn, 501, NULL);
#endif // !MONGOOSE_NO_CGI
  } else if (is_not_modified(conn, &st)) {
    send_http_error(conn, 304, NULL);
  } else if ((conn->endpoint.fd = open(path, O_RDONLY | O_BINARY)) != -1) {
    // O_BINARY is required for Windows, otherwise in default text mode
    // two bytes \r\n will be read as one.
    open_file_endpoint(conn, path, &st);
  } else {
    send_http_error(conn, 404, NULL);
  }
#endif  // MONGOOSE_NO_FILESYSTEM
}

static void send_continue_if_expected(struct connection *conn) {
  static const char expect_response[] = "HTTP/1.1 100 Continue\r\n\r\n";
  const char *expect_hdr = mg_get_header(&conn->mg_conn, "Expect");

  if (expect_hdr != NULL && !mg_strcasecmp(expect_hdr, "100-continue")) {
    spool(&conn->remote_iobuf, expect_response, sizeof(expect_response) - 1);
  }
}

static int is_valid_uri(const char *uri) {
  // Conform to http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.1.2
  // URI can be an asterisk (*) or should start with slash.
  return uri[0] == '/' || (uri[0] == '*' && uri[1] == '\0');
}

static void try_http_parse_and_set_content_length(struct connection *conn) {
  struct iobuf *io = &conn->local_iobuf;

  if (conn->request_len == 0 &&
      (conn->request_len = get_request_len(io->buf, io->len)) > 0) {
    // If request is buffered in, remove it from the iobuf. This is because
    // iobuf could be reallocated, and pointers in parsed request could
    // become invalid.
    conn->request = (char *) malloc(conn->request_len);
    memcpy(conn->request, io->buf, conn->request_len);
    DBG(("%p [%.*s]", conn, conn->request_len, conn->request));
    discard_leading_iobuf_bytes(io, conn->request_len);
    conn->request_len = parse_http_message(conn->request, conn->request_len,
                                           &conn->mg_conn);
    if (conn->request_len > 0) {
      const char *cl_hdr = mg_get_header(&conn->mg_conn, "Content-Length");
      conn->cl = cl_hdr == NULL ? 0 : to64(cl_hdr);
      conn->mg_conn.content_len = (long int) conn->cl;
    }
  }
}

static void process_request(struct connection *conn) {
  struct iobuf *io = &conn->local_iobuf;

  try_http_parse_and_set_content_length(conn);
  DBG(("%p %d %d %d [%.*s]", conn, conn->request_len, io->len, conn->flags,
       io->len, io->buf));
  if (conn->request_len < 0 ||
      (conn->request_len > 0 && !is_valid_uri(conn->mg_conn.uri))) {
    send_http_error(conn, 400, NULL);
  } else if (conn->request_len == 0 && io->len > MAX_REQUEST_SIZE) {
    send_http_error(conn, 413, NULL);
  } else if (conn->request_len > 0 &&
             strcmp(conn->mg_conn.http_version, "1.0") != 0 &&
             strcmp(conn->mg_conn.http_version, "1.1") != 0) {
    send_http_error(conn, 505, NULL);
  } else if (conn->request_len > 0 && conn->endpoint_type == EP_NONE) {
#ifndef MONGOOSE_NO_WEBSOCKET
    send_websocket_handshake_if_requested(&conn->mg_conn);
#endif
    send_continue_if_expected(conn);
    open_local_endpoint(conn, 0);
  }

#ifndef MONGOOSE_NO_CGI
  if (conn->endpoint_type == EP_CGI && io->len > 0) {
    forward_post_data(conn);
  }
#endif
  if (conn->endpoint_type == EP_USER) {
    call_request_handler_if_data_is_buffered(conn);
  }
#ifndef MONGOOSE_NO_DAV
  if (conn->endpoint_type == EP_PUT && io->len > 0) {
    forward_put_data(conn);
  }
#endif
}

static void call_http_client_handler(struct connection *conn, int code) {
  conn->mg_conn.status_code = code;
  // For responses without Content-Lengh, use the whole buffer
  if (conn->cl == 0 && code == MG_DOWNLOAD_SUCCESS) {
    conn->mg_conn.content_len = conn->local_iobuf.len;
  }
  conn->mg_conn.content = conn->local_iobuf.buf;
  if (conn->handler(&conn->mg_conn) || code == MG_CONNECT_FAILURE ||
      code == MG_DOWNLOAD_FAILURE) {
    conn->flags |= CONN_CLOSE;
  }
  discard_leading_iobuf_bytes(&conn->local_iobuf, conn->mg_conn.content_len);
  conn->flags = conn->mg_conn.status_code = 0;
  conn->cl = conn->num_bytes_sent = conn->request_len = 0;
  free(conn->request);
  conn->request = NULL;
}

static void process_response(struct connection *conn) {
  struct iobuf *io = &conn->local_iobuf;

  try_http_parse_and_set_content_length(conn);
  DBG(("%p %d %d [%.*s]", conn, conn->request_len, io->len,
       io->len > 40 ? 40 : io->len, io->buf));
  if (conn->request_len < 0 ||
      (conn->request_len == 0 && io->len > MAX_REQUEST_SIZE)) {
    call_http_client_handler(conn, MG_DOWNLOAD_FAILURE);
  }
  if (io->len >= conn->cl) {
    call_http_client_handler(conn, MG_DOWNLOAD_SUCCESS);
  }
}

static void read_from_socket(struct connection *conn) {
  char buf[IOBUF_SIZE];
  int n = 0;

  if (conn->endpoint_type == EP_CLIENT && conn->flags & CONN_CONNECTING) {
    callback_http_client_on_connect(conn);
    return;
  }

#ifdef MONGOOSE_USE_SSL
  if (conn->ssl != NULL) {
    if (conn->flags & CONN_SSL_HANDS_SHAKEN) {
      n = SSL_read(conn->ssl, buf, sizeof(buf));
    } else {
      if (SSL_accept(conn->ssl) == 1) {
        conn->flags |= CONN_SSL_HANDS_SHAKEN;
      }
      return;
    }
  } else
#endif
  {
    n = recv(conn->client_sock, buf, sizeof(buf), 0);
  }

  DBG(("%p %d %d (1)", conn, n, conn->flags));
  if (is_error(n)) {
    if (conn->endpoint_type == EP_CLIENT && conn->local_iobuf.len > 0) {
      call_http_client_handler(conn, MG_DOWNLOAD_SUCCESS);
    }
    conn->flags |= CONN_CLOSE;
  } else if (n > 0) {
    spool(&conn->local_iobuf, buf, n);
    if (conn->endpoint_type == EP_CLIENT) {
      process_response(conn);
    } else {
      process_request(conn);
    }
  }
  DBG(("%p %d %d (2)", conn, n, conn->flags));
}

int mg_connect(struct mg_server *server, const char *host, int port,
               int use_ssl, mg_handler_t handler, void *param) {
  sock_t sock = INVALID_SOCKET;
  struct sockaddr_in sin;
  struct hostent *he = NULL;
  struct connection *conn = NULL;
  int connect_ret_val;

  if (host == NULL || (he = gethostbyname(host)) == NULL ||
      (sock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) return 0;
#ifndef MONGOOSE_USE_SSL
  if (use_ssl) return 0;
#endif

  sin.sin_family = AF_INET;
  sin.sin_port = htons((uint16_t) port);
  sin.sin_addr = * (struct in_addr *) he->h_addr_list[0];
  set_non_blocking_mode(sock);

  connect_ret_val = connect(sock, (struct sockaddr *) &sin, sizeof(sin));
  if (is_error(connect_ret_val)) {
    return 0;
  } else if ((conn = (struct connection *) calloc(1, sizeof(*conn))) == NULL) {
    closesocket(sock);
    return 0;
  }

  conn->server = server;
  conn->client_sock = sock;
  conn->endpoint_type = EP_CLIENT;
  conn->handler = handler;
  conn->mg_conn.server_param = server->server_data;
  conn->mg_conn.connection_param = param;
  conn->birth_time = conn->last_activity_time = time(NULL);
  conn->flags = CONN_CONNECTING;
  conn->mg_conn.status_code = MG_CONNECT_FAILURE;
#ifdef MONGOOSE_USE_SSL
  if (use_ssl && (conn->ssl = SSL_new(server->client_ssl_ctx)) != NULL) {
    SSL_set_fd(conn->ssl, sock);
  }
#endif
  LINKED_LIST_ADD_TO_FRONT(&server->active_connections, &conn->link);
  DBG(("%p %s:%d", conn, host, port));

  return 1;
}

#ifndef MONGOOSE_NO_LOGGING
static void log_header(const struct mg_connection *conn, const char *header,
                       FILE *fp) {
  const char *header_value;

  if ((header_value = mg_get_header(conn, header)) == NULL) {
    (void) fprintf(fp, "%s", " -");
  } else {
    (void) fprintf(fp, " \"%s\"", header_value);
  }
}

static void log_access(const struct connection *conn, const char *path) {
  const struct mg_connection *c = &conn->mg_conn;
  FILE *fp = (path == NULL) ?  NULL : fopen(path, "a+");
  char date[64], user[100];

  if (fp == NULL) return;
  strftime(date, sizeof(date), "%d/%b/%Y:%H:%M:%S %z",
           localtime(&conn->birth_time));

  flockfile(fp);
  mg_parse_header(mg_get_header(&conn->mg_conn, "Authorization"), "username",
                  user, sizeof(user));
  fprintf(fp, "%s - %s [%s] \"%s %s HTTP/%s\" %d %" INT64_FMT,
          c->remote_ip, user[0] == '\0' ? "-" : user, date,
          c->request_method ? c->request_method : "-",
          c->uri ? c->uri : "-", c->http_version,
          c->status_code, conn->num_bytes_sent);
  log_header(c, "Referer", fp);
  log_header(c, "User-Agent", fp);
  fputc('\n', fp);
  fflush(fp);

  funlockfile(fp);
  fclose(fp);
}
#endif

static void close_local_endpoint(struct connection *conn) {
  struct mg_connection *c = &conn->mg_conn;
  // Must be done before free()
  int keep_alive = should_keep_alive(&conn->mg_conn) &&
    (conn->endpoint_type == EP_FILE || conn->endpoint_type == EP_USER);
  DBG(("%p %d %d %d", conn, conn->endpoint_type, keep_alive, conn->flags));

  switch (conn->endpoint_type) {
    case EP_PUT: close(conn->endpoint.fd); break;
    case EP_FILE: close(conn->endpoint.fd); break;
    case EP_CGI: closesocket(conn->endpoint.cgi_sock); break;
    default: break;
  }

#ifndef MONGOOSE_NO_LOGGING
  if (c->status_code > 0 && conn->endpoint_type != EP_CLIENT &&
      c->status_code != 400) {
    log_access(conn, conn->server->config_options[ACCESS_LOG_FILE]);
  }
#endif

  // Gobble possible POST data sent to the URI handler
  discard_leading_iobuf_bytes(&conn->local_iobuf, conn->mg_conn.content_len);
  conn->endpoint_type = EP_NONE;
  conn->cl = conn->num_bytes_sent = conn->request_len = conn->flags = 0;
  c->request_method = c->uri = c->http_version = c->query_string = NULL;
  c->num_headers = c->status_code = c->is_websocket = c->content_len = 0;
  free(conn->request);
  conn->request = NULL;

  if (keep_alive) {
    process_request(conn);  // Can call us recursively if pipelining is used
  } else {
    conn->flags |= conn->remote_iobuf.len == 0 ? CONN_CLOSE : CONN_SPOOL_DONE;
  }
}

static void transfer_file_data(struct connection *conn) {
  char buf[IOBUF_SIZE];
  int n = read(conn->endpoint.fd, buf, conn->cl < (int64_t) sizeof(buf) ?
               (int) conn->cl : (int) sizeof(buf));

  if (is_error(n)) {
    close_local_endpoint(conn);
  } else if (n > 0) {
    conn->cl -= n;
    spool(&conn->remote_iobuf, buf, n);
    if (conn->cl <= 0) {
      close_local_endpoint(conn);
    }
  }
}

static void execute_iteration(struct mg_server *server) {
  struct ll *lp, *tmp;
  struct connection *conn;
  union { mg_handler_t f; void *p; } msg[2];

  recv(server->ctl[1], (void *) msg, sizeof(msg), 0);
  LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
    conn = LINKED_LIST_ENTRY(lp, struct connection, link);
    conn->mg_conn.connection_param = msg[1].p;
    msg[0].f(&conn->mg_conn);
  }
}

void add_to_set(sock_t sock, fd_set *set, sock_t *max_fd) {
  FD_SET(sock, set);
  if (sock > *max_fd) {
    *max_fd = sock;
  }
}

unsigned int mg_poll_server(struct mg_server *server, int milliseconds) {
  struct ll *lp, *tmp;
  struct connection *conn;
  struct timeval tv;
  fd_set read_set, write_set;
  sock_t max_fd = -1;
  time_t current_time = time(NULL), expire_time = current_time -
    MONGOOSE_USE_IDLE_TIMEOUT_SECONDS;

  if (server->listening_sock == INVALID_SOCKET) return 0;

  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  add_to_set(server->listening_sock, &read_set, &max_fd);
  add_to_set(server->ctl[1], &read_set, &max_fd);

  LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
    conn = LINKED_LIST_ENTRY(lp, struct connection, link);
    add_to_set(conn->client_sock, &read_set, &max_fd);
    if (conn->endpoint_type == EP_CLIENT && (conn->flags & CONN_CONNECTING)) {
      add_to_set(conn->client_sock, &write_set, &max_fd);
    }
    if (conn->endpoint_type == EP_FILE) {
      transfer_file_data(conn);
    } else if (conn->endpoint_type == EP_CGI) {
      add_to_set(conn->endpoint.cgi_sock, &read_set, &max_fd);
    }
    if (conn->remote_iobuf.len > 0 && !(conn->flags & CONN_BUFFER)) {
      add_to_set(conn->client_sock, &write_set, &max_fd);
    } else if (conn->flags & CONN_CLOSE) {
      close_conn(conn);
    }
  }

  tv.tv_sec = milliseconds / 1000;
  tv.tv_usec = (milliseconds % 1000) * 1000;

  if (select(max_fd + 1, &read_set, &write_set, NULL, &tv) > 0) {
    if (FD_ISSET(server->ctl[1], &read_set)) {
      execute_iteration(server);
    }

    // Accept new connections
    if (FD_ISSET(server->listening_sock, &read_set)) {
      while ((conn = accept_new_connection(server)) != NULL) {
        conn->birth_time = conn->last_activity_time = current_time;
      }
    }

    // Read/write from clients
    LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
      conn = LINKED_LIST_ENTRY(lp, struct connection, link);
      if (FD_ISSET(conn->client_sock, &read_set)) {
        conn->last_activity_time = current_time;
        read_from_socket(conn);
      }
#ifndef MONGOOSE_NO_CGI
      if (conn->endpoint_type == EP_CGI &&
          FD_ISSET(conn->endpoint.cgi_sock, &read_set)) {
        read_from_cgi(conn);
      }
#endif
      if (FD_ISSET(conn->client_sock, &write_set)) {
        if (conn->endpoint_type == EP_CLIENT &&
            (conn->flags & CONN_CONNECTING)) {
          read_from_socket(conn);
        } else if (!(conn->flags & CONN_BUFFER)) {
          conn->last_activity_time = current_time;
          write_to_socket(conn);
        }
      }
    }
  }

  // Close expired connections and those that need to be closed
  LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
    conn = LINKED_LIST_ENTRY(lp, struct connection, link);
    if (conn->mg_conn.is_websocket) {
      ping_idle_websocket_connection(conn, current_time);
    }
    if (conn->flags & CONN_LONG_RUNNING) {
      conn->mg_conn.wsbits = conn->flags & CONN_CLOSE ? 1 : 0;
      if (call_request_handler(conn) == MG_REQUEST_PROCESSED) {
        conn->flags |= CONN_CLOSE;
      }
    }
    if (conn->flags & CONN_CLOSE || conn->last_activity_time < expire_time) {
      close_conn(conn);
    }
  }

  return (unsigned int) current_time;
}

void mg_destroy_server(struct mg_server **server) {
  int i;
  struct ll *lp, *tmp;

  if (server != NULL && *server != NULL) {
    struct mg_server *s = *server;
    // Do one last poll, see https://github.com/cesanta/mongoose/issues/286
    mg_poll_server(s, 0);
    closesocket(s->listening_sock);
    closesocket(s->ctl[0]);
    closesocket(s->ctl[1]);
    LINKED_LIST_FOREACH(&s->active_connections, lp, tmp) {
      close_conn(LINKED_LIST_ENTRY(lp, struct connection, link));
    }
    for (i = 0; i < (int) ARRAY_SIZE(s->config_options); i++) {
      free(s->config_options[i]);  // It is OK to free(NULL)
    }
#ifdef MONGOOSE_USE_SSL
    if (s->ssl_ctx != NULL) SSL_CTX_free((*server)->ssl_ctx);
    if (s->client_ssl_ctx != NULL) SSL_CTX_free(s->client_ssl_ctx);
#endif
    free(s);
    *server = NULL;
  }
}

// Apply function to all active connections.
void mg_iterate_over_connections(struct mg_server *server, mg_handler_t handler,
                                 void *param) {
  // Send closure (function + parameter) to the IO thread to execute
  union { mg_handler_t f; void *p; } msg[2];
  msg[0].f = handler;
  msg[1].p = param;
  send(server->ctl[0], (void *) msg, sizeof(msg), 0);
}

static int get_var(const char *data, size_t data_len, const char *name,
                   char *dst, size_t dst_len) {
  const char *p, *e, *s;
  size_t name_len;
  int len;

  if (dst == NULL || dst_len == 0) {
    len = -2;
  } else if (data == NULL || name == NULL || data_len == 0) {
    len = -1;
    dst[0] = '\0';
  } else {
    name_len = strlen(name);
    e = data + data_len;
    len = -1;
    dst[0] = '\0';

    // data is "var1=val1&var2=val2...". Find variable first
    for (p = data; p + name_len < e; p++) {
      if ((p == data || p[-1] == '&') && p[name_len] == '=' &&
          !mg_strncasecmp(name, p, name_len)) {

        // Point p to variable value
        p += name_len + 1;

        // Point s to the end of the value
        s = (const char *) memchr(p, '&', (size_t)(e - p));
        if (s == NULL) {
          s = e;
        }
        assert(s >= p);

        // Decode variable into destination buffer
        len = mg_url_decode(p, (size_t)(s - p), dst, dst_len, 1);

        // Redirect error code from -1 to -2 (destination buffer too small).
        if (len == -1) {
          len = -2;
        }
        break;
      }
    }
  }

  return len;
}

int mg_get_var(const struct mg_connection *conn, const char *name,
               char *dst, size_t dst_len) {
  int len = get_var(conn->query_string, conn->query_string == NULL ? 0 :
                    strlen(conn->query_string), name, dst, dst_len);
  if (len < 0) {
    len = get_var(conn->content, conn->content_len, name, dst, dst_len);
  }
  return len;
}

static int get_line_len(const char *buf, int buf_len) {
  int len = 0;
  while (len < buf_len && buf[len] != '\n') len++;
  return buf[len] == '\n' ? len + 1: -1;
}

int mg_parse_multipart(const char *buf, int buf_len,
                       char *var_name, int var_name_len,
                       char *file_name, int file_name_len,
                       const char **data, int *data_len) {
  static const char cd[] = "Content-Disposition: ";
  //struct mg_connection c;
  int hl, bl, n, ll, pos, cdl = sizeof(cd) - 1;
  //char *p;

  if (buf == NULL || buf_len <= 0) return 0;
  if ((hl = get_request_len(buf, buf_len)) <= 0) return 0;
  if (buf[0] != '-' || buf[1] != '-' || buf[2] == '\n') return 0;

  // Get boundary length
  bl = get_line_len(buf, buf_len);

  // Loop through headers, fetch variable name and file name
  var_name[0] = file_name[0] = '\0';
  for (n = bl; (ll = get_line_len(buf + n, hl - n)) > 0; n += ll) {
    if (mg_strncasecmp(cd, buf + n, cdl) == 0) {
      parse_header(buf + n + cdl, ll - (cdl + 2), "name",
                   var_name, var_name_len);
      parse_header(buf + n + cdl, ll - (cdl + 2), "filename",
                   file_name, file_name_len);
    }
  }

  // Scan body, search for terminating boundary
  for (pos = hl; pos + (bl - 2) < buf_len; pos++) {
    if (buf[pos] == '-' && !memcmp(buf, &buf[pos], bl - 2)) {
      if (data_len != NULL) *data_len = (pos - 2) - hl;
      if (data != NULL) *data = buf + hl;
      return pos;
    }
  }

  return 0;
}

const char **mg_get_valid_option_names(void) {
  return static_config_options;
}

static int get_option_index(const char *name) {
  int i;

  for (i = 0; static_config_options[i * 2] != NULL; i++) {
    if (strcmp(static_config_options[i * 2], name) == 0) {
      return i;
    }
  }
  return -1;
}

static void set_default_option_values(char **opts) {
  const char *value, **all_opts = mg_get_valid_option_names();
  int i;

  for (i = 0; all_opts[i * 2] != NULL; i++) {
    value = all_opts[i * 2 + 1];
    if (opts[i] == NULL && value != NULL) {
      opts[i] = mg_strdup(value);
    }
  }
}

// Valid listening port spec is: [ip_address:]port, e.g. "80", "127.0.0.1:3128"
static int parse_port_string(const char *str, union socket_address *sa) {
  unsigned int a, b, c, d, port;
  int len = 0;
#ifdef MONGOOSE_USE_IPV6
  char buf[100];
#endif

  // MacOS needs that. If we do not zero it, subsequent bind() will fail.
  // Also, all-zeroes in the socket address means binding to all addresses
  // for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT).
  memset(sa, 0, sizeof(*sa));
  sa->sin.sin_family = AF_INET;

  if (sscanf(str, "%u.%u.%u.%u:%u%n", &a, &b, &c, &d, &port, &len) == 5) {
    // Bind to a specific IPv4 address, e.g. 192.168.1.5:8080
    sa->sin.sin_addr.s_addr = htonl((a << 24) | (b << 16) | (c << 8) | d);
    sa->sin.sin_port = htons((uint16_t) port);
#if defined(MONGOOSE_USE_IPV6)
  } else if (sscanf(str, "[%49[^]]]:%u%n", buf, &port, &len) == 2 &&
             inet_pton(AF_INET6, buf, &sa->sin6.sin6_addr)) {
    // IPv6 address, e.g. [3ffe:2a00:100:7031::1]:8080
    sa->sin6.sin6_family = AF_INET6;
    sa->sin6.sin6_port = htons((uint16_t) port);
#endif
  } else if (sscanf(str, "%u%n", &port, &len) == 1) {
    // If only port is specified, bind to IPv4, INADDR_ANY
    sa->sin.sin_port = htons((uint16_t) port);
  } else {
    port = 0;   // Parsing failure. Make port invalid.
  }

  return port <= 0xffff && str[len] == '\0';
}

const char *mg_set_option(struct mg_server *server, const char *name,
                          const char *value) {
  int ind = get_option_index(name);
  const char *error_msg = NULL;

  if (ind < 0) {
    error_msg = "No such option";
  } else {
    if (server->config_options[ind] != NULL) {
      free(server->config_options[ind]);
    }
    server->config_options[ind] = mg_strdup(value);
    DBG(("%s [%s]", name, value));

    if (ind == LISTENING_PORT) {
      if (server->listening_sock != INVALID_SOCKET) {
        closesocket(server->listening_sock);
      }
      parse_port_string(server->config_options[LISTENING_PORT], &server->lsa);
      server->listening_sock = open_listening_socket(&server->lsa);
      if (server->listening_sock == INVALID_SOCKET) {
        error_msg = "Cannot bind to port";
      } else {
        sockaddr_to_string(server->local_ip, sizeof(server->local_ip),
                           &server->lsa);
        if (!strcmp(value, "0")) {
          char buf[10];
          mg_snprintf(buf, sizeof(buf), "%d",
                      (int) ntohs(server->lsa.sin.sin_port));
          free(server->config_options[ind]);
          server->config_options[ind] = mg_strdup(buf);
        }
      }
#ifndef _WIN32
    } else if (ind == RUN_AS_USER) {
      struct passwd *pw;
      if ((pw = getpwnam(value)) == NULL) {
        error_msg = "Unknown user";
      } else if (setgid(pw->pw_gid) != 0) {
        error_msg = "setgid() failed";
      } else if (setuid(pw->pw_uid) != 0) {
        error_msg = "setuid() failed";
      }
#endif
#ifdef MONGOOSE_USE_SSL
    } else if (ind == SSL_CERTIFICATE) {
      //SSL_library_init();
      if ((server->ssl_ctx = SSL_CTX_new(SSLv23_server_method())) == NULL) {
        error_msg = "SSL_CTX_new() failed";
      } else if (SSL_CTX_use_certificate_file(server->ssl_ctx, value, 1) == 0 ||
                 SSL_CTX_use_PrivateKey_file(server->ssl_ctx, value, 1) == 0) {
        error_msg = "Cannot load PEM file";
      } else {
        SSL_CTX_use_certificate_chain_file(server->ssl_ctx, value);
      }
#endif
    }
  }

  return error_msg;
}

void mg_set_request_handler(struct mg_server *server, mg_handler_t handler) {
  server->request_handler = handler;
}

void mg_set_http_error_handler(struct mg_server *server, mg_handler_t handler) {
  server->error_handler = handler;
}

void mg_set_auth_handler(struct mg_server *server, mg_handler_t handler) {
  server->auth_handler = handler;
}

void mg_set_listening_socket(struct mg_server *server, int sock) {
  if (server->listening_sock != INVALID_SOCKET) {
    closesocket(server->listening_sock);
  }
  server->listening_sock = (sock_t) sock;
}

int mg_get_listening_socket(struct mg_server *server) {
  return server->listening_sock;
}

const char *mg_get_option(const struct mg_server *server, const char *name) {
  const char **opts = (const char **) server->config_options;
  int i = get_option_index(name);
  return i == -1 ? NULL : opts[i] == NULL ? "" : opts[i];
}

struct mg_server *mg_create_server(void *server_data) {
  struct mg_server *server = (struct mg_server *) calloc(1, sizeof(*server));

#ifdef _WIN32
  WSADATA data;
  WSAStartup(MAKEWORD(2, 2), &data);
#else
  // Ignore SIGPIPE signal, so if browser cancels the request, it
  // won't kill the whole process.
  signal(SIGPIPE, SIG_IGN);
#endif

  LINKED_LIST_INIT(&server->active_connections);

  // Create control socket pair. Do it in a loop to protect from
  // interrupted syscalls in mg_socketpair().
  do {
    mg_socketpair(server->ctl);
  } while (server->ctl[0] == INVALID_SOCKET);

#ifdef MONGOOSE_USE_SSL
  SSL_library_init();
  server->client_ssl_ctx = SSL_CTX_new(SSLv23_client_method());
#endif

  server->server_data = server_data;
  server->listening_sock = INVALID_SOCKET;
  set_default_option_values(server->config_options);

  return server;
}
