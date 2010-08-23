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

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS // Disable deprecation warning in VS2005
#else
#define _XOPEN_SOURCE 600 // For flockfile() on Linux
#define _LARGEFILE_SOURCE // Enable 64-bit file offsets
#endif

#ifndef _WIN32_WCE // Some ANSI #includes are not available on Windows CE
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#endif // !_WIN32_WCE

#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

#if defined(_WIN32)  // Windows specific #includes and #defines
#define _WIN32_WINNT 0x0400 // To make it link in VS2005
#include <windows.h>

#ifndef _WIN32_WCE
#include <process.h>
#include <direct.h>
#include <io.h>
#else // _WIN32_WCE
#include <winsock2.h>
#define NO_CGI // WinCE has no pipes

#define FILENAME_MAX MAX_PATH
#define BUFSIZ  4096
typedef long off_t;

#define errno   GetLastError()
#define strerror(x)  _ultoa(x, (char *) _alloca(sizeof(x) *3 ), 10)
#endif // _WIN32_WCE

#define EPOCH_DIFF 0x019DB1DED53E8000 // 116444736000000000 nsecs
#define RATE_DIFF 10000000 // 100 nsecs
#define MAKEUQUAD(lo, hi) ((uint64_t)(((uint32_t)(lo)) | \
      ((uint64_t)((uint32_t)(hi))) << 32))
#define SYS2UNIX_TIME(lo, hi) \
  (time_t) ((MAKEUQUAD((lo), (hi)) - EPOCH_DIFF) / RATE_DIFF)

// Visual Studio 6 does not know __func__ or __FUNCTION__
// The rest of MS compilers use __FUNCTION__, not C99 __func__
// Also use _strtoui64 on modern M$ compilers
#if defined(_MSC_VER) && _MSC_VER < 1300
#define STRX(x) #x
#define STR(x) STRX(x)
#define __func__ "line " STR(__LINE__)
#define strtoull(x, y, z) strtoul(x, y, z)
#define strtoll(x, y, z) strtol(x, y, z)
#else
#define __func__  __FUNCTION__
#define strtoull(x, y, z) _strtoui64(x, y, z)
#define strtoll(x, y, z) _strtoi64(x, y, z)
#endif // _MSC_VER

#define ERRNO   GetLastError()
#define NO_SOCKLEN_T
#define SSL_LIB   "ssleay32.dll"
#define CRYPTO_LIB  "libeay32.dll"
#define DIRSEP '\\'
#define IS_DIRSEP_CHAR(c) ((c) == '/' || (c) == '\\')
#define O_NONBLOCK  0
#if !defined(EWOULDBLOCK)
#define EWOULDBLOCK  WSAEWOULDBLOCK
#endif // !EWOULDBLOCK
#define _POSIX_
#define INT64_FMT  "I64d"

#define SHUT_WR 1
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define sleep(x) Sleep((x) * 1000)

#define pipe(x) _pipe(x, BUFSIZ, _O_BINARY)
#define popen(x, y) _popen(x, y)
#define pclose(x) _pclose(x)
#define close(x) _close(x)
#define dlsym(x,y) GetProcAddress((HINSTANCE) (x), (y))
#define RTLD_LAZY  0
#define fseeko(x, y, z) fseek((x), (y), (z))
#define fdopen(x, y) _fdopen((x), (y))
#define write(x, y, z) _write((x), (y), (unsigned) z)
#define read(x, y, z) _read((x), (y), (unsigned) z)
#define flockfile(x) (void) 0
#define funlockfile(x) (void) 0

#if !defined(fileno)
#define fileno(x) _fileno(x)
#endif // !fileno MINGW #defines fileno

typedef HANDLE pthread_mutex_t;
typedef HANDLE pthread_cond_t;
typedef DWORD pthread_t;
#define pid_t HANDLE // MINGW typedefs pid_t to int. Using #define here.

struct timespec {
  long tv_nsec;
  long tv_sec;
};

static int pthread_mutex_lock(pthread_mutex_t *);
static int pthread_mutex_unlock(pthread_mutex_t *);
static FILE *mg_fopen(const char *path, const char *mode);

#if defined(HAVE_STDINT)
#include <stdint.h>
#else
typedef unsigned int  uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned __int64 uint64_t;
typedef __int64   int64_t;
#define INT64_MAX  9223372036854775807
#endif // HAVE_STDINT

// POSIX dirent interface
struct dirent {
  char d_name[FILENAME_MAX];
};

typedef struct DIR {
  HANDLE   handle;
  WIN32_FIND_DATAW info;
  struct dirent  result;
} DIR;

#else    // UNIX  specific
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdint.h>
#include <inttypes.h>

#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <pthread.h>
#define SSL_LIB   "libssl.so"
#define CRYPTO_LIB  "libcrypto.so"
#define DIRSEP   '/'
#define IS_DIRSEP_CHAR(c) ((c) == '/')
#define O_BINARY  0
#define closesocket(a) close(a)
#define mg_fopen(x, y) fopen(x, y)
#define mg_mkdir(x, y) mkdir(x, y)
#define mg_remove(x) remove(x)
#define mg_rename(x, y) rename(x, y)
#define ERRNO errno
#define INVALID_SOCKET (-1)
#define INT64_FMT PRId64
typedef int SOCKET;

#endif // End of Windows and UNIX specific includes

#include "mongoose.h"

#define MONGOOSE_VERSION "2.9"
#define PASSWORDS_FILE_NAME ".htpasswd"
#define CGI_ENVIRONMENT_SIZE 4096
#define MAX_CGI_ENVIR_VARS 64
#define MAX_REQUEST_SIZE 8192
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define DEBUG_MGS_PREFIX "*** Mongoose debug *** "

#if defined(DEBUG)
#define DEBUG_TRACE(x) do {printf x; putchar('\n'); fflush(stdout);} while (0)
#else
#define DEBUG_TRACE(x)
#endif // DEBUG

// Darwin prior to 7.0 and Win32 do not have socklen_t
#ifdef NO_SOCKLEN_T
typedef int socklen_t;
#endif // NO_SOCKLEN_T

typedef enum {MG_FALSE, MG_TRUE} bool_t;
typedef void * (*mg_thread_func_t)(void *);

static const char *http_500_error = "Internal Server Error";

// Snatched from OpenSSL includes. I put the prototypes here to be independent
// from the OpenSSL source installation. Having this, mongoose + SSL can be
// built on any system with binary SSL libraries installed.
typedef struct ssl_st SSL;
typedef struct ssl_method_st SSL_METHOD;
typedef struct ssl_ctx_st SSL_CTX;

#define SSL_ERROR_WANT_READ 2
#define SSL_ERROR_WANT_WRITE 3
#define SSL_FILETYPE_PEM 1
#define CRYPTO_LOCK  1

// Dynamically loaded SSL functionality
struct ssl_func {
  const char *name;   // SSL function name
  void  (*ptr)(void); // Function pointer
};

#define SSL_free(x) (* (void (*)(SSL *)) ssl_sw[0].ptr)(x)
#define SSL_accept(x) (* (int (*)(SSL *)) ssl_sw[1].ptr)(x)
#define SSL_connect(x) (* (int (*)(SSL *)) ssl_sw[2].ptr)(x)
#define SSL_read(x,y,z) (* (int (*)(SSL *, void *, int))   \
    ssl_sw[3].ptr)((x),(y),(z))
#define SSL_write(x,y,z) (* (int (*)(SSL *, const void *,int))  \
    ssl_sw[4].ptr)((x), (y), (z))
#define SSL_get_error(x,y)(* (int (*)(SSL *, int)) ssl_sw[5])((x), (y))
#define SSL_set_fd(x,y) (* (int (*)(SSL *, SOCKET)) ssl_sw[6].ptr)((x), (y))
#define SSL_new(x) (* (SSL * (*)(SSL_CTX *)) ssl_sw[7].ptr)(x)
#define SSL_CTX_new(x) (* (SSL_CTX * (*)(SSL_METHOD *)) ssl_sw[8].ptr)(x)
#define SSLv23_server_method() (* (SSL_METHOD * (*)(void)) ssl_sw[9].ptr)()
#define SSL_library_init() (* (int (*)(void)) ssl_sw[10].ptr)()
#define SSL_CTX_use_PrivateKey_file(x,y,z) (* (int (*)(SSL_CTX *, \
        const char *, int)) ssl_sw[11].ptr)((x), (y), (z))
#define SSL_CTX_use_certificate_file(x,y,z) (* (int (*)(SSL_CTX *, \
        const char *, int)) ssl_sw[12].ptr)((x), (y), (z))
#define SSL_CTX_set_default_passwd_cb(x,y) \
  (* (void (*)(SSL_CTX *, mg_callback_t)) ssl_sw[13].ptr)((x),(y))
#define SSL_CTX_free(x) (* (void (*)(SSL_CTX *)) ssl_sw[14].ptr)(x)
#define ERR_get_error() (* (unsigned long (*)(void)) ssl_sw[15].ptr)()
#define ERR_error_string(x, y) (* (char * (*)(unsigned long, char *)) ssl_sw[16].ptr)((x), (y))
#define SSL_load_error_strings() (* (void (*)(void)) ssl_sw[17].ptr)()

#define CRYPTO_num_locks() (* (int (*)(void)) crypto_sw[0].ptr)()
#define CRYPTO_set_locking_callback(x)     \
  (* (void (*)(void (*)(int, int, const char *, int))) \
   crypto_sw[1].ptr)(x)
#define CRYPTO_set_id_callback(x)     \
  (* (void (*)(unsigned long (*)(void))) crypto_sw[2].ptr)(x)

// set_ssl_option() function updates this array.
// It loads SSL library dynamically and changes NULLs to the actual addresses
// of respective functions. The macros above (like SSL_connect()) are really
// just calling these functions indirectly via the pointer.
static struct ssl_func ssl_sw[] = {
  {"SSL_free",   NULL},
  {"SSL_accept",   NULL},
  {"SSL_connect",   NULL},
  {"SSL_read",   NULL},
  {"SSL_write",   NULL},
  {"SSL_get_error",  NULL},
  {"SSL_set_fd",   NULL},
  {"SSL_new",   NULL},
  {"SSL_CTX_new",   NULL},
  {"SSLv23_server_method", NULL},
  {"SSL_library_init",  NULL},
  {"SSL_CTX_use_PrivateKey_file", NULL},
  {"SSL_CTX_use_certificate_file",NULL},
  {"SSL_CTX_set_default_passwd_cb",NULL},
  {"SSL_CTX_free",  NULL},
  {"ERR_get_error",  NULL},
  {"ERR_error_string", NULL},
  {"SSL_load_error_strings", NULL},
  {NULL,    NULL}
};

// Similar array as ssl_sw. These functions could be located in different lib.
static struct ssl_func crypto_sw[] = {
  {"CRYPTO_num_locks",  NULL},
  {"CRYPTO_set_locking_callback", NULL},
  {"CRYPTO_set_id_callback", NULL},
  {NULL,    NULL}
};

