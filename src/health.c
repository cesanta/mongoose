#include "arch.h"
#include "config.h"
#include "health.h"

#if MG_ENABLE_HEALTH
#if !MG_ENABLE_CUSTOM_USAGE
#if MG_ARCH == MG_ARCH_ESP32
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#elif MG_ENABLE_FREERTOS && defined(__GNUC__) && defined(__ELF__)
// Some FreeRTOS allocators provide current free bytes but no low watermark.
extern size_t xPortGetMinimumEverFreeHeapSize(void) __attribute__((weak));
#endif

#if MG_ENABLE_FREERTOS && MG_ARCH == MG_ARCH_CUBE && \
    configUSE_TRACE_FACILITY == 1 && configRECORD_STACK_HIGH_ADDRESS != 1
#warning \
    "Set configRECORD_STACK_HIGH_ADDRESS to 1 in FreeRTOSConfig.h to report stack total and peak"
#endif

size_t mg_usage(int task, int metric) {
#if MG_ARCH == MG_ARCH_ESP32 || \
    (MG_ENABLE_FREERTOS && defined(configTOTAL_HEAP_SIZE))
  if (metric == MG_USAGE_HEAP_TOTAL || metric == MG_USAGE_HEAP_USED ||
      metric == MG_USAGE_HEAP_PEAK) {
    size_t total, available;
#if MG_ARCH == MG_ARCH_ESP32
    total = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    if (metric == MG_USAGE_HEAP_TOTAL) return total;
    available = metric == MG_USAGE_HEAP_USED
                    ? heap_caps_get_free_size(MALLOC_CAP_DEFAULT)
                    : heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
#else
    total = configTOTAL_HEAP_SIZE;
    if (metric == MG_USAGE_HEAP_TOTAL) return total;
    if (metric == MG_USAGE_HEAP_USED) {
      available = xPortGetFreeHeapSize();
    } else {
#if defined(__GNUC__) && defined(__ELF__)
      if (xPortGetMinimumEverFreeHeapSize == NULL) return 0;
      available = xPortGetMinimumEverFreeHeapSize();
#else
      return 0;
#endif
    }
#endif
    return available <= total ? total - available : 0;
  }
#endif

#if MG_ENABLE_FREERTOS || MG_ARCH == MG_ARCH_ESP32
#if configUSE_TRACE_FACILITY == 1
  UBaseType_t count = uxTaskGetNumberOfTasks();
  if (metric == MG_USAGE_NUM_TASKS) return (size_t) count;
  if (task >= 0 && (size_t) task < count) {
    TaskStatus_t tasks[count];
    if ((size_t) task >= uxTaskGetSystemState(tasks, count, NULL)) return 0;
    TaskStatus_t *t = &tasks[task];
    if (metric == MG_USAGE_TASK_NAME) return (size_t) t->pcTaskName;
#if (MG_ARCH == MG_ARCH_CUBE || MG_ARCH == MG_ARCH_ESP32) && \
    configRECORD_STACK_HIGH_ADDRESS == 1
    size_t total = 0;
    uintptr_t tcb = (uintptr_t) t->xHandle;
    uintptr_t base = (uintptr_t) t->pxStackBase;
    if (tcb != 0 &&
        t->pcTaskName == (const char *) (tcb + offsetof(StaticTask_t, ucDummy7))) {
      StackType_t *end;
      uintptr_t field = tcb + offsetof(StaticTask_t, pxDummy8);
      memcpy(&end, (const void *) field, sizeof(end));
      if (base != 0 && (uintptr_t) end >= base &&
          (uintptr_t) end - base <= ((size_t) -1) - sizeof(*end))
        total = (size_t) ((uintptr_t) end - base) + sizeof(*end);
    }
    if (metric == MG_USAGE_STACK_TOTAL) return total;
    if (metric == MG_USAGE_STACK_PEAK) {
      size_t available =
          (size_t) t->usStackHighWaterMark * sizeof(StackType_t);
      return available <= total ? total - available : 0;
    }
#endif
  }
#endif
#endif
  (void) task, (void) metric;
  return 0;
}
#endif

static size_t print_mem_stat(mg_pfn_t fn, void *arg, const char *name,
                             size_t total, size_t used, size_t peak) {
  return mg_xprintf(fn, arg, "{%m:%m,%m:%zu,%m:%zu,%m:%zu}", MG_ESC("name"),
                    MG_ESC(name == NULL ? "" : name), MG_ESC("total"), total,
                    MG_ESC("used"), used, MG_ESC("peak"), peak);
}

