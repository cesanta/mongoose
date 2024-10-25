#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_CH32V307
// RM: https://www.wch-ic.com/downloads/CH32FV2x_V3xRM_PDF.html

static bool mg_ch32v307_write(void *, const void *, size_t);
static bool mg_ch32v307_swap(void);

static struct mg_flash s_mg_flash_ch32v307 = {
    (void *) 0x08000000,  // Start
    480 * 1024,           // Size, first 320k is 0-wait
    4 * 1024,             // Sector size, 4k
    4,                    // Align, 32 bit
    mg_ch32v307_write,
    mg_ch32v307_swap,
};

#define FLASH_BASE 0x40022000
#define FLASH_ACTLR (FLASH_BASE + 0)
#define FLASH_KEYR (FLASH_BASE + 4)
#define FLASH_OBKEYR (FLASH_BASE + 8)
#define FLASH_STATR (FLASH_BASE + 12)
#define FLASH_CTLR (FLASH_BASE + 16)
#define FLASH_ADDR (FLASH_BASE + 20)
#define FLASH_OBR (FLASH_BASE + 28)
#define FLASH_WPR (FLASH_BASE + 32)

MG_IRAM static void flash_unlock(void) {
  static bool unlocked;
  if (unlocked == false) {
    MG_REG(FLASH_KEYR) = 0x45670123;
    MG_REG(FLASH_KEYR) = 0xcdef89ab;
    unlocked = true;
  }
}

MG_IRAM static void flash_wait(void) {
  while (MG_REG(FLASH_STATR) & MG_BIT(0)) (void) 0;
}

MG_IRAM static void mg_ch32v307_erase(void *addr) {
  // MG_INFO(("%p", addr));
  flash_unlock();
  flash_wait();
  MG_REG(FLASH_ADDR) = (uint32_t) addr;
  MG_REG(FLASH_CTLR) |= MG_BIT(1) | MG_BIT(6);  // PER | STRT;
  flash_wait();
}

MG_IRAM static bool is_page_boundary(const void *addr) {
  uint32_t val = (uint32_t) addr;
  return (val & (s_mg_flash_ch32v307.secsz - 1)) == 0;
}

MG_IRAM static bool mg_ch32v307_write(void *addr, const void *buf, size_t len) {
  // MG_INFO(("%p %p %lu", addr, buf, len));
  // mg_hexdump(buf, len);
  flash_unlock();
  const uint16_t *src = (uint16_t *) buf, *end = &src[len / 2];
  uint16_t *dst = (uint16_t *) addr;
  MG_REG(FLASH_CTLR) |= MG_BIT(0);  // Set PG
  // MG_INFO(("CTLR: %#lx", MG_REG(FLASH_CTLR)));
  while (src < end) {
    if (is_page_boundary(dst)) mg_ch32v307_erase(dst);
    *dst++ = *src++;
    flash_wait();
  }
  MG_REG(FLASH_CTLR) &= ~MG_BIT(0);  // Clear PG
  return true;
}

MG_IRAM bool mg_ch32v307_swap(void) {
  return true;
}

// just overwrite instead of swap
MG_IRAM static void single_bank_swap(char *p1, char *p2, size_t s, size_t ss) {
  // no stdlib calls here
  for (size_t ofs = 0; ofs < s; ofs += ss) {
    mg_ch32v307_write(p1 + ofs, p2 + ofs, ss);
  }
  *((volatile uint32_t *) 0xbeef0000) |= 1U << 7;  // NVIC_SystemReset()
}

bool mg_ota_begin(size_t new_firmware_size) {
  return mg_ota_flash_begin(new_firmware_size, &s_mg_flash_ch32v307);
}

bool mg_ota_write(const void *buf, size_t len) {
  return mg_ota_flash_write(buf, len, &s_mg_flash_ch32v307);
}

bool mg_ota_end(void) {
  if (mg_ota_flash_end(&s_mg_flash_ch32v307)) {
    // Swap partitions. Pray power does not go away
    MG_INFO(("Swapping partitions, size %u (%u sectors)",
             s_mg_flash_ch32v307.size,
             s_mg_flash_ch32v307.size / s_mg_flash_ch32v307.secsz));
    MG_INFO(("Do NOT power off..."));
    mg_log_level = MG_LL_NONE;
    // TODO() disable IRQ, s_flash_irq_disabled = true;
    // Runs in RAM, will reset when finished
    single_bank_swap(
        (char *) s_mg_flash_ch32v307.start,
        (char *) s_mg_flash_ch32v307.start + s_mg_flash_ch32v307.size / 2,
        s_mg_flash_ch32v307.size / 2, s_mg_flash_ch32v307.secsz);
  }
  return false;
}
#endif
