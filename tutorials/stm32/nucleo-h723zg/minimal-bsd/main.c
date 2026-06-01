// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Two FreeRTOS tasks sharing one Mongoose event manager via the BSD socket
// shim (MG_ENABLE_BSD_SOCKETS).  task1 owns the manager and drives the
// network; task2 accepts connections and spawns per-connection echo_tasks
// that speak standard BSD sockets.

#include "hal.h"
#include "mongoose.h"

#ifndef UART_DEBUG
#define UART_DEBUG USART3
#define UART_DEBUG_TX_PIN PIN('D', 8)
#define UART_DEBUG_RX_PIN PIN('D', 9)
#else
#define UART_DEBUG_TX_PIN PIN('A', 9)
#define UART_DEBUG_RX_PIN PIN('A', 10)
#endif

#define LED1 PIN('B', 0)
#define LED2 PIN('E', 1)
#define LED3 PIN('B', 14)

int _write(int fd, char *ptr, int len) {
  if (fd == 1) hal_uart_write_buf(UART_DEBUG, ptr, (size_t) len);
  return len;
}

static void blink_task(void) {
  static uint64_t t = 0;
  if (hal_timer_expired(&t, 500, hal_get_tick())) hal_gpio_toggle(LED1);
}

uint64_t mg_millis(void) { return hal_get_tick(); }

bool mg_random(void *buf, size_t len) {
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = hal_rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), hal_get_tick());
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n", hal_get_tick());
    }
  }
}

// task1: network owner.  Runs the Mongoose event loop (mg_bsd_poll drives both
// the TCP/IP stack and the BSD command queue) and blinks the LED.
static void task1(void *args) {
  static struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
  for (;;) {
    mg_mgr_poll(&mgr, 0);
    mg_bsd_poll(&mgr);
    blink_task();
  }
  (void) args;
}

// client_task: connects to mongoose.ws:80, fetches "/", logs response length.
// Spawned when the echo server receives a message starting with "42".
static void client_task(void *args) {
  struct hostent *h = gethostbyname("mongoose.ws");
  if (!h) { MG_ERROR(("DNS failed")); vTaskDelete(NULL); return; }
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sa = {.sin_family = AF_INET, .sin_port = htons(80)};
  memcpy(&sa.sin_addr, h->h_addr, 4);
  if (connect(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    MG_ERROR(("connect failed")); close(fd); vTaskDelete(NULL); return;
  }
  const char *req = "GET / HTTP/1.0\r\nHost: mongoose.ws\r\n\r\n";
  send(fd, req, strlen(req), 0);
  size_t total = 0;
  char buf[512];
  ssize_t n;
  while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) total += (size_t) n;
  MG_INFO(("mongoose.ws response: %lu bytes", (unsigned long) total));
  close(fd);
  vTaskDelete(NULL);
  (void) args;
}

// echo_task: one per accepted connection.  Echoes data back.
// If a message starts with "42", also spawns a client_task to fetch mongoose.ws.
static void echo_task(void *args) {
  int fd = (int) (uintptr_t) args;
  char buf[512];
  ssize_t n;
  while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) {
    if (n >= 2 && buf[0] == '4' && buf[1] == '2')
      xTaskCreate(client_task, "client", 1024, NULL, configMAX_PRIORITIES - 1, NULL);
    send(fd, buf, (size_t) n, 0);
  }
  close(fd);
  vTaskDelete(NULL);
}

// task2: accept loop.  Waits for incoming connections on port 1234 and spawns
// an echo_task for each one, allowing concurrent clients.
static void task2(void *args) {
  mg_bsd_init();
  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sa = {.sin_family = AF_INET, .sin_port = htons(1234), .sin_addr = {INADDR_ANY}};
  bind(lfd, (struct sockaddr *) &sa, sizeof(sa));
  listen(lfd, 5);
  MG_INFO(("BSD echo server on :1234"));
  for (;;) {
    int fd = accept(lfd, NULL, NULL);
    if (fd < 0) continue;
    xTaskCreate(echo_task, "echo", 512, (void *) (uintptr_t) fd,
                configMAX_PRIORITIES - 1, NULL);
  }
  (void) args;
}

int main(void) {
  hal_clock_init();
  hal_uart_init(UART_DEBUG, UART_DEBUG_TX_PIN, UART_DEBUG_RX_PIN, 115200);
  hal_rng_init();
  hal_ethernet_init();
  hal_gpio_output(LED1);
  hal_gpio_output(LED2);
  hal_gpio_output(LED3);
  MG_INFO(("CPU clock: %lu MHz", SystemCoreClock / 1000000));

  // task2 at higher priority than task1 so that when task1 pushes data into
  // recv_q, task2 preempts immediately, echoes, and yields before MG_EV_CLOSE.
  xTaskCreate(task1, "task1", 2048, NULL, configMAX_PRIORITIES - 2, NULL);
  xTaskCreate(task2, "task2", 256,  NULL, configMAX_PRIORITIES - 1, NULL);
  vTaskStartScheduler();
  return 0;
}
