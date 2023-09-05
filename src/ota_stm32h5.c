#include "arch.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_STM32H5

#define FLASH_BANK1 0x08000000
#define FLASH_SIZE1 0x00100000
#define FLASH_BANK2 0x08100000
#define FLASH_SIZE2 0x00100000
#define FLASH_SSIZE 8192  // Sector (page) size, 8k

// Keep OTA data at the beginning of the last sector of the flash bank
#define FLASH_BANK1_OTA_DATA (FLASH_BANK1 + FLASH_SIZE1 - FLASH_SSIZE)
#define FLASH_BANK2_OTA_DATA (FLASH_BANK2 + FLASH_SIZE2 - FLASH_SSIZE)

#define FLASH_BASE 0x40022000          // Base address of the flash controller
#define FLASH_KEYR (FLASH_BASE + 0x4)  // See RM0481 7.11
#define FLASH_OPTKEYR (FLASH_BASE + 0xc)
#define FLASH_OPTCR (FLASH_BASE + 0x1c)
#define FLASH_NSSR (FLASH_BASE + 0x20)
#define FLASH_NSCR (FLASH_BASE + 0x28)
#define FLASH_NSCCR (FLASH_BASE + 0x30)
#define FLASH_OPTSR_CUR (FLASH_BASE + 0x50)
#define FLASH_OPTSR_PRG (FLASH_BASE + 0x54)

#undef REG
#define REG(x) ((volatile uint32_t *) (x))[0]
#undef BIT
#define BIT(x) (((uint32_t) 1U) << (x))
#undef SETBITS
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)

static uint32_t s_addr;   // Current address to write to
static uint32_t s_crc32;  // Firmware checksum

static void flash_unlock(void) {
  static bool unlocked = false;
  if (unlocked == false) {
    REG(FLASH_KEYR) = 0x45670123;
    REG(FLASH_KEYR) = 0Xcdef89ab;
    REG(FLASH_OPTKEYR) = 0x08192a3b;
    REG(FLASH_OPTKEYR) = 0x4c5d6e7f;
    unlocked = true;
  }
}

static int flash_page_start(volatile uint32_t *dst) {
  uint32_t addr = (uint32_t) (uintptr_t) dst;
  return (addr & (FLASH_SSIZE - 1)) == 0;
}

static bool flash_is_err(void) {
  return REG(FLASH_NSSR) & ((BIT(8) - 1) << 17);  // RM0481 7.11.9
}

static void flash_wait(void) {
  while ((REG(FLASH_NSSR) & BIT(0)) && (REG(FLASH_NSSR) & BIT(16)) == 0) {
    (void) 0;
  }
}

static void flash_prep(void) {
  flash_wait();                              // Wait until ready
  REG(FLASH_NSCR) = 0UL;                     // Clear control reg
  REG(FLASH_NSCCR) = ((BIT(9) - 1) << 16U);  // Clear all errors
}

static bool flash_bank_is_swapped(void) {
  return REG(FLASH_OPTCR) & BIT(31);  // RM0481 7.11.8
}

static void flash_erase(uint32_t sector) {
  flash_prep();
  if ((sector < 128 && flash_bank_is_swapped()) ||
      (sector > 127 && !flash_bank_is_swapped())) {
    REG(FLASH_NSCR) |= BIT(31);  // Set FLASH_CR_BKSEL
  }
  if (sector > 127) sector -= 128;
  REG(FLASH_NSCR) |= BIT(2) | (sector << 6);  // SectorErase | sector_num
  REG(FLASH_NSCR) |= BIT(5);                  // Start erasing
  // MG_INFO(("ERASE %lu, CR %#lx SR %#lx", sector, REG(FLASH_NSCR),
  // REG(FLASH_NSSR)));
  flash_prep();
}

static bool flash_swap_bank(void) {
  uint32_t desired = flash_bank_is_swapped() ? 0 : BIT(31);
  flash_unlock();
  flash_prep();
  // printf("OPTSR_PRG 1 %#lx\n", FLASH->OPTSR_PRG);
  SETBITS(REG(FLASH_OPTSR_PRG), BIT(31), desired);
  // printf("OPTSR_PRG 2 %#lx\n", FLASH->OPTSR_PRG);
  REG(FLASH_OPTCR) |= BIT(1);  // OPTSTART
  while ((REG(FLASH_OPTSR_CUR) & BIT(31)) != desired) (void) 0;
  return true;
}

