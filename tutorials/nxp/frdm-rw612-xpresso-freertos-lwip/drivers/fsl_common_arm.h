/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_COMMON_ARM_H_
#define FSL_COMMON_ARM_H_

/*
 * For CMSIS pack RTE.
 * CMSIS pack RTE generates "RTC_Components.h" which contains the statements
 * of the related <RTE_Components_h> element for all selected software components.
 */
#ifdef _RTE_
#include "RTE_Components.h"
#endif

/*!
 * @addtogroup ksdk_common
 * @{
 */

/*! @name Atomic modification
 *
 * These macros are used for atomic access, such as read-modify-write
 * to the peripheral registers.
 *
 * Take @ref SDK_ATOMIC_LOCAL_CLEAR_AND_SET as an example: the parameter @c addr
 * means the address of the peripheral register or variable you want to modify
 * atomically, the parameter @c clearBits is the bits to clear, the parameter
 * @c setBits it the bits to set.
 * For example, to set a 32-bit register bit1:bit0 to 0b10, use like this:
 *
 * @code
   volatile uint32_t * reg = (volatile uint32_t *)REG_ADDR;

   SDK_ATOMIC_LOCAL_CLEAR_AND_SET(reg, 0x03, 0x02);
   @endcode
 *
 * In this example, the register bit1:bit0 are cleared and bit1 is set, as a result,
 * register bit1:bit0 = 0b10.
 *
 * @note For the platforms don't support exclusive load and store, these macros
 * disable the global interrupt to pretect the modification.
 *
 * @note These macros only guarantee the local processor atomic operations. For
 * the multi-processor devices, use hardware semaphore such as SEMA42 to
 * guarantee exclusive access if necessary.
 *
 * @{
 */

/*!
 * @def SDK_ATOMIC_LOCAL_ADD(addr, val)
 * Add value \a val from the variable at address \a address.
 *
 * @def SDK_ATOMIC_LOCAL_SUB(addr, val)
 * Subtract value \a val to the variable at address \a address.
 *
 * @def SDK_ATOMIC_LOCAL_SET(addr, bits)
 * Set the bits specifiled by \a bits to the variable at address \a address.
 *
 * @def SDK_ATOMIC_LOCAL_CLEAR(addr, bits)
 * Clear the bits specifiled by \a bits to the variable at address \a address.
 *
 * @def SDK_ATOMIC_LOCAL_TOGGLE(addr, bits)
 * Toggle the bits specifiled by \a bits to the variable at address \a address.
 *
 * @def SDK_ATOMIC_LOCAL_CLEAR_AND_SET(addr, clearBits, setBits)
 * For the variable at address \a address, clear the bits specifiled by \a clearBits
 * and set the bits specifiled by \a setBits.

 * @def SDK_ATOMIC_LOCAL_COMPARE_AND_SET(addr, expected, newValue)
 * For the variable at address \a address, check whether the value equal to \a expected. If value same as \a expected
 * then update \a newValue to address and return \b true , else return \b false .
 *
 * @def SDK_ATOMIC_LOCAL_TEST_AND_SET(addr, newValue)
 * For the variable at address \a address, set as \a newValue value and return old value.
 */

/* clang-format off */
#if ((defined(__ARM_ARCH_7M__     ) && (__ARM_ARCH_7M__      == 1)) || \
     (defined(__ARM_ARCH_7EM__    ) && (__ARM_ARCH_7EM__     == 1)) || \
     (defined(__ARM_ARCH_8M_MAIN__) && (__ARM_ARCH_8M_MAIN__ == 1)) || \
     (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ == 1)))
/* clang-format on */

/* If the LDREX and STREX are supported, use them. */
#define _SDK_ATOMIC_LOCAL_OPS_1BYTE(addr, val, ops) \
    do                                              \
    {                                               \
        (val) = __LDREXB(addr);                     \
        (ops);                                      \
    } while (0UL != __STREXB((val), (addr)))

#define _SDK_ATOMIC_LOCAL_OPS_2BYTE(addr, val, ops) \
    do                                              \
    {                                               \
        (val) = __LDREXH(addr);                     \
        (ops);                                      \
    } while (0UL != __STREXH((val), (addr)))

#define _SDK_ATOMIC_LOCAL_OPS_4BYTE(addr, val, ops) \
    do                                              \
    {                                               \
        (val) = __LDREXW(addr);                     \
        (ops);                                      \
    } while (0UL != __STREXW((val), (addr)))

static inline void _SDK_AtomicLocalAdd1Byte(volatile uint8_t *addr, uint8_t val)
{
    uint8_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_1BYTE(addr, s_val, s_val += val);
}

static inline void _SDK_AtomicLocalAdd2Byte(volatile uint16_t *addr, uint16_t val)
{
    uint16_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_2BYTE(addr, s_val, s_val += val);
}

static inline void _SDK_AtomicLocalAdd4Byte(volatile uint32_t *addr, uint32_t val)
{
    uint32_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_4BYTE(addr, s_val, s_val += val);
}

static inline void _SDK_AtomicLocalSub1Byte(volatile uint8_t *addr, uint8_t val)
{
    uint8_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_1BYTE(addr, s_val, s_val -= val);
}

static inline void _SDK_AtomicLocalSub2Byte(volatile uint16_t *addr, uint16_t val)
{
    uint16_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_2BYTE(addr, s_val, s_val -= val);
}

static inline void _SDK_AtomicLocalSub4Byte(volatile uint32_t *addr, uint32_t val)
{
    uint32_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_4BYTE(addr, s_val, s_val -= val);
}

static inline void _SDK_AtomicLocalSet1Byte(volatile uint8_t *addr, uint8_t bits)
{
    uint8_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_1BYTE(addr, s_val, s_val |= bits);
}

static inline void _SDK_AtomicLocalSet2Byte(volatile uint16_t *addr, uint16_t bits)
{
    uint16_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_2BYTE(addr, s_val, s_val |= bits);
}

