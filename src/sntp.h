// Time / SNTP functions

#pragma once

#include "net.h"

// Return milliseconds since boot (uptime). Never goes backwards.
// On RTOS targets (FreeRTOS, ThreadX, Zephyr, etc.) uses the RTOS tick count.
// On STM32/Cube uses HAL_GetTick(). On Arduino uses millis().
// On Linux/macOS uses CLOCK_MONOTONIC_RAW or CLOCK_MONOTONIC; falls back to
// CLOCK_REALTIME if neither is available.
uint64_t mg_millis(void);

// Boot timestamp in ms since Epoch. 0 until the first successful SNTP sync;
// updated automatically by mg_sntp_connect() on each successful response.
extern uint64_t mg_boot_timestamp_ms;

// Return milliseconds since Epoch: mg_millis() + mg_boot_timestamp_ms.
// Until a successful SNTP request completes, this is identical to mg_millis().
uint64_t mg_now(void);

// Checks whether a periodic deadline has expired.
//
// Returns:
//   True when the timer has fired; false otherwise.
// Example:
//   uint64_t timer = 0;
//   if (mg_timer_expired(&timer, 1000, mg_millis())) {
//     MG_INFO(("every second"));
//   }
// Full examples:
//   tutorials/http/http-server/arduino/teensy41-http,
//   tutorials/mqtt/mqtt-client
// Related APIs:
//   mg_millis(), mg_now()
// Notes:
//   Initialise *expiration to 0 before first use. On expiry, this function
//   advances *expiration by period and handles time wrap-around.
bool mg_timer_expired(uint64_t *expiration, uint64_t period, uint64_t now);

// Connect to an SNTP server and send a time request.
// `url` defaults to "udp://time.google.com:123" when NULL.
// On success the internal boot timestamp is updated so that mg_now() returns
// the correct wall-clock time, and MG_EV_SNTP_TIME is fired on `fn` (if not
// NULL) with ev_data pointing to a uint64_t containing the epoch in milliseconds.
// Pass fn == NULL and fn_data == NULL for a fire-and-forget sync that only
// updates mg_boot_timestamp_ms. Typical polling usage:
//
// ```c
// uint64_t timer = 0;
// for (;;) {
//   uint64_t period = mg_boot_timestamp_ms == 0 ? 1000 : 3600 * 1000;
//   if (mg_timer_expired(&timer, period, mg_millis())) {
//     mg_sntp_connect(&mgr, NULL, NULL, NULL);
//   }
//   mg_mgr_poll(&mgr, 1000);
// }
// ```
struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fn_data);

// Private API, do not expose
void mg_sntp_request(struct mg_connection *c);
int64_t mg_sntp_parse(const unsigned char *buf, size_t len);
