// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const char *s_debug_level = "2";
static const char *s_root_dir = ".";
static const char *s_listening_address = "http://localhost:8000";
static const char *s_enable_hexdump = "no";
static const char *s_ssi_pattern = "#.shtml";

static struct mg_mgr s_mgr;
static struct mg_timer s_timer;

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_serve_opts opts = {s_root_dir, s_ssi_pattern};
    mg_http_serve_dir(c, ev_data, &opts);
  }
  (void) fn_data;
}

static void usage(const char *prog) {
  fprintf(stderr,
          "Mongoose v.%s, built " __DATE__ " " __TIME__
          "\nUsage: %s OPTIONS\n"
          "  -D LEVEL  - debug level, from 0 to 4, default: '%s'\n"
          "  -H yes|no - enable traffic hexdump, default: '%s'\n"
          "  -S GLOB   - glob pattern for SSI files, default: '%s'\n"
          "  -d DIR    - directory to serve, default: '%s'\n"
          "  -l ADDR   - listening address, default: '%s'\n",
          MG_VERSION, prog, s_debug_level, s_enable_hexdump, s_ssi_pattern,
          s_root_dir, s_listening_address);
  exit(EXIT_FAILURE);
}

static void cfg(const char *progname, const char *optname, const char *value) {
  if (value == NULL) {
    usage(progname);
  } else if (strcmp(optname, "-D") == 0) {
    s_debug_level = value;
  } else if (strcmp(optname, "-H") == 0) {
    s_enable_hexdump = value;
  } else if (strcmp(optname, "-S") == 0) {
    s_ssi_pattern = value;
  } else if (strcmp(optname, "-d") == 0) {
    s_root_dir = value;
  } else if (strcmp(optname, "-l") == 0) {
    s_listening_address = value;
  } else {
    usage(progname);
  }
  LOG(LL_INFO, ("Setting option [%s] to [%s]", optname, value));
}

static FILE *opencfg(const char *exe, const char *mode) {
  char path[1024];
  const char *p = strrchr(exe, MG_DIRSEP);
  if (p == NULL) p = strrchr(exe, '/');
  snprintf(path, sizeof(path), "%.*s%c%s", p == NULL ? 1 : (int) (p - exe),
           p == NULL ? "." : exe, MG_DIRSEP, "mongoose.conf");
  return fopen(path, mode);
}

static void parse_config_file(const char *exe) {
  FILE *fp = opencfg(exe, "rb");
  if (fp != NULL) {
    char line[8192], *name;
    int i, line_no = 0;
    LOG(LL_INFO, ("Processing config file..."));
    while (fgets(line, sizeof(line), fp) != NULL) {
      line_no++;
      // Trim whitespaces at the end
      for (i = strlen(line); i > 0 && isspace(line[i - 1]);) line[--i] = '\0';
      // Ignore empty lines and comments
      for (i = 0; isspace(line[i]);) i++;
      if (line[i] == '#' || line[i] == '\0') continue;
      name = &line[i];
      while (line[i] != '\0' && !isspace(line[i])) i++;
      if (line[i] != ' ') {
        LOG(LL_ERROR, ("Ignoring invalid line %d: %s", line_no, line));
      } else {
        line[i++] = '\0';  // nul-terminate name
        while (isspace(line[i])) i++;
        cfg(exe, name, strdup(&line[i]));
      }
    }
    fclose(fp);
  }
}

static void ccb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = ev_data;
    LOG(LL_DEBUG, ("New version: %.*s", (int) hm->body.len, hm->body.ptr));
    c->is_closing = 1;
  }
  (void) fn_data;
}

// Check latest version, alert user if upgrade is available
static void timer_fn(void *arg) {
  struct mg_connection *c =
      mg_http_connect(&s_mgr, "http://cesanta.com", ccb, NULL);
  if (c != NULL)
    mg_printf(c, "%s",
              "GET /downloads/mws/version.json HTTP/1.0\r\n"
              "Host: cesanta.com\r\n"
              "\r\n");
  (void) arg;
}

static void init_mongoose(int argc, char *argv[]) {
  struct mg_connection *c;
  int i;

  // Configure options from a command-line flags and configuration file
  for (i = 1; i < argc; i += 2) cfg(argv[0], argv[i], argv[i + 1]);
  parse_config_file(argv[0]);

  // Initialise stuff
  mg_log_set(s_debug_level);
  mg_mgr_init(&s_mgr);
  if ((c = mg_http_listen(&s_mgr, s_listening_address, cb, &s_mgr)) == NULL) {
    LOG(LL_ERROR, ("Cannot listen on %s. Use http://ADDR:PORT or :PORT",
                   s_listening_address));
    exit(EXIT_FAILURE);
  }
  mg_timer_init(&s_timer, 3600000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn,
                &s_mgr);
  if (mg_casecmp(s_enable_hexdump, "yes") == 0) c->is_hexdumping = 1;
  s_root_dir = realpath(s_root_dir, NULL);
  if (s_root_dir == NULL) s_root_dir = ".";
  LOG(LL_INFO, ("Starting Mongoose v%s, serving %s", MG_VERSION, s_root_dir));
}

#ifdef _WIN32
#include "win32/winmain.c"
#else
int main(int argc, char *argv[]) {
  init_mongoose(argc, argv);
  for (;;) mg_mgr_poll(&s_mgr, 1000);
  mg_timer_free(&s_timer);
  mg_mgr_free(&s_mgr);
  return 0;
}
#endif
