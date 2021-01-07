// Copyright (c) 2004-2013 Sergey Lyubka
// Copyright (c) 2013-2020 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in <https://www.cesanta.com/license>.

#pragma once
#define MG_VERSION "7.0"


#define MG_ARCH_CUSTOM 0
#define MG_ARCH_UNIX 1
#define MG_ARCH_WIN32 2
#define MG_ARCH_ESP32 3
#define MG_ARCH_ESP8266 4
#define MG_ARCH_CC3100 5
#define MG_ARCH_CC3200 6
#define MG_ARCH_CC3220 7
#define MG_ARCH_MSP432 8
#define MG_ARCH_TM4C129 9
#define MG_ARCH_MBED 10
#define MG_ARCH_WINCE 11
#define MG_ARCH_NXP_LPC 12
#define MG_ARCH_NXP_KINETIS 13
#define MG_ARCH_NRF51 14
#define MG_ARCH_NRF52 15
#define MG_ARCH_PIC32 16
#define MG_ARCH_RS14100 17
#define MG_ARCH_STM32 18
#define MG_ARCH_FREERTOS 19

#if !defined(MG_ARCH)
#if defined(TARGET_IS_MSP432P4XX) || defined(__MSP432P401R__)
#define MG_ARCH MG_ARCH_MSP432
#elif defined(cc3200) || defined(TARGET_IS_CC3200)
#define MG_ARCH MG_ARCH_CC3200
#elif defined(cc3220) || defined(TARGET_IS_CC3220)
#define MG_ARCH MG_ARCH_CC3220
#elif defined(__unix__) || defined(__APPLE__)
#define MG_ARCH MG_ARCH_UNIX
#elif defined(WINCE)
#define MG_ARCH MG_ARCH_WINCE
#elif defined(_WIN32)
#define MG_ARCH MG_ARCH_WIN32
#elif defined(__MBED__)
#define MG_ARCH MG_ARCH_MBED
#elif defined(__USE_LPCOPEN)
#define MG_ARCH MG_ARCH_NXP_LPC
#elif defined(FRDM_K64F) || defined(FREEDOM)
#define MG_ARCH MG_ARCH_NXP_KINETIS
#elif defined(PIC32)
#define MG_ARCH MG_ARCH_PIC32
#elif defined(ICACHE_FLASH) || defined(ICACHE_RAM_ATTR)
#define MG_ARCH MG_ARCH_ESP8266
#elif defined(ESP_PLATFORM)
#define MG_ARCH MG_ARCH_ESP32
#elif defined(TARGET_IS_TM4C129_RA0) || defined(TARGET_IS_TM4C129_RA1) || \
    defined(TARGET_IS_TM4C129_RA2)
#define MG_ARCH MG_ARCH_TM4C129
#elif defined(RS14100)
#define MG_ARCH MG_ARCH_RS14100
#elif defined(STM32)
#define MG_ARCH MG_ARCH_STM32
#endif

#if !defined(MG_ARCH)
#error "MG_ARCH is not specified and we couldn't guess it."
#endif
#endif  // !defined(MG_ARCH)

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Standard C headers
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#if MG_ARCH == MG_ARCH_CUSTOM
#include <mongoose_custom.h>
#endif







#if !defined(PRINTF_LIKE)
#if defined(__GNUC__) || defined(__clang__) || defined(__TI_COMPILER_VERSION__)
#define PRINTF_LIKE(f, a) __attribute__((format(printf, f, a)))
#else
#define PRINTF_LIKE(f, a)
#endif
#endif


#if MG_ARCH == MG_ARCH_ESP32

#include <dirent.h>
#include <netdb.h>
#define MG_DIRSEP '/'

#endif


#if MG_ARCH == MG_ARCH_ESP8266

#include <dirent.h>
#include <esp_system.h>
#include <netdb.h>
#include <stdbool.h>
#include <sys/time.h>
#define MG_DIRSEP '/'

#endif


#if MG_ARCH == MG_ARCH_FREERTOS
#include <FreeRTOS.h>

#include <task.h>

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <stdbool.h>

