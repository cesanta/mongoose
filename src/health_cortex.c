#include "health_cortex.h"
#include "printf.h"

#if MG_HEALTH == MG_HEALTH_CORTEX

#define MG_SCB_CPUID (*(volatile uint32_t *) 0xe000ed00)
#define MG_SCB_CFSR (*(volatile uint32_t *) 0xe000ed28)
#define MG_SCB_HFSR (*(volatile uint32_t *) 0xe000ed2c)
#define MG_SCB_DFSR (*(volatile uint32_t *) 0xe000ed30)
#define MG_SCB_MMFAR (*(volatile uint32_t *) 0xe000ed34)
#define MG_SCB_BFAR (*(volatile uint32_t *) 0xe000ed38)
#define MG_SCB_AFSR (*(volatile uint32_t *) 0xe000ed3c)
#define MG_SCB_ABFSR (*(volatile uint32_t *) 0xe000efa8)

#define MG_CPU_CORTEX_M0 0xc20
#define MG_CPU_CORTEX_M23 0xd20
#define MG_CPU_CORTEX_M3 0xc23
#define MG_CPU_CORTEX_M4 0xc24
#define MG_CPU_CORTEX_M7 0xc27
#define MG_CPU_CORTEX_M0P 0xc60
#define MG_CPU_CORTEX_M33 0xd21
#define MG_CPU_CORTEX_M35P 0xd31
#define MG_CPU_CORTEX_M52 0xd24
#define MG_CPU_CORTEX_M55 0xd22
#define MG_CPU_CORTEX_M85 0xd23

extern uint32_t __StackLimit __attribute__((weak));
extern uint32_t __StackTop __attribute__((weak));
extern uint32_t _sstack __attribute__((weak));
extern uint32_t _estack __attribute__((weak));
extern bool mg_health_cortex_stack_range(const void *, uintptr_t *, uintptr_t *)
    __attribute__((weak));

static struct mg_health_cortex_record s_record;
static char s_report[MG_HEALTH_CORTEX_REPORT_SIZE];

static bool storage_init(void *storage, size_t size) {
  // Note: actual init is handled in main()
  (void) storage;
  (void) size;
  return true;
}

static bool storage_read(const void *storage, size_t offset, void *buf,
                         size_t len) {
  volatile const uint8_t *src = (volatile const uint8_t *) storage + offset;
  uint8_t *dst = (uint8_t *) buf;
  size_t i;
  for (i = 0; i < len; i++) dst[i] = src[i];
  return true;
}

static bool storage_write(void *storage, size_t offset, const void *buf,
                          size_t len) {
  volatile uint8_t *dst = (volatile uint8_t *) storage + offset;
  const uint8_t *src = (const uint8_t *) buf;
  size_t i;
  for (i = 0; i < len; i++) dst[i] = src[i];
  return true;
}

struct mg_health_cortex mg_health_cortex = {
    MG_HEALTH_CORTEX_STORAGE,
    MG_HEALTH_CORTEX_STORAGE_SIZE,
    storage_init,
    storage_read,
    storage_write,
};

static bool storage_bounds(const struct mg_health_cortex *ctx, size_t offset,
                           size_t len) {
  return ctx != NULL && ctx->storage != NULL && offset <= ctx->storage_size &&
         len <= ctx->storage_size - offset;
}

static bool storage_configured(const struct mg_health_cortex *ctx) {
  return ctx != NULL && ctx->storage != NULL && ctx->storage_size != 0 &&
         ctx->storage_size >= sizeof(struct mg_health_cortex_record) &&
         ctx->init != NULL && ctx->read != NULL && ctx->write != NULL;
}

static bool record_read(struct mg_health_cortex *ctx,
                        struct mg_health_cortex_record *record) {
  static const char magic[] = MG_HEALTH_MAGIC;
  if (ctx == NULL || ctx->read == NULL ||
      !storage_bounds(ctx, 0, sizeof(*record)) ||
      !ctx->read(ctx->storage, 0, record, sizeof(*record))) {
    return false;
  }
  return memcmp(record->magic, magic, sizeof(record->magic)) == 0 &&
         record->version == MG_HEALTH_CORTEX_VERSION &&
         record->stack_word_count <= MG_HEALTH_CORTEX_STACK_WORDS;
}

static bool record_write(struct mg_health_cortex *ctx,
                         const struct mg_health_cortex_record *record) {
  char invalid[sizeof(record->magic)] = {0};
  size_t offset = sizeof(record->magic);
  if (ctx == NULL || ctx->write == NULL ||
      !storage_bounds(ctx, 0, sizeof(*record)) ||
      !ctx->write(ctx->storage, 0, &invalid, sizeof(invalid))) {
    return false;
  }
  if (!ctx->write(ctx->storage, offset, (const uint8_t *) record + offset,
                  sizeof(*record) - offset)) {
    return false;
  }
  if (!ctx->write(ctx->storage, 0, record->magic, sizeof(record->magic)))
    return false;
  return true;
}

