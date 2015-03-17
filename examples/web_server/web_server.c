// Copyright (c) 2004-2013 Sergey Lyubka
// Copyright (c) 2013-2014 Cesanta Software Limited

#undef UNICODE                    // Use ANSI WinAPI functions
#undef _UNICODE                   // Use multibyte encoding on Windows
#define _MBCS                     // Use multibyte encoding on Windows
#define _WIN32_WINNT 0x500        // Enable MIIM_BITMAP
#define _CRT_SECURE_NO_WARNINGS   // Disable deprecation warning in VS2005
#define _XOPEN_SOURCE 600         // For PATH_MAX on linux
#undef WIN32_LEAN_AND_MEAN        // Let windows.h always include winsock2.h

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>

#include "mongoose.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>  // For chdir()
#include <winsvc.h>
#include <shlobj.h>

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#ifndef S_ISDIR
#define S_ISDIR(x) ((x) & _S_IFDIR)
#endif

#define DIRSEP '\\'
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define sleep(x) Sleep((x) * 1000)
#define abs_path(rel, abs, abs_size) _fullpath((abs), (rel), (abs_size))
#define SIGCHLD 0
typedef struct _stat file_stat_t;
#define stat(x, y) _stat((x), (y))
#else
typedef struct stat file_stat_t;
#include <sys/wait.h>
#include <unistd.h>

#ifdef IOS
#include <ifaddrs.h>
#endif

#define DIRSEP '/'
#define __cdecl
#define abs_path(rel, abs, abs_size) realpath((rel), (abs))
#endif // _WIN32

#define MAX_OPTIONS 100
#define MAX_CONF_FILE_LINE_SIZE (8 * 1024)

#ifndef MVER
#define MVER MONGOOSE_VERSION
#endif

static int exit_flag;
static char server_name[50];        // Set by init_server_name()
static char s_config_file[PATH_MAX];  // Set by process_command_line_arguments
static struct mg_server *server;    // Set by start_mongoose()
static const char *s_default_document_root = ".";
static const char *s_default_listening_port = "8080";
static char **s_argv = { NULL };

static void set_options(char *argv[]);

#if !defined(CONFIG_FILE)
#define CONFIG_FILE "mongoose.conf"
#endif /* !CONFIG_FILE */

static void __cdecl signal_handler(int sig_num) {
  // Reinstantiate signal handler
  signal(sig_num, signal_handler);

#ifndef _WIN32
  // Do not do the trick with ignoring SIGCHLD, cause not all OSes (e.g. QNX)
  // reap zombies if SIGCHLD is ignored. On QNX, for example, waitpid()
  // fails if SIGCHLD is ignored, making system() non-functional.
  if (sig_num == SIGCHLD) {
    do {} while (waitpid(-1, &sig_num, WNOHANG) > 0);
  } else
#endif
  { exit_flag = sig_num; }
}

static void vnotify(const char *fmt, va_list ap, int must_exit) {
  vfprintf(stderr, fmt, ap);
  fputc('\n', stderr);
  if (must_exit) {
    exit(EXIT_FAILURE);
  }
}

static void notify(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vnotify(fmt, ap, 0);
  va_end(ap);
}

static void die(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vnotify(fmt, ap, 1);
  va_end(ap);
}

static void show_usage_and_exit(void) {
  const char **names;
  int i;

  fprintf(stderr, "Mongoose version %s (c) Sergey Lyubka, built on %s\n",
          MVER, __DATE__);
  fprintf(stderr, "Usage:\n");
#if !defined(MONGOOSE_NO_AUTH) && !defined(MONGOOSE_NO_FILESYSTEM)
  fprintf(stderr, "  mongoose -A <htpasswd_file> <realm> <user> <passwd>\n");
#endif
  fprintf(stderr, "  mongoose [config_file]\n");
  fprintf(stderr, "  mongoose [-option value ...]\n");
  fprintf(stderr, "\nOPTIONS:\n");

  names = mg_get_valid_option_names();
  for (i = 0; names[i] != NULL; i += 2) {
    fprintf(stderr, "  -%s %s\n",
            names[i], names[i + 1] == NULL ? "<empty>" : names[i + 1]);
  }
  exit(EXIT_FAILURE);
}