#define MG_DIRSEP '/'
#define IPPROTO_TCP FREERTOS_IPPROTO_TCP
#define IPPROTO_UDP FREERTOS_IPPROTO_UDP
#define AF_INET FREERTOS_AF_INET
#define SOCK_STREAM FREERTOS_SOCK_STREAM
#define SOCK_DGRAM FREERTOS_SOCK_DGRAM
#define SO_BROADCAST 0
#define SO_ERROR 0
#define SOL_SOCKET 0
#define SO_REUSEADDR 0
#define sockaddr_in freertos_sockaddr
struct sockaddr {
  uint8_t sa_len, sa_family;
};
#define accept(a, b, c) FreeRTOS_accept((a), (b), (c))
#define connect(a, b, c) FreeRTOS_connect((a), (b), (c))
#define bind(a, b, c) FreeRTOS_bind((a), (b), (c))
#define listen(a, b) FreeRTOS_listen((a), (b))
#define socket(a, b, c) FreeRTOS_socket((a), (b), (c))
#define send(a, b, c, d) FreeRTOS_send((a), (b), (c), (d))
#define recv(a, b, c, d) FreeRTOS_recv((a), (b), (c), (d))
#define setsockopt(a, b, c, d, e) FreeRTOS_setsockopt((a), (b), (c), (d), (e))
#define sendto(a, b, c, d, e, f) FreeRTOS_sendto((a), (b), (c), (d), (e), (f))
#define recvfrom(a, b, c, d, e, f) \
  FreeRTOS_recvfrom((a), (b), (c), (d), (e), (f))
#define closesocket(x) FreeRTOS_closesocket(x)
#define gethostbyname(x) FreeRTOS_gethostbyname(x)

#include <ff_stdio.h>

#undef FILE
#define FILE FF_FILE
//#define SEEK_SET FF_SEEK_SET
//#define SEEK_END FF_SEEK_END
#define stat(a, b) ff_stat((a), (b))
#define fopen(a, b) ff_fopen((a), (b))
#define fclose(a) ff_fclose(a)
#define fread(a, b, c, d) ff_fread((a), (b), (c), (d))
#define fwrite(a, b, c, d) ff_fwrite((a), (b), (c), (d))
#define vfprintf ff_vfprintf
#define fprintf ff_fprintf
#define remove(a) ff_remove(a)
#define rename(a, b) ff_rename((a), (b), 1)

static inline int ff_vfprintf(FF_FILE *fp, const char *fmt, va_list ap) {
  char *buf = NULL;
  int n = mg_vasprintf(&buf, 0, fmt, ap);
  if (buf != NULL) ff_fwrite(buf, 1, n, fp), free(buf);
  return n;
}

#endif


#if MG_ARCH == MG_ARCH_UNIX

#define _DARWIN_UNLIMITED_SELECT 1

#include <arpa/inet.h>
#include <dirent.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#define MG_DIRSEP '/'
#define MG_ENABLE_POSIX 1

#endif


#if MG_ARCH == MG_ARCH_WIN32

#if defined(_MSC_VER) && _MSC_VER < 1700
#define __func__ ""
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int bool;
enum { false = 0, true = 1 };
#else
#include <stdbool.h>
#include <stdint.h>
#include <ws2tcpip.h>
#endif

#include <winsock2.h>

// Protect from calls like std::snprintf in app code
// See https://github.com/cesanta/mongoose/issues/1047
#ifndef __cplusplus
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strdup(x) _strdup(x)
#endif

typedef int socklen_t;
#define MG_DIRSEP '\\'
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#define realpath(a, b) _fullpath((b), (a), 512)
#ifndef va_copy
#ifdef __va_copy
#define va_copy __va_copy
#else
#define va_copy(x, y) (x) = (y)
#endif
#endif
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) &_S_IFMT) == _S_IFDIR)
#endif

#endif


#ifndef MG_ENABLE_LWIP
#define MG_ENABLE_LWIP 0
#endif

#if MG_ENABLE_LWIP
#define MG_ENABLE_SOCKET 0
#else
#define MG_ENABLE_SOCKET 1
#endif

#ifndef MG_ENABLE_MBEDTLS
#define MG_ENABLE_MBEDTLS 0
#endif

#ifndef MG_ENABLE_OPENSSL
#define MG_ENABLE_OPENSSL 0
#endif

