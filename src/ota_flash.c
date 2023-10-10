#include "arch.h"
#include "device.h"
#include "log.h"
#include "ota.h"

// This OTA implementation uses the internal flash API outlined in device.h
// It splits flash into 2 equal partitions, and stores OTA status in the
// last sector of the partition.

#if MG_OTA == MG_OTA_FLASH

#define MG_OTADATA_KEY 0xb07afed0

static char *s_addr;      // Current address to write to
static size_t s_size;     // Firmware size to flash. In-progress indicator
static uint32_t s_crc32;  // Firmware checksum
static bool s_is_swapping;  // Firmwares are currently in the swapping process

struct mg_otadata {
  uint32_t crc32, size, timestamp, status, rollback;
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
      struct mg_otadata od = {crc32, size, now, MG_OTA_FIRST_BOOT, false};
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
#if MG_DEVICE_DUAL_BANK
  struct mg_otadata od = {};
  int bank = mg_flash_bank();
  uint32_t key = MG_OTADATA_KEY + 1;
  if ((fw == MG_FIRMWARE_CURRENT && bank == 2)) key++;
  if ((fw == MG_FIRMWARE_PREVIOUS && bank == 1)) key++;
  mg_flash_load(NULL, key, &od, sizeof(od));
  // MG_DEBUG(("Loaded OTA data. fw %d, bank %d, key %p", fw, bank, key));
  // mg_hexdump(&od, sizeof(od));
  return od;
#else
  struct mg_otadata od = {};
  uint32_t key = 0;
  if (fw == MG_FIRMWARE_CURRENT) {
    key = MG_OTADATA_KEY + 1;
  } else if (fw == MG_FIRMWARE_PREVIOUS) {
    key = MG_OTADATA_KEY + 2;
  }
  mg_flash_load(NULL, key, &od, sizeof(od));
  return od;
#endif
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

uint32_t mg_ota_is_rollback(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.rollback;
}

bool mg_ota_commit(void) {
  struct mg_otadata od = mg_otadata(MG_FIRMWARE_CURRENT);
  od.status = MG_OTA_COMMITTED;
  uint32_t key = MG_OTADATA_KEY + mg_flash_bank();
  return mg_flash_save(NULL, key, &od, sizeof(od));
}

bool mg_ota_rollback(void) {
  MG_DEBUG(("Rolling firmware back"));
#if MG_DEVICE_DUAL_BANK == 0
  struct mg_otadata od = mg_otadata(MG_FIRMWARE_PREVIOUS);
  if (od.status == MG_OTA_UNAVAILABLE) return false;
  od.rollback = true;
  mg_flash_save(NULL, MG_OTADATA_KEY + 2, &od, sizeof(od));
#endif
  return mg_flash_swap_bank();
}

static bool MG_IRAM mg_ota_copy_to_flash(char *dst, char *src, size_t len) {
  size_t align = mg_flash_write_align();
  size_t len_aligned_down = MG_ROUND_DOWN(len, align);
  bool ok = false;
  if (len_aligned_down) ok = mg_flash_write(dst, src, len_aligned_down);
  if (!ok) return ok;
  if (len_aligned_down < len) {
    size_t left = len - len_aligned_down;
    char tmp[align];
    for (size_t i = 0; i < sizeof(tmp); i++) tmp[i] = 0xff;
    for (size_t i = 0; i < left; i++) {
      tmp[i] = src[len_aligned_down + i];
    }
    ok = mg_flash_write(dst + len_aligned_down, tmp, sizeof(tmp));
  }

  return ok;
}

static bool MG_IRAM mg_ota_swap_sectors(char *sector_1, char *sector_2,
                                        size_t len_1, size_t len_2) {
  if (len_1 > mg_flash_sector_size() || len_2 > mg_flash_sector_size())
    return false;

  // copy from sector 1 to RAM
  char ram_tmp[len_1];
  char *c = ram_tmp;
  for (uint32_t i = 0; i < len_1; i++) {
    *c++ = sector_1[i];
  }

  bool ok = mg_ota_copy_to_flash(sector_1, sector_2, len_2);
  if (!ok) return false;
  ok = mg_ota_copy_to_flash(sector_2, ram_tmp, len_1);
  return ok;
}

bool MG_IRAM mg_ota_is_swapping(void) {
  return s_is_swapping;
}

void MG_IRAM mg_ota_bootloader(void) {
  struct mg_otadata crnt_ot = mg_otadata(MG_FIRMWARE_CURRENT);
  struct mg_otadata prev_ot = mg_otadata(MG_FIRMWARE_PREVIOUS);

  if (crnt_ot.status == MG_OTA_FIRST_BOOT) {
    crnt_ot.status = MG_OTA_UNCOMMITTED;
    mg_flash_save(NULL, MG_OTADATA_KEY + 1, &crnt_ot, sizeof(crnt_ot));
  } else if (crnt_ot.rollback) {
    crnt_ot.rollback = false;
    mg_flash_save(NULL, MG_OTADATA_KEY + 1, &crnt_ot, sizeof(crnt_ot));
  } else if (prev_ot.status == MG_OTA_FIRST_BOOT || prev_ot.rollback) {
    int crnt_sector_1 = 0, crnt_sector_2 = 0;
    char *sector_1 = mg_flash_start();
    char *sector_2 = mg_flash_start() + mg_flash_size() / 2;

    if (crnt_ot.size == 0)
      crnt_ot.size = mg_flash_sector_size();
    int sector_count_1 = crnt_ot.size / mg_flash_sector_size();
    if (crnt_ot.size % mg_flash_sector_size()) sector_count_1++;
    if (sector_count_1 > 3) {
      MG_ERROR(("Firmware in the first partition exceeds maximum size."));
      return;
    }

    if (prev_ot.size == 0)
      prev_ot.size = mg_flash_sector_size();
    int sector_count_2 = prev_ot.size / mg_flash_sector_size();
    if (prev_ot.size % mg_flash_sector_size()) sector_count_2++;
    if (sector_count_2 > 3) {
      MG_ERROR(("Firmware in the second partition exceeds maximum size."));
      return;
    }

    MG_DEBUG(("Starting firmware swap\n"));
    mg_flash_save(NULL, MG_OTADATA_KEY + 1, &prev_ot, sizeof(prev_ot));
    mg_flash_save(NULL, MG_OTADATA_KEY + 2, &crnt_ot, sizeof(crnt_ot));

    s_is_swapping = true;
    for (; crnt_sector_1 < sector_count_1 && crnt_sector_2 < sector_count_2;
         crnt_sector_1++, crnt_sector_2++) {
      size_t len_1, len_2;
      if (crnt_sector_1 == sector_count_1 - 1) {
        len_1 = crnt_ot.size % mg_flash_sector_size();
        if (len_1 == 0) len_1 = mg_flash_sector_size();
      } else {
        len_1 = mg_flash_sector_size();
      }

      if (crnt_sector_2 == sector_count_2 - 1) {
        len_2 = prev_ot.size % mg_flash_sector_size();
        if (len_2 == 0) len_2 = mg_flash_sector_size();
      } else {
        len_2 = mg_flash_sector_size();
      }

      mg_ota_swap_sectors(sector_1, sector_2, len_1, len_2);
      sector_1 += mg_flash_sector_size();
      sector_2 += mg_flash_sector_size();
    }

    while (crnt_sector_1 < sector_count_1) {
      size_t len = mg_flash_sector_size();
      if (crnt_sector_1 == sector_count_1 - 1)
        len = crnt_ot.size % mg_flash_sector_size();

      mg_ota_copy_to_flash(sector_2, sector_1, len);
      sector_1 += mg_flash_sector_size();
      sector_2 += mg_flash_sector_size();
      crnt_sector_1++;
    }

    while (crnt_sector_2 < sector_count_2) {
      size_t len = mg_flash_sector_size();
      if (crnt_sector_2 == sector_count_2 - 1)
        len = prev_ot.size % mg_flash_sector_size();

      mg_ota_copy_to_flash(sector_1, sector_2, len);
      sector_1 += mg_flash_sector_size();
      sector_2 += mg_flash_sector_size();
      crnt_sector_2++;
    }

    void (*firmware_reset_handler)(void) =
        (void (*)(void)) * ((uint32_t *) (mg_flash_start() + 4));
    firmware_reset_handler();
  }
}

#endif
