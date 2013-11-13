#include "internal.h"

#if defined(_WIN32)
static pthread_t pthread_self(void) {
  return GetCurrentThreadId();
}

static int pthread_mutex_init(pthread_mutex_t *mutex, void *unused) {
  (void) unused;
  *mutex = CreateMutex(NULL, FALSE, NULL);
  return *mutex == NULL ? -1 : 0;
}

static int pthread_mutex_destroy(pthread_mutex_t *mutex) {
  return CloseHandle(*mutex) == 0 ? -1 : 0;
}

static int pthread_mutex_lock(pthread_mutex_t *mutex) {
  return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

static int pthread_mutex_unlock(pthread_mutex_t *mutex) {
  return ReleaseMutex(*mutex) == 0 ? -1 : 0;
}

static int pthread_cond_init(pthread_cond_t *cv, const void *unused) {
  (void) unused;
  cv->signal = CreateEvent(NULL, FALSE, FALSE, NULL);
  cv->broadcast = CreateEvent(NULL, TRUE, FALSE, NULL);
  return cv->signal != NULL && cv->broadcast != NULL ? 0 : -1;
}

static int pthread_cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex) {
  HANDLE handles[] = {cv->signal, cv->broadcast};
  ReleaseMutex(*mutex);
  WaitForMultipleObjects(2, handles, FALSE, INFINITE);
  return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

static int pthread_cond_signal(pthread_cond_t *cv) {
  return SetEvent(cv->signal) == 0 ? -1 : 0;
}

static int pthread_cond_broadcast(pthread_cond_t *cv) {
  // Implementation with PulseEvent() has race condition, see
  // http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
  return PulseEvent(cv->broadcast) == 0 ? -1 : 0;
}

static int pthread_cond_destroy(pthread_cond_t *cv) {
  return CloseHandle(cv->signal) && CloseHandle(cv->broadcast) ? 0 : -1;
}

// For Windows, change all slashes to backslashes in path names.
static void change_slashes_to_backslashes(char *path) {
  int i;

  for (i = 0; path[i] != '\0'; i++) {
    if (path[i] == '/')
      path[i] = '\\';
    // i > 0 check is to preserve UNC paths, like \\server\file.txt
    if (path[i] == '\\' && i > 0)
      while (path[i + 1] == '\\' || path[i + 1] == '/')
        (void) memmove(path + i + 1,
            path + i + 2, strlen(path + i + 1));
  }
}

// Encode 'path' which is assumed UTF-8 string, into UNICODE string.
// wbuf and wbuf_len is a target buffer and its length.
static void to_unicode(const char *path, wchar_t *wbuf, size_t wbuf_len) {
  char buf[PATH_MAX * 2], buf2[PATH_MAX * 2];

  mg_strlcpy(buf, path, sizeof(buf));
  change_slashes_to_backslashes(buf);

  // Convert to Unicode and back. If doubly-converted string does not
  // match the original, something is fishy, reject.
  memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int) wbuf_len);
  WideCharToMultiByte(CP_UTF8, 0, wbuf, (int) wbuf_len, buf2, sizeof(buf2),
                      NULL, NULL);
  if (strcmp(buf, buf2) != 0) {
    wbuf[0] = L'\0';
  }
}

#if defined(_WIN32_WCE)
static time_t time(time_t *ptime) {
  time_t t;
  SYSTEMTIME st;
  FILETIME ft;

  GetSystemTime(&st);
  SystemTimeToFileTime(&st, &ft);
  t = SYS2UNIX_TIME(ft.dwLowDateTime, ft.dwHighDateTime);

  if (ptime != NULL) {
    *ptime = t;
  }

  return t;
}

static struct tm *localtime(const time_t *ptime, struct tm *ptm) {
  int64_t t = ((int64_t) *ptime) * RATE_DIFF + EPOCH_DIFF;
  FILETIME ft, lft;
  SYSTEMTIME st;
  TIME_ZONE_INFORMATION tzinfo;

  if (ptm == NULL) {
    return NULL;
  }

  * (int64_t *) &ft = t;
  FileTimeToLocalFileTime(&ft, &lft);
  FileTimeToSystemTime(&lft, &st);
  ptm->tm_year = st.wYear - 1900;
  ptm->tm_mon = st.wMonth - 1;
  ptm->tm_wday = st.wDayOfWeek;
  ptm->tm_mday = st.wDay;
  ptm->tm_hour = st.wHour;
  ptm->tm_min = st.wMinute;
  ptm->tm_sec = st.wSecond;
  ptm->tm_yday = 0; // hope nobody uses this
  ptm->tm_isdst =
    GetTimeZoneInformation(&tzinfo) == TIME_ZONE_ID_DAYLIGHT ? 1 : 0;