#define EV_HANDLER NULL

static char *sdup(const char *str) {
  char *p;
  if ((p = (char *) malloc(strlen(str) + 1)) != NULL) {
    strcpy(p, str);
  }
  return p;
}

static void set_option(char **options, const char *name, const char *value) {
  int i;

  for (i = 0; i < MAX_OPTIONS - 3; i++) {
    if (options[i] == NULL) {
      options[i] = sdup(name);
      options[i + 1] = sdup(value);
      options[i + 2] = NULL;
      break;
    } else if (!strcmp(options[i], name)) {
      free(options[i + 1]);
      options[i + 1] = sdup(value);
      break;
    }
  }

  if (i == MAX_OPTIONS - 3) {
    die("%s", "Too many options specified");
  }
}

static void process_command_line_arguments(char *argv[], char **options) {
  char line[MAX_CONF_FILE_LINE_SIZE], opt[sizeof(line)], val[sizeof(line)],
       *p, cpath[PATH_MAX];
  FILE *fp = NULL;
  size_t i, cmd_line_opts_start = 1, line_no = 0;

  // Should we use a config file ?
  if (argv[1] != NULL && argv[1][0] != '-') {
    snprintf(cpath, sizeof(cpath), "%s", argv[1]);
    cmd_line_opts_start = 2;
  } else if ((p = strrchr(argv[0], DIRSEP)) == NULL) {
    // No command line flags specified. Look where binary lives
    snprintf(cpath, sizeof(cpath), "%s", CONFIG_FILE);
  } else {
    snprintf(cpath, sizeof(cpath), "%.*s%c%s",
             (int) (p - argv[0]), argv[0], DIRSEP, CONFIG_FILE);
  }
  abs_path(cpath, s_config_file, sizeof(s_config_file));

  fp = fopen(s_config_file, "r");

  // If config file was set in command line and open failed, die
  if (cmd_line_opts_start == 2 && fp == NULL) {
    die("Cannot open config file %s: %s", s_config_file, strerror(errno));
  }

  // Load config file settings first
  if (fp != NULL) {
    fprintf(stderr, "Loading config file %s\n", s_config_file);

    // Loop over the lines in config file
    while (fgets(line, sizeof(line), fp) != NULL) {
      line_no++;

      // Ignore empty lines and comments
      for (i = 0; isspace(* (unsigned char *) &line[i]); ) i++;
      if (line[i] == '#' || line[i] == '\0') {
        continue;
      }

      if (sscanf(line, "%s %[^\r\n#]", opt, val) != 2) {
        printf("%s: line %d is invalid, ignoring it:\n %s",
               s_config_file, (int) line_no, line);
      } else {
        set_option(options, opt, val);
      }
    }

    fclose(fp);
  }

  // If we're under MacOS and started by launchd, then the second
  // argument is process serial number, -psn_.....
  // In this case, don't process arguments at all.
  if (argv[1] == NULL || memcmp(argv[1], "-psn_", 5) != 0) {
    // Handle command line flags.
    // They override config file and default settings.
    for (i = cmd_line_opts_start; argv[i] != NULL; i += 2) {
      if (argv[i][0] != '-' || argv[i + 1] == NULL) {
        show_usage_and_exit();
      }
      set_option(options, &argv[i][1], argv[i + 1]);
    }
  }
}

static void init_server_name(void) {
  const char *descr = "";
  snprintf(server_name, sizeof(server_name), "Mongoose web server v.%s%s",
           MVER, descr);
}

