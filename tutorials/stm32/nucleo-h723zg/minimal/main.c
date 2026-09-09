// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "hardfault.h"
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

struct crash_record g_hardfault_record;

void hal_crash_retrieve(struct crash_record *r) {
  volatile const uint32_t *src = (volatile const uint32_t *) D3_BKPSRAM_BASE;
  uint32_t *dst = (uint32_t *) r;
  size_t words = sizeof(*r) / sizeof(*dst);
  for (size_t i = 0; i < words; i++) dst[i] = src[i];
  if (r->magic != CRASH_RECORD_MAGIC) {
    for (size_t i = 0; i < words; i++) dst[i] = 0;
  }
}

void hal_crash_store(struct crash_record *r) {
  volatile uint32_t *dst = (volatile uint32_t *) D3_BKPSRAM_BASE;
  const uint32_t *src = (const uint32_t *) r;
  size_t words = sizeof(*r) / sizeof(*src);

  dst[0] = 0;  // Invalidate the old record before replacing it
  __DMB();
  for (size_t i = 1; i < words; i++) dst[i] = src[i];
  __DMB();
  dst[0] = src[0];  // Publish a complete record by writing magic last
  __DSB();
}

void hal_storage_init(void) {
  hal_backup_domain_init();
  RCC->AHB4ENR |= RCC_AHB4ENR_BKPRAMEN;
  (void) RCC->AHB4ENR;
  PWR->CR2 |= PWR_CR2_BREN;
  while ((PWR->CR2 & PWR_CR2_BRRDY) == 0) (void) 0;
}

struct mg_str serialize_crash_record(void) {
  static char buf[1024];
  struct crash_record *r = &g_hardfault_record;
  size_t n;

  if (r->magic != CRASH_RECORD_MAGIC) return mg_str("{\"valid\":false}");
  n = mg_snprintf(
      buf, sizeof(buf),
      "{\"valid\":true,\"cpuid\":\"%08lx\",\"sp\":\"%08lx\","
      "\"exc_return\":\"%08lx\",\"stack_valid\":%ld,"
      "\"r0\":\"%08lx\",\"r1\":\"%08lx\",\"r2\":\"%08lx\","
      "\"r3\":\"%08lx\",\"r12\":\"%08lx\",\"lr\":\"%08lx\","
      "\"pc\":\"%08lx\",\"psr\":\"%08lx\","
      "\"cfsr\":\"%08lx\",\"hfsr\":\"%08lx\","
      "\"dfsr\":\"%08lx\",\"afsr\":\"%08lx\","
      "\"bfar\":\"%08lx\",\"mmfar\":\"%08lx\","
      "\"abfsr\":\"%08lx\",\"caller_sp\":\"%08lx\","
      "\"caller_valid\":%ld,\"caller\":[\"%08lx\",\"%08lx\","
      "\"%08lx\",\"%08lx\"]}",
      (unsigned long) r->cpuid, (unsigned long) r->sp,
      (unsigned long) r->exc_return, (long) r->stack_valid,
      (unsigned long) r->r0, (unsigned long) r->r1,
      (unsigned long) r->r2, (unsigned long) r->r3,
      (unsigned long) r->r12, (unsigned long) r->lr,
      (unsigned long) r->pc, (unsigned long) r->psr,
      (unsigned long) r->cfsr, (unsigned long) r->hfsr,
      (unsigned long) r->dfsr, (unsigned long) r->afsr,
      (unsigned long) r->bfar, (unsigned long) r->mmfar,
      (unsigned long) r->abfsr, (unsigned long) r->caller_sp,
      (long) r->caller_valid, (unsigned long) r->caller[0],
      (unsigned long) r->caller[1], (unsigned long) r->caller[2],
      (unsigned long) r->caller[3]);
  if (n >= sizeof(buf)) n = sizeof(buf) - 1;
  return mg_str_n(buf, n);
}

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (hal_timer_expired(&blink_timer, 500, hal_get_tick())) {
    hal_gpio_toggle(LED1);
  }
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
  if (ev == MG_EV_HTTP_HDRS) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/ota/update"), NULL)) {
      mg_http_start_ota(c, hm, NULL);
    }
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), hal_get_tick());
    } else if (mg_match(hm->uri, mg_str("/api/ota/commit"), NULL)) {
      c->data[0] = 1;
      mg_http_reply(c, 200, "", "ok\n");
    } else if (mg_match(hm->uri, mg_str("/api/ota/rollback"), NULL)) {
      c->data[0] = 2;
      mg_http_reply(c, 200, "", "ok\n");
    } else if (mg_match(hm->uri, mg_str("/api/kill"), NULL)) {
      SCB->SHCSR &= ~SCB_SHCSR_USGFAULTENA_Msk;
      __asm volatile ("udf #0");
    } else if (mg_match(hm->uri, mg_str("/api/report"), NULL)) {
      struct mg_str report = serialize_crash_record();
      mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%.*s\n",
                    (int) report.len, report.buf);
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n", hal_get_tick());
    }
  } else if (ev == MG_EV_CLOSE && c->data[0] == 1) {
    MG_OTA_STATE_SET(MG_OTA_CONFIRMED);
    NVIC_SystemReset();
  } else if (ev == MG_EV_CLOSE && c->data[0] == 2) {
    MG_OTA_STATE_SET(MG_OTA_CONFIRMED);
    MG_OTA_ROLLBACK();
  }
}

int main(void) {
  hal_clock_init();
  hal_storage_init();
  hal_crash_retrieve(&g_hardfault_record);
  hal_uart_init(UART_DEBUG, UART_DEBUG_TX_PIN, UART_DEBUG_RX_PIN, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);
  hal_rng_init();
  hal_ethernet_init();
  hal_gpio_output(LED1);
  hal_gpio_output(LED2);
  hal_gpio_output(LED3);

  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));

  //MG_OTA_BOOT_CHECK();  // Must be called after clock init

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    blink_task();
  }

  return 0;
}
