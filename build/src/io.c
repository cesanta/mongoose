#include "internal.h"

// Return number of bytes left to read for this connection
static int64_t left_to_read(const struct mg_connection *conn) {
  return conn->content_len + conn->request_len - conn->num_bytes_read;
}

// Write data to the IO channel - opened file descriptor, socket or SSL
// descriptor. Return number of bytes written.
static int64_t push(FILE *fp, SOCKET sock, SSL *ssl, const char *buf,
                    int64_t len) {
  int64_t sent;
  int n, k;

  (void) ssl;  // Get rid of warning
  sent = 0;
  while (sent < len) {

    // How many bytes we send in this iteration
    k = len - sent > INT_MAX ? INT_MAX : (int) (len - sent);

#if !defined(NO_SSL)
    if (ssl != NULL) {
      n = SSL_write(ssl, buf + sent, k);
    } else
#endif
    if (fp != NULL) {
      n = (int) fwrite(buf + sent, 1, (size_t) k, fp);
      if (ferror(fp))
        n = -1;
    } else {
      n = send(sock, buf + sent, (size_t) k, MSG_NOSIGNAL);
    }

    if (n <= 0)
      break;

    sent += n;
  }

  return sent;
}

// Read from IO channel - opened file descriptor, socket, or SSL descriptor.
// Return negative value on error, or number of bytes read on success.
static int pull(FILE *fp, struct mg_connection *conn, char *buf, int len) {
  int nread;

  if (len <= 0) return 0;
  if (fp != NULL) {
    // Use read() instead of fread(), because if we're reading from the CGI
    // pipe, fread() may block until IO buffer is filled up. We cannot afford
    // to block and must pass all read bytes immediately to the client.
    nread = read(fileno(fp), buf, (size_t) len);
#ifndef NO_SSL
  } else if (conn->ssl != NULL) {
    nread = SSL_read(conn->ssl, buf, len);
#endif
  } else {
    nread = recv(conn->client.sock, buf, (size_t) len, 0);
  }
  if (nread > 0) {
    conn->num_bytes_read += nread;
  }

  return conn->ctx->stop_flag ? -1 : nread;
}

static int pull_all(FILE *fp, struct mg_connection *conn, char *buf, int len) {
  int n, nread = 0;

  while (len > 0 && conn->ctx->stop_flag == 0) {
    n = pull(fp, conn, buf + nread, len);
    if (n < 0) {
      nread = n;  // Propagate the error
      break;
    } else if (n == 0) {
      break;  // No more data to read
    } else {
      nread += n;
      len -= n;
    }
  }

  return nread;
}

int mg_read(struct mg_connection *conn, void *buf, int len) {
  int n, buffered_len, nread = 0;
  int64_t left;

  if (conn->content_len <= 0) {
    return 0;
  }

  // conn->buf           body
  //    |=================|==========|===============|
  //    |<--request_len-->|                          |
  //    |<-----------data_len------->|      conn->buf + conn->buf_size

  // First, check for data buffered in conn->buf by read_request().
  if (len > 0 && (buffered_len = conn->data_len - conn->request_len) > 0) {
    char *body = conn->buf + conn->request_len;
    if (buffered_len > len) buffered_len = len;
    if (buffered_len > conn->content_len) buffered_len = (int)conn->content_len;

    memcpy(buf, body, (size_t) buffered_len);
    memmove(body, body + buffered_len,
            &conn->buf[conn->data_len] - &body[buffered_len]);
    len -= buffered_len;
    conn->data_len -= buffered_len;
    nread += buffered_len;
  }

  // Read data from the socket.
  if (len > 0 && (left = left_to_read(conn)) > 0) {
    if (left < len) {
      len = (int) left;
    }
    n = pull_all(NULL, conn, (char *) buf + nread, (int) len);
    nread = n >= 0 ? nread + n : n;
  }

  return nread;
}

int mg_write(struct mg_connection *conn, const void *buf, int len) {
  return (int) push(NULL, conn->client.sock, conn->ssl, (const char *) buf,
                    (int64_t) len);
}

// Keep reading the input (either opened file descriptor fd, or socket sock,
// or SSL descriptor ssl) into buffer buf, until \r\n\r\n appears in the
// buffer (which marks the end of HTTP request). Buffer buf may already
// have some data. The length of the data is stored in nread.
// Upon every read operation, increase nread by the number of bytes read.
static int read_request(FILE *fp, struct mg_connection *conn,
                        char *buf, int bufsiz, int *nread) {
  int request_len, n = 0;

  request_len = get_request_len(buf, *nread);
  while (conn->ctx->stop_flag == 0 &&
         *nread < bufsiz &&
         request_len == 0 &&
         (n = pull(fp, conn, buf + *nread, bufsiz - *nread)) > 0) {
    *nread += n;
    assert(*nread <= bufsiz);
    request_len = get_request_len(buf, *nread);
  }

  return request_len <= 0 && n <= 0 ? -1 : request_len;
}

// Send len bytes from the opened file to the client.
static void send_file_data(struct mg_connection *conn, FILE *fp,
                           int64_t offset, int64_t len) {
  char buf[MG_BUF_LEN];
  int num_read, num_written, to_read;

  // If offset is beyond file boundaries, don't send anything
  if (offset > 0 && fseeko(fp, offset, SEEK_SET) != 0) {
    return;
  }

  while (len > 0) {
    // Calculate how much to read from the file in the buffer
    to_read = sizeof(buf);
    if ((int64_t) to_read > len) {
      to_read = (int) len;
    }

    // Read from file, exit the loop on error
    if ((num_read = fread(buf, 1, (size_t) to_read, fp)) <= 0) {
      break;
    }

    // Send read bytes to the client, exit the loop on error
    if ((num_written = mg_write(conn, buf, (size_t) num_read)) != num_read) {
      break;
    }

    // Both read and were successful, adjust counters
    conn->num_bytes_sent += num_written;
    len -= num_written;
  }
}

