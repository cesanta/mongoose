#include "arch.h"
#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA != MG_OTA_NONE && MG_OTA != MG_OTA_CUSTOM

static char *s_addr;      // Current address to write to
static size_t s_size;     // Firmware size to flash. In-progress indicator
static uint32_t s_crc32;  // Firmware checksum

bool mg_ota_flash_begin(size_t new_firmware_size, struct mg_flash *flash) {
  bool ok = false;
  if (s_size) {
    MG_ERROR(("OTA already in progress. Call mg_ota_end()"));
  } else {
    size_t half = flash->size / 2;
    s_crc32 = 0;
    s_addr = (char *) flash->start + half;
    MG_DEBUG(("FW %lu bytes, max %lu", new_firmware_size, half));
    if (new_firmware_size < half) {
      ok = true;
      s_size = new_firmware_size;
      MG_INFO(("Starting OTA, firmware size %lu", s_size));
    } else {
      MG_ERROR(("Firmware %lu is too big to fit %lu", new_firmware_size, half));
    }
  }
  return ok;
}

bool mg_ota_flash_write(const void *buf, size_t len, struct mg_flash *flash) {
  bool ok = false;
  if (s_size == 0) {
    MG_ERROR(("OTA is not started, call mg_ota_begin()"));
  } else {
    size_t len_aligned_down = MG_ROUND_DOWN(len, flash->align);
    if (len_aligned_down) ok = flash->write_fn(s_addr, buf, len_aligned_down);
    if (len_aligned_down < len) {
      size_t left = len - len_aligned_down;
      char tmp[flash->align];
      memset(tmp, 0xff, sizeof(tmp));
      memcpy(tmp, (char *) buf + len_aligned_down, left);
      ok = flash->write_fn(s_addr + len_aligned_down, tmp, sizeof(tmp));
    }
    s_crc32 = mg_crc32(s_crc32, (char *) buf, len);  // Update CRC
    MG_DEBUG(("%#x %p %lu -> %d", s_addr - len, buf, len, ok));
    s_addr += len;
  }
  return ok;
}

bool mg_ota_flash_end(struct mg_flash *flash) {
  char *base = (char *) flash->start + flash->size / 2;
  bool ok = false;
  if (s_size) {
    size_t size = (size_t) (s_addr - base);
    uint32_t crc32 = mg_crc32(0, base, s_size);
    if (size == s_size && crc32 == s_crc32) ok = true;
    MG_DEBUG(("CRC: %x/%x, size: %lu/%lu, status: %s", s_crc32, crc32, s_size,
              size, ok ? "ok" : "fail"));
    s_size = 0;
    if (ok) ok = flash->swap_fn();
  }
  MG_INFO(("Finishing OTA: %s", ok ? "ok" : "fail"));
  return ok;
}

#endif
