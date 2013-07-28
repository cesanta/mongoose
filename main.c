// Copyright (c) 2004-2013 Sergey Lyubka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS  // Disable deprecation warning in VS2005
#else
#define _XOPEN_SOURCE 600  // For PATH_MAX on linux
#endif

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

#include "mongoose.h"

#ifdef _WIN32
#include <windows.h>
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
#define WINCDECL __cdecl
#define abs_path(rel, abs, abs_size) _fullpath((abs), (rel), (abs_size))
#else
#include <sys/wait.h>
#include <unistd.h>
#define DIRSEP '/'
#define WINCDECL
#define abs_path(rel, abs, abs_size) realpath((rel), (abs))
#endif // _WIN32

#define MAX_OPTIONS 100
#define MAX_CONF_FILE_LINE_SIZE (8 * 1024)

static int exit_flag;
static char server_name[40];        // Set by init_server_name()
static char config_file[PATH_MAX];  // Set by process_command_line_arguments()
static struct mg_context *ctx;      // Set by start_mongoose()

#if !defined(CONFIG_FILE)
#define CONFIG_FILE "mongoose.conf"
#endif /* !CONFIG_FILE */

static void WINCDECL signal_handler(int sig_num) {
  exit_flag = sig_num;
}

static void die(const char *fmt, ...) {
  va_list ap;
  char msg[200];

  va_start(ap, fmt);
  vsnprintf(msg, sizeof(msg), fmt, ap);
  va_end(ap);

#if defined(_WIN32)
  MessageBox(NULL, msg, "Error", MB_OK);
#else
  fprintf(stderr, "%s\n", msg);
#endif

  exit(EXIT_FAILURE);
}

static void show_usage_and_exit(void) {
  const char **names;
  int i;

  fprintf(stderr, "Mongoose version %s (c) Sergey Lyubka, built on %s\n",
          mg_version(), __DATE__);
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  mongoose -A <htpasswd_file> <realm> <user> <passwd>\n");
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

#if defined(_WIN32) || defined(USE_COCOA)
static const char *config_file_top_comment =
"# Mongoose web server configuration file.\n"
"# For detailed description of every option, visit\n"
"# https://github.com/valenok/mongoose/blob/master/UserManual.md\n"
"# Lines starting with '#' and empty lines are ignored.\n"
"# To make a change, remove leading '#', modify option's value,\n"
"# save this file and then restart Mongoose.\n\n";

static const char *get_url_to_first_open_port(const struct mg_context *ctx) {
  static char url[100];
  const char *open_ports = mg_get_option(ctx, "listening_ports");
  int a, b, c, d, port, n;

  if (sscanf(open_ports, "%d.%d.%d.%d:%d%n", &a, &b, &c, &d, &port, &n) == 5) {
    snprintf(url, sizeof(url), "%s://%d.%d.%d.%d:%d",
             open_ports[n] == 's' ? "https" : "http", a, b, c, d, port);
  } else if (sscanf(open_ports, "%d%n", &port, &n) == 1) {
    snprintf(url, sizeof(url), "%s://localhost:%d",
             open_ports[n] == 's' ? "https" : "http", port);
  } else {
    snprintf(url, sizeof(url), "%s", "http://localhost:8080");
  }

  return url;
}

static void create_config_file(const char *path) {
  const char **names, *value;
  FILE *fp;
  int i;

  // Create config file if it is not present yet
  if ((fp = fopen(path, "r")) != NULL) {
    fclose(fp);
  } else if ((fp = fopen(path, "a+")) != NULL) {
    fprintf(fp, "%s", config_file_top_comment);
    names = mg_get_valid_option_names();
    for (i = 0; names[i * 2] != NULL; i++) {
      value = mg_get_option(ctx, names[i * 2]);
      fprintf(fp, "# %s %s\n", names[i * 2], value ? value : "<value>");
    }
    fclose(fp);
  }
}
#endif

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
  char line[MAX_CONF_FILE_LINE_SIZE], opt[sizeof(line)], val[sizeof(line)], *p;
  FILE *fp = NULL;
  size_t i, cmd_line_opts_start = 1, line_no = 0;

  // Should we use a config file ?
  if (argv[1] != NULL && argv[1][0] != '-') {
    snprintf(config_file, sizeof(config_file), "%s", argv[1]);
    cmd_line_opts_start = 2;
  } else if ((p = strrchr(argv[0], DIRSEP)) == NULL) {
    // No command line flags specified. Look where binary lives
    snprintf(config_file, sizeof(config_file), "%s", CONFIG_FILE);
  } else {
    snprintf(config_file, sizeof(config_file), "%.*s%c%s",
             (int) (p - argv[0]), argv[0], DIRSEP, CONFIG_FILE);
  }

  fp = fopen(config_file, "r");

  // If config file was set in command line and open failed, die
  if (cmd_line_opts_start == 2 && fp == NULL) {
    die("Cannot open config file %s: %s", config_file, strerror(errno));
  }

  // Load config file settings first
  if (fp != NULL) {
    fprintf(stderr, "Loading config file %s\n", config_file);

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
               config_file, (int) line_no, line);
      } else {
        set_option(options, opt, val);
      }
    }

    (void) fclose(fp);
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
  snprintf(server_name, sizeof(server_name), "Mongoose web server v.%s",
           mg_version());
}