#ifndef MG_ENABLE_FS
#define MG_ENABLE_FS 1
#endif

#ifndef MG_ENABLE_SSI
#define MG_ENABLE_SSI 0
#endif

#ifndef MG_ENABLE_IPV6
#define MG_ENABLE_IPV6 0
#endif

#ifndef MG_ENABLE_LOG
#define MG_ENABLE_LOG 1
#endif

#ifndef MG_ENABLE_MGOS
#define MG_ENABLE_MGOS 0
#endif

#ifndef MG_ENABLE_MD5
#define MG_ENABLE_MD5 0
#endif

#ifndef MG_ENABLE_DIRECTORY_LISTING
#define MG_ENABLE_DIRECTORY_LISTING 0
#endif

#ifndef MG_ENABLE_HTTP_DEBUG_ENDPOINT
#define MG_ENABLE_HTTP_DEBUG_ENDPOINT 0
#endif

#ifndef MG_ENABLE_SOCKETPAIR
#define MG_ENABLE_SOCKETPAIR 0
#endif

// Granularity of the send/recv IO buffer growth
#ifndef MG_IO_SIZE
#define MG_IO_SIZE 512
#endif

// Maximum size of the recv IO buffer
#ifndef MG_MAX_RECV_BUF_SIZE
#define MG_MAX_RECV_BUF_SIZE (3 * 1024 * 1024)
#endif

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 40
#endif


#include <ctype.h>
#include <string.h>

struct mg_str {
  const char *ptr;
  size_t len;
};

#define MG_NULL_STR \
  { NULL, 0 }

struct mg_str mg_str(const char *s);
struct mg_str mg_str_n(const char *s, size_t n);
int mg_lower(const char *s);
int mg_ncasecmp(const char *s1, const char *s2, size_t len);
int mg_casecmp(const char *s1, const char *s2);
int mg_vcmp(const struct mg_str *s1, const char *s2);
int mg_vcasecmp(const struct mg_str *str1, const char *str2);
int mg_strcmp(const struct mg_str str1, const struct mg_str str2);
struct mg_str mg_strstrip(struct mg_str s);
struct mg_str mg_strdup(const struct mg_str s);
const char *mg_strstr(const struct mg_str haystack, const struct mg_str needle);





#if MG_ENABLE_MGOS
#include <common/cs_dbg.h>
#else
#if MG_ENABLE_LOG
#define LOG(level, args)                                                   \
  do {                                                                     \
    if (mg_log_prefix((level), __FILE__, __LINE__, __func__)) mg_log args; \
  } while (0)
enum { LL_NONE, LL_ERROR, LL_INFO, LL_DEBUG, LL_VERBOSE_DEBUG };
bool mg_log_prefix(int ll, const char *file, int line, const char *fname);
void mg_log(const char *fmt, ...) PRINTF_LIKE(1, 2);
void mg_log_set(const char *spec);
void mg_log_set_callback(void (*fn)(const void *, int, void *), void *param);
#else
#define LOG(level, args)
#endif
#endif


struct mg_timer {
  int period_ms;            // Timer period in milliseconds
  int flags;                // Possible flags values below
#define MG_TIMER_REPEAT 1   // Call function periodically, otherwise run once
#define MG_TIMER_RUN_NOW 2  // Call immediately when timer is set
  void (*fn)(void *);       // Function to call
  void *arg;                // Function agrument
  unsigned long expire;     // Expiration timestamp in milliseconds
  struct mg_timer *next;    // Linkage in g_timers list
};

extern struct mg_timer *g_timers;  // Global list of timers

void mg_timer_init(struct mg_timer *, int ms, int, void (*fn)(void *), void *);
void mg_timer_free(struct mg_timer *);
void mg_timer_poll(unsigned long uptime_ms);





