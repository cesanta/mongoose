#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_MCXW71

// MCXW71SRM, section 11.2: ROM flash API, 8 KB sectors, 16-byte phrases,
// 128-byte pages. Only the CM33 application flash is used; RF flash and IFR
// (including the write-once ROM configuration) are left alone.
struct mg_mcxw71_flash_config {
  struct {
    uint32_t base, size, blocks;
    uint32_t ifr_base, ifr_size;
  } mem[2];
};

struct mg_mcxw71_flash_api {
  uint32_t (*init)(struct mg_mcxw71_flash_config *);
  uint32_t (*erase)(struct mg_mcxw71_flash_config *, void *, uint32_t, uint32_t,
                    uint32_t);
  uint32_t (*phrase)(struct mg_mcxw71_flash_config *, void *, uint32_t,
                     uint32_t *, uint32_t);
  uint32_t (*page)(struct mg_mcxw71_flash_config *, void *, uint32_t,
                   uint32_t *, uint32_t);
};

// The flash API pointer is the second entry in the ROM API table.
#define MG_MCXW71_FLASH_API \
  (*(const struct mg_mcxw71_flash_api *const *) 0x14816fe4U)
#define MG_MCXW71_ERASE_KEY 0x6b65666cU  // FOUR_CHAR_CODE('l', 'f', 'e', 'k')

static struct mg_mcxw71_flash_config s_mcxw71_config;
static size_t s_mcxw71_size, s_mcxw71_received, s_mcxw71_pending;
static uint32_t s_mcxw71_buf[4];
static bool s_mcxw71_failed;

static bool mg_mcxw71_write(void *, const void *, size_t);
static bool mg_mcxw71_swap(void);
static struct mg_flash s_mg_flash_mcxw71 = {
    (void *) 0, 0x100000, 8192, 16, mg_mcxw71_write, mg_mcxw71_swap};
struct mg_flash *mg_flash = &s_mg_flash_mcxw71;

// Everything called while flash is busy, or after the application has been
// erased, must execute from RAM or ROM. Do not log or call libc here.
MG_IRAM static bool mg_mcxw71_write(void *addr, const void *buf, size_t len) {
  uintptr_t dst = (uintptr_t) addr;
  const volatile uint8_t *src = (const volatile uint8_t *) buf;
  uint32_t irq, status = 0;
  if (dst >= s_mg_flash_mcxw71.size || len > s_mg_flash_mcxw71.size - dst ||
      (dst % 16) != 0 || (len % 16) != 0)
    return false;

  irq = __get_PRIMASK();
  __disable_irq();
  while (len > 0 && status == 0) {
    uint32_t words[32];  // ROM needs a word-aligned source in RAM
    volatile uint8_t *p = (volatile uint8_t *) words;
    size_t i, n = (dst % 128) == 0 && len >= 128 ? 128 : 16;
    for (i = 0; i < n; i++) p[i] = src[i];

    // Match the MCUXpresso ROM wrappers' cache maintenance. Interrupts stay
    // masked until the flash command completes and stale cache data is gone.
    MCM->CPCR2 |= MCM_CPCR2_CCBC_MASK;
    __DSB();
    __ISB();
    if ((dst % 8192) == 0) {
      status = MG_MCXW71_FLASH_API->erase(
          &s_mcxw71_config, FMU0, (uint32_t) dst, 8192, MG_MCXW71_ERASE_KEY);
    }
    if (status == 0) {
      if (n == 128) {
        status = MG_MCXW71_FLASH_API->page(&s_mcxw71_config, FMU0,
                                           (uint32_t) dst, words, 128);
      } else {
        status = MG_MCXW71_FLASH_API->phrase(&s_mcxw71_config, FMU0,
                                             (uint32_t) dst, words, 16);
      }
    }
    MCM->CPCR2 |= MCM_CPCR2_CCBC_MASK;
    __DSB();
    __ISB();
    dst += n, src += n, len -= n;
  }
  __set_PRIMASK(irq);
  return status == 0;
}

