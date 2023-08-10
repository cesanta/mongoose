
// <<< Use Configuration Wizard in Context Menu >>>

// <o> System Architecture
//   <0=> Bare metal
//   <1=> FreeRTOS
//   <2=> CMSIS-RTOS v1
//   <3=> CMSIS-RTOS v2
// <i> Select either bare metal operation or a supported RTOS
// <i> FreeRTOS uses direct FreeRTOS calls
// <i> CMSIS-RTOS1 supports only Keil RTX through API v1
// <i> CMSIS-RTOS2 supports Keil RTX5 and FreeRTOS through API v2

#define MG_CMSISPACK_ARCH 2


// <h> Networking support
// <o> Networking stack
//   <0=> Built-in
//   <1=> lwIP
//   <2=> FreeRTOS-Plus-TCP
//   <3=> MDK (RL)
// <i> Select the networking stack to use with Mongoose Library
// <i> The built-in stack can run on bare metal or over any RTOS
// <i> "lwIP" requires using an RTOS and BSD socket mode
// <i> "MDK" requires using CMSIS-RTOS1 (RTX + RL) or CMSIS-RTOS2 (MDK Plus or Pro), and BSD socket mode
#define MG_CMSISPACK_NET 0

// <q> Use Mbed-TLS
// <i> Mongoose will use Mbed-TLS calls for TLS-related functionality
#define MG_ENABLE_MBEDTLS 0
// </h>


// <c1> Enable custom mg_millis()
// <i> Use a user-provided function to get uptime in milliseconds, otherwise Mongoose will default to using time(). Except for bare metal, Mongoose will use the time base for the configured architecture
//#define MG_ENABLE_CUSTOM_MILLIS 1
// </c>


// <c1> Enable custom mg_rand()
// <i> Use a user-provided function to generate random numbers, otherwise Mongoose will default to using rand()
#define MG_ENABLE_CUSTOM_RANDOM 1
// </c>


// <h> Filesystem support
// <q> Enable packed (embedded) filesystem
#define MG_ENABLE_PACKED_FS 1
// </h>


// <<< end of configuration section >>>

// Translate to Mongoose macros
#if MG_CMSISPACK_ARCH == 1
#undef MG_ARCH
#define MG_ARCH MG_ARCH_FREERTOS
#elif MG_CMSISPACK_ARCH == 2
#undef MG_ARCH
#define MG_ARCH MG_ARCH_CMSIS_RTOS1
#elif MG_CMSISPACK_ARCH == 3
#undef MG_ARCH
#define MG_ARCH MG_ARCH_CMSIS_RTOS2
#endif
#if MG_CMSISPACK_NET == 0
#define MG_ENABLE_TCPIP 1
#elif MG_CMSISPACK_NET == 1
#define MG_ENABLE_LWIP 1
#elif MG_CMSISPACK_NET == 2
#define MG_ENABLE_FREERTOS_TCP 1
#elif MG_CMSISPACK_NET == 3
#define MG_ENABLE_RL 1
#endif


// Add your customization below this comment
#define MG_ENABLE_DRIVER_STM32 1