char *mg_file_read(const char *path);
size_t mg_file_size(const char *path);
int mg_file_write(const char *path, const char *fmt, ...);
void mg_random(void *buf, size_t len);
bool mg_globmatch(const char *pattern, int plen, const char *s, int n);
bool mg_next_comma_entry(struct mg_str *s, struct mg_str *k, struct mg_str *v);
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
char *mg_hexdump(const void *buf, size_t len);
char *mg_hex(const void *buf, int len, char *dst);
void mg_unhex(const char *buf, int len, unsigned char *to);
unsigned long mg_unhexn(const char *s, int len);
int mg_asprintf(char **buf, size_t size, const char *fmt, ...);
int mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap);
int64_t mg_to64(struct mg_str str);
double mg_time(void);
unsigned long mg_millis(void);
void mg_usleep(unsigned long usecs);

#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)

#define MG_SWAP16(x) ((((x) >> 8) & 255) | ((x & 255) << 8))
#define MG_SWAP32(x) \
  (((x) >> 24) | (((x) &0xff0000) >> 8) | (((x) &0xff00) << 8) | ((x) << 24))

#if !defined(WEAK)
#if (defined(__GNUC__) || defined(__clang__) || \
     defined(__TI_COMPILER_VERSION__)) &&       \
    !defined(_WIN32)
#define WEAK __attribute__((weak))
#else
#define WEAK
#endif
#endif

// Expands to a string representation of its argument: e.g.
// MG_STRINGIFY_LITERAL(5) expands to "5"
#if !defined(_MSC_VER) || _MSC_VER >= 1900
#define MG_STRINGIFY_LITERAL(...) #__VA_ARGS__
#else
#define MG_STRINGIFY_LITERAL(x) #x
#endif

#define LIST_ADD_HEAD(type_, head_, elem_) \
  do {                                     \
    (elem_)->next = (*head_);              \
    *(head_) = (elem_);                    \
  } while (0)

#define LIST_ADD_TAIL(type_, head_, elem_) \
  do {                                     \
    type_ **h = head_;                     \
    while (*h != NULL) h = &(*h)->next;    \
    *h = (elem_);                          \
  } while (0)

#define LIST_DELETE(type_, head_, elem_)   \
  do {                                     \
    type_ **h = head_;                     \
    while (*h != (elem_)) h = &(*h)->next; \
    *h = (elem_)->next;                    \
  } while (0)

// Expands to a string representation of its argument, which can be a macro:
// #define FOO 123
// MG_STRINGIFY_MACRO(FOO)  // Expands to 123
#define MG_STRINGIFY_MACRO(x) MG_STRINGIFY_LITERAL(x)



unsigned short mg_url_port(const char *url);
int mg_url_is_ssl(const char *url);
struct mg_str mg_url_host(const char *url);
struct mg_str mg_url_user(const char *url);
struct mg_str mg_url_pass(const char *url);
const char *mg_url_uri(const char *url);


#include <stddef.h>

struct mg_iobuf {
  unsigned char *buf;
  size_t size, len;
};

void mg_iobuf_init(struct mg_iobuf *, size_t);
void mg_iobuf_resize(struct mg_iobuf *, size_t);
void mg_iobuf_free(struct mg_iobuf *);
size_t mg_iobuf_append(struct mg_iobuf *, const void *, size_t, size_t);
size_t mg_iobuf_delete(struct mg_iobuf *, size_t);

int mg_base64_update(unsigned char p, char *to, int len);
int mg_base64_final(char *to, int len);
int mg_base64_encode(const unsigned char *p, int n, char *to);
int mg_base64_decode(const char *src, int n, char *dst);




typedef struct {
  uint32_t buf[4];
  uint32_t bits[2];
  unsigned char in[64];
} mg_md5_ctx;

void mg_md5_init(mg_md5_ctx *c);
void mg_md5_update(mg_md5_ctx *c, const unsigned char *data, size_t len);
void mg_md5_final(mg_md5_ctx *c, unsigned char[16]);




typedef struct {
  uint32_t state[5];
  uint32_t count[2];
  unsigned char buffer[64];
} mg_sha1_ctx;

void mg_sha1_init(mg_sha1_ctx *);
void mg_sha1_update(mg_sha1_ctx *, const unsigned char *data, size_t len);
void mg_sha1_final(unsigned char digest[20], mg_sha1_ctx *);
void mg_hmac_sha1(const unsigned char *key, size_t key_len,
                  const unsigned char *text, size_t text_len,
                  unsigned char out[20]);


struct mg_connection;
typedef void (*mg_event_handler_t)(struct mg_connection *, int ev,
                                   void *ev_data, void *fn_data);