static int log_message(const struct mg_connection *conn, const char *message) {
  (void) conn;
  printf("%s\n", message);
  return 0;
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

static void verify_existence(char **options, const char *option_name,
                             int must_be_dir) {
  struct stat st;
  const char *path = get_option(options, option_name);

  if (path != NULL && (stat(path, &st) != 0 ||
                       ((S_ISDIR(st.st_mode) ? 1 : 0) != must_be_dir))) {
    die("Invalid path for %s: [%s]: (%s). Make sure that path is either "
        "absolute, or it is relative to mongoose executable.",
        option_name, path, strerror(errno));
  }
}

static void set_absolute_path(char *options[], const char *option_name,
                              const char *path_to_mongoose_exe) {
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
    if ((p = strrchr(path_to_mongoose_exe, DIRSEP)) == NULL) {
      getcwd(path, sizeof(path));
    } else {
      snprintf(path, sizeof(path), "%.*s", (int) (p - path_to_mongoose_exe),
               path_to_mongoose_exe);
    }

    strncat(path, "/", sizeof(path) - 1);
    strncat(path, option_value, sizeof(path) - 1);

    // Absolutize the path, and set the option
    abs_path(path, abs, sizeof(abs));
    set_option(options, option_name, abs);
  }
}

static void start_mongoose(int argc, char *argv[]) {
  struct mg_callbacks callbacks;
  char *options[MAX_OPTIONS];
  int i;

  // Edit passwords file if -A option is specified
  if (argc > 1 && !strcmp(argv[1], "-A")) {
    if (argc != 6) {
      show_usage_and_exit();
    }
    exit(mg_modify_passwords_file(argv[2], argv[3], argv[4], argv[5]) ?
         EXIT_SUCCESS : EXIT_FAILURE);
  }

  // Show usage if -h or --help options are specified
  if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
    show_usage_and_exit();
  }

  options[0] = NULL;
  set_option(options, "document_root", ".");

  // Update config based on command line arguments
  process_command_line_arguments(argv, options);

  // Make sure we have absolute paths for files and directories
  // https://github.com/valenok/mongoose/issues/181
  set_absolute_path(options, "document_root", argv[0]);
  set_absolute_path(options, "put_delete_auth_file", argv[0]);
  set_absolute_path(options, "cgi_interpreter", argv[0]);
  set_absolute_path(options, "access_log_file", argv[0]);
  set_absolute_path(options, "error_log_file", argv[0]);
  set_absolute_path(options, "global_auth_file", argv[0]);
  set_absolute_path(options, "ssl_certificate", argv[0]);

  // Make extra verification for certain options
  verify_existence(options, "document_root", 1);
  verify_existence(options, "cgi_interpreter", 0);
  verify_existence(options, "ssl_certificate", 0);

  // Setup signal handler: quit on Ctrl-C
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  // Start Mongoose
  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.log_message = &log_message;
  ctx = mg_start(&callbacks, NULL, (const char **) options);
  for (i = 0; options[i] != NULL; i++) {
    free(options[i]);
  }

  if (ctx == NULL) {
    die("%s", "Failed to start Mongoose.");
  }
}

