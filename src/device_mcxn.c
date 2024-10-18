#include "device.h"
#include "log.h"

#if MG_DEVICE == MG_DEVICE_MCXN

struct mg_flash_config {
uint32_t addr;
uint32_t size;
uint32_t blocks;
uint32_t page_size;
uint32_t sector_size;
uint32_t ffr[6];
uint32_t reserved0[5];
uint32_t *bootctx;
bool useahb;
};

struct mg_flash_driver_interface {
uint32_t version;
uint32_t (*init)(struct mg_flash_config *);
uint32_t (*erase)(struct mg_flash_config *, uint32_t start, uint32_t len, uint32_t key);
uint32_t (*program)(struct mg_flash_config *, uint32_t start, uint8_t *src, uint32_t len);
uint32_t (*verify_erase)(struct mg_flash_config *, uint32_t start, uint32_t len);
uint32_t (*verify_program)(struct mg_flash_config *, uint32_t start, uint32_t len, const uint8_t *expected, uint32_t *addr, uint32_t *failed);
uint32_t reserved1[12];
uint32_t (*read)(struct mg_flash_config *, uint32_t start, uint8_t *dest, uint32_t len);
uint32_t reserved2[4];
uint32_t (*deinit)(struct mg_flash_config *);
};
#define mg_flash_driver ((struct mg_flash_driver_interface*) (*((uint32_t*)0x1303fc00 + 4)))
#define MG_MCXN_FLASK_KEY (('k' << 24) | ('e' << 16) | ('f' << 8) | 'l')

static bool s_flash_irq_disabled;

// Flash phrase: 16 bytes; smallest portion that can be programmed in one operation.
// Flash page: 128 bytes; largest portion that can be programmed in one operation.
// Flash sector: 8 KB; smallest portion that can be erased in one operation.
// Flash API mg_flash_driver->program: "start" and "len" must be page-size aligned
MG_IRAM void *mg_flash_start(void) {
  return (void *) 0;
}
MG_IRAM size_t mg_flash_size(void) {
  return 2 * 1024 * 1024; // 2MB
}
MG_IRAM size_t mg_flash_sector_size(void) {
  return 8 * 1024;  // 8KB
}
MG_IRAM size_t mg_flash_write_align(void) {
  return 128; // use 128 instead of 16, using ROM API instead of FMU
}
MG_IRAM int mg_flash_bank(void) {
  return 0;
}

MG_IRAM static bool flash_sector_start(volatile uint32_t *dst) {
  char *base = (char *) mg_flash_start(), *end = base + mg_flash_size();
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % mg_flash_sector_size()) == 0;
}

MG_IRAM static bool flash_erase(struct mg_flash_config *config, void *addr) {
  if (flash_sector_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
    return false;
  }
  uint32_t dst = (uint32_t) addr - (uint32_t) mg_flash_start(); // future-proof
  uint32_t status = mg_flash_driver->erase(config, dst, mg_flash_sector_size(), MG_MCXN_FLASK_KEY);
  bool ok = status == 0;
  if (!ok) MG_ERROR(("Flash write error: %lu", status));
  MG_DEBUG(("Sector starting at %p erasure: %s", addr, ok ? "ok" : "fail"));
  return ok;
}

MG_IRAM bool mg_flash_erase(void *addr) {
  uint32_t status;
  struct mg_flash_config config;
  if ((status = mg_flash_driver->init(&config)) != 0) {
    MG_ERROR(("Flash driver init error: %lu", status));
    return false;
  }
  bool ok = flash_erase(&config, addr);
  mg_flash_driver->deinit(&config);
  return ok;
}

MG_IRAM bool mg_flash_swap_bank(size_t size) {
  // Swap partitions. Pray power does not go away
  size_t fs = mg_flash_size(), ss = mg_flash_sector_size();
  char *partition1 = mg_flash_start();
  char *partition2 = mg_flash_start() + fs / 2;
  size_t ofs, max = size == 0 ? fs / 2 - ss : size;
  char *tmpsector = malloc(ss);
  bool ramtmp = (tmpsector != NULL);
  if (!ramtmp) {
    MG_ERROR(("OOM, swapping in flash"));
    // We use the last sector of partition2 for OTA data/config storage
    // Therefore we can use last sector of partition1 for swapping
    tmpsector = partition1 + fs / 2 - ss;  // Last sector of partition1
  }
  MG_INFO(("Swapping partitions, size %u (%u sectors)", max, max / ss));
  MG_INFO(("Do NOT power off..."));
  mg_log_level = MG_LL_NONE;
  s_flash_irq_disabled = true;
  for (ofs = 0; ofs < max; ofs += ss) { // no stdlib calls here
    if (ramtmp) {
      for (size_t i = 0; i < ss; i++) tmpsector[i] = partition1[ofs + i];
    } else {
      mg_flash_write(tmpsector, partition1 + ofs, ss);
    }
    mg_flash_write(partition1 + ofs, partition2 + ofs, ss);
    mg_flash_write(partition2 + ofs, tmpsector, ss);
  }
  mg_device_reset();
  return true;
}

MG_IRAM bool mg_flash_write(void *addr, const void *buf, size_t len) {
  bool ok = false;
  uint32_t status;
  struct mg_flash_config config;
  if ((status = mg_flash_driver->init(&config)) != 0) {
    MG_ERROR(("Flash driver init error: %lu", status));
    return false;
  }
  if ((len % mg_flash_write_align()) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, mg_flash_write_align()));
    goto fwxit;
  }
  if (((size_t)addr - (size_t)mg_flash_start()) % mg_flash_write_align() != 0) {
    MG_ERROR(("%p is not on a page boundary", addr));
    goto fwxit;
  }

  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  ok = true;

  MG_DEBUG(("Writing flash @ %p, %lu bytes", addr, len));
  MG_ARM_DISABLE_IRQ();
  while (ok && src < end) {
    if (flash_sector_start(dst) && flash_erase(&config, dst) == false) {
      ok = false;
      break;
    }
    uint32_t dst_ofs = (uint32_t) dst - (uint32_t) mg_flash_start();
    // assume source is in RAM or in a different bank or read-while-write
    // TODO(scaprile): maybe optimize calls doing one per sector ?
    status = mg_flash_driver->program(&config, dst_ofs, (uint8_t *)src, mg_flash_write_align());
    src = (uint32_t *) ((char *) src + mg_flash_write_align());
    dst = (uint32_t *) ((char *) dst + mg_flash_write_align());
    if (status != 0) {
      MG_ERROR(("Flash write error: %lu", status));
      ok = false;
    }
  }
  if (!s_flash_irq_disabled) MG_ARM_ENABLE_IRQ();
  MG_DEBUG(("Flash write %lu bytes @ %p: %s.", len, dst, ok ? "ok" : "fail"));

fwxit:
  mg_flash_driver->deinit(&config);
  return ok;
}

MG_IRAM void mg_device_reset(void) {
  MG_DEBUG(("Resetting device..."));
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}

#endif
