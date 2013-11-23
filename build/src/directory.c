#include "internal.h"

static void print_dir_entry(const struct de *de) {
  char size[64], mod[64], href[PATH_MAX * 3];
  const char *slash = de->file.is_directory ? "/" : "";

  if (de->file.is_directory) {
    mg_snprintf(size, sizeof(size), "%s", "[DIRECTORY]");
  } else {
     // We use (signed) cast below because MSVC 6 compiler cannot
     // convert unsigned __int64 to double. Sigh.
    if (de->file.size < 1024) {
      mg_snprintf(size, sizeof(size), "%d", (int) de->file.size);
    } else if (de->file.size < 0x100000) {
      mg_snprintf(size, sizeof(size),
                  "%.1fk", (double) de->file.size / 1024.0);
    } else if (de->file.size < 0x40000000) {
      mg_snprintf(size, sizeof(size),
                  "%.1fM", (double) de->file.size / 1048576);
    } else {
      mg_snprintf(size, sizeof(size),
                  "%.1fG", (double) de->file.size / 1073741824);
    }
  }
  strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M",
           localtime(&de->file.modification_time));
  mg_url_encode(de->file_name, href, sizeof(href));
  de->conn->num_bytes_sent += mg_chunked_printf(de->conn,
      "<tr><td><a href=\"%s%s%s\">%s%s</a></td>"
      "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
      de->conn->request_info.uri, href, slash, de->file_name, slash, mod, size);
}

// This function is called from send_directory() and used for
// sorting directory entries by size, or name, or modification time.
// On windows, __cdecl specification is needed in case if project is built
// with __stdcall convention. qsort always requires __cdels callback.
static int WINCDECL compare_dir_entries(const void *p1, const void *p2) {
  const struct de *a = (const struct de *) p1, *b = (const struct de *) p2;
  const char *query_string = a->conn->request_info.query_string;
  int cmp_result = 0;

  if (query_string == NULL) {
    query_string = "na";
  }

  if (a->file.is_directory && !b->file.is_directory) {
    return -1;  // Always put directories on top
  } else if (!a->file.is_directory && b->file.is_directory) {
    return 1;   // Always put directories on top
  } else if (*query_string == 'n') {
    cmp_result = strcmp(a->file_name, b->file_name);
  } else if (*query_string == 's') {
    cmp_result = a->file.size == b->file.size ? 0 :
      a->file.size > b->file.size ? 1 : -1;
  } else if (*query_string == 'd') {
    cmp_result = a->file.modification_time == b->file.modification_time ? 0 :
      a->file.modification_time > b->file.modification_time ? 1 : -1;
  }

  return query_string[1] == 'd' ? -cmp_result : cmp_result;
}

static int must_hide_file(struct mg_connection *conn, const char *path) {
  const char *pw_pattern = "**" PASSWORDS_FILE_NAME "$";
  const char *pattern = conn->ctx->config[HIDE_FILES];
  return match_prefix(pw_pattern, strlen(pw_pattern), path) > 0 ||
    (pattern != NULL && match_prefix(pattern, strlen(pattern), path) > 0);
}

static int scan_directory(struct mg_connection *conn, const char *dir,
                          void *data, void (*cb)(struct de *, void *)) {
  char path[PATH_MAX];
  struct dirent *dp;
  DIR *dirp;
  struct de de;

  if ((dirp = opendir(dir)) == NULL) {
    return 0;
  } else {
    de.conn = conn;

    while ((dp = readdir(dirp)) != NULL) {
      // Do not show current dir and hidden files
      if (!strcmp(dp->d_name, ".") ||
          !strcmp(dp->d_name, "..") ||
          must_hide_file(conn, dp->d_name)) {
        continue;
      }

      mg_snprintf(path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);

      // If we don't memset stat structure to zero, mtime will have
      // garbage and strftime() will segfault later on in
      // print_dir_entry(). memset is required only if mg_stat()
      // fails. For more details, see
      // http://code.google.com/p/mongoose/issues/detail?id=79
      memset(&de.file, 0, sizeof(de.file));
      mg_stat(path, &de.file);

      de.file_name = dp->d_name;
      cb(&de, data);
    }
    (void) closedir(dirp);
  }
  return 1;
}

static int remove_directory(struct mg_connection *conn, const char *dir) {
  char path[PATH_MAX];
  struct dirent *dp;
  DIR *dirp;
  struct de de;

  if ((dirp = opendir(dir)) == NULL) {
    return 0;
  } else {
    de.conn = conn;

    while ((dp = readdir(dirp)) != NULL) {
      // Do not show current dir, but show hidden files
      if (!strcmp(dp->d_name, ".") ||
          !strcmp(dp->d_name, "..")) {
        continue;
      }

      mg_snprintf(path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);

      // If we don't memset stat structure to zero, mtime will have
      // garbage and strftime() will segfault later on in
      // print_dir_entry(). memset is required only if mg_stat()
      // fails. For more details, see
      // http://code.google.com/p/mongoose/issues/detail?id=79
      memset(&de.file, 0, sizeof(de.file));
      mg_stat(path, &de.file);
      if(de.file.modification_time) {
          if(de.file.is_directory) {
              remove_directory(conn, path);
          } else {
              mg_remove(path);
          }
      }

    }
    (void) closedir(dirp);

    rmdir(dir);
  }

  return 1;
}