static const char *month_names[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// Unified socket address. For IPv6 support, add IPv6 address structure
// in the union u.
struct usa {
  socklen_t len;
  union {
    struct sockaddr sa;
    struct sockaddr_in sin;
  } u;
};

// Describes a string (chunk of memory).
struct vec {
  const char *ptr;
  size_t len;
};

// Structure used by mg_stat() function. Uses 64 bit file length.
struct mgstat {
  bool_t is_directory;  // Directory marker 
  int64_t size;         // File size
  time_t mtime;         // Modification time
};

// Describes listening socket, or socket which was accept()-ed by the master
// thread and queued for future handling by the worker thread.
struct socket {
  struct socket *next;  // Linkage
  SOCKET sock;          // Listening socket
  struct usa lsa;       // Local socket address
  struct usa rsa;       // Remote socket address
  bool_t is_ssl;        // Is socket SSL-ed
};

struct mg_context {
  int stop_flag;     // Should we stop event loop
  SSL_CTX *ssl_ctx;  // SSL context
  const struct mg_config *config; // Mongoose configuration

  struct socket *listening_sockets;

  int num_threads; // Number of threads
  int num_idle;    // Number of idle threads

  pthread_mutex_t mutex;   // Protects (max|num)_threads
  pthread_cond_t thr_cond; // Condvar for thread sync

  struct socket queue[20];   // Accepted sockets
  int sq_head;               // Head of the socket queue
  int sq_tail;               // Tail of the socket queue
  pthread_cond_t empty_cond; // Socket queue empty condvar
  pthread_cond_t full_cond;  // Socket queue full condvar
};

struct mg_connection {
  struct mg_request_info request_info;
  struct mg_context *ctx;
  SSL *ssl;                   // SSL descriptor
  struct socket client;       // Connected client
  time_t birth_time;          // Time connection was accepted
  int64_t num_bytes_sent;     // Total bytes sent to client
  int64_t content_len;     // Content-Length header value
  int64_t consumed_content;   // How many bytes of content is already read
  char buf[MAX_REQUEST_SIZE]; // Buffer for received data
  int request_len;            // Size of the request + headers in a buffer
  int data_len;               // Total size of data in a buffer
};

// Print error message to the opened error log stream.
static void cry(struct mg_connection *conn, const char *fmt, ...) {
  char buf[BUFSIZ];
  mg_callback_t log_callback;
  enum mg_error_t processed = MG_ERROR;
  va_list ap;
  FILE *fp;
  time_t timestamp;

  va_start(ap, fmt);
  (void) vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  // Do not lock when getting the callback value, here and below.
  // I suppose this is fine, since function cannot disappear in the
  // same way string option can.
  log_callback = conn->ctx->config->event_log_handler;
  conn->request_info.log_message = buf;
  if (log_callback != NULL) {
    processed = log_callback(conn, &conn->request_info);
  }
  if (processed == MG_ERROR) {
    fp = conn->ctx->config->error_log_file == NULL ? stderr :
      mg_fopen(conn->ctx->config->error_log_file, "a+");

    if (fp != NULL) {
      flockfile(fp);
      timestamp = time(NULL);

      (void) fprintf(fp,
          "[%010lu] [error] [client %s] ",
          (unsigned long) timestamp,
          inet_ntoa(conn->client.rsa.u.sin.sin_addr));

      if (conn->request_info.request_method != NULL) {
        (void) fprintf(fp, "%s %s: ",
            conn->request_info.request_method,
            conn->request_info.uri);
      }

      (void) fprintf(fp, "%s", buf);
      fputc('\n', fp);
      funlockfile(fp);
      if (fp != stderr) {
        fclose(fp);
      }
    }
  }
  conn->request_info.log_message = NULL;
}

// Return OpenSSL error message
static const char *ssl_error(void) {
  unsigned long err;
  err = ERR_get_error();
  return err == 0 ? "" : ERR_error_string(err, NULL);
}

// Return fake connection structure. Used for logging, if connection
// is not applicable at the moment of logging.
static struct mg_connection *fc(struct mg_context *ctx) {
  static struct mg_connection fake_connection;
  fake_connection.ctx = ctx;
  return &fake_connection;
}

const char *mg_version(void) {
  return MONGOOSE_VERSION;
}

static void mg_strlcpy(register char *dst, register const char *src, size_t n) {
  for (; *src != '\0' && n > 1; n--) {
    *dst++ = *src++;
  }
  *dst = '\0';
}

static int lowercase(const char *s) {
  return tolower(* (unsigned char *) s);
}

static int mg_strncasecmp(const char *s1, const char *s2, size_t len) {
  int diff = 0;

  if (len > 0)
    do {
      diff = lowercase(s1++) - lowercase(s2++);
    } while (diff == 0 && s1[-1] != '\0' && --len > 0);

  return diff;
}

static int mg_strcasecmp(const char *s1, const char *s2) {
  int diff;

  do {
    diff = lowercase(s1++) - lowercase(s2++);
  } while (diff == 0 && s1[-1] != '\0');

  return diff;
}

static char * mg_strndup(const char *ptr, size_t len) {
  char *p;

  if ((p = (char *) malloc(len + 1)) != NULL) {
    mg_strlcpy(p, ptr, len + 1);
  }

  return p;
}

static char * mg_strdup(const char *str) {
  return mg_strndup(str, strlen(str));
}

// Like snprintf(), but never returns negative value, or the value
// that is larger than a supplied buffer.
// Thanks to Adam Zeldis to pointing snprintf()-caused vulnerability
// in his audit report.
static int mg_vsnprintf(struct mg_connection *conn, char *buf, size_t buflen,
                        const char *fmt, va_list ap) {
  int n;

  if (buflen == 0)
    return 0;

  n = vsnprintf(buf, buflen, fmt, ap);

  if (n < 0) {
    cry(conn, "vsnprintf error");
    n = 0;
  } else if (n >= (int) buflen) {
    cry(conn, "truncating vsnprintf buffer: [%.*s]",
        n > 200 ? 200 : n, buf);
    n = (int) buflen - 1;
  }
  buf[n] = '\0';

  return n;
}

static int mg_snprintf(struct mg_connection *conn, char *buf, size_t buflen,
                       const char *fmt, ...) {
  va_list ap;
  int n;

  va_start(ap, fmt);
  n = mg_vsnprintf(conn, buf, buflen, fmt, ap);
  va_end(ap);

  return n;
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

// Return HTTP header value, or NULL if not found.
static const char *get_header(const struct mg_request_info *ri,
                              const char *name) {
  int i;

  for (i = 0; i < ri->num_headers; i++)
    if (!mg_strcasecmp(name, ri->http_headers[i].name))
      return ri->http_headers[i].value;

  return NULL;
}

const char *mg_get_header(const struct mg_connection *conn, const char *name) {
  return get_header(&conn->request_info, name);
}

// A helper function for traversing comma separated list of values.
// It returns a list pointer shifted to the next value, of NULL if the end
// of the list found.
// Value is stored in val vector. If value has form "x=y", then eq_val
// vector is initialized to point to the "y" part, and val vector length
// is adjusted to point only to "x".
static const char *next_option(const char *list, struct vec *val,
                               struct vec *eq_val) {
  if (list == NULL || *list == '\0') {
    /* End of the list */
    list = NULL;
  } else {
    val->ptr = list;
    if ((list = strchr(val->ptr, ',')) != NULL) {
      /* Comma found. Store length and shift the list ptr */
      val->len = list - val->ptr;
      list++;
    } else {
      /* This value is the last one */
      list = val->ptr + strlen(val->ptr);
      val->len = list - val->ptr;
    }

    if (eq_val != NULL) {
      /*
       * Value has form "x=y", adjust pointers and lengths
       * so that val points to "x", and eq_val points to "y".
       */
      eq_val->len = 0;
      eq_val->ptr = memchr(val->ptr, '=', val->len);
      if (eq_val->ptr != NULL) {
        eq_val->ptr++;  /* Skip over '=' character */
        eq_val->len = val->ptr + val->len - eq_val->ptr;
        val->len = (eq_val->ptr - val->ptr) - 1;
      }
    }
  }

  return list;
}

#if !defined(NO_CGI)
static bool_t match_extension(const char *path, const char *ext_list) {
  struct vec ext_vec;
  size_t path_len;

  path_len = strlen(path);

  while ((ext_list = next_option(ext_list, &ext_vec, NULL)) != NULL)
    if (ext_vec.len < path_len &&
        mg_strncasecmp(path + path_len - ext_vec.len,
          ext_vec.ptr, ext_vec.len) == 0)
      return MG_TRUE;

  return MG_FALSE;
}
#endif // !NO_CGI

static void send_http_error(struct mg_connection *conn, int status,
                            const char *reason, const char *fmt, ...) {
  char buf[BUFSIZ];
  va_list ap;
  int len;
  mg_callback_t error_handler;
  bool_t handled;

  DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: %d %s", __func__, status, reason));
  conn->request_info.status_code = status;

  error_handler = conn->ctx->config->http_error_handler;
  handled = error_handler ?
    error_handler(conn, &conn->request_info) : MG_ERROR;

  if (handled == MG_ERROR) {
    buf[0] = '\0';
    len = 0;

    /* Errors 1xx, 204 and 304 MUST NOT send a body */
    if (status > 199 && status != 204 && status != 304) {
      len = mg_snprintf(conn, buf, sizeof(buf), "Error %d: %s", status, reason);
      cry(conn, "%s", buf);

      va_start(ap, fmt);
      len += mg_vsnprintf(conn, buf + len, sizeof(buf) - len, fmt, ap);
      va_end(ap);
      conn->num_bytes_sent = len;
    }

    (void) mg_printf(conn,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n%s", status, reason, len, buf);
  }
}

#ifdef _WIN32
static int pthread_mutex_init(pthread_mutex_t *mutex, void *unused) {
  unused = NULL;
  *mutex = CreateMutex(NULL, MG_FALSE, NULL);
  return *mutex == NULL ? -1 : 0;
}

static int pthread_mutex_destroy(pthread_mutex_t *mutex) {
  return CloseHandle(*mutex) == 0 ? -1 : 0;
}

static int pthread_mutex_lock(pthread_mutex_t *mutex) {
  return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

static int pthread_mutex_unlock(pthread_mutex_t *mutex) {
  return ReleaseMutex(*mutex) == 0 ? -1 : 0;
}

static int pthread_cond_init(pthread_cond_t *cv, const void *unused) {
  unused = NULL;
  *cv = CreateEvent(NULL, MG_FALSE, MG_FALSE, NULL);
  return *cv == NULL ? -1 : 0;
}

static int pthread_cond_timedwait(pthread_cond_t *cv, pthread_mutex_t *mutex,
                                  const struct timespec *ts) {
  DWORD status;
  DWORD msec = INFINITE;
  time_t now;

  if (ts != NULL) {
    now = time(NULL);
    msec = 1000 * (now > ts->tv_sec ? 0 : ts->tv_sec - now);
  }

  (void) ReleaseMutex(*mutex);
  status = WaitForSingleObject(*cv, msec);
  (void) WaitForSingleObject(*mutex, INFINITE);

  return status == WAIT_OBJECT_0 ? 0 : -1;
}

static int pthread_cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex) {
  return pthread_cond_timedwait(cv, mutex, NULL);
}

static int pthread_cond_signal(pthread_cond_t *cv) {
  return SetEvent(*cv) == 0 ? -1 : 0;
}

static int pthread_cond_destroy(pthread_cond_t *cv) {
  return CloseHandle(*cv) == 0 ? -1 : 0;
}

static pthread_t pthread_self(void) {
  return GetCurrentThreadId();
}

// For Windows, change all slashes to backslashes in path names.
static void change_slashes_to_backslashes(char *path) {
  int i;

  for (i = 0; path[i] != '\0'; i++) {
    if (path[i] == '/')
      path[i] = '\\';
    // i > 0 check is to preserve UNC paths, like \\server\file.txt
    if (path[i] == '\\' && i > 0)
      while (path[i + 1] == '\\' || path[i + 1] == '/')
        (void) memmove(path + i + 1,
            path + i + 2, strlen(path + i + 1));
  }
}

// Encode 'path' which is assumed UTF-8 string, into UNICODE string.
// wbuf and wbuf_len is a target buffer and its length.
static void to_unicode(const char *path, wchar_t *wbuf, size_t wbuf_len) {
  char buf[FILENAME_MAX], *p;

  mg_strlcpy(buf, path, sizeof(buf));
  change_slashes_to_backslashes(buf);

  // Point p to the end of the file name
  p = buf + strlen(buf) - 1;

  // Trim trailing backslash character
  while (p > buf && *p == '\\' && p[-1] != ':') {
    *p-- = '\0';
  }

   // Protect from CGI code disclosure.
   // This is very nasty hole. Windows happily opens files with
   // some garbage in the end of file name. So fopen("a.cgi    ", "r")
   // actually opens "a.cgi", and does not return an error!
  if (*p == 0x20 ||               // No space at the end
      (*p == 0x2e && p > buf) ||  // No '.' but allow '.' as full path
      *p == 0x2b ||               // No '+'
      (*p & ~0x7f)) {             // And generally no non-ascii chars
    (void) fprintf(stderr, "Rejecting suspicious path: [%s]", buf);
    buf[0] = '\0';
  }

  (void) MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int) wbuf_len);
}

#if defined(_WIN32_WCE)
static time_t time(time_t *ptime) {
  time_t t;
  SYSTEMTIME st;
  FILETIME ft;

  GetSystemTime(&st);
  SystemTimeToFileTime(&st, &ft);
  t = SYS2UNIX_TIME(ft.dwLowDateTime, ft.dwHighDateTime);

  if (ptime != NULL) {
    *ptime = t;
  }

  return t;
}

static time_t mktime(struct tm *ptm) {
  SYSTEMTIME st;
  FILETIME ft, lft;

  st.wYear = ptm->tm_year + 1900;
  st.wMonth = ptm->tm_mon + 1;
  st.wDay = ptm->tm_mday;
  st.wHour = ptm->tm_hour;
  st.wMinute = ptm->tm_min;
  st.wSecond = ptm->tm_sec;
  st.wMilliseconds = 0;

  SystemTimeToFileTime(&st, &ft);
  LocalFileTimeToFileTime(&ft, &lft);
  return (time_t) ((MAKEUQUAD(lft.dwLowDateTime, lft.dwHighDateTime) -
                    EPOCH_DIFF) / RATE_DIFF);
}

static struct tm *localtime(const time_t *ptime, struct tm *ptm) {
  int64_t t = ((int64_t) *ptime) * RATE_DIFF + EPOCH_DIFF;
  FILETIME ft, lft;
  SYSTEMTIME st;
  TIME_ZONE_INFORMATION tzinfo;

  if (ptm == NULL) {
    return NULL;
  }

  * (int64_t *) &ft = t;
  FileTimeToLocalFileTime(&ft, &lft);
  FileTimeToSystemTime(&lft, &st);
  ptm->tm_year = st.wYear - 1900;
  ptm->tm_mon = st.wMonth - 1;
  ptm->tm_wday = st.wDayOfWeek;
  ptm->tm_mday = st.wDay;
  ptm->tm_hour = st.wHour;
  ptm->tm_min = st.wMinute;
  ptm->tm_sec = st.wSecond;
  ptm->tm_yday = 0; // hope nobody uses this
  ptm->tm_isdst =
    GetTimeZoneInformation(&tzinfo) == TIME_ZONE_ID_DAYLIGHT ? 1 : 0;

  return ptm;
}

static size_t strftime(char *dst, size_t dst_size, const char *fmt,
                       const struct tm *tm) {
  (void) snprintf(dst, dst_size, "implement strftime() for WinCE");
  return 0;
} 
#endif

static int mg_rename(const char* oldname, const char* newname) {
  wchar_t woldbuf[FILENAME_MAX];
  wchar_t wnewbuf[FILENAME_MAX];

  to_unicode(oldname, woldbuf, ARRAY_SIZE(woldbuf));
  to_unicode(newname, wnewbuf, ARRAY_SIZE(wnewbuf));

  return MoveFileW(woldbuf, wnewbuf) ? 0 : -1;
}


static FILE *mg_fopen(const char *path, const char *mode) {
  wchar_t wbuf[FILENAME_MAX], wmode[20];

  to_unicode(path, wbuf, ARRAY_SIZE(wbuf));
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, ARRAY_SIZE(wmode));

  return _wfopen(wbuf, wmode);
}

static int mg_stat(const char *path, struct mgstat *stp) {
  int ok = -1; // Error
  wchar_t wbuf[FILENAME_MAX];
  WIN32_FILE_ATTRIBUTE_DATA info;

  to_unicode(path, wbuf, ARRAY_SIZE(wbuf));

  if (GetFileAttributesExW(wbuf, GetFileExInfoStandard, &info) != 0) {
    stp->size = MAKEUQUAD(info.nFileSizeLow, info.nFileSizeHigh);
    stp->mtime = SYS2UNIX_TIME(info.ftLastWriteTime.dwLowDateTime,
        info.ftLastWriteTime.dwHighDateTime);
    stp->is_directory =
      info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    ok = 0;  // Success
  }

  return ok;
}

static int mg_remove(const char *path) {
  wchar_t wbuf[FILENAME_MAX];
  to_unicode(path, wbuf, ARRAY_SIZE(wbuf));
  return DeleteFileW(wbuf) ? 0 : -1;
}

static int mg_mkdir(const char *path, int mode) {
  char buf[FILENAME_MAX];
  wchar_t wbuf[FILENAME_MAX];

  mode = 0; // Unused
  mg_strlcpy(buf, path, sizeof(buf));
  change_slashes_to_backslashes(buf);

  (void) MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, sizeof(wbuf));

  return CreateDirectoryW(wbuf, NULL) ? 0 : -1;
}

