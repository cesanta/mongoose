// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "math.h"
#include "mongoose.h"

#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define HTTP_ADDR "http://0.0.0.0:8000"
#else
#define HTTP_ADDR "http://0.0.0.0:80"
#endif

#define NUM_POINTS 257  // How many data points to send

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
    // rand() - 0.5 + sin(x * 0.00002) * 40 + sin(x * 0.001) * 5 + sin(x * 0.1)
    // * 2;
    p.y1 = 100 + 50 * sin(i * 0.1) + rand() % 21;
    p.y2 = 70 + 30 * sin(i * 0.03) + rand() % 17;
    n += mg_xprintf(fn, arg, "%s[%hu,%hu,%hu]", comma, p.x, p.y1, p.y2);
    comma = ",";
  }
  n += mg_xprintf(fn, arg, "]");
  return n;
}

static void get_points(union mg_val *val) {
  val->fn = (void (*)(void)) print_points;
}

static bool s_led;

static void get_led(union mg_val *val) {
  val->b = s_led;
}

static void set_led(const union mg_val *val) {
  s_led = val->b;
}

// Modify this. This represents device's state to the dashboard
static struct mg_field fields[] = {
    {"led", MG_VAL_BOOL, get_led, set_led},
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
}

int main(void) {
  mongoose_init();
  for (;;) {
    mongoose_poll();
  }
  return 0;
}
