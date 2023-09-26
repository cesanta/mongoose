#include "device.h"

#if MG_DEVICE == MG_DEVICE_NONE
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
}
#endif