// Implementation of POSIX opendir/closedir/readdir for Windows.
static DIR * opendir(const char *name) {
  DIR *dir = NULL;
  wchar_t wpath[FILENAME_MAX];
  DWORD attrs;

  if (name == NULL) {
    SetLastError(ERROR_BAD_ARGUMENTS);
  } else if ((dir = (DIR *) malloc(sizeof(*dir))) == NULL) {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  } else {
    to_unicode(name, wpath, ARRAY_SIZE(wpath));
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

struct dirent * readdir(DIR *dir) {
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

#define set_close_on_exec(fd) // No FD_CLOEXEC on Windows

static int start_thread(struct mg_context *ctx, mg_thread_func_t func,
                        void *param) {
  HANDLE hThread;
  ctx = NULL; // Unused

  hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) func, param, 0,
                         NULL);
  if (hThread != NULL) {
    (void) CloseHandle(hThread);
  }

  return hThread == NULL ? -1 : 0;
}

static HANDLE dlopen(const char *dll_name, int flags) {
  wchar_t wbuf[FILENAME_MAX];

  flags = 0; // Unused
  to_unicode(dll_name, wbuf, ARRAY_SIZE(wbuf));

  return LoadLibraryW(wbuf);
}

#if !defined(NO_CGI)
static int kill(pid_t pid, int sig_num) {
  (void) TerminateProcess(pid, sig_num);
  (void) CloseHandle(pid);
  return 0;
}

static pid_t spawn_process(struct mg_connection *conn, const char *prog,
                           char *envblk, char *envp[], int fd_stdin,
                           int fd_stdout, const char *dir) {
  HANDLE me;
  char *p, *interp, cmdline[FILENAME_MAX], line[FILENAME_MAX];
  FILE *fp;
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;

  envp = NULL; // Unused

  (void) memset(&si, 0, sizeof(si));
  (void) memset(&pi, 0, sizeof(pi));

  // TODO(lsm): redirect CGI errors to the error log file
  si.cb  = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;

  me = GetCurrentProcess();
  (void) DuplicateHandle(me, (HANDLE) _get_osfhandle(fd_stdin), me,
      &si.hStdInput, 0, MG_TRUE, DUPLICATE_SAME_ACCESS);
  (void) DuplicateHandle(me, (HANDLE) _get_osfhandle(fd_stdout), me,
      &si.hStdOutput, 0, MG_TRUE, DUPLICATE_SAME_ACCESS);

  // If CGI file is a script, try to read the interpreter line
  interp = conn->ctx->options[OPT_CGI_INTERPRETER];
  if (interp == NULL) {
    line[2] = '\0';
    (void) mg_snprintf(conn, cmdline, sizeof(cmdline), "%s%c%s",
        dir, DIRSEP, prog);
    if ((fp = fopen(cmdline, "r")) != NULL) {
      (void) fgets(line, sizeof(line), fp);
      if (memcmp(line, "#!", 2) != 0)
        line[2] = '\0';
      // Trim whitespaces from interpreter name
      for (p = &line[strlen(line) - 1]; p > line &&
          isspace(*p); p--) {
        *p = '\0';
      }
      (void) fclose(fp);
    }
    interp = line + 2;
  }

  if ((p = (char *) strrchr(prog, '/')) != NULL) {
    prog = p + 1;
  }

  (void) mg_snprintf(conn, cmdline, sizeof(cmdline), "%s%s%s",
      interp, interp[0] == '\0' ? "" : " ", prog);

  (void) mg_snprintf(conn, line, sizeof(line), "%s", dir);
  change_slashes_to_backslashes(line);

  DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: Running [%s]", __func__, cmdline));
  if (CreateProcessA(NULL, cmdline, NULL, NULL, MG_TRUE,
        CREATE_NEW_PROCESS_GROUP, envblk, line, &si, &pi) == 0) {
    cry(conn, "%s: CreateProcess(%s): %d",
        __func__, cmdline, ERRNO);
    pi.hProcess = (pid_t) -1;
  } else {
    (void) close(fd_stdin);
    (void) close(fd_stdout);
  }

  (void) CloseHandle(si.hStdOutput);
  (void) CloseHandle(si.hStdInput);
  (void) CloseHandle(pi.hThread);

  return (pid_t) pi.hProcess;
}
#endif /* !NO_CGI */

static int set_non_blocking_mode(SOCKET sock) {
  unsigned long on = 1;
  return ioctlsocket(sock, FIONBIO, &on);
}

#else
static int mg_stat(const char *path, struct mgstat *stp) {
  struct stat st;
  int ok;

  if (stat(path, &st) == 0) {
    ok = 0;
    stp->size = st.st_size;
    stp->mtime = st.st_mtime;
    stp->is_directory = S_ISDIR(st.st_mode);
  } else {
    ok = -1;
  }

  return ok;
}

static void set_close_on_exec(int fd) {
  (void) fcntl(fd, F_SETFD, FD_CLOEXEC);
}

static int start_thread(struct mg_context *ctx, mg_thread_func_t func,
                        void *param) {
  pthread_t thread_id;
  pthread_attr_t attr;
  int retval;

  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  (void) pthread_attr_setstacksize(&attr, sizeof(struct mg_connection) * 2);

  if ((retval = pthread_create(&thread_id, &attr, func, param)) != 0) {
    cry(fc(ctx), "%s: %s", __func__, strerror(retval));
  }

  return retval;
}

#ifndef NO_CGI
static pid_t spawn_process(struct mg_connection *conn, const char *prog,
                           char *envblk, char *envp[], int fd_stdin,
                           int fd_stdout, const char *dir) {
  pid_t pid;
  const char *interp;

  envblk = NULL; // Unused

  if ((pid = fork()) == -1) {
    // Parent
    send_http_error(conn, 500, http_500_error, "fork(): %s", strerror(ERRNO));
  } else if (pid == 0) {
    // Child
    if (chdir(dir) != 0) {
      cry(conn, "%s: chdir(%s): %s", __func__, dir, strerror(ERRNO));
    } else if (dup2(fd_stdin, 0) == -1) {
      cry(conn, "%s: dup2(stdin, %d): %s", __func__, fd_stdin, strerror(ERRNO));
    } else if (dup2(fd_stdout, 1) == -1) {
      cry(conn, "%s: dup2(stdout, %d): %s", __func__, fd_stdout, strerror(ERRNO));
    } else {
      (void) dup2(fd_stdout, 2);
      (void) close(fd_stdin);
      (void) close(fd_stdout);

      // Execute CGI program. No need to lock: new process
      interp = conn->ctx->config->cgi_interpreter;
      if (interp == NULL) {
        (void) execle(prog, prog, NULL, envp);
        cry(conn, "%s: execle(%s): %s", __func__, prog, strerror(ERRNO));
      } else {
        (void) execle(interp, interp, prog, NULL, envp);
        cry(conn, "%s: execle(%s %s): %s", __func__, interp, prog,
            strerror(ERRNO));
      }
    }
    exit(EXIT_FAILURE);
  } else {
    // Parent. Close stdio descriptors
    (void) close(fd_stdin);
    (void) close(fd_stdout);
  }

  return pid;
}
#endif // !NO_CGI

static int set_non_blocking_mode(SOCKET sock) {
  int flags;

  flags = fcntl(sock, F_GETFL, 0);
  (void) fcntl(sock, F_SETFL, flags | O_NONBLOCK);

  return 0;
}
#endif // _WIN32

// Write data to the IO channel - opened file descriptor, socket or SSL
// descriptor. Return number of bytes written.
static int64_t push(FILE *fp, SOCKET sock, SSL *ssl, const char *buf,
                    int64_t len) {
  int64_t sent;
  int n, k;

  sent = 0;
  while (sent < len) {

    /* How many bytes we send in this iteration */
    k = len - sent > INT_MAX ? INT_MAX : (int) (len - sent);

    if (ssl != NULL) {
      n = SSL_write(ssl, buf + sent, k);
    } else if (fp != NULL) {
      n = fwrite(buf + sent, 1, k, fp);
      if (ferror(fp))
        n = -1;
    } else {
      n = send(sock, buf + sent, k, 0);
    }

    if (n < 0)
      break;

    sent += n;
  }

  return sent;
}

// Read from IO channel - opened file descriptor, socket, or SSL descriptor.
// Return number of bytes read.
static int pull(FILE *fp, SOCKET sock, SSL *ssl, char *buf, int len) {
  int nread;

  if (ssl != NULL) {
    nread = SSL_read(ssl, buf, len);
  } else if (fp != NULL) {
    nread = fread(buf, 1, (size_t) len, fp);
    if (ferror(fp))
      nread = -1;
  } else {
    nread = recv(sock, buf, (size_t) len, 0);
  }

  return nread;
}

int mg_read(struct mg_connection *conn, void *buf, size_t len) {
  return pull(NULL, conn->client.sock, conn->ssl, (char *) buf, (int) len);
}

int mg_write(struct mg_connection *conn, const void *buf, size_t len) {
  return (int) push(NULL, conn->client.sock, conn->ssl,
      (const char *) buf, (int64_t) len);
}

int mg_printf(struct mg_connection *conn, const char *fmt, ...) {
  char buf[MAX_REQUEST_SIZE];
  int len;
  va_list ap;

  va_start(ap, fmt);
  len = mg_vsnprintf(conn, buf, sizeof(buf), fmt, ap);
  va_end(ap);

  return mg_write(conn, buf, len);
}

// URL-decode input buffer into destination buffer.
// 0-terminate the destination buffer. Return the length of decoded data.
// form-url-encoded data differs from URI encoding in a way that it
// uses '+' as character for space, see RFC 1866 section 8.2.1
// http://ftp.ics.uci.edu/pub/ietf/html/rfc1866.txt
static size_t url_decode(const char *src, size_t src_len, char *dst,
                         size_t dst_len, bool_t is_form_url_encoded) {
  size_t i, j;
  int a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

  for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
    if (src[i] == '%' &&
        isxdigit(* (unsigned char *) (src + i + 1)) &&
        isxdigit(* (unsigned char *) (src + i + 2))) {
      a = tolower(* (unsigned char *) (src + i + 1));
      b = tolower(* (unsigned char *) (src + i + 2));
      dst[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
      i += 2;
    } else if (is_form_url_encoded && src[i] == '+') {
      dst[j] = ' ';
    } else {
      dst[j] = src[i];
    }
  }

  dst[j] = '\0'; /* Null-terminate the destination */

  return j;
}

// Scan given buffer and fetch the value of the given variable.
// It can be specified in query string, or in the POST data.
// Return NULL if the variable not found, or allocated 0-terminated value.
// It is caller's responsibility to free the returned value.
enum mg_error_t mg_get_var(const char *buf, size_t buf_len,
                           const char *var_name, char *var_value,
                           size_t var_value_len) {
  const char *p, *e, *s;
  char *val;
  size_t  var_len, len;
  enum mg_error_t ret_val;

  var_len = strlen(var_name);
  e = buf + buf_len;
  val = NULL;
  ret_val = MG_NOT_FOUND;
  var_value[0] = '\0';

  // buf is "var1=val1&var2=val2...". Find variable first
  for (p = buf; p + var_len < e; p++)
    if ((p == buf || p[-1] == '&') && p[var_len] == '=' &&
        !mg_strncasecmp(var_name, p, var_len)) {

      // Point p to variable value
      p += var_len + 1;

      // Point s to the end of the value
      s = (const char *) memchr(p, '&', e - p);
      if (s == NULL) {
        s = e;
      }

      /* Try to allocate the buffer */
      len = s - p;
      if (len >= var_value_len) {
        ret_val = MG_BUFFER_TOO_SMALL;
      } else {
        url_decode(p, len, var_value, len + 1, MG_TRUE);
        ret_val = MG_SUCCESS;
      }
      break;
    }

  return ret_val;
}

enum mg_error_t mg_get_qsvar(const struct mg_request_info *ri,
                             const char *var_name, char *var_value,
                             size_t var_value_len) {
  return ri->query_string == NULL ? MG_NOT_FOUND : mg_get_var(ri->query_string,
        strlen(ri->query_string), var_name, var_value, var_value_len);
}

enum mg_error_t mg_get_cookie(const struct mg_connection *conn,
                              const char *cookie_name, char *dst,
                              size_t dst_size) {
  const char *s, *p, *end;
  int len;

  dst[0] = '\0';
  if ((s = mg_get_header(conn, "Cookie")) == NULL) {
    return MG_NOT_FOUND;
  }

  len = strlen(cookie_name);
  end = s + strlen(s);

  for (; (s = strstr(s, cookie_name)) != NULL; s += len)
    if (s[len] == '=') {
      s += len + 1;
      if ((p = strchr(s, ' ')) == NULL)
        p = end;
      if (p[-1] == ';')
        p--;
      if (*s == '"' && p[-1] == '"' && p > s + 1) {
        s++;
        p--;
      }
      if ((size_t) (p - s) >= dst_size) {
        return MG_BUFFER_TOO_SMALL;
      } else {
        mg_strlcpy(dst, s, (p - s) + 1);
        return MG_SUCCESS;
      }
    }

  return MG_NOT_FOUND;
}

// Mongoose allows to specify multiple directories to serve,
// like /var/www,/~bob:/home/bob. That means that root directory depends on URI.
// This function returns root dir for given URI.
static int get_document_root(const struct mg_connection *conn,
                             struct vec *document_root) {
  const char *root, *uri;
  int len_of_matched_uri;
  struct vec uri_vec, path_vec;

  uri = conn->request_info.uri;
  len_of_matched_uri = 0;
  root = next_option(conn->ctx->config->document_root, document_root, NULL);

  while ((root = next_option(root, &uri_vec, &path_vec)) != NULL) {
    if (memcmp(uri, uri_vec.ptr, uri_vec.len) == 0) {
      *document_root = path_vec;
      len_of_matched_uri = uri_vec.len;
      break;
    }
  }

  return len_of_matched_uri;
}

static void convert_uri_to_file_name(struct mg_connection *conn,
                                     const char *uri, char *buf,
                                     size_t buf_len) {
  struct vec vec;
  int match_len;

  match_len = get_document_root(conn, &vec);
  mg_snprintf(conn, buf, buf_len, "%.*s%s", vec.len, vec.ptr, uri + match_len);

#ifdef _WIN32
  change_slashes_to_backslashes(buf);
#endif /* _WIN32 */

  DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: [%s] -> [%s]", __func__, uri, buf));
}

// Setup listening socket on given address, return socket.
// Address format: [local_ip_address:]port_number
static SOCKET mg_open_listening_port(struct mg_context *ctx, const char *str,
                                     struct usa *usa) {
  SOCKET sock;
  int on = 1, a, b, c, d, port;

  // MacOS needs that. If we do not zero it, bind() will fail.
  (void) memset(usa, 0, sizeof(*usa));

  if (sscanf(str, "%d.%d.%d.%d:%d", &a, &b, &c, &d, &port) == 5) {
    // IP address to bind to is specified
    usa->u.sin.sin_addr.s_addr =
      htonl((a << 24) | (b << 16) | (c << 8) | d);
  } else if (sscanf(str, "%d", &port) == 1) {
    // Only port number is specified. Bind to all addresses
    usa->u.sin.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    return INVALID_SOCKET;
  }

  usa->len   = sizeof(usa->u.sin);
  usa->u.sin.sin_family  = AF_INET;
  usa->u.sin.sin_port  = htons((uint16_t) port);

  if ((sock = socket(PF_INET, SOCK_STREAM, 6)) != INVALID_SOCKET &&
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
        (char *) &on, sizeof(on)) == 0 &&
      bind(sock, &usa->u.sa, usa->len) == 0 &&
      listen(sock, 20) == 0) {
    // Success
    set_close_on_exec(sock);
  } else {
    // Error
    cry(fc(ctx), "%s(%d): %s", __func__, port, strerror(ERRNO));
    if (sock != INVALID_SOCKET)
      (void) closesocket(sock);
    sock = INVALID_SOCKET;
  }

  return sock;
}

// Check whether full request is buffered. Return:
//   -1  if request is malformed
//    0  if request is not yet fully buffered
//   >0  actual request length, including last \r\n\r\n
static int get_request_len(const char *buf, int buflen) {
  const char *s, *e;
  int len = 0;

  for (s = buf, e = s + buflen - 1; len <= 0 && s < e; s++)
    // Control characters are not allowed but >=128 is.
    if (!isprint(* (unsigned char *) s) && *s != '\r' &&
        *s != '\n' && * (unsigned char *) s < 128) {
      len = -1;
    } else if (s[0] == '\n' && s[1] == '\n') {
      len = (int) (s - buf) + 2;
    } else if (s[0] == '\n' && &s[1] < e &&
        s[1] == '\r' && s[2] == '\n') {
      len = (int) (s - buf) + 3;
    }

  return len;
}

// Convert month to the month number. Return -1 on error, or month number
static int month_number_to_month_name(const char *s) {
  size_t i;

  for (i = 0; i < ARRAY_SIZE(month_names); i++)
    if (!strcmp(s, month_names[i]))
      return (int) i;

  return -1;
}

