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
    MG_DEBUG(("Firmware %lu bytes, max %lu", new_firmware_size, max));
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

MG_IRAM static uint32_t mg_fwkey(int fw) {
  uint32_t key = MG_OTADATA_KEY + fw;
  int bank = mg_flash_bank();
  if (bank == 2 && fw == MG_FIRMWARE_PREVIOUS) key--;
  if (bank == 2 && fw == MG_FIRMWARE_CURRENT) key++;
  return key;
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
      uint32_t key = mg_fwkey(MG_FIRMWARE_PREVIOUS);
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

MG_IRAM static struct mg_otadata mg_otadata(int fw) {
  uint32_t key = mg_fwkey(fw);
  struct mg_otadata od = {};
  MG_INFO(("Loading %s OTA data", fw == MG_FIRMWARE_CURRENT ? "curr" : "prev"));
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

MG_IRAM bool mg_ota_commit(void) {
  bool ok = true;
  struct mg_otadata od = mg_otadata(MG_FIRMWARE_CURRENT);
  if (od.status != MG_OTA_COMMITTED) {
    od.status = MG_OTA_COMMITTED;
    MG_INFO(("Committing current firmware, OD size %lu", sizeof(od)));
    ok = mg_flash_save(NULL, mg_fwkey(MG_FIRMWARE_CURRENT), &od, sizeof(od));
  }
  return ok;
}

bool mg_ota_rollback(void) {
  MG_DEBUG(("Rolling firmware back"));
  if (mg_flash_bank() == 0) {
    // No dual bank support. Mark previous firmware as FIRST_BOOT
    struct mg_otadata prev = mg_otadata(MG_FIRMWARE_PREVIOUS);
    prev.status = MG_OTA_FIRST_BOOT;
    return mg_flash_save(NULL, MG_OTADATA_KEY + MG_FIRMWARE_PREVIOUS, &prev,
                         sizeof(prev));
  } else {
    return mg_flash_swap_bank();
  }
}

MG_IRAM void mg_ota_boot(void) {
  MG_INFO(("Booting. Flash bank: %d", mg_flash_bank()));
  struct mg_otadata curr = mg_otadata(MG_FIRMWARE_CURRENT);
  struct mg_otadata prev = mg_otadata(MG_FIRMWARE_PREVIOUS);

  if (curr.status == MG_OTA_FIRST_BOOT) {
    if (prev.status == MG_OTA_UNAVAILABLE) {
      MG_INFO(("Setting previous firmware state to committed"));
      prev.status = MG_OTA_COMMITTED;
      mg_flash_save(NULL, mg_fwkey(MG_FIRMWARE_PREVIOUS), &prev, sizeof(prev));
    }
    curr.status = MG_OTA_UNCOMMITTED;
    MG_INFO(("First boot, setting status to UNCOMMITTED"));
    mg_flash_save(NULL, mg_fwkey(MG_FIRMWARE_CURRENT), &curr, sizeof(curr));
  } else if (prev.status == MG_OTA_FIRST_BOOT && mg_flash_bank() == 0) {
    // Swap paritions. Pray power does not disappear
    size_t fs = mg_flash_size(), ss = mg_flash_sector_size();
    char *partition1 = mg_flash_start();
    char *partition2 = mg_flash_start() + fs / 2;
    size_t ofs, max = fs / 2 - ss;  // Set swap size to the whole partition

    if (curr.status != MG_OTA_UNAVAILABLE &&
        prev.status != MG_OTA_UNAVAILABLE) {
      // We know exact sizes of both firmwares.
      // Shrink swap size to the MAX(firmware1, firmware2)
      size_t sz = curr.size > prev.size ? curr.size : prev.size;
      if (sz > 0 && sz < max) max = sz;
    }

    // MG_OTA_FIRST_BOOT -> MG_OTA_UNCOMMITTED
    prev.status = MG_OTA_UNCOMMITTED;
    mg_flash_save(NULL, MG_OTADATA_KEY + MG_FIRMWARE_CURRENT, &prev,
                  sizeof(prev));
    mg_flash_save(NULL, MG_OTADATA_KEY + MG_FIRMWARE_PREVIOUS, &curr,
                  sizeof(curr));

    MG_INFO(("Swapping partitions, size %u (%u sectors)", max, max / ss));
    MG_INFO(("Do NOT power off..."));
    mg_log_level = MG_LL_NONE;

    // We use the last sector of partition2 for OTA data/config storage
    // Therefore we can use last sector of partition1 for swapping
    char *tmpsector = partition1 + fs / 2 - ss;  // Last sector of partition1
    (void) tmpsector;
    for (ofs = 0; ofs < max; ofs += ss) {
      // mg_flash_erase(tmpsector);
      mg_flash_write(tmpsector, partition1 + ofs, ss);
      // mg_flash_erase(partition1 + ofs);
      mg_flash_write(partition1 + ofs, partition2 + ofs, ss);
      // mg_flash_erase(partition2 + ofs);
      mg_flash_write(partition2 + ofs, tmpsector, ss);
    }
    mg_device_reset();
  }
}
#endif
