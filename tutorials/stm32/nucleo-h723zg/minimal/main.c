// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

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

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (hal_timer_expired(&blink_timer, 500, hal_get_tick())) {
    hal_gpio_toggle(LED2);
  }
}

// Fault handler body. Runs in exception context: no printf, no malloc, no
// blocking calls. Records the crash reason and a backtrace into the health
// record, then resets.
// "used" keeps the linker from garbage-collecting this section: the only
// reference is the "b fault_c" branch in the naked handler below
__attribute__((used, noinline)) static void fault_c(uint32_t *sp) {
  extern uint8_t _estack;  // End of the main RAM region, defined in link.ld
  size_t n = 0;
  mg_health_record.reset_reason = MG_HEALTH_RESET_FAULT;
  // Frame 0 is the faulting PC, frame 1 the caller's LR. Deeper frames come
  // from a heuristic stack walk: BL pushes an odd return address that lives
  // in flash. A stack word that merely looks like an address shows up as a
  // bogus frame when symbolised, which is easy to filter by eye
  mg_health_record.backtrace[n++] = sp[6] & ~1U;  // Stacked PC
  mg_health_record.backtrace[n++] = sp[5] & ~1U;  // Stacked LR
  for (uint32_t *p = sp + 8;
       n < MG_HEALTH_BACKTRACE &&
       (uintptr_t) p < (uintptr_t) sp + 4096U &&   // Bound the scan
       (uintptr_t) p < (uintptr_t) &_estack;       // Stay in RAM
       p++) {
    uint32_t v = *p;
    if ((v & 1U) && v >= 0x08000000U && v < 0x08000000U + 1024U * 1024U) {
      mg_health_record.backtrace[n++] = v & ~1U;
    }
  }
  NVIC_SystemReset();
}

// Common fault entry. EXC_RETURN bit 2 tells which stack was in use:
// 0 = MSP, 1 = PSP. Load the faulting SP into r0 and hand it to fault_c()
__attribute__((naked)) void HardFault_Handler(void) {
  __asm volatile(
      "tst lr, #4\n\t"   // Test EXC_RETURN bit 2
      "ite eq\n\t"       // If zero, use MSP; else PSP
      "mrseq r0, msp\n\t"
      "mrsne r0, psp\n\t"
      "b fault_c\n\t");
}

// Route the other fault types through the same entry point
void MemManage_Handler(void) __attribute__((alias("HardFault_Handler")));
void BusFault_Handler(void) __attribute__((alias("HardFault_Handler")));
void UsageFault_Handler(void) __attribute__((alias("HardFault_Handler")));

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

  // Enable MemManage, BusFault and UsageFault so faults are precise and
  // routed to their own handlers instead of escalating to HardFault
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk |
                SCB_SHCSR_USGFAULTENA_Msk;

  MG_OTA_BOOT_CHECK();  // Must be called after clock init
  MG_HEALTH_INIT();     // Must be called after clock init

  hal_uart_init(UART_DEBUG, UART_DEBUG_TX_PIN, UART_DEBUG_RX_PIN, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);
  hal_rng_init();
  hal_ethernet_init();
  hal_gpio_output(LED1);
  hal_gpio_output(LED2);
  hal_gpio_output(LED3);

  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));
  // Report the previous boot's crash backtrace, if any
  if (mg_health_reason() == MG_HEALTH_RESET_FAULT) {
    MG_INFO(("Previous boot faulted. Backtrace:"));
    for (int i = 0; i < MG_HEALTH_BACKTRACE; i++) {
      if (mg_health_record.backtrace[i] == 0) break;
      MG_INFO(("  #%d 0x%08lx", i,
               (unsigned long) mg_health_record.backtrace[i]));
    }
  }
  * (volatile uint32_t *) 0 = 123;  // Deliberate fault: write to unmapped memory
  for (;;) (void) 0;

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_mdash_init(&mgr);  // Token comes from MG_MDASH_KEY in mongoose_config.h
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    mg_mdash_poll(&mgr);
    blink_task();
  }

  return 0;
}
