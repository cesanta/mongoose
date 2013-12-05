// Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>
// Copyright (c) 2013 Cesanta Software Limited
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
// license, as set out in <http://cesanta.com/products.html>.

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
typedef int socklen_t;
typedef int pid_t;
typedef SOCKET sock_t;
typedef unsigned int  uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned __int64 uint64_t;
typedef __int64   int64_t;
typedef CRITICAL_SECTION mutex_t;
typedef struct _stati64 file_stat_t;
#pragma comment(lib, "ws2_32.lib")
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define INT64_FMT  "I64d"
#define mutex_init(x) InitializeCriticalSection(x)
#define mutex_destroy(x) DeleteCriticalSection(x)
#define mutex_lock(x) EnterCriticalSection(x)
#define mutex_unlock(x) LeaveCriticalSection(x)
#define S_ISDIR(x) ((x) & _S_IFDIR)
#define sleep(x) Sleep((x) * 1000)
#define stat(x, y) _stati64((x), (y))
#define to64(x) _atoi64(x)
#ifndef va_copy
#define va_copy(x,y) x = y
#endif // MINGW #defines va_copy
#ifndef __func__
#define __func__ ""
#endif
#else
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
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
#define INVALID_SOCKET ((sock_t) -1)
#define INT64_FMT PRId64
#define to64(x) strtoll(x, NULL, 10)
#endif

//#include "mongoose.h"
#include "core.h"

struct linked_list_link { struct linked_list_link *prev, *next; };
#define LINKED_LIST_INIT(N)  ((N)->next = (N)->prev = (N))
#define LINKED_LIST_DECLARE_AND_INIT(H)  struct linked_list_link H = { &H, &H }
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
#define LUA_SCRIPT_PATTERN "mg_*.lua$"

// Extra HTTP headers to send in every static file reply
#if !defined(EXTRA_HTTP_HEADERS)
#define EXTRA_HTTP_HEADERS ""
#endif

#ifdef ENABLE_DBG
#define DBG(x) do { printf("%s::%s ", __FILE__, __func__); \
  printf x; putchar('\n'); fflush(stdout); } while(0)
#else
#define DBG(x)
#endif

union socket_address {
  struct sockaddr sa;
  struct sockaddr_in sin;
#ifdef USE_IPV6
  struct sockaddr_in6 sin6;
#endif
};

struct vec {
  const char *ptr;
  int len;
};

struct uri_handler {
  struct linked_list_link link;
  char *uri;
  mg_uri_handler_t handler;
};

// NOTE(lsm): this enum shoulds be in sync with the config_options.
enum {
  ACCESS_CONTROL_LIST, ACCESS_LOG_FILE, AUTH_DOMAIN, CGI_INTERPRETER,
  CGI_PATTERN, DOCUMENT_ROOT, ENABLE_DIRECTORY_LISTING, ERROR_LOG_FILE,
  EXTRA_MIME_TYPES, GLOBAL_AUTH_FILE, HIDE_FILES_PATTERN, IDLE_TIMEOUT_MS,
  INDEX_FILES, LISTENING_PORT, NUM_THREADS, PUT_DELETE_AUTH_FILE, RUN_AS_USER,
  SSL_CERTIFICATE, SSI_PATTERN, URL_REWRITES, NUM_OPTIONS
};

static const char *static_config_options[] = {
  "access_control_list", NULL,
  "access_log_file", NULL,
  "auth_domain", "mydomain.com",
  "cgi_interpreter", NULL,
  "cgi_pattern", "**.cgi$|**.pl$|**.php$",
  "document_root",  NULL,
  "enable_directory_listing", "yes",
  "error_log_file", NULL,
  "extra_mime_types", NULL,
  "global_auth_file", NULL,
  "hide_files_patterns", NULL,
  "idle_timeout_ms", "30000",
  "index_files","index.html,index.htm,index.cgi,index.shtml,index.php,index.lp",
  "listening_port", NULL,
  "num_threads", "50",
  "put_delete_auth_file", NULL,
  "run_as_user", NULL,
  "ssl_certificate", NULL,
  "ssi_pattern", "**.shtml$|**.shtm$",
  "url_rewrites", NULL,
  NULL
};

