// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "hardfault.h"
#include "mongoose.h"
#include <stdarg.h>
#include <unwind.h>

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
  if (r->magic != CRASH_RECORD_MAGIC ||
      r->version != CRASH_RECORD_VERSION ||
      r->stack_word_count > CRASH_STACK_WORDS) {
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

static int json_append(char *buf, size_t size, size_t *offset,
                       const char *fmt, ...) {
  va_list ap;
  size_t available, n;

  if (*offset >= size) return 0;
  available = size - *offset;
  va_start(ap, fmt);
  n = mg_vsnprintf(buf + *offset, available, fmt, &ap);
  va_end(ap);
  if (n >= available) {
    *offset = size;
    return 0;
  }
  *offset += n;
  return 1;
}

struct mg_str serialize_crash_record(void) {
  static char buf[16384];
  struct crash_record *r = &g_hardfault_record;
  size_t n = 0;
  int ok;

  if (r->magic != CRASH_RECORD_MAGIC) return mg_str("{\"valid\":false}");
  ok = json_append(
      buf, sizeof(buf), &n,
      "{\"valid\":true,\"version\":%lu,\"arch\":\"armv7e-m\","
      "\"core\":\"cortex-m7\",\"image\":\"firmware.elf\","
      "\"map\":\"firmware.elf.map\",\"binary\":\"firmware.bin\","
      "\"cpuid\":\"0x%08lx\",\"exception_sp\":\"0x%08lx\","
      "\"exc_return\":\"0x%08lx\",\"exception_valid\":%s,"
      "\"stack_valid\":%s,\"regs\":{"
      "\"r0\":\"0x%08lx\",\"r1\":\"0x%08lx\","
      "\"r2\":\"0x%08lx\",\"r3\":\"0x%08lx\","
      "\"r4\":\"0x%08lx\",\"r5\":\"0x%08lx\","
      "\"r6\":\"0x%08lx\",\"r7\":\"0x%08lx\","
      "\"r8\":\"0x%08lx\",\"r9\":\"0x%08lx\","
      "\"r10\":\"0x%08lx\",\"r11\":\"0x%08lx\","
      "\"r12\":\"0x%08lx\",\"sp\":\"0x%08lx\","
      "\"lr\":\"0x%08lx\",\"pc\":\"0x%08lx\","
      "\"xpsr\":\"0x%08lx\"},\"fault\":{"
      "\"cfsr\":\"0x%08lx\",\"hfsr\":\"0x%08lx\","
      "\"dfsr\":\"0x%08lx\",\"afsr\":\"0x%08lx\","
      "\"mmfar\":\"0x%08lx\",\"bfar\":\"0x%08lx\","
      "\"abfsr\":\"0x%08lx\"},\"stack\":{"
      "\"addr\":\"0x%08lx\",\"word_size\":4,\"words\":[",
      (unsigned long) r->version, (unsigned long) r->cpuid,
      (unsigned long) r->exception_sp, (unsigned long) r->exc_return,
      r->exception_valid > 0 ? "true" : "false",
      r->stack_valid > 0 ? "true" : "false",
      (unsigned long) r->r0, (unsigned long) r->r1,
      (unsigned long) r->r2, (unsigned long) r->r3,
      (unsigned long) r->r4, (unsigned long) r->r5,
      (unsigned long) r->r6, (unsigned long) r->r7,
      (unsigned long) r->r8, (unsigned long) r->r9,
      (unsigned long) r->r10, (unsigned long) r->r11,
      (unsigned long) r->r12, (unsigned long) r->sp,
      (unsigned long) r->lr, (unsigned long) r->pc,
      (unsigned long) r->psr, (unsigned long) r->cfsr,
      (unsigned long) r->hfsr, (unsigned long) r->dfsr,
      (unsigned long) r->afsr, (unsigned long) r->mmfar,
      (unsigned long) r->bfar, (unsigned long) r->abfsr,
      (unsigned long) r->sp);
  for (size_t i = 0; ok && i < r->stack_word_count; i++) {
    ok = json_append(buf, sizeof(buf), &n, "%s\"0x%08lx\"",
                     i == 0 ? "" : ",",
                     (unsigned long) r->stack_words[i]);
  }
  if (ok) ok = json_append(buf, sizeof(buf), &n, "]}}");
  if (!ok)
    return mg_str("{\"valid\":false,\"error\":\"JSON overflow\"}");
  return mg_str_n(buf, n);
}

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (hal_timer_expired(&blink_timer, 500, hal_get_tick())) {
    hal_gpio_toggle(LED2);
  }
}

// Route the other fault types through the same entry point
//void MemManage_Handler(void) __attribute__((alias("HardFault_Handler")));
//void BusFault_Handler(void) __attribute__((alias("HardFault_Handler")));
//void UsageFault_Handler(void) __attribute__((alias("HardFault_Handler")));

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
    } else if (mg_match(hm->uri, mg_str("/api/kill"), NULL)) {
      SCB->SHCSR &= ~SCB_SHCSR_USGFAULTENA_Msk;
      __asm volatile("udf #0");
    }  else if (mg_match(hm->uri, mg_str("/api/report"), NULL)) {
      struct mg_str report = serialize_crash_record();
      mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%.*s\n",
                    (int) report.len, report.buf);
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n",
                    hal_get_tick());
    }
  }
}

int main(void) {
  MG_HEALTH_INIT();
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
