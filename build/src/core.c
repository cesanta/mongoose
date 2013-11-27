#ifdef _WIN32
#undef _UNICODE
#define _MBCS
#define _CRT_SECURE_NO_WARNINGS // Disable deprecation warning in VS2005+
#else
#define _XOPEN_SOURCE 600     // For flockfile() on Linux
#define __STDC_FORMAT_MACROS  // <inttypes.h> wants this for C++
#define __STDC_LIMIT_MACROS   // C++ wants that for INT64_MAX
#define _LARGEFILE_SOURCE     // Enable fseeko() and ftello() functions
#define _FILE_OFFSET_BITS 64  // Enable 64-bit file offsets
#endif

#ifdef _MSC_VER
#pragma warning (disable : 4127)  // FD_SET() emits warning, disable it
#pragma warning (disable : 4204)  // missing c99 support
#endif

// Disable WIN32_LEAN_AND_MEAN. This makes windows.h always include winsock2.h
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
typedef int socklen_t;
typedef int pid_t;
typedef SOCKET sock_t;
typedef unsigned int  uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned __int64 uint64_t;
typedef __int64   int64_t;
#pragma comment(lib, "ws2_32.lib")
#define snprintf _snprintf
#define INT64_FMT  "I64d"
#else
#include <inttypes.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#define closesocket(x) close(x)
typedef int sock_t;
#define INVALID_SOCKET ((sock_t) -1)
#define INT64_FMT PRId64
#endif

#include "mongoose.h"

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

union socket_address {
  struct sockaddr sa;
  struct sockaddr_in sin;
#ifdef USE_IPV6
  struct sockaddr_in6 sin6;
#endif
};

struct vec {
  const char *ptr;
  size_t len;
};

// NOTE(lsm): this enum shoulds be in sync with the config_options.
enum {
  ACCESS_CONTROL_LIST, ACCESS_LOG_FILE, AUTH_DOMAIN, CGI_INTERPRETER,
  CGI_PATTERN, DOCUMENT_ROOT, ENABLE_DIRECTORY_LISTING, ERROR_LOG_FILE,
  EXTRA_MIME_TYPES, GLOBAL_AUTH_FILE, HIDE_FILES_PATTERN, IDLE_TIMEOUT_MS,
  INDEX_FILES, LISTENING_PORT, NUM_THREADS, PUT_DELETE_AUTH_FILE, RUN_AS_USER,
  SSL_CERTIFICATE, SSI_PATTERN, URL_REWRITES, NUM_OPTIONS
};

struct mg_server {
  sock_t listening_sock;
  union socket_address lsa;   // Listening socket address
  struct linked_list_link active_connections;
  char *config_options[NUM_OPTIONS];
  mg_event_handler_t event_handler;
  void *user_data;
};

struct iobuf {
  char *buf;    // Buffer that holds the data
  int size;   // Buffer size
  int len;    // Number of bytes currently in a buffer
};

union endpoint {
  int fd;       // Opened regular local file
  sock_t sock;  // CGI socket
  void *ssl;    // SSL descriptor
};

