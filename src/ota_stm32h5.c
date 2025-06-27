#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_STM32H5

static bool mg_stm32h5_write(void *, const void *, size_t);
static bool mg_stm32h5_swap(void);

static struct mg_flash s_mg_flash_stm32h5 = {
    (void *) 0x08000000,  // Start
    2 * 1024 * 1024,      // Size, 2Mb
    8 * 1024,             // Sector size, 8k
    16,                   // Align, 128 bit
    mg_stm32h5_write,
    mg_stm32h5_swap,
};

#define MG_FLASH_BASE 0x40022000          // Base address of the flash controller
#define FLASH_KEYR (MG_FLASH_BASE + 0x4)  // See RM0481 7.11
#define FLASH_OPTKEYR (MG_FLASH_BASE + 0xc)
#define FLASH_OPTCR (MG_FLASH_BASE + 0x1c)
#define FLASH_NSSR (MG_FLASH_BASE + 0x20)
#define FLASH_NSCR (MG_FLASH_BASE + 0x28)
#define FLASH_NSCCR (MG_FLASH_BASE + 0x30)
#define FLASH_OPTSR_CUR (MG_FLASH_BASE + 0x50)
#define FLASH_OPTSR_PRG (MG_FLASH_BASE + 0x54)

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
  char *base = (char *) s_mg_flash_stm32h5.start,
       *end = base + s_mg_flash_stm32h5.size;
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % s_mg_flash_stm32h5.secsz) == 0;
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

static bool mg_stm32h5_erase(void *location) {
  bool ok = false;
  if (flash_page_start(location) == false) {
    MG_ERROR(("%p is not on a sector boundary"));
  } else {
    uintptr_t diff = (char *) location - (char *) s_mg_flash_stm32h5.start;
    uint32_t sector = diff / s_mg_flash_stm32h5.secsz;
    uint32_t saved_cr = MG_REG(FLASH_NSCR);  // Save CR value
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
    MG_REG(FLASH_NSCR) = saved_cr;  // Restore saved CR
  }
  return ok;
}

static bool mg_stm32h5_swap(void) {
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

static bool mg_stm32h5_write(void *addr, const void *buf, size_t len) {
  if ((len % s_mg_flash_stm32h5.align) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, s_mg_flash_stm32h5.align));
    return false;
  }
  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool ok = true;
  MG_ARM_DISABLE_IRQ();
  flash_unlock();
  flash_clear_err();
  MG_REG(FLASH_NSCR) = MG_BIT(1);  // Set programming flag
  while (ok && src < end) {
    if (flash_page_start(dst) && mg_stm32h5_erase(dst) == false) {
      ok = false;
      break;
    }
    *(volatile uint32_t *) dst++ = *src++;
    flash_wait();
    if (flash_is_err()) ok = false;
  }
  MG_ARM_ENABLE_IRQ();
  MG_DEBUG(("Flash write %lu bytes @ %p: %s. CR %#lx SR %#lx", len, dst,
            flash_is_err() ? "fail" : "ok", MG_REG(FLASH_NSCR),
            MG_REG(FLASH_NSSR)));
  MG_REG(FLASH_NSCR) = 0;  // Clear flags
  return ok;
}

bool mg_ota_begin(size_t new_firmware_size) {
#ifdef __ZEPHYR__
  *((uint32_t *)0xE000ED94) = 0;
  MG_DEBUG(("Jailbreak %s", *((uint32_t *)0xE000ED94) == 0 ? "successful" : "failed"));
#endif
  return mg_ota_flash_begin(new_firmware_size, &s_mg_flash_stm32h5);
}

bool mg_ota_write(const void *buf, size_t len) {
  return mg_ota_flash_write(buf, len, &s_mg_flash_stm32h5);
}

// Actual bank swap is deferred until reset, it is safe to execute in flash
bool mg_ota_end(void) {
  if(!mg_ota_flash_end(&s_mg_flash_stm32h5)) return false;
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
  return true;
}
#endif