static bool stack_range(const void *ptr, uintptr_t *lo, uintptr_t *hi) {
  if (mg_health_cortex_stack_range != NULL &&
      mg_health_cortex_stack_range(ptr, lo, hi)) {
    return *lo < *hi;
  } else if (&__StackLimit != NULL && &__StackTop != NULL) {
    *lo = (uintptr_t) &__StackLimit;
    *hi = (uintptr_t) &__StackTop;
  } else if (&_sstack != NULL && &_estack != NULL) {
    *lo = (uintptr_t) &_sstack;
    *hi = (uintptr_t) &_estack;
  } else {
    return false;
  }
  return *lo < *hi;
}

static int stack_valid(const void *ptr, size_t words) {
  uintptr_t lo, hi, p = (uintptr_t) ptr;
  if ((p & 3U) != 0) return 0;
  if (!stack_range(ptr, &lo, &hi)) return -1;
  return p >= lo && p <= hi && words <= (hi - p) / sizeof(uint32_t);
}

static bool has_fault_status(uint32_t part) {
  return part == MG_CPU_CORTEX_M3 || part == MG_CPU_CORTEX_M4 ||
         part == MG_CPU_CORTEX_M7 || part == MG_CPU_CORTEX_M33 ||
         part == MG_CPU_CORTEX_M35P || part == MG_CPU_CORTEX_M52 ||
         part == MG_CPU_CORTEX_M55 || part == MG_CPU_CORTEX_M85;
}

static const char *core_name(uint32_t part) {
  switch (part) {
    case MG_CPU_CORTEX_M0:
      return "cortex-m0";
    case MG_CPU_CORTEX_M0P:
      return "cortex-m0+";
    case MG_CPU_CORTEX_M23:
      return "cortex-m23";
    case MG_CPU_CORTEX_M3:
      return "cortex-m3";
    case MG_CPU_CORTEX_M4:
      return "cortex-m4";
    case MG_CPU_CORTEX_M7:
      return "cortex-m7";
    case MG_CPU_CORTEX_M33:
      return "cortex-m33";
    case MG_CPU_CORTEX_M35P:
      return "cortex-m35p";
    case MG_CPU_CORTEX_M52:
      return "cortex-m52";
    case MG_CPU_CORTEX_M55:
      return "cortex-m55";
    case MG_CPU_CORTEX_M85:
      return "cortex-m85";
    default:
      return "unknown";
  }
}

static const char *arch_name(uint32_t part) {
  if (part == MG_CPU_CORTEX_M0 || part == MG_CPU_CORTEX_M0P) return "armv6-m";
  if (part == MG_CPU_CORTEX_M3) return "armv7-m";
  if (part == MG_CPU_CORTEX_M4 || part == MG_CPU_CORTEX_M7) return "armv7e-m";
  return "armv8-m";
}

static bool json_append(char *buf, size_t size, size_t *offset, const char *fmt,
                        ...) {
  va_list ap;
  size_t available, n;
  if (*offset >= size) return false;
  available = size - *offset;
  va_start(ap, fmt);
  n = mg_vsnprintf(buf + *offset, available, fmt, &ap);
  va_end(ap);
  if (n >= available) {
    *offset = size;
    return false;
  }
  *offset += n;
  return true;
}

void mg_health_cortex_init(struct mg_health_cortex *ctx) {
  if (storage_configured(ctx)) {
    (void) ctx->init(ctx->storage, ctx->storage_size);
  }
}

struct mg_str mg_health_cortex_get_blob(void *data) {
  struct mg_health_cortex *ctx = (struct mg_health_cortex *) data;
  struct mg_health_cortex_record *r = &s_record;
  uint32_t part;
  size_t n = 0, i;
  bool ok;