// Parse date-time string, and return the corresponding time_t value
static time_t parse_date_string(const char *s) {
  time_t current_time;
  struct tm tm, *tmp;
  char mon[32];
  int sec, min, hour, mday, month, year;

  (void) memset(&tm, 0, sizeof(tm));
  sec = min = hour = mday = month = year = 0;

  if (((sscanf(s, "%d/%3s/%d %d:%d:%d",
            &mday, mon, &year, &hour, &min, &sec) == 6) ||
        (sscanf(s, "%d %3s %d %d:%d:%d",
                &mday, mon, &year, &hour, &min, &sec) == 6) ||
        (sscanf(s, "%*3s, %d %3s %d %d:%d:%d",
                &mday, mon, &year, &hour, &min, &sec) == 6) ||
        (sscanf(s, "%d-%3s-%d %d:%d:%d",
                &mday, mon, &year, &hour, &min, &sec) == 6)) &&
      (month = month_number_to_month_name(mon)) != -1) {
    tm.tm_mday = mday;
    tm.tm_mon = month;
    tm.tm_year = year;
    tm.tm_hour = hour;
    tm.tm_min = min;
    tm.tm_sec = sec;
  }

  if (tm.tm_year > 1900) {
    tm.tm_year -= 1900;
  } else if (tm.tm_year < 70) {
    tm.tm_year += 100;
  }

  // Set Daylight Saving Time field
  current_time = time(NULL);
  tmp = localtime(&current_time);
  tm.tm_isdst = tmp->tm_isdst;

  return mktime(&tm);
}

// Protect against directory disclosure attack by removing '..',
// excessive '/' and '\' characters
static void remove_double_dots_and_double_slashes(char *s) {
  char *p = s;

  while (*s != '\0') {
    *p++ = *s++;
    if (s[-1] == '/' || s[-1] == '\\') {
      // Skip all following slashes and backslashes
      while (*s == '/' || *s == '\\') {
        s++;
      }

      // Skip all double-dots
      while (*s == '.' && s[1] == '.') {
        s += 2;
      }
    }
  }
  *p = '\0';
}

static const struct {
  const char *extension;
  size_t ext_len;
  const char *mime_type;
  size_t mime_type_len;
} builtin_mime_types[] = {
  {".html", 5, "text/html",   9},
  {".htm", 4, "text/html",   9},
  {".shtm", 5, "text/html",   9},
  {".shtml", 6, "text/html",   9},
  {".css", 4, "text/css",   8},
  {".js",  3, "application/x-javascript", 24},
  {".ico", 4, "image/x-icon",   12},
  {".gif", 4, "image/gif",   9},
  {".jpg", 4, "image/jpeg",   10},
  {".jpeg", 5, "image/jpeg",   10},
  {".png", 4, "image/png",   9},
  {".svg", 4, "image/svg+xml",  13},
  {".torrent", 8, "application/x-bittorrent", 24},
  {".wav", 4, "audio/x-wav",   11},
  {".mp3", 4, "audio/x-mp3",   11},
  {".mid", 4, "audio/mid",   9},
  {".m3u", 4, "audio/x-mpegurl",  15},
  {".ram", 4, "audio/x-pn-realaudio",  20},
  {".xml", 4, "text/xml",   8},
  {".xslt", 5, "application/xml",  15},
  {".ra",  3, "audio/x-pn-realaudio",  20},
  {".doc", 4, "application/msword",  19},
  {".exe", 4, "application/octet-stream", 24},
  {".zip", 4, "application/x-zip-compressed", 28},
  {".xls", 4, "application/excel",  17},
  {".tgz", 4, "application/x-tar-gz",  20},
  {".tar", 4, "application/x-tar",  17},
  {".gz",  3, "application/x-gunzip",  20},
  {".arj", 4, "application/x-arj-compressed", 28},
  {".rar", 4, "application/x-arj-compressed", 28},
  {".rtf", 4, "application/rtf",  15},
  {".pdf", 4, "application/pdf",  15},
  {".swf", 4, "application/x-shockwave-flash",29},
  {".mpg", 4, "video/mpeg",   10},
  {".mpeg", 5, "video/mpeg",   10},
  {".asf", 4, "video/x-ms-asf",  14},
  {".avi", 4, "video/x-msvideo",  15},
  {".bmp", 4, "image/bmp",   9},
  {NULL,  0, NULL,    0}
};

// Look at the "path" extension and figure what mime type it has.
// Store mime type in the vector.
static void get_mime_type(struct mg_context *ctx, const char *path,
                          struct vec *vec) {
  struct vec ext_vec, mime_vec;
  const char *list, *ext;
  size_t i, path_len;

  path_len = strlen(path);

  // Scan user-defined mime types first, in case user wants to
  // override default mime types.
  list = ctx->config->mime_types;
  while ((list = next_option(list, &ext_vec, &mime_vec)) != NULL) {
    // ext now points to the path suffix
    ext = path + path_len - ext_vec.len;
    if (mg_strncasecmp(ext, ext_vec.ptr, ext_vec.len) == 0) {
      *vec = mime_vec;
      return;
    }
  }

  // Now scan built-in mime types
  for (i = 0; builtin_mime_types[i].extension != NULL; i++) {
    ext = path + (path_len - builtin_mime_types[i].ext_len);
    if (path_len > builtin_mime_types[i].ext_len &&
        mg_strcasecmp(ext, builtin_mime_types[i].extension) == 0) {
      vec->ptr = builtin_mime_types[i].mime_type;
      vec->len = builtin_mime_types[i].mime_type_len;
      return;
    }
  }

  // Nothing found. Fall back to "text/plain"
  vec->ptr = "text/plain";
  vec->len = 10;
}

#ifndef HAVE_MD5
typedef struct MD5Context {
  uint32_t buf[4];
  uint32_t bits[2];
  unsigned char in[64];
} MD5_CTX;

#if __BYTE_ORDER == 1234
#define byteReverse(buf, len) // Do nothing
#else
static void byteReverse(unsigned char *buf, unsigned longs) {
  uint32_t t;
  do {
    t = (uint32_t) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
      ((unsigned) buf[1] << 8 | buf[0]);
    *(uint32_t *) buf = t;
    buf += 4;
  } while (--longs);
}
#endif

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

  ((uint32_t *) ctx->in)[14] = ctx->bits[0];
  ((uint32_t *) ctx->in)[15] = ctx->bits[1];

  MD5Transform(ctx->buf, (uint32_t *) ctx->in);
  byteReverse((unsigned char *) ctx->buf, 4);
  memcpy(digest, ctx->buf, 16);
  memset((char *) ctx, 0, sizeof(ctx));
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

// Return stringified MD5 hash for list of vectors. Buffer must be 33 bytes.
void mg_md5(char *buf, ...) {
  unsigned char hash[16];
  const char *p;
  va_list ap;
  MD5_CTX ctx;

  MD5Init(&ctx);

  va_start(ap, buf);
  while ((p = va_arg(ap, const char *)) != NULL) {
    MD5Update(&ctx, (unsigned char *) p, (int) strlen(p));
  }
  va_end(ap);

  MD5Final(hash, &ctx);
  bin2str(buf, hash, sizeof(hash));
}

// Check the user's password, return 1 if OK
static bool_t check_password(const char *method, const char *ha1,
                             const char *uri, const char *nonce, const char *nc,
                             const char *cnonce, const char *qop,
                             const char *response) {
  char ha2[32 + 1], expected_response[32 + 1];

  // NOTE(lsm): due to a bug in MSIE, we do not compare the URI
  // TODO(lsm): check for authentication timeout
  if (// strcmp(dig->uri, c->ouri) != 0 ||
      strlen(response) != 32
      // || now - strtoul(dig->nonce, NULL, 10) > 3600
      ) {
    return MG_FALSE;
  }

  mg_md5(ha2, method, ":", uri, NULL);
  mg_md5(expected_response, ha1, ":", nonce, ":", nc,
      ":", cnonce, ":", qop, ":", ha2, NULL);

  return mg_strcasecmp(response, expected_response) == 0;
}

// Use the global passwords file, if specified by auth_gpass option,
// or search for .htpasswd in the requested directory.
static FILE *open_auth_file(struct mg_connection *conn, const char *path) {
  struct mg_context *ctx = conn->ctx;
  char name[FILENAME_MAX];
  const char *p, *e;
  struct mgstat st;
  FILE *fp;

  if (ctx->config->global_passwords_file != NULL) {
    // Use global passwords file
    fp =  mg_fopen(ctx->config->global_passwords_file, "r");
    if (fp == NULL)
      cry(fc(ctx), "fopen(%s): %s",
          ctx->config->global_passwords_file, strerror(ERRNO));
  } else if (!mg_stat(path, &st) && st.is_directory) {
    (void) mg_snprintf(conn, name, sizeof(name), "%s%c%s",
        path, DIRSEP, PASSWORDS_FILE_NAME);
    fp = mg_fopen(name, "r");
  } else {
     // Try to find .htpasswd in requested directory.
    for (p = path, e = p + strlen(p) - 1; e > p; e--)
      if (IS_DIRSEP_CHAR(*e))
        break;
    (void) mg_snprintf(conn, name, sizeof(name), "%.*s%c%s",
        (int) (e - p), p, DIRSEP, PASSWORDS_FILE_NAME);
    fp = mg_fopen(name, "r");
  }

  return fp;
}

// Parsed Authorization header
struct ah {
  char *user, *uri, *cnonce, *response, *qop, *nc, *nonce;
};

static bool_t parse_auth_header(struct mg_connection *conn, char *buf,
                                size_t buf_size, struct ah *ah) {
  char *name, *value, *s;
  const char *auth_header;

  if ((auth_header = mg_get_header(conn, "Authorization")) == NULL ||
      mg_strncasecmp(auth_header, "Digest ", 7) != 0) {
    return MG_FALSE;
  }

  // Make modifiable copy of the auth header
  (void) mg_strlcpy(buf, auth_header + 7, buf_size);

  s = buf;
  (void) memset(ah, 0, sizeof(*ah));

  // Gobble initial spaces
  while (isspace(* (unsigned char *) s)) {
    s++;
  }

  // Parse authorization header
  for (;;) {
    name = skip(&s, "=");
    value = skip(&s, " ");

    // Handle commas: Digest username="a", realm="b", ...
    if (value[strlen(value) - 1] == ',') {
      value[strlen(value) - 1] = '\0';
    }

    // Trim double quotes around values
    if (*value == '"') {
      value++;
      value[strlen(value) - 1] = '\0';
    } else if (*value == '\0') {
      break;
    }

    if (!strcmp(name, "username")) {
      ah->user = value;
    } else if (!strcmp(name, "cnonce")) {
      ah->cnonce = value;
    } else if (!strcmp(name, "response")) {
      ah->response = value;
    } else if (!strcmp(name, "uri")) {
      ah->uri = value;
    } else if (!strcmp(name, "qop")) {
      ah->qop = value;
    } else if (!strcmp(name, "nc")) {
      ah->nc = value;
    } else if (!strcmp(name, "nonce")) {
      ah->nonce = value;
    }
  }

  // CGI needs it as REMOTE_USER
  if (ah->user != NULL) {
    conn->request_info.remote_user = mg_strdup(ah->user);
  }

  return MG_TRUE;
}

// Authorize against the opened passwords file. Return 1 if authorized.
static bool_t authorize(struct mg_connection *conn, FILE *fp) {
  struct ah ah;
  char line[256], f_user[256], ha1[256], f_domain[256], buf[MAX_REQUEST_SIZE];

  if (!parse_auth_header(conn, buf, sizeof(buf), &ah)) {
    return MG_FALSE;
  }

  // Loop over passwords file
  while (fgets(line, sizeof(line), fp) != NULL) {
    if (sscanf(line, "%[^:]:%[^:]:%s", f_user, f_domain, ha1) != 3) {
      continue;
    }

    if (!strcmp(ah.user, f_user) &&
        !strcmp(conn->ctx->config->auth_domain, f_domain))
      return check_password(
            conn->request_info.request_method,
            ha1, ah.uri, ah.nonce, ah.nc, ah.cnonce, ah.qop,
            ah.response);
  }

  return MG_FALSE;
}

// Return MG_TRUE if request is authorised, MG_FALSE otherwise.
static bool_t check_authorization(struct mg_connection *conn,
                                  const char *path) {
  FILE *fp;
  char fname[FILENAME_MAX];
  struct vec uri_vec, filename_vec;
  const char *list;
  bool_t authorized;

  fp = NULL;
  authorized = MG_TRUE;

  list = conn->ctx->config->protect;
  while ((list = next_option(list, &uri_vec, &filename_vec)) != NULL) {
    if (!memcmp(conn->request_info.uri, uri_vec.ptr, uri_vec.len)) {
      (void) mg_snprintf(conn, fname, sizeof(fname), "%.*s",
          filename_vec.len, filename_vec.ptr);
      if ((fp = mg_fopen(fname, "r")) == NULL) {
        cry(conn, "%s: cannot open %s: %s", __func__, fname, strerror(errno));
      }
      break;
    }
  }

  if (fp == NULL) {
    fp = open_auth_file(conn, path);
  }

  if (fp != NULL) {
    authorized = authorize(conn, fp);
    (void) fclose(fp);
  }

  return authorized;
}

static void send_authorization_request(struct mg_connection *conn) {
  conn->request_info.status_code = 401;
  (void) mg_printf(conn,
      "HTTP/1.1 401 Unauthorized\r\n"
      "WWW-Authenticate: Digest qop=\"auth\", "
      "realm=\"%s\", nonce=\"%lu\"\r\n\r\n",
      conn->ctx->config->auth_domain,
      (unsigned long) time(NULL));
}

static bool_t is_authorized_for_put(struct mg_connection *conn) {
  FILE *fp;
  int ret = MG_FALSE;

  fp = conn->ctx->config->put_delete_passwords_file == NULL ? NULL :
    mg_fopen(conn->ctx->config->put_delete_passwords_file, "r");


  if (fp != NULL) {
    ret = authorize(conn, fp);
    (void) fclose(fp);
  }

  return ret;
}

enum mg_error_t mg_modify_passwords_file(struct mg_context *ctx,
                                         const char *fname, const char *user,
                                         const char *pass) {
  int found;
  char line[512], u[512], d[512], ha1[33], tmp[FILENAME_MAX];
  const char *domain;
  FILE *fp, *fp2;

  found = 0;
  fp = fp2 = NULL;
  domain = ctx->config->auth_domain;

  // Regard empty password as no password - remove user record.
  if (pass[0] == '\0') {
    pass = NULL;
  }

  (void) snprintf(tmp, sizeof(tmp), "%s.tmp", fname);

  // Create the file if does not exist
  if ((fp = mg_fopen(fname, "a+")) != NULL) {
    (void) fclose(fp);
  }

  // Open the given file and temporary file
  if ((fp = mg_fopen(fname, "r")) == NULL) {
    cry(fc(ctx), "Cannot open %s: %s", fname, strerror(errno));
    return MG_ERROR;
  } else if ((fp2 = mg_fopen(tmp, "w+")) == NULL) {
    cry(fc(ctx), "Cannot open %s: %s", tmp, strerror(errno));
    return MG_ERROR;
  }

  // Copy the stuff to temporary file
  while (fgets(line, sizeof(line), fp) != NULL) {
    if (sscanf(line, "%[^:]:%[^:]:%*s", u, d) != 2) {
      continue;
    }

    if (!strcmp(u, user) && !strcmp(d, domain)) {
      found++;
      if (pass != NULL) {
        mg_md5(ha1, user, ":", domain, ":", pass, NULL);
        fprintf(fp2, "%s:%s:%s\n", user, domain, ha1);
      }
    } else {
      (void) fprintf(fp2, "%s", line);
    }
  }

  // If new user, just add it
  if (!found && pass != NULL) {
    mg_md5(ha1, user, ":", domain, ":", pass, NULL);
    (void) fprintf(fp2, "%s:%s:%s\n", user, domain, ha1);
  }

  // Close files
  (void) fclose(fp);
  (void) fclose(fp2);

  // Put the temp file in place of real file
  (void) mg_remove(fname);
  (void) mg_rename(tmp, fname);

  return MG_SUCCESS;
}

