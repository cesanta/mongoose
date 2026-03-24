#pragma once

// Build environment. Possible options are at https://github.com/cesanta/mongoose/blob/master/src/arch.h
// #define MG_ARCH MG_ARCH_CUSTOM

// Choice of the network stack. By default, use an external BSD-compatible stack
// #define MG_ENABLE_FREERTOS_TCP 0  // Amazon FreeRTOS-TCP
// #define MG_ENABLE_LWIP 0          // lWIP
// #define MG_ENABLE_TCPIP 0         // Mongoose built-in
// #define MG_ENABLE_RL 0            // ARM MDK
// #define MG_ENABLE_POLL 0          // Only for BSD stack. Use poll()
// #define MG_ENABLE_EPOLL 0         // Only for Linux/BSD stack. Use epoll()
// #define MG_ENABLE_WINSOCK 1       // Only for Windows stack. Use Winsock
// #define MG_ENABLE_SOCKET !MG_ENABLE_TCPIP
// #define MG_SOCK_LISTEN_BACKLOG_SIZE 3  // Only for socket-based stacks

// Only for built-in network stack, MG_ENABLE_TCPIP == 1
// #define MG_ENABLE_TCPIP_DRIVER_INIT 1  // enabled built-in driver for
// #define MG_SET_MAC_ADDRESS(mac)        // set MAC address for built-in driver
// #define MG_SET_WIFI_CONFIG(data)
// #define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 0

// Choose a driver for built-in network stack, MG_ENABLE_TCPIP == 1
// #define MG_ENABLE_DRIVER_STM32H 0
// #define MG_ENABLE_DRIVER_STM32F 0
// #define MG_ENABLE_DRIVER_IMXRT 0
// #define MG_ENABLE_DRIVER_CMSIS 0
// #define MG_ENABLE_DRIVER_RA 0
// #define MG_ENABLE_DRIVER_RW612 0
// #define MG_ENABLE_DRIVER_SAME54 0
// #define MG_ENABLE_DRIVER_MCXN 0
// #define MG_ENABLE_DRIVER_TM4C 0
// #define MG_ENABLE_DRIVER_TMS570 0
// #define MG_ENABLE_DRIVER_XMC7 0
// #define MG_ENABLE_DRIVER_XMC 0

// Filesystem support
// #define MG_ENABLE_PACKED_FS 0  // Enable packed filesystem support
// #define MG_ENABLE_FATFS 0      // Enable FATFS support, mg_fs_packed
// #define MG_ENABLE_POSIX_FS 0   // Enable POSIX filesystem support, mg_fs_posix
// #define MG_PATH_MAX 128        // Maximum path length
// #define MG_DIRSEP '/'          // Directory separator

// Misc options
// #define MG_ENABLE_LOG 1           // Enable logging
// #define MG_TLS MG_TLS_NONE        // MG_TLS_NONE, MG_TLS_MBED, MG_TLS_OPENSSL, MG_TLS_CUSTOM
// #define MG_ENABLE_IPV6 0          // Enable IPv6 support
// #define MG_IPV6_V6ONLY 0          // IPv6 socket binds only to V6, not V4 address
// #define MG_ENABLE_CUSTOM_CALLOC 0 // Set to 1 to let user define mg_calloc() and mg_free()
// #define MG_ENABLE_CUSTOM_RANDOM 0 // Set to 1 to let user define mg_random()
// #define MG_ENABLE_CUSTOM_MILLIS 0 // Set to 1 to let user define mg_millis()
// #define MG_ENABLE_SSI 0           // Enable SSI support
// #define MG_ENABLE_DIRLIST 0       // Enable directory listing
// #define MG_IO_SIZE 512            // Granularity of the send/recv IO buffer growth
// #define MG_MAX_RECV_SIZE (3UL * 1024UL * 1024UL)  // Maximum recv IO buffer size
// #define MG_DATA_SIZE 32           // struct mg_connection :: data size
// #define MG_MAX_HTTP_HEADERS 30    // Maximum number of HTTP headers
// #define MG_HTTP_INDEX "index.html"  // Default HTTP index file
// #define MG_ENABLE_LINES 0         // Show unamalgamated line numbers