struct dir_scan_data {
  struct de *entries;
  int num_entries;
  int arr_size;
};

// Behaves like realloc(), but frees original pointer on failure
static void *realloc2(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (new_ptr == NULL) {
    free(ptr);
  }
  return new_ptr;
}

static void dir_scan_callback(struct de *de, void *data) {
  struct dir_scan_data *dsd = (struct dir_scan_data *) data;

  if (dsd->entries == NULL || dsd->num_entries >= dsd->arr_size) {
    dsd->arr_size *= 2;
    dsd->entries = (struct de *) realloc2(dsd->entries, dsd->arr_size *
                                          sizeof(dsd->entries[0]));
  }
  if (dsd->entries == NULL) {
    // TODO(lsm): propagate an error to the caller
    dsd->num_entries = 0;
  } else {
    dsd->entries[dsd->num_entries].file_name = mg_strdup(de->file_name);
    dsd->entries[dsd->num_entries].file = de->file;
    dsd->entries[dsd->num_entries].conn = de->conn;
    dsd->num_entries++;
  }
}

static void handle_directory_request(struct mg_connection *conn,
                                     const char *dir) {
  int i, sort_direction;
  struct dir_scan_data data = { NULL, 0, 128 };

  if (!scan_directory(conn, dir, &data, dir_scan_callback)) {
    send_http_error(conn, 500, "Cannot open directory",
                    "Error: opendir(%s): %s", dir, strerror(ERRNO));
    return;
  }

  sort_direction = conn->request_info.query_string != NULL &&
    conn->request_info.query_string[1] == 'd' ? 'a' : 'd';

  conn->must_close = 1;
  mg_printf(conn, "%s",
            "HTTP/1.1 200 OK\r\n"
            "Transfer-Encoding: Chunked\r\n"
            "Content-Type: text/html; charset=utf-8\r\n\r\n");

  conn->num_bytes_sent += mg_chunked_printf(conn,
      "<html><head><title>Index of %s</title>"
      "<style>th {text-align: left;}</style></head>"
      "<body><h1>Index of %s</h1><pre><table cellpadding=\"0\">"
      "<tr><th><a href=\"?n%c\">Name</a></th>"
      "<th><a href=\"?d%c\">Modified</a></th>"
      "<th><a href=\"?s%c\">Size</a></th></tr>"
      "<tr><td colspan=\"3\"><hr></td></tr>",
      conn->request_info.uri, conn->request_info.uri,
      sort_direction, sort_direction, sort_direction);

  // Print first entry - link to a parent directory
  conn->num_bytes_sent += mg_chunked_printf(conn,
      "<tr><td><a href=\"%s%s\">%s</a></td>"
      "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
      conn->request_info.uri, "..", "Parent directory", "-", "-");

  // Sort and print directory entries
  qsort(data.entries, (size_t) data.num_entries, sizeof(data.entries[0]),
        compare_dir_entries);
  for (i = 0; i < data.num_entries; i++) {
    print_dir_entry(&data.entries[i]);
    free(data.entries[i].file_name);
  }
  free(data.entries);

  conn->num_bytes_sent += mg_chunked_printf(conn, "%s",
                                            "</table></body></html>");
  conn->num_bytes_sent += mg_write(conn, "0\r\n\r\n", 5);
  conn->status_code = 200;
}

// For a given PUT path, create all intermediate subdirectories
// for given path. Return 0 if the path itself is a directory,
// or -1 on error, 1 if OK.
static int put_dir(const char *path) {
  char buf[PATH_MAX];
  const char *s, *p;
  struct file file = STRUCT_FILE_INITIALIZER;
  int len, res = 1;

  for (s = p = path + 2; (p = strchr(s, '/')) != NULL; s = ++p) {
    len = p - path;
    if (len >= (int) sizeof(buf)) {
      res = -1;
      break;
    }
    memcpy(buf, path, len);
    buf[len] = '\0';

    // Try to create intermediate directory
    DEBUG_TRACE(("mkdir(%s)", buf));
    if (!mg_stat(buf, &file) && mg_mkdir(buf, 0755) != 0) {
      res = -1;
      break;
    }

    // Is path itself a directory?
    if (p[1] == '\0') {
      res = 0;
    }
  }

  return res;
}