  return ptm;
}

static struct tm *gmtime(const time_t *ptime, struct tm *ptm) {
  // FIXME(lsm): fix this.
  return localtime(ptime, ptm);
}

static size_t strftime(char *dst, size_t dst_size, const char *fmt,
                       const struct tm *tm) {
  (void) snprintf(dst, dst_size, "implement strftime() for WinCE");
  return 0;
}
#endif

// Windows happily opens files with some garbage at the end of file name.
// For example, fopen("a.cgi    ", "r") on Windows successfully opens
// "a.cgi", despite one would expect an error back.
// This function returns non-0 if path ends with some garbage.
static int path_cannot_disclose_cgi(const char *path) {
  static const char *allowed_last_characters = "_-";
  int last = path[strlen(path) - 1];
  return isalnum(last) || strchr(allowed_last_characters, last) != NULL;
}

static int mg_stat(const char *path, struct file *filep) {
  wchar_t wbuf[PATH_MAX] = L"\\\\?\\";
  WIN32_FILE_ATTRIBUTE_DATA info;

  filep->modification_time = 0;
  to_unicode(path, wbuf + 4, ARRAY_SIZE(wbuf) - 4);
  if (GetFileAttributesExW(wbuf, GetFileExInfoStandard, &info) != 0) {
    filep->size = MAKEUQUAD(info.nFileSizeLow, info.nFileSizeHigh);
    filep->modification_time = SYS2UNIX_TIME(
        info.ftLastWriteTime.dwLowDateTime,
        info.ftLastWriteTime.dwHighDateTime);
    filep->is_directory = info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    // If file name is fishy, reset the file structure and return error.
    // Note it is important to reset, not just return the error, cause
    // functions like is_file_opened() check the struct.
    if (!filep->is_directory && !path_cannot_disclose_cgi(path)) {
      memset(filep, 0, sizeof(*filep));
    }
  }

  return filep->modification_time != 0;
}

static int mg_remove(const char *path) {
  wchar_t wbuf[PATH_MAX];
  to_unicode(path, wbuf, ARRAY_SIZE(wbuf));
  return DeleteFileW(wbuf) ? 0 : -1;
}

static int mg_mkdir(const char *path, int mode) {
  char buf[PATH_MAX];
  wchar_t wbuf[PATH_MAX];

  (void) mode;
  mg_strlcpy(buf, path, sizeof(buf));
  change_slashes_to_backslashes(buf);

  (void) MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, ARRAY_SIZE(wbuf));

  return CreateDirectoryW(wbuf, NULL) ? 0 : -1;
}

