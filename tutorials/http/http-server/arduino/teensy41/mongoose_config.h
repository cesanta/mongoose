#pragma once

#define MG_ARCH MG_ARCH_NEWLIB     // Use ARM toolchain
#define MG_ENABLE_TCPIP 1          // Enable built-in network stack
#define MG_ENABLE_DRIVER_IMXRT 1   // Enable RTxx driver
#define MG_ENABLE_CUSTOM_MILLIS 1  // Let user implement mg_millis()
#define MG_ENABLE_POSIX_FS 0       // Disable POSIX filesystem
#define MG_ENABLE_PACKED_FS 1      // Enable packed filesystem

#define HTTP_URL "http://0.0.0.0"
#define HTTPS_URL "https://0.0.0.0"
