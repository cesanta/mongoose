#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_STM32F

static bool mg_stm32f_write(void *, const void *, size_t);
static bool mg_stm32f_swap(void);

static struct mg_flash s_mg_flash_stm32f = {
    (void *) 0x08000000,  // Start
    0,                    // Size, FLASH_SIZE_REG
    0,                    // Irregular sector size
    32,                   // Align, 256 bit
    mg_stm32f_write,
    mg_stm32f_swap,
};

#define MG_FLASH_BASE 0x40023c00
#define MG_FLASH_KEYR 0x04
#define MG_FLASH_SR 0x0c
#define MG_FLASH_CR 0x10
#define MG_FLASH_OPTCR 0x14
#define MG_FLASH_SIZE_REG_F7 0x1FF0F442
#define MG_FLASH_SIZE_REG_F4 0x1FFF7A22

#define STM_DBGMCU_IDCODE 0xE0042000
#define STM_DEV_ID (MG_REG(STM_DBGMCU_IDCODE) & (MG_BIT(12) - 1))
#define SYSCFG_MEMRMP 0x40013800

#define MG_FLASH_SIZE_REG_LOCATION \
  ((STM_DEV_ID >= 0x449) ? MG_FLASH_SIZE_REG_F7 : MG_FLASH_SIZE_REG_F4)

static size_t flash_size(void) {
  return (MG_REG(MG_FLASH_SIZE_REG_LOCATION) & 0xFFFF) * 1024;
}

MG_IRAM static int is_dualbank(void) {
  // only F42x/F43x series (0x419) support dual bank
  return STM_DEV_ID == 0x419;
}

MG_IRAM static void flash_unlock(void) {
  static bool unlocked = false;
  if (unlocked == false) {
    MG_REG(MG_FLASH_BASE + MG_FLASH_KEYR) = 0x45670123;
    MG_REG(MG_FLASH_BASE + MG_FLASH_KEYR) = 0xcdef89ab;
    unlocked = true;
  }
}

#define MG_FLASH_CONFIG_16_64_128 1   // used by STM32F7
#define MG_FLASH_CONFIG_32_128_256 2  // used by STM32F4 and F2

MG_IRAM static bool flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) s_mg_flash_stm32f.start;
  char *end = base + s_mg_flash_stm32f.size;

  if (is_dualbank() && dst >= (uint32_t *) (base + (end - base) / 2)) {
    dst = (uint32_t *) ((uint32_t) dst - (end - base) / 2);
  }

  uint32_t flash_config = MG_FLASH_CONFIG_16_64_128;
  if (STM_DEV_ID >= 0x449) {
    flash_config = MG_FLASH_CONFIG_32_128_256;
  }

  volatile char *p = (char *) dst;
  if (p >= base && p < end) {
    if (p < base + 16 * 1024 * 4 * flash_config) {
      if ((p - base) % (16 * 1024 * flash_config) == 0) return true;
    } else if (p == base + 16 * 1024 * 4 * flash_config) {
      return true;
    } else if ((p - base) % (128 * 1024 * flash_config) == 0) {
      return true;
    }
  }
  return false;
}

MG_IRAM static int flash_sector(volatile uint32_t *addr) {
  char *base = (char *) s_mg_flash_stm32f.start;
  char *end = base + s_mg_flash_stm32f.size;
  bool addr_in_bank_2 = false;
  if (is_dualbank() && addr >= (uint32_t *) (base + (end - base) / 2)) {
    addr = (uint32_t *) ((uint32_t) addr - (end - base) / 2);
    addr_in_bank_2 = true;
  }
  volatile char *p = (char *) addr;
  uint32_t flash_config = MG_FLASH_CONFIG_16_64_128;
  if (STM_DEV_ID >= 0x449) {
    flash_config = MG_FLASH_CONFIG_32_128_256;
  }
  int sector = -1;
  if (p >= base && p < end) {
    if (p < base + 16 * 1024 * 4 * flash_config) {
      sector = (p - base) / (16 * 1024 * flash_config);
    } else if (p >= base + 64 * 1024 * flash_config &&
               p < base + 128 * 1024 * flash_config) {
      sector = 4;
    } else {
      sector = (p - base) / (128 * 1024 * flash_config) + 4;
    }
  }
  if (sector == -1) return -1;
  if (addr_in_bank_2) sector += 12;  // a bank has 12 sectors
  return sector;
}

MG_IRAM static bool flash_is_err(void) {
  return MG_REG(MG_FLASH_BASE + MG_FLASH_SR) & ((MG_BIT(7) - 1) << 1);
}

MG_IRAM static void flash_wait(void) {
  while (MG_REG(MG_FLASH_BASE + MG_FLASH_SR) & (MG_BIT(16))) (void) 0;
}

MG_IRAM static void flash_clear_err(void) {
  flash_wait();                                // Wait until ready
  MG_REG(MG_FLASH_BASE + MG_FLASH_SR) = 0xf2;  // Clear all errors
}

