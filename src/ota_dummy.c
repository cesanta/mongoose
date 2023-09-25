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
bool mg_ota_commit(void) {
  return true;
}
bool mg_ota_rollback(void) {
  return true;
}
int mg_ota_status(int fw) {
  (void) fw;
  return 0;
}
uint32_t mg_ota_crc32(int fw) {
  (void) fw;
  return 0;
}
uint32_t mg_ota_timestamp(int fw) {
  (void) fw;
  return 0;
}
size_t mg_ota_size(int fw) {
  (void) fw;
  return 0;
}
#endif
