#include "health.h"
#include "health_cortex.h"

struct mg_health mg_health_record;

void mg_health_init(void) {
  static const char magic[] = MG_HEALTH_MAGIC;
  memset(&mg_health_record, 0, sizeof(mg_health_record));
  memcpy(mg_health_record.magic, magic, sizeof(mg_health_record.magic));
#if MG_HEALTH == MG_HEALTH_CORTEX
  mg_health_cortex_init(&mg_health_cortex);
  mg_health_record.get_blob = mg_health_cortex_get_blob;
  mg_health_record.fn_data = &mg_health_cortex;
#endif
}

struct mg_str mg_health_get_blob(void) {
  return !mg_health_valid() || mg_health_record.get_blob == NULL
             ? mg_str("")
             : mg_health_record.get_blob(mg_health_record.fn_data);
}
