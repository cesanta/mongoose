#pragma once

// Build options: https://mongoose.ws/docs/getting-started/build-options/
#define MG_ARCH MG_ARCH_PICOSDK
#define MG_OTA MG_OTA_PICOSDK
#define MG_IO_SIZE 2048

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_PICO_W 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0

// MAC address is read from OTP by the driver
// Set your Wi-Fi configuration using a custom function:
extern void wifi_setconfig(void *, void *);
#define MG_SET_WIFI_CONFIG(ifp, data) wifi_setconfig(ifp, data)
