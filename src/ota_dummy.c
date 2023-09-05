#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_NONE
static struct mg_ota_data s_od[2] = {
    {MG_OTA_MAGIC, (uint32_t) -1, (uint32_t) -1, (uint32_t) -1, (uint32_t) -1,
     (uint32_t) -1},
    {MG_OTA_MAGIC, (uint32_t) -1, (uint32_t) -1, (uint32_t) -1, (uint32_t) -1,
     (uint32_t) -1},
};
bool mg_ota_begin(size_t new_firmware_size) {
  MG_DEBUG(("Starting firmware update, size %lu", new_firmware_size));
  return true;
}
bool mg_ota_write(const void *buf, size_t len) {
  MG_DEBUG(("%p %lu", buf, len));
  return true;
}
bool mg_ota_end(void) {
  return true;
}
bool mg_ota_status(struct mg_ota_data od[2]) {
  od[0] = s_od[0];
  od[1] = s_od[1];
  return true;
}
bool mg_ota_commit(void) {
  s_od[0].golden = 0;
  return true;
}
bool mg_ota_rollback(void) {
  return true;
}
void mg_sys_reset(void) {
  MG_DEBUG(("Resetting device..."));
}
#endif
