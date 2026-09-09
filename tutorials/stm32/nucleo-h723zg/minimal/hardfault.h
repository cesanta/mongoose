#pragma once

#include <stdint.h>
#include <stdio.h>

#define CRASH_RECORD_MAGIC 0x43524153U
#define CRASH_RECORD_VERSION 1U
#define CRASH_STORAGE_SIZE 4096U
#define CRASH_STACK_WORDS 980U

struct crash_record {
  uint32_t magic;
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
  uint32_t stack_words[CRASH_STACK_WORDS];
};

_Static_assert(sizeof(struct crash_record) <= CRASH_STORAGE_SIZE,
               "crash record does not fit in backup SRAM");

extern struct crash_record g_hardfault_record;
void hal_crash_retrieve(struct crash_record *r);
void hal_crash_store(struct crash_record *r);
void hal_storage_init(void);