static inline void _SDK_AtomicLocalSet4Byte(volatile uint32_t *addr, uint32_t bits)
{
    uint32_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_4BYTE(addr, s_val, s_val |= bits);
}

static inline void _SDK_AtomicLocalClear1Byte(volatile uint8_t *addr, uint8_t bits)
{
    uint8_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_1BYTE(addr, s_val, s_val &= ~bits);
}

static inline void _SDK_AtomicLocalClear2Byte(volatile uint16_t *addr, uint16_t bits)
{
    uint16_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_2BYTE(addr, s_val, s_val &= ~bits);
}

static inline void _SDK_AtomicLocalClear4Byte(volatile uint32_t *addr, uint32_t bits)
{
    uint32_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_4BYTE(addr, s_val, s_val &= ~bits);
}

static inline void _SDK_AtomicLocalToggle1Byte(volatile uint8_t *addr, uint8_t bits)
{
    uint8_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_1BYTE(addr, s_val, s_val ^= bits);
}

static inline void _SDK_AtomicLocalToggle2Byte(volatile uint16_t *addr, uint16_t bits)
{
    uint16_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_2BYTE(addr, s_val, s_val ^= bits);
}

static inline void _SDK_AtomicLocalToggle4Byte(volatile uint32_t *addr, uint32_t bits)
{
    uint32_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_4BYTE(addr, s_val, s_val ^= bits);
}

static inline void _SDK_AtomicLocalClearAndSet1Byte(volatile uint8_t *addr, uint8_t clearBits, uint8_t setBits)
{
    uint8_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_1BYTE(addr, s_val, s_val = (s_val & ~clearBits) | setBits);
}

static inline void _SDK_AtomicLocalClearAndSet2Byte(volatile uint16_t *addr, uint16_t clearBits, uint16_t setBits)
{
    uint16_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_2BYTE(addr, s_val, s_val = (s_val & ~clearBits) | setBits);
}

static inline void _SDK_AtomicLocalClearAndSet4Byte(volatile uint32_t *addr, uint32_t clearBits, uint32_t setBits)
{
    uint32_t s_val;

    _SDK_ATOMIC_LOCAL_OPS_4BYTE(addr, s_val, s_val = (s_val & ~clearBits) | setBits);
}

#define SDK_ATOMIC_LOCAL_ADD(addr, val)                                                                                        \
    ((1UL == sizeof(*(addr))) ?                                                                                                \
         _SDK_AtomicLocalAdd1Byte((volatile uint8_t *)(volatile void *)(addr), (uint8_t)(val)) :                               \
         ((2UL == sizeof(*(addr))) ? _SDK_AtomicLocalAdd2Byte((volatile uint16_t *)(volatile void *)(addr), (uint16_t)(val)) : \
                                     _SDK_AtomicLocalAdd4Byte((volatile uint32_t *)(volatile void *)(addr), (uint32_t)(val))))

#define SDK_ATOMIC_LOCAL_SUB(addr, val)                                                                                        \
    ((1UL == sizeof(*(addr))) ?                                                                                                \
         _SDK_AtomicLocalSub1Byte((volatile uint8_t *)(volatile void *)(addr), (uint8_t)(val)) :                               \
         ((2UL == sizeof(*(addr))) ? _SDK_AtomicLocalSub2Byte((volatile uint16_t *)(volatile void *)(addr), (uint16_t)(val)) : \
                                     _SDK_AtomicLocalSub4Byte((volatile uint32_t *)(volatile void *)(addr), (uint32_t)(val))))

#define SDK_ATOMIC_LOCAL_SET(addr, bits)                                                                                        \
    ((1UL == sizeof(*(addr))) ?                                                                                                 \
         _SDK_AtomicLocalSet1Byte((volatile uint8_t *)(volatile void *)(addr), (uint8_t)(bits)) :                               \
         ((2UL == sizeof(*(addr))) ? _SDK_AtomicLocalSet2Byte((volatile uint16_t *)(volatile void *)(addr), (uint16_t)(bits)) : \
                                     _SDK_AtomicLocalSet4Byte((volatile uint32_t *)(volatile void *)(addr), (uint32_t)(bits))))

#define SDK_ATOMIC_LOCAL_CLEAR(addr, bits)                                                                 \
    ((1UL == sizeof(*(addr))) ?                                                                            \
         _SDK_AtomicLocalClear1Byte((volatile uint8_t *)(volatile void *)(addr), (uint8_t)(bits)) :        \
         ((2UL == sizeof(*(addr))) ?                                                                       \
              _SDK_AtomicLocalClear2Byte((volatile uint16_t *)(volatile void *)(addr), (uint16_t)(bits)) : \
              _SDK_AtomicLocalClear4Byte((volatile uint32_t *)(volatile void *)(addr), (uint32_t)(bits))))

#define SDK_ATOMIC_LOCAL_TOGGLE(addr, bits)                                                                 \
    ((1UL == sizeof(*(addr))) ?                                                                             \
         _SDK_AtomicLocalToggle1Byte((volatile uint8_t *)(volatile void *)(addr), (uint8_t)(bits)) :        \
         ((2UL == sizeof(*(addr))) ?                                                                        \
              _SDK_AtomicLocalToggle2Byte((volatile uint16_t *)(volatile void *)(addr), (uint16_t)(bits)) : \
              _SDK_AtomicLocalToggle4Byte((volatile uint32_t *)(volatile void *)(addr), (uint32_t)(bits))))

#define SDK_ATOMIC_LOCAL_CLEAR_AND_SET(addr, clearBits, setBits)                                                                           \
    ((1UL == sizeof(*(addr))) ?                                                                                                            \
         _SDK_AtomicLocalClearAndSet1Byte((volatile uint8_t *)(volatile void *)(addr), (uint8_t)(clearBits), (uint8_t)(setBits)) :         \
         ((2UL == sizeof(*(addr))) ?                                                                                                       \
              _SDK_AtomicLocalClearAndSet2Byte((volatile uint16_t *)(volatile void *)(addr), (uint16_t)(clearBits), (uint16_t)(setBits)) : \
              _SDK_AtomicLocalClearAndSet4Byte((volatile uint32_t *)(volatile void *)(addr), (uint32_t)(clearBits), (uint32_t)(setBits))))