struct de {
  struct mg_connection *conn;
  char *file_name;
  struct mgstat st;
};

static void url_encode(const char *src, char *dst, size_t dst_len) {
  const char *dont_escape = "._-$,;~()";
  const char *hex = "0123456789abcdef";
  const char *end = dst + dst_len - 1;

  for (; *src != '\0' && dst < end; src++, dst++) {
    if (isalnum(*(unsigned char *) src) ||
        strchr(dont_escape, * (unsigned char *) src) != NULL) {
      *dst = *src;
    } else if (dst + 2 < end) {
      dst[0] = '%';
      dst[1] = hex[(* (unsigned char *) src) >> 4];
      dst[2] = hex[(* (unsigned char *) src) & 0xf];
      dst += 2;
    }
  }

  *dst = '\0';
}

static void print_dir_entry(struct de *de) {
  char size[64], mod[64], href[FILENAME_MAX];

  if (de->st.is_directory) {
    (void) mg_snprintf(de->conn, size, sizeof(size), "%s", "[DIRECTORY]");
  } else {
     // We use (signed) cast below because MSVC 6 compiler cannot
     // convert unsigned __int64 to double. Sigh.
    if (de->st.size < 1024) {
      (void) mg_snprintf(de->conn, size, sizeof(size),
          "%lu", (unsigned long) de->st.size);
    } else if (de->st.size < 1024 * 1024) {
      (void) mg_snprintf(de->conn, size, sizeof(size),
          "%.1fk", (double) de->st.size / 1024.0);
    } else if (de->st.size < 1024 * 1024 * 1024) {
      (void) mg_snprintf(de->conn, size, sizeof(size),
          "%.1fM", (double) de->st.size / 1048576);
    } else {
      (void) mg_snprintf(de->conn, size, sizeof(size),
          "%.1fG", (double) de->st.size / 1073741824);
    }
  }
  (void) strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M", localtime(&de->st.mtime));
  url_encode(de->file_name, href, sizeof(href));
  de->conn->num_bytes_sent += mg_printf(de->conn,
      "<tr><td><a href=\"%s%s%s\">%s%s</a></td>"
      "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
      de->conn->request_info.uri, href, de->st.is_directory ? "/" : "",
      de->file_name, de->st.is_directory ? "/" : "", mod, size);
}

// This function is called from send_directory() and used for
// sorting directory entries by size, or name, or modification time.
static int compare_dir_entries(const void *p1, const void *p2) {
  const struct de *a = (struct de *) p1, *b = (struct de *) p2;
  const char *query_string = a->conn->request_info.query_string;
  int cmp_result = 0;

  if (query_string == NULL) {
    query_string = "na";
  }

  if (a->st.is_directory && !b->st.is_directory) {
    return -1;  // Always put directories on top
  } else if (!a->st.is_directory && b->st.is_directory) {
    return 1;   // Always put directories on top
  } else if (*query_string == 'n') {
    cmp_result = strcmp(a->file_name, b->file_name);
  } else if (*query_string == 's') {
    cmp_result = a->st.size == b->st.size ? 0 :
      a->st.size > b->st.size ? 1 : -1;
  } else if (*query_string == 'd') {
    cmp_result = a->st.mtime == b->st.mtime ? 0 :
      a->st.mtime > b->st.mtime ? 1 : -1;
  }

  return query_string[1] == 'd' ? -cmp_result : cmp_result;
}

static void handle_directory_request(struct mg_connection *conn,
                                     const char *dir) {
  struct dirent *dp;
  DIR *dirp;
  struct de *entries = NULL;
  char path[FILENAME_MAX];
  int i, sort_direction, num_entries = 0, arr_size = 128;

  if ((dirp = opendir(dir)) == NULL) {
    send_http_error(conn, 500, "Cannot open directory",
        "Error: opendir(%s): %s", path, strerror(ERRNO));
    return;
  }

  (void) mg_printf(conn, "%s",
      "HTTP/1.1 200 OK\r\n"
      "Connection: close\r\n"
      "Content-Type: text/html; charset=utf-8\r\n\r\n");

  sort_direction = conn->request_info.query_string != NULL &&
    conn->request_info.query_string[1] == 'd' ? 'a' : 'd';

  while ((dp = readdir(dirp)) != NULL) {

    // Do not show current dir and passwords file
    if (!strcmp(dp->d_name, ".") ||
        !strcmp(dp->d_name, "..") ||
        !strcmp(dp->d_name, PASSWORDS_FILE_NAME))
      continue;

    if (entries == NULL || num_entries >= arr_size) {
      arr_size *= 2;
      entries = (struct de *) realloc(entries,
          arr_size * sizeof(entries[0]));
    }

    if (entries == NULL) {
      send_http_error(conn, 500, "Cannot open directory",
          "%s", "Error: cannot allocate memory");
      return;
    }

    mg_snprintf(conn, path, sizeof(path), "%s%c%s", dir, DIRSEP, dp->d_name);

    // If we don't memset stat structure to zero, mtime will have
    // garbage and strftime() will segfault later on in
    // print_dir_entry(). memset is required only if mg_stat()
    // fails. For more details, see
    // http://code.google.com/p/mongoose/issues/detail?id=79
    if (mg_stat(path, &entries[num_entries].st) != 0) {
      memset(&entries[num_entries].st, 0, sizeof(entries[num_entries].st));
    }

    entries[num_entries].conn = conn;
    entries[num_entries].file_name = mg_strdup(dp->d_name);
    num_entries++;
  }
  (void) closedir(dirp);

  conn->num_bytes_sent += mg_printf(conn,
      "<html><head><title>Index of %s</title>"
      "<style>th {text-align: left;}</style></head>"
      "<body><h1>Index of %s</h1><pre><table cellpadding=\"0\">"
      "<tr><th><a href=\"?n%c\">Name</a></th>"
      "<th><a href=\"?d%c\">Modified</a></th>"
      "<th><a href=\"?s%c\">Size</a></th></tr>"
      "<tr><td colspan=\"3\"><hr></td></tr>",
      conn->request_info.uri, conn->request_info.uri,
      sort_direction, sort_direction, sort_direction);

  // Print first entry - link to a parent directory
  conn->num_bytes_sent += mg_printf(conn,
      "<tr><td><a href=\"%s%s\">%s</a></td>"
      "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
      conn->request_info.uri, "..", "Parent directory", "-", "-");

  // Sort and print directory entries
  qsort(entries, num_entries, sizeof(entries[0]), compare_dir_entries);
  for (i = 0; i < num_entries; i++) {
    print_dir_entry(&entries[i]);
    free(entries[i].file_name);
  }
  free(entries);

  conn->num_bytes_sent += mg_printf(conn, "%s", "</table></body></html>");
  conn->request_info.status_code = 200;
}

// Send len bytes from the opened file to the client.
static void send_file_data(struct mg_connection *conn, FILE *fp, int64_t len) {
  char buf[BUFSIZ];
  int to_read, num_read, num_written;

  while (len > 0) {
    // Calculate how much to read from the file in the buffer
    to_read = sizeof(buf);
    if ((int64_t) to_read > len)
      to_read = (int) len;

    // Read from file, exit the loop on error
    if ((num_read = fread(buf, 1, to_read, fp)) == 0)
      break;

    // Send read bytes to the client, exit the loop on error
    if ((num_written = mg_write(conn, buf, num_read)) != num_read)
      break;

    // Both read and were successful, adjust counters
    conn->num_bytes_sent += num_written;
    len -= num_written;
  }
}

static int parse_range_header(const char *header, int64_t *a, int64_t *b) {
  return sscanf(header, "bytes=%" INT64_FMT "-%" INT64_FMT, a, b);
}

static void handle_file_request(struct mg_connection *conn, const char *path,
                                struct mgstat *stp) {
  char date[64], lm[64], etag[64], range[64];
  const char *fmt = "%a, %d %b %Y %H:%M:%S %Z", *msg = "OK", *hdr;
  time_t curtime = time(NULL);
  int64_t cl, r1, r2;
  struct vec mime_vec;
  FILE *fp;
  int n;

  get_mime_type(conn->ctx, path, &mime_vec);
  cl = stp->size;
  conn->request_info.status_code = 200;
  range[0] = '\0';

  if ((fp = mg_fopen(path, "rb")) == NULL) {
    send_http_error(conn, 500, http_500_error,
        "fopen(%s): %s", path, strerror(ERRNO));
    return;
  }
  set_close_on_exec(fileno(fp));

  // If Range: header specified, act accordingly
  r1 = r2 = 0;
  hdr = mg_get_header(conn, "Range");
  if (hdr != NULL && (n = parse_range_header(hdr, &r1, &r2)) > 0) {
    conn->request_info.status_code = 206;
    (void) fseeko(fp, (off_t) r1, SEEK_SET);
    cl = n == 2 ? r2 - r1 + 1: cl - r1;
    (void) mg_snprintf(conn, range, sizeof(range),
        "Content-Range: bytes "
        "%" INT64_FMT "-%"
        INT64_FMT "/%" INT64_FMT "\r\n",
        r1, r1 + cl - 1, stp->size);
    msg = "Partial Content";
  }

  // Prepare Etag, Date, Last-Modified headers
  (void) strftime(date, sizeof(date), fmt, localtime(&curtime));
  (void) strftime(lm, sizeof(lm), fmt, localtime(&stp->mtime));
  (void) mg_snprintf(conn, etag, sizeof(etag), "%lx.%lx",
      (unsigned long) stp->mtime, (unsigned long) stp->size);

  (void) mg_printf(conn,
      "HTTP/1.1 %d %s\r\n"
      "Date: %s\r\n"
      "Last-Modified: %s\r\n"
      "Etag: \"%s\"\r\n"
      "Content-Type: %.*s\r\n"
      "Content-Length: %" INT64_FMT "\r\n"
      "Connection: close\r\n"
      "Accept-Ranges: bytes\r\n"
      "%s\r\n",
      conn->request_info.status_code, msg, date, lm, etag,
      mime_vec.len, mime_vec.ptr, cl, range);

  if (strcmp(conn->request_info.request_method, "HEAD") != 0)
    send_file_data(conn, fp, cl);
  (void) fclose(fp);
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

static bool_t is_valid_http_method(const char *method) {
  return !strcmp(method, "GET") ||
      !strcmp(method, "POST") ||
      !strcmp(method, "HEAD") ||
      !strcmp(method, "PUT") ||
      !strcmp(method, "DELETE");
}

// Parse HTTP request, fill in mg_request_info structure.
static bool_t parse_http_request(char *buf, struct mg_request_info *ri) {
  int status = MG_FALSE;

  ri->request_method = skip(&buf, " ");
  ri->uri = skip(&buf, " ");
  ri->http_version = skip(&buf, "\r\n");

  if (is_valid_http_method(ri->request_method) &&
      ri->uri[0] == '/' &&
      strncmp(ri->http_version, "HTTP/", 5) == 0) {
    ri->http_version += 5;   /* Skip "HTTP/" */
    parse_http_headers(&buf, ri);
    status = MG_TRUE;
  }

  return status;
}

// Keep reading the input (either opened file descriptor fd, or socket sock,
// or SSL descriptor ssl) into buffer buf, until \r\n\r\n appears in the
// buffer (which marks the end of HTTP request). Buffer buf may already
// have some data. The length of the data is stored in nread.
// Upon every read operation, increase nread by the number of bytes read.
static int read_request(FILE *fp, SOCKET sock, SSL *ssl, char *buf, int bufsiz,
                        int *nread) {
  int n, request_len;

  request_len = 0;
  while (*nread < bufsiz && request_len == 0) {
    n = pull(fp, sock, ssl, buf + *nread, bufsiz - *nread);
    if (n <= 0) {
      break;
    } else {
      *nread += n;
      request_len = get_request_len(buf, *nread);
    }
  }

  return request_len;
}

// For given directory path, substitute it to valid index file.
// Return 0 if index file has been found, -1 if not found.
// If the file is found, it's stats is returned in stp.
static bool_t substitute_index_file(struct mg_connection *conn, char *path,
                                    size_t path_len, struct mgstat *stp) {
  const char *list;
  struct mgstat st;
  struct vec filename_vec;
  size_t n;
  bool_t found;

  n = strlen(path);

  // The 'path' given to us points to the directory. Remove all trailing
  // directory separator characters from the end of the path, and
  // then append single directory separator character.
  while (n > 0 && IS_DIRSEP_CHAR(path[n - 1])) {
    n--;
  }
  path[n] = DIRSEP;

  // Traverse index files list. For each entry, append it to the given
  // path and see if the file exists. If it exists, break the loop
  list = conn->ctx->config->index_files;
  found = MG_FALSE;

  while ((list = next_option(list, &filename_vec, NULL)) != NULL) {

    // Ignore too long entries that may overflow path buffer
    if (filename_vec.len > path_len - n)
      continue;

    // Prepare full path to the index file
    (void) mg_strlcpy(path + n + 1, filename_vec.ptr, filename_vec.len + 1);

    // Does it exist?
    if (mg_stat(path, &st) == 0) {
      // Yes it does, break the loop
      *stp = st;
      found = MG_TRUE;
      break;
    }
  }

  // If no index file exists, restore directory path
  if (found == MG_FALSE) {
    path[n] = '\0';
  }

  return found;
}

// Return True if we should reply 304 Not Modified.
static bool_t is_not_modified(const struct mg_connection *conn,
                              const struct mgstat *stp) {
  const char *ims = mg_get_header(conn, "If-Modified-Since");
  return ims != NULL && stp->mtime <= parse_date_string(ims);
}

static bool_t handle_request_body(struct mg_connection *conn, FILE *fp) {
  const char *expect, *data;
  char buf[BUFSIZ];
  int to_read, nread, data_len;
  bool_t status = MG_FALSE;

  expect = mg_get_header(conn, "Expect");

  if (conn->content_len == -1) {
    send_http_error(conn, 411, "Length Required", "");
  } else if (expect != NULL && mg_strcasecmp(expect, "100-continue")) {
    send_http_error(conn, 417, "Expectation Failed", "");
  } else {
    if (expect != NULL) {
      (void) mg_printf(conn, "%s", "HTTP/1.1 100 Continue\r\n\r\n");
    }
    data = conn->buf + conn->request_len;
    data_len = conn->data_len - conn->request_len;
    assert(data_len >= 0);

    if (conn->content_len <= (int64_t) data_len) {
#if 0
      ri->post_data_len = (int) content_len;
#endif
      // If fp is NULL, this is embedded mode, and we do not
      // have to do anything: POST data is already there,
      // no need to allocate a buffer and copy it in.
      // If fp != NULL, we need to write the data.
      status = fp == NULL || (push(fp, INVALID_SOCKET, NULL, data,
            conn->content_len) == conn->content_len) ?  MG_TRUE : MG_FALSE;
    } else {
      push(fp, INVALID_SOCKET, NULL, data, (int64_t) data_len);
      conn->consumed_content += data_len;

      while (conn->consumed_content < conn->content_len) {
        to_read = sizeof(buf);
        if ((int64_t) to_read > conn->content_len - conn->consumed_content) {
          to_read = (int) (conn->content_len - conn->consumed_content);
        }
        nread = pull(NULL, conn->client.sock, conn->ssl, buf, to_read);
        if (nread <= 0 || push(fp, INVALID_SOCKET, NULL, buf, nread) != nread) {
          break;
        }
        conn->consumed_content += nread;
      }
      if (conn->consumed_content == conn->content_len) {
        status = MG_TRUE;
      }
    }

    // Each error code path in this function must send an error
    if (status != MG_TRUE) {
      send_http_error(conn, 577, http_500_error,
          "%s", "Error handling body data");
    }
  }

  return status;
}

#if !defined(NO_CGI)
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
  char buf[CGI_ENVIRONMENT_SIZE]; // Environment buffer
  int len; // Space taken
  char *vars[MAX_CGI_ENVIR_VARS]; // char **envp
  int nvars; // Number of variables
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
  n = mg_vsnprintf(block->conn, added, (size_t) space, fmt, ap);
  va_end(ap);

  // Make sure we do not overflow buffer and the envp array
  if (n > 0 && n < space &&
      block->nvars < (int) ARRAY_SIZE(block->vars) - 2) {
    // Append a pointer to the added string into the envp array
    block->vars[block->nvars++] = block->buf + block->len;
    // Bump up used length counter. Include \0 terminator
    block->len += n + 1;
  }

  return added;
}

