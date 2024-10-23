#include "device.h"
#include "log.h"

#if MG_DEVICE == MG_DEVICE_STM32F7
#define FLASH_BASE1 0x40023c00  // Base address for bank1
#define FLASH_KEYR 0x04
#define FLASH_SR 0x0c
#define FLASH_CR 0x10
#define FLASH_OPTCR 0x14
#define FLASH_SIZE_REG 0x1FF0F442

MG_IRAM void *mg_flash_start(void) {
  return (void *) 0x08000000;
}
MG_IRAM size_t mg_flash_size(void) {
  return (MG_REG(FLASH_SIZE_REG) & 0xFFFF) * 1024;
}
MG_IRAM size_t mg_flash_sector_size(void) {
  return 0;  // sector size is not constant
}
MG_IRAM size_t mg_flash_write_align(void) {
  return 32;  // 256 bit
}
MG_IRAM int mg_flash_bank(void) {
  if (mg_flash_size() < 2 * 1024 * 1024) return 0;  // No dual bank support
  return MG_REG(FLASH_BASE1 + FLASH_OPTCR) & MG_BIT(31) ? 2 : 1;
}

MG_IRAM static void flash_unlock(void) {
  static bool unlocked = false;
  if (unlocked == false) {
    MG_REG(FLASH_BASE1 + FLASH_KEYR) = 0x45670123;
    MG_REG(FLASH_BASE1 + FLASH_KEYR) = 0xcdef89ab;
    unlocked = true;
  }
}

MG_IRAM static bool flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) mg_flash_start(), *end = base + mg_flash_size();
  volatile char *p = (char *) dst;
  if (p >= base && p < end) {
    if (p < base + 32 * 1024 * 4) {
      if ((p - base) % (32 * 1024) == 0) return true;
    } else if (p == base + 32 * 1024 * 4) {
      return true;
    } else if ((p - base) % (256 * 1024) == 0) return true;
  }
  return false;
}

MG_IRAM static int flash_sector(volatile uint32_t *addr) {
  char *base = (char *) mg_flash_start(), *end = base + mg_flash_size();
  volatile char *p = (char *) addr;
  if (p >= base && p < end) {
    if (p < base + 32 * 1024 * 4) {
      return (p - base) / (32 * 1024);
    } else if (p >= base + 128 * 1024 && p < base + 256 * 1024) {
      return 4;
    } else {
      return (p - base) / (256 * 1024) + 4;
    }
  }
  return -1;
}

MG_IRAM static bool flash_is_err(uint32_t bank) {
  (void) bank;
  return MG_REG(FLASH_BASE1 + FLASH_SR) & ((MG_BIT(7) - 1) << 1);
}

MG_IRAM static void flash_wait(uint32_t bank) {
  (void) bank;
  while (MG_REG(FLASH_BASE1 + FLASH_SR) & (MG_BIT(16))) (void) 0;
}

MG_IRAM static void flash_clear_err(uint32_t bank) {
  flash_wait(bank);                                      // Wait until ready
  MG_REG(FLASH_BASE1 + FLASH_SR) = (MG_BIT(7) - 1) << 1;  // Clear all errors
}

MG_IRAM static bool flash_bank_is_swapped(uint32_t bank) {
  return 0;
}

// Figure out flash bank based on the address
MG_IRAM static uint32_t flash_bank(void *addr) {
  return 0;
}

MG_IRAM bool mg_flash_erase(void *addr) {
  bool ok = false;
  if (flash_page_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
  } else {
    int sector = flash_sector(addr);
    if (sector < 0) return false;
    flash_unlock();
    flash_wait(0);
    uint32_t cr = MG_BIT(1); // SER
    cr |= MG_BIT(16); // STRT
    cr |= (sector & 15) << 3; // sector
    MG_REG(FLASH_BASE1 + FLASH_CR) = cr;
    ok = !flash_is_err(0);
    MG_DEBUG(("Erase sector %lu @ %p %s. CR %#lx SR %#lx", sector, addr,
              ok ? "ok" : "fail", MG_REG(FLASH_BASE1 + FLASH_CR),
              MG_REG(FLASH_BASE1 + FLASH_SR)));
    // After we have erased the sector, set CR flags for programming
    // 2 << 8 is word write parallelism, bit(0) is PG. RM0385, section 3.7.5
    MG_REG(FLASH_BASE1 + FLASH_CR) = MG_BIT(0) | (2 << 8);
  }
  return ok;
}

MG_IRAM bool mg_flash_swap_bank(void) {
  return true;
}

MG_IRAM bool mg_flash_write(void *addr, const void *buf, size_t len) {
  if ((len % mg_flash_write_align()) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, mg_flash_write_align()));
    return false;
  }
  uint32_t bank = flash_bank(addr);
  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool ok = true;
  flash_unlock();
  flash_clear_err(bank);
  MG_REG(FLASH_BASE1 + FLASH_CR) = MG_BIT(0);   // Set programming flag
  MG_REG(FLASH_BASE1 + FLASH_CR) |= MG_BIT(9);  // 32-bit write parallelism
  MG_DEBUG(("Writing flash @ %p, %lu bytes", addr, len));
  MG_ARM_DISABLE_IRQ();
  while (ok && src < end) {
    if (flash_page_start(dst) && mg_flash_erase(dst) == false) break;
    *(volatile uint32_t *) dst++ = *src++;
    flash_wait(bank);
    if (flash_is_err(bank)) ok = false;
  }
  MG_ARM_ENABLE_IRQ();
  MG_DEBUG(("Flash write %lu bytes @ %p: %s. CR %#lx SR %#lx", len, dst,
            ok ? "ok" : "fail", MG_REG(FLASH_BASE1 + FLASH_CR),
            MG_REG(FLASH_BASE1 + FLASH_SR)));
  MG_REG(FLASH_BASE1 + FLASH_CR) &= ~MG_BIT(0);  // Clear programming flag
  return ok;
}

MG_IRAM void mg_device_reset(void) {
  // SCB->AIRCR = ((0x5fa << SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk);
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}
#endif
