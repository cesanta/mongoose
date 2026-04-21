// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define HTTP_ADDR "http://0.0.0.0:8000"
#else
#define HTTP_ADDR "http://0.0.0.0:80"
#endif

// Control panel
static bool s_fan, s_relay1, s_relay2;

static struct mg_field fields_control[] = {
    {"fan", MG_VAL_BOOL, &s_fan, sizeof(s_fan)},
    {"relay1", MG_VAL_BOOL, &s_relay1, sizeof(s_relay1)},
    {"relay2", MG_VAL_BOOL, &s_relay2, sizeof(s_relay2)},
    {NULL, 0, NULL, 0},
};

static void sync_control(bool is_write) {
  if (is_write) {
    // gpio_write(FAN_PIN, s_fan);
    // gpio_write(RELAY_1_PIN, s_relay1);
    // gpio_write(RELAY_2_PIN, s_relay2);
  } else {
    // s_fan = gpio_read(FAN_PIN);
    // s_relay1 = gpio_read(RELAY_1_PIN);
    // s_relay2 = gpio_read(RELAY_2_PIN);
  }
}

// Read-only device Metrics
static int s_ram = 32, s_cpu = 37, s_humidity = 59;
static double s_temperature = 24.8;

static struct mg_field fields_metrics[] = {
    {"ram", MG_VAL_INT, &s_ram, sizeof(s_ram)},
    {"cpu", MG_VAL_INT, &s_cpu, sizeof(s_cpu)},
    {"humidity", MG_VAL_INT, &s_humidity, sizeof(s_humidity)},
    {"temperature", MG_VAL_DBL, &s_temperature, sizeof(s_temperature)},
    {NULL, 0, NULL, 0},
};

static void sync_metrics(bool is_write) {
  if (is_write) return;
  // Read values from hardware into the state variables
  // s_ram = calculate_current_free_ram();
}

// Read-write device settings
static double s_volume = 17.2;
static int s_log_level = MG_LL_DEBUG;
static char s_name[20] = "Dublin";

static struct mg_field fields_settings[] = {
    {"volume", MG_VAL_DBL, &s_volume, sizeof(s_volume)},
    {"name", MG_VAL_STR, &s_name, sizeof(s_name)},
    {"log_level", MG_VAL_INT, &s_log_level, sizeof(s_log_level)},
    {NULL, 0, NULL, 0},
};

static void sync_settings(bool is_write) {
  if (is_write) {
    mg_log_level = s_log_level;
  } else {
    s_log_level = mg_log_level;
  }
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
    {"data", MG_VAL_STR, s_chart1_data, sizeof(s_chart1_data)},
    {NULL, 0, NULL, 0},
};

// Randomly generate graph points and serialise them into a string
void sync_chart1(bool is_write) {
  size_t i, len = 0;
  if (is_write) return;
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

static struct mg_field_set field_sets[] = {
    {"control", fields_control, sync_control, 0, 0},
    {"metrics", fields_metrics, sync_metrics, 0, 0},
    {"settings", fields_settings, sync_settings, 0, 0},
    {"chart1", fields_chart1, sync_chart1, 0, 0},
    {0, 0, 0, 0, 0},
};
static struct mg_mgr s_mgr;
static struct mg_dash s_dashboard = {field_sets};

void mongoose_init(void) {
  mg_mgr_init(&s_mgr);
  mg_http_listen(&s_mgr, HTTP_ADDR, mg_dash_ev_handler, &s_dashboard);
}

void mongoose_poll(void) {
  mg_mgr_poll(&s_mgr, 1);

  {
    // Send metrics change periodically
    static uint64_t timer = 0;
    if (mg_timer_expired(&timer, 30000, mg_now())) {
      sync_metrics(false);
      mg_dash_send_change(&s_mgr, &s_dashboard, "metrics");
    }
  }
}
