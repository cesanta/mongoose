// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

static struct mg_dash s_dash;
static int authenticate(char *user, size_t userlen, const char *pass);

// Action buttons
static bool s_action1, s_action2;
static uint64_t s_action2_timeout;
static bool actions_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  (void) u;
  if (op == MG_DASH_READ) {
    if (s_action2_timeout > mg_now()) s_action2 = true;
    return true;
  }
  if (op == MG_DASH_WRITE) {
    if (s_action2 == true) s_action2_timeout = mg_now() + 750;
    return true;
  }
  return false;
}
static struct mg_field fields_actions[] = {
    {"action1", MG_VAL_BOOL, &s_action1, sizeof(s_action1)},
    {"action2", MG_VAL_BOOL, &s_action2, sizeof(s_action2)},
    {NULL, MG_VAL_INT, NULL, 0},
};
static struct mg_field_set set_actions = {"actions", fields_actions, actions_fn,
                                          NULL,      NULL,           NULL};

// Control panel
// s_led1, s_led2, s_led3 are used to communicate LED status
static bool s_led1, s_led2, s_led3;
static bool leds_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  (void) u;
  return op == MG_DASH_READ || op == MG_DASH_WRITE;
}
static struct mg_field fields_leds[] = {
    {"led1", MG_VAL_BOOL, &s_led1, sizeof(s_led1)},
    {"led2", MG_VAL_BOOL, &s_led2, sizeof(s_led2)},
    {"led3", MG_VAL_BOOL, &s_led3, sizeof(s_led3)},
    {NULL, MG_VAL_INT, NULL, 0},
};
static struct mg_field_set set_leds = {"leds", fields_leds, leds_fn,
                                       NULL,   NULL,        NULL};

// Read-only device Metrics
static int s_ram = 32, s_cpu = 7;
static double s_temperature = 24.8;

static bool metrics_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  (void) u;
  if (op != MG_DASH_READ) return false;
  s_ram = 25 + (rand() % 16);
  s_cpu = 7 + (rand() % 21);
  s_temperature = 14.8 + ((double) rand() / RAND_MAX) * 20.0;
  return true;
}