#define SDK_ATOMIC_LOCAL_COMPARE_AND_SET(addr, expected, newValue)   \
    ((1UL == sizeof(*(addr))) ?                                                                                                            \
         _SDK_AtomicLocalCompareAndSet1Byte((volatile uint8_t *)(volatile void *)(addr), (uint8_t)(expected), (uint8_t)(newValue)) :         \
         ((2UL == sizeof(*(addr))) ?                                                                                                       \
              _SDK_AtomicLocalCompareAndSet2Byte((volatile uint16_t *)(volatile void *)(addr), (uint16_t)(expected), (uint16_t)(newValue)) : \
              _SDK_AtomicLocalCompareAndSet4Byte((volatile uint32_t *)(volatile void *)(addr), (uint32_t)(expected), (uint32_t)(newValue))))

#define SDK_ATOMIC_LOCAL_TEST_AND_SET(addr, newValue)                 \
    ((1UL == sizeof(*(addr))) ?                                                                                                            \
         _SDK_AtomicLocalTestAndSet1Byte((volatile uint8_t *)(volatile void *)(addr), (uint8_t)(newValue)) :         \
         ((2UL == sizeof(*(addr))) ?                                                                                                       \
              _SDK_AtomicLocalTestAndSet2Byte((volatile uint16_t *)(volatile void *)(addr), (uint16_t)(newValue)) : \
              _SDK_AtomicLocalTestAndSet4Byte((volatile uint32_t *)(volatile void *)(addr), (uint32_t)(newValue))))
#else

#define SDK_ATOMIC_LOCAL_ADD(addr, val)      \
    do                                       \
    {                                        \
        uint32_t s_atomicOldInt;             \
        s_atomicOldInt = DisableGlobalIRQ(); \
        *(addr) += (val);                    \
        EnableGlobalIRQ(s_atomicOldInt);     \
    } while (false)

#define SDK_ATOMIC_LOCAL_SUB(addr, val)      \
    do                                       \
    {                                        \
        uint32_t s_atomicOldInt;             \
        s_atomicOldInt = DisableGlobalIRQ(); \
        *(addr) -= (val);                    \
        EnableGlobalIRQ(s_atomicOldInt);     \
    } while (false)

#define SDK_ATOMIC_LOCAL_SET(addr, bits)     \
    do                                       \
    {                                        \
        uint32_t s_atomicOldInt;             \
        s_atomicOldInt = DisableGlobalIRQ(); \
        *(addr) |= (bits);                   \
        EnableGlobalIRQ(s_atomicOldInt);     \
    } while (false)

#define SDK_ATOMIC_LOCAL_CLEAR(addr, bits)   \
    do                                       \
    {                                        \
        uint32_t s_atomicOldInt;             \
        s_atomicOldInt = DisableGlobalIRQ(); \
        *(addr) &= ~(bits);                  \
        EnableGlobalIRQ(s_atomicOldInt);     \
    } while (false)

#define SDK_ATOMIC_LOCAL_TOGGLE(addr, bits)  \
    do                                       \
    {                                        \
        uint32_t s_atomicOldInt;             \
        s_atomicOldInt = DisableGlobalIRQ(); \
        *(addr) ^= (bits);                   \
        EnableGlobalIRQ(s_atomicOldInt);     \
    } while (false)

#define SDK_ATOMIC_LOCAL_CLEAR_AND_SET(addr, clearBits, setBits) \
    do                                                           \
    {                                                            \
        uint32_t s_atomicOldInt;                                 \
        s_atomicOldInt = DisableGlobalIRQ();                     \
        *(addr)        = (*(addr) & ~(clearBits)) | (setBits);   \
        EnableGlobalIRQ(s_atomicOldInt);                         \
    } while (false)

#define SDK_ATOMIC_LOCAL_COMPARE_AND_SET(addr, expected, newValue) \
    _SDK_AtomicLocalCompareAndSet((uint32_t *)addr, (uint32_t)expected, (uint32_t)newValue)

#define SDK_ATOMIC_LOCAL_TEST_AND_SET(addr, newValue)     \
    _SDK_AtomicTestAndSet((uint32_t *)addr, (uint32_t)newValue)

#endif
/*! @} */

/*! @name Timer utilities */
/*! @{ */
/*! Macro to convert a microsecond period to raw count value */
#define USEC_TO_COUNT(us, clockFreqInHz) (uint64_t)(((uint64_t)(us) * (clockFreqInHz)) / 1000000U)
/*! Macro to convert a raw count value to microsecond */
#define COUNT_TO_USEC(count, clockFreqInHz) (uint64_t)((uint64_t)(count)*1000000U / (clockFreqInHz))

/*! Macro to convert a millisecond period to raw count value */
#define MSEC_TO_COUNT(ms, clockFreqInHz) (uint64_t)((uint64_t)(ms) * (clockFreqInHz) / 1000U)
/*! Macro to convert a raw count value to millisecond */
#define COUNT_TO_MSEC(count, clockFreqInHz) (uint64_t)((uint64_t)(count)*1000U / (clockFreqInHz))
/*! @} */

/*! @name ISR exit barrier
 * @{
 *
 * ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
 * exception return operation might vector to incorrect interrupt.
 * For Cortex-M7, if core speed much faster than peripheral register write speed,
 * the peripheral interrupt flags may be still set after exiting ISR, this results to
 * the same error similar with errata 83869.
 */
#if (defined __CORTEX_M) && ((__CORTEX_M == 4U) || (__CORTEX_M == 7U))
#define SDK_ISR_EXIT_BARRIER __DSB()
#else
#define SDK_ISR_EXIT_BARRIER
#endif

/*! @} */

/*! @name Alignment variable definition macros */
/*! @{ */
#if (defined(__ICCARM__))
/*
 * Workaround to disable MISRA C message suppress warnings for IAR compiler.
 * http:/ /supp.iar.com/Support/?note=24725
 */
