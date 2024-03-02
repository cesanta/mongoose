
#if defined(MG_ARCH)
#define MG_STMPACK_ARCH MG_ARCH
#undef MG_ARCH
#elif defined(__GNUC__)
#define MG_STMPACK_ARCH MG_ARCH_NEWLIB
#endif

#define MG_ENABLE_PACKED_FS  1

#define MG_ENABLE_CUSTOM_MILLIS  1

#define MG_ENABLE_MBEDTLS  0

#define MG_ARCH MG_STMPACK_ARCH

#define MG_STMPACK_NET 0

#define MG_ENABLE_CUSTOM_RANDOM  1

// Translate to Mongoose macros
#if MG_STMPACK_NET == 0
#define MG_ENABLE_TCPIP 1
#elif MG_STMPACK_NET == 1
#define MG_ENABLE_LWIP 1
#elif MG_STMPACK_NET == 2
#define MG_ENABLE_FREERTOS_TCP 1
#elif MG_STMPACK_NET == 3
#define MG_ENABLE_RL 1
#endif

#if MG_ENABLE_PACKED_FS
#define MG_ENABLE_POSIX_FS 0
#endif

// See https://mongoose.ws/documentation/#build-options