static struct mg_field fields_metrics[] = {
    {"ram", MG_VAL_INT, &s_ram, sizeof(s_ram)},
    {"cpu", MG_VAL_INT, &s_cpu, sizeof(s_cpu)},
    {"temperature", MG_VAL_DBL, &s_temperature, sizeof(s_temperature)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set set_metrics = {"metrics", fields_metrics, metrics_fn,
                                          NULL,      NULL,           NULL};

// Read-write device settings
static bool s_enable_login = false;
static double s_volume = 17.2;
static int s_log_level = MG_LL_DEBUG;
static char s_name[20] = "Dublin";
static int s_ota_interval = 30;
static char s_ota_version[] = "1.1.2";
static char s_ota_status[40] = "No scans yet";
static char s_ota_url[100] = "https://my-product.com/ota.json";

static bool settings_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  if (u->level < 3) return false;
  if (op == MG_DASH_READ) {
    s_log_level = mg_log_level;
    return true;
  }
  if (op == MG_DASH_WRITE && u->level >= 7) {
    mg_log_level = s_log_level;
    s_dash.authenticate = s_enable_login ? authenticate : NULL;
    return true;
  }
  return false;
}

static struct mg_field fields_settings[] = {
    {"volume", MG_VAL_DBL, &s_volume, sizeof(s_volume)},
    {"name", MG_VAL_STR, &s_name, sizeof(s_name)},
    {"log_level", MG_VAL_INT, &s_log_level, sizeof(s_log_level)},
    {"enable_login", MG_VAL_BOOL, &s_enable_login, sizeof(s_enable_login)},
    {"ota_version", MG_VAL_STR, &s_ota_version, sizeof(s_ota_version)},
    {"ota_status", MG_VAL_STR, &s_ota_status, sizeof(s_ota_status)},
    {"ota_url", MG_VAL_STR, &s_ota_url, sizeof(s_ota_url)},
    {"ota_interval", MG_VAL_INT, &s_ota_interval, sizeof(s_ota_interval)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set set_settings = {
    "settings", fields_settings, settings_fn, NULL, NULL, NULL};

#define NUM_POINTS_GRAPH1 7  // How many graph1 data points to send

struct point {
  uint16_t x;   // X coordinate of the data point, usually this is time
  uint16_t y1;  // We can plot multiple values per graph,
  uint16_t y2;  // let's plot 2 values: y1 and y2.
};

// Serialise chart data into a string: [[x,y1,y2],[x,y1,y2],...]
// One uint16_t takes 5 character max, so one entry is 19 bytes max.
// We could be more efficient by storing hex or base64-encoded data, but
// let's keep it simple. However if we do, update data parsing logic in
// the web_root/main.js
#define POINT_SIZE 19
static char s_graph1_data[NUM_POINTS_GRAPH1 * POINT_SIZE + 2 + 1];

// Randomly generate graph points and serialise them into a string
void read_graph1(void) {
  size_t i, len = 0;
  len += mg_snprintf(s_graph1_data + len, sizeof(s_graph1_data) - len, "[");
  for (i = 0; i < NUM_POINTS_GRAPH1; i++) {
    struct point p;
    p.x = (uint16_t) i;
    p.y1 = (uint16_t) (85 - ((49 * i) / NUM_POINTS_GRAPH1) + (rand() % 9));
    p.y2 = (uint16_t) (20 + ((69 * i) / NUM_POINTS_GRAPH1) + (rand() % 5));
    len += mg_snprintf(s_graph1_data + len, sizeof(s_graph1_data) - len,
                       "%s[%hu,%hu,%hu]", i > 0 ? "," : "", p.x, p.y1, p.y2);
  }
  len += mg_snprintf(s_graph1_data + len, sizeof(s_graph1_data) - len, "]");
}

static struct mg_field fields_graph1[] = {
    {"data", MG_VAL_RAW, s_graph1_data, sizeof(s_graph1_data)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static bool graph1_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  (void) u;
  if (op != MG_DASH_READ) return false;
  read_graph1();
  return true;
}
static struct mg_field_set set_graph1 = {"graph1", fields_graph1, graph1_fn,
                                         NULL,     NULL,          NULL};

#define NUM_POINTS_GRAPH2 100  // How many graph2 data points to send
static char s_graph2_data[NUM_POINTS_GRAPH2 * 4 + 2 + 1];
static bool s_graph2_report = true;

// Adjust t += 7, i * 9, and i * 3
void read_graph2(void) {
  static uint16_t t;
  size_t i, len = 0;
  len = mg_snprintf(s_graph2_data, sizeof(s_graph2_data), "[");
  t += 23;
  for (i = 0; i < NUM_POINTS_GRAPH2; i++) {
    uint16_t x = (uint16_t) (i * 7 + t);
    uint16_t y = (uint16_t) (i * 3 - t / 2);
    uint8_t a = (uint8_t) (x ^ (x >> 3)) & 255;
    uint8_t b = (uint8_t) (y ^ (y >> 4)) & 255;
    int v =
        70 + ((a < 128 ? a : 255 - a) >> 1) + ((b < 128 ? b : 255 - b) >> 2);
    len += mg_snprintf(s_graph2_data + len, sizeof(s_graph2_data) - len,
                       "%s%hhu", i > 0 ? "," : "", (uint8_t) v);
  }
  len += mg_snprintf(s_graph2_data + len, sizeof(s_graph2_data) - len, "]");
}

static struct mg_field fields_graph2[] = {
    {"data", MG_VAL_RAW, s_graph2_data, sizeof(s_graph2_data)},
    {"report", MG_VAL_BOOL, &s_graph2_report, sizeof(s_graph2_report)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static bool graph2_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  (void) u;
  if (op == MG_DASH_READ) read_graph2();
  return op == MG_DASH_READ || op == MG_DASH_WRITE;
}
static struct mg_field_set set_graph2 = {"graph2", fields_graph2, graph2_fn,
                                         NULL,     NULL,          NULL};

static int authenticate(char *user, size_t userlen, const char *pass) {
  int level = 0;  // Authentication failure
  if (strcmp(pass, "admin") == 0) {
    mg_snprintf(user, userlen, "%s", "admin");
    level = 7;  // Administrator
  } else if (strcmp(pass, "user") == 0) {
    mg_snprintf(user, userlen, "%s", "user");
    level = 3;  // Ordinary dude
  }
  return level;
}

static struct file {
  int index;
  char name[64];
  size_t size;
  uint64_t checksum;
} s_file;
static struct mg_field_set set_files;

static bool get_dir(const struct mg_dash_user *u, char *buf, size_t len) {
  (void) u;
  mg_snprintf(buf, len, "%s", "/tmp/dashboard");
  return true;
}

static struct mg_field fields_files[] = {
    {"name", MG_VAL_STR, s_file.name, sizeof(s_file.name)},
    {"size", MG_VAL_UINT64, &s_file.size, 0},
    {"checksum", MG_VAL_UINT64, &s_file.checksum, sizeof(s_file.checksum)},
    {NULL, MG_VAL_INT, NULL, 0},
};

// Custom reader: let the framework fill "name" and "size", then add our
// own field. A real implementation would hash the file instead of
// hardcoding zero
static bool files_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  if (op != MG_DASH_READ) return false;
  if (!mg_dash_dir_read(&set_files, u)) return false;
  s_file.checksum = 0;
  return true;
}

static struct mg_field_set set_files = {"files",       fields_files, files_fn,
                                        &s_file.index, get_dir,      NULL};

void mg_dash_init(struct mg_mgr *mgr) {
  MG_DASH_ADD_FIELD_SET(&s_dash, &set_files);
  MG_DASH_ADD_FIELD_SET(&s_dash, &set_leds);
  MG_DASH_ADD_FIELD_SET(&s_dash, &set_metrics);
  MG_DASH_ADD_FIELD_SET(&s_dash, &set_settings);
  MG_DASH_ADD_FIELD_SET(&s_dash, &set_graph1);
  MG_DASH_ADD_FIELD_SET(&s_dash, &set_graph2);
  MG_DASH_ADD_FIELD_SET(&s_dash, &set_actions);

#if MG_ARCH == MG_ARCH_UNIX
  // Create demo files in the upload directory
  mkdir("/tmp/dashboard", 0755);
  mg_file_printf(&mg_fs_posix, "/tmp/dashboard/device-config.json",
                 "{\"model\":\"example\",\"fw\":\"1.0\"}");
  mg_file_printf(&mg_fs_posix, "/tmp/dashboard/device-log-2026-04-25.txt",
                 "2026-04-25 12:00:00 Device started\n");
#endif

  mg_mem_files = mg_packed_files;
  mg_http_listen(mgr, MG_HTTP_ADDR, mg_dash_ev_handler, &s_dash);
}

void mg_dash_poll(struct mg_mgr *mgr) {
  // Send metrics change periodically
  static uint64_t timer1 = 0, timer2 = 0;
  if (mg_timer_expired(&timer1, 30000, mg_now())) {
    mg_dash_send_change(mgr, &set_metrics);
  }
  if (mg_timer_expired(&timer2, 120, mg_now()) && s_graph2_report) {
    mg_dash_send_change(mgr, &set_graph2);
  }
  if (s_action2 == true && s_action2_timeout < mg_now()) {
    s_action2 = false;
    mg_dash_send_change(mgr, &set_actions);
  }
}

// On desktop, build with -DMAIN flag to include main().
// On embedded system, run this code in your main() function
#ifdef MAIN
int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_dash_init(&mgr);

  for (;;) {
    mg_mgr_poll(&mgr, 1);
    mg_dash_poll(&mgr);
  }

  return 0;
}
#endif