_Pragma("diag_suppress=Pm120")
#define SDK_PRAGMA(x) _Pragma(#x)
    _Pragma("diag_error=Pm120")
/*! Macro to define a variable with alignbytes alignment */
#define SDK_ALIGN(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
/*! Macro to define a variable with alignbytes alignment */
#define SDK_ALIGN(var, alignbytes) __attribute__((aligned(alignbytes))) var
#elif defined(__GNUC__) || defined(DOXYGEN_OUTPUT)
/*! Macro to define a variable with alignbytes alignment */
#define SDK_ALIGN(var, alignbytes) var __attribute__((aligned(alignbytes)))
#else
#error Toolchain not supported
#endif

/*! Macro to define a variable with L1 d-cache line size alignment */
#if defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define SDK_L1DCACHE_ALIGN(var) SDK_ALIGN(var, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#endif
/*! Macro to define a variable with L2 cache line size alignment */
#if defined(FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
#define SDK_L2CACHE_ALIGN(var) SDK_ALIGN(var, FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
#endif

/*! Macro to change a value to a given size aligned value */
#define SDK_SIZEALIGN(var, alignbytes) \
    ((unsigned int)((var) + ((alignbytes)-1U)) & (unsigned int)(~(unsigned int)((alignbytes)-1U)))
/*! @} */

/*!
 * @name Non-cacheable region definition macros
 *
 * For initialized non-zero non-cacheable variables, please use "AT_NONCACHEABLE_SECTION_INIT(var) ={xx};" or
 * "AT_NONCACHEABLE_SECTION_ALIGN_INIT(var) ={xx};" in your projects to define them. For zero-inited non-cacheable
 * variables, please use "AT_NONCACHEABLE_SECTION(var);" or "AT_NONCACHEABLE_SECTION_ALIGN(var);" to define them,
 * these zero-inited variables will be initialized to zero in system startup.
 *
 * @note For GCC, when the non-cacheable section is required, please define "__STARTUP_INITIALIZE_NONCACHEDATA"
 * in your projects to make sure the non-cacheable section variables will be initialized in system startup.
 *
 * @{
 */

/*!
 * @def AT_NONCACHEABLE_SECTION(var)
 * Define a variable \a var, and place it in non-cacheable section.
 *
 * @def AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes)
 * Define a variable \a var, and place it in non-cacheable section, the start address
 * of the variable is aligned to \a alignbytes.
 *
 * @def AT_NONCACHEABLE_SECTION_INIT(var)
 * Define a variable \a var with initial value, and place it in non-cacheable section.
 *
 * @def AT_NONCACHEABLE_SECTION_ALIGN_INIT(var, alignbytes)
 * Define a variable \a var with initial value, and place it in non-cacheable section,
 * the start address of the variable is aligned to \a alignbytes.
 */

#if ((!(defined(FSL_FEATURE_HAS_NO_NONCACHEABLE_SECTION) && FSL_FEATURE_HAS_NO_NONCACHEABLE_SECTION)) && \
     defined(FSL_FEATURE_L1ICACHE_LINESIZE_BYTE))

#if (defined(__ICCARM__))
#define AT_NONCACHEABLE_SECTION(var)                   var @"NonCacheable"
#define AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"NonCacheable"
#define AT_NONCACHEABLE_SECTION_INIT(var)              var @"NonCacheable.init"
#define AT_NONCACHEABLE_SECTION_ALIGN_INIT(var, alignbytes) \
    SDK_PRAGMA(data_alignment = alignbytes) var @"NonCacheable.init"

#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
#define AT_NONCACHEABLE_SECTION_INIT(var) __attribute__((section("NonCacheable.init"))) var
#define AT_NONCACHEABLE_SECTION_ALIGN_INIT(var, alignbytes) \
    __attribute__((section("NonCacheable.init"))) __attribute__((aligned(alignbytes))) var
#if (defined(__CC_ARM))
#define AT_NONCACHEABLE_SECTION(var) __attribute__((section("NonCacheable"), zero_init)) var
#define AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section("NonCacheable"), zero_init)) __attribute__((aligned(alignbytes))) var
#else
#define AT_NONCACHEABLE_SECTION(var) __attribute__((section(".bss.NonCacheable"))) var
#define AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section(".bss.NonCacheable"))) __attribute__((aligned(alignbytes))) var
#endif

#elif (defined(__GNUC__)) || defined(DOXYGEN_OUTPUT)
/* For GCC, when the non-cacheable section is required, please define "__STARTUP_INITIALIZE_NONCACHEDATA"
 * in your projects to make sure the non-cacheable section variables will be initialized in system startup.
 */
#define AT_NONCACHEABLE_SECTION_INIT(var) __attribute__((section("NonCacheable.init"))) var
#define AT_NONCACHEABLE_SECTION_ALIGN_INIT(var, alignbytes) \
    __attribute__((section("NonCacheable.init"))) var __attribute__((aligned(alignbytes)))
#define AT_NONCACHEABLE_SECTION(var) __attribute__((section("NonCacheable,\"aw\",%nobits @"))) var
#define AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section("NonCacheable,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))
#else
#error Toolchain not supported.
#endif

#else

#define AT_NONCACHEABLE_SECTION(var)                        var
#define AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes)      SDK_ALIGN(var, alignbytes)
#define AT_NONCACHEABLE_SECTION_INIT(var)                   var
#define AT_NONCACHEABLE_SECTION_ALIGN_INIT(var, alignbytes) SDK_ALIGN(var, alignbytes)

#endif

/*! @} */

/*!
 * @name Cache line region definition macros
 *
 * For initialized non-zero cache line variables, please use "AT_CACHE_LINE_SECTION_INIT(var) ={xx};"
 * For zero-inited cache line variables, please use "AT_CACHE_LINE_SECTION(var);"
 *
 * @note This section is applicable to cached memory only, say external sdram, cached ocram, etc.
 *       Please avoid to use this section for none-cached memory, say TCM.
 *       So only those targets, which utilize the cached memory, say flexspi_nor_sdram_debug, support 
 *       this kind of section.
 * @{
 */

