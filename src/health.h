// Device health monitoring

#pragma once

#include "arch.h"
#include "printf.h"

#ifndef MG_HEALTH_MAGIC
#define MG_HEALTH_MAGIC {'M', 'G', 'H', '3'}  // '3' is the layout version
#endif

#if !defined(MG_HEALTH_RAM) && (defined(STM32H7_DEV_ID) || defined(STM32H5))
#define MG_HEALTH_RAM __attribute__((section(".mg_health")))
#endif

// Places the record in RAM that survives a reset. Define in mongoose_config.h
// to match the linker script, e.g. __attribute__((section(".mg_health")))
#ifndef MG_HEALTH_RAM
#define MG_HEALTH_RAM
#endif

// Why the device restarted. Hardware usually reports several flags at once,
// so these are ordered by how much they tell you: the most specific cause a
// device reports wins
enum mg_health_reason {
  MG_HEALTH_RESET_UNKNOWN = 0,  // Nothing reported, or no masks configured
  MG_HEALTH_RESET_POWER,        // Power-on reset, RAM contents are lost
  MG_HEALTH_RESET_BROWNOUT,     // Supply dipped below the brown-out level
  MG_HEALTH_RESET_PIN,          // External reset pin, e.g. the reset button
  MG_HEALTH_RESET_SOFTWARE,     // Firmware asked for it, e.g. NVIC_SystemReset
  MG_HEALTH_RESET_LOWPOWER,     // Illegal or failed low-power transition
  MG_HEALTH_RESET_WATCHDOG,     // Watchdog fired: the application stopped
  MG_HEALTH_RESET_FAULT         // Fault handler ran, see regs[] and saved_stack
};

#ifndef MG_HEALTH_DATA_SIZE
#define MG_HEALTH_DATA_SIZE 250
#endif

struct mg_health {
  char magic[4];          // MG_HEALTH_MAGIC when the record holds valid data
  uint32_t reset_reason;  // enum mg_health_reason that started this boot
  uint32_t cpuid;         // ARM CPU ID
  uint32_t data[MG_HEALTH_DATA_SIZE];
};

extern struct mg_health mg_health_record;  // Defined in health.c

// Return true if the record holds valid data, i.e. it survived the reset and
// was written by this firmware layout
static inline bool mg_health_valid(void) {
  static const char magic[] = MG_HEALTH_MAGIC;
  return memcmp(mg_health_record.magic, magic,
                sizeof(mg_health_record.magic)) == 0;
}

static inline int mg_health_reason(void) {
  return mg_health_valid() ? (int) mg_health_record.reset_reason
                           : MG_HEALTH_RESET_UNKNOWN;
}

static inline const char *mg_health_reason_str(int reason) {
  switch (reason) {
    case MG_HEALTH_RESET_POWER: return "power";
    case MG_HEALTH_RESET_BROWNOUT: return "brownout";
    case MG_HEALTH_RESET_PIN: return "pin";
    case MG_HEALTH_RESET_SOFTWARE: return "software";
    case MG_HEALTH_RESET_LOWPOWER: return "lowpower";
    case MG_HEALTH_RESET_WATCHDOG: return "watchdog";
    case MG_HEALTH_RESET_FAULT: return "fault";
    default: return "unknown";
  }
}

// Start a new boot: validate the record, count the boot, store the reset
// reason. The current boot moves to prev_* slots first, so the previous
// boot's uptime and reason stay readable. Called by MG_HEALTH_INIT()
static inline void mg_health_init(void) {
  char magic[] = MG_HEALTH_MAGIC;
  // struct mg_health *h = mg_health_get();
  if (!mg_health_valid()) {  // First boot, or RAM lost its contents
    memset(&mg_health_record, 0, sizeof(mg_health_record));
    memcpy(mg_health_record.magic, magic, sizeof(mg_health_record.magic));
  }
}

#define MG_HEALTH_INIT() mg_health_init()
extern size_t mg_print_crash_record(mg_pfn_t, void *, va_list *);