#ifdef _WIN32
enum {
  ID_ICON = 100, ID_QUIT, ID_SETTINGS, ID_SEPARATOR, ID_INSTALL_SERVICE,
  ID_REMOVE_SERVICE, ID_STATIC, ID_GROUP, ID_SAVE, ID_RESET_DEFAULTS,
  ID_STATUS, ID_CONNECT,

  // All dynamically created text boxes for options have IDs starting from
  // ID_CONTROLS, incremented by one.
  ID_CONTROLS = 200,

  // Text boxes for files have "..." buttons to open file browser. These
  // buttons have IDs that are ID_FILE_BUTTONS_DELTA higher than associated
  // text box ID.
  ID_FILE_BUTTONS_DELTA = 1000
};
static HICON hIcon;
static SERVICE_STATUS ss;
static SERVICE_STATUS_HANDLE hStatus;
static const char *service_magic_argument = "--";
static NOTIFYICONDATA TrayIcon;

static void WINAPI ControlHandler(DWORD code) {
  if (code == SERVICE_CONTROL_STOP || code == SERVICE_CONTROL_SHUTDOWN) {
    ss.dwWin32ExitCode = 0;
    ss.dwCurrentState = SERVICE_STOPPED;
  }
  SetServiceStatus(hStatus, &ss);
}

static void WINAPI ServiceMain(void) {
  ss.dwServiceType = SERVICE_WIN32;
  ss.dwCurrentState = SERVICE_RUNNING;
  ss.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

  hStatus = RegisterServiceCtrlHandler(server_name, ControlHandler);
  SetServiceStatus(hStatus, &ss);

  while (ss.dwCurrentState == SERVICE_RUNNING) {
    Sleep(1000);
  }
  mg_stop(ctx);

  ss.dwCurrentState = SERVICE_STOPPED;
  ss.dwWin32ExitCode = (DWORD) -1;
  SetServiceStatus(hStatus, &ss);
}


static void show_error(void) {
  char buf[256];
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                buf, sizeof(buf), NULL);
  MessageBox(NULL, buf, "Error", MB_OK);
}

static void *align(void *ptr, DWORD alig) {
  ULONG ul = (ULONG) ptr;
  ul += alig;
  ul &= ~alig;
  return ((void *) ul);
}

static int is_boolean_option(const char *option_name) {
  return !strcmp(option_name, "enable_directory_listing") ||
    !strcmp(option_name, "enable_keep_alive");
}

static int is_filename_option(const char *option_name) {
  return !strcmp(option_name, "cgi_interpreter") ||
    !strcmp(option_name, "global_auth_file") ||
    !strcmp(option_name, "put_delete_auth_file") ||
    !strcmp(option_name, "access_log_file") ||
    !strcmp(option_name, "error_log_file") ||
    !strcmp(option_name, "ssl_certificate");
}

static int is_directory_option(const char *option_name) {
  return !strcmp(option_name, "document_root");
}

static int is_numeric_options(const char *option_name) {
  return !strcmp(option_name, "num_threads");
}

static void save_config(HWND hDlg, FILE *fp) {
  char value[2000];
  const char **options, *name, *default_value;
  int i, id;

  fprintf(fp, "%s", config_file_top_comment);
  options = mg_get_valid_option_names();
  for (i = 0; options[i * 2] != NULL; i++) {
    name = options[i * 2];
    id = ID_CONTROLS + i;
    if (is_boolean_option(name)) {
      snprintf(value, sizeof(value), "%s",
               IsDlgButtonChecked(hDlg, id) ? "yes" : "no");
    } else {
      GetDlgItemText(hDlg, id, value, sizeof(value));
    }
    default_value = options[i * 2 + 1] == NULL ? "" : options[i * 2 + 1];
    // If value is the same as default, skip it
    if (strcmp(value, default_value) != 0) {
      fprintf(fp, "%s %s\n", name, value);
    }
  }
}