/*!
 * @def AT_CACHE_LINE_SECTION(var)
 * Define a variable \a var, which is cache line size aligned and be placed in CacheLineData section.
 *
 * @def AT_CACHE_LINE_SECTION_INIT(var)
 * Define a variable \a var with initial value, which is cache line size aligned and be placed in CacheLineData.init section.
 */

#if defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define CACHE_LINE_DATA SDK_L1DCACHE_ALIGN
#elif defined(FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
#define CACHE_LINE_DATA SDK_L2CACHE_ALIGN
#endif

#if (defined(__ICCARM__))
#define AT_CACHE_LINE_SECTION(var)                   CACHE_LINE_DATA(var) @"CacheLineData"
#define AT_CACHE_LINE_SECTION_INIT(var)              CACHE_LINE_DATA(var) @"CacheLineData.init"

#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
#if (defined(__CC_ARM))
#define AT_CACHE_LINE_SECTION(var)                   __attribute__((section("CacheLineData"), zero_init)) CACHE_LINE_DATA(var)
#else
#define AT_CACHE_LINE_SECTION(var)                   __attribute__((section("CacheLineData"))) CACHE_LINE_DATA(var)
#endif
#define AT_CACHE_LINE_SECTION_INIT(var)              __attribute__((section("CacheLineData.init"))) CACHE_LINE_DATA(var)

#elif (defined(__GNUC__)) || defined(DOXYGEN_OUTPUT)
#define AT_CACHE_LINE_SECTION(var)                   __attribute__((section("CacheLineData,\"aw\",%nobits @"))) CACHE_LINE_DATA(var)
#define AT_CACHE_LINE_SECTION_INIT(var)              __attribute__((section("CacheLineData.init"))) CACHE_LINE_DATA(var)

#else
#error Toolchain not supported.
#endif

/*! @} */

/*!
 * @name Time sensitive region
 * @{
 */

/*!
 * @def AT_QUICKACCESS_SECTION_CODE(func)
 * Place function in a section which can be accessed quickly by core.
 *
 * @def AT_QUICKACCESS_SECTION_DATA(var)
 * Place data in a section which can be accessed quickly by core.
 *
 * @def AT_QUICKACCESS_SECTION_DATA_ALIGN(var, alignbytes)
 * Place data in a section which can be accessed quickly by core, and the variable
 * address is set to align with \a alignbytes.
 */
#if (defined(__ICCARM__))
#define AT_QUICKACCESS_SECTION_CODE(func) func @"CodeQuickAccess"
#define AT_QUICKACCESS_SECTION_DATA(var)  var @"DataQuickAccess"
#define AT_QUICKACCESS_SECTION_DATA_ALIGN(var, alignbytes) \
    SDK_PRAGMA(data_alignment = alignbytes) var @"DataQuickAccess"
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
#define AT_QUICKACCESS_SECTION_CODE(func) __attribute__((section("CodeQuickAccess"), __noinline__)) func
#define AT_QUICKACCESS_SECTION_DATA(var)  __attribute__((section("DataQuickAccess"))) var
#define AT_QUICKACCESS_SECTION_DATA_ALIGN(var, alignbytes) \
    __attribute__((section("DataQuickAccess"))) __attribute__((aligned(alignbytes))) var
#elif (defined(__GNUC__)) || defined(DOXYGEN_OUTPUT)
#define AT_QUICKACCESS_SECTION_CODE(func) __attribute__((section("CodeQuickAccess"), __noinline__)) func
#define AT_QUICKACCESS_SECTION_DATA(var)  __attribute__((section("DataQuickAccess"))) var
#define AT_QUICKACCESS_SECTION_DATA_ALIGN(var, alignbytes) \
    __attribute__((section("DataQuickAccess"))) var __attribute__((aligned(alignbytes)))
#else
#error Toolchain not supported.
#endif /* defined(__ICCARM__) */
/*! @} */

/*!
 * @name Ram Function
 * @{
 *
 * @def RAMFUNCTION_SECTION_CODE(func)
 * Place function in ram.
 */
#if (defined(__ICCARM__))
#define RAMFUNCTION_SECTION_CODE(func) func @"RamFunction"
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
#define RAMFUNCTION_SECTION_CODE(func) __attribute__((section("RamFunction"))) func
#elif (defined(__GNUC__)) || defined(DOXYGEN_OUTPUT)
#define RAMFUNCTION_SECTION_CODE(func) __attribute__((section("RamFunction"))) func
#else
#error Toolchain not supported.
#endif /* defined(__ICCARM__) */
/*! @} */

/*!
 * @def MSDK_REG_SECURE_ADDR(x)
 * Convert the register address to the one used in secure mode.
 *
 * @def MSDK_REG_NONSECURE_ADDR(x)
 * Convert the register address to the one used in non-secure mode.
 */

#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
#define MSDK_REG_SECURE_ADDR(x) ((uintptr_t)(x) | (0x1UL << 28))
#define MSDK_REG_NONSECURE_ADDR(x) ((uintptr_t)(x) & ~(0x1UL << 28))
#else
#define MSDK_REG_SECURE_ADDR(x) (x)
#define MSDK_REG_NONSECURE_ADDR(x) (x)
#endif

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
        void DefaultISR(void);
#endif

/*
 * The fsl_clock.h is included here because it needs MAKE_VERSION/MAKE_STATUS/status_t
 * defined in previous of this file.
 */
#include "fsl_clock.h"

/*
 * Chip level peripheral reset API, for MCUs that implement peripheral reset control external to a peripheral
 */
#if ((defined(FSL_FEATURE_SOC_SYSCON_COUNT) && (FSL_FEATURE_SOC_SYSCON_COUNT > 0)) || \
     (defined(FSL_FEATURE_SOC_ASYNC_SYSCON_COUNT) && (FSL_FEATURE_SOC_ASYNC_SYSCON_COUNT > 0)))
