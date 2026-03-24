#include "fs.h"
#include "str.h"
#include "util.h"

struct mg_str mg_file_read(struct mg_fs *fs, const char *path) {
  struct mg_str data;
  if (fs->info(path, &data.len, NULL) == MG_FS_FILE &&
      (data.buf = (char *) mg_calloc(1, data.len)) != NULL) {
    fs->reader(path, 0, data.buf, data.len);
  } else {
    data.buf = NULL, data.len = 0;
  }
  return data;
}

bool mg_file_write(struct mg_fs *fs, const char *path, const void *buf,
                   size_t len) {
  fs->del(path);
  return fs->writer(path, 0, buf, len) == len;
}

// struct mg_file_pfn_data {
//   struct mg_fs *fs;
//   const char *path;
// };

// static void mg_pfn_file(char ch, void *param) {
//   struct mg_file_pfn_data *data = (struct mg_file_pfn_data *) param;
//   size_t size = 0;
//   data->fs->info(data->path, &size, NULL);
//   data->fs->writer(data->path, size, &ch, 1);
// }


// size_t mg_file_printf(struct mg_fs *fs, const char *path, const char *fmt, ...) {
//   struct mg_file_pfn_data data = {fs, path};
//   va_list ap;
//   size_t result = 0;
//   fs->del(path);
//   va_start(ap, fmt);
//   result = mg_xprintf(mg_pfn_file, &data, fmt, &ap);
//   va_end(ap);
//   return result;
// }
