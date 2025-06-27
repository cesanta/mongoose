#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_STM32H7 || MG_OTA == MG_OTA_STM32H7_DUAL_CORE

// - H723/735 RM 4.3.3: Note: The application can simultaneously request a read
// and a write operation through the AXI interface.
//   - We only need IRAM for partition swapping in the H723, however, all
//   related functions must reside in IRAM for this to be possible.
// - Linker files for other devices won't define a .iram section so there's no
// associated penalty

static bool mg_stm32h7_write(void *, const void *, size_t);
static bool mg_stm32h7_swap(void);

static struct mg_flash s_mg_flash_stm32h7 = {
    (void *) 0x08000000,  // Start
    0,                    // Size, FLASH_SIZE_REG
    128 * 1024,           // Sector size, 128k
    32,                   // Align, 256 bit
    mg_stm32h7_write,
    mg_stm32h7_swap,
};

#define FLASH_BASE1 0x52002000  // Base address for bank1
#define FLASH_BASE2 0x52002100  // Base address for bank2
#define FLASH_KEYR 0x04         // See RM0433 4.9.2
#define FLASH_OPTKEYR 0x08
#define FLASH_OPTCR 0x18
#define FLASH_SR 0x10
#define FLASH_CR 0x0c
#define FLASH_CCR 0x14
#define FLASH_OPTSR_CUR 0x1c
#define FLASH_OPTSR_PRG 0x20
#define FLASH_SIZE_REG 0x1ff1e880

#define IS_DUALCORE() (MG_OTA == MG_OTA_STM32H7_DUAL_CORE)

MG_IRAM static bool is_dualbank(void) {
  if (IS_DUALCORE()) {
    // H745/H755 and H747/H757 are running on dual core.
    // Using only the 1st bank (mapped to CM7), in order not to interfere
    // with the 2nd bank (CM4), possibly causing CM4 to boot unexpectedly.
    return false;
  }
  return (s_mg_flash_stm32h7.size < 2 * 1024 * 1024) ? false : true;
}

MG_IRAM static void flash_unlock(void) {
  static bool unlocked = false;
  if (unlocked == false) {
    MG_REG(FLASH_BASE1 + FLASH_KEYR) = 0x45670123;
    MG_REG(FLASH_BASE1 + FLASH_KEYR) = 0xcdef89ab;
    if (is_dualbank()) {
      MG_REG(FLASH_BASE2 + FLASH_KEYR) = 0x45670123;
      MG_REG(FLASH_BASE2 + FLASH_KEYR) = 0xcdef89ab;
    }
    MG_REG(FLASH_BASE1 + FLASH_OPTKEYR) = 0x08192a3b;  // opt reg is "shared"
    MG_REG(FLASH_BASE1 + FLASH_OPTKEYR) = 0x4c5d6e7f;  // thus unlock once
    unlocked = true;
  }
}

MG_IRAM static bool flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) s_mg_flash_stm32h7.start,
       *end = base + s_mg_flash_stm32h7.size;
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % s_mg_flash_stm32h7.secsz) == 0;
}

MG_IRAM static bool flash_is_err(uint32_t bank) {
  return MG_REG(bank + FLASH_SR) & ((MG_BIT(11) - 1) << 17);  // RM0433 4.9.5
}

MG_IRAM static void flash_wait(uint32_t bank) {
  while (MG_REG(bank + FLASH_SR) & (MG_BIT(0) | MG_BIT(2))) (void) 0;
}

MG_IRAM static void flash_clear_err(uint32_t bank) {
  flash_wait(bank);                                      // Wait until ready
  MG_REG(bank + FLASH_CCR) = ((MG_BIT(11) - 1) << 16U);  // Clear all errors
}

MG_IRAM static bool flash_bank_is_swapped(uint32_t bank) {
  return MG_REG(bank + FLASH_OPTCR) & MG_BIT(31);  // RM0433 4.9.7
}

// Figure out flash bank based on the address
MG_IRAM static uint32_t flash_bank(void *addr) {
  size_t ofs = (char *) addr - (char *) s_mg_flash_stm32h7.start;
  if (!is_dualbank()) return FLASH_BASE1;
  return ofs < s_mg_flash_stm32h7.size / 2 ? FLASH_BASE1 : FLASH_BASE2;
}

// read-while-write, no need to disable IRQs for standalone usage
MG_IRAM static bool mg_stm32h7_erase(void *addr) {
  bool ok = false;
  if (flash_page_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
  } else {
    uintptr_t diff = (char *) addr - (char *) s_mg_flash_stm32h7.start;
    uint32_t sector = diff / s_mg_flash_stm32h7.secsz;
    uint32_t bank = flash_bank(addr);
    uint32_t saved_cr = MG_REG(bank + FLASH_CR);  // Save CR value

    flash_unlock();
    if (sector > 7) sector -= 8;

    flash_clear_err(bank);
    MG_REG(bank + FLASH_CR) = MG_BIT(5);             // 32-bit write parallelism
    MG_REG(bank + FLASH_CR) |= (sector & 7U) << 8U;  // Sector to erase
    MG_REG(bank + FLASH_CR) |= MG_BIT(2);            // Sector erase bit
    MG_REG(bank + FLASH_CR) |= MG_BIT(7);            // Start erasing
    ok = !flash_is_err(bank);
    MG_DEBUG(("Erase sector %lu @ %p %s. CR %#lx SR %#lx", sector, addr,
              ok ? "ok" : "fail", MG_REG(bank + FLASH_CR),
              MG_REG(bank + FLASH_SR)));
    MG_REG(bank + FLASH_CR) = saved_cr;  // Restore CR
  }
  return ok;
}