static bool mg_mcxw71_swap(void) {
  // The common helper verifies the staged image before the RAM copy below.
  // A failed write must never commit, even if all expected bytes arrived.
  return !s_mcxw71_failed;
}

MG_IRAM static void mg_mcxw71_apply(size_t size) {
  size_t ofs, half = s_mg_flash_mcxw71.size / 2;
  __disable_irq();
  SysTick->CTRL = 0;
  for (ofs = 0; ofs < size; ofs += 8192) {
    if (!mg_mcxw71_write((void *) (uintptr_t) ofs,
                         (const void *) (uintptr_t) (half + ofs), 8192)) {
      // The application may already be erased. Stay in RAM for debugger
      // recovery rather than return to partially overwritten flash.
      for (;;) __NOP();
    }
  }
  __DSB();
  SCB->AIRCR = 0x05fa0004U;
  __DSB();
  for (;;) __NOP();
}

bool mg_ota_begin(size_t new_firmware_size) {
  uint32_t status;
  if (s_mcxw71_size != 0 || new_firmware_size == 0) return false;
  status = MG_MCXW71_FLASH_API->init(&s_mcxw71_config);
  if (status != 0 || s_mcxw71_config.mem[0].base != 0 ||
      s_mcxw71_config.mem[0].size < 16384 ||
      (s_mcxw71_config.mem[0].size % 16384) != 0) {
    MG_ERROR(("Flash init failed: %lu", (unsigned long) status));
    return false;
  }
  s_mg_flash_mcxw71.size = s_mcxw71_config.mem[0].size;
  if (!mg_ota_flash_begin(new_firmware_size, &s_mg_flash_mcxw71)) return false;
  s_mcxw71_size = new_firmware_size;
  s_mcxw71_received = s_mcxw71_pending = 0;
  s_mcxw71_failed = false;
  return true;
}

bool mg_ota_write(const void *buf, size_t len) {
  const uint8_t *p = (const uint8_t *) buf;
  uint8_t *pending = (uint8_t *) s_mcxw71_buf;
  if (s_mcxw71_size == 0 || s_mcxw71_failed) return false;
  if (len > s_mcxw71_size - s_mcxw71_received || (len > 0 && buf == NULL)) {
    s_mcxw71_failed = true;
    return false;
  }
  s_mcxw71_received += len;
  if (s_mcxw71_pending > 0) {
    size_t n = 16 - s_mcxw71_pending;
    if (n > len) n = len;
    if (n > 0) memcpy(pending + s_mcxw71_pending, p, n);
    s_mcxw71_pending += n;
    if (n > 0) p += n;
    len -= n;
    if (s_mcxw71_pending == 16) {
      s_mcxw71_failed = !mg_ota_flash_write(pending, 16, &s_mg_flash_mcxw71);
      s_mcxw71_pending = 0;
    }
  }
  if (!s_mcxw71_failed && len >= 16) {
    size_t n = MG_ROUND_DOWN(len, 16);
    s_mcxw71_failed = !mg_ota_flash_write(p, n, &s_mg_flash_mcxw71);
    p += n, len -= n;
  }
  if (!s_mcxw71_failed && len > 0) {
    memcpy(pending + s_mcxw71_pending, p, len);
    s_mcxw71_pending += len;
  }
  return !s_mcxw71_failed;
}

bool mg_ota_end(void) {
  bool ok;
  size_t size = s_mcxw71_size;
  if (size == 0) return false;
  if (s_mcxw71_received != size) s_mcxw71_failed = true;
  if (!s_mcxw71_failed && s_mcxw71_pending > 0) {
    s_mcxw71_failed =
        !mg_ota_flash_write(s_mcxw71_buf, s_mcxw71_pending, &s_mg_flash_mcxw71);
  }
  ok = mg_ota_flash_end(&s_mg_flash_mcxw71);
  s_mcxw71_size = s_mcxw71_pending = 0;
  if (ok) {
    // Like RW612, this overwrites the application. It does not provide
    // rollback or power-loss recovery during the copy. Keep power connected.
    MG_INFO(("Applying %lu bytes from staging flash", (unsigned long) size));
    mg_mcxw71_apply(size);
  }
  return false;
}

#endif
