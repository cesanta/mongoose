#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_NONE
bool mg_ota_begin(size_t new_firmware_size) {
  (void) new_firmware_size;
  return true;
}
bool mg_ota_write(const void *buf, size_t len) {
  (void) buf, (void) len;
  return true;
}
bool mg_ota_end(void) {
  return true;
}
#endif