#if 0
struct mg_fn {
  struct mg_fn *next;
  mg_event_handler_t fn;
  void *fn_data;
};

void mg_fn_add(struct mg_connection *c, mg_event_handler_t fn, void *fn_data);
void mg_fn_del(struct mg_connection *c, mg_event_handler_t fn);
#endif

void mg_call(struct mg_connection *c, int ev, void *ev_data);
void mg_error(struct mg_connection *c, const char *fmt, ...);

enum {
  MG_EV_ERROR,      // Error                        char *error_message
  MG_EV_POLL,       // mg_mgr_poll iteration        unsigned long *millis
  MG_EV_RESOLVE,    // Host name is resolved        NULL
  MG_EV_CONNECT,    // Connection established       NULL
  MG_EV_ACCEPT,     // Connection accepted          NULL
  MG_EV_READ,       // Data received from socket    struct mg_str *
  MG_EV_WRITE,      // Data written to socket       int *num_bytes_written
  MG_EV_CLOSE,      // Connection closed            NULL
  MG_EV_HTTP_MSG,   // HTTP request/response        struct mg_http_message *
  MG_EV_WS_OPEN,    // Websocket handshake done     NULL
  MG_EV_WS_MSG,     // Websocket message received   struct mg_ws_message *
  MG_EV_WS_CTL,     // Websocket control message    struct mg_ws_message *
  MG_EV_MQTT_CMD,   // MQTT low-level command       struct mg_mqtt_message *
  MG_EV_MQTT_MSG,   // MQTT PUBLISH received        struct mg_mqtt_message *
  MG_EV_MQTT_OPEN,  // MQTT CONNACK received        int *connack_status_code
  MG_EV_SNTP_TIME,  // SNTP time received           struct timeval *
  MG_EV_USER,       // Starting ID for user events
};







struct mg_dns {
  const char *url;          // DNS server URL
  struct mg_connection *c;  // DNS server connection
};

struct mg_addr {
  uint16_t port;    // TCP or UDP port in network byte order
  uint32_t ip;      // IP address in network byte order
  uint8_t ip6[16];  // IPv6 address
  bool is_ip6;      // True when address is IPv6 address
};

struct mg_mgr {
  struct mg_connection *conns;  // List of active connections
  struct mg_dns dns4;           // DNS for IPv4
  struct mg_dns dns6;           // DNS for IPv6
  int dnstimeout;               // DNS resolve timeout in milliseconds
  unsigned long nextid;         // Next connection ID
#if MG_ARCH == MG_ARCH_FREERTOS
  SocketSet_t ss;  // NOTE(lsm): referenced from socket struct
#endif
};

struct mg_connection {
  struct mg_connection *next;  // Linkage in struct mg_mgr :: connections
  struct mg_mgr *mgr;          // Our container
  struct mg_addr peer;         // Remote peer address
  void *fd;                    // Connected socket, or LWIP data
  unsigned long id;            // Auto-incrementing unique connection ID
  struct mg_iobuf recv;        // Incoming data
  struct mg_iobuf send;        // Outgoing data
  mg_event_handler_t fn;       // User-specified event handler function
  void *fn_data;               // User-speficied function parameter
  mg_event_handler_t pfn;      // Protocol-specific handler function
  void *pfn_data;              // Protocol-specific function parameter
  char label[32];              // Arbitrary label
  void *tls;                   // TLS specific data
  unsigned is_listening : 1;   // Listening connection
  unsigned is_client : 1;      // Outbound (client) connection
  unsigned is_accepted : 1;    // Accepted (server) connection
  unsigned is_resolving : 1;   // Non-blocking DNS resolv is in progress
  unsigned is_connecting : 1;  // Non-blocking connect is in progress
  unsigned is_tls : 1;         // TLS-enabled connection
  unsigned is_tls_hs : 1;      // TLS handshake is in progress
  unsigned is_udp : 1;         // UDP connection
  unsigned is_websocket : 1;   // WebSocket connection
  unsigned is_hexdumping : 1;  // Hexdump in/out traffic
  unsigned is_draining : 1;    // Send remaining data, then close and free
  unsigned is_closing : 1;     // Close and free the connection immediately
  unsigned is_readable : 1;    // Connection is ready to read
  unsigned is_writable : 1;    // Connection is ready to write
};

