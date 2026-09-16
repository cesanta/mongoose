// Device health monitoring
//
// A health provider owns the report format and returns it as an opaque blob.
// Applications and transports use mg_health_get_blob() without knowing how the
// report was collected or encoded.

#pragma once

#include "str.h"

#define MG_HEALTH_NONE 0
#define MG_HEALTH_CORTEX 1

#ifndef MG_HEALTH
#define MG_HEALTH MG_HEALTH_NONE
#endif

#ifndef MG_HEALTH_MAGIC
#define MG_HEALTH_MAGIC {'M', 'G', 'H', '3'}  // '3' is the layout version
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

typedef struct mg_str mg_health_blob;

struct mg_health {
  char magic[4];
  mg_health_blob (*get_blob)(void *);
  void *fn_data;
  uint32_t reset_reason;
};

extern struct mg_health mg_health_record;

// Return true if the record holds valid data, i.e. it survived the reset and
// was written by this firmware layout
static inline bool mg_health_valid(void) {
  static const char magic[] = MG_HEALTH_MAGIC;
  return memcmp(mg_health_record.magic, magic,
                sizeof(mg_health_record.magic)) == 0;
}

// Select and initialise the configured health provider. Call after
// initializing the board-specific persistent-storage
void mg_health_init(void);

// Return the provider's opaque report. The returned memory is provider-owned
// and remains valid until the next call to this function
struct mg_str mg_health_get_blob(void);

static inline int mg_health_reason(void) {
  return mg_health_valid() ? (int) mg_health_record.reset_reason
                           : MG_HEALTH_RESET_UNKNOWN;
}

static inline const char *mg_health_reason_str(int reason) {
  switch (reason) {
    case MG_HEALTH_RESET_POWER:
      return "power";
    case MG_HEALTH_RESET_BROWNOUT:
      return "brownout";
    case MG_HEALTH_RESET_PIN:
      return "pin";
    case MG_HEALTH_RESET_SOFTWARE:
      return "software";
    case MG_HEALTH_RESET_LOWPOWER:
      return "lowpower";
    case MG_HEALTH_RESET_WATCHDOG:
      return "watchdog";
    case MG_HEALTH_RESET_FAULT:
      return "fault";
    default:
      return "unknown";
  }
}

#define MG_HEALTH_INIT() mg_health_init()
