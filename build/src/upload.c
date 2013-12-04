#include "internal.h"

FILE *mg_upload(struct mg_connection *conn, const char *destination_dir,
                char *path, int path_len) {
  const char *content_type_header, *boundary_start;
  char *buf, fname[1024], boundary[100], *s, *p;
  int bl, n, i, j, headers_len, boundary_len, eof, buf_len, to_read, len = 0;
  FILE *fp;

  // Request looks like this:
  //
  // POST /upload HTTP/1.1
  // Host: 127.0.0.1:8080
  // Content-Length: 244894
  // Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryRVr
  //
  // ------WebKitFormBoundaryRVr
  // Content-Disposition: form-data; name="file"; filename="accum.png"
  // Content-Type: image/png
  //
  //  <89>PNG
  //  <PNG DATA>
  // ------WebKitFormBoundaryRVr

  // Extract boundary string from the Content-Type header
  if ((content_type_header = mg_get_header(conn, "Content-Type")) == NULL ||
      (boundary_start = mg_strcasestr(content_type_header,
                                      "boundary=")) == NULL ||
      (sscanf(boundary_start, "boundary=\"%99[^\"]\"", boundary) == 0 &&
       sscanf(boundary_start, "boundary=%99s", boundary) == 0) ||
      boundary[0] == '\0') {
    return NULL;
  }

  boundary_len = strlen(boundary);
  bl = boundary_len + 4;  // \r\n--<boundary>

  //                     buf
  // conn->buf            |<--------- buf_len ------>|
  //    |=================|==========|===============|
  //    |<--request_len-->|<--len--->|               |
  //    |<-----------data_len------->|      conn->buf + conn->buf_size

  buf = conn->buf + conn->request_len;
  buf_len = conn->buf_size - conn->request_len;
  len = conn->data_len - conn->request_len;

  for (;;) {
    // Pull in headers
    assert(len >= 0 && len <= buf_len);
    to_read = buf_len - len;
    if (to_read > left_to_read(conn)) {
      to_read = (int) left_to_read(conn);
    }
    while (len < buf_len &&
           (n = pull(NULL, conn, buf + len, to_read)) > 0) {
      len += n;
    }
    if ((headers_len = get_request_len(buf, len)) <= 0) {
      break;
    }

    // Fetch file name.
    fname[0] = '\0';
    for (i = j = 0; i < headers_len; i++) {
      if (buf[i] == '\r' && buf[i + 1] == '\n') {
        buf[i] = buf[i + 1] = '\0';
        // TODO(lsm): don't expect filename to be the 3rd field,
        // parse the header properly instead.
        sscanf(&buf[j], "Content-Disposition: %*s %*s filename=\"%1023[^\"]",
               fname);
        j = i + 2;
      }
    }

    // Give up if the headers are not what we expect
    if (fname[0] == '\0') {
      break;
    }

    // Move data to the beginning of the buffer
    assert(len >= headers_len);
    memmove(buf, &buf[headers_len], len - headers_len);
    len -= headers_len;
    conn->data_len = conn->request_len + len;

    // We open the file with exclusive lock held. This guarantee us
    // there is no other thread can save into the same file simultaneously.
    fp = NULL;

    // Construct destination file name. Do not allow paths to have slashes.
    s = fname;
    if ((p = strrchr(fname, '/')) != NULL && p > s) s = p;
    if ((p = strrchr(fname, '\\')) != NULL && p > s) s = p;

    // Open file in binary mode. TODO: set an exclusive lock.
    snprintf(path, path_len, "%s/%s", destination_dir, s);
    if ((fp = fopen(path, "wb")) == NULL) {
      break;
    }

    // Read POST data, write into file until boundary is found.
    eof = n = 0;
    do {
      len += n;
      for (i = 0; i < len - bl; i++) {
        if (!memcmp(&buf[i], "\r\n--", 4) &&
            !memcmp(&buf[i + 4], boundary, boundary_len)) {
          // Found boundary, that's the end of file data.
          fwrite(buf, 1, i, fp);
          eof = 1;
          memmove(buf, &buf[i + bl], len - (i + bl));
          len -= i + bl;
          break;
        }
      }
      if (!eof && len > bl) {
        fwrite(buf, 1, len - bl, fp);
        memmove(buf, &buf[len - bl], bl);
        len = bl;
      }
      to_read = buf_len - len;
      if (to_read > left_to_read(conn)) {
        to_read = (int) left_to_read(conn);
      }
    } while (!eof && (n = pull(NULL, conn, buf + len, to_read)) > 0);
    conn->data_len = conn->request_len + len;

    if (eof) {
      rewind(fp);
      return fp;
    } else {
      fclose(fp);
    }
  }

  return NULL;
}


