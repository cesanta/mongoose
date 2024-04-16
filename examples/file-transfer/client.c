// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved
//
// Example HTTP client. Connect to `s_url`, send request, wait for a response,
// print the response and exit.
// You can change `s_url` from the command line by executing: ./example YOUR_URL
//
// To enable SSL/TLS, , see https://mongoose.ws/tutorials/tls/#how-to-build

#include "mongoose.h"

static int s_debug_level = MG_LL_INFO;
static const char *s_user = "user";
static const char *s_pass = "pass";
static const char *s_fname = NULL;
static struct mg_fd *fd;  // file descriptor
static size_t fsize;
static const char *s_url = "http://localhost:8090/upload/foo.txt";
static const uint64_t s_timeout_ms = 1500;  // Connect timeout in milliseconds

// Print HTTP response and signal that we're done
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    // Connection created. Store connect expiration time in c->data
    *(uint64_t *) c->data = mg_millis() + s_timeout_ms;
  } else if (ev == MG_EV_POLL) {
    if (mg_millis() > *(uint64_t *) c->data &&
        (c->is_connecting || c->is_resolving)) {
      mg_error(c, "Connect timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    // Connected to server. Extract host name from URL
    struct mg_str host = mg_url_host(s_url);
    // Send request
    MG_DEBUG(("Connected, send request"));
    mg_printf(c,
              "POST %s HTTP/1.0\r\n"
              "Host: %.*s\r\n"
              "Content-Type: octet-stream\r\n"
              "Content-Length: %d\r\n",
              mg_url_uri(s_url), (int) host.len, host.buf, fsize);
    mg_http_bauth(c, s_user, s_pass);  // Add Basic auth header
    mg_printf(c, "%s", "\r\n");        // End HTTP headers
  } else if (ev == MG_EV_WRITE && c->send.len < MG_IO_SIZE) {
    uint8_t *buf = alloca(MG_IO_SIZE);
    size_t len = MG_IO_SIZE - c->send.len;
    len = fsize < len ? fsize : len;
    fd->fs->rd(fd->fd, buf, len);
    mg_send(c, buf, len);
    fsize -= len;
    MG_DEBUG(("sent %u bytes", len));
  } else if (ev == MG_EV_HTTP_MSG) {
    MG_DEBUG(("MSG"));
    // Response is received. Print it
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    printf("%.*s", (int) hm->body.len, hm->body.buf);
    c->is_draining = 1;  // Tell mongoose to close this connection
    mg_fs_close(fd);
    *(bool *) c->fn_data = true;  // Tell event loop to stop
  } else if (ev == MG_EV_ERROR) {
    MG_DEBUG(("ERROR"));
    mg_fs_close(fd);
    *(bool *) c->fn_data = true;  // Error, tell event loop to stop
  }
}

static void usage(const char *prog) {
  fprintf(stderr,
          "File Transfer client based on Mongoose v.%s\n"
          "Usage: %s -f NAME OPTIONS\n"
          "  -u NAME   - user name, default: '%s'\n"
          "  -p PWD    - password, default: '%s'\n"
          "  -U URL    - Full server URL, including destination file name; "
          "default: '%s'\n"
          "  -f NAME   - File to send\n"
          "  -v LEVEL  - debug level, from 0 to 4, default: %d\n",
          MG_VERSION, prog, s_user, s_pass, s_url, s_debug_level);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;  // Event manager
  bool done = false;  // Event handler flips it to true
  time_t mtime;
  int i;

  // Parse command-line flags
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-f") == 0) {
      s_fname = argv[++i];
    } else if (strcmp(argv[i], "-u") == 0) {
      s_user = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0) {
      s_pass = argv[++i];
    } else if (strcmp(argv[i], "-U") == 0) {
      s_url = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0) {
      s_debug_level = atoi(argv[++i]);
    } else {
      usage(argv[0]);
    }
  }
  if (s_fname == NULL) usage(argv[0]);
  mg_fs_posix.st(s_fname, &fsize, &mtime);
  if (fsize == 0 ||
      (fd = mg_fs_open(&mg_fs_posix, s_fname, MG_FS_READ)) == NULL) {
    MG_ERROR(("open failed: %d", errno));
    exit(EXIT_FAILURE);
  }

  mg_log_set(s_debug_level);
  mg_mgr_init(&mgr);                        // Initialise event manager
  mg_http_connect(&mgr, s_url, fn, &done);  // Create client connection
  while (!done) mg_mgr_poll(&mgr, 50);      // Event manager loops until 'done'
  mg_mgr_free(&mgr);                        // Free resources
  return 0;
}
