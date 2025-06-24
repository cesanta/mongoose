#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_PICOSDK

// Both RP2040 and RP2350 have no flash, low-level flash access support in
// bootrom, and high-level support in Pico-SDK (2.0+ for the RP2350)
// - The RP2350 in RISC-V mode is not tested
// NOTE(): See OTA design notes

static bool mg_picosdk_write(void *, const void *, size_t);
static bool mg_picosdk_swap(void);

static struct mg_flash s_mg_flash_picosdk = {
    (void *) 0x10000000,  // Start; functions handle offset
#ifdef PICO_FLASH_SIZE_BYTES
    PICO_FLASH_SIZE_BYTES,  // Size, from board definitions
#else
    0x200000,  // Size, guess... is 2M enough ?
#endif
    FLASH_SECTOR_SIZE,  // Sector size, from hardware_flash
    FLASH_PAGE_SIZE,    // Align, from hardware_flash
    mg_picosdk_write,      mg_picosdk_swap,
};

#define MG_MODULO2(x, m) ((x) & ((m) -1))

static bool __no_inline_not_in_flash_func(flash_sector_start)(
    volatile uint32_t *dst) {
  char *base = (char *) s_mg_flash_picosdk.start,
       *end = base + s_mg_flash_picosdk.size;
  volatile char *p = (char *) dst;
  return p >= base && p < end &&
         MG_MODULO2(p - base, s_mg_flash_picosdk.secsz) == 0;
}

static bool __no_inline_not_in_flash_func(flash_erase)(void *addr) {
  if (flash_sector_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
    return false;
  }
  void *dst = (void *) ((char *) addr - (char *) s_mg_flash_picosdk.start);
  flash_range_erase((uint32_t) dst, s_mg_flash_picosdk.secsz);
  MG_DEBUG(("Sector starting at %p erasure", addr));
  return true;
}

static bool __no_inline_not_in_flash_func(mg_picosdk_swap)(void) {
  // TODO(): RP2350 might have some A/B functionality (DS 5.1)
  return true;
}

static bool s_flash_irq_disabled;

static bool __no_inline_not_in_flash_func(mg_picosdk_write)(void *addr,
                                                            const void *buf,
                                                            size_t len) {
  if ((len % s_mg_flash_picosdk.align) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, s_mg_flash_picosdk.align));
    return false;
  }
  if ((((size_t) addr - (size_t) s_mg_flash_picosdk.start) %
       s_mg_flash_picosdk.align) != 0) {
    MG_ERROR(("%p is not on a page boundary", addr));
    return false;
  }

  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);

#ifndef __riscv
  MG_ARM_DISABLE_IRQ();
#else
  asm volatile("csrrc zero, mstatus, %0" : : "i"(1 << 3) : "memory");
#endif
  while (src < end) {
    uint32_t dst_ofs = (uint32_t) dst - (uint32_t) s_mg_flash_picosdk.start;
    if (flash_sector_start(dst) && flash_erase(dst) == false) break;
    // flash_range_program() runs in RAM and handles writing up to
    // FLASH_PAGE_SIZE bytes. Source must not be in flash
    flash_range_program((uint32_t) dst_ofs, (uint8_t *) src,
                        s_mg_flash_picosdk.align);
    src = (uint32_t *) ((char *) src + s_mg_flash_picosdk.align);
    dst = (uint32_t *) ((char *) dst + s_mg_flash_picosdk.align);
  }
  if (!s_flash_irq_disabled) {
#ifndef __riscv
    MG_ARM_ENABLE_IRQ();
#else
    asm volatile("csrrs mstatus, %0" : : "i"(1 << 3) : "memory");
#endif
  }
  MG_DEBUG(("Flash write %lu bytes @ %p.", len, dst));
  return true;
}

// just overwrite instead of swap
static void __no_inline_not_in_flash_func(single_bank_swap)(char *p1, char *p2,
                                                            size_t s,
                                                            size_t ss) {
  char *tmp = mg_calloc(1, ss);
  if (tmp == NULL) return;
#if PICO_RP2040
  uint32_t xip[256 / sizeof(uint32_t)];
  void *dst = (void *) ((char *) p1 - (char *) s_mg_flash_picosdk.start);
  size_t count = MG_ROUND_UP(s, ss);
  // use SDK function calls to get BootROM function pointers
  rom_connect_internal_flash_fn connect = (rom_connect_internal_flash_fn) rom_func_lookup(ROM_FUNC_CONNECT_INTERNAL_FLASH);
  rom_flash_exit_xip_fn xit = (rom_flash_exit_xip_fn) rom_func_lookup(ROM_FUNC_FLASH_EXIT_XIP);
  rom_flash_range_program_fn program = (rom_flash_range_program_fn) rom_func_lookup(ROM_FUNC_FLASH_RANGE_PROGRAM);
  rom_flash_flush_cache_fn flush = (rom_flash_flush_cache_fn) rom_func_lookup(ROM_FUNC_FLASH_FLUSH_CACHE);
  // no stdlib calls here.
  MG_ARM_DISABLE_IRQ();
  // 2nd bootloader (XIP) is in flash, SDK functions copy it to RAM on entry
  for (size_t i = 0; i < 256 / sizeof(uint32_t); i++)
    xip[i] = ((uint32_t *) (s_mg_flash_picosdk.start))[i];
  flash_range_erase((uint32_t) dst, count);
  // flash has been erased, no XIP to copy. Only BootROM calls possible
  for (uint32_t ofs = 0; ofs < s; ofs += ss) {
    for (size_t i = 0; i < ss; i++) tmp[i] = p2[ofs + i];
    __compiler_memory_barrier();
    connect();
    xit();
    program((uint32_t) dst + ofs, tmp, ss);
    flush();
    ((void (*)(void))((intptr_t) xip + 1))(); // enter XIP again
  }
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;  // AIRCR = SYSRESETREQ
#else
  // RP2350 has BootRAM and copies second bootloader there, SDK uses that copy,
  // It might also be able to take advantage of partition swapping
  rom_reboot_fn reboot = (rom_reboot_fn) rom_func_lookup(ROM_FUNC_REBOOT);
  for (size_t ofs = 0; ofs < s; ofs += ss) {
    for (size_t i = 0; i < ss; i++) tmp[i] = p2[ofs + i];
    mg_picosdk_write(p1 + ofs, tmp, ss);
  }
  reboot(BOOT_TYPE_NORMAL | 0x100, 1, 0, 0); // 0x100: NO_RETURN_ON_SUCCESS
#endif
}

bool mg_ota_begin(size_t new_firmware_size) {
  return mg_ota_flash_begin(new_firmware_size, &s_mg_flash_picosdk);
}

bool mg_ota_write(const void *buf, size_t len) {
  return mg_ota_flash_write(buf, len, &s_mg_flash_picosdk);
}

bool mg_ota_end(void) {
  if (mg_ota_flash_end(&s_mg_flash_picosdk)) {
    // Swap partitions. Pray power does not go away
    MG_INFO(("Swapping partitions, size %u (%u sectors)",
             s_mg_flash_picosdk.size,
             s_mg_flash_picosdk.size / s_mg_flash_picosdk.secsz));
    MG_INFO(("Do NOT power off..."));
    mg_log_level = MG_LL_NONE;
    s_flash_irq_disabled = true;
    // Runs in RAM, will reset when finished or return on failure
    single_bank_swap(
        (char *) s_mg_flash_picosdk.start,
        (char *) s_mg_flash_picosdk.start + s_mg_flash_picosdk.size / 2,
        s_mg_flash_picosdk.size / 2, s_mg_flash_picosdk.secsz);
  }
  return false;
}
#endif
