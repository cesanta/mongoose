#include "device.h"
#include "log.h"

#if MG_DEVICE == MG_DEVICE_CH32V307
// RM: https://www.wch-ic.com/downloads/CH32FV2x_V3xRM_PDF.html

#define FLASH_BASE 0x40022000
#define FLASH_ACTLR (FLASH_BASE + 0)
#define FLASH_KEYR (FLASH_BASE + 4)
#define FLASH_OBKEYR (FLASH_BASE + 8)
#define FLASH_STATR (FLASH_BASE + 12)
#define FLASH_CTLR (FLASH_BASE + 16)
#define FLASH_ADDR (FLASH_BASE + 20)
#define FLASH_OBR (FLASH_BASE + 28)
#define FLASH_WPR (FLASH_BASE + 32)

void *mg_flash_start(void) {
  return (void *) 0x08000000;
}
size_t mg_flash_size(void) {
  return 480 * 1024;  // First 320k is 0-wait
}
size_t mg_flash_sector_size(void) {
  return 4096;
}
size_t mg_flash_write_align(void) {
  return 4;
}
int mg_flash_bank(void) {
  return 0;
}
void mg_device_reset(void) {
  *((volatile uint32_t *) 0xbeef0000) |= 1U << 7;  // NVIC_SystemReset()
}
static void flash_unlock(void) {
  static bool unlocked;
  if (unlocked == false) {
    MG_REG(FLASH_KEYR) = 0x45670123;
    MG_REG(FLASH_KEYR) = 0xcdef89ab;
    unlocked = true;
  }
}
static void flash_wait(void) {
  while (MG_REG(FLASH_STATR) & MG_BIT(0)) (void) 0;
}

bool mg_flash_erase(void *addr) {
  //MG_INFO(("%p", addr));
  flash_unlock();
  flash_wait();
  MG_REG(FLASH_ADDR) = (uint32_t) addr;
  MG_REG(FLASH_CTLR) |= MG_BIT(1) | MG_BIT(6);  // PER | STRT;
  flash_wait();
  return true;
}

static bool is_page_boundary(const void *addr) {
  uint32_t val = (uint32_t) addr;
  return (val & (mg_flash_sector_size() - 1)) == 0;
}

bool mg_flash_write(void *addr, const void *buf, size_t len) {
  //MG_INFO(("%p %p %lu", addr, buf, len));
  //mg_hexdump(buf, len);
  flash_unlock();
  const uint16_t *src = (uint16_t *) buf, *end = &src[len / 2];
  uint16_t *dst = (uint16_t *) addr;
  MG_REG(FLASH_CTLR) |= MG_BIT(0);  // Set PG
  //MG_INFO(("CTLR: %#lx", MG_REG(FLASH_CTLR)));
  while (src < end) {
    if (is_page_boundary(dst)) mg_flash_erase(dst);
    *dst++ = *src++;
    flash_wait();
  }
  MG_REG(FLASH_CTLR) &= ~MG_BIT(0);  // Clear PG
  return true;
}
#endif