static void prepare_cgi_environment(struct mg_connection *conn,
                                    const char *prog,
                                    struct cgi_env_block *blk) {
  const char *s, *script_filename, *slash;
  struct vec var_vec, root;
  char *p;
  int  i;

  blk->len = blk->nvars = 0;
  blk->conn = conn;

  // SCRIPT_FILENAME
  script_filename = prog;
  if ((s = strrchr(prog, '/')) != NULL)
    script_filename = s + 1;

  get_document_root(conn, &root);

  addenv(blk, "SERVER_NAME=%s", conn->ctx->config->auth_domain);
  addenv(blk, "SERVER_ROOT=%.*s", root.len, root.ptr);
  addenv(blk, "DOCUMENT_ROOT=%.*s", root.len, root.ptr);

  // Prepare the environment block
  addenv(blk, "%s", "GATEWAY_INTERFACE=CGI/1.1");
  addenv(blk, "%s", "SERVER_PROTOCOL=HTTP/1.1");
  addenv(blk, "%s", "REDIRECT_STATUS=200"); // For PHP
  addenv(blk, "SERVER_PORT=%d", ntohs(conn->client.lsa.u.sin.sin_port));
  addenv(blk, "REQUEST_METHOD=%s", conn->request_info.request_method);
  addenv(blk, "REMOTE_ADDR=%s",
      inet_ntoa(conn->client.rsa.u.sin.sin_addr));
  addenv(blk, "REMOTE_PORT=%d", conn->request_info.remote_port);
  addenv(blk, "REQUEST_URI=%s", conn->request_info.uri);

  slash = strrchr(conn->request_info.uri, '/');
  addenv(blk, "SCRIPT_NAME=%.*s%s",
      (slash - conn->request_info.uri) + 1, conn->request_info.uri,
      script_filename);

  addenv(blk, "SCRIPT_FILENAME=%s", script_filename);
  addenv(blk, "PATH_TRANSLATED=%s", prog);
  addenv(blk, "HTTPS=%s", conn->ssl == NULL ? "off" : "on");

  if ((s = mg_get_header(conn, "Content-Type")) != NULL)
    addenv(blk, "CONTENT_TYPE=%s", s);

  if (conn->request_info.query_string != NULL)
    addenv(blk, "QUERY_STRING=%s", conn->request_info.query_string);

  if ((s = mg_get_header(conn, "Content-Length")) != NULL)
    addenv(blk, "CONTENT_LENGTH=%s", s);

  if ((s = getenv("PATH")) != NULL)
    addenv(blk, "PATH=%s", s);

#if defined(_WIN32)
  if ((s = getenv("COMSPEC")) != NULL)
    addenv(blk, "COMSPEC=%s", s);
  if ((s = getenv("SYSTEMROOT")) != NULL)
    addenv(blk, "SYSTEMROOT=%s", s);
#else
  if ((s = getenv("LD_LIBRARY_PATH")) != NULL)
    addenv(blk, "LD_LIBRARY_PATH=%s", s);
#endif /* _WIN32 */

  if ((s = getenv("PERLLIB")) != NULL)
    addenv(blk, "PERLLIB=%s", s);

  if (conn->request_info.remote_user != NULL) {
    addenv(blk, "REMOTE_USER=%s", conn->request_info.remote_user);
    addenv(blk, "%s", "AUTH_TYPE=Digest");
  }

  // Add all headers as HTTP_* variables
  for (i = 0; i < conn->request_info.num_headers; i++) {
    p = addenv(blk, "HTTP_%s=%s",
        conn->request_info.http_headers[i].name,
        conn->request_info.http_headers[i].value);

    // Convert variable name into uppercase, and change - to _
    for (; *p != '=' && *p != '\0'; p++) {
      if (*p == '-')
        *p = '_';
      *p = (char) toupper(* (unsigned char *) p);
    }
  }

  // Add user-specified variables
  s = conn->ctx->config->cgi_environment;
  while ((s = next_option(s, &var_vec, NULL)) != NULL) {
    addenv(blk, "%.*s", var_vec.len, var_vec.ptr);
  }

  blk->vars[blk->nvars++] = NULL;
  blk->buf[blk->len++] = '\0';

  assert(blk->nvars < (int) ARRAY_SIZE(blk->vars));
  assert(blk->len > 0);
  assert(blk->len < (int) sizeof(blk->buf));
}

static void handle_cgi_request(struct mg_connection *conn, const char *prog) {
  int headers_len, data_len, i, fd_stdin[2], fd_stdout[2];
  const char *status;
  char buf[MAX_REQUEST_SIZE], *pbuf, dir[FILENAME_MAX], *p;
  struct mg_request_info ri;
  struct cgi_env_block blk;
  FILE *in, *out;
  pid_t pid;

  prepare_cgi_environment(conn, prog, &blk);

  // CGI must be executed in its own directory
  (void) mg_snprintf(conn, dir, sizeof(dir), "%s", prog);
  if ((p = strrchr(dir, DIRSEP)) != NULL) {
    *p++ = '\0';
  }

  pid = (pid_t) -1;
  fd_stdin[0] = fd_stdin[1] = fd_stdout[0] = fd_stdout[1] = -1;
  in = out = NULL;

  if (pipe(fd_stdin) != 0 || pipe(fd_stdout) != 0) {
    send_http_error(conn, 500, http_500_error,
        "Cannot create CGI pipe: %s", strerror(ERRNO));
    goto done;
  } else if ((pid = spawn_process(conn, p, blk.buf, blk.vars,
          fd_stdin[0], fd_stdout[1], dir)) == (pid_t) -1) {
    goto done;
  } else if ((in = fdopen(fd_stdin[1], "wb")) == NULL ||
      (out = fdopen(fd_stdout[0], "rb")) == NULL) {
    send_http_error(conn, 500, http_500_error,
        "fopen: %s", strerror(ERRNO));
    goto done;
  }

  setbuf(in, NULL);
  setbuf(out, NULL);

  // spawn_process() must close those!
  // If we don't mark them as closed, close() attempt before
  // return from this function throws an exception on Windows.
  // Windows does not like when closed descriptor is closed again.
  fd_stdin[0] = fd_stdout[1] = -1;

  // Send POST data to the CGI process if needed
  if (!strcmp(conn->request_info.request_method, "POST") &&
      !handle_request_body(conn, in)) {
    goto done;
  }

  // Now read CGI reply into a buffer. We need to set correct
  // status code, thus we need to see all HTTP headers first.
  // Do not send anything back to client, until we buffer in all
  // HTTP headers.
  data_len = 0;
  headers_len = read_request(out, INVALID_SOCKET, NULL,
      buf, sizeof(buf), &data_len);
  if (headers_len <= 0) {
    send_http_error(conn, 500, http_500_error,
        "CGI program sent malformed HTTP headers: [%.*s]",
        data_len, buf);
    goto done;
  }
  pbuf = buf;
  buf[headers_len - 1] = '\0';
  parse_http_headers(&pbuf, &ri);

  // Make up and send the status line
  status = get_header(&ri, "Status");
  conn->request_info.status_code = status == NULL ? 200 : atoi(status);
  (void) mg_printf(conn, "HTTP/1.1 %d OK\r\n", conn->request_info.status_code);

  // Send headers
  for (i = 0; i < ri.num_headers; i++) {
    mg_printf(conn, "%s: %s\r\n",
              ri.http_headers[i].name, ri.http_headers[i].value);
  }
  (void) mg_write(conn, "\r\n", 2);

  // Send chunk of data that may be read after the headers
  conn->num_bytes_sent += mg_write(conn, buf + headers_len,
                                   data_len - headers_len);

  // Read the rest of CGI output and send to the client
  send_file_data(conn, out, INT64_MAX);

done:
  if (pid != (pid_t) -1) {
    kill(pid, SIGKILL);
  }
  if (fd_stdin[0] != -1) {
    (void) close(fd_stdin[0]);
  }
  if (fd_stdout[1] != -1) {
    (void) close(fd_stdout[1]);
  }

  if (in != NULL) {
    (void) fclose(in);
  } else if (fd_stdin[1] != -1) {
    (void) close(fd_stdin[1]);
  }

  if (out != NULL) {
    (void) fclose(out);
  } else if (fd_stdout[0] != -1) {
    (void) close(fd_stdout[0]);
  }
}
#endif // !NO_CGI

// For a given PUT path, create all intermediate subdirectories
// for given path. Return 0 if the path itself is a directory,
// or -1 on error, 1 if OK.
static int put_dir(const char *path) {
  char buf[FILENAME_MAX];
  const char *s, *p;
  struct mgstat st;
  size_t len;

  for (s = p = path + 2; (p = strchr(s, '/')) != NULL; s = ++p) {
    len = p - path;
    assert(len < sizeof(buf));
    (void) memcpy(buf, path, len);
    buf[len] = '\0';

    // Try to create intermediate directory
    if (mg_stat(buf, &st) == -1 && mg_mkdir(buf, 0755) != 0) {
      return -1;
    }

    // Is path itself a directory?
    if (p[1] == '\0') {
      return 0;
    }
  }

  return 1;
}

static void put_file(struct mg_connection *conn, const char *path) {
  struct mgstat st;
  const char *range;
  int64_t r1, r2;
  FILE *fp;
  int rc;

  conn->request_info.status_code = mg_stat(path, &st) == 0 ? 200 : 201;

  if ((rc = put_dir(path)) == 0) {
    mg_printf(conn, "HTTP/1.1 %d OK\r\n\r\n", conn->request_info.status_code);
  } else if (rc == -1) {
    send_http_error(conn, 500, http_500_error,
        "put_dir(%s): %s", path, strerror(ERRNO));
  } else if ((fp = mg_fopen(path, "wb+")) == NULL) {
    send_http_error(conn, 500, http_500_error,
        "fopen(%s): %s", path, strerror(ERRNO));
  } else {
    set_close_on_exec(fileno(fp));
    range = mg_get_header(conn, "Content-Range");
    r1 = r2 = 0;
    if (range != NULL && parse_range_header(range, &r1, &r2) > 0) {
      conn->request_info.status_code = 206;
      // TODO(lsm): handle seek error
      (void) fseeko(fp, (off_t) r1, SEEK_SET);
    }
    if (handle_request_body(conn, fp))
      (void) mg_printf(conn, "HTTP/1.1 %d OK\r\n\r\n",
          conn->request_info.status_code);
    (void) fclose(fp);
  }
}

static void send_ssi_file(struct mg_connection *, const char *, FILE *, int);

static void do_ssi_include(struct mg_connection *conn, const char *ssi,
                           char *tag, int include_level) {
  char file_name[BUFSIZ], path[FILENAME_MAX], *p;
  struct vec root;
  bool_t is_ssi;
  FILE *fp;

  get_document_root(conn, &root);

  // sscanf() is safe here, since send_ssi_file() also uses buffer
  // of size BUFSIZ to get the tag. So strlen(tag) is always < BUFSIZ.
  if (sscanf(tag, " virtual=\"%[^\"]\"", file_name) == 1) {
    // File name is relative to the webserver root
    (void) mg_snprintf(conn, path, sizeof(path), "%.*s%c%s",
        root.len, root.ptr, DIRSEP, file_name);
  } else if (sscanf(tag, " file=\"%[^\"]\"", file_name) == 1) {
    // File name is relative to the webserver working directory
    // or it is absolute system path
    (void) mg_snprintf(conn, path, sizeof(path), "%s", file_name);
  } else if (sscanf(tag, " \"%[^\"]\"", file_name) == 1) {
    // File name is relative to the currect document
    (void) mg_snprintf(conn, path, sizeof(path), "%s", ssi);
    if ((p = strrchr(path, DIRSEP)) != NULL) {
      p[1] = '\0';
    }
    (void) mg_snprintf(conn, path + strlen(path),
        sizeof(path) - strlen(path), "%s", file_name);
  } else {
    cry(conn, "Bad SSI #include: [%s]", tag);
    return;
  }

  if ((fp = mg_fopen(path, "rb")) == NULL) {
    cry(conn, "Cannot open SSI #include: [%s]: fopen(%s): %s",
        tag, path, strerror(ERRNO));
  } else {
    set_close_on_exec(fileno(fp));
    is_ssi = match_extension(path, conn->ctx->config->ssi_extensions);
    if (is_ssi) {
      send_ssi_file(conn, path, fp, include_level + 1);
    } else {
      send_file_data(conn, fp, INT64_MAX);
    }
    (void) fclose(fp);
  }
}

static void do_ssi_exec(struct mg_connection *conn, char *tag) {
  char cmd[BUFSIZ];
  FILE *fp;

  if (sscanf(tag, " \"%[^\"]\"", cmd) != 1) {
    cry(conn, "Bad SSI #exec: [%s]", tag);
  } else if ((fp = popen(cmd, "r")) == NULL) {
    cry(conn, "Cannot SSI #exec: [%s]: %s", cmd, strerror(ERRNO));
  } else {
    send_file_data(conn, fp, INT64_MAX);
    (void) pclose(fp);
  }
}