#include "fsl_reset.h"
#endif

#if defined(FSL_FEATURE_IRQSTEER_EXT_INT_MAX_NUM) && (FSL_FEATURE_IRQSTEER_EXT_INT_MAX_NUM > 0) && defined(FSL_FEATURE_IRQSTEER_IRQ_START_INDEX) && (FSL_FEATURE_IRQSTEER_IRQ_START_INDEX > 0)
void IRQSTEER_EnableInterrupt(int32_t instIdx, IRQn_Type irq);
void IRQSTEER_DisableInterrupt(int32_t instIdx, IRQn_Type irq);
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief Enable specific interrupt.
 *
 * Enable LEVEL1 interrupt. For some devices, there might be multiple interrupt
 * levels. For example, there are NVIC and intmux. Here the interrupts connected
 * to NVIC are the LEVEL1 interrupts, because they are routed to the core directly.
 * The interrupts connected to intmux are the LEVEL2 interrupts, they are routed
 * to NVIC first then routed to core.
 *
 * This function only enables the LEVEL1 interrupts. The number of LEVEL1 interrupts
 * is indicated by the feature macro FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS.
 *
 * @param interrupt The IRQ number.
 * @retval kStatus_Success Interrupt enabled successfully
 * @retval kStatus_Fail Failed to enable the interrupt
 */
static inline status_t EnableIRQ(IRQn_Type interrupt)
{
    status_t status = kStatus_Success;

    if (NotAvail_IRQn == interrupt)
    {
        status = kStatus_Fail;
    }

#if defined(FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS) && (FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS > 0)
    else if ((int32_t)interrupt >= (int32_t)FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS)
    {
#if defined(FSL_FEATURE_IRQSTEER_EXT_INT_MAX_NUM) && (FSL_FEATURE_IRQSTEER_EXT_INT_MAX_NUM > 0) && defined(FSL_FEATURE_IRQSTEER_IRQ_START_INDEX) && (FSL_FEATURE_IRQSTEER_IRQ_START_INDEX > 0)
        int32_t irqsteerInstIdx = (int32_t)((interrupt + 1 - FSL_FEATURE_IRQSTEER_IRQ_START_INDEX) / FSL_FEATURE_IRQSTEER_EXT_INT_MAX_NUM);

        IRQSTEER_EnableInterrupt(irqsteerInstIdx, interrupt);
#else
        status = kStatus_Fail;
#endif
    }
#endif

    else
    {
#if defined(__GIC_PRIO_BITS)
        GIC_EnableIRQ(interrupt);
#else
        NVIC_EnableIRQ(interrupt);
#endif
    }

    return status;
}

/*!
 * @brief Disable specific interrupt.
 *
 * Disable LEVEL1 interrupt. For some devices, there might be multiple interrupt
 * levels. For example, there are NVIC and intmux. Here the interrupts connected
 * to NVIC are the LEVEL1 interrupts, because they are routed to the core directly.
 * The interrupts connected to intmux are the LEVEL2 interrupts, they are routed
 * to NVIC first then routed to core.
 *
 * This function only disables the LEVEL1 interrupts. The number of LEVEL1 interrupts
 * is indicated by the feature macro FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS.
 *
 * @param interrupt The IRQ number.
 * @retval kStatus_Success Interrupt disabled successfully
 * @retval kStatus_Fail Failed to disable the interrupt
 */
static inline status_t DisableIRQ(IRQn_Type interrupt)
{
    status_t status = kStatus_Success;

    if (NotAvail_IRQn == interrupt)
    {
        status = kStatus_Fail;
    }

#if defined(FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS) && (FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS > 0)
    else if ((int32_t)interrupt >= (int32_t)FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS)
    {
#if defined(FSL_FEATURE_IRQSTEER_EXT_INT_MAX_NUM) && (FSL_FEATURE_IRQSTEER_EXT_INT_MAX_NUM > 0) && defined(FSL_FEATURE_IRQSTEER_IRQ_START_INDEX) && (FSL_FEATURE_IRQSTEER_IRQ_START_INDEX > 0)
        int32_t irqsteerInstIdx = (int32_t)((interrupt - FSL_FEATURE_IRQSTEER_IRQ_START_INDEX) / FSL_FEATURE_IRQSTEER_EXT_INT_MAX_NUM);

        IRQSTEER_DisableInterrupt(irqsteerInstIdx, interrupt);
#else
        status = kStatus_Fail;
#endif
    }
#endif

    else
    {
#if defined(__GIC_PRIO_BITS)
        GIC_DisableIRQ(interrupt);
#else
        NVIC_DisableIRQ(interrupt);
#endif
    }

    return status;
}

/*!
 * @brief Enable the IRQ, and also set the interrupt priority.
 *
 * Only handle LEVEL1 interrupt. For some devices, there might be multiple interrupt
 * levels. For example, there are NVIC and intmux. Here the interrupts connected
 * to NVIC are the LEVEL1 interrupts, because they are routed to the core directly.
 * The interrupts connected to intmux are the LEVEL2 interrupts, they are routed
 * to NVIC first then routed to core.
 *
 * This function only handles the LEVEL1 interrupts. The number of LEVEL1 interrupts
 * is indicated by the feature macro FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS.
 *
 * @param interrupt The IRQ to Enable.
 * @param priNum Priority number set to interrupt controller register.
 * @retval kStatus_Success Interrupt priority set successfully
 * @retval kStatus_Fail Failed to set the interrupt priority.
 */
static inline status_t EnableIRQWithPriority(IRQn_Type interrupt, uint8_t priNum)
{
    status_t status = kStatus_Success;

    if (NotAvail_IRQn == interrupt)
    {
        status = kStatus_Fail;
    }

#if defined(FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS) && (FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS > 0)
    else if ((int32_t)interrupt >= (int32_t)FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS)
    {
        status = kStatus_Fail;
    }
#endif

    else
    {
#if defined(__GIC_PRIO_BITS)
        GIC_SetPriority(interrupt, priNum);
        GIC_EnableIRQ(interrupt);
#else
        NVIC_SetPriority(interrupt, priNum);
        NVIC_EnableIRQ(interrupt);
#endif
    }

    return status;
}

