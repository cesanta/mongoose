#include "main.h"

static void cli_wifi(const char *ssid, const char *pass) {
  if (wifi_init(ssid, pass)) {
    mg_file_printf(&mg_fs_posix, WIFI_FILE, "{%m:%m,%m:%m}\n", mg_print_esc, 0,
                   "ssid", mg_print_esc, 0, ssid, mg_print_esc, 0, "pass",
                   mg_print_esc, 0, pass);
    MG_INFO(("Reboot now"));
  }
}

static void cli_ls(void) {
  DIR *dirp = opendir(FS_ROOT);
  struct dirent *dp;
  if (dirp == NULL) {
    MG_ERROR(("Cannot open FS: %d", errno));
  } else {
    while ((dp = readdir(dirp)) != NULL) {
      /* Do not show current and parent dirs */
      if (strcmp((const char *) dp->d_name, ".") == 0 ||
          strcmp((const char *) dp->d_name, "..") == 0) {
        continue;
      } else {
        printf("%s\n", dp->d_name);
      }
    }
    closedir(dirp);
  }
}

static void cli_cat(const char *fname) {
  char path[MG_PATH_MAX];
  snprintf(path, sizeof(path), "%s/%s", FS_ROOT, fname);
  FILE *fp = fopen(path, "r");
  if (fp != NULL) {
    int ch;
    while ((ch = fgetc(fp)) != EOF) putchar(ch);
    fclose(fp);
  }
}

static void cli_rm(const char *fname) {
  char path[100];
  snprintf(path, sizeof(path), "%s/%s", FS_ROOT, fname);
  remove(path);
}

static struct mg_iobuf in;

void cli(uint8_t input_byte) {
  if (input_byte == 0 || input_byte == 0xff) return;
  if (in.len >= 128) in.len = 0;
  mg_iobuf_add(&in, in.len, &input_byte, sizeof(input_byte));

  if (input_byte == '\n') {
    const char *arrow = "---";
    char buf0[10], buf1[50], buf2[250];

    in.buf[in.len] = '\0';
    buf0[0] = buf1[0] = buf2[0] = '\0';
    sscanf((char *) in.buf, "%9s %49s %249[^\r\n]", buf0, buf1, buf2);

    printf("%s CLI command: '%s'\n", arrow, buf0);
    if (strcmp(buf0, "reboot") == 0) {
      esp_restart();
    } else if (strcmp(buf0, "ls") == 0) {
      cli_ls();
    } else if (strcmp(buf0, "cat") == 0) {
      cli_cat(buf1);
    } else if (strcmp(buf0, "rm") == 0) {
      cli_rm(buf1);
    } else if (strcmp(buf0, "reboot") == 0) {
      esp_restart();
    } else if (strcmp(buf0, "ll") == 0) {
      mg_log_set(atoi(buf1));
    } else if (strcmp(buf0, "wifi") == 0) {
      cli_wifi(buf1, buf2);
    } else {
      printf("%s %s\n", arrow, "Unknown command. Usage:");
      printf("%s %s\n", arrow, "  set NAME VALUE");
      printf("%s %s\n", arrow, "  rm FILENAME");
      printf("%s %s\n", arrow, "  cat FILENAME");
      printf("%s %s\n", arrow, "  ls");
      printf("%s %s\n", arrow, "  reboot");
      printf("%s %s\n", arrow, "  wifi WIFI_NET WIFI_PASS");
    }
    printf("%s %s\n", arrow, "CLI output end");
    in.len = 0;
  }
}

void cli_init(void) {
  mg_iobuf_init(&in, 0, 32);
}
