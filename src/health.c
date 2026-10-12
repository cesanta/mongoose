#include "config.h"
#include "health.h"

#if MG_ENABLE_HEALTH
// The one health record. Lives in RAM that survives a warm reset, see
// MG_HEALTH_RAM and the .mg_health region in the linker script
struct mg_health mg_health_record MG_HEALTH_RAM;

// STM32 H7, H5, C5 support. Needs the _estack symbol from the linker script
#if defined(STM32H7_DEV_ID) || defined(STM32H5) || defined(STM32C5)
// Fault handler body. Runs in exception context: no printf, no malloc, no
// blocking calls. Copies the unwind registers and raw stack into the health
// record, then resets.
// "used" keeps the linker from garbage-collecting this section: the only
// reference is the "b fault_c" branch in the naked handler below
__attribute__((used, noinline)) static void fault_c(uint32_t *frame,
                                                    uint32_t exc_return) {
  extern uint32_t _estack;
  uint32_t *stack = frame + 8 + ((exc_return & (1U << 4)) ? 0 : 18);
  uint32_t *r = mg_health_record.data;

  if (frame[7] & (1U << 9)) stack++;    // Eight-byte stack alignment padding
  r[0] = (uint32_t) (uintptr_t) stack;  // SP
  r[1] = frame[5];                      // LR
  r[2] = frame[6];                      // PC
  for (size_t i = 0; i < MG_HEALTH_DATA_SIZE - 3; i++) {  // Raw stack
    r[3 + i] = stack + i < &_estack ? stack[i] : 0;
  }
  mg_health_record.cpuid = SCB->CPUID;
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
#endif

// Start a new boot: validate the record. Called by MG_HEALTH_INIT()
void mg_health_init(void) {
  char magic[] = MG_HEALTH_MAGIC;
  if (!mg_health_valid()) {  // First boot, or RAM lost its contents
    memset(&mg_health_record, 0, sizeof(mg_health_record));
    memcpy(mg_health_record.magic, magic, sizeof(mg_health_record.magic));
  }
#if MG_ARCH == MG_ARCH_ESP32
#if CONFIG_IDF_TARGET_ARCH_XTENSA
  mg_health_record.arch = (uint16_t) MG_HEALTH_ARCH_XTENSA;
#elif CONFIG_IDF_TARGET_ARCH_RISCV
  mg_health_record.arch = (uint16_t) MG_HEALTH_ARCH_RISCV;
#endif
  esp_reset_reason_t rr = esp_reset_reason();
  mg_health_record.reset_reason = esp2mg(rr);
#if CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH == 0
  MG_ERROR(("MG_HEALTH is enabled but CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH=n"));
#else
    // After a crash, copy the backtrace from the core dump in flash: the same
    // addresses the panic handler prints as "Backtrace: PC:SP PC:SP ...".
    // The core dump stays in flash, so check the reset reason, otherwise an
    // old crash is reported on every boot
    esp_core_dump_summary_t *s = calloc(1, sizeof(*s));
    uint32_t *out = mg_health_record.data;
    size_t n = 0;
    memset(out, 0, sizeof(mg_health_record.data));
    if (s == NULL) return;
    if ((rr != ESP_RST_PANIC && rr != ESP_RST_INT_WDT &&
         rr != ESP_RST_TASK_WDT)  // Not a crash caught by the panic handler
        || esp_core_dump_get_summary(s) != ESP_OK) {
      free(s);
      return;
    }
    out[n++] = s->exc_pc;
    memcpy(&out[n], s->exc_task, 16), n += 4;
#if CONFIG_IDF_TARGET_ARCH_XTENSA
    out[n++] = s->ex_info.exc_a[0];
    out[n++] = s->ex_info.exc_a[1];
    size_t len = s->exc_bt_info.depth;
    if (len > MG_HEALTH_DATA_SIZE - n) len = (MG_HEALTH_DATA_SIZE - n);
    memcpy(&out[n], s->exc_bt_info.bt, 4 * len); // sizeof(uint32_t)
#elif CONFIG_IDF_TARGET_ARCH_RISCV
    out[n++] = s->ex_info.exc_ra;
    out[n++] = s->ex_info.exc_sp;
    size_t len = s->exc_bt_info.dump_size / 4 ; // sizeof(uint32_t)
    if (len > MG_HEALTH_DATA_SIZE - n) len = (MG_HEALTH_DATA_SIZE - n);
    memcpy(&out[n], s->exc_bt_info.stackdump, 4 * len);
#endif
    free(s);
#endif  // CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH
#endif  // MG_ARCH_ESP32
}

static size_t print_uint32(void (*fn)(char, void *), void *arg, va_list *ap) {
  size_t n = va_arg(*ap, size_t);
  uint32_t *p = va_arg(*ap, uint32_t *);
  size_t len = 0;
  while (n--) len += mg_xprintf(fn, arg, "%s%lu", len == 0 ? "" : ",", *p++);
  return len;
}

size_t mg_print_crash_record(void (*fn)(char, void *), void *arg, va_list *ap) {
  size_t len = 0;
  if (mg_health_valid() &&
      (mg_health_record.reset_reason == (uint16_t) MG_HEALTH_RESET_FAULT
#if MG_ARCH == MG_ARCH_ESP32
       || mg_health_record.reset_reason == (uint16_t) MG_HEALTH_RESET_WATCHDOG
#endif
       )) {
    size_t n = sizeof(mg_health_record.data) / sizeof(mg_health_record.data[0]);
    len += mg_xprintf(fn, arg, "{%m:%hhu,%m:%hu,%m:%hu,%m:%u,%m:[%M]}",  //
                      MG_ESC("version"), mg_health_record.magic[3],      //
                      MG_ESC("arch"), mg_health_record.arch,             //
                      MG_ESC("reason"), mg_health_record.reset_reason,   //
                      MG_ESC("cpuid"), mg_health_record.cpuid,           //
                      MG_ESC("data"), print_uint32, n, mg_health_record.data);
  } else {
    len += mg_xprintf(fn, arg, "{}");
  }
  (void) ap;
  return len;
}
#endif