/*!
 * @brief Set the IRQ priority.
 *
 * Only handle LEVEL1 interrupt. For some devices, there might be multiple interrupt
 * levels. For example, there are NVIC and intmux. Here the interrupts connected
 * to NVIC are the LEVEL1 interrupts, because they are routed to the core directly.
 * The interrupts connected to intmux are the LEVEL2 interrupts, they are routed
 * to NVIC first then routed to core.
 *
 * This function only handles the LEVEL1 interrupts. The number of LEVEL1 interrupts
 * is indicated by the feature macro FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS.
 *
 * @param interrupt The IRQ to set.
 * @param priNum Priority number set to interrupt controller register.
 *
 * @retval kStatus_Success Interrupt priority set successfully
 * @retval kStatus_Fail Failed to set the interrupt priority.
 */
static inline status_t IRQ_SetPriority(IRQn_Type interrupt, uint8_t priNum)
{
    status_t status = kStatus_Success;

    if (NotAvail_IRQn == interrupt)
    {
        status = kStatus_Fail;
    }

#if defined(FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS) && (FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS > 0)
    else if ((int32_t)interrupt >= (int32_t)FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS)
    {
        status = kStatus_Fail;
    }
#endif

    else
    {
#if defined(__GIC_PRIO_BITS)
        GIC_SetPriority(interrupt, priNum);
#else
        NVIC_SetPriority(interrupt, priNum);
#endif
    }

    return status;
}

/*!
 * @brief Clear the pending IRQ flag.
 *
 * Only handle LEVEL1 interrupt. For some devices, there might be multiple interrupt
 * levels. For example, there are NVIC and intmux. Here the interrupts connected
 * to NVIC are the LEVEL1 interrupts, because they are routed to the core directly.
 * The interrupts connected to intmux are the LEVEL2 interrupts, they are routed
 * to NVIC first then routed to core.
 *
 * This function only handles the LEVEL1 interrupts. The number of LEVEL1 interrupts
 * is indicated by the feature macro FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS.
 *
 * @param interrupt The flag which IRQ to clear.
 *
 * @retval kStatus_Success Interrupt priority set successfully
 * @retval kStatus_Fail Failed to set the interrupt priority.
 */
static inline status_t IRQ_ClearPendingIRQ(IRQn_Type interrupt)
{
    status_t status = kStatus_Success;

    if (NotAvail_IRQn == interrupt)
    {
        status = kStatus_Fail;
    }

#if defined(FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS) && (FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS > 0)
    else if ((int32_t)interrupt >= (int32_t)FSL_FEATURE_NUMBER_OF_LEVEL1_INT_VECTORS)
    {
        status = kStatus_Fail;
    }
#endif

    else
    {
#if defined(__GIC_PRIO_BITS)
        GIC_ClearPendingIRQ(interrupt);
#else
        NVIC_ClearPendingIRQ(interrupt);
#endif
    }

    return status;
}

/*!
 * @brief Disable the global IRQ
 *
 * Disable the global interrupt and return the current primask register. User is required to provided the primask
 * register for the EnableGlobalIRQ().
 *
 * @return Current primask value.
 */
static inline uint32_t DisableGlobalIRQ(void)
{
    uint32_t mask;

#if defined(CPSR_I_Msk)
    mask = __get_CPSR() & CPSR_I_Msk;
#elif defined(DAIF_I_BIT)
    mask = __get_DAIF() & DAIF_I_BIT;
#else
    mask = __get_PRIMASK();
#endif
    __disable_irq();

    return mask;
}

/*!
 * @brief Enable the global IRQ
 *
 * Set the primask register with the provided primask value but not just enable the primask. The idea is for the
 * convenience of integration of RTOS. some RTOS get its own management mechanism of primask. User is required to
 * use the EnableGlobalIRQ() and DisableGlobalIRQ() in pair.
 *
 * @param primask value of primask register to be restored. The primask value is supposed to be provided by the
 * DisableGlobalIRQ().
 */
static inline void EnableGlobalIRQ(uint32_t primask)
{
#if defined(CPSR_I_Msk)
    __set_CPSR((__get_CPSR() & ~CPSR_I_Msk) | primask);
#elif defined(DAIF_I_BIT)
    if (0UL == primask)
    {
        __enable_irq();
    }
#else
    __set_PRIMASK(primask);
#endif
}

#if defined(ENABLE_RAM_VECTOR_TABLE)
/*!
 * @brief install IRQ handler
 *
 * @param irq IRQ number
 * @param irqHandler IRQ handler address
 * @return The old IRQ handler address
 */
uint32_t InstallIRQHandler(IRQn_Type irq, uint32_t irqHandler);
#endif /* ENABLE_RAM_VECTOR_TABLE. */

#if (defined(FSL_FEATURE_SOC_SYSCON_COUNT) && (FSL_FEATURE_SOC_SYSCON_COUNT > 0))

/*
 * When FSL_FEATURE_POWERLIB_EXTEND is defined to non-zero value,
 * powerlib should be used instead of these functions.
 */
#if !(defined(FSL_FEATURE_POWERLIB_EXTEND) && (FSL_FEATURE_POWERLIB_EXTEND != 0))
/*!
 * @brief Enable specific interrupt for wake-up from deep-sleep mode.
 *
 * Enable the interrupt for wake-up from deep sleep mode.
 * Some interrupts are typically used in sleep mode only and will not occur during
 * deep-sleep mode because relevant clocks are stopped. However, it is possible to enable
 * those clocks (significantly increasing power consumption in the reduced power mode),
 * making these wake-ups possible.
 *
 * @note This function also enables the interrupt in the NVIC (EnableIRQ() is called internaly).
 *
 * @param interrupt The IRQ number.
 */
void EnableDeepSleepIRQ(IRQn_Type interrupt);