MG_IRAM static bool mg_stm32f_erase(void *addr) {
  bool ok = false;
  if (flash_page_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
  } else {
    int sector = flash_sector(addr);
    if (sector < 0) return false;
    uint32_t sector_reg = sector;
    if (is_dualbank() && sector >= 12) {
      // 3.9.8 Flash control register (FLASH_CR) for F42xxx and F43xxx
      // BITS[7:3]
      sector_reg -= 12;
      sector_reg |= MG_BIT(4);
    }
    flash_unlock();
    flash_wait();
    uint32_t cr = MG_BIT(1);       // SER
    cr |= MG_BIT(16);              // STRT
    cr |= (sector_reg & 31) << 3;  // sector
    MG_REG(MG_FLASH_BASE + MG_FLASH_CR) = cr;
    ok = !flash_is_err();
    MG_DEBUG(("Erase sector %lu @ %p %s. CR %#lx SR %#lx", sector, addr,
              ok ? "ok" : "fail", MG_REG(MG_FLASH_BASE + MG_FLASH_CR),
              MG_REG(MG_FLASH_BASE + MG_FLASH_SR)));
    // After we have erased the sector, set CR flags for programming
    // 2 << 8 is word write parallelism, bit(0) is PG. RM0385, section 3.7.5
    MG_REG(MG_FLASH_BASE + MG_FLASH_CR) = MG_BIT(0) | (2 << 8);
    flash_clear_err();
  }
  return ok;
}

MG_IRAM static bool mg_stm32f_swap(void) {
  // STM32 F42x/F43x support dual bank, however, the memory mapping
  // change will not be carried through a hard reset. Therefore, we will use
  // the single bank approach for this family as well.
  return true;
}

static bool s_flash_irq_disabled;

MG_IRAM static bool mg_stm32f_write(void *addr, const void *buf, size_t len) {
  if ((len % s_mg_flash_stm32f.align) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, s_mg_flash_stm32f.align));
    return false;
  }
  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool ok = true;
  MG_ARM_DISABLE_IRQ();
  flash_unlock();
  flash_clear_err();
  MG_REG(MG_FLASH_BASE + MG_FLASH_CR) = MG_BIT(0) | MG_BIT(9);  // PG, 32-bit
  flash_wait();
  MG_DEBUG(("Writing flash @ %p, %lu bytes", addr, len));
  while (ok && src < end) {
    if (flash_page_start(dst) && mg_stm32f_erase(dst) == false) break;
    *(volatile uint32_t *) dst++ = *src++;
    MG_DSB();  // ensure flash is written with no errors
    flash_wait();
    if (flash_is_err()) ok = false;
  }
  if (!s_flash_irq_disabled) MG_ARM_ENABLE_IRQ();
  MG_DEBUG(("Flash write %lu bytes @ %p: %s. CR %#lx SR %#lx", len, dst,
            ok ? "ok" : "fail", MG_REG(MG_FLASH_BASE + MG_FLASH_CR),
            MG_REG(MG_FLASH_BASE + MG_FLASH_SR)));
  MG_REG(MG_FLASH_BASE + MG_FLASH_CR) &= ~MG_BIT(0);  // Clear programming flag
  return ok;
}

// just overwrite instead of swap
MG_IRAM void single_bank_swap(char *p1, char *p2, size_t size) {
  // no stdlib calls here
  mg_stm32f_write(p1, p2, size);
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}

bool mg_ota_begin(size_t new_firmware_size) {
  s_mg_flash_stm32f.size = flash_size();
#ifdef __ZEPHYR__
  *((uint32_t *)0xE000ED94) = 0;
  MG_DEBUG(("Jailbreak %s", *((uint32_t *)0xE000ED94) == 0 ? "successful" : "failed"));
#endif
  return mg_ota_flash_begin(new_firmware_size, &s_mg_flash_stm32f);
}

bool mg_ota_write(const void *buf, size_t len) {
  return mg_ota_flash_write(buf, len, &s_mg_flash_stm32f);
}

bool mg_ota_end(void) {
  if (mg_ota_flash_end(&s_mg_flash_stm32f)) {
    // Swap partitions. Pray power does not go away
    MG_INFO(("Swapping partitions, size %u (%u sectors)",
             s_mg_flash_stm32f.size, STM_DEV_ID == 0x449 ? 8 : 12));
    MG_INFO(("Do NOT power off..."));
    mg_log_level = MG_LL_NONE;
    s_flash_irq_disabled = true;
    char *p1 = (char *) s_mg_flash_stm32f.start;
    char *p2 = p1 + s_mg_flash_stm32f.size / 2;
    size_t size = s_mg_flash_stm32f.size / 2;
    // Runs in RAM, will reset when finished
    single_bank_swap(p1, p2, size);
  }
  return false;
}
#endif