static void send_ssi_file(struct mg_connection *conn, const char *path,
                          FILE *fp, int include_level) {
  char buf[BUFSIZ];
  int ch, len, in_ssi_tag;

  if (include_level > 10) {
    cry(conn, "SSI #include level is too deep (%s)", path);
    return;
  }

  in_ssi_tag = MG_FALSE;
  len = 0;

  while ((ch = fgetc(fp)) != EOF) {
    if (in_ssi_tag && ch == '>') {
      in_ssi_tag = MG_FALSE;
      buf[len++] = (char) ch;
      buf[len] = '\0';
      assert(len <= (int) sizeof(buf));
      if (len < 6 || memcmp(buf, "<!--#", 5) != 0) {
        // Not an SSI tag, pass it
        (void) mg_write(conn, buf, len);
      } else {
        if (!memcmp(buf + 5, "include", 7)) {
          do_ssi_include(conn, path, buf + 12, include_level);
        } else if (!memcmp(buf + 5, "exec", 4)) {
          do_ssi_exec(conn, buf + 9);
        } else {
          cry(conn, "%s: unknown SSI " "command: \"%s\"", path, buf);
        }
      }
      len = 0;
    } else if (in_ssi_tag) {
      if (len == 5 && memcmp(buf, "<!--#", 5) != 0) {
        // Not an SSI tag
        in_ssi_tag = MG_FALSE;
      } else if (len == (int) sizeof(buf) - 2) {
        cry(conn, "%s: SSI tag is too large", path);
        len = 0;
      }
      buf[len++] = ch & 0xff;
    } else if (ch == '<') {
      in_ssi_tag = MG_TRUE;
      if (len > 0) {
        (void) mg_write(conn, buf, len);
      }
      len = 0;
      buf[len++] = ch & 0xff;
    } else {
      buf[len++] = ch & 0xff;
      if (len == (int) sizeof(buf)) {
        (void) mg_write(conn, buf, len);
        len = 0;
      }
    }
  }

  // Send the rest of buffered data
  if (len > 0) {
    (void) mg_write(conn, buf, len);
  }
}

static void handle_ssi_file_request(struct mg_connection *conn,
                                    const char *path) {
  FILE *fp;

  if ((fp = mg_fopen(path, "rb")) == NULL) {
    send_http_error(conn, 500, http_500_error, "fopen(%s): %s", path,
                    strerror(ERRNO));
  } else {
    set_close_on_exec(fileno(fp));
    (void) mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\nConnection: close\r\n\r\n");
    send_ssi_file(conn, path, fp, 0);
    (void) fclose(fp);
  }
}

// This is the heart of the Mongoose's logic.
// This function is called when the request is read, parsed and validated,
// and Mongoose must decide what action to take: serve a file, or
// a directory, or call embedded function, etcetera.
static void handle_request(struct mg_connection *conn) {
  struct mg_request_info *ri;
  char path[FILENAME_MAX];
  int uri_len;
  struct mgstat st;
  mg_callback_t new_request_callback;

  ri = &conn->request_info;
  if ((conn->request_info.query_string = strchr(ri->uri, '?')) != NULL) {
    * conn->request_info.query_string++ = '\0';
  }
  uri_len = strlen(ri->uri);
  new_request_callback = conn->ctx->config->new_request_handler;
  (void) url_decode(ri->uri, uri_len, ri->uri, uri_len + 1, MG_FALSE);
  remove_double_dots_and_double_slashes(ri->uri);
  convert_uri_to_file_name(conn, ri->uri, path, sizeof(path));

  DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: %s", __func__, ri->uri));
  if (new_request_callback && new_request_callback(conn, ri) == MG_TRUE) {
    // Do nothing, callback has served the request
  } else if (!check_authorization(conn, path)) {
    send_authorization_request(conn);
  } else if (strstr(path, PASSWORDS_FILE_NAME)) {
    // Do not allow to view passwords files
    send_http_error(conn, 403, "Forbidden", "Access Forbidden");
  } else if (conn->ctx->config->document_root == NULL) {
    send_http_error(conn, 404, "Not Found", "Not Found");
  } else if ((!strcmp(ri->request_method, "PUT") ||
        !strcmp(ri->request_method, "DELETE")) &&
      (conn->ctx->config->put_delete_passwords_file == NULL ||
       !is_authorized_for_put(conn))) {
    send_authorization_request(conn);
  } else if (!strcmp(ri->request_method, "PUT")) {
    put_file(conn, path);
  } else if (!strcmp(ri->request_method, "DELETE")) {
    if (mg_remove(path) == 0) {
      send_http_error(conn, 200, "OK", "");
    } else {
      send_http_error(conn, 500, http_500_error, "remove(%s): %s", path,
                      strerror(ERRNO));
    }
  } else if (mg_stat(path, &st) != 0) {
    send_http_error(conn, 404, "Not Found", "%s", "File not found");
  } else if (st.is_directory && ri->uri[uri_len - 1] != '/') {
    (void) mg_printf(conn,
        "HTTP/1.1 301 Moved Permanently\r\n"
        "Location: %s/\r\n\r\n", ri->uri);
  } else if (st.is_directory &&
      substitute_index_file(conn, path, sizeof(path), &st) == MG_FALSE) {
    if (conn->ctx->config->enable_directory_listing) {
      handle_directory_request(conn, path);
    } else {
      send_http_error(conn, 403, "Directory Listing Denied",
          "Directory listing denied");
    }
#if !defined(NO_CGI)
  } else if (match_extension(path, conn->ctx->config->cgi_extensions)) {
    if (strcmp(ri->request_method, "POST") &&
        strcmp(ri->request_method, "GET")) {
      send_http_error(conn, 501, "Not Implemented",
          "Method %s is not implemented", ri->request_method);
    } else {
      handle_cgi_request(conn, path);
    }
#endif /* NO_CGI */
  } else if (match_extension(path, conn->ctx->config->ssi_extensions)) {
    handle_ssi_file_request(conn, path);
  } else if (is_not_modified(conn, &st)) {
    send_http_error(conn, 304, "Not Modified", "");
  } else {
    handle_file_request(conn, path, &st);
  }
}

static void close_all_listening_sockets(struct mg_context *ctx) {
  struct socket *sp, *tmp;
  for (sp = ctx->listening_sockets; sp != NULL; sp = tmp) {
    tmp = sp->next;
    (void) closesocket(sp->sock);
    free(sp);
  }
  ctx->listening_sockets = NULL;
}

static enum mg_error_t set_ports_option(struct mg_context *ctx) {
  SOCKET sock;
  int is_ssl;
  struct vec vec;
  struct socket *listener;
  const char *list = ctx->config->listening_ports;

  while ((list = next_option(list, &vec, NULL)) != NULL) {
    is_ssl = vec.ptr[vec.len - 1] == 's' ? MG_TRUE : MG_FALSE;

    if ((listener = calloc(1, sizeof(*listener))) == NULL) {
      cry(fc(ctx), "%s", "Too many listeninig sockets");
      return MG_ERROR;
    } else if ((sock = mg_open_listening_port(ctx,
            vec.ptr, &listener->lsa)) == INVALID_SOCKET) {
      cry(fc(ctx), "cannot bind to %.*s", vec.len, vec.ptr);
      return MG_ERROR;
    } else if (is_ssl == MG_TRUE && ctx->ssl_ctx == NULL) {
      (void) closesocket(sock);
      cry(fc(ctx), "cannot add SSL socket, please specify "
          "-ssl_cert option BEFORE -ports option");
      return MG_ERROR;
    } else {
      listener->sock = sock;
      listener->is_ssl = is_ssl;
      listener->next = ctx->listening_sockets;
      ctx->listening_sockets = listener;
    }
  }

  return MG_SUCCESS;
}

static void log_header(const struct mg_connection *conn, const char *header,
                       FILE *fp) {
  const char *header_value;

  if ((header_value = mg_get_header(conn, header)) == NULL) {
    (void) fprintf(fp, "%s", " -");
  } else {
    (void) fprintf(fp, " \"%s\"", header_value);
  }
}

static void log_access(const struct mg_connection *conn) {
  const struct mg_request_info *ri;
  FILE *fp;
  char date[64];

  fp = conn->ctx->config->access_log_file == NULL ?  NULL :
    mg_fopen(conn->ctx->config->access_log_file, "a+");

  if (fp == NULL)
    return;

  (void) strftime(date, sizeof(date), "%d/%b/%Y:%H:%M:%S %z",
      localtime(&conn->birth_time));

  ri = &conn->request_info;

  flockfile(fp);

  (void) fprintf(fp,
      "%s - %s [%s] \"%s %s HTTP/%s\" %d %" INT64_FMT,
      inet_ntoa(conn->client.rsa.u.sin.sin_addr),
      ri->remote_user == NULL ? "-" : ri->remote_user,
      date,
      ri->request_method ? ri->request_method : "-",
      ri->uri ? ri->uri : "-",
      ri->http_version,
      conn->request_info.status_code, conn->num_bytes_sent);
  log_header(conn, "Referer", fp);
  log_header(conn, "User-Agent", fp);
  (void) fputc('\n', fp);
  (void) fflush(fp);

  funlockfile(fp);
  (void) fclose(fp);
}

static bool_t isbyte(int n) {
  return n >= 0 && n <= 255 ? MG_TRUE : MG_FALSE;
}

// Verify given socket address against the ACL.
// Return -1 if ACL is malformed, 0 if address is disallowed, 1 if allowed.
static enum mg_error_t check_acl(struct mg_context *ctx,
                                 const struct usa *usa) {
  int a, b, c, d, n, mask, allowed;
  char flag;
  uint32_t acl_subnet, acl_mask, remote_ip;
  struct vec vec;
  const char *list = ctx->config->acl;

  if (list == NULL) {
    return MG_SUCCESS;
  }

  (void) memcpy(&remote_ip, &usa->u.sin.sin_addr, sizeof(remote_ip));

  // If any ACL is set, deny by default
  allowed = '-';

  while ((list = next_option(list, &vec, NULL)) != NULL) {
    mask = 32;

    if (sscanf(vec.ptr, "%c%d.%d.%d.%d%n", &flag, &a, &b, &c, &d, &n) != 5) {
      cry(fc(ctx), "%s: subnet must be [+|-]x.x.x.x[/x]", __func__);
      return MG_ERROR;
    } else if (flag != '+' && flag != '-') {
      cry(fc(ctx), "%s: flag must be + or -: [%s]", __func__, vec.ptr);
      return MG_ERROR;
    } else if (!isbyte(a)||!isbyte(b)||!isbyte(c)||!isbyte(d)) {
      cry(fc(ctx), "%s: bad ip address: [%s]", __func__, vec.ptr);
      return MG_ERROR;
    } else if (sscanf(vec.ptr + n, "/%d", &mask) == 0) {
      // Do nothing, no mask specified
    } else if (mask < 0 || mask > 32) {
      cry(fc(ctx), "%s: bad subnet mask: %d [%s]", __func__, n, vec.ptr);
      return MG_ERROR;
    }

    acl_subnet = (a << 24) | (b << 16) | (c << 8) | d;
    acl_mask = mask ? 0xffffffffU << (32 - mask) : 0;

    if (acl_subnet == (ntohl(remote_ip) & acl_mask)) {
      allowed = flag;
    }
  }

  return allowed == '+' ? MG_SUCCESS : MG_ERROR;
}

static void add_to_set(SOCKET fd, fd_set *set, int *max_fd) {
  FD_SET(fd, set);
  if (fd > (SOCKET) *max_fd) {
    *max_fd = (int) fd;
  }
}

#if !defined(_WIN32)
static enum mg_error_t set_uid_option(struct mg_context *ctx) {
  struct passwd *pw;
  const char *uid = ctx->config->uid;
  enum mg_error_t error;

  if (uid == NULL) {
    error = MG_SUCCESS;
  } else {
    if ((pw = getpwnam(uid)) == NULL) {
      cry(fc(ctx), "%s: unknown user [%s]", __func__, uid);
    } else if (setgid(pw->pw_gid) == -1) {
      cry(fc(ctx), "%s: setgid(%s): %s", __func__, uid, strerror(errno));
    } else if (setuid(pw->pw_uid) == -1) {
      cry(fc(ctx), "%s: setuid(%s): %s", __func__, uid, strerror(errno));
    } else {
      error = MG_SUCCESS;
    }
  }

  return error;
}
#endif // !_WIN32

#if !defined(NO_SSL)
static pthread_mutex_t *ssl_mutexes;

static void ssl_locking_callback(int mode, int mutex_num, const char *file,
                                 int line) {
  line = 0;    // Unused
  file = NULL; // Unused

  if (mode & CRYPTO_LOCK) {
    (void) pthread_mutex_lock(&ssl_mutexes[mutex_num]);
  } else {
    (void) pthread_mutex_unlock(&ssl_mutexes[mutex_num]);
  }
}

static unsigned long ssl_id_callback(void) {
  return (unsigned long) pthread_self();
}

static bool_t load_dll(struct mg_context *ctx, const char *dll_name,
                       struct ssl_func *sw) {
  union {void *p; void (*fp)(void);} u;
  void  *dll_handle;
  struct ssl_func *fp;

  if ((dll_handle = dlopen(dll_name, RTLD_LAZY)) == NULL) {
    cry(fc(ctx), "%s: cannot load %s", __func__, dll_name);
    return MG_FALSE;
  }

  for (fp = sw; fp->name != NULL; fp++) {
#ifdef _WIN32
    // GetProcAddress() returns pointer to function
    u.fp = (void (*)(void)) dlsym(dll_handle, fp->name);
#else
    // dlsym() on UNIX returns void *. ISO C forbids casts of data pointers to
    // function pointers. We need to use a union to make a cast.
    u.p = dlsym(dll_handle, fp->name);
#endif /* _WIN32 */
    if (u.fp == NULL) {
      cry(fc(ctx), "%s: cannot find %s", __func__, fp->name);
      return MG_FALSE;
    } else {
      fp->ptr = u.fp;
    }
  }

  return MG_TRUE;
}

// Dynamically load SSL library. Set up ctx->ssl_ctx pointer.
static enum mg_error_t set_ssl_option(struct mg_context *ctx) {
  SSL_CTX *CTX;
  int i, size;
  const char *pem = ctx->config->ssl_certificate;

  if (pem == NULL) {
    return MG_SUCCESS;
  }

  if (load_dll(ctx, SSL_LIB, ssl_sw) == MG_FALSE ||
      load_dll(ctx, CRYPTO_LIB, crypto_sw) == MG_FALSE) {
    return MG_ERROR;
  }

  // Initialize SSL crap
  SSL_library_init();
  SSL_load_error_strings();

  if ((CTX = SSL_CTX_new(SSLv23_server_method())) == NULL) {
    cry(fc(ctx), "SSL_CTX_new error: %s", ssl_error());
  } else if (ctx->config->ssl_password_handler != NULL) {
    SSL_CTX_set_default_passwd_cb(CTX, ctx->config->ssl_password_handler);
  }

  if (CTX != NULL && SSL_CTX_use_certificate_file(CTX, pem,
        SSL_FILETYPE_PEM) == 0) {
    cry(fc(ctx), "%s: cannot open %s: %s", __func__, pem, ssl_error());
    return MG_ERROR;
  } else if (CTX != NULL && SSL_CTX_use_PrivateKey_file(CTX, pem,
        SSL_FILETYPE_PEM) == 0) {
    cry(fc(ctx), "%s: cannot open %s: %s", NULL, pem, ssl_error());
    return MG_ERROR;
  }

  // Initialize locking callbacks, needed for thread safety.
  // http://www.openssl.org/support/faq.html#PROG1
  size = sizeof(pthread_mutex_t) * CRYPTO_num_locks();
  if ((ssl_mutexes = (pthread_mutex_t *) malloc(size)) == NULL) {
    cry(fc(ctx), "%s: cannot allocate mutexes: %s", __func__, ssl_error());
    return MG_ERROR;
  }

