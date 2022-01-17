#include "fs.h"

struct mg_fd *mg_fs_open(struct mg_fs *fs, const char *path, int flags) {
  struct mg_fd *fd = (struct mg_fd *) calloc(1, sizeof(*fd));
  if (fd != NULL) {
    fd->fd = fs->open(path, flags);
    fd->fs = fs;
    if (fd->fd == NULL) {
      free(fd);
      fd = NULL;
    }
  }
  return fd;
}

void mg_fs_close(struct mg_fd *fd) {
  if (fd != NULL) {
    fd->fs->close(fd->fd);
    free(fd);
  }
}
