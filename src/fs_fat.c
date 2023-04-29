#include "arch.h"
#include "fs.h"

#if MG_ENABLE_FATFS
#include <ff.h>

static int mg_days_from_epoch(int y, int m, int d) {
  y -= m <= 2;
  int era = y / 400;
  int yoe = y - era * 400;
  int doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return era * 146097 + doe - 719468;
}

static time_t mg_timegm(const struct tm *t) {
  int year = t->tm_year + 1900;
  int month = t->tm_mon;  // 0-11
  if (month > 11) {
    year += month / 12;
    month %= 12;
  } else if (month < 0) {
    int years_diff = (11 - month) / 12;
    year -= years_diff;
    month += 12 * years_diff;
  }
  int x = mg_days_from_epoch(year, month + 1, t->tm_mday);
  return 60 * (60 * (24L * x + t->tm_hour) + t->tm_min) + t->tm_sec;
}

static time_t ff_time_to_epoch(uint16_t fdate, uint16_t ftime) {
  struct tm tm;
  memset(&tm, 0, sizeof(struct tm));
  tm.tm_sec = (ftime << 1) & 0x3e;
  tm.tm_min = ((ftime >> 5) & 0x3f);
  tm.tm_hour = ((ftime >> 11) & 0x1f);
  tm.tm_mday = (fdate & 0x1f);
  tm.tm_mon = ((fdate >> 5) & 0x0f) - 1;
  tm.tm_year = ((fdate >> 9) & 0x7f) + 80;
  return mg_timegm(&tm);
}

static int ff_stat(const char *path, size_t *size, time_t *mtime) {
  FILINFO fi;
  if (path[0] == '\0') {
    if (size) *size = 0;
    if (mtime) *mtime = 0;
    return MG_FS_DIR;
  } else if (f_stat(path, &fi) == 0) {
    if (size) *size = (size_t) fi.fsize;
    if (mtime) *mtime = ff_time_to_epoch(fi.fdate, fi.ftime);
    return MG_FS_READ | MG_FS_WRITE | ((fi.fattrib & AM_DIR) ? MG_FS_DIR : 0);
  } else {
    return 0;
  }
}

static void ff_list(const char *dir, void (*fn)(const char *, void *),
                    void *userdata) {
  DIR d;
  FILINFO fi;
  if (f_opendir(&d, dir) == FR_OK) {
    while (f_readdir(&d, &fi) == FR_OK && fi.fname[0] != '\0') {
      if (!strcmp(fi.fname, ".") || !strcmp(fi.fname, "..")) continue;
      fn(fi.fname, userdata);
    }
    f_closedir(&d);
  }
}

static void *ff_open(const char *path, int flags) {
  FIL f;
  unsigned char mode = FA_READ;
  if (flags & MG_FS_WRITE) mode |= FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND;
  if (f_open(&f, path, mode) == 0) {
    FIL *fp;
    if ((fp = calloc(1, sizeof(*fp))) != NULL) {
      memcpy(fp, &f, sizeof(*fp));
      return fp;
    }
  }
  return NULL;
}

static void ff_close(void *fp) {
  if (fp != NULL) {
    f_close((FIL *) fp);
    free(fp);
  }
}

static size_t ff_read(void *fp, void *buf, size_t len) {
  UINT n = 0, misalign = ((size_t) buf) & 3;
  if (misalign) {
    char aligned[4];
    f_read((FIL *) fp, aligned, len > misalign ? misalign : len, &n);
    memcpy(buf, aligned, n);
  } else {
    f_read((FIL *) fp, buf, len, &n);
  }
  return n;
}

static size_t ff_write(void *fp, const void *buf, size_t len) {
  UINT n = 0;
  return f_write((FIL *) fp, (char *) buf, len, &n) == FR_OK ? n : 0;
}

static size_t ff_seek(void *fp, size_t offset) {
  f_lseek((FIL *) fp, offset);
  return offset;
}

static bool ff_rename(const char *from, const char *to) {
  return f_rename(from, to) == FR_OK;
}

static bool ff_remove(const char *path) {
  return f_unlink(path) == FR_OK;
}

static bool ff_mkdir(const char *path) {
  return f_mkdir(path) == FR_OK;
}

struct mg_fs mg_fs_fat = {ff_stat,  ff_list, ff_open,   ff_close,  ff_read,
                          ff_write, ff_seek, ff_rename, ff_remove, ff_mkdir};
#endif
