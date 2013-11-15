#include "internal.h"

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

int mg_vprintf(struct mg_connection *conn, const char *fmt, va_list ap) {
  char mem[MG_BUF_LEN], *buf = mem;
  int len;

  if ((len = alloc_vprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
    len = mg_write(conn, buf, (size_t) len);
  }
  if (buf != mem && buf != NULL) {
    free(buf);
  }

  return len;
}

int mg_printf(struct mg_connection *conn, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  return mg_vprintf(conn, fmt, ap);
}

static int mg_chunked_printf(struct mg_connection *conn, const char *fmt, ...) {
  char mem[MG_BUF_LEN], *buf = mem;
  int len;

  va_list ap;
  va_start(ap, fmt);
  if ((len = alloc_vprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
    len = mg_printf(conn, "%X\r\n%s\r\n", len, buf);
  }

  if (buf != mem && buf != NULL) {
    free(buf);
  }

  return len;
}


