#include "log.h"
#include "printf.h"
#include "ssi.h"

#ifndef MG_MAX_SSI_DEPTH
#define MG_MAX_SSI_DEPTH 5
#endif

#ifndef MG_SSI_BUFSIZ
#define MG_SSI_BUFSIZ 1024
#endif

#if MG_ENABLE_SSI
static char *mg_ssi(const char *path, const char *root, int depth) {
  struct mg_iobuf b = {NULL, 0, 0, MG_IO_SIZE};
  FILE *fp = fopen(path, "rb");
  if (fp != NULL) {
    char buf[MG_SSI_BUFSIZ], arg[sizeof(buf)];
    int ch, intag = 0;
    size_t len = 0;
    buf[0] = arg[0] = '\0';
    while ((ch = fgetc(fp)) != EOF) {
      if (intag && ch == '>' && buf[len - 1] == '-' && buf[len - 2] == '-') {
        buf[len++] = (char) (ch & 0xff);
        buf[len] = '\0';
        if (sscanf(buf, "<!--#include file=\"%[^\"]", arg)) {
          char tmp[MG_PATH_MAX + MG_SSI_BUFSIZ + 10],
              *p = (char *) path + strlen(path), *data;
          while (p > path && p[-1] != MG_DIRSEP && p[-1] != '/') p--;
          mg_snprintf(tmp, sizeof(tmp), "%.*s%s", (int) (p - path), path, arg);
          if (depth < MG_MAX_SSI_DEPTH &&
              (data = mg_ssi(tmp, root, depth + 1)) != NULL) {
            mg_iobuf_add(&b, b.len, data, strlen(data));
            free(data);
          } else {
            MG_ERROR(("%s: file=%s error or too deep", path, arg));
          }
        } else if (sscanf(buf, "<!--#include virtual=\"%[^\"]", arg)) {
          char tmp[MG_PATH_MAX + MG_SSI_BUFSIZ + 10], *data;
          mg_snprintf(tmp, sizeof(tmp), "%s%s", root, arg);
          if (depth < MG_MAX_SSI_DEPTH &&
              (data = mg_ssi(tmp, root, depth + 1)) != NULL) {
            mg_iobuf_add(&b, b.len, data, strlen(data));
            free(data);
          } else {
            MG_ERROR(("%s: virtual=%s error or too deep", path, arg));
          }
        } else {
          // Unknown SSI tag
          MG_ERROR(("Unknown SSI tag: %.*s", (int) len, buf));
          mg_iobuf_add(&b, b.len, buf, len);
        }
        intag = 0;
        len = 0;
      } else if (ch == '<') {
        intag = 1;
        if (len > 0) mg_iobuf_add(&b, b.len, buf, len);
        len = 0;
        buf[len++] = (char) (ch & 0xff);
      } else if (intag) {
        if (len == 5 && strncmp(buf, "<!--#", 5) != 0) {
          intag = 0;
        } else if (len >= sizeof(buf) - 2) {
          MG_ERROR(("%s: SSI tag is too large", path));
          len = 0;
        }
        buf[len++] = (char) (ch & 0xff);
      } else {
        buf[len++] = (char) (ch & 0xff);
        if (len >= sizeof(buf)) {
          mg_iobuf_add(&b, b.len, buf, len);
          len = 0;
        }
      }
    }
    if (len > 0) mg_iobuf_add(&b, b.len, buf, len);
    if (b.len > 0) mg_iobuf_add(&b, b.len, "", 1);  // nul-terminate
    fclose(fp);
  }
  (void) depth;
  (void) root;
  return (char *) b.buf;
}

void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath) {
  const char *headers = "Content-Type: text/html; charset=utf-8\r\n";
  char *data = mg_ssi(fullpath, root, 0);
  mg_http_reply(c, 200, headers, "%s", data == NULL ? "" : data);
  free(data);
}
#else
void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath) {
  mg_http_reply(c, 501, NULL, "SSI not enabled");
  (void) root, (void) fullpath;
}
#endif