void mg_mgr_poll(struct mg_mgr *, int ms);
void mg_mgr_init(struct mg_mgr *);
void mg_mgr_free(struct mg_mgr *);

struct mg_connection *mg_listen(struct mg_mgr *, const char *url,
                                mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_connect(struct mg_mgr *, const char *url,
                                 mg_event_handler_t fn, void *fn_data);
int mg_send(struct mg_connection *, const void *, size_t);
int mg_printf(struct mg_connection *, const char *fmt, ...);
int mg_vprintf(struct mg_connection *, const char *fmt, va_list ap);
char *mg_straddr(struct mg_connection *, char *, size_t);
bool mg_socketpair(int *s1, int *s2);
bool mg_aton(struct mg_str str, struct mg_addr *addr);
char *mg_ntoa(const struct mg_addr *addr, char *buf, size_t len);






struct mg_http_header {
  struct mg_str name;
  struct mg_str value;
};

struct mg_http_message {
  //        GET /foo/bar/baz?aa=b&cc=ddd HTTP/1.1
  // method |-| |----uri---| |--query--| |proto-|

  struct mg_str method, uri, query, proto;             // Request/response line
  struct mg_http_header headers[MG_MAX_HTTP_HEADERS];  // Headers
  struct mg_str body;                                  // Body
  struct mg_str message;  // Request line + headers + body
};

// Parameter for mg_http_serve_dir()
struct mg_http_serve_opts {
  const char *root_dir;     // Web root directory, must be non-NULL
  const char *ssi_pattern;  // SSI filename pattern, e.g. #.shtml
};

int mg_http_parse(const char *s, size_t len, struct mg_http_message *);
int mg_http_get_request_len(const unsigned char *buf, size_t buf_len);
void mg_http_printf_chunk(struct mg_connection *cnn, const char *fmt, ...);
void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len);
struct mg_connection *mg_http_listen(struct mg_mgr *, const char *url,
                                     mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_http_connect(struct mg_mgr *, const char *url,
                                      mg_event_handler_t fn, void *fn_data);
void mg_http_serve_dir(struct mg_connection *, struct mg_http_message *hm,
                       struct mg_http_serve_opts *);
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *,
                        const char *, const char *mime, const char *headers);
void mg_http_reply(struct mg_connection *, int status_code, const char *headers,
                   const char *body_fmt, ...);
struct mg_str *mg_http_get_header(struct mg_http_message *, const char *name);
void mg_http_event_handler(struct mg_connection *c, int ev);
int mg_http_get_var(const struct mg_str *, const char *name, char *, int);
int mg_url_decode(const char *s, size_t n, char *to, size_t to_len, int form);
void mg_http_creds(struct mg_http_message *, char *user, int, char *pass, int);
bool mg_http_match_uri(const struct mg_http_message *, const char *glob);
int mg_http_upload(struct mg_connection *, struct mg_http_message *hm,
                   const char *dir);
void mg_http_bauth(struct mg_connection *, const char *user, const char *pass);


void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath);



struct mg_tls_opts {
  const char *ca;        // CA certificate file. For both listeners and clients
  const char *cert;      // Certificate
  const char *certkey;   // Certificate key
  const char *ciphers;   // Cipher list
  const char *srvname;   // If not NULL, enables server name verification
};

int mg_tls_init(struct mg_connection *, struct mg_tls_opts *);
int mg_tls_free(struct mg_connection *);
int mg_tls_send(struct mg_connection *, const void *buf, size_t len, int *fail);
int mg_tls_recv(struct mg_connection *, void *buf, size_t len, int *fail);
int mg_tls_handshake(struct mg_connection *);


#define WEBSOCKET_OP_CONTINUE 0
#define WEBSOCKET_OP_TEXT 1
#define WEBSOCKET_OP_BINARY 2
#define WEBSOCKET_OP_CLOSE 8
#define WEBSOCKET_OP_PING 9
#define WEBSOCKET_OP_PONG 10

#define WEBSOCKET_FLAGS_MASK_FIN 128
#define WEBSOCKET_FLAGS_MASK_OP 15



