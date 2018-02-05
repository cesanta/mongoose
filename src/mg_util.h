/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === Utility API
 */

#ifndef CS_MONGOOSE_SRC_UTIL_H_
#define CS_MONGOOSE_SRC_UTIL_H_

#include <stdio.h>

#include "mg_common.h"
#include "mg_net_if.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MG_MAX_PATH
#ifdef PATH_MAX
#define MG_MAX_PATH PATH_MAX
#else
#define MG_MAX_PATH 256
#endif
#endif

/*
 * Fetches substring from input string `s`, `end` into `v`.
 * Skips initial delimiter characters. Records first non-delimiter character
 * at the beginning of substring `v`. Then scans the rest of the string
 * until a delimiter character or end-of-string is found.
 * `delimiters` is a 0-terminated string containing delimiter characters.
 * Either one of `delimiters` or `end_string` terminates the search.
 * Returns an `s` pointer, advanced forward where parsing has stopped.
 */
const char *mg_skip(const char *s, const char *end_string,
                    const char *delimiters, struct mg_str *v);

/*
 * Decodes base64-encoded string `s`, `len` into the destination `dst`.
 * The destination has to have enough space to hold the decoded buffer.
 * Decoding stops either when all strings have been decoded or invalid an
 * character appeared.
 * Destination is '\0'-terminated.
 * Returns the number of decoded characters. On success, that should be equal
 * to `len`. On error (invalid character) the return value is smaller then
 * `len`.
 */
int mg_base64_decode(const unsigned char *s, int len, char *dst);

/*
 * Base64-encode chunk of memory `src`, `src_len` into the destination `dst`.
 * Destination has to have enough space to hold encoded buffer.
 * Destination is '\0'-terminated.
 */
void mg_base64_encode(const unsigned char *src, int src_len, char *dst);

#if MG_ENABLE_FILESYSTEM
/*
 * Performs a 64-bit `stat()` call against a given file.
 *
 * `path` should be UTF8 encoded.
 *
 * Return value is the same as for `stat()` syscall.
 */
int mg_stat(const char *path, cs_stat_t *st);

/*
 * Opens the given file and returns a file stream.
 *
 * `path` and `mode` should be UTF8 encoded.
 *
 * Return value is the same as for the `fopen()` call.
 */
FILE *mg_fopen(const char *path, const char *mode);

/*
 * Opens the given file and returns a file stream.
 *
 * `path` should be UTF8 encoded.
 *
 * Return value is the same as for the `open()` syscall.
 */
int mg_open(const char *path, int flag, int mode);

/*
 * Reads data from the given file stream.
 *
 * Return value is a number of bytes readen.
 */
size_t mg_fread(void *ptr, size_t size, size_t count, FILE *f);

/*
 * Writes data to the given file stream.
 *
 * Return value is a number of bytes wtitten.
 */
size_t mg_fwrite(const void *ptr, size_t size, size_t count, FILE *f);

#endif /* MG_ENABLE_FILESYSTEM */

#if MG_ENABLE_THREADS
/*
 * Starts a new detached thread.
 * Arguments and semantics are the same as pthead's `pthread_create()`.
 * `thread_func` is a thread function, `thread_func_param` is a parameter
 * that is passed to the thread function.
 */
void *mg_start_thread(void *(*thread_func)(void *), void *thread_func_param);
#endif

void mg_set_close_on_exec(sock_t);

#define MG_SOCK_STRINGIFY_IP 1
#define MG_SOCK_STRINGIFY_PORT 2
#define MG_SOCK_STRINGIFY_REMOTE 4
/*
 * Converts a connection's local or remote address into string.
 *
 * The `flags` parameter is a bit mask that controls the behaviour,
 * see `MG_SOCK_STRINGIFY_*` definitions.
 *
 * - MG_SOCK_STRINGIFY_IP - print IP address
 * - MG_SOCK_STRINGIFY_PORT - print port number
 * - MG_SOCK_STRINGIFY_REMOTE - print remote peer's IP/port, not local address
 *
 * If both port number and IP address are printed, they are separated by `:`.
 * If compiled with `-DMG_ENABLE_IPV6`, IPv6 addresses are supported.
 * Return length of the stringified address.
 */
int mg_conn_addr_to_str(struct mg_connection *c, char *buf, size_t len,
                        int flags);
#if MG_NET_IF == MG_NET_IF_SOCKET
/* Legacy interface. */
void mg_sock_to_str(sock_t sock, char *buf, size_t len, int flags);
#endif

/*
 * Convert the socket's address into string.
 *
 * `flags` is MG_SOCK_STRINGIFY_IP and/or MG_SOCK_STRINGIFY_PORT.
 */
int mg_sock_addr_to_str(const union socket_address *sa, char *buf, size_t len,
                        int flags);

#if MG_ENABLE_HEXDUMP
/*
 * Generates a human-readable hexdump of memory chunk.
 *
 * Takes a memory buffer `buf` of length `len` and creates a hex dump of that
 * buffer in `dst`. The generated output is a-la hexdump(1).
 * Returns the length of generated string, excluding terminating `\0`. If
 * returned length is bigger than `dst_len`, the overflow bytes are discarded.
 */
int mg_hexdump(const void *buf, int len, char *dst, int dst_len);

/* Same as mg_hexdump, but with output going to file instead of a buffer. */
void mg_hexdumpf(FILE *fp, const void *buf, int len);

/*
 * Generates human-readable hexdump of the data sent or received by the
 * connection. `path` is a file name where hexdump should be written.
 * `num_bytes` is a number of bytes sent/received. `ev` is one of the `MG_*`
 * events sent to an event handler. This function is supposed to be called from
 * the event handler.
 */
void mg_hexdump_connection(struct mg_connection *nc, const char *path,
                           const void *buf, int num_bytes, int ev);
#endif

/*
 * Returns true if target platform is big endian.
 */
int mg_is_big_endian(void);

/*
 * Use with cs_base64_init/update/finish in order to write out base64 in chunks.
 */
void mg_mbuf_append_base64_putc(char ch, void *user_data);

/*
 * Encode `len` bytes starting at `data` as base64 and append them to an mbuf.
 */
void mg_mbuf_append_base64(struct mbuf *mbuf, const void *data, size_t len);

/*
 * Generate a Basic Auth header and appends it to buf.
 * If pass is NULL, then user is expected to contain the credentials pair
 * already encoded as `user:pass`.
 */
void mg_basic_auth_header(const struct mg_str user, const struct mg_str pass,
                          struct mbuf *buf);

/*
 * URL-escape the specified string.
 * All non-printable characters are escaped, plus `._-$,;~()/`.
 * Input need not be NUL-terminated, but the returned string is.
 * Returned string is heap-allocated and must be free()'d.
 */
struct mg_str mg_url_encode(const struct mg_str src);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_UTIL_H_ */