struct mg_connection {
  struct linked_list_link link;   // Linkage to server->active_connections
  struct mg_server *server;
  sock_t client_sock;         // Connected client
  union socket_address csa;   // Client's socket address
  struct iobuf local_iobuf;
  struct iobuf remote_iobuf;
  union endpoint endpoint;
  enum { EP_NONE, EP_FILE, EP_SOCKET, EP_SSL } endpoint_type;
  enum { CONN_CLOSE = 1 } flags;
  time_t expire_time;
  struct mg_request_info request_info;
  int request_len;
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
  "listening_port", "8080",
  "num_threads", "50",
  "put_delete_auth_file", NULL,
  "run_as_user", NULL,
  "ssl_certificate", NULL,
  "ssi_pattern", "**.shtml$|**.shtm$",
  "url_rewrites", NULL,
  NULL
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

const char *mg_get_option2(const struct mg_server *srv, const char *name) {
  int i = get_option_index(name);
  return i == -1 ? NULL : srv->config_options[i] == NULL ? "" :
    srv->config_options[i];
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

  printf("Starting commad: [%s]\n", cmd);
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
  char *copy = malloc(strlen(str) + 1);
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

static struct mg_connection *accept_new_connection(struct mg_server *server) {
  union socket_address sa;
  socklen_t len = sizeof(sa);
  sock_t sock = INVALID_SOCKET;
  struct mg_connection *conn = NULL;

  if ((sock = accept(server->listening_sock, &sa.sa, &len)) == INVALID_SOCKET) {
  } else if (sock >= FD_SETSIZE) {
    closesocket(sock);
  } else if (!check_acl(server->config_options[ACCESS_CONTROL_LIST],
                        ntohl(* (uint32_t *) &sa.sin.sin_addr))) {
    closesocket(sock);
  } else if ((conn = calloc(1, sizeof(*conn) + MAX_REQUEST_SIZE)) == NULL) {
    closesocket(sock);
  } else {
    // Put so socket structure into the queue
    set_close_on_exec(sock);
    set_non_blocking_mode(sock);
    conn->client_sock = sock;
    conn->csa = sa;
    conn->local_iobuf.buf = (char *) conn + sizeof(*conn);
    conn->local_iobuf.size = MAX_REQUEST_SIZE;
    conn->remote_iobuf.buf = calloc(1, IOBUF_SIZE);
    conn->remote_iobuf.size = IOBUF_SIZE;
    LINKED_LIST_ADD_TO_FRONT(&server->active_connections, &conn->link);
    printf("added conn %p\n", conn);
  }

  return conn;
}

static void close_conn(struct mg_connection *conn) {
  printf("closing %p\n", conn);
  LINKED_LIST_REMOVE(&conn->link);
  closesocket(conn->client_sock);
  free(conn->remote_iobuf.buf);
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

// Parse HTTP headers from the given buffer, advance buffer to the point
// where parsing stopped.
static void parse_http_headers(char **buf, struct mg_request_info *ri) {
  int i;

  for (i = 0; i < (int) ARRAY_SIZE(ri->http_headers); i++) {
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

// Parse HTTP request, fill in mg_request_info structure.
// This function modifies the buffer by NUL-terminating
// HTTP request components, header names and header values.
static int parse_http_message(char *buf, int len, struct mg_request_info *ri) {
  int is_request, request_length = get_request_len((unsigned char *) buf, len);
  if (request_length > 0) {
    // Reset attributes. DO NOT TOUCH is_ssl, remote_ip, remote_port
    ri->remote_user = ri->request_method = ri->uri = ri->http_version = NULL;
    ri->num_headers = 0;

    buf[request_length - 1] = '\0';

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
      request_length = -1;
    } else {
      if (is_request) {
        ri->http_version += 5;
      }
      parse_http_headers(&buf, ri);
    }
  }
  return request_length;
}

static int parse_range_header(const char *header, int64_t *a, int64_t *b) {
  return sscanf(header, "bytes=%" INT64_FMT "-%" INT64_FMT, a, b);
}

static void read_from_client(struct mg_connection *conn) {
  struct iobuf *io = &conn->local_iobuf;
  int n = recv(conn->client_sock, io->buf + io->len, io->size - io->len, 0);

  printf("read %d bytes\n", n);
  assert(io->len >= 0);
  assert(io->len <= io->size);

  if (n == 0 || (n < 0 && (errno == EINTR || errno == EAGAIN))) {
    conn->flags |= CONN_CLOSE;
  } else if (n > 0) {
    io->len += n;
    if (conn->request_len == 0) {
      conn->request_len = parse_http_message(io->buf, io->len,
                                             &conn->request_info);
    }
    if (conn->request_len < 0 ||
        (conn->request_len == 0 || io->len >= io->size)) {
      conn->flags |= CONN_CLOSE;
    } else if (conn->request_len > 0) {
      printf("%.*s", (int) io->len, io->buf);
    }
  }
}

void add_to_set(sock_t sock, fd_set *set, sock_t *max_fd) {
  FD_SET(sock, set);
  if (sock > *max_fd) {
    *max_fd = sock;
  }
}

void mg_poll_server(struct mg_server *server, unsigned int milliseconds) {
  struct linked_list_link *lp, *tmp;
  struct mg_connection *conn;
  struct timeval tv;
  fd_set read_set, write_set;
  sock_t max_fd = -1;
  time_t current_time = time(NULL), expire_time = current_time + 3;

  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  add_to_set(server->listening_sock, &read_set, &max_fd);

  LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
    conn = LINKED_LIST_ENTRY(lp, struct mg_connection, link);
    add_to_set(conn->client_sock, &read_set, &max_fd);
  }

  tv.tv_sec = milliseconds / 1000;
  tv.tv_usec = (milliseconds % 1000) * 1000;

  if (select(max_fd + 1, &read_set, &write_set, NULL, &tv) > 0) {
    if (FD_ISSET(server->listening_sock, &read_set)) {
      while ((conn = accept_new_connection(server)) != NULL) {
        conn->expire_time = expire_time;
      }
    }

    LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
      conn = LINKED_LIST_ENTRY(lp, struct mg_connection, link);
      if (FD_ISSET(conn->client_sock, &read_set)) {
        conn->expire_time = expire_time;
        read_from_client(conn);
      }
    }
  }

  // Close expired connections and those that need to be closed
  LINKED_LIST_FOREACH(&server->active_connections, lp, tmp) {
    conn = LINKED_LIST_ENTRY(lp, struct mg_connection, link);
    if (conn->flags & CONN_CLOSE || current_time > conn->expire_time) {
      close_conn(conn);
    }
  }
}

void mg_destroy_server(struct mg_server **server) {
  struct linked_list_link *lp, *tmp;
  struct mg_connection *conn;

  if (server != NULL && *server != NULL) {
    closesocket((*server)->listening_sock);
    LINKED_LIST_FOREACH(&(*server)->active_connections, lp, tmp) {
      conn = LINKED_LIST_ENTRY(lp, struct mg_connection, link);
      LINKED_LIST_REMOVE(&conn->link);
      free(conn);
    }
    free(*server);
    *server = NULL;
  }
}

struct mg_server *mg_create_server(const char *opts[], mg_event_handler_t func,
                                   void *user_data) {
  struct mg_server *server = calloc(1, sizeof(*server));
  const char *name, *value;
  char error_msg[100] = {'\0'};
  int i;

#ifdef _WIN32
  WSADATA data;
  WSAStartup(MAKEWORD(2, 2), &data);
#endif

  server->event_handler = func;
  server->user_data = user_data;
  LINKED_LIST_INIT(&server->active_connections);

  while (opts != NULL && (name = *opts++) != NULL) {
    if ((i = get_option_index(name)) == -1) {
      snprintf(error_msg, sizeof(error_msg), "Invalid option: [%s]", name);
    } else if ((value = *opts++) == NULL) {
      snprintf(error_msg, sizeof(error_msg), "[%s] cannot be NULL", name);
    } else {
      if (server->config_options[i] != NULL) {
        free(server->config_options[i]);
      }
      server->config_options[i] = mg_strdup(value);
      printf("[%s] -> [%s]\n", name, value);
    }
  }

  // Set default values
  for (i = 0; static_config_options[i * 2] != NULL; i++) {
    value = static_config_options[i * 2 + 1];
    if (server->config_options[i] == NULL && value != NULL) {
      server->config_options[i] = mg_strdup(value);
    }
  }
  parse_port_string(server->config_options[LISTENING_PORT], &server->lsa);
  server->listening_sock = open_listening_socket(&server->lsa);
  set_non_blocking_mode(server->listening_sock);

  if (error_msg[0] != '\0') {
    mg_destroy_server(&server);
  }

  return server;
}

int main(int argc, char *argv[]) {
  const char *options[] = {"listening_port", "8080", NULL};
  struct mg_server *server = mg_create_server(options, NULL, NULL);
  for (;;) {
    mg_poll_server(server, 1000);
  }
  mg_destroy_server(&server);

#if 0
  char buf[1000];
  int fds[2];
  struct iobuf loc, rem;

  mg_socketpair(fds);
  if (!start_process(argv[1], NULL, NULL, fds[1])) {
    printf("start_process() failed\n");
  }

  init_iobuf(&rem, buf, sizeof(buf), IO_FILE, (void *) fileno(stdout));
  init_iobuf(&loc, NULL, 0, IO_SOCKET, (void *) fds[0]);

  while ((rem.len = read_from_iobuf(&loc, rem.buf, rem.size)) > 0) {
    write_to_iobuf(&rem);
  }

  close_iobuf(&rem);
  close_iobuf(&loc);
#endif

  return 0;
}