// Implementation of POSIX opendir/closedir/readdir for Windows.
static DIR * opendir(const char *name) {
  DIR *dir = NULL;
  wchar_t wpath[PATH_MAX];
  DWORD attrs;

  if (name == NULL) {
    SetLastError(ERROR_BAD_ARGUMENTS);
  } else if ((dir = (DIR *) malloc(sizeof(*dir))) == NULL) {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  } else {
    to_unicode(name, wpath, ARRAY_SIZE(wpath));
    attrs = GetFileAttributesW(wpath);
    if (attrs != 0xFFFFFFFF &&
        ((attrs & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)) {
      (void) wcscat(wpath, L"\\*");
      dir->handle = FindFirstFileW(wpath, &dir->info);
      dir->result.d_name[0] = '\0';
    } else {
      free(dir);
      dir = NULL;
    }
  }

  return dir;
}

static int closedir(DIR *dir) {
  int result = 0;

  if (dir != NULL) {
    if (dir->handle != INVALID_HANDLE_VALUE)
      result = FindClose(dir->handle) ? 0 : -1;

    free(dir);
  } else {
    result = -1;
    SetLastError(ERROR_BAD_ARGUMENTS);
  }

  return result;
}

static struct dirent *readdir(DIR *dir) {
  struct dirent *result = 0;

  if (dir) {
    if (dir->handle != INVALID_HANDLE_VALUE) {
      result = &dir->result;
      (void) WideCharToMultiByte(CP_UTF8, 0,
          dir->info.cFileName, -1, result->d_name,
          sizeof(result->d_name), NULL, NULL);

      if (!FindNextFileW(dir->handle, &dir->info)) {
        (void) FindClose(dir->handle);
        dir->handle = INVALID_HANDLE_VALUE;
      }

    } else {
      SetLastError(ERROR_FILE_NOT_FOUND);
    }
  } else {
    SetLastError(ERROR_BAD_ARGUMENTS);
  }

  return result;
}

#ifndef HAVE_POLL
static int poll(struct pollfd *pfd, int n, int milliseconds) {
  struct timeval tv;
  fd_set set;
  int i, result;
  SOCKET maxfd = 0;

  tv.tv_sec = milliseconds / 1000;
  tv.tv_usec = (milliseconds % 1000) * 1000;
  FD_ZERO(&set);

  for (i = 0; i < n; i++) {
    FD_SET((SOCKET) pfd[i].fd, &set);
    pfd[i].revents = 0;

    if (pfd[i].fd > maxfd) {
        maxfd = pfd[i].fd;
    }
  }

  if ((result = select(maxfd + 1, &set, NULL, NULL, &tv)) > 0) {
    for (i = 0; i < n; i++) {
      if (FD_ISSET(pfd[i].fd, &set)) {
        pfd[i].revents = POLLIN;
      }
    }
  }

  return result;
}
#endif // HAVE_POLL

static void set_close_on_exec(SOCKET sock) {
  (void) SetHandleInformation((HANDLE) sock, HANDLE_FLAG_INHERIT, 0);
}

int mg_start_thread(mg_thread_func_t f, void *p) {
  return (long)_beginthread((void (__cdecl *)(void *)) f, 0, p) == -1L ? -1 : 0;
}

static HANDLE dlopen(const char *dll_name, int flags) {
  wchar_t wbuf[PATH_MAX];
  (void) flags;
  to_unicode(dll_name, wbuf, ARRAY_SIZE(wbuf));
  return LoadLibraryW(wbuf);
}

#if !defined(NO_CGI)
#define SIGKILL 0
static int kill(pid_t pid, int sig_num) {
  (void) TerminateProcess(pid, sig_num);
  (void) CloseHandle(pid);
  return 0;
}

static void trim_trailing_whitespaces(char *s) {
  char *e = s + strlen(s) - 1;
  while (e > s && isspace(* (unsigned char *) e)) {
    *e-- = '\0';
  }
}

static pid_t spawn_process(struct mg_connection *conn, const char *prog,
                           char *envblk, char *envp[], int fdin,
                           int fdout, const char *dir) {
  HANDLE me;
  char *interp, full_interp[PATH_MAX], full_dir[PATH_MAX],
       cmdline[PATH_MAX], buf[PATH_MAX];
  FILE *fp;
  STARTUPINFOA si;
  PROCESS_INFORMATION pi = { 0 };

  (void) envp;

  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);

  // TODO(lsm): redirect CGI errors to the error log file
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;

  me = GetCurrentProcess();
  DuplicateHandle(me, (HANDLE) _get_osfhandle(fdin), me,
                  &si.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
  DuplicateHandle(me, (HANDLE) _get_osfhandle(fdout), me,
                  &si.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);

  // If CGI file is a script, try to read the interpreter line
  interp = conn->ctx->config[CGI_INTERPRETER];
  if (interp == NULL) {
    buf[0] = buf[1] = '\0';

    // Read the first line of the script into the buffer
    snprintf(cmdline, sizeof(cmdline), "%s%c%s", dir, '/', prog);
    if ((fp = mg_fopen(cmdline, "r")) != NULL) {
      fgets(buf, sizeof(buf), fp);
      fclose(fp);
      buf[sizeof(buf) - 1] = '\0';
    }

    if (buf[0] == '#' && buf[1] == '!') {
      trim_trailing_whitespaces(buf + 2);
    } else {
      buf[2] = '\0';
    }
    interp = buf + 2;
  }

  if (interp[0] != '\0') {
    GetFullPathNameA(interp, sizeof(full_interp), full_interp, NULL);
    interp = full_interp;
  }
  GetFullPathNameA(dir, sizeof(full_dir), full_dir, NULL);

  mg_snprintf(cmdline, sizeof(cmdline), "%s%s\"%s\\%s\"",
              interp, interp[0] == '\0' ? "" : " ", full_dir, prog);

  DEBUG_TRACE(("Running [%s]", cmdline));
  if (CreateProcessA(NULL, cmdline, NULL, NULL, TRUE,
        CREATE_NEW_PROCESS_GROUP, envblk, NULL, &si, &pi) == 0) {
    cry(conn, "%s: CreateProcess(%s): %ld",
        __func__, cmdline, ERRNO);
    pi.hProcess = (pid_t) -1;
  }

  (void) CloseHandle(si.hStdOutput);
  (void) CloseHandle(si.hStdInput);
  (void) CloseHandle(pi.hThread);

  return (pid_t) pi.hProcess;
}
#endif // !NO_CGI

static int set_non_blocking_mode(SOCKET sock) {
  unsigned long on = 1;
  return ioctlsocket(sock, FIONBIO, &on);
}
#endif
