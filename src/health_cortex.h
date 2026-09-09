#pragma once

#include "health.h"

#define MG_HEALTH_CORTEX_VERSION 1U

#ifndef MG_HEALTH_CORTEX_STORAGE
#define MG_HEALTH_CORTEX_STORAGE NULL
#endif

#ifndef MG_HEALTH_CORTEX_STORAGE_SIZE
#define MG_HEALTH_CORTEX_STORAGE_SIZE 0
#endif

#ifndef MG_HEALTH_CORTEX_STACK_WORDS
#define MG_HEALTH_CORTEX_STACK_WORDS 980U
#endif

#ifndef MG_HEALTH_CORTEX_REPORT_SIZE
#define MG_HEALTH_CORTEX_REPORT_SIZE 16384U
#endif

// Storage operations are deliberately format-agnostic. H723 uses the default
// direct-memory implementation; boards with different retained storage can
// replace these callbacks before calling MG_HEALTH_INIT()
struct mg_health_cortex {
  void *storage;
  size_t storage_size;
  bool (*init)(void *, size_t);
  bool (*read)(const void *, size_t, void *, size_t);
  bool (*write)(void *, size_t, const void *, size_t);
};

struct mg_health_cortex_record {
  char magic[4];
  uint32_t version;

  uint32_t cpuid;
  uint32_t exception_sp;
  uint32_t sp;
  uint32_t exc_return;
  int32_t exception_valid;
  int32_t stack_valid;

  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t r11;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t psr;

  uint32_t cfsr;
  uint32_t hfsr;
  uint32_t dfsr;
  uint32_t afsr;
  uint32_t bfar;
  uint32_t mmfar;
  uint32_t abfsr;

  uint32_t stack_word_count;
  uint32_t stack_words[MG_HEALTH_CORTEX_STACK_WORDS];
};

extern struct mg_health_cortex mg_health_cortex;

void mg_health_cortex_init(struct mg_health_cortex *);
struct mg_str mg_health_cortex_get_blob(void *);