/*!
 * @brief Disable specific interrupt for wake-up from deep-sleep mode.
 *
 * Disable the interrupt for wake-up from deep sleep mode.
 * Some interrupts are typically used in sleep mode only and will not occur during
 * deep-sleep mode because relevant clocks are stopped. However, it is possible to enable
 * those clocks (significantly increasing power consumption in the reduced power mode),
 * making these wake-ups possible.
 *
 * @note This function also disables the interrupt in the NVIC (DisableIRQ() is called internaly).
 *
 * @param interrupt The IRQ number.
 */
void DisableDeepSleepIRQ(IRQn_Type interrupt);
#endif /* FSL_FEATURE_POWERLIB_EXTEND */
#endif /* FSL_FEATURE_SOC_SYSCON_COUNT */

#if defined(DWT)
/*!
 * @brief Enable the counter to get CPU cycles.
 */
void MSDK_EnableCpuCycleCounter(void);

/*!
 * @brief Get the current CPU cycle count.
 *
 * @return Current CPU cycle count.
 */
uint32_t MSDK_GetCpuCycleCount(void);
#endif

#if defined(FSL_FEATURE_MEASURE_CRITICAL_SECTION) && (FSL_FEATURE_MEASURE_CRITICAL_SECTION != 0)
typedef uint32_t (*getTimestamp_t)(void); /*!< Function to get the current time stamp. */

/*!
 * @rief Initialize the context of the critical section measurement and assign
 * the function to get the current timestamp.
 *
 * @param getTimestamp The function to get the current timestamp.
 */
void InitCriticalSectionMeasurementContext(getTimestamp_t func);

/*!
 * @brief Disable the global IRQ with critical section ID
 *
 * Extended function of DisableGlobalIRQ. Apart from the standard operation, also check
 * the id of the protected critical section and mark the begining for timer.
 * User is required to provided the primask register for the EnableGlobalIRQEx.
 *
 * @param id The id for critical section.
 * @return Current primask value.
 */
uint32_t DisableGlobalIRQEx(uint32_t id);

/*!
 * @brief Enable the global IRQ and calculate the execution time of critical section
 *
 * Extended function of EnableGlobalIRQ. Apart from the standard operation, also
 * marks the exit of the critical section and calculate the execution time for the section.
 * User is required to use the DisableGlobalIRQEx and EnableGlobalIRQEx in pair.
 *
 * @param primask value of primask register to be restored. The primask value is supposed to be provided by the
 * DisableGlobalIRQEx().
 */
void EnableGlobalIRQEx(uint32_t primask);
#endif


/* clang-format off */
#if ((defined(__ARM_ARCH_7M__     ) && (__ARM_ARCH_7M__      == 1)) || \
     (defined(__ARM_ARCH_7EM__    ) && (__ARM_ARCH_7EM__     == 1)) || \
     (defined(__ARM_ARCH_8M_MAIN__) && (__ARM_ARCH_8M_MAIN__ == 1)) || \
     (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ == 1)))
/* clang-format on */
static inline bool _SDK_AtomicLocalCompareAndSet1Byte(volatile uint8_t *addr, uint8_t expected, uint8_t newValue)
{
    uint8_t s_actual;

    do
    {
        s_actual = __LDREXB(addr);
        if (s_actual != expected)
        {
            __CLREX();
            return false;
        }
    } while (__STREXB((newValue), (addr)));

    return true;
    
}

static inline bool _SDK_AtomicLocalCompareAndSet2Byte(volatile uint16_t *addr, uint16_t expected, uint16_t newValue)
{
    uint16_t s_actual;

    do
    {
        s_actual = __LDREXH(addr);
        if (s_actual != expected)
        {
            __CLREX();
            return false;
        }
    } while (__STREXH((newValue), (addr)));

    return true;
}

static inline bool _SDK_AtomicLocalCompareAndSet4Byte(volatile uint32_t *addr, uint32_t expected, uint32_t newValue)
{
    uint32_t s_actual;

    do
    {
        s_actual = __LDREXW(addr);
        if (s_actual != expected)
        {
            __CLREX();
            return false;
        }
    } while (__STREXW((newValue), (addr)));

    return true;
}

static inline uint8_t _SDK_AtomicLocalTestAndSet1Byte(volatile uint8_t *addr, uint8_t newValue)
{
    uint8_t s_old;

    do
    {
        s_old = __LDREXB(addr);
    } while (__STREXB((newValue), (addr)));

    return s_old;
}

static inline uint16_t _SDK_AtomicLocalTestAndSet2Byte(volatile uint16_t *addr, uint16_t newValue)
{
    uint16_t s_old;

    do
    {
        s_old = __LDREXH(addr);
    } while (__STREXH((newValue), (addr)));

    return s_old;
}

static inline uint32_t _SDK_AtomicLocalTestAndSet4Byte(volatile uint32_t *addr, uint32_t newValue)
{
    uint32_t s_old;

    do
    {
        s_old = __LDREXW(addr);
    } while (__STREXW((newValue), (addr)));

    return s_old;
}

#else
static inline bool _SDK_AtomicLocalCompareAndSet(uint32_t *addr, uint32_t expected, uint32_t newValue)
{
    uint32_t s_atomicOldInt;
    uint32_t s_actual;

    s_atomicOldInt = DisableGlobalIRQ();
    
    s_actual = *addr;
    if (s_actual == expected)
    {
        *addr = newValue;
        EnableGlobalIRQ(s_atomicOldInt);
        return true;
    }
    else
    {
        EnableGlobalIRQ(s_atomicOldInt);
        return false;
    }
}

static inline uint32_t _SDK_AtomicTestAndSet(uint32_t *addr, uint32_t newValue)
{
    uint32_t s_atomicOldInt = DisableGlobalIRQ();

    uint32_t oldValue = (uint32_t)(*addr);
    *addr = newValue;

    EnableGlobalIRQ(s_atomicOldInt);
    return oldValue;
}

#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @} */

#endif /* FSL_COMMON_ARM_H_ */
