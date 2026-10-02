#include "config.h"
#include "health.h"

// The one health record. Lives in RAM that survives a warm reset, see
// MG_HEALTH_RAM and the .mg_health region in the linker script
struct mg_health mg_health_record MG_HEALTH_RAM;

// STM32 H723 support. Needs the _estack symbol from the linker script
#if MG_ENABLE_HEALTH && defined(STM32H723xx_H)
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

  if (frame[7] & (1U << 9)) stack++;  // Eight-byte stack alignment padding
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
      mg_health_record.reset_reason == MG_HEALTH_RESET_FAULT) {
    size_t n = sizeof(mg_health_record.data) / sizeof(mg_health_record.data[0]);
    len += mg_xprintf(fn, arg, "{%m:%hhu,%m:%u,%m:[%M]}",            //
                      MG_ESC("version"), mg_health_record.magic[3],  //
                      MG_ESC("cpuid"), mg_health_record.cpuid,       //
                      MG_ESC("data"), print_uint32, n, mg_health_record.data);
  } else {
    len += mg_xprintf(fn, arg, "{}");
  }
  (void) ap;
  return len;
}
