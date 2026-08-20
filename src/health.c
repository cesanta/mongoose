#include "health.h"

// The one health record. Lives in RAM that survives a warm reset, see
// MG_HEALTH_RAM and the .mg_health region in the linker script
struct mg_health mg_health_record MG_HEALTH_RAM;