MG_IRAM static bool mg_stm32h7_swap(void) {
  if (!is_dualbank()) return true;
  uint32_t bank = FLASH_BASE1;
  uint32_t desired = flash_bank_is_swapped(bank) ? 0 : MG_BIT(31);
  flash_unlock();
  flash_clear_err(bank);
  // printf("OPTSR_PRG 1 %#lx\n", FLASH->OPTSR_PRG);
  MG_SET_BITS(MG_REG(bank + FLASH_OPTSR_PRG), MG_BIT(31), desired);
  // printf("OPTSR_PRG 2 %#lx\n", FLASH->OPTSR_PRG);
  MG_REG(bank + FLASH_OPTCR) |= MG_BIT(1);  // OPTSTART
  while ((MG_REG(bank + FLASH_OPTSR_CUR) & MG_BIT(31)) != desired) (void) 0;
  return true;
}

static bool s_flash_irq_disabled;

MG_IRAM static bool mg_stm32h7_write(void *addr, const void *buf, size_t len) {
  if ((len % s_mg_flash_stm32h7.align) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, s_mg_flash_stm32h7.align));
    return false;
  }
  uint32_t bank = flash_bank(addr);
  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool ok = true;
  MG_ARM_DISABLE_IRQ();
  flash_unlock();
  flash_clear_err(bank);
  MG_REG(bank + FLASH_CR) = MG_BIT(1);   // Set programming flag
  MG_REG(bank + FLASH_CR) |= MG_BIT(5);  // 32-bit write parallelism
  while (ok && src < end) {
    if (flash_page_start(dst) && mg_stm32h7_erase(dst) == false) {
      ok = false;
      break;
    }
    *(volatile uint32_t *) dst++ = *src++;
    flash_wait(bank);
    if (flash_is_err(bank)) ok = false;
  }
  if (!s_flash_irq_disabled) MG_ARM_ENABLE_IRQ();
  MG_DEBUG(("Flash write %lu bytes @ %p: %s. CR %#lx SR %#lx", len, dst,
            ok ? "ok" : "fail", MG_REG(bank + FLASH_CR),
            MG_REG(bank + FLASH_SR)));
  MG_REG(bank + FLASH_CR) &= ~MG_BIT(1);  // Clear programming flag
  return ok;
}

// just overwrite instead of swap
MG_IRAM static void single_bank_swap(char *p1, char *p2, size_t s, size_t ss) {
  // no stdlib calls here
  for (size_t ofs = 0; ofs < s; ofs += ss) {
    mg_stm32h7_write(p1 + ofs, p2 + ofs, ss);
  }
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}

bool mg_ota_begin(size_t new_firmware_size) {
  s_mg_flash_stm32h7.size = MG_REG(FLASH_SIZE_REG) * 1024;
  if (IS_DUALCORE()) {
    // Using only the 1st bank (mapped to CM7)
    s_mg_flash_stm32h7.size /= 2;
  }
#ifdef __ZEPHYR__
  *((uint32_t *)0xE000ED94) = 0;
  MG_DEBUG(("Jailbreak %s", *((uint32_t *)0xE000ED94) == 0 ? "successful" : "failed"));
#endif
  return mg_ota_flash_begin(new_firmware_size, &s_mg_flash_stm32h7);
}

bool mg_ota_write(const void *buf, size_t len) {
  return mg_ota_flash_write(buf, len, &s_mg_flash_stm32h7);
}

bool mg_ota_end(void) {
  if (mg_ota_flash_end(&s_mg_flash_stm32h7)) {
    if (is_dualbank()) {
      // Bank swap is deferred until reset, been executing in flash, reset
      *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
    } else {
      // Swap partitions. Pray power does not go away
      MG_INFO(("Swapping partitions, size %u (%u sectors)",
               s_mg_flash_stm32h7.size,
               s_mg_flash_stm32h7.size / s_mg_flash_stm32h7.secsz));
      MG_INFO(("Do NOT power off..."));
      mg_log_level = MG_LL_NONE;
      s_flash_irq_disabled = true;
      // Runs in RAM, will reset when finished
      single_bank_swap(
          (char *) s_mg_flash_stm32h7.start,
          (char *) s_mg_flash_stm32h7.start + s_mg_flash_stm32h7.size / 2,
          s_mg_flash_stm32h7.size / 2, s_mg_flash_stm32h7.secsz);
    }
  }
  return false;
}
#endif
