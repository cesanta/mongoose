#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_FREERTOS

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_NXP_WIFI 1
#define MG_ENABLE_PACKED_FS 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0


#define HTTP_URL "http://0.0.0.0:80"
#define HTTPS_URL "https://0.0.0.0:443"
