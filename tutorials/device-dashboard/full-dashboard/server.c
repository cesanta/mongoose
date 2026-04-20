// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define HTTP_ADDR "http://0.0.0.0:8000"
#else
#define HTTP_ADDR "http://0.0.0.0:80"
#endif

#define NUM_POINTS 7  // How many data points to send

struct point {
  uint16_t x;   // X coordinate of the data point, usually this is time
  uint16_t y1;  // We can plot multiple values per graph,
  uint16_t y2;  // let's plot 2 values: y1 and y2.
};

// This function prints a JSON array of plot points
size_t print_points(void (*fn)(char, void *), void *arg, va_list *ap) {
  size_t i, n = 0;
  const char *comma = "";
  (void) ap;
  n += mg_xprintf(fn, arg, "[");
  for (i = 0; i < NUM_POINTS; i++) {
    struct point p;
    p.x = (uint16_t) i;
    p.y1 = 80 -
           (uint16_t) ((90 * (i > (NUM_POINTS - 1) / 4
                                  ? i - (NUM_POINTS - 1) / 4
                                  : (NUM_POINTS - 1) / 4 - i)) /
                       (NUM_POINTS - 1)) +
           (rand() % 9);
    p.y2 = 20 + (uint16_t) ((69 * i) / NUM_POINTS) + (rand() % 5);
    n += mg_xprintf(fn, arg, "%s[%hu,%hu,%hu]", comma, p.x, p.y1, p.y2);
    comma = ",";
  }
  n += mg_xprintf(fn, arg, "]");
  return n;
}
static void get_points(union mg_val *val) {
  val->fn = (void (*)(void)) print_points;
}

static int s_ram = 32, s_cpu = 37, s_humidity = 59;
static double s_temp = 24.8;
// This function prints a JSON object with read-only metrics
size_t print_metrics(void (*fn)(char, void *), void *arg, va_list *ap) {
  size_t n = 0;
  (void) ap;
  n += mg_xprintf(fn, arg, "{");
  n += mg_xprintf(fn, arg, "%s%m:%.3g", " ", MG_ESC("temperature"), s_temp);
  n += mg_xprintf(fn, arg, "%s%m:%d", ",", MG_ESC("humidity"), s_humidity);
  n += mg_xprintf(fn, arg, "%s%m:%d", ",", MG_ESC("cpu"), s_cpu);
  n += mg_xprintf(fn, arg, "%s%m:%d", ",", MG_ESC("ram"), s_ram);
  n += mg_xprintf(fn, arg, "}");
  return n;
}
static void get_metrics(union mg_val *val) {
  val->fn = (void (*)(void)) print_metrics;
}

static double s_volume = 17.2;
static char s_name[20] = "Dublin";
// This function prints a JSON object with read-only metrics
size_t print_settings(void (*fn)(char, void *), void *arg, va_list *ap) {
  size_t n = 0;
  (void) ap;
  n += mg_xprintf(fn, arg, "{");
  n += mg_xprintf(fn, arg, "%s%m:%g", " ", MG_ESC("volume"), s_volume);
  n += mg_xprintf(fn, arg, "%s%m:%m", ",", MG_ESC("name"), MG_ESC(s_name));
  n += mg_xprintf(fn, arg, "}");
  return n;
}
static void set_settings(const union mg_val *val) {
  mg_json_unescape(val->s, "$.name", s_name, sizeof(s_name));
  mg_json_get_num(val->s, "$.volume", &s_volume);
}
static void get_settings(union mg_val *val) {
  val->fn = (void (*)(void)) print_settings;
}

static bool s_fan, s_relay1, s_relay2;
size_t print_controls(void (*fn)(char, void *), void *arg, va_list *ap) {
  size_t n = 0;
  (void) ap;
  n += mg_xprintf(fn, arg, "{");
  n += mg_xprintf(fn, arg, "%s%m:%s", " ", MG_ESC("fan"),
                  s_fan ? "true" : "false");
  n += mg_xprintf(fn, arg, "%s%m:%s", ",", MG_ESC("relay1"),
                  s_relay1 ? "true" : "false");
  n += mg_xprintf(fn, arg, "%s%m:%s", ",", MG_ESC("relay2"),
                  s_relay2 ? "true" : "false");
  n += mg_xprintf(fn, arg, "}");
  return n;
}
static void set_controls(const union mg_val *val) {
  mg_json_get_bool(val->s, "$.fan", &s_fan);
  mg_json_get_bool(val->s, "$.relay1", &s_relay1);
  mg_json_get_bool(val->s, "$.relay2", &s_relay2);
}
static void get_controls(union mg_val *val) {
  val->fn = (void (*)(void)) print_controls;
}

// Modify this. This represents device's state to the dashboard
static struct mg_field fields[] = {
    {"controls", MG_VAL_FN, get_controls, set_controls},
    {"settings", MG_VAL_FN, get_settings, set_settings},
    {"metrics", MG_VAL_FN, get_metrics, NULL},
    {"points", MG_VAL_FN, get_points, NULL},
    {NULL, 0, 0, 0},
};

struct mg_mgr mgr;
struct mg_dash dash = {fields};

void mongoose_init(void) {
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, HTTP_ADDR, mg_dash_ev_handler, &dash);
}

void mongoose_poll(void) {
  mg_mgr_poll(&mgr, 1);

  // Simulate metrics change periodically
  static uint64_t timer = 0;
  if (mg_timer_expired(&timer, 30000, mg_now())) {
    // s_temp = 14.8 + ((double)rand() / RAND_MAX) * 20.0;
    // s_humidity = 37 + (rand() % 21);
    // s_cpu = 3 + (rand() % 21);
    // s_ram = 25 + (rand() % 16);
    mg_dash_send_change(&mgr, mg_dash_find_field(fields, mg_str("metrics")));
    mg_dash_send_change(&mgr, mg_dash_find_field(fields, mg_str("points")));
  }
}

int main(void) {
  mongoose_init();
  for (;;) {
    mongoose_poll();
  }
  return 0;
}
