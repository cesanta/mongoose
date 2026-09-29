// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Four FreeRTOS tasks: network owner, TCP echo, UDP echo, and web client.

#include "hal.h"
#include "mongoose.h"

void echo_start(void);

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

uint64_t mg_millis(void) {
  return hal_get_tick();
}

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
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n",
                    hal_get_tick());
    }
  }
}

// Network owner. Runs the Mongoose event loop
static void net_core_task(void *arg) {
  MG_INFO(("Stating Mongoose event loop..."));
  for (;;) {
    mg_mgr_poll((struct mg_mgr *) arg, 0);
    blink_task();
  }
}

#define PRIO (configMAX_PRIORITIES - 1)

int main(void) {
  hal_clock_init();
  hal_uart_init(UART_DEBUG, UART_DEBUG_TX_PIN, UART_DEBUG_RX_PIN, 115200);
  hal_rng_init();
  hal_ethernet_init();
  hal_gpio_output(LED1);
  hal_gpio_output(LED2);
  hal_gpio_output(LED3);
  MG_INFO(("CPU clock: %lu MHz", SystemCoreClock / 1000000));

  static struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:80", http_ev_handler, NULL);
  mg_http_listen(&mgr, "udp://0.0.0.0:1236", NULL, NULL);

  xTaskCreate(net_core_task, "net_core", 2048, &mgr, PRIO, 0);
  echo_start();
  vTaskStartScheduler();
  return 0;
}
