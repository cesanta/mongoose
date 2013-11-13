#include "internal.h"

#if !defined(_WIN32)
static int mg_stat(const char *path, struct file *filep) {
  struct stat st;

  filep->modification_time = (time_t) 0;
  if (stat(path, &st) == 0) {
    filep->size = st.st_size;
    filep->modification_time = st.st_mtime;
    filep->is_directory = S_ISDIR(st.st_mode);

    // See https://github.com/cesanta/mongoose/issues/109
    // Some filesystems report modification time as 0. Artificially
    // bump it up to mark mg_stat() success.
    if (filep->modification_time == (time_t) 0) {
      filep->modification_time = (time_t) 1;
    }
  }

  return filep->modification_time != (time_t) 0;
}

static void set_close_on_exec(int fd) {
  fcntl(fd, F_SETFD, FD_CLOEXEC);
}

int mg_start_thread(mg_thread_func_t func, void *param) {
  pthread_t thread_id;
  pthread_attr_t attr;
  int result;

  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if USE_STACK_SIZE > 1
  // Compile-time option to control stack size, e.g. -DUSE_STACK_SIZE=16384
  (void) pthread_attr_setstacksize(&attr, USE_STACK_SIZE);
#endif

  result = pthread_create(&thread_id, &attr, func, param);
  pthread_attr_destroy(&attr);

  return result;
}

#ifndef NO_CGI
static pid_t spawn_process(struct mg_connection *conn, const char *prog,
                           char *envblk, char *envp[], int fdin,
                           int fdout, const char *dir) {
  pid_t pid;
  const char *interp;

  (void) envblk;

  if ((pid = fork()) == -1) {
    // Parent
    send_http_error(conn, 500, http_500_error, "fork(): %s", strerror(ERRNO));
  } else if (pid == 0) {
    // Child
    if (chdir(dir) != 0) {
      cry(conn, "%s: chdir(%s): %s", __func__, dir, strerror(ERRNO));
    } else if (dup2(fdin, 0) == -1) {
      cry(conn, "%s: dup2(%d, 0): %s", __func__, fdin, strerror(ERRNO));
    } else if (dup2(fdout, 1) == -1) {
      cry(conn, "%s: dup2(%d, 1): %s", __func__, fdout, strerror(ERRNO));
    } else {
      // Not redirecting stderr to stdout, to avoid output being littered
      // with the error messages.
      (void) close(fdin);
      (void) close(fdout);

      // After exec, all signal handlers are restored to their default values,
      // with one exception of SIGCHLD. According to POSIX.1-2001 and Linux's
      // implementation, SIGCHLD's handler will leave unchanged after exec
      // if it was set to be ignored. Restore it to default action.
      signal(SIGCHLD, SIG_DFL);

      interp = conn->ctx->config[CGI_INTERPRETER];
      if (interp == NULL) {
        (void) execle(prog, prog, NULL, envp);
        cry(conn, "%s: execle(%s): %s", __func__, prog, strerror(ERRNO));
      } else {
        (void) execle(interp, interp, prog, NULL, envp);
        cry(conn, "%s: execle(%s %s): %s", __func__, interp, prog,
            strerror(ERRNO));
      }
    }
    exit(EXIT_FAILURE);
  }

  return pid;
}
#endif // !NO_CGI

static int set_non_blocking_mode(SOCKET sock) {
  int flags;

  flags = fcntl(sock, F_GETFL, 0);
  (void) fcntl(sock, F_SETFL, flags | O_NONBLOCK);

  return 0;
}
#endif // _WIN32