static BOOL CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lP) {
  FILE *fp;
  int i;
  const char *name, *value, **options = mg_get_valid_option_names();

  switch (msg) {
    case WM_CLOSE:
      DestroyWindow(hDlg);
      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case ID_SAVE:
          EnableWindow(GetDlgItem(hDlg, ID_SAVE), FALSE);
          if ((fp = fopen(config_file, "w+")) != NULL) {
            save_config(hDlg, fp);
            fclose(fp);
            mg_stop(ctx);
            start_mongoose(__argc, __argv);
          }
          EnableWindow(GetDlgItem(hDlg, ID_SAVE), TRUE);
          break;
        case ID_RESET_DEFAULTS:
          for (i = 0; options[i * 2] != NULL; i++) {
            name = options[i * 2];
            value = options[i * 2 + 1] == NULL ? "" : options[i * 2 + 1];
            if (is_boolean_option(name)) {
              CheckDlgButton(hDlg, ID_CONTROLS + i, !strcmp(value, "yes") ?
                             BST_CHECKED : BST_UNCHECKED);
            } else {
              SetWindowText(GetDlgItem(hDlg, ID_CONTROLS + i), value);
            }
          }
          break;
      }

      for (i = 0; options[i * 2] != NULL; i++) {
        name = options[i * 2];
        if ((is_filename_option(name) || is_directory_option(name)) &&
            LOWORD(wParam) == ID_CONTROLS + i + ID_FILE_BUTTONS_DELTA) {
          OPENFILENAME of;
          BROWSEINFO bi;
          char path[PATH_MAX] = "";

          memset(&of, 0, sizeof(of));
          of.lStructSize = sizeof(of);
          of.hwndOwner = (HWND) hDlg;
          of.lpstrFile = path;
          of.nMaxFile = sizeof(path);
          of.lpstrInitialDir = mg_get_option(ctx, "document_root");
          of.Flags = OFN_CREATEPROMPT | OFN_NOCHANGEDIR;

          memset(&bi, 0, sizeof(bi));
          bi.hwndOwner = (HWND) hDlg;
          bi.lpszTitle = "Choose WWW root directory:";
          bi.ulFlags = BIF_RETURNONLYFSDIRS;

          if (is_directory_option(name)) {
            SHGetPathFromIDList(SHBrowseForFolder(&bi), path);
          } else {
            GetOpenFileName(&of);
          }

          if (path[0] != '\0') {
            SetWindowText(GetDlgItem(hDlg, ID_CONTROLS + i), path);
          }
        }
      }

      break;

    case WM_INITDIALOG:
      SendMessage(hDlg, WM_SETICON,(WPARAM) ICON_SMALL, (LPARAM) hIcon);
      SendMessage(hDlg, WM_SETICON,(WPARAM) ICON_BIG, (LPARAM) hIcon);
      SetWindowText(hDlg, "Mongoose settings");
      SetFocus(GetDlgItem(hDlg, ID_SAVE));
      for (i = 0; options[i * 2] != NULL; i++) {
        name = options[i * 2];
        value = mg_get_option(ctx, name);
        if (is_boolean_option(name)) {
          CheckDlgButton(hDlg, ID_CONTROLS + i, !strcmp(value, "yes") ?
                         BST_CHECKED : BST_UNCHECKED);
        } else {
          SetDlgItemText(hDlg, ID_CONTROLS + i, value == NULL ? "" : value);
        }
      }
      break;
    default:
      break;
  }

  return FALSE;
}

static void add_control(unsigned char **mem, DLGTEMPLATE *dia, WORD type,
                        DWORD id, DWORD style, WORD x, WORD y,
                        WORD cx, WORD cy, const char *caption) {
  DLGITEMTEMPLATE *tp;
  LPWORD p;

  dia->cdit++;

  *mem = align(*mem, 3);
  tp = (DLGITEMTEMPLATE *) *mem;

  tp->id = (WORD)id;
  tp->style = style;
  tp->dwExtendedStyle = 0;
  tp->x = x;
  tp->y = y;
  tp->cx = cx;
  tp->cy = cy;

  p = align(*mem + sizeof(*tp), 1);
  *p++ = 0xffff;
  *p++ = type;

  while (*caption != '\0') {
    *p++ = (WCHAR) *caption++;
  }
  *p++ = 0;
  p = align(p, 1);

  *p++ = 0;
  *mem = (unsigned char *) p;
}

