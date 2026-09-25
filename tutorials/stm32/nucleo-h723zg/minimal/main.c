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

enum { CRASH_SP, CRASH_LR, CRASH_PC, CRASH_STACK };

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

static void hal_storage_init(void) {
  RCC->AHB4ENR |= RCC_AHB4ENR_BKPRAMEN;
  (void) RCC->AHB4ENR;
  PWR->CR2 |= PWR_CR2_BREN;
  while ((PWR->CR2 & PWR_CR2_BRRDY) == 0) (void) 0;
}

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (hal_timer_expired(&blink_timer, 500, hal_get_tick())) {
    hal_gpio_toggle(LED2);
  }
}

// Fault handler body. Runs in exception context: no printf, no malloc, no
// blocking calls. Copies the unwind registers and raw stack into the health
// record, then resets.
// "used" keeps the linker from garbage-collecting this section: the only
// reference is the "b fault_c" branch in the naked handler below
__attribute__((used, noinline)) static void fault_c(uint32_t *frame,
                                                     uint32_t exc_return) {
  extern uint32_t _estack;
  uint32_t *stack = frame + 8 + ((exc_return & (1U << 4)) ? 0 : 18);
  uint32_t *r = mg_health_record.backtrace;

  if (frame[7] & (1U << 9)) stack++;  // Eight-byte stack alignment padding
  r[CRASH_SP] = (uint32_t) (uintptr_t) stack;
  r[CRASH_LR] = frame[5];
  r[CRASH_PC] = frame[6];
  for (size_t i = 0; i < MG_HEALTH_BACKTRACE - CRASH_STACK; i++) {
    r[CRASH_STACK + i] = stack + i < &_estack ? stack[i] : 0;
  }
  mg_health_record.reset_reason = MG_HEALTH_RESET_FAULT;
  NVIC_SystemReset();
}

// Common fault entry. EXC_RETURN bit 2 tells which stack was in use:
// 0 = MSP, 1 = PSP. Load the faulting SP into r0 and hand it to fault_c()
__attribute__((naked)) void HardFault_Handler(void) {
  __asm volatile(
      "mov r1, lr\n\t"
      "tst lr, #4\n\t"
      "ite eq\n\t"
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
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), hal_get_tick());
    } else if (mg_match(hm->uri, mg_str("/api/kill"), NULL)) {
      SCB->SHCSR &= ~SCB_SHCSR_USGFAULTENA_Msk;
      __asm volatile("udf #0");
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n",
                    hal_get_tick());
    }
  }
}

int main(void) {
  hal_clock_init();
  hal_storage_init();
  MG_HEALTH_INIT();  // Must be called after clock init
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
    uint32_t *r = mg_health_record.backtrace;
    static char buf[512];
    size_t len = mg_snprintf(
        buf, sizeof(buf),
        "{\"image\":\"firmware.elf\",\"binary\":\"firmware.bin\","
        "\"regs\":{\"sp\":\"0x%08lx\",\"lr\":\"0x%08lx\","
        "\"pc\":\"0x%08lx\"},\"stack\":{\"addr\":\"0x%08lx\","
        "\"words\":[",
        (unsigned long) r[CRASH_SP], (unsigned long) r[CRASH_LR],
        (unsigned long) r[CRASH_PC], (unsigned long) r[CRASH_SP]);
    for (size_t i = CRASH_STACK; i < MG_HEALTH_BACKTRACE; i++) {
      len += mg_snprintf(buf + len, sizeof(buf) - len, "%s\"0x%08lx\"",
                         i == CRASH_STACK ? "" : ",",
                         (unsigned long) r[i]);
    }
    mg_snprintf(buf + len, sizeof(buf) - len, "]}}");
    MG_INFO(("Previous boot crash: %s", buf));
  }

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    blink_task();
  }

  return 0;
}
