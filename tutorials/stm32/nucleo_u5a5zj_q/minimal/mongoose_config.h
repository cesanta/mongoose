#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_ARMGCC

#define MG_ENABLE_TCPIP 1
#define MG_TLS MG_TLS_BUILTIN
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_DRIVER_ST67W6 1

// Set your Wi-Fi configuration using a custom function:
extern void wifi_setconfig(void *data);
#define MG_SET_WIFI_CONFIG(data) wifi_setconfig(data)
