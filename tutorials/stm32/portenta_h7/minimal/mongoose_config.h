#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_ARMGCC
#define MG_TLS MG_TLS_BUILTIN

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_DRIVER_CYW_SDIO 1

// MAC address is read from OTP by the driver

// Set your Wi-Fi configuration using a custom function:
extern void wifi_setconfig(void *data);
#define MG_SET_WIFI_CONFIG(data) wifi_setconfig(data)
