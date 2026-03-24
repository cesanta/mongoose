// Copyright (c) Cesanta Software Limited
// All rights reserved.

// This program is used to pack arbitrary data into a C binary. It takes
// a list of files as an input, and produc  es a .c data file that contains
// contents of all these files as a collection of byte arrays.
//
// Usage:
//   1. Compile this file:
//      cc -o pack pack.c
//
//   2. Convert list of files into single .c:
//      ./pack file1.data file2.data > fs.c
//
//   3. In your application code, you can access files using this function:
//      const char *mg_unpack(const char *file_name, size_t *size);
//
//   4. Build your app with fs.c:
//      cc -o my_app my_app.c fs.c

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  int i, j, ch;
  const char *strip_prefix = "";

  printf("%s", "#include \"mongoose.h\"\n");
  printf("%s", "\n");

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-s") == 0) {
      strip_prefix = argv[++i];
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      fprintf(stderr, "Usage: %s[-s STRIP_PREFIX] files...\n", argv[0]);
      exit(EXIT_FAILURE);
    } else {
      char ascii[12];
      FILE *fp = fopen(argv[i], "rb");
      if (fp == NULL) {
        fprintf(stderr, "Cannot open [%s]: %s\n", argv[i], strerror(errno));
        exit(EXIT_FAILURE);
      }

      printf("static const unsigned char v%d[] = {\n", i);
      for (j = 0; (ch = fgetc(fp)) != EOF; j++) {
        if (j == (int) sizeof(ascii)) {
          printf(" // %.*s\n", j, ascii);
          j = 0;
        }
        ascii[j] = (char) ((ch >= ' ' && ch <= '~' && ch != '\\') ? ch : '.');
        printf(" %3u,", ch);
      }
      // Append zero byte at the end, to make text files appear in memory
      // as nul-terminated strings.
      printf(" 0 // %.*s\n};\n", j, ascii);
      fclose(fp);
    }
  }

  printf("%s", "\nconst struct mg_mem_file mg_packed_files[] = {\n");

  for (i = 1; i < argc; i++) {
    struct stat st;
    const char *name = argv[i];
    size_t n = strlen(strip_prefix);
    if (strcmp(argv[i], "-s") == 0) {
      i++;
      continue;
    }
    stat(argv[i], &st);
    if (strncmp(name, strip_prefix, n) == 0) name += n;
    printf("  {\"/%s\", v%d, sizeof(v%d) - 1, %lu},\n", name, i, i,
           (unsigned long) st.st_mtime);
  }
  printf("%s", "  {NULL, NULL, 0, 0}\n");
  printf("%s", "};\n\n");

  return EXIT_SUCCESS;
}