static void show_settings_dialog() {
#define HEIGHT 15
#define WIDTH 400
#define LABEL_WIDTH 80

  unsigned char mem[4096], *p;
  const char **option_names, *long_option_name;
  DWORD style;
  DLGTEMPLATE *dia = (DLGTEMPLATE *) mem;
  WORD i, cl, x, y, width, nelems = 0;
  static int guard;

  static struct {
    DLGTEMPLATE template; // 18 bytes
    WORD menu, class;
    wchar_t caption[1];
    WORD fontsiz;
    wchar_t fontface[7];
  } dialog_header = {{WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_VISIBLE |
    DS_SETFONT | WS_DLGFRAME, WS_EX_TOOLWINDOW, 0, 200, 200, WIDTH, 0},
    0, 0, L"", 8, L"Tahoma"};

  if (guard == 0) {
    guard++;
  } else {
    return;
  }

  (void) memset(mem, 0, sizeof(mem));
  (void) memcpy(mem, &dialog_header, sizeof(dialog_header));
  p = mem + sizeof(dialog_header);

  option_names = mg_get_valid_option_names();
  for (i = 0; option_names[i * 2] != NULL; i++) {
    long_option_name = option_names[i * 2];
    style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
    x = 10 + (WIDTH / 2) * (nelems % 2);
    y = (nelems/2 + 1) * HEIGHT + 5;
    width = WIDTH / 2 - 20 - LABEL_WIDTH;
    if (is_numeric_options(long_option_name)) {
      style |= ES_NUMBER;
      cl = 0x81;
      style |= WS_BORDER | ES_AUTOHSCROLL;
    } else if (is_boolean_option(long_option_name)) {
      cl = 0x80;
      style |= BS_AUTOCHECKBOX;
    } else if (is_filename_option(long_option_name) ||
               is_directory_option(long_option_name)) {
      style |= WS_BORDER | ES_AUTOHSCROLL;
      width -= 20;
      cl = 0x81;
      add_control(&p, dia, 0x80,
                  ID_CONTROLS + i + ID_FILE_BUTTONS_DELTA,
                  WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                  (WORD) (x + width + LABEL_WIDTH + 5),
                  y, 15, 12, "...");
    } else {
      cl = 0x81;
      style |= WS_BORDER | ES_AUTOHSCROLL;
    }
    add_control(&p, dia, 0x82, ID_STATIC, WS_VISIBLE | WS_CHILD,
                x, y, LABEL_WIDTH, HEIGHT, long_option_name);
    add_control(&p, dia, cl, ID_CONTROLS + i, style,
                (WORD) (x + LABEL_WIDTH), y, width, 12, "");
    nelems++;
  }

  y = (WORD) (((nelems + 1) / 2 + 1) * HEIGHT + 5);
  add_control(&p, dia, 0x80, ID_GROUP, WS_CHILD | WS_VISIBLE |
              BS_GROUPBOX, 5, 5, WIDTH - 10, y, " Settings ");
  y += 10;
  add_control(&p, dia, 0x80, ID_SAVE,
              WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
              WIDTH - 70, y, 65, 12, "Save Settings");
  add_control(&p, dia, 0x80, ID_RESET_DEFAULTS,
              WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
              WIDTH - 140, y, 65, 12, "Reset to defaults");
  add_control(&p, dia, 0x82, ID_STATIC,
              WS_CHILD | WS_VISIBLE | WS_DISABLED,
              5, y, 180, 12, server_name);

  dia->cy = ((nelems + 1) / 2 + 1) * HEIGHT + 30;
  DialogBoxIndirectParam(NULL, dia, NULL, DlgProc, (LPARAM) NULL);
  guard--;
}