static int is_path_absolute(const char *path) {
#ifdef _WIN32
  return path != NULL &&
    ((path[0] == '\\' && path[1] == '\\') ||  // UNC path, e.g. \\server\dir
     (isalpha(path[0]) && path[1] == ':' && path[2] == '\\'));  // E.g. X:\dir
#else
  return path != NULL && path[0] == '/';
#endif
}

static char *get_option(char **options, const char *option_name) {
  int i;

  for (i = 0; options[i] != NULL; i++)
    if (!strcmp(options[i], option_name))
      return options[i + 1];

  return NULL;
}

static void *serving_thread_func(void *param) {
  struct mg_server *srv = (struct mg_server *) param;
  while (exit_flag == 0) {
    mg_poll_server(srv, 1000);
  }
  return NULL;
}

static int path_exists(const char *path, int is_dir) {
  file_stat_t st;
  return path == NULL || (stat(path, &st) == 0 &&
                          ((S_ISDIR(st.st_mode) ? 1 : 0) == is_dir));
}

static void verify_existence(char **options, const char *name, int is_dir) {
  const char *path = get_option(options, name);
  if (!path_exists(path, is_dir)) {
    notify("Invalid path for %s: [%s]: (%s). Make sure that path is either "
           "absolute, or it is relative to mongoose executable.",
           name, path, strerror(errno));
  }
}

static void set_absolute_path(char *options[], const char *option_name) {
  char path[PATH_MAX], abs[PATH_MAX], *option_value;
  const char *p;

  // Check whether option is already set
  option_value = get_option(options, option_name);

  // If option is already set and it is an absolute path,
  // leave it as it is -- it's already absolute.
  if (option_value != NULL && !is_path_absolute(option_value)) {
    // Not absolute. Use the directory where mongoose executable lives
    // be the relative directory for everything.
    // Extract mongoose executable directory into path.
    if ((p = strrchr(s_config_file, DIRSEP)) == NULL) {
      getcwd(path, sizeof(path));
    } else {
      snprintf(path, sizeof(path), "%.*s", (int) (p - s_config_file),
               s_config_file);
    }

    strncat(path, "/", sizeof(path) - 1);
    strncat(path, option_value, sizeof(path) - 1);

    // Absolutize the path, and set the option
    abs_path(path, abs, sizeof(abs));
    set_option(options, option_name, abs);
  }
}

#if !defined(MONGOOSE_NO_AUTH) && !defined(MONGOOSE_NO_FILESYSTEM)
int modify_passwords_file(const char *fname, const char *domain,
                          const char *user, const char *pass) {
  int found;
  char line[512], u[512], d[512], ha1[33], tmp[PATH_MAX];
  FILE *fp, *fp2;

  found = 0;
  fp = fp2 = NULL;

  // Regard empty password as no password - remove user record.
  if (pass != NULL && pass[0] == '\0') {
    pass = NULL;
  }

  (void) snprintf(tmp, sizeof(tmp), "%s.tmp", fname);

  // Create the file if does not exist
  if ((fp = fopen(fname, "a+")) != NULL) {
    fclose(fp);
  }

  // Open the given file and temporary file
  if ((fp = fopen(fname, "r")) == NULL) {
    return 0;
  } else if ((fp2 = fopen(tmp, "w+")) == NULL) {
    fclose(fp);
    return 0;
  }

  // Copy the stuff to temporary file
  while (fgets(line, sizeof(line), fp) != NULL) {
    if (sscanf(line, "%[^:]:%[^:]:%*s", u, d) != 2) {
      continue;
    }

    if (!strcmp(u, user) && !strcmp(d, domain)) {
      found++;
      if (pass != NULL) {
        mg_md5(ha1, user, ":", domain, ":", pass, NULL);
        fprintf(fp2, "%s:%s:%s\n", user, domain, ha1);
      }
    } else {
      fprintf(fp2, "%s", line);
    }
  }

  // If new user, just add it
  if (!found && pass != NULL) {
    mg_md5(ha1, user, ":", domain, ":", pass, NULL);
    fprintf(fp2, "%s:%s:%s\n", user, domain, ha1);
  }

  // Close files
  fclose(fp);
  fclose(fp2);

  // Put the temp file in place of real file
  remove(fname);
  rename(tmp, fname);

  return 1;
}
#endif

