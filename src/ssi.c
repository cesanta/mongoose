#include "log.h"
#include "ssi.h"

#ifndef MG_MAX_SSI_DEPTH
#define MG_MAX_SSI_DEPTH 5
#endif

#if MG_ENABLE_SSI
static char *mg_ssi(const char *path, const char *root, int depth) {
  struct mg_iobuf b = {NULL, 0, 0};
  FILE *fp = fopen(path, "rb");
  if (fp != NULL) {
    char buf[BUFSIZ], arg[sizeof(buf)];
    int ch, intag = 0;
    size_t len = 0, align = MG_IO_SIZE;
    while ((ch = fgetc(fp)) != EOF) {
      if (intag && ch == '>' && buf[len - 1] == '-' && buf[len - 2] == '-') {
        buf[len++] = ch & 0xff;
        if (sscanf(buf, "<!--#include file=\"%[^\"]", arg)) {
          char tmp[PATH_MAX], *p = (char *) path + strlen(path), *data;
          while (p > path && p[-1] != MG_DIRSEP && p[-1] != '/') p--;
          snprintf(tmp, sizeof(tmp), "%.*s%s", (int) (p - path), path, arg);
          if (depth < MG_MAX_SSI_DEPTH &&
              (data = mg_ssi(tmp, root, depth + 1)) != NULL) {
            mg_iobuf_append(&b, data, strlen(data), align);
            free(data);
          } else {
            LOG(LL_ERROR, ("%s: file=%s error or too deep", path, arg));
          }
        } else if (sscanf(buf, "<!--#include virtual=\"%[^\"]", arg)) {
          char tmp[PATH_MAX], *data;
          snprintf(tmp, sizeof(tmp), "%s%s", root, arg);
          if (depth < MG_MAX_SSI_DEPTH &&
              (data = mg_ssi(tmp, root, depth + 1)) != NULL) {
            mg_iobuf_append(&b, data, strlen(data), align);
            free(data);
          } else {
            LOG(LL_ERROR, ("%s: virtual=%s error or too deep", path, arg));
          }
        } else {
          // Unknown SSI tag
          LOG(LL_INFO, ("Unknown SSI tag: %.*s", (int) len, buf));
          mg_iobuf_append(&b, buf, len, align);
        }
        intag = 0;
        len = 0;
      } else if (ch == '<') {
        intag = 1;
        if (len > 0) mg_iobuf_append(&b, buf, len, align);
        len = 0;
        buf[len++] = ch & 0xff;
      } else if (intag) {
        if (len == 5 && strncmp(buf, "<!--#", 5) != 0) {
          intag = 0;
        } else if (len >= sizeof(buf) - 2) {
          LOG(LL_ERROR, ("%s: SSI tag is too large", path));
          len = 0;
        }
        buf[len++] = ch & 0xff;
      } else {
        buf[len++] = ch & 0xff;
        if (len >= sizeof(buf)) {
          mg_iobuf_append(&b, buf, len, align);
          len = 0;
        }
      }
    }
    if (len > 0) mg_iobuf_append(&b, buf, len, align);
    if (b.len > 0) mg_iobuf_append(&b, "", 1, align);  // nul-terminate
    fclose(fp);
  }
  (void) depth;
  (void) root;
  return (char *) b.buf;
}

void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath) {
  char *data = mg_ssi(fullpath, root, 0);
  mg_http_reply(c, 200, "", "%s", data == NULL ? "" : data);
  free(data);
}
#endif