struct mg_server {
  sock_t listening_sock;
  union socket_address lsa;   // Listening socket address
  struct linked_list_link active_connections;
  struct linked_list_link uri_handlers;
  char *config_options[NUM_OPTIONS];
  void *server_data;
  sock_t ctl[2];  // Control socketpair. Used to wake up from select() call
};

struct iobuf {
  char *buf;    // Buffer that holds the data
  int size;     // Buffer size
  int len;      // Number of bytes currently in a buffer
};

union endpoint {
  int fd;                   // Opened regular local file
  void *ssl;                // SSL descriptor
  struct uri_handler *uh;   // URI handler user function
};

enum endpoint_type { EP_NONE, EP_FILE, EP_USER };
enum connection_flags { CONN_CLOSE = 1, CONN_SPOOL_DONE = 2 };

struct connection {
  struct mg_connection mg_conn;   // XXX: Must be first
  struct linked_list_link link;   // Linkage to server->active_connections
  struct mg_server *server;
  sock_t client_sock;             // Connected client
  struct iobuf local_iobuf;
  struct iobuf remote_iobuf;
  union endpoint endpoint;
  enum endpoint_type endpoint_type;
  time_t expire_time;
  char *path_info;
  char *request;
  int request_len;  // Request length, including last \r\n after last header
  int flags;        // CONN_* flags: CONN_CLOSE, CONN_SPOOL_DONE, etc
  mutex_t mutex;    // Guards concurrent mg_write() calls
};

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