  for (i = 0; i < CRYPTO_num_locks(); i++) {
    pthread_mutex_init(&ssl_mutexes[i], NULL);
  }

  CRYPTO_set_locking_callback(&ssl_locking_callback);
  CRYPTO_set_id_callback(&ssl_id_callback);

  // Done with everything. Save the context.
  ctx->ssl_ctx = CTX;

  return MG_SUCCESS;
}
#endif // !NO_SSL

static enum mg_error_t set_gpass_option(struct mg_context *ctx) {
  struct mgstat mgstat;
  const char *path = ctx->config->global_passwords_file;
  return path == NULL || mg_stat(path, &mgstat) == 0 ?  MG_SUCCESS : MG_ERROR;
}

static enum mg_error_t set_acl_option(struct mg_context *ctx) {
  struct usa fake;
  return check_acl(ctx, &fake);
}

static bool_t verify_document_root(struct mg_context *ctx, const char *root) {
  char path[FILENAME_MAX], *p;
  struct mgstat buf;

  if ((p = strchr(root, ',')) == NULL) {
    mg_strlcpy(path, root, sizeof(path));
  } else {
    mg_strlcpy(path, root, p - root + 1);
  }

  if (mg_stat(path, &buf) != 0) {
    cry(fc(ctx), "Invalid root directory: \"%s\"", root);
    return MG_FALSE;
  }
  return MG_TRUE;
}

static void reset_per_request_attributes(struct mg_connection *conn) {
  if (conn->request_info.remote_user != NULL) {
    free((void *) conn->request_info.remote_user);
    conn->request_info.remote_user = NULL;
  }
  conn->request_info.status_code = -1;
  conn->num_bytes_sent = 0;
  conn->content_len = -1;
  conn->consumed_content = 0;
  conn->request_len = conn->data_len = 0;
}

static void close_socket_gracefully(SOCKET sock) {
  char buf[BUFSIZ];
  int n;

  // Send FIN to the client
  (void) shutdown(sock, SHUT_WR);
  set_non_blocking_mode(sock);

  // Read and discard pending data. If we do not do that and close the
  // socket, the data in the send buffer may be discarded. This
  // behaviour is seen on Windows, when client keeps sending data
  // when server decide to close the connection; then when client
  // does recv() it gets no data back.
  do {
    n = pull(NULL, sock, NULL, buf, sizeof(buf));
  } while (n > 0);

  // Now we know that our FIN is ACK-ed, safe to close
  (void) closesocket(sock);
}

static void close_connection(struct mg_connection *conn) {
  reset_per_request_attributes(conn);

  if (conn->ssl) {
    SSL_free(conn->ssl);
    conn->ssl = NULL;
  }

  if (conn->client.sock != INVALID_SOCKET) {
    close_socket_gracefully(conn->client.sock);
  }
}

static void discard_current_request_from_buffer(struct mg_connection *conn) {
  int over_len, body_len;

  over_len = conn->data_len - conn->request_len;
  assert(over_len >= 0);

  if (conn->content_len == -1) {
    body_len = 0;
  } else if (conn->content_len < (int64_t) over_len) {
    body_len = (int) conn->content_len;
  } else {
    body_len = over_len;
  }

  conn->data_len -= conn->request_len + body_len;
  memmove(conn->buf, conn->buf + conn->request_len + body_len, conn->data_len);
}

static void process_new_connection(struct mg_connection *conn) {
  struct mg_request_info *ri = &conn->request_info;
  const char *cl;

  // TODO(lsm): do not clear everything, since it deletes IP address
  // info set up by worker_thread().
  (void) memset(&conn->request_info, 0, sizeof(conn->request_info));
  reset_per_request_attributes(conn);

  // If next request is not pipelined, read it in
  if ((conn->request_len = get_request_len(conn->buf, conn->data_len)) == 0) {
    conn->request_len = read_request(NULL, conn->client.sock, conn->ssl,
        conn->buf, sizeof(conn->buf), &conn->data_len);
  }
  assert(conn->data_len >= conn->request_len);
  if (conn->request_len <= 0) {
    return; // Remote end closed the connection
  }

  // Nul-terminate the request cause parse_http_request() uses sscanf
  conn->buf[conn->request_len - 1] = '\0';
  if (!parse_http_request(conn->buf, ri)) {
    // Do not put garbage in the access log, just send it back to the client
    send_http_error(conn, 400, "Bad Request",
        "Can not parse request: [%.*s]", conn->data_len, conn->buf);
  } else if (strcmp(ri->http_version, "1.0") &&
             strcmp(ri->http_version, "1.1")) {
    // Request seems valid, but HTTP version is strange
    send_http_error(conn, 505, "HTTP version not supported", "");
    log_access(conn);
  } else {
    // Request is valid, handle it
    cl = get_header(ri, "Content-Length");
    conn->content_len = cl == NULL ? -1 : strtoll(cl, NULL, 10);
    conn->birth_time = time(NULL);
    handle_request(conn);
    log_access(conn);
    discard_current_request_from_buffer(conn);
  }
}

// Worker threads take accepted socket from the queue
static bool_t get_socket(struct mg_context *ctx, struct socket *sp) {
  struct timespec ts;

  (void) pthread_mutex_lock(&ctx->mutex);
  DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: thread %p: going idle",
        __func__, (void *) pthread_self()));

  // If the queue is empty, wait. We're idle at this point.
  ctx->num_idle++;
  while (ctx->sq_head == ctx->sq_tail) {
    ts.tv_nsec = 0;
    ts.tv_sec = time(NULL) + 5;
    if (pthread_cond_timedwait(&ctx->empty_cond, &ctx->mutex, &ts) != 0) {
      // Timeout! release the mutex and return
      (void) pthread_mutex_unlock(&ctx->mutex);
      return MG_FALSE;
    }
  }
  assert(ctx->sq_head > ctx->sq_tail);

  // We're going busy now: got a socket to process!
  ctx->num_idle--;

  // Copy socket from the queue and increment tail
  *sp = ctx->queue[ctx->sq_tail % ARRAY_SIZE(ctx->queue)];
  ctx->sq_tail++;
  DEBUG_TRACE((DEBUG_MGS_PREFIX
        "%s: thread %p grabbed socket %d, going busy",
        __func__, (void *) pthread_self(), sp->sock));

  // Wrap pointers if needed
  while (ctx->sq_tail > (int) ARRAY_SIZE(ctx->queue)) {
    ctx->sq_tail -= ARRAY_SIZE(ctx->queue);
    ctx->sq_head -= ARRAY_SIZE(ctx->queue);
  }

  (void) pthread_cond_signal(&ctx->full_cond);
  (void) pthread_mutex_unlock(&ctx->mutex);

  return MG_TRUE;
}

static void worker_thread(struct mg_context *ctx) {
  struct mg_connection conn;

  DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: thread %p starting",
        __func__, (void *) pthread_self()));

  (void) memset(&conn, 0, sizeof(conn));

  while (ctx->stop_flag == 0 && get_socket(ctx, &conn.client) == MG_TRUE) {
    conn.birth_time = time(NULL);
    conn.ctx = ctx;

    // Fill in IP, port info early so even if SSL setup below fails,
    // error handler would have the corresponding info.
    // Thanks to Johannes Winkelmann for the patch.
    conn.request_info.remote_port = ntohs(conn.client.rsa.u.sin.sin_port);
    (void) memcpy(&conn.request_info.remote_ip,
        &conn.client.rsa.u.sin.sin_addr.s_addr, 4);
    conn.request_info.remote_ip = ntohl(conn.request_info.remote_ip);
    conn.request_info.is_ssl = conn.client.is_ssl;

    if (conn.client.is_ssl && (conn.ssl = SSL_new(conn.ctx->ssl_ctx)) == NULL) {
      cry(&conn, "%s: SSL_new: %s", __func__, ssl_error());
    } else if (conn.client.is_ssl &&
        SSL_set_fd(conn.ssl, conn.client.sock) != 1) {
      cry(&conn, "%s: SSL_set_fd: %s", __func__, ssl_error());
    } else if (conn.client.is_ssl && SSL_accept(conn.ssl) != 1) {
      cry(&conn, "%s: SSL handshake error: %s", __func__, ssl_error());
    } else {
      process_new_connection(&conn);
    }

    close_connection(&conn);
  }

  // Signal master that we're done with connection and exiting
  (void) pthread_mutex_lock(&ctx->mutex);
  ctx->num_threads--;
  ctx->num_idle--;
  (void) pthread_cond_signal(&ctx->thr_cond);
  assert(ctx->num_threads >= 0);
  (void) pthread_mutex_unlock(&ctx->mutex);

  DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: thread %p exiting",
        __func__, (void *) pthread_self()));
}

// Master thread adds accepted socket to a queue
static void put_socket(struct mg_context *ctx, const struct socket *sp) {
  (void) pthread_mutex_lock(&ctx->mutex);

  // If the queue is full, wait
  while (ctx->sq_head - ctx->sq_tail >= (int) ARRAY_SIZE(ctx->queue)) {
    (void) pthread_cond_wait(&ctx->full_cond, &ctx->mutex);
  }
  assert(ctx->sq_head - ctx->sq_tail < (int) ARRAY_SIZE(ctx->queue));

  // Copy socket to the queue and increment head
  ctx->queue[ctx->sq_head % ARRAY_SIZE(ctx->queue)] = *sp;
  ctx->sq_head++;
  DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: queued socket %d", __func__, sp->sock));

  // If there are no idle threads, start one
  if (ctx->num_idle == 0 && ctx->num_threads < atoi(ctx->config->num_threads)) {
    if (start_thread(ctx, (mg_thread_func_t) worker_thread, ctx) != 0) {
      cry(fc(ctx), "Cannot start thread: %d", ERRNO);
    } else {
      ctx->num_threads++;
    }
  }

  (void) pthread_cond_signal(&ctx->empty_cond);
  (void) pthread_mutex_unlock(&ctx->mutex);
}

static void accept_new_connection(const struct socket *listener,
                                  struct mg_context *ctx) {
  struct socket accepted;
  bool_t allowed;

  accepted.rsa.len = sizeof(accepted.rsa.u.sin);
  accepted.lsa = listener->lsa;
  if ((accepted.sock = accept(listener->sock, &accepted.rsa.u.sa,
                              &accepted.rsa.len)) == INVALID_SOCKET) {
    return;
  }

  allowed = check_acl(ctx, &accepted.rsa) == MG_SUCCESS;

  if (allowed == MG_SUCCESS) {
    // Put accepted socket structure into the queue
    DEBUG_TRACE((DEBUG_MGS_PREFIX "%s: accepted socket %d",
          __func__, accepted.sock));
    accepted.is_ssl = listener->is_ssl;
    put_socket(ctx, &accepted);
  } else {
    cry(fc(ctx), "%s: %s is not allowed to connect",
        __func__, inet_ntoa(accepted.rsa.u.sin.sin_addr));
    (void) closesocket(accepted.sock);
  }
}

static void master_thread(struct mg_context *ctx) {
  fd_set read_set;
  struct timeval tv;
  struct socket *sp;
  int max_fd;

  while (ctx->stop_flag == 0) {
    FD_ZERO(&read_set);
    max_fd = -1;

    // Add listening sockets to the read set
    for (sp = ctx->listening_sockets; sp != NULL; sp = sp->next) {
      add_to_set(sp->sock, &read_set, &max_fd);
    }

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if (select(max_fd + 1, &read_set, NULL, NULL, &tv) < 0) {
#ifdef _WIN32
      // On windows, if read_set and write_set are empty,
      // select() returns "Invalid parameter" error
      // (at least on my Windows XP Pro). So in this case, we sleep here.
      sleep(1);
#endif // _WIN32
    } else {
      for (sp = ctx->listening_sockets; sp != NULL; sp = sp->next)
        if (FD_ISSET(sp->sock, &read_set))
          accept_new_connection(sp, ctx);
    }
  }

  // Stop signal received: somebody called mg_stop. Quit.
  close_all_listening_sockets(ctx);

  // Wait until all threads finish
  (void) pthread_mutex_lock(&ctx->mutex);
  while (ctx->num_threads > 0) {
    (void) pthread_cond_wait(&ctx->thr_cond, &ctx->mutex);
  }
  (void) pthread_mutex_unlock(&ctx->mutex);

  // Deallocate SSL context
  if (ctx->ssl_ctx != NULL) {
    SSL_CTX_free(ctx->ssl_ctx);
  }

  (void) pthread_mutex_destroy(&ctx->mutex);
  (void) pthread_cond_destroy(&ctx->thr_cond);
  (void) pthread_cond_destroy(&ctx->empty_cond);
  (void) pthread_cond_destroy(&ctx->full_cond);

  // Signal mg_stop() that we're done
  ctx->stop_flag = 2;
}

void mg_stop(struct mg_context *ctx) {
  ctx->stop_flag = 1;

  // Wait until mg_fini() stops
  while (ctx->stop_flag != 2) {
    (void) sleep(1);
  }

  assert(ctx->num_threads == 0);
  free(ctx);

#if defined(_WIN32)
  (void) WSACleanup();
#endif // _WIN32
}

struct mg_context * mg_start(struct mg_config *config) {
  struct mg_context *ctx, fake_ctx;

#if defined(_WIN32)
  WSADATA data;
  WSAStartup(MAKEWORD(2,2), &data);
#endif // _WIN32

  // TODO(lsm): make a copy of the config
  fake_ctx.config = config;

  if (config->listening_ports == NULL ||
      config->num_threads == NULL ||
      config->auth_domain == NULL) {
    cry(fc(&fake_ctx), "Please specify "
        "num_threads, listening_ports, auth_domain");
    return NULL;
  } else if (config->document_root != NULL &&
      verify_document_root(&fake_ctx, config->document_root) != MG_TRUE) {
    cry(fc(&fake_ctx), "Invalid root directory: \"%s\"", config->document_root);
    return NULL;
  } else if ((ctx = calloc(1, sizeof(*ctx))) == NULL) {
    cry(fc(&fake_ctx), "Cannot allocate mongoose context");
    return NULL;
  }
  ctx->config = config;

  // NOTE(lsm): order is important here. SSL certificates must
  // be initialized before listening ports. UID must be set last.
  if (set_ssl_option(ctx) == MG_ERROR ||
      set_ports_option(ctx) == MG_ERROR ||
      set_gpass_option(ctx) == MG_ERROR ||
#if !defined(_WIN32)
      set_uid_option(ctx) == MG_ERROR ||
#endif
      set_acl_option(ctx) == MG_ERROR) {
    free(ctx);
    return NULL;
  }

#if !defined(_WIN32)
  // Ignore SIGPIPE signal, so if browser cancels the request, it
  // won't kill the whole process.
  (void) signal(SIGPIPE, SIG_IGN);
#endif // !_WIN32

  (void) pthread_mutex_init(&ctx->mutex, NULL);
  (void) pthread_cond_init(&ctx->thr_cond, NULL);
  (void) pthread_cond_init(&ctx->empty_cond, NULL);
  (void) pthread_cond_init(&ctx->full_cond, NULL);

  // Start master (listening) thread
  start_thread(ctx, (mg_thread_func_t) master_thread, ctx);

  return ctx;
}
