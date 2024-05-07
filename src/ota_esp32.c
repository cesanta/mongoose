#include "arch.h"

#if MG_ARCH == MG_ARCH_ESP32 && MG_OTA == MG_OTA_ESP32

static const esp_partition_t *s_ota_update_partition;
static esp_ota_handle_t s_ota_update_handle;
static bool s_ota_success;

// Those empty macros do nothing, but mark places in the code which could
// potentially trigger a watchdog reboot due to the log flash erase operation
#define disable_wdt()
#define enable_wdt()

bool mg_ota_begin(size_t new_firmware_size) {
  if (s_ota_update_partition != NULL) {
    MG_ERROR(("Update in progress. Call mg_ota_end() ?"));
    return false;
  } else {
    s_ota_success = false;
    disable_wdt();
    s_ota_update_partition = esp_ota_get_next_update_partition(NULL);
    esp_err_t err = esp_ota_begin(s_ota_update_partition, new_firmware_size,
                                  &s_ota_update_handle);
    enable_wdt();
    MG_DEBUG(("esp_ota_begin(): %d", err));
    s_ota_success = (err == ESP_OK);
  }
  return s_ota_success;
}

bool mg_ota_write(const void *buf, size_t len) {
  disable_wdt();
  esp_err_t err = esp_ota_write(s_ota_update_handle, buf, len);
  enable_wdt();
  MG_INFO(("esp_ota_write(): %d", err));
  s_ota_success = err == ESP_OK;
  return s_ota_success;
}

bool mg_ota_end(void) {
  esp_err_t err = esp_ota_end(s_ota_update_handle);
  MG_DEBUG(("esp_ota_end(%p): %d", s_ota_update_handle, err));
  if (s_ota_success && err == ESP_OK) {
    err = esp_ota_set_boot_partition(s_ota_update_partition);
    s_ota_success = (err == ESP_OK);
  }
  MG_DEBUG(("Finished ESP32 OTA, success: %d", s_ota_success));
  s_ota_update_partition = NULL;
  return s_ota_success;
}

#endif