static bool flash_write(uint32_t addr, const void *buf, size_t len) {
  volatile uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool success = true;
  flash_unlock();
  flash_prep();
  while (success && src < end) {
    uint32_t pageno = ((uint32_t) dst - FLASH_BANK1) / FLASH_SSIZE;
    if (flash_page_start(dst)) flash_erase(pageno);
    REG(FLASH_NSCR) = BIT(1);  // Set programming flag
    do {
      *dst++ = *src++;
      flash_wait();
    } while (src < end && !flash_page_start(dst) && !flash_is_err());
#if 0
    do {
      *dst++ = *src++;
      printf("WRITE %p, CR %#lx SR %#lx OCR %#lx %#lx %#lx\n", dst, FLASH->NSCR,
             FLASH->NSSR, FLASH->OPTCR, src[-1], dst[-1]);
    } while (src < end && !flash_page_start(dst));
#endif
    // if (FLASH->NSSR & FLASH_SR_WBNE) FLASH->NSCR |= FLASH_CR_FW;
    if (REG(FLASH_NSSR) & BIT(1)) REG(FLASH_NSCR) |= BIT(4);
    MG_INFO(("W %p, CR %#lx SR %#lx OCR %#lx %#lx %#lx", dst, REG(FLASH_NSCR),
             REG(FLASH_NSSR), REG(FLASH_OPTCR), src[-1], dst[-1]));
    flash_wait();
    if (flash_is_err() || (src[-1] != dst[-1])) {
      // printf("  E %lx CR %#lx SR %#lx, %#lx %#lx\n", (unsigned long) dst,
      //        FLASH->NSCR, FLASH->NSSR, src[-1], dst[-1]);
      success = false;
    }
  }
  return success;
}
void mg_sys_reset(void) {
  // SCB->AIRCR = ((0x5fa << SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk);
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}

bool mg_ota_begin(size_t new_firmware_size) {
  s_crc32 = 0;
  s_addr = FLASH_BANK2;
  MG_DEBUG(("Firmware %lu bytes, max %lu", new_firmware_size, FLASH_SIZE2));
  return new_firmware_size < FLASH_SIZE2;
}

bool mg_ota_write(const void *buf, size_t len) {
  bool ok = flash_write(s_addr, buf, len);         // Write chunk to flash
  s_crc32 = mg_crc32(s_crc32, (char *) buf, len);  // Update CRC
  MG_DEBUG(("%#x %p %lu -> %d", s_addr, buf, len, ok));
  s_addr += len;
  return ok;
}

static bool w32(uint32_t addr, uint32_t value) {
  return flash_write(addr, &value, sizeof(value));
}

bool mg_ota_end(void) {
  bool ok = false;
  size_t size = s_addr - FLASH_BANK2;
  uint32_t crc = mg_crc32(0, (char *) FLASH_BANK2, size);
  if (crc == s_crc32) {
    uint32_t now = (uint32_t) (mg_now() / 1000);
    struct mg_ota_data od = {MG_OTA_MAGIC, crc, size, now, -1, -1};
    ok = flash_write(FLASH_BANK2_OTA_DATA, &od, sizeof(od));
  }
  MG_DEBUG(("CRC check: %x %x", s_crc32, crc));
  return ok && flash_swap_bank();
}

bool mg_ota_status(struct mg_ota_data od[2]) {
  od[0] = *(struct mg_ota_data *) FLASH_BANK1_OTA_DATA;
  od[1] = *(struct mg_ota_data *) FLASH_BANK2_OTA_DATA;
  return od[0].magic == MG_OTA_MAGIC;
}

bool mg_ota_commit(void) {
  struct mg_ota_data *od = (struct mg_ota_data *) FLASH_BANK1_OTA_DATA;
  bool ok = false;
  if (od->magic == MG_OTA_MAGIC && od->golden == 0) {
    ok = true;                             // Already clean, do nothing
  } else if (od->magic == MG_OTA_MAGIC) {  // Dirty!
    ok = w32((uint32_t) &od->golden, 0);
  }
  return ok;
}

bool mg_ota_rollback(void) {
  return flash_swap_bank();
}
#endif
