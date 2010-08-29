/*
 * Copyright (c) 2004-2009 Sergey Lyubka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * $Id: main.c 518 2010-05-03 12:55:35Z valenok $
 */
 
#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS	/* Disable deprecation warning in VS2005 */
#endif /* _WIN32 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#include "mongoose.h"

#ifdef _WIN32
#include <windows.h>
#include <winsvc.h>
#define DIRSEP			'\\'
#define	snprintf		_snprintf
#if !defined(__LCC__)
#define	strdup(x)		_strdup(x)
#endif /* !MINGW */
#define	sleep(x)		Sleep((x) * 1000)
#else
#include <sys/wait.h>
#include <unistd.h>		/* For pause() */
#define DIRSEP '/'
#endif /* _WIN32 */

static int exit_flag;	                /* Program termination flag	*/

#if !defined(CONFIG_FILE)
#define	CONFIG_FILE		"mongoose.conf"
#endif /* !CONFIG_FILE */

#define MAX_OPTIONS 40

static void signal_handler(int sig_num) {
#if !defined(_WIN32)
  if (sig_num == SIGCHLD) {
    do {
    } while (waitpid(-1, &sig_num, WNOHANG) > 0);
  } else
#endif /* !_WIN32 */
  {
    exit_flag = sig_num;
  }
}

/*
 * Edit the passwords file.
 */
static int mg_edit_passwords(const char *fname, const char *domain,
                             const char *user, const char *pass) {
  struct mg_context	*ctx;
  const char *options[] = {"authentication_domain", NULL, NULL};
  int success;

  options[1] = domain;
  ctx = mg_start(NULL, options);
  success = mg_modify_passwords_file(ctx, fname, user, pass);
  mg_stop(ctx);

  return success;
}

static void show_usage_and_exit(void) {
  const char **names;
  int i, len;

  fprintf(stderr, "Mongoose version %s (c) Sergey Lyubka\n", mg_version());
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  mongoose -A <htpasswd_file> <realm> <user> <passwd>\n");
  fprintf(stderr, "  mongoose <config_file>\n");
  fprintf(stderr, "  mongoose [-option value ...]\n");
  fprintf(stderr, "OPTIONS:\n  ");

  names = mg_get_valid_option_names();
  len = 2;
  for (i = 0; names[i] != NULL; i++) {
    len += strlen(names[i]) + 1;
    if (len >= 80) {
      len = strlen(names[i]) + 3;
      fprintf(stderr, "%s", "\n  ");
    }
    fprintf(stderr, "%s%c", names[i], names[i + 1] == NULL ? '\n' : ',');
  }
  fprintf(stderr, "See  http://code.google.com/p/mongoose/wiki/MongooseManual"
          " for more details.\n");
  fprintf(stderr, "Example:\n  mongoose -listening_ports 80,443s "
          "-enable_directory_listing no\n");

  exit(EXIT_FAILURE);
}

static char *sdup(const char *str) {
  char *p;
  if ((p = malloc(strlen(str) + 1)) != NULL) {
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
    }
  }

  if (i == MAX_OPTIONS - 3) {
    fprintf(stderr, "%s\n", "Too many options specified");
    exit(EXIT_FAILURE);
  }
}

static void process_command_line_arguments(char *argv[], char **options) {
  const char	*config_file = CONFIG_FILE;
  char line[512], opt[512], *vals[100], val[512], path[FILENAME_MAX], *p;
  FILE *fp;
  size_t i, line_no = 0;

  /* First find out, which config file to open */
  for (i = 1; argv[i] != NULL && argv[i][0] == '-'; i += 2)
    if (argv[i + 1] == NULL)
      show_usage_and_exit();

  if (argv[i] != NULL && argv[i + 1] != NULL) {
    /* More than one non-option arguments are given */
    show_usage_and_exit();
  } else if (argv[i] != NULL) {
    /* Just one non-option argument is given, this is config file */
    config_file = argv[i];
  } else {
    /* No config file specified. Look for one where binary lives */
    if ((p = strrchr(argv[0], DIRSEP)) != 0) {
      snprintf(path, sizeof(path), "%.*s%s",
               (int) (p - argv[0]) + 1, argv[0], config_file);
      config_file = path;
    }
  }

  fp = fopen(config_file, "r");

  /* If config file was set in command line and open failed, exit */
  if (fp == NULL && argv[i] != NULL) {
    fprintf(stderr, "cannot open config file %s: %s\n",
            config_file, strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* Reset temporary value holders */
  (void) memset(vals, 0, sizeof(vals));

  if (fp != NULL) {
    printf("Loading config file %s, ignoring command line arguments\n",
           config_file);

    /* Loop over the lines in config file */
    while (fgets(line, sizeof(line), fp) != NULL) {

      line_no++;

      /* Ignore empty lines and comments */
      if (line[0] == '#' || line[0] == '\n')
        continue;

      if (sscanf(line, "%s %[^\r\n#]", opt, val) != 2) {
        fprintf(stderr, "%s: line %d is invalid\n",
                config_file, (int) line_no);
        exit(EXIT_FAILURE);
      }
      set_option(options, opt, val);
    }

    (void) fclose(fp);
  } else {
    for (i = 1; argv[i] != NULL && argv[i][0] == '-'; i += 2)
      set_option(options, &argv[i][1], argv[i + 1]);
  }
}

int main(int argc, char *argv[]) {
  struct mg_context	*ctx;
  char *options[MAX_OPTIONS];
  int i;

  /* Edit passwords file if -A option is specified */
  if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'A') {
    if (argc != 6) {
      show_usage_and_exit();
    }
    exit(mg_edit_passwords(argv[2], argv[3], argv[4], argv[5]) ?
         EXIT_SUCCESS : EXIT_FAILURE);
  }

  /* Show usage if -h or --help options are specified */
  if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
    show_usage_and_exit();
  }

  /* Update config based on command line arguments */
  options[0] = NULL;
  process_command_line_arguments(argv, options);

  /* Setup signal handler: quit on Ctrl-C */
#ifndef _WIN32
  signal(SIGCHLD, signal_handler);
#endif /* _WIN32 */
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  /* Start Mongoose */
  ctx = mg_start(NULL, (const char **) options);
  for (i = 0; options[i] != NULL; i++) {
    free(options[i]);
  }

  if (ctx == NULL) {
    (void) printf("%s\n", "Cannot initialize Mongoose context");
    exit(EXIT_FAILURE);
  }

  printf("Mongoose %s started on port(s) %s with web root [%s]\n",
         mg_version(), mg_get_option(ctx, "listening_ports"),
         mg_get_option(ctx, "document_root"));

  fflush(stdout);
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
