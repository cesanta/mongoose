#include "arch.h"
#include "log.h"
#include "ota.h"
#include "device.h"

// This OTA implementation uses the internal flash API outlined in device.h
// It splits flash into 2 equal partitions, and stores OTA status in the
// last sector of the partition.

#if MG_OTA == MG_OTA_FLASH

#define MG_OTADATA_KEY 0xb07afed0

static char *s_addr;      // Current address to write to
static size_t s_size;     // Firmware size to flash. In-progress indicator
static uint32_t s_crc32;  // Firmware checksum

struct mg_otadata {
  uint32_t crc32, size, timestamp, status;
};

bool mg_ota_begin(size_t new_firmware_size) {
  bool ok = false;
  if (s_size) {
    MG_ERROR(("OTA already in progress. Call mg_ota_end()"));
  } else {
    size_t half = mg_flash_size() / 2, max = half - mg_flash_sector_size();
    s_crc32 = 0;
    s_addr = (char *) mg_flash_start() + half;
    MG_DEBUG(("Firmware %lu bytes, max %lu", s_size, max));
    if (new_firmware_size < max) {
      ok = true;
      s_size = new_firmware_size;
      MG_INFO(("Starting OTA, firmware size %lu", s_size));
    } else {
      MG_ERROR(("Firmware %lu is too big to fit %lu", new_firmware_size, max));
    }
  }
  return ok;
}

bool mg_ota_write(const void *buf, size_t len) {
  bool ok = false;
  if (s_size == 0) {
    MG_ERROR(("OTA is not started, call mg_ota_begin()"));
  } else {
    size_t align = mg_flash_write_align();
    size_t len_aligned_down = MG_ROUND_DOWN(len, align);
    if (len_aligned_down) ok = mg_flash_write(s_addr, buf, len_aligned_down);
    if (len_aligned_down < len) {
      size_t left = len - len_aligned_down;
      char tmp[align];
      memset(tmp, 0xff, sizeof(tmp));
      memcpy(tmp, (char *) buf + len_aligned_down, left);
      ok = mg_flash_write(s_addr + len_aligned_down, tmp, sizeof(tmp));
    }
    s_crc32 = mg_crc32(s_crc32, (char *) buf, len);  // Update CRC
    MG_DEBUG(("%#x %p %lu -> %d", s_addr - len, buf, len, ok));
    s_addr += len;
  }
  return ok;
}

bool mg_ota_end(void) {
  char *base = (char *) mg_flash_start() + mg_flash_size() / 2;
  bool ok = false;
  if (s_size) {
    size_t size = s_addr - base;
    uint32_t crc32 = mg_crc32(0, base, s_size);
    if (size == s_size && crc32 == s_crc32) {
      uint32_t now = (uint32_t) (mg_now() / 1000);
      struct mg_otadata od = {crc32, size, now, MG_OTA_FIRST_BOOT};
      uint32_t key = MG_OTADATA_KEY + (mg_flash_bank() == 2 ? 1 : 2);
      ok = mg_flash_save(NULL, key, &od, sizeof(od));
    }
    MG_DEBUG(("CRC: %x/%x, size: %lu/%lu, status: %s", s_crc32, crc32, s_size,
              size, ok ? "ok" : "fail"));
    s_size = 0;
    if (ok) ok = mg_flash_swap_bank();
  }
  MG_INFO(("Finishing OTA: %s", ok ? "ok" : "fail"));
  return ok;
}

static struct mg_otadata mg_otadata(int fw) {
  struct mg_otadata od = {};
  int bank = mg_flash_bank();
  uint32_t key = MG_OTADATA_KEY + 1;
  if ((fw == MG_FIRMWARE_CURRENT && bank == 2)) key++;
  if ((fw == MG_FIRMWARE_PREVIOUS && bank == 1)) key++;
  mg_flash_load(NULL, key, &od, sizeof(od));
  // MG_DEBUG(("Loaded OTA data. fw %d, bank %d, key %p", fw, bank, key));
  // mg_hexdump(&od, sizeof(od));
  return od;
}

int mg_ota_status(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.status;
}
uint32_t mg_ota_crc32(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.crc32;
}
uint32_t mg_ota_timestamp(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.timestamp;
}
size_t mg_ota_size(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.size;
}

bool mg_ota_commit(void) {
  struct mg_otadata od = mg_otadata(MG_FIRMWARE_CURRENT);
  od.status = MG_OTA_COMMITTED;
  uint32_t key = MG_OTADATA_KEY + mg_flash_bank();
  return mg_flash_save(NULL, key, &od, sizeof(od));
}

bool mg_ota_rollback(void) {
  MG_DEBUG(("Rolling firmware back"));
  return mg_flash_swap_bank();
}
#endif