static void start_mongoose(int argc, char *argv[]) {
  s_argv = argv;
  if ((server = mg_create_server(NULL, EV_HANDLER)) == NULL) {
    die("%s", "Failed to start Mongoose.");
  }

#if !defined(MONGOOSE_NO_AUTH) && !defined(MONGOOSE_NO_FILESYSTEM)
  // Edit passwords file if -A option is specified
  if (argc > 1 && !strcmp(argv[1], "-A")) {
    if (argc != 6) {
      show_usage_and_exit();
    }
    exit(modify_passwords_file(argv[2], argv[3], argv[4], argv[5]) ?
         EXIT_SUCCESS : EXIT_FAILURE);
  }
#endif

  // Show usage if -h or --help options are specified
  if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
    show_usage_and_exit();
  }
  set_options(argv);
}

static void set_options(char *argv[]) {
  char *options[MAX_OPTIONS];
  int i;

  options[0] = NULL;
  set_option(options, "document_root", s_default_document_root);
  set_option(options, "listening_port", s_default_listening_port);

  // Update config based on command line arguments
  process_command_line_arguments(argv, options);

  // Make sure we have absolute paths for files and directories
  // https://github.com/valenok/mongoose/issues/181
  set_absolute_path(options, "document_root");
  set_absolute_path(options, "dav_auth_file");
  set_absolute_path(options, "cgi_interpreter");
  set_absolute_path(options, "access_log_file");
  set_absolute_path(options, "global_auth_file");
  set_absolute_path(options, "ssl_certificate");

  if (!path_exists(get_option(options, "document_root"), 1)) {
    set_option(options, "document_root", s_default_document_root);
    set_absolute_path(options, "document_root");
    notify("Setting document_root to [%s]",
           mg_get_option(server, "document_root"));
  }

  // Make extra verification for certain options
  verify_existence(options, "document_root", 1);
  verify_existence(options, "cgi_interpreter", 0);
  verify_existence(options, "ssl_certificate", 0);

  for (i = 0; options[i] != NULL; i += 2) {
    const char *msg = mg_set_option(server, options[i], options[i + 1]);
    if (msg != NULL) {
      notify("Failed to set option [%s] to [%s]: %s",
             options[i], options[i + 1], msg);
      if (!strcmp(options[i], "listening_port")) {
        mg_set_option(server, "listening_port", s_default_listening_port);
        notify("Setting %s to [%s]", options[i], s_default_listening_port);
      }
    }
    free(options[i]);
    free(options[i + 1]);
  }

  // Change current working directory to document root. This way,
  // scripts can use relative paths.
  chdir(mg_get_option(server, "document_root"));

#if 0
  // Add an ability to pass listening socket to mongoose
  {
    const char *env = getenv("MONGOOSE_LISTENING_SOCKET");
    if (env != NULL && atoi(env) > 0 ) {
      mg_set_listening_socket(server, atoi(env));
    }
  }
#endif

  // Setup signal handler: quit on Ctrl-C
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
#ifndef _WIN32
  signal(SIGCHLD, signal_handler);
#endif
}

int main(int argc, char *argv[]) {
  init_server_name();
  start_mongoose(argc, argv);
  printf("%s serving [%s] on port %s\n",
         server_name, mg_get_option(server, "document_root"),
         mg_get_option(server, "listening_port"));
  fflush(stdout);  // Needed, Windows terminals might not be line-buffered
  serving_thread_func(server);
  printf("Exiting on signal %d ...", exit_flag);
  fflush(stdout);
  mg_destroy_server(&server);
  printf("%s\n", " done.");

  return EXIT_SUCCESS;
}
