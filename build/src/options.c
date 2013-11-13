#include "internal.h"

// This array must be in sync with enum in internal.h
static const char *config_options[] = {
  "cgi_pattern", "**.cgi$|**.pl$|**.php$",
  "cgi_environment", NULL,
  "put_delete_auth_file", NULL,
  "cgi_interpreter", NULL,
  "protect_uri", NULL,
  "authentication_domain", "mydomain.com",
  "ssi_pattern", "**.shtml$|**.shtm$",
  "throttle", NULL,
  "access_log_file", NULL,
  "enable_directory_listing", "yes",
  "error_log_file", NULL,
  "global_auth_file", NULL,
  "index_files",
    "index.html,index.htm,index.cgi,index.shtml,index.php,index.lp",
  "enable_keep_alive", "no",
  "access_control_list", NULL,
  "extra_mime_types", NULL,
  "listening_ports", "8080",
  "document_root",  NULL,
  "ssl_certificate", NULL,
  "num_threads", "50",
  "run_as_user", NULL,
  "url_rewrite_patterns", NULL,
  "hide_files_patterns", NULL,
  "request_timeout_ms", "30000",
  NULL
};

const char **mg_get_valid_option_names(void) {
  return config_options;
}

static int get_option_index(const char *name) {
  int i;

  for (i = 0; config_options[i * 2] != NULL; i++) {
    if (strcmp(config_options[i * 2], name) == 0) {
      return i;
    }
  }
  return -1;
}

const char *mg_get_option(const struct mg_context *ctx, const char *name) {
  int i;
  if ((i = get_option_index(name)) == -1) {
    return NULL;
  } else if (ctx->config[i] == NULL) {
    return "";
  } else {
    return ctx->config[i];
  }
}