  if (!storage_configured(ctx)) {
    return mg_str(
        "{\"valid\":false,\"error\":\"health storage is not configured\"}");
  }
  if (!record_read(ctx, r)) return mg_str("{\"valid\":false}");
  part = (r->cpuid >> 4) & 0xfffU;
  ok = json_append(
      s_report, sizeof(s_report), &n,
      "{\"valid\":true,\"version\":%lu,\"arch\":\"%s\","
      "\"core\":\"%s\",\"image\":\"firmware.elf\","
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
      (unsigned long) r->version, arch_name(part), core_name(part),
      (unsigned long) r->cpuid, (unsigned long) r->exception_sp,
      (unsigned long) r->exc_return, r->exception_valid > 0 ? "true" : "false",
      r->stack_valid > 0 ? "true" : "false", (unsigned long) r->r0,
      (unsigned long) r->r1, (unsigned long) r->r2, (unsigned long) r->r3,
      (unsigned long) r->r4, (unsigned long) r->r5, (unsigned long) r->r6,
      (unsigned long) r->r7, (unsigned long) r->r8, (unsigned long) r->r9,
      (unsigned long) r->r10, (unsigned long) r->r11, (unsigned long) r->r12,
      (unsigned long) r->sp, (unsigned long) r->lr, (unsigned long) r->pc,
      (unsigned long) r->psr, (unsigned long) r->cfsr, (unsigned long) r->hfsr,
      (unsigned long) r->dfsr, (unsigned long) r->afsr,
      (unsigned long) r->mmfar, (unsigned long) r->bfar,
      (unsigned long) r->abfsr, (unsigned long) r->sp);
  for (i = 0; ok && i < r->stack_word_count; i++) {
    ok = json_append(s_report, sizeof(s_report), &n, "%s\"0x%08lx\"",
                     i == 0 ? "" : ",", (unsigned long) r->stack_words[i]);
  }
  if (ok) ok = json_append(s_report, sizeof(s_report), &n, "]}}");
  return ok ? mg_str_n(s_report, n)
            : mg_str("{\"valid\":false,\"error\":\"JSON overflow\"}");
}

static void __attribute__((used)) mg_health_cortex_handler(
    uint32_t *exception_stack, uint32_t lr,
    const uint32_t *callee_saved) {
  static const char magic[] = MG_HEALTH_MAGIC;
  uint32_t cpuid = MG_SCB_CPUID;
  uint32_t part = (cpuid >> 4) & 0xfffU;
  size_t fp_words = (lr & (1U << 4)) == 0 ? 18U : 0U;
  int valid = stack_valid(exception_stack, fp_words + 8U);
  struct mg_health_cortex_record *r = &s_record;
  volatile uint32_t *words = (volatile uint32_t *) r;
  size_t i;

  for (i = 0; i < sizeof(*r) / sizeof(*words); i++) words[i] = 0;
  r->version = MG_HEALTH_CORTEX_VERSION;
  r->cpuid = cpuid;
  r->exception_sp = (uint32_t) (uintptr_t) exception_stack;
  r->exc_return = lr;
  r->exception_valid = valid;
  r->r4 = callee_saved[0];
  r->r5 = callee_saved[1];
  r->r6 = callee_saved[2];
  r->r7 = callee_saved[3];
  r->r8 = callee_saved[4];
  r->r9 = callee_saved[5];
  r->r10 = callee_saved[6];
  r->r11 = callee_saved[7];
  if (valid > 0) {
    uint32_t *core_frame = exception_stack + fp_words;
    uint32_t *fault_sp;
    uintptr_t lo, hi, p;
    size_t count;
    r->r0 = core_frame[0];
    r->r1 = core_frame[1];
    r->r2 = core_frame[2];
    r->r3 = core_frame[3];
    r->r12 = core_frame[4];
    r->lr = core_frame[5];
    r->pc = core_frame[6];
    r->psr = core_frame[7];
    fault_sp = core_frame + 8;
    if (r->psr & (1U << 9)) fault_sp++;
    r->sp = (uint32_t) (uintptr_t) fault_sp;
    r->stack_valid = stack_valid(fault_sp, 0);
    p = (uintptr_t) fault_sp;
    if (r->stack_valid > 0 && stack_range(fault_sp, &lo, &hi)) {
      (void) lo;
      count = (hi - p) / sizeof(uint32_t);
      if (count > MG_HEALTH_CORTEX_STACK_WORDS)
        count = MG_HEALTH_CORTEX_STACK_WORDS;
      r->stack_word_count = (uint32_t) count;
      for (i = 0; i < count; i++) r->stack_words[i] = fault_sp[i];
    }
  }
  if (has_fault_status(part)) {
    r->cfsr = MG_SCB_CFSR;
    r->hfsr = MG_SCB_HFSR;
    r->dfsr = MG_SCB_DFSR;
    r->afsr = MG_SCB_AFSR;
    r->bfar = MG_SCB_BFAR;
    r->mmfar = MG_SCB_MMFAR;
    if (part == MG_CPU_CORTEX_M7) r->abfsr = MG_SCB_ABFSR;
  }
  memcpy(r->magic, magic, sizeof(r->magic));
  (void) record_write(&mg_health_cortex, r);
  for (;;) (void) 0;
}

void HardFault_Handler(void);
__attribute__((naked)) void HardFault_Handler(void) {
  __asm volatile(
      "mov r0, lr \n"
      "movs r1, #4 \n"
      "tst r0, r1 \n"
      "beq 1f \n"
      "mrs r0, psp \n"
      "b 2f \n"
      "1: \n"
      "mrs r0, msp \n"
      "2: \n"
      "mov r1, lr \n"
      "stmdb sp!, {r4-r11} \n"
      "mov r2, sp \n"
      "b mg_health_cortex_handler \n");
}

#endif
