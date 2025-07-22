[#ftl]

#define MG_ARCH MG_ARCH_CUBE

#define MG_ENABLE_TCPIP 1

[#list SWIPdatas as SWIP]  
  [#if SWIP.defines??]
    [#list SWIP.defines as def]
#define ${def.name} [#if def.value == "true"] 1 [#elseif def.value == "false"] 0 [#else]${def.value}[/#if]

    [/#list]
  [/#if]
[/#list]

// Translate to Mongoose macros
#if MG_STMPACK_TLS == 0
#define MG_TLS MG_TLS_NONE
#elif MG_STMPACK_TLS == 1
#define MG_TLS MG_TLS_BUILTIN
#elif MG_STMPACK_TLS == 2
#define MG_TLS MG_TLS_MBEDTLS
#elif MG_STMPACK_TLS == 3
#define MG_TLS MG_TLS_WOLFSSL
#endif


// See https://mongoose.ws/documentation/#build-options