static int manage_service(int action) {
  static const char *service_name = "Mongoose";
  SC_HANDLE hSCM = NULL, hService = NULL;
  SERVICE_DESCRIPTION descr = {server_name};
  char path[PATH_MAX + 20];  // Path to executable plus magic argument
  int success = 1;

  if ((hSCM = OpenSCManager(NULL, NULL, action == ID_INSTALL_SERVICE ?
                            GENERIC_WRITE : GENERIC_READ)) == NULL) {
    success = 0;
    show_error();
  } else if (action == ID_INSTALL_SERVICE) {
    GetModuleFileName(NULL, path, sizeof(path));
    strncat(path, " ", sizeof(path));
    strncat(path, service_magic_argument, sizeof(path));
    hService = CreateService(hSCM, service_name, service_name,
                             SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
                             SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
                             path, NULL, NULL, NULL, NULL, NULL);
    if (hService) {
      ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &descr);
    } else {
      show_error();
    }
  } else if (action == ID_REMOVE_SERVICE) {
    if ((hService = OpenService(hSCM, service_name, DELETE)) == NULL ||
        !DeleteService(hService)) {
      show_error();
    }
  } else if ((hService = OpenService(hSCM, service_name,
                                     SERVICE_QUERY_STATUS)) == NULL) {
    success = 0;
  }

  CloseServiceHandle(hService);
  CloseServiceHandle(hSCM);

  return success;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam,
                                   LPARAM lParam) {
  static SERVICE_TABLE_ENTRY service_table[] = {
    {server_name, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
    {NULL, NULL}
  };
  int service_installed;
  char buf[200], *service_argv[] = {__argv[0], NULL};
  POINT pt;
  HMENU hMenu;
  static UINT s_uTaskbarRestart; // for taskbar creation

  switch (msg) {
    case WM_CREATE:
      if (__argv[1] != NULL &&
          !strcmp(__argv[1], service_magic_argument)) {
        start_mongoose(1, service_argv);
        StartServiceCtrlDispatcher(service_table);
        exit(EXIT_SUCCESS);
      } else {
        start_mongoose(__argc, __argv);
        s_uTaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));
      }
      break;
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case ID_QUIT:
          mg_stop(ctx);
          Shell_NotifyIcon(NIM_DELETE, &TrayIcon);
          PostQuitMessage(0);
          return 0;
        case ID_SETTINGS:
          show_settings_dialog();
          break;
        case ID_INSTALL_SERVICE:
        case ID_REMOVE_SERVICE:
          manage_service(LOWORD(wParam));
          break;
        case ID_CONNECT:
          printf("[%s]\n", get_url_to_first_open_port(ctx));
          ShellExecute(NULL, "open", get_url_to_first_open_port(ctx),
                       NULL, NULL, SW_SHOW);
          break;
      }
      break;
    case WM_USER:
      switch (lParam) {
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
          hMenu = CreatePopupMenu();
          AppendMenu(hMenu, MF_STRING | MF_GRAYED, ID_SEPARATOR, server_name);
          AppendMenu(hMenu, MF_SEPARATOR, ID_SEPARATOR, "");
          service_installed = manage_service(0);
          snprintf(buf, sizeof(buf), "NT service: %s installed",
                   service_installed ? "" : "not");
          AppendMenu(hMenu, MF_STRING | MF_GRAYED, ID_SEPARATOR, buf);
          AppendMenu(hMenu, MF_STRING | (service_installed ? MF_GRAYED : 0),
                     ID_INSTALL_SERVICE, "Install service");
          AppendMenu(hMenu, MF_STRING | (!service_installed ? MF_GRAYED : 0),
                     ID_REMOVE_SERVICE, "Deinstall service");
          AppendMenu(hMenu, MF_SEPARATOR, ID_SEPARATOR, "");
          AppendMenu(hMenu, MF_STRING, ID_CONNECT, "Start browser");
          AppendMenu(hMenu, MF_STRING, ID_SETTINGS, "Edit Settings");
          AppendMenu(hMenu, MF_SEPARATOR, ID_SEPARATOR, "");
          AppendMenu(hMenu, MF_STRING, ID_QUIT, "Exit");
          GetCursorPos(&pt);
          SetForegroundWindow(hWnd);
          TrackPopupMenu(hMenu, 0, pt.x, pt.y, 0, hWnd, NULL);
          PostMessage(hWnd, WM_NULL, 0, 0);
          DestroyMenu(hMenu);
          break;
      }
      break;
    case WM_CLOSE:
      mg_stop(ctx);
      Shell_NotifyIcon(NIM_DELETE, &TrayIcon);
      PostQuitMessage(0);
      return 0;  // We've just sent our own quit message, with proper hwnd.
    default:
      if (msg==s_uTaskbarRestart)
        Shell_NotifyIcon(NIM_ADD, &TrayIcon);
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdline, int show) {
  WNDCLASS cls;
  HWND hWnd;
  MSG msg;

  init_server_name();
  memset(&cls, 0, sizeof(cls));
  cls.lpfnWndProc = (WNDPROC) WindowProc;
  cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  cls.lpszClassName = server_name;

  RegisterClass(&cls);
  hWnd = CreateWindow(cls.lpszClassName, server_name, WS_OVERLAPPEDWINDOW,
                      0, 0, 0, 0, NULL, NULL, NULL, NULL);
  ShowWindow(hWnd, SW_HIDE);

  TrayIcon.cbSize = sizeof(TrayIcon);
  TrayIcon.uID = ID_ICON;
  TrayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  TrayIcon.hIcon = hIcon = LoadImage(GetModuleHandle(NULL),
                                     MAKEINTRESOURCE(ID_ICON),
                                     IMAGE_ICON, 16, 16, 0);
  TrayIcon.hWnd = hWnd;
  snprintf(TrayIcon.szTip, sizeof(TrayIcon.szTip), "%s", server_name);
  TrayIcon.uCallbackMessage = WM_USER;
  Shell_NotifyIcon(NIM_ADD, &TrayIcon);

  while (GetMessage(&msg, hWnd, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Return the WM_QUIT value.
  return msg.wParam;
}
#elif defined(USE_COCOA)
#import <Cocoa/Cocoa.h>

@interface Mongoose : NSObject<NSApplicationDelegate>
- (void) openBrowser;
- (void) shutDown;
@end

@implementation Mongoose
- (void) openBrowser {
  [[NSWorkspace sharedWorkspace]
    openURL:[NSURL URLWithString:
      [NSString stringWithUTF8String:get_url_to_first_open_port(ctx)]]];
}
- (void) editConfig {
  create_config_file(config_file);
  [[NSWorkspace sharedWorkspace]
    openFile:[NSString stringWithUTF8String:config_file]
    withApplication:@"TextEdit"];
}
- (void)shutDown{
  [NSApp terminate:nil];
}
@end

int main(int argc, char *argv[]) {
  init_server_name();
  start_mongoose(argc, argv);

  [NSAutoreleasePool new];
  [NSApplication sharedApplication];

  // Add delegate to process menu item actions
  Mongoose *myDelegate = [[Mongoose alloc] autorelease];
  [NSApp setDelegate: myDelegate];

  // Run this app as agent
  ProcessSerialNumber psn = { 0, kCurrentProcess };
  TransformProcessType(&psn, kProcessTransformToBackgroundApplication);
  SetFrontProcess(&psn);

  // Add status bar menu
  id menu = [[NSMenu new] autorelease];

  // Add version menu item
  [menu addItem:[[[NSMenuItem alloc]
    //initWithTitle:[NSString stringWithFormat:@"%s", server_name]
    initWithTitle:[NSString stringWithUTF8String:server_name]
    action:@selector(noexist) keyEquivalent:@""] autorelease]];

  // Add configuration menu item
  [menu addItem:[[[NSMenuItem alloc]
    initWithTitle:@"Edit configuration"
    action:@selector(editConfig) keyEquivalent:@""] autorelease]];

  // Add connect menu item
  [menu addItem:[[[NSMenuItem alloc]
    initWithTitle:@"Open web root in a browser"
    action:@selector(openBrowser) keyEquivalent:@""] autorelease]];

  // Separator
  [menu addItem:[NSMenuItem separatorItem]];

  // Add quit menu item
  [menu addItem:[[[NSMenuItem alloc]
    initWithTitle:@"Quit"
    action:@selector(shutDown) keyEquivalent:@"q"] autorelease]];

  // Attach menu to the status bar
  id item = [[[NSStatusBar systemStatusBar]
    statusItemWithLength:NSVariableStatusItemLength] retain];
  [item setHighlightMode:YES];
  [item setImage:[NSImage imageNamed:@"mongoose_22x22.png"]];
  [item setMenu:menu];

  // Run the app
  [NSApp activateIgnoringOtherApps:YES];
  [NSApp run];

  mg_stop(ctx);

  return EXIT_SUCCESS;
}
#else
int main(int argc, char *argv[]) {
  init_server_name();
  start_mongoose(argc, argv);
  printf("%s started on port(s) %s with web root [%s]\n",
         server_name, mg_get_option(ctx, "listening_ports"),
         mg_get_option(ctx, "document_root"));
  while (exit_flag == 0) {
    sleep(1);
  }
  printf("Exiting on signal %d, waiting for all threads to finish...",
         exit_flag);
  fflush(stdout);
  mg_stop(ctx);
  printf("%s", " done.\n");

  return EXIT_SUCCESS;
}
#endif /* _WIN32 */
