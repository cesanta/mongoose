// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

static struct mg_dash s_dashboard;

// Control panel
// s_led1, s_led2, s_led3 are used to communicate LED status
static bool s_led1, s_led2, s_led3;

static struct mg_field fields_leds[] = {
    {"led1", MG_VAL_BOOL, &s_led1, sizeof(s_led1)},
    {"led2", MG_VAL_BOOL, &s_led2, sizeof(s_led2)},
    {"led3", MG_VAL_BOOL, &s_led3, sizeof(s_led3)},
    {NULL, MG_VAL_INT, NULL, 0},
};

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

// Read-only device Metrics
static int s_ram = 32, s_cpu = 7;
static double s_temperature = 24.8;

static struct mg_field fields_metrics[] = {
    {"ram", MG_VAL_INT, &s_ram, sizeof(s_ram)},
    {"cpu", MG_VAL_INT, &s_cpu, sizeof(s_cpu)},
    {"temperature", MG_VAL_DBL, &s_temperature, sizeof(s_temperature)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static void read_metrics(void) {
  s_ram = 25 + (rand() % 16);  // Sumulate metric change
  s_cpu = 7 + (rand() % 21);
  s_temperature = 14.8 + ((double) rand() / RAND_MAX) * 20.0;
}

// Read-write device settings
static double s_volume = 17.2;
static int s_log_level = MG_LL_DEBUG;
static char s_name[20] = "Dublin";

static struct mg_field fields_settings[] = {
    {"volume", MG_VAL_DBL, &s_volume, sizeof(s_volume)},
    {"name", MG_VAL_STR, &s_name, sizeof(s_name)},
    {"log_level", MG_VAL_INT, &s_log_level, sizeof(s_log_level)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static void write_settings(void) {
  mg_log_level = s_log_level;
}

static void read_settings(void) {
  s_log_level = mg_log_level;
}

#define NUM_POINTS 7  // How many data points to send

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
static char s_chart1_data[NUM_POINTS * POINT_SIZE + 2 + 1];

static struct mg_field fields_chart1[] = {
    {"data", MG_VAL_RAW, s_chart1_data, sizeof(s_chart1_data)},
    {NULL, MG_VAL_INT, NULL, 0},
};

// Randomly generate graph points and serialise them into a string
void read_chart1(void) {
  size_t i, len = 0;
  len += mg_snprintf(s_chart1_data + len, sizeof(s_chart1_data) - len, "[");
  for (i = 0; i < NUM_POINTS; i++) {
    struct point p;
    p.x = (uint16_t) i;
    p.y1 = 85 - (uint16_t) ((49 * i) / NUM_POINTS) + (rand() % 9);
    p.y2 = 20 + (uint16_t) ((69 * i) / NUM_POINTS) + (rand() % 5);
    len += mg_snprintf(s_chart1_data + len, sizeof(s_chart1_data) - len,
                       "%s[%hu,%hu,%hu]", i > 0 ? "," : "", p.x, p.y1, p.y2);
  }
  len += mg_snprintf(s_chart1_data + len, sizeof(s_chart1_data) - len, "]");
}

// Files array: [{"name": "foo.txt", "size": 1234}]
static char s_files[1024];
static struct mg_field fields_files[] = {
    {"data", MG_VAL_RAW, s_files, sizeof(s_files)},
    {NULL, MG_VAL_INT, NULL, 0},
};
static void read_files(void) {
  size_t len = 0;
  struct mg_dash_file *f;
  len += mg_snprintf(s_files + len, sizeof(s_files) - len, "[");
  for (f = s_dashboard.files; f; f = f->next) {
    len += mg_snprintf(s_files + len, sizeof(s_files) - len, "%s{%m:%m,%m:%u}",
                       len > 1 ? "," : "", MG_ESC("name"), MG_ESC(f->name),
                       MG_ESC("size"), f->size);
  }
  len += mg_snprintf(s_files + len, sizeof(s_files) - len, "]");
}

// Each field set exports REST HTTP API /api/NAME
static struct mg_field_set field_sets[] = {
    {"leds", fields_leds, read_leds, write_leds, 0, 0},
    {"metrics", fields_metrics, read_metrics, NULL, 0, 0},
    {"settings", fields_settings, read_settings, write_settings, 0, 0},
    {"chart1", fields_chart1, read_chart1, NULL, 0, 0},
    {"files", fields_files, read_files, NULL, 0, 0},
    {0, 0, NULL, NULL, 0, 0},
};

void mg_dash_init(struct mg_mgr *mgr) {
  s_dashboard.sets = field_sets;
  mg_dash_file_add(&s_dashboard, mg_str("device-config.json"), 1234);
  mg_dash_file_add(&s_dashboard, mg_str("device-log-2026-04-25.txt"), 1327854);
  mg_http_listen(mgr, MG_HTTP_ADDR, mg_dash_ev_handler, &s_dashboard);
}

void mg_dash_poll(struct mg_mgr *mgr) {
  // Send metrics change periodically
  static uint64_t timer = 0;
  if (mg_timer_expired(&timer, 30000, mg_now())) {
    mg_dash_send_change(mgr, &s_dashboard, "metrics");
  }
}
