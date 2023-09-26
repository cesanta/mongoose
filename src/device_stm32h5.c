#include "log.h"
#include "device.h"

#if MG_DEVICE == MG_DEVICE_STM32H5

#define FLASH_BASE 0x40022000          // Base address of the flash controller
#define FLASH_KEYR (FLASH_BASE + 0x4)  // See RM0481 7.11
#define FLASH_OPTKEYR (FLASH_BASE + 0xc)
#define FLASH_OPTCR (FLASH_BASE + 0x1c)
#define FLASH_NSSR (FLASH_BASE + 0x20)
#define FLASH_NSCR (FLASH_BASE + 0x28)
#define FLASH_NSCCR (FLASH_BASE + 0x30)
#define FLASH_OPTSR_CUR (FLASH_BASE + 0x50)
#define FLASH_OPTSR_PRG (FLASH_BASE + 0x54)

void *mg_flash_start(void) {
  return (void *) 0x08000000;
}
size_t mg_flash_size(void) {
  return 2 * 1024 * 1024;  // 2Mb
}
size_t mg_flash_sector_size(void) {
  return 8 * 1024;  // 8k
}
size_t mg_flash_write_align(void) {
  return 16;  // 128 bit
}
int mg_flash_bank(void) {
  return MG_REG(FLASH_OPTCR) & MG_BIT(31) ? 2 : 1;
}

static void flash_unlock(void) {
  static bool unlocked = false;
  if (unlocked == false) {
    MG_REG(FLASH_KEYR) = 0x45670123;
    MG_REG(FLASH_KEYR) = 0Xcdef89ab;
    MG_REG(FLASH_OPTKEYR) = 0x08192a3b;
    MG_REG(FLASH_OPTKEYR) = 0x4c5d6e7f;
    unlocked = true;
  }
}

static int flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) mg_flash_start(), *end = base + mg_flash_size();
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % mg_flash_sector_size()) == 0;
}

static bool flash_is_err(void) {
  return MG_REG(FLASH_NSSR) & ((MG_BIT(8) - 1) << 17);  // RM0481 7.11.9
}

static void flash_wait(void) {
  while ((MG_REG(FLASH_NSSR) & MG_BIT(0)) &&
         (MG_REG(FLASH_NSSR) & MG_BIT(16)) == 0) {
    (void) 0;
  }
}

static void flash_clear_err(void) {
  flash_wait();                                    // Wait until ready
  MG_REG(FLASH_NSCCR) = ((MG_BIT(9) - 1) << 16U);  // Clear all errors
}

static bool flash_bank_is_swapped(void) {
  return MG_REG(FLASH_OPTCR) & MG_BIT(31);  // RM0481 7.11.8
}

bool mg_flash_erase(void *location) {
  bool ok = false;
  if (flash_page_start(location) == false) {
    MG_ERROR(("%p is not on a sector boundary"));
  } else {
    uintptr_t diff = (char *) location - (char *) mg_flash_start();
    uint32_t sector = diff / mg_flash_sector_size();
    flash_unlock();
    flash_clear_err();
    MG_REG(FLASH_NSCR) = 0;
    if ((sector < 128 && flash_bank_is_swapped()) ||
        (sector > 127 && !flash_bank_is_swapped())) {
      MG_REG(FLASH_NSCR) |= MG_BIT(31);  // Set FLASH_CR_BKSEL
    }
    if (sector > 127) sector -= 128;
    MG_REG(FLASH_NSCR) |= MG_BIT(2) | (sector << 6);  // Erase | sector_num
    MG_REG(FLASH_NSCR) |= MG_BIT(5);                  // Start erasing
    flash_wait();
    ok = !flash_is_err();
    MG_DEBUG(("Erase sector %lu @ %p: %s. CR %#lx SR %#lx", sector, location,
              ok ? "ok" : "fail", MG_REG(FLASH_NSCR), MG_REG(FLASH_NSSR)));
    // mg_hexdump(location, 32);
  }
  return ok;
}

bool mg_flash_swap_bank(void) {
  uint32_t desired = flash_bank_is_swapped() ? 0 : MG_BIT(31);
  flash_unlock();
  flash_clear_err();
  // printf("OPTSR_PRG 1 %#lx\n", FLASH->OPTSR_PRG);
  MG_SET_BITS(MG_REG(FLASH_OPTSR_PRG), MG_BIT(31), desired);
  // printf("OPTSR_PRG 2 %#lx\n", FLASH->OPTSR_PRG);
  MG_REG(FLASH_OPTCR) |= MG_BIT(1);  // OPTSTART
  while ((MG_REG(FLASH_OPTSR_CUR) & MG_BIT(31)) != desired) (void) 0;
  return true;
}

bool mg_flash_write(void *addr, const void *buf, size_t len) {
  if ((len % mg_flash_write_align()) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, mg_flash_write_align()));
    return false;
  }
  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool ok = true;
  flash_unlock();
  flash_clear_err();
  MG_ARM_DISABLE_IRQ();
  // MG_DEBUG(("Starting flash write %lu bytes @ %p", len, addr));
  while (ok && src < end) {
    if (flash_page_start(dst) && mg_flash_erase(dst) == false) break;
    MG_REG(FLASH_NSCR) = MG_BIT(1);  // Set programming flag
    *(volatile uint32_t *) dst++ = *src++;
    flash_wait();
    if (flash_is_err()) ok = false;
  }
  MG_DEBUG(("Flash write %lu bytes @ %p: %s. CR %#lx SR %#lx", len, dst,
            flash_is_err() ? "fail" : "ok", MG_REG(FLASH_NSCR),
            MG_REG(FLASH_NSSR)));
  if (flash_is_err()) ok = false;
  // mg_hexdump(addr, len > 32 ? 32 : len);
  //  MG_REG(FLASH_NSCR) &= ~MG_BIT(1);  // Set programming flag
  MG_REG(FLASH_NSCR) = 0;  // Clear flags
  MG_ARM_ENABLE_IRQ();
  return ok;
}

void mg_device_reset(void) {
  // SCB->AIRCR = ((0x5fa << SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk);
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}
#endif