struct mg_ws_message {
  struct mg_str data;
  uint8_t flags;  // Websocket message flags
};

struct mg_connection *mg_ws_connect(struct mg_mgr *, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...);
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *);
size_t mg_ws_send(struct mg_connection *, const char *buf, size_t len, int op);




struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fn_data);
void mg_sntp_send(struct mg_connection *c, unsigned long utc);
int mg_sntp_parse(const unsigned char *buf, size_t len, struct timeval *tv);





#define MQTT_CMD_CONNECT 1
#define MQTT_CMD_CONNACK 2
#define MQTT_CMD_PUBLISH 3
#define MQTT_CMD_PUBACK 4
#define MQTT_CMD_PUBREC 5
#define MQTT_CMD_PUBREL 6
#define MQTT_CMD_PUBCOMP 7
#define MQTT_CMD_SUBSCRIBE 8
#define MQTT_CMD_SUBACK 9
#define MQTT_CMD_UNSUBSCRIBE 10
#define MQTT_CMD_UNSUBACK 11
#define MQTT_CMD_PINGREQ 12
#define MQTT_CMD_PINGRESP 13
#define MQTT_CMD_DISCONNECT 14

#define MQTT_QOS(qos) ((qos) << 1)
#define MQTT_GET_QOS(flags) (((flags) &0x6) >> 1)
#define MQTT_SET_QOS(flags, qos) (flags) = ((flags) & ~0x6) | ((qos) << 1)

struct mg_mqtt_opts {
  struct mg_str client_id;
  struct mg_str will_topic;
  struct mg_str will_message;
  uint8_t qos;         // Quality of service
  bool will_retain;    // Retain last will
  bool clean;          // Use clean session, 0 or 1
  uint16_t keepalive;  // Keep-alive timer in seconds
};

struct mg_mqtt_message {
  struct mg_str topic;    // Parsed topic
  struct mg_str data;     // Parsed message
  struct mg_str dgram;    // Whole MQTT datagram, including headers
  uint16_t id;  // Set for PUBACK, PUBREC, PUBREL, PUBCOMP, SUBACK, PUBLISH
  uint8_t cmd;  // MQTT command, one of MQTT_CMD_*
  uint8_t qos;  // Quality of service
  uint8_t ack;  // Connack return code. 0 - success
};

struct mg_connection *mg_mqtt_connect(struct mg_mgr *, const char *url,
                                      struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data);
void mg_mqtt_pub(struct mg_connection *, struct mg_str *topic,
                 struct mg_str *data);
void mg_mqtt_sub(struct mg_connection *, struct mg_str *topic);
int mg_mqtt_parse(const uint8_t *buf, size_t len, struct mg_mqtt_message *m);
void mg_mqtt_send_header(struct mg_connection *, uint8_t cmd, uint8_t flags,
                         uint32_t len);
int mg_mqtt_next_sub(struct mg_mqtt_message *msg, struct mg_str *topic,
                     uint8_t *qos, int pos);





// Mongoose sends DNS queries that contain only one question:
// either A (IPv4) or AAAA (IPv6) address lookup.
// Therefore, we expect zero or one answer.
// If `resolved` is true, then `addr` contains resolved IPv4 or IPV6 address.
struct mg_dns_message {
  uint16_t txnid;       // Transaction ID
  bool resolved;        // Resolve successful, addr is set
  struct mg_addr addr;  // Resolved address
  char name[256];       // Host name
};

struct mg_dns_header {
  uint16_t txnid;  // Transaction ID
  uint16_t flags;
  uint16_t num_questions;
  uint16_t num_answers;
  uint16_t num_authority_prs;
  uint16_t num_other_prs;
};

// DNS resource record
struct mg_dns_rr {
  uint16_t nlen;    // Name or pointer length
  uint16_t atype;   // Address type
  uint16_t aclass;  // Address class
  uint16_t alen;    // Address length
};

void mg_resolve(struct mg_connection *, struct mg_str *, int);
void mg_resolve_cancel(struct mg_connection *);
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *);
size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *);
size_t mg_dns_decode_name(const uint8_t *, size_t, size_t, char *, size_t);
