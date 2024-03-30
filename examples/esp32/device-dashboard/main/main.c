// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "net.h"

#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!

void app_main(void) {
  // Setup wifi. This function is implemented in wifi.c
  // It blocks until connected to the configured WiFi network
  void wifi_init(const char *ssid, const char *pass);
  wifi_init(WIFI_SSID, WIFI_PASS);

  // Connected to WiFi, now start HTTP server
  struct mg_mgr mgr;
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", HTTP_URL));
#if MG_ENABLE_MBEDTLS
  MG_INFO(("Listening on     : %s", HTTPS_URL));
#endif

  web_init(&mgr);
  for (;;) mg_mgr_poll(&mgr, 1000);  // Infinite event loop
}

#if MG_DEVICE == MG_DEVICE_CUSTOM
void *mg_flash_start(void) {
  return NULL;
}
size_t mg_flash_size(void) {
  return 0;
}
size_t mg_flash_sector_size(void) {
  return 0;
}
size_t mg_flash_write_align(void) {
  return 0;
}
int mg_flash_bank(void) {
  return 0;
}
bool mg_flash_erase(void *location) {
  (void) location;
  return false;
}
bool mg_flash_swap_bank(void) {
  return true;
}
bool mg_flash_write(void *addr, const void *buf, size_t len) {
  (void) addr, (void) buf, (void) len;
  return false;
}
void mg_device_reset(void) {
  esp_restart();
}
#endif

#if MG_OTA == MG_OTA_CUSTOM
#include "esp_app_format.h"
#include "esp_ota_ops.h"

static size_t s_size = 0;           // Firmware size to flash. In-progress indicator
static bool rx_checked = false;     // Whether firmware being received has been checked as valid
const esp_partition_t * update_partition = NULL;    // The partition the update is being applied to
esp_ota_handle_t update_handle = 0; // Handle of the current update process

const esp_partition_t * get_partition_from_fw(int fw) {
  if (MG_FIRMWARE_CURRENT == fw) {
    return esp_ota_get_running_partition();
  }
  const esp_partition_t * p = esp_ota_get_last_invalid_partition();
  if (NULL == p) {
    p = esp_ota_get_next_update_partition(NULL);
  }
  return p;
}

// Returns true if the data buffer contains the header of a valid firmware
const size_t fw_size_required = sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t);
bool check_fw_header(const void* buf, size_t len) {
  // Ensure we have received enough data to do the check
  if (len < fw_size_required) {
    MG_ERROR(("Insufficient data to check firmware header - %d bytes, require %d bytes", len, fw_size_required));
    return false;
  }

  // Check the magic word of the received firmare
  esp_app_desc_t* rx_app_info = (esp_app_desc_t*)(((char*)buf) + sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t));
  if (ESP_APP_DESC_MAGIC_WORD != rx_app_info->magic_word) {
    MG_ERROR(("Invalid app - invalid magic word"));
    return false;
  }
  MG_INFO(("Received firmware: %s %s", rx_app_info->project_name, rx_app_info->version));

  // Get the name/version of the running firmware
  const esp_partition_t *current = esp_ota_get_running_partition();
  esp_app_desc_t curr_app_info;
  esp_err_t res = esp_ota_get_partition_description(current, &curr_app_info);
  if (ESP_OK != res) {
    MG_ERROR(("Unable to get name/version of running firmware"));
    return false;
  }
  MG_INFO(("Running firmware: %s %s", curr_app_info.project_name, curr_app_info.version));

  // Check the name of the new firmware is the same as the old firmware
  if (memcmp(curr_app_info.project_name, rx_app_info->project_name, sizeof(curr_app_info.project_name)) != 0) {
    MG_ERROR(("Firmware name is different. Wrong firmware uploaded! Current %s Uploaded %s", curr_app_info.project_name, rx_app_info->project_name));
    return false;
  }

  // Check the version of the new firmware is different from the old firmware
  if (memcmp(curr_app_info.version, rx_app_info->version, sizeof(curr_app_info.version)) == 0) {
    MG_ERROR(("Firmware version is the same. Wrong firmware uploaded! Current %s, Uploaded %s", curr_app_info.version, rx_app_info->version));
    //return false;
  }

  return true;
}

