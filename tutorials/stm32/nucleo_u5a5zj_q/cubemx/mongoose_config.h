#pragma once
#define MG_ARCH MG_ARCH_CUBE
#define MG_ENABLE_DRIVER_ST67W6 1
extern void wifi_setconfig(void *data);
#define MG_SET_WIFI_CONFIG(data) wifi_setconfig(data)