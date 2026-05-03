// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

// Action buttons
static bool s_action1, s_action2;
static uint64_t s_action2_timeout;
static void write_actions(void) {
  if (s_action2 == true) s_action2_timeout = mg_now() + 750;
}
static void read_actions(void) {
  if (s_action2_timeout > mg_now()) s_action2 = true;
}
static struct mg_field fields_actions[] = {
    {"action1", MG_VAL_BOOL, &s_action1, sizeof(s_action1)},
    {"action2", MG_VAL_BOOL, &s_action2, sizeof(s_action2)},
    {NULL, MG_VAL_INT, NULL, 0},
};
static struct mg_field_set set_actions = {
    "actions", fields_actions, read_actions, write_actions, 0, 0, NULL,
};

// Control panel
// s_led1, s_led2, s_led3 are used to communicate LED status
static bool s_led1, s_led2, s_led3;
static void write_leds(void) {
  // gpio_write(LED1_PIN, s_led1);
  // gpio_write(LED2_PIN, s_led2);
  // gpio_write(LED3_PIN, s_led3);
}
static void read_leds(void) {
  // s_led1 = gpio_read(LED1_PIN);
  // s_led2 = gpio_read(LED2_PIN);
  // s_led3 = gpio_read(LED3_PIN);
}
static struct mg_field fields_leds[] = {
    {"led1", MG_VAL_BOOL, &s_led1, sizeof(s_led1)},
    {"led2", MG_VAL_BOOL, &s_led2, sizeof(s_led2)},
    {"led3", MG_VAL_BOOL, &s_led3, sizeof(s_led3)},
    {NULL, MG_VAL_INT, NULL, 0},
};
static struct mg_field_set set_leds = {
    "leds", fields_leds, read_leds, write_leds, 0, 0, NULL,
};

// Read-only device Metrics
static int s_ram = 32, s_cpu = 7;
static double s_temperature = 24.8;

static void read_metrics(void) {
  s_ram = 25 + (rand() % 16);  // Sumulate metric change
  s_cpu = 7 + (rand() % 21);
  s_temperature = 14.8 + ((double) rand() / RAND_MAX) * 20.0;
}

static struct mg_field fields_metrics[] = {
    {"ram", MG_VAL_INT, &s_ram, sizeof(s_ram)},
    {"cpu", MG_VAL_INT, &s_cpu, sizeof(s_cpu)},
    {"temperature", MG_VAL_DBL, &s_temperature, sizeof(s_temperature)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set set_metrics = {
    "metrics", fields_metrics, read_metrics, NULL, 0, 0, NULL,
};

// Read-write device settings
static double s_volume = 17.2;
static int s_log_level = MG_LL_DEBUG;
static char s_name[20] = "Dublin";
static int s_ota_interval = 30;
static char s_ota_version[] = "1.1.2";
static char s_ota_status[40] = "No scans yet";
static char s_ota_url[100] = "https://my-product.com/ota.json";

static void write_settings(void) {
  mg_log_level = s_log_level;
}

static void read_settings(void) {
  s_log_level = mg_log_level;
}

static struct mg_field fields_settings[] = {
    {"volume", MG_VAL_DBL, &s_volume, sizeof(s_volume)},
    {"name", MG_VAL_STR, &s_name, sizeof(s_name)},
    {"log_level", MG_VAL_INT, &s_log_level, sizeof(s_log_level)},
    {"ota_version", MG_VAL_STR, &s_ota_version, sizeof(s_ota_version)},
    {"ota_status", MG_VAL_STR, &s_ota_status, sizeof(s_ota_status)},
    {"ota_url", MG_VAL_STR, &s_ota_url, sizeof(s_ota_url)},
    {"ota_interval", MG_VAL_INT, &s_ota_interval, sizeof(s_ota_interval)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set set_settings = {
    "settings", fields_settings, read_settings, write_settings, 0, 0, NULL,
};

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
    p.y1 = 85 - (uint16_t) ((49 * i) / NUM_POINTS_GRAPH1) + (rand() % 9);
    p.y2 = 20 + (uint16_t) ((69 * i) / NUM_POINTS_GRAPH1) + (rand() % 5);
    len += mg_snprintf(s_graph1_data + len, sizeof(s_graph1_data) - len,
                       "%s[%hu,%hu,%hu]", i > 0 ? "," : "", p.x, p.y1, p.y2);
  }
  len += mg_snprintf(s_graph1_data + len, sizeof(s_graph1_data) - len, "]");
}

static struct mg_field fields_graph1[] = {
    {"data", MG_VAL_RAW, s_graph1_data, sizeof(s_graph1_data)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set set_graph1 = {
    "graph1", fields_graph1, read_graph1, NULL, 0, 0, NULL,
};

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
    uint8_t a = (x ^ (x >> 3)) & 255;
    uint8_t b = (y ^ (y >> 4)) & 255;
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

static struct mg_field_set set_graph2 = {
    "graph2", fields_graph2, read_graph2, NULL, 0, 0, NULL,
};

static int authenticate(const char *user, const char *pass) {
  int level = 0;  // Authentication failure
  if (strcmp(pass, "admin") == 0) {
    level = 7;  // Administrator
  } else if (strcmp(pass, "user") == 0) {
    level = 3;  // Ordinary dude
  }
  (void) user;
  return level;
}

void mg_dash_init(struct mg_mgr *mgr) {
  static struct mg_dash dash;  // Important: keep it static!

  MG_DASH_ADD_FIELD_SET(&dash, &set_leds);
  MG_DASH_ADD_FIELD_SET(&dash, &set_metrics);
  MG_DASH_ADD_FIELD_SET(&dash, &set_settings);
  MG_DASH_ADD_FIELD_SET(&dash, &set_graph1);
  MG_DASH_ADD_FIELD_SET(&dash, &set_graph2);
  MG_DASH_ADD_FIELD_SET(&dash, &set_actions);

  // Add two fake files - for demonstration
  mg_dash_file_add(mg_str("device-config.json"), 1234);
  mg_dash_file_add(mg_str("device-log-2026-04-25.txt"), 1327854);

  // Require authentication
  dash.authenticate = authenticate;

  mg_mem_files = mg_packed_files;
  mg_http_listen(mgr, MG_HTTP_ADDR, mg_dash_ev_handler, &dash);
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
