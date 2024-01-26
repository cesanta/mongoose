// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include <signal.h>
#include "mongoose.h"

static int s_debug_level = MG_LL_INFO;
static int s_max_size = 10000;
static const char *s_root_dir = "web_root";
static const char *s_upld_dir = "upload";
static const char *s_listening_address = "http://0.0.0.0:8090";
static const char *s_user = "user";
static const char *s_pass = "pass";

// Handle interrupts, like Ctrl-C
static int s_signo;
static void signal_handler(int signo) {
  s_signo = signo;
}

static bool authuser(struct mg_http_message *hm) {
  char user[256], pass[256];
  mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
  if (strcmp(user, s_user) == 0 && strcmp(pass, s_pass) == 0) return true;
  return false;
}

// Streaming upload example. Demonstrates how to use MG_EV_READ events
// to get large payload in smaller chunks. To test, use curl utility:
static void cb(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_READ) {
    // Parse the incoming data ourselves. If we can parse the request,
    // store two size_t variables in c->data: expected len and recv len.
    size_t *data = (size_t *) c->data;
    struct mg_fd *fd = (struct mg_fd *) c->fn_data;  // get file descriptor
    if (data[0]) {  // Already parsed, receiving body
      data[1] += c->recv.len;
      MG_DEBUG(("Got chunk len %lu, %lu total", c->recv.len, data[1]));
      fd->fs->wr(fd->fd, c->recv.buf, c->recv.len);
      c->recv.len = 0;  // And cleanup the receive buffer. Streaming!
      if (data[1] >= data[0]) {
        mg_fs_close(fd);
        mg_http_reply(c, 200, "", "ok\n");
      }
    } else if (c->is_resp == 0) {
      struct mg_http_message hm;
      int n = mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
      if (n < 0) mg_error(c, "Bad response");
      if (n > 0) {
        if (mg_http_match_uri(&hm, "/upload/#")) {
          if (!authuser(&hm)) {
            mg_http_reply(c, 403, "", "Denied\n");
            c->is_draining = 1;  // Tell mongoose to close this connection
          } else if (hm.body.len > (size_t) s_max_size) {
            mg_http_reply(c, 400, "", "Too long\n");
            c->is_draining = 1;  // Tell mongoose to close this connection
          } else if (hm.uri.len == 8) {  // 8: /upload/
            mg_http_reply(c, 400, "", "Name required\n");
            c->is_draining = 1;  // Tell mongoose to close this connection
          } else if (strlen(s_upld_dir) + (hm.uri.len - 8) + 2 >
                     MG_PATH_MAX) {  // 2: MG_DIRSEP + NUL
            mg_http_reply(c, 400, "", "Path is too long\n");
            c->is_draining = 1;  // Tell mongoose to close this connection
          } else {
            char fpath[MG_PATH_MAX];
            snprintf(fpath, MG_PATH_MAX, "%s%c", s_upld_dir, MG_DIRSEP);
            strncat(fpath, hm.uri.ptr + 8, hm.uri.len - 8);
            if (!mg_path_is_sane(fpath)) {
              mg_http_reply(c, 400, "", "Invalid path\n");
              c->is_draining = 1;  // Tell mongoose to close this connection
            } else {
              MG_DEBUG(("Got request, chunk len %lu", c->recv.len - n));
              if ((fd = mg_fs_open(&mg_fs_posix, fpath, MG_FS_WRITE)) == NULL) {
                mg_http_reply(c, 400, "", "open failed: %d\n", errno);
                c->is_draining = 1;  // Tell mongoose to close this connection
              } else {
                c->fn_data = fd;
                c->recv.len -= n;  // remove headers
                data[0] = hm.body.len;
                data[1] = c->recv.len;
                if (c->recv.len)
                  fd->fs->wr(fd->fd, c->recv.buf + n, c->recv.len);
                c->recv.len = 0;  // consume data
                if (data[1] >= data[0]) {
                  mg_fs_close(fd);
                  mg_http_reply(c, 200, "", "ok\n");
                }
              }
            }
          }
          c->is_resp = 1;  // ignore the rest of the body
        } else {
          struct mg_http_serve_opts opts = {0};
          opts.root_dir = s_root_dir;
          mg_http_serve_dir(c, &hm, &opts);
        }
      }
    }
  }
  (void) ev_data;
}

static void usage(const char *prog) {
  fprintf(stderr,
          "File Transfer server based on Mongoose v.%s\n"
          "Usage: %s OPTIONS\n"
          "  -u NAME   - user name, default: '%s'\n"
          "  -p PWD    - password, default: '%s'\n"
          "  -d DIR    - directory to serve, default: '%s'\n"
          "  -D DIR    - directory to store uploads, default: '%s'\n"
          "  -s SIZE   - maximum allowed file size, default: '%d'\n"
          "  -l ADDR   - listening address, default: '%s'\n"
          "  -v LEVEL  - debug level, from 0 to 4, default: %d\n",
          MG_VERSION, prog, s_user, s_pass, s_root_dir, s_upld_dir, s_max_size,
          s_listening_address, s_debug_level);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  char spath[MG_PATH_MAX] = ".";
  char upath[MG_PATH_MAX] = ".";
  struct mg_mgr mgr;
  int i;

  // Parse command-line flags
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {
      s_root_dir = argv[++i];
    } else if (strcmp(argv[i], "-D") == 0) {
      s_upld_dir = argv[++i];
    } else if (strcmp(argv[i], "-u") == 0) {
      s_user = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0) {
      s_pass = argv[++i];
    } else if (strcmp(argv[i], "-l") == 0) {
      s_listening_address = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0) {
      s_debug_level = atoi(argv[++i]);
    } else if (strcmp(argv[i], "-s") == 0) {
      s_max_size = atoi(argv[++i]);
    } else {
      usage(argv[0]);
    }
  }

  // Root directory must not contain double dots. Make it absolute
  // Do the conversion only if the root dir spec does not contain overrides
  if (strchr(s_root_dir, ',') == NULL) {
    realpath(s_root_dir, spath);
    s_root_dir = spath;
  }
  if (strchr(s_upld_dir, ',') == NULL) {
    realpath(s_upld_dir, upath);
    s_upld_dir = upath;
  }

  // Initialise stuff
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  mg_log_set(s_debug_level);
  mg_mgr_init(&mgr);
  // use mg_listen instead of mg_http_listen to be able to override the parser
  // and shape buffering
  if (mg_listen(&mgr, s_listening_address, cb, NULL) == NULL) {
    MG_ERROR(("Cannot listen on %s.", s_listening_address));
    exit(EXIT_FAILURE);
  }

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", s_listening_address));
  MG_INFO(("Web root         : [%s]", s_root_dir));
  MG_INFO(("Uploading to     : [%s]", s_upld_dir));
  while (s_signo == 0) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  MG_INFO(("Exiting on signal %d", s_signo));
  return 0;
}
