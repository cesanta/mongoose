/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef WINCE

const char *strerror(int err) {
  /*
   * TODO(alashkin): there is no strerror on WinCE;
   * look for similar wce_xxxx function
   */
  static char buf[10];
  snprintf(buf, sizeof(buf), "%d", err);
  return buf;
}

int open(const char *filename, int oflag, int pmode) {
  /*
   * TODO(alashkin): mg_open function is not used in mongoose
   * but exists in documentation as utility function
   * Shall we delete it at all or implement for WinCE as well?
   */
  DebugBreak();
  return 0; /* for compiler */
}

int _wstati64(const wchar_t *path, cs_stat_t *st) {
  DWORD fa = GetFileAttributesW(path);
  if (fa == INVALID_FILE_ATTRIBUTES) {
    return -1;
  }
  memset(st, 0, sizeof(*st));
  if ((fa & FILE_ATTRIBUTE_DIRECTORY) == 0) {
    HANDLE h;
    FILETIME ftime;
    st->st_mode |= _S_IFREG;
    h = CreateFileW(path, GENERIC_READ, 0, NULL, OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
      return -1;
    }
    st->st_size = GetFileSize(h, NULL);
    GetFileTime(h, NULL, NULL, &ftime);
    st->st_mtime = (uint32_t)((((uint64_t) ftime.dwLowDateTime +
                                ((uint64_t) ftime.dwHighDateTime << 32)) /
                               10000000.0) -
                              11644473600);
    CloseHandle(h);
  } else {
    st->st_mode |= _S_IFDIR;
  }
  return 0;
}

/* Windows CE doesn't have neither gmtime nor strftime */
static void mg_gmt_time_string(char *buf, size_t buf_len, time_t *t) {
  FILETIME ft;
  SYSTEMTIME systime;
  if (t != NULL) {
    uint64_t filetime = (*t + 11644473600) * 10000000;
    ft.dwLowDateTime = filetime & 0xFFFFFFFF;
    ft.dwHighDateTime = (filetime & 0xFFFFFFFF00000000) >> 32;
    FileTimeToSystemTime(&ft, &systime);
  } else {
    GetSystemTime(&systime);
  }
  /* There is no PRIu16 in WinCE SDK */
  snprintf(buf, buf_len, "%d.%d.%d %d:%d:%d GMT", (int) systime.wYear,
           (int) systime.wMonth, (int) systime.wDay, (int) systime.wHour,
           (int) systime.wMinute, (int) systime.wSecond);
}

#endif
