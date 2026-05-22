#pragma once
#define MG_ARCH MG_ARCH_CUBE
#define MG_ENABLE_DRIVER_CYW_SDIO 1
extern void wifi_setconfig(void *data);
#define MG_SET_WIFI_CONFIG(ifp, data) wifi_setconfig(data)