bool mg_ota_begin(size_t new_firmware_size) {
  rx_checked = false;
  if (s_size) {
    MG_ERROR(("OTA already in progress. Call mg_ota_end()"));
    return false;
  }
  int partition_size = mg_ota_size(MG_FIRMWARE_PREVIOUS);
  if (new_firmware_size > partition_size) {
    MG_ERROR(("Firmware %lu bytes, max %lu", new_firmware_size, partition_size));
    return false;
  }
  update_partition = esp_ota_get_next_update_partition(NULL);
  if (NULL == update_partition) {
    MG_ERROR(("esp_ota_get_next_update_partition returned NULL"));
    return false;
  }
  esp_err_t res = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
  if (res != ESP_OK) {
    esp_ota_abort(update_handle);
    MG_ERROR(("esp_ota_begin failed (%s)", esp_err_to_name(res)));
    return false;
  }
  s_size = new_firmware_size;
  MG_INFO(("Starting OTA, firmware size %lu", s_size));
  return true;
}

bool mg_ota_write(const void *buf, size_t len) {
  if (s_size == 0) {
    MG_ERROR(("OTA is not started, call mg_ota_begin()"));
    return false;
  }
  if (!rx_checked) {
    if (!check_fw_header(buf, len)) {
      return false;
    }
    rx_checked = true;
  }
  esp_err_t res = esp_ota_write(update_handle, buf, len);
  if (res != ESP_OK) {
    esp_ota_abort(update_handle);
    MG_ERROR(("esp_ota_write FAILED (%s)", esp_err_to_name(res)));
    return false;
  }
  return true;
}

bool mg_ota_end(void) {
  if (0 == s_size) {
    MG_INFO(("Finishing OTA: fail"));
    return false;
  }
  s_size = 0;
  esp_err_t err = esp_ota_end(update_handle);
  if (err != ESP_OK) {
    MG_ERROR(("esp_ota_end failed (%s)!", esp_err_to_name(err)));
    return false;
  }
  err = esp_ota_set_boot_partition(update_partition);
  if (err != ESP_OK) {
    MG_ERROR(("esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err)));
    return false;
  }
  return true;
}

bool mg_ota_commit(void) {
  esp_ota_mark_app_valid_cancel_rollback();
  return true;
}

bool mg_ota_rollback(void) {
  if(esp_ota_check_rollback_is_possible()) {
    esp_ota_mark_app_invalid_rollback_and_reboot();
    return true;
  }
  MG_ERROR(("Rollback NOT possible"));
  return false;
}

int mg_ota_status(int fw) {
  const esp_partition_t *p = get_partition_from_fw(fw);
  if (NULL == p) {
    return 0;
  }
  int status = MG_OTA_UNAVAILABLE;

  esp_ota_img_states_t img_state = ESP_OTA_IMG_UNDEFINED;
  esp_err_t res = esp_ota_get_state_partition(p, &img_state);
  if (ESP_OK == res) {
    if (ESP_OTA_IMG_VALID == img_state) {
      status = MG_OTA_COMMITTED;
    } else if (ESP_OTA_IMG_UNDEFINED == img_state) {
      status = MG_OTA_UNCOMMITTED;
    } else if (ESP_OTA_IMG_INVALID == img_state) {
      status = MG_OTA_UNAVAILABLE;
    } else if (ESP_OTA_IMG_ABORTED == img_state) {
      status = MG_OTA_UNAVAILABLE;
    } else if (ESP_OTA_IMG_NEW == img_state) {
      status = MG_OTA_FIRST_BOOT;
    } else if (ESP_OTA_IMG_PENDING_VERIFY == img_state) {
      status = MG_OTA_UNCOMMITTED;
    }
  }
  return status;
}

uint32_t mg_ota_crc32(int fw) {
  (void) fw;
  return 0;
}

uint32_t mg_ota_timestamp(int fw) {
  const esp_partition_t *p = get_partition_from_fw(fw);

  esp_app_desc_t app;
  esp_err_t res = esp_ota_get_partition_description(p, &app);
  if (ESP_OK != res)
    {
    return 0;
    }

  struct tm datetime = {};
  if (NULL == strptime(app.date, "%b %d %Y", &datetime))
    {
    return 0;
    }
  if (NULL == strptime(app.time, "%H:%M:%S", &datetime))
    {
    return 0;
    }
  return mktime(&datetime);
}

size_t mg_ota_size(int fw) {
  const esp_partition_t *p = get_partition_from_fw(fw);
  if (NULL == p) {
    return 0;
  }
  return p->size;
}
#endif
