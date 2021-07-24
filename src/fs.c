#include "fs.h"

int mg_stat(const char *path, mg_stat_t *st) {
#ifdef _WIN32
  wchar_t tmp[MG_PATH_MAX];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, tmp, sizeof(tmp) / sizeof(tmp[0]));
  return _wstati64(tmp, st);
#else
  return stat(path, st);
#endif
}