size_t mg_print_mem_stats(mg_pfn_t fn, void *arg, va_list *ap) {
  int i, count;
  size_t heap_total = mg_usage(0, MG_USAGE_HEAP_TOTAL);
  size_t heap_used = mg_usage(0, MG_USAGE_HEAP_USED);
  size_t heap_peak = mg_usage(0, MG_USAGE_HEAP_PEAK);
  size_t len = mg_xprintf(fn, arg, "[");
  len += print_mem_stat(fn, arg, "heap", heap_total, heap_used, heap_peak);
  count = (int) mg_usage(-1, MG_USAGE_NUM_TASKS);
  for (i = 0; i < count; i++) {
    const char *name = (const char *) mg_usage(i, MG_USAGE_TASK_NAME);
    len += mg_xprintf(fn, arg, ",");
    len += print_mem_stat(fn, arg, name, mg_usage(i, MG_USAGE_STACK_TOTAL),
                          mg_usage(i, MG_USAGE_STACK_USED),
                          mg_usage(i, MG_USAGE_STACK_PEAK));
  }
  (void) ap;
  return len + mg_xprintf(fn, arg, "]");
}

#if MG_ARCH == MG_ARCH_ESP32
// reusable for other platforms, currently ESP32-only
struct mg_health_coredump {
  const void *ptr;
  size_t poffset;
  size_t size;
};

static struct mg_health_coredump s_coredump;
#endif

bool mg_health_coredump_begin(size_t *size) {
#if MG_ARCH == MG_ARCH_ESP32 && CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH
  const esp_partition_t *p = esp_partition_find_first(
      ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_COREDUMP, NULL);
  size_t address = 0, len = 0;
  esp_err_t err = esp_core_dump_image_get(&address, &len);
  if (size == NULL || s_coredump.ptr != NULL || err != ESP_OK || p == NULL)
    return false;
  s_coredump.ptr = p;
  s_coredump.poffset = address - p->address;
  s_coredump.size = len;
  *size = len;
  return true;
#else
  (void) size;
  return false;
#endif
}

bool mg_health_coredump_read(size_t offset, void *buf, size_t *len) {
#if MG_ARCH == MG_ARCH_ESP32 && CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH
  const esp_partition_t *p = (const esp_partition_t *) s_coredump.ptr;
  if (p == NULL || offset > s_coredump.size) return false;
  if (*len > s_coredump.size - offset) *len = s_coredump.size - offset;
  return esp_partition_read(p, s_coredump.poffset + offset, buf, *len) ==
         ESP_OK;
#else
  (void) offset, (void) buf, (void) len;
  return false;
#endif
}

bool mg_health_coredump_end(void) {
#if MG_ARCH == MG_ARCH_ESP32 && CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH
  bool ok = s_coredump.ptr != NULL;
  memset(&s_coredump, 0, sizeof(s_coredump));
  return ok;
#else
  return false;
#endif
}

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
  mg_health_record.arch = MG_HEALTH_ARCH_ARM;
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
  mg_health_record.arch = MG_HEALTH_ARCH_XTENSA;
#elif CONFIG_IDF_TARGET_ARCH_RISCV
  mg_health_record.arch = MG_HEALTH_ARCH_RISCV;
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
  esp_core_dump_summary_t s;
  uint32_t *out = mg_health_record.data;
  size_t n = 0;
  memset(out, 0, sizeof(mg_health_record.data));
  if ((rr != ESP_RST_PANIC && rr != ESP_RST_INT_WDT &&
       rr != ESP_RST_TASK_WDT) ||
      esp_core_dump_get_summary(&s) != ESP_OK) {
    return;  //  If not panic, and if cannot get crash info, give up
  }
  out[n++] = s.exc_pc;                      // Crash PC
  memcpy(&out[n], s.exc_task, 16), n += 4;  // Crashed task name
#if CONFIG_IDF_TARGET_ARCH_XTENSA
  out[n++] = s.ex_info.exc_a[0];
  out[n++] = s.ex_info.exc_a[1];
  size_t len = s.exc_bt_info.depth;
  if (len > MG_HEALTH_DATA_SIZE - n) len = MG_HEALTH_DATA_SIZE - n;
  memcpy(&out[n], s.exc_bt_info.bt, 4 * len);
#elif CONFIG_IDF_TARGET_ARCH_RISCV
  out[n++] = s.ex_info.ra;
  out[n++] = s.ex_info.sp;
  size_t len = s.exc_bt_info.dump_size / 4;
  if (len > MG_HEALTH_DATA_SIZE - n) len = MG_HEALTH_DATA_SIZE - n;
  memcpy(&out[n], s.exc_bt_info.stackdump, 4 * len);
#endif
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
      (mg_health_record.reset_reason == MG_HEALTH_RESET_FAULT ||
       mg_health_record.reset_reason == MG_HEALTH_RESET_WATCHDOG)) {
    size_t n = sizeof(mg_health_record.data) / sizeof(mg_health_record.data[0]);
    len += mg_xprintf(fn, arg, "{%m:%hhu,%m:%lu,%m:%lu,%m:%u,%m:[%M]}",  //
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
