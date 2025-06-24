#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_MCXN

// - Flash phrase: 16 bytes; smallest portion programmed in one operation.
// - Flash page: 128 bytes; largest portion programmed in one operation.
// - Flash sector: 8 KB; smallest portion that can be erased in one operation.
// - Flash API mg_flash_driver->program: "start" and "len" must be page-size
// aligned; to use 'phrase', FMU register access is needed. Using ROM

static bool mg_mcxn_write(void *, const void *, size_t);
static bool mg_mcxn_swap(void);

static struct mg_flash s_mg_flash_mcxn = {
    (void *) 0,  // Start, filled at init
    0,           // Size, filled at init
    0,           // Sector size, filled at init
    0,           // Align, filled at init
    mg_mcxn_write,
    mg_mcxn_swap,
};

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
  uint32_t (*erase)(struct mg_flash_config *, uint32_t start, uint32_t len,
                    uint32_t key);
  uint32_t (*program)(struct mg_flash_config *, uint32_t start, uint8_t *src,
                      uint32_t len);
  uint32_t (*verify_erase)(struct mg_flash_config *, uint32_t start,
                           uint32_t len);
  uint32_t (*verify_program)(struct mg_flash_config *, uint32_t start,
                             uint32_t len, const uint8_t *expected,
                             uint32_t *addr, uint32_t *failed);
  uint32_t reserved1[12];
  uint32_t (*read)(struct mg_flash_config *, uint32_t start, uint8_t *dest,
                   uint32_t len);
  uint32_t reserved2[4];
  uint32_t (*deinit)(struct mg_flash_config *);
};
#define mg_flash_driver \
  ((struct mg_flash_driver_interface *) (*((uint32_t *) 0x1303fc00 + 4)))
#define MG_MCXN_FLASK_KEY (('k' << 24) | ('e' << 16) | ('f' << 8) | 'l')

MG_IRAM static bool flash_sector_start(volatile uint32_t *dst) {
  char *base = (char *) s_mg_flash_mcxn.start,
       *end = base + s_mg_flash_mcxn.size;
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % s_mg_flash_mcxn.secsz) == 0;
}

MG_IRAM static bool flash_erase(struct mg_flash_config *config, void *addr) {
  if (flash_sector_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
    return false;
  }
  uint32_t dst =
      (uint32_t) addr - (uint32_t) s_mg_flash_mcxn.start;  // future-proof
  uint32_t status = mg_flash_driver->erase(config, dst, s_mg_flash_mcxn.secsz,
                                           MG_MCXN_FLASK_KEY);
  bool ok = (status == 0);
  if (!ok) MG_ERROR(("Flash write error: %lu", status));
  MG_DEBUG(("Sector starting at %p erasure: %s", addr, ok ? "ok" : "fail"));
  return ok;
}

#if 0
// read-while-write, no need to disable IRQs for standalone usage
MG_IRAM static bool mg_mcxn_erase(void *addr) {
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
#endif

MG_IRAM static bool mg_mcxn_swap(void) {
  // TODO(): no devices so far
  return true;
}

static bool s_flash_irq_disabled;

MG_IRAM static bool mg_mcxn_write(void *addr, const void *buf, size_t len) {
  bool ok = false;
  uint32_t status;
  struct mg_flash_config config;
  if ((status = mg_flash_driver->init(&config)) != 0) {
    MG_ERROR(("Flash driver init error: %lu", status));
    return false;
  }
  if ((len % s_mg_flash_mcxn.align) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, s_mg_flash_mcxn.align));
    goto fwxit;
  }
  if ((((size_t) addr - (size_t) s_mg_flash_mcxn.start) %
       s_mg_flash_mcxn.align) != 0) {
    MG_ERROR(("%p is not on a page boundary", addr));
    goto fwxit;
  }

  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  ok = true;

  MG_ARM_DISABLE_IRQ();
  while (ok && src < end) {
    if (flash_sector_start(dst) && flash_erase(&config, dst) == false) {
      ok = false;
      break;
    }
    uint32_t dst_ofs = (uint32_t) dst - (uint32_t) s_mg_flash_mcxn.start;
    // assume source is in RAM or in a different bank or read-while-write
    status = mg_flash_driver->program(&config, dst_ofs, (uint8_t *) src,
                                      s_mg_flash_mcxn.align);
    src = (uint32_t *) ((char *) src + s_mg_flash_mcxn.align);
    dst = (uint32_t *) ((char *) dst + s_mg_flash_mcxn.align);
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

// try to swap (honor dual image), otherwise just overwrite
MG_IRAM static void single_bank_swap(char *p1, char *p2, size_t s, size_t ss) {
  char *tmp = mg_calloc(1, ss);
  // no stdlib calls here
  for (size_t ofs = 0; ofs < s; ofs += ss) {
    if (tmp != NULL)
      for (size_t i = 0; i < ss; i++) tmp[i] = p1[ofs + i];
    mg_mcxn_write(p1 + ofs, p2 + ofs, ss);
    if (tmp != NULL) mg_mcxn_write(p2 + ofs, tmp, ss);
  }
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}

bool mg_ota_begin(size_t new_firmware_size) {
  uint32_t status;
  struct mg_flash_config config;
  if ((status = mg_flash_driver->init(&config)) != 0) {
    MG_ERROR(("Flash driver init error: %lu", status));
    return false;
  }
  s_mg_flash_mcxn.start = (void *) config.addr;
  s_mg_flash_mcxn.size = config.size;
  s_mg_flash_mcxn.secsz = config.sector_size;
  s_mg_flash_mcxn.align = config.page_size;
  mg_flash_driver->deinit(&config);
  MG_DEBUG(
      ("%lu-byte flash @%p, using %lu-byte sectors with %lu-byte-aligned pages",
       s_mg_flash_mcxn.size, s_mg_flash_mcxn.start, s_mg_flash_mcxn.secsz,
       s_mg_flash_mcxn.align));
  return mg_ota_flash_begin(new_firmware_size, &s_mg_flash_mcxn);
}

bool mg_ota_write(const void *buf, size_t len) {
  return mg_ota_flash_write(buf, len, &s_mg_flash_mcxn);
}

bool mg_ota_end(void) {
  if (mg_ota_flash_end(&s_mg_flash_mcxn)) {
    if (0) {  // is_dualbank()
      // TODO(): no devices so far
      *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
    } else {
      // Swap partitions. Pray power does not go away
      MG_INFO(("Swapping partitions, size %u (%u sectors)",
               s_mg_flash_mcxn.size,
               s_mg_flash_mcxn.size / s_mg_flash_mcxn.secsz));
      MG_INFO(("Do NOT power off..."));
      mg_log_level = MG_LL_NONE;
      s_flash_irq_disabled = true;
      // Runs in RAM, will reset when finished
      single_bank_swap(
          (char *) s_mg_flash_mcxn.start,
          (char *) s_mg_flash_mcxn.start + s_mg_flash_mcxn.size / 2,
          s_mg_flash_mcxn.size / 2, s_mg_flash_mcxn.secsz);
    }
  }
  return false;
}
#endif
