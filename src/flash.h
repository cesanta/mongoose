#include "arch.h"
#include "ota.h"

#if MG_OTA != MG_OTA_NONE && MG_OTA != MG_OTA_CUSTOM

struct mg_flash {
  void *start;    // Address at which flash starts
  size_t size;    // Flash size
  size_t secsz;   // Sector size
  size_t align;   // Write alignment
  bool (*write_fn)(void *, const void *, size_t);  // Write function
  bool (*swap_fn)(void);                           // Swap partitions
};

bool mg_ota_flash_begin(size_t new_firmware_size, struct mg_flash *flash);
bool mg_ota_flash_write(const void *buf, size_t len, struct mg_flash *flash);
bool mg_ota_flash_end(struct mg_flash *flash);

#endif