static const char *static_month_names[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

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

const char *mg_get_option(const struct mg_server *srv, const char *name) {
  int i = get_option_index(name);
  return i == -1 ? NULL : srv->config_options[i] == NULL ? "" :
    srv->config_options[i];
}

int mg_start_thread(void *(*f)(void *), void *p) {
#ifdef _WIN32
  return (long)_beginthread((void (__cdecl *)(void *)) f, 0, p) == -1L ? -1 : 0;
#else
  pthread_t thread_id;
  pthread_attr_t attr;
  int result;

  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if USE_STACK_SIZE > 1
  // Compile-time option to control stack size, e.g. -DUSE_STACK_SIZE=16384
  (void) pthread_attr_setstacksize(&attr, USE_STACK_SIZE);
#endif

  result = pthread_create(&thread_id, &attr, f, p);
  pthread_attr_destroy(&attr);

  return result;
#endif
}

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

#ifdef _WIN32
static pid_t start_process(char *cmd, char *env[], char *dir, sock_t sock) {
  STARTUPINFOA si = {0};
  PROCESS_INFORMATION pi = {0};
  HANDLE hs = (HANDLE) sock, me = GetCurrentProcess();

  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;

  DuplicateHandle(me, hs, me, &si.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
  DuplicateHandle(me, hs, me, &si.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
  si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
  closesocket(sock);

  DBG(("Starting commad: [%s]", cmd));
  CreateProcess(NULL, cmd, NULL, NULL, TRUE,
                CREATE_NEW_PROCESS_GROUP, (void *) env, dir, &si, &pi);

  CloseHandle(si.hStdOutput);
  CloseHandle(si.hStdInput);
  CloseHandle(pi.hThread);

  return (pid_t) pi.hProcess;
}
#else
#endif

// 'sa' must be an initialized address to bind to
static sock_t open_listening_socket(union socket_address *sa) {
  sock_t on = 1, sock = INVALID_SOCKET;

  if ((sock = socket(sa->sa.sa_family, SOCK_STREAM, 6)) == INVALID_SOCKET ||
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *) &on, sizeof(on)) ||
      bind(sock, &sa->sa, sa->sa.sa_family == AF_INET ?
           sizeof(sa->sin) : sizeof(sa->sa)) != 0 ||
      listen(sock, SOMAXCONN) != 0) {
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

// Valid listening port spec is: [ip_address:]port, e.g. "80", "127.0.0.1:3128"
static int parse_port_string(const char *str, union socket_address *sa) {
  unsigned int a, b, c, d, port;
#if defined(USE_IPV6)
  char buf[100];
#endif

  // MacOS needs that. If we do not zero it, subsequent bind() will fail.
  // Also, all-zeroes in the socket address means binding to all addresses
  // for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT).
  memset(sa, 0, sizeof(*sa));
  sa->sin.sin_family = AF_INET;

  if (sscanf(str, "%u.%u.%u.%u:%u", &a, &b, &c, &d, &port) == 5) {
    // Bind to a specific IPv4 address, e.g. 192.168.1.5:8080
    sa->sin.sin_addr.s_addr = htonl((a << 24) | (b << 16) | (c << 8) | d);
    sa->sin.sin_port = htons((uint16_t) port);
#if defined(USE_IPV6)
  } else if (sscanf(str, "[%49[^]]]:%d%n", buf, &port, &len) == 2 &&
             inet_pton(AF_INET6, buf, &so->lsa.sin6.sin6_addr)) {
    // IPv6 address, e.g. [3ffe:2a00:100:7031::1]:8080
    so->lsa.sin6.sin6_family = AF_INET6;
    so->lsa.sin6.sin6_port = htons((uint16_t) port);
#endif
  } else if (sscanf(str, "%u", &port) == 1) {
    // If only port is specified, bind to IPv4, INADDR_ANY
    sa->sin.sin_port = htons((uint16_t) port);
  } else {
    port = 0;   // Parsing failure. Make port invalid.
  }

  return port > 0 && port < 0xffff;
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
  } else {
    set_close_on_exec(sock);
    set_non_blocking_mode(sock);
    conn->server = server;
    conn->client_sock = sock;
    mutex_init(&conn->mutex);
    LINKED_LIST_ADD_TO_FRONT(&server->active_connections, &conn->link);
    DBG(("added conn %p", conn));
  }

  return conn;
}

static void close_conn(struct connection *conn) {
  DBG(("closing %p", conn));
  LINKED_LIST_REMOVE(&conn->link);
  closesocket(conn->client_sock);
  free(conn->request);            // It's OK to free(NULL)
  free(conn->remote_iobuf.buf);   // It's OK to free(NULL)
  free(conn->local_iobuf.buf);    // It's OK to free(NULL)
  mutex_destroy(&conn->mutex);
  free(conn);
}

// Check whether full request is buffered. Return:
//   -1  if request is malformed
//    0  if request is not yet fully buffered
//   >0  actual request length, including last \r\n\r\n
static int get_request_len(const unsigned char *buf, int buf_len) {
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
static int parse_http_message(char *buf, int len, struct mg_connection *ri) {
  int is_request, n;

  // Reset attributes. DO NOT TOUCH remote_ip, remote_port
  ri->request_method = ri->uri = ri->http_version = NULL;
  ri->num_headers = 0;
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

// Perform case-insensitive match of string against pattern
static int match_prefix(const char *pattern, int pattern_len, const char *str) {
  const char *or_str;
  int i, j, len, res;

  if ((or_str = (const char *) memchr(pattern, '|', pattern_len)) != NULL) {
    res = match_prefix(pattern, or_str - pattern, str);
    return res > 0 ? res :
        match_prefix(or_str + 1, (pattern + pattern_len) - (or_str + 1), str);
  }

  i = j = 0;
  res = -1;
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

// Return HTTP header value, or NULL if not found.
const char *mg_get_header(const struct mg_connection *ri, const char *s) {
  int i;

  for (i = 0; i < ri->num_headers; i++)
    if (!mg_strcasecmp(s, ri->http_headers[i].name))
      return ri->http_headers[i].value;

  return NULL;
}

// Return 1 if real file has been found, 0 otherwise
static int convert_uri_to_file_name(struct connection *conn, char *buf,
                                    size_t buf_len, file_stat_t *st) {
  struct vec a, b;
  const char *rewrites = conn->server->config_options[URL_REWRITES],
        *root = conn->server->config_options[DOCUMENT_ROOT],
        *cgi_pat = conn->server->config_options[CGI_PATTERN],
        *uri = conn->mg_conn.uri;
  char *p;
  int match_len;

  // No filesystem access
  if (root == NULL) return 0;

  // Handle URL rewrites
  snprintf(buf, buf_len, "%s%s", root, uri);
  while ((rewrites = next_option(rewrites, &a, &b)) != NULL) {
    if ((match_len = match_prefix(a.ptr, a.len, uri)) > 0) {
      snprintf(buf, buf_len, "%.*s%s", (int) b.len, b.ptr, uri + match_len);
      break;
    }
  }

  if (stat(buf, st) == 0) return 1;

  // Support PATH_INFO for CGI scripts.
  for (p = buf + strlen(root); *p != '\0'; p++) {
    if (*p == '/') {
      *p = '\0';
      if (match_prefix(cgi_pat, strlen(cgi_pat), buf) > 0 && !stat(buf, st)) {
        *p = '/';
        conn->path_info = mg_strdup(p);
        break;
      }
      *p = '/';
    }
  }

  return 0;
}

static int spool(struct iobuf *io, const void *buf, int len) {
  static const float mult = 1.2;
  char *p = NULL;
  int new_len = 0;

  assert(io->len >= 0);
  assert(io->len <= io->size);

  //DBG(("1. %d %d %d", len, io->len, io->size));
  if (len <= 0) {
  } else if ((new_len = io->len + len) < io->size) {
    memcpy(io->buf + io->len, buf, len);
    io->len = new_len;
  } else if ((p = (char *) realloc(io->buf, new_len * mult)) != NULL) {
    io->buf = p;
    memcpy(io->buf + io->len, buf, len);
    io->len = new_len;
    io->size = new_len * mult;
  } else {
    len = 0;
  }
  DBG(("%d %d %d", len, io->len, io->size));

  return len;
}

int mg_write(struct mg_connection *c, const void *buf, int len) {
  struct connection *conn = (struct connection *) c;
  int ret;

  mutex_lock(&conn->mutex);
  ret = spool(&conn->remote_iobuf, buf, len);
  mutex_unlock(&conn->mutex);
  send(conn->server->ctl[1], ".", 1, 0);  // Wake up select call

  return ret;
}

static int is_error(int n) {
  return n == 0 || (n < 0 && errno != EINTR && errno != EAGAIN);
}

static void write_to_client(struct connection *conn) {
  struct iobuf *io = &conn->remote_iobuf;
  int n = send(conn->client_sock, io->buf, io->len, 0);

  //DBG(("Written %d of %d(%d): [%.*s]", n, io->len, io->size, 0, io->buf));

  if (is_error(n)) {
    conn->flags |= CONN_CLOSE;
  } else if (n > 0) {
    memmove(io->buf, io->buf + n, io->len - n);
    io->len -= n;
  }
  if (io->len == 0 && conn->flags & CONN_SPOOL_DONE) {
    conn->flags |= CONN_CLOSE;
  }
}

const char *mg_get_mime_type(const char *path) {
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

  return "text/plain";
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

  vec->ptr = mg_get_mime_type(path);
  vec->len = strlen(vec->ptr);
}

static int parse_range_header(const char *header, int64_t *a, int64_t *b) {
  return sscanf(header, "bytes=%" INT64_FMT "-%" INT64_FMT, a, b);
}

static void gmt_time_string(char *buf, size_t buf_len, time_t *t) {
  strftime(buf, buf_len, "%a, %d %b %Y %H:%M:%S GMT", gmtime(t));
}

static void construct_etag(char *buf, size_t buf_len, const file_stat_t *st) {
  snprintf(buf, buf_len, "\"%lx.%" INT64_FMT "\"",
           (unsigned long) st->st_mtime, (int64_t) st->st_size);
}

static void open_file_endpoint(struct connection *conn, const char *path,
                               file_stat_t *st) {
  char date[64], lm[64], etag[64], range[64], headers[500];
  const char *msg = "OK", *hdr;
  time_t curtime = time(NULL);
  int64_t cl, r1, r2;
  struct vec mime_vec;
  int n, status_code = 200;

  conn->endpoint_type = EP_FILE;
  set_close_on_exec(conn->endpoint.fd);

  get_mime_type(conn->server, path, &mime_vec);
  cl = st->st_size;
  range[0] = '\0';

  // If Range: header specified, act accordingly
  r1 = r2 = 0;
  hdr = mg_get_header(&conn->mg_conn, "Range");
  if (hdr != NULL && (n = parse_range_header(hdr, &r1, &r2)) > 0 &&
      r1 >= 0 && r2 >= 0) {
    status_code = 206;
    cl = n == 2 ? (r2 > cl ? cl : r2) - r1 + 1: cl - r1;
    snprintf(range, sizeof(range), "Content-Range: bytes "
             "%" INT64_FMT "-%" INT64_FMT "/%" INT64_FMT "\r\n",
             r1, r1 + cl - 1, (int64_t) st->st_size);
    msg = "Partial Content";
    lseek(conn->endpoint.fd, r1, SEEK_SET);
  }

  // Prepare Etag, Date, Last-Modified headers. Must be in UTC, according to
  // http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3
  gmt_time_string(date, sizeof(date), &curtime);
  gmt_time_string(lm, sizeof(lm), &st->st_mtime);
  construct_etag(etag, sizeof(etag), st);

  n = snprintf(headers, sizeof(headers),
               "HTTP/1.1 %d %s\r\n"
               "Date: %s\r\n"
               "Last-Modified: %s\r\n"
               "Etag: %s\r\n"
               "Content-Type: %.*s\r\n"
               "Content-Length: %" INT64_FMT "\r\n"
               "Connection: %s\r\n"
               "Accept-Ranges: bytes\r\n"
               "%s%s\r\n",
               status_code, msg, date, lm, etag, (int) mime_vec.len,
               mime_vec.ptr, cl, "keep-alive", range, EXTRA_HTTP_HEADERS);
  spool(&conn->remote_iobuf, headers, n);

  if (!strcmp(conn->mg_conn.request_method, "HEAD")) {
    conn->flags |= CONN_SPOOL_DONE;
    close(conn->endpoint.fd);
    conn->endpoint_type = EP_NONE;
  }
}

// Convert month to the month number. Return -1 on error, or month number
static int get_month_index(const char *s) {
  int i;

  for (i = 0; i < (int) ARRAY_SIZE(static_month_names); i++)
    if (!strcmp(s, static_month_names[i]))
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

static struct uri_handler *find_uri_handler(struct mg_server *server,
                                            const char *uri) {
  struct linked_list_link *lp, *tmp;
  struct uri_handler *uh;

  LINKED_LIST_FOREACH(&server->uri_handlers, lp, tmp) {
    uh = LINKED_LIST_ENTRY(lp, struct uri_handler, link);
    if (!strcmp(uh->uri, uri)) return uh;
  }

  return NULL;
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
    strncpy(path + n + 1, filename_vec.ptr, filename_vec.len + 1);

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

static void send_http_error(struct connection *conn, const char *fmt, ...) {
  char buf[500];
  int len;
  va_list ap;

  va_start(ap, fmt);
  len = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  spool(&conn->remote_iobuf, buf, len);
  conn->flags |= CONN_SPOOL_DONE;
}

static void call_uri_handler_if_data_is_buffered(struct connection *conn) {
  if (conn->local_iobuf.len >= conn->mg_conn.content_len) {
    conn->endpoint.uh->handler(&conn->mg_conn);
    close_local_endpoint(conn);
  }
}

static void open_local_endpoint(struct connection *conn) {
  char path[MAX_PATH_SIZE] = {'\0'};
  file_stat_t st;
  int exists = 0, is_directory = 0;
  const char *cl_hdr = mg_get_header(&conn->mg_conn, "Content-Length");

  conn->mg_conn.content_len = cl_hdr == NULL ? 0 : to64(cl_hdr);

  // Call URI handler if one is registered for this URI
  conn->endpoint.uh = find_uri_handler(conn->server, conn->mg_conn.uri);
  if (conn->endpoint.uh != NULL) {
    conn->endpoint_type = EP_USER;
    conn->mg_conn.content = conn->local_iobuf.buf;
    call_uri_handler_if_data_is_buffered(conn);
    return;
  }

  exists = convert_uri_to_file_name(conn, path, sizeof(path), &st);
  is_directory = S_ISDIR(st.st_mode);

  if (!exists) {
    send_http_error(conn, "%s", "HTTP/1.1 404 Not Found\r\n\r\n");
  } else if (is_directory && !find_index_file(conn, path, sizeof(path), &st)) {
    send_http_error(conn, "%s", "HTTP/1.1 403 Listing Denied\r\n\r\n");
  } else if (match_prefix(LUA_SCRIPT_PATTERN, 6, path) > 0) {
    send_http_error(conn, "%s", "HTTP/1.1 501 Not Implemented\r\n\r\n");
    conn->flags |= CONN_SPOOL_DONE;
  } else if (is_not_modified(conn, &st)) {
    send_http_error(conn, "%s", "HTTP/1.1 304 Not Modified\r\n\r\n");
  } else if ((conn->endpoint.fd = open(path, O_RDONLY)) != -1) {
    open_file_endpoint(conn, path, &st);
  } else {
    send_http_error(conn, "%s", "HTTP/1.1 404 Not Found\r\n\r\n");
  }
}

static void send_continue_if_expected(struct connection *conn) {
  static const char expect_response[] = "HTTP/1.1 100 Continue\r\n\r\n";
  const char *expect_hdr = mg_get_header(&conn->mg_conn, "Expect");

  if (expect_hdr != NULL) {
    spool(&conn->remote_iobuf, expect_response, sizeof(expect_response) - 1);
  }
}

static void process_request(struct connection *conn) {
  struct iobuf *io = &conn->local_iobuf;

  if (conn->request_len == 0 &&
      (conn->request_len = get_request_len((unsigned char  *) io->buf,
                                           io->len)) > 0) {
    // If request is buffered in, remove it from the iobuf. This is because
    // iobuf could be reallocated, and pointers in parsed request could
    // become invalid.
    conn->request = (char *) malloc(conn->request_len);
    memcpy(conn->request, io->buf, conn->request_len);
    memmove(io->buf, io->buf + conn->request_len, io->len - conn->request_len);
    io->len -= conn->request_len;
    conn->request_len = parse_http_message(conn->request, conn->request_len,
                                           &conn->mg_conn);
    DBG(("request_len = %d", conn->request_len));
  }

  if (conn->request_len < 0 ||
      (conn->request_len == 0 && io->len > MAX_REQUEST_SIZE)) {
    // Invalid request, or request is too big: close the connection
    conn->flags |= CONN_CLOSE;
  } else if (conn->request_len > 0 && conn->endpoint_type == EP_NONE) {
    send_continue_if_expected(conn);
    open_local_endpoint(conn);
  } else if (conn->endpoint_type == EP_USER) {
    call_uri_handler_if_data_is_buffered(conn);
  }
}

static void read_from_client(struct connection *conn) {
  char buf[IOBUF_SIZE];
  int n = recv(conn->client_sock, buf, sizeof(buf), 0);

  if (is_error(n)) {
    conn->flags |= CONN_CLOSE;
  } else if (n > 0) {
    spool(&conn->local_iobuf, buf, n);
    process_request(conn);
  }
}

static int should_keep_alive(const struct connection *conn) {
  const char *method = conn->mg_conn.request_method;
  const char *http_version = conn->mg_conn.http_version;
  const char *header = mg_get_header(&conn->mg_conn, "Connection");
  return method != NULL && !strcmp(method, "GET") &&
    ((header != NULL && !strcmp(header, "keep-alive")) ||
     (header == NULL && http_version && !strcmp(http_version, "1.1")));
}

static void close_local_endpoint(struct connection *conn) {
  int keep_alive = should_keep_alive(conn);  // Must be done before free()

  //DBG(("Closing for conn %p", conn));

  // Close file descriptor
  switch (conn->endpoint_type) {
    case EP_FILE: close(conn->endpoint.fd); break;
    case EP_USER: break;
    case EP_NONE: break;
    default: assert(0); break;
  }

  conn->endpoint_type = EP_NONE;
  conn->request_len = 0;
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
  int n = read(conn->endpoint.fd, buf, sizeof(buf));

  if (is_error(n)) {
    close_local_endpoint(conn);
  } else if (n > 0) {
    spool(&conn->remote_iobuf, buf, n);
  }
}

void add_to_set(sock_t sock, fd_set *set, sock_t *max_fd) {
  FD_SET(sock, set);
  if (sock > *max_fd) {
    *max_fd = sock;
  }
}

void mg_poll_server(struct mg_server *server, int milliseconds) {
  struct linked_list_link *lp, *tmp;
  struct connection *conn;
  struct timeval tv;
  fd_set read_set, write_set;
  sock_t max_fd = -1;
  time_t current_time = time(NULL), expire_time = current_time +
    atoi(server->config_options[IDLE_TIMEOUT_MS]) / 1000;

  if (server->listening_sock == INVALID_SOCKET) return;

  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  add_to_set(server->listening_sock, &read_set, &max_fd);
  add_to_set(server->ctl[0], &read_set, &max_fd);

  LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
    conn = LINKED_LIST_ENTRY(lp, struct connection, link);
    add_to_set(conn->client_sock, &read_set, &max_fd);
    if (conn->endpoint_type == EP_FILE) {
      transfer_file_data(conn);
    }
    if (conn->remote_iobuf.len > 0) {
      add_to_set(conn->client_sock, &write_set, &max_fd);
    } else if (conn->flags & CONN_CLOSE) {
      close_conn(conn);
    }
  }

  tv.tv_sec = milliseconds / 1000;
  tv.tv_usec = (milliseconds % 1000) * 1000;

  if (select(max_fd + 1, &read_set, &write_set, NULL, &tv) > 0) {
    // If control socket is set, just read from it. It meant to wake up
    // this select loop when another thread writes to any connection
    if (FD_ISSET(server->ctl[0], &read_set)) {
      char buf[500];
      recv(server->ctl[0], buf, sizeof(buf), 0);
    }

    // Accept new connections
    if (FD_ISSET(server->listening_sock, &read_set)) {
      while ((conn = accept_new_connection(server)) != NULL) {
        conn->expire_time = expire_time;
      }
    }

    // Read/write from clients
    LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
      conn = LINKED_LIST_ENTRY(lp, struct connection, link);
      if (FD_ISSET(conn->client_sock, &read_set)) {
        conn->expire_time = expire_time;
        read_from_client(conn);
      }
      if (FD_ISSET(conn->client_sock, &write_set)) {
        conn->expire_time = expire_time;
        write_to_client(conn);
      }
    }
  }

  // Close expired connections and those that need to be closed
  LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
    conn = LINKED_LIST_ENTRY(lp, struct connection, link);
    if (conn->flags & CONN_CLOSE || current_time > conn->expire_time) {
      close_conn(conn);
    }
  }
}

void mg_destroy_server(struct mg_server **server) {
  struct linked_list_link *lp, *tmp;

  if (server != NULL && *server != NULL) {
    closesocket((*server)->listening_sock);
    closesocket((*server)->ctl[0]);
    closesocket((*server)->ctl[1]);
    LINKED_LIST_FOREACH(&(*server)->active_connections, lp, tmp) {
      free(LINKED_LIST_ENTRY(lp, struct connection, link));
    }
    LINKED_LIST_FOREACH(&(*server)->uri_handlers, lp, tmp) {
      free(LINKED_LIST_ENTRY(lp, struct uri_handler, link));
    }
    free(*server);
    *server = NULL;
  }
}

// Apply function to all active connections. Return number of active
// connections. Function could be NULL.
static int mg_iterate_over_connections(struct mg_server *server,
                                void (*func)(struct mg_connection *, void *),
                                void *param) {
  struct linked_list_link *lp, *tmp;
  struct connection *conn;
  int num_connections = 0;

  LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
    num_connections++;
    conn = LINKED_LIST_ENTRY(lp, struct connection, link);
    if (conn->endpoint_type == EP_USER && func != NULL) {
      func((struct mg_connection *) conn, param);
    }
  }

  return num_connections;
}

void mg_add_uri_handler(struct mg_server *server, const char *uri,
                        mg_uri_handler_t handler) {
  struct uri_handler *p = (struct uri_handler *) malloc(sizeof(*p));
  if (p != NULL) {
    LINKED_LIST_ADD_TO_FRONT(&server->uri_handlers, &p->link);
    p->uri = mg_strdup(uri);
    p->handler = handler;
  }
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

    if (ind == LISTENING_PORT) {
      if (server->listening_sock != INVALID_SOCKET) {
        closesocket(server->listening_sock);
      }
      parse_port_string(server->config_options[LISTENING_PORT], &server->lsa);
      server->listening_sock = open_listening_socket(&server->lsa);
      if (server->listening_sock == INVALID_SOCKET) {
        error_msg = "Cannot bind to port";
      } else {
        set_non_blocking_mode(server->listening_sock);
      }
    }
  }

  return error_msg;
}

struct mg_server *mg_create_server(void *server_data) {
  struct mg_server *server = (struct mg_server *) calloc(1, sizeof(*server));
  const char *value;
  int i;

#ifdef _WIN32
  WSADATA data;
  WSAStartup(MAKEWORD(2, 2), &data);
#endif

  LINKED_LIST_INIT(&server->active_connections);
  LINKED_LIST_INIT(&server->uri_handlers);
  mg_socketpair(server->ctl);
  server->server_data = server_data;
  server->listening_sock = INVALID_SOCKET;

  // Set default options values
  for (i = 0; static_config_options[i * 2] != NULL; i++) {
    value = static_config_options[i * 2 + 1];
    if (server->config_options[i] == NULL && value != NULL) {
      server->config_options[i] = mg_strdup(value);
    }
  }

  return server;
}

// End of library, start of the application code

static void iterate_callback(struct mg_connection *c, void *param) {
  if (c->connection_param != NULL) {
    char buf[20];
    int len = snprintf(buf, sizeof(buf), "%d", * (int *) param);
    mg_write(c, buf, len);
  }
}

static void *timer_thread(void *param) {
  struct mg_server *server = (struct mg_server *) param;
  int i;

  for (i = 0; i < 1000; i++) {
    sleep(1);
    mg_iterate_over_connections(server, iterate_callback, &i);
  }

  return NULL;
}

static int websocket_handler(struct mg_connection *conn) {
  char headers[500], content[500];
  int headers_len, content_len;

  content_len = snprintf(content, sizeof(content), "%s %s, POST len %d\n",
                         conn->request_method, conn->uri, conn->content_len);
  headers_len = snprintf(headers, sizeof(headers), "HTTP/1.0 200 OK\r\n"
                         "Content-Length: %d\r\n\r\n", content_len);

  mg_write(conn, headers, headers_len);
  mg_write(conn, content, content_len);

  return 1;
}


int main(void) {
  struct mg_server *server = mg_create_server(NULL);

  mg_set_option(server, "listening_port", "8080");
  mg_set_option(server, "document_root", ".");
  mg_add_uri_handler(server, "/ws", websocket_handler);
  mg_start_thread(timer_thread, server);

  printf("Started on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 3000);
  }

  mg_destroy_server(&server);
  return 0;
}
