/**************************************************************************//**
 * @file     cmsis_gcc.h
 * @brief    CMSIS compiler GCC header file
 * @version  V6.0.0
 * @date     27. July 2024
 ******************************************************************************/
/*
 * Copyright (c) 2009-2023 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CMSIS_GCC_H
#define __CMSIS_GCC_H

#pragma GCC system_header   /* treat file as system include file */

#include <arm_acle.h>

/* Fallback for __has_builtin */
#ifndef __has_builtin
  #define __has_builtin(x) (0)
#endif

/* CMSIS compiler specific defines */
#ifndef   __ASM
  #define __ASM                                  __asm
#endif
#ifndef   __INLINE
  #define __INLINE                               inline
#endif
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE                        static inline
#endif
#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif
#ifndef   __NO_RETURN
  #define __NO_RETURN                            __attribute__((__noreturn__))
#endif
#ifndef   CMSIS_DEPRECATED
  #define CMSIS_DEPRECATED                       __attribute__((deprecated))
#endif
#ifndef   __USED
  #define __USED                                 __attribute__((used))
#endif
#ifndef   __WEAK
  #define __WEAK                                 __attribute__((weak))
#endif
#ifndef   __PACKED
  #define __PACKED                               __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_STRUCT
  #define __PACKED_STRUCT                        struct __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_UNION
  #define __PACKED_UNION                         union __attribute__((packed, aligned(1)))
#endif
#ifndef   __UNALIGNED_UINT16_WRITE
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  __PACKED_STRUCT T_UINT16_WRITE { uint16_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void *)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT16_READ
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  __PACKED_STRUCT T_UINT16_READ { uint16_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)
#endif
#ifndef   __UNALIGNED_UINT32_WRITE
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  __PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void *)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT32_READ
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  __PACKED_STRUCT T_UINT32_READ { uint32_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)
#endif
#ifndef   __ALIGNED
  #define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif
#ifndef   __RESTRICT
  #define __RESTRICT                             __restrict
#endif
#ifndef   __COMPILER_BARRIER
  #define __COMPILER_BARRIER()                   __ASM volatile("":::"memory")
#endif
#ifndef __NO_INIT
  #define __NO_INIT                              __attribute__ ((section (".noinit")))
#endif
#ifndef __ALIAS
  #define __ALIAS(x)                             __attribute__ ((alias(x)))
#endif

/* ##########################  Core Instruction Access  ######################### */
/** \defgroup CMSIS_Core_InstructionInterface CMSIS Core Instruction Interface
  Access to dedicated instructions
  @{
*/

/* Define macros for porting to both thumb1 and thumb2.
 * For thumb1, use low register (r0-r7), specified by constraint "l"
 * Otherwise, use general registers, specified by constraint "r" */
#if defined (__thumb__) && !defined (__thumb2__)
#define __CMSIS_GCC_OUT_REG(r) "=l" (r)
#define __CMSIS_GCC_RW_REG(r) "+l" (r)
#define __CMSIS_GCC_USE_REG(r) "l" (r)
#else
#define __CMSIS_GCC_OUT_REG(r) "=r" (r)
#define __CMSIS_GCC_RW_REG(r) "+r" (r)
#define __CMSIS_GCC_USE_REG(r) "r" (r)
#endif

/**
  \brief   No Operation
  \details No Operation does nothing. This instruction can be used for code alignment purposes.
 */
#define __NOP()         __ASM volatile ("nop")


/**
  \brief   Wait For Interrupt
  \details Wait For Interrupt is a hint instruction that suspends execution until one of a number of events occurs.
 */
#define __WFI()         __ASM volatile ("wfi":::"memory")


/**
  \brief   Wait For Event
  \details Wait For Event is a hint instruction that permits the processor to enter
           a low-power state until one of a number of events occurs.
 */
#define __WFE()         __ASM volatile ("wfe":::"memory")


/**
  \brief   Send Event
  \details Send Event is a hint instruction. It causes an event to be signaled to the CPU.
 */
#define __SEV()         __ASM volatile ("sev")


/**
  \brief   Instruction Synchronization Barrier
  \details Instruction Synchronization Barrier flushes the pipeline in the processor,
           so that all instructions following the ISB are fetched from cache or memory,
           after the instruction has been completed.
 */
__STATIC_FORCEINLINE void __ISB(void)
{
  __ASM volatile ("isb 0xF":::"memory");
}


/**
  \brief   Data Synchronization Barrier
  \details Acts as a special kind of Data Memory Barrier.
           It completes when all explicit memory accesses before this instruction complete.
 */
__STATIC_FORCEINLINE void __DSB(void)
{
  __ASM volatile ("dsb 0xF":::"memory");
}


/**
  \brief   Data Memory Barrier
  \details Ensures the apparent order of the explicit memory operations before
           and after the instruction, without ensuring their completion.
 */
__STATIC_FORCEINLINE void __DMB(void)
{
  __ASM volatile ("dmb 0xF":::"memory");
}


/**
  \brief   Reverse byte order (32 bit)
  \details Reverses the byte order in unsigned integer value. For example, 0x12345678 becomes 0x78563412.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
__STATIC_FORCEINLINE uint32_t __REV(uint32_t value)
{
  return __builtin_bswap32(value);
}


/**
  \brief   Reverse byte order (16 bit)
  \details Reverses the byte order within each halfword of a word. For example, 0x12345678 becomes 0x34127856.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
__STATIC_FORCEINLINE uint32_t __REV16(uint32_t value)
{
  uint32_t result;

  __ASM ("rev16 %0, %1" : __CMSIS_GCC_OUT_REG (result) : __CMSIS_GCC_USE_REG (value) );
  return (result);
}


/**
  \brief   Reverse byte order (16 bit)
  \details Reverses the byte order in a 16-bit value and returns the signed 16-bit result. For example, 0x0080 becomes 0x8000.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
__STATIC_FORCEINLINE int16_t __REVSH(int16_t value)
{
  return (int16_t)__builtin_bswap16(value);
}


/**
  \brief   Rotate Right in unsigned value (32 bit)
  \details Rotate Right (immediate) provides the value of the contents of a register rotated by a variable number of bits.
  \param [in]    op1  Value to rotate
  \param [in]    op2  Number of Bits to rotate
  \return               Rotated value
 */
__STATIC_FORCEINLINE uint32_t __ROR(uint32_t op1, uint32_t op2)
{
  op2 %= 32U;
  if (op2 == 0U)
  {
    return op1;
  }
  return (op1 >> op2) | (op1 << (32U - op2));
}


/**
  \brief   Breakpoint
  \details Causes the processor to enter Debug state.
           Debug tools can use this to investigate system state when the instruction at a particular address is reached.
  \param [in]    value  is ignored by the processor.
                 If required, a debugger can use it to store additional information about the breakpoint.
 */
#define __BKPT(value) __ASM volatile ("bkpt "#value)


/**
  \brief   Reverse bit order of value
  \details Reverses the bit order of the given value.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
__STATIC_FORCEINLINE uint32_t __RBIT(uint32_t value)
{
  uint32_t result;

#if (__ARM_ARCH_ISA_THUMB >= 2)
   __ASM ("rbit %0, %1" : "=r" (result) : "r" (value) );
#else
  uint32_t s = (4U /*sizeof(v)*/ * 8U) - 1U; /* extra shift needed at end */

  result = value;                      /* r will be reversed bits of v; first get LSB of v */
  for (value >>= 1U; value != 0U; value >>= 1U)
  {
    result <<= 1U;
    result |= value & 1U;
    s--;
  }
  result <<= s;                        /* shift when v's highest bits are zero */
#endif
  return (result);
}


/**
  \brief   Count leading zeros
  \details Counts the number of leading zeros of a data value.
  \param [in]  value  Value to count the leading zeros
  \return             number of leading zeros in value
 */
__STATIC_FORCEINLINE uint8_t __CLZ(uint32_t value)
{
  /* Even though __builtin_clz produces a CLZ instruction on ARM, formally
     __builtin_clz(0) is undefined behaviour, so handle this case specially.
     This guarantees ARM-compatible results if happening to compile on a non-ARM
     target, and ensures the compiler doesn't decide to activate any
     optimisations using the logic "value was passed to __builtin_clz, so it
     is non-zero".
     ARM GCC 7.3 and possibly earlier will optimise this test away, leaving a
     single CLZ instruction.
   */
  if (value == 0U)
  {
    return 32U;
  }
  return __builtin_clz(value);
}


#if (__ARM_FEATURE_SAT    >= 1)
/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (1..32)
  \return             Saturated value
 */
#define __SSAT(value, sat) __ssat(value, sat)


/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (0..31)
  \return             Saturated value
 */
#define __USAT(value, sat) __usat(value, sat)

#else /* (__ARM_FEATURE_SAT >= 1) */
/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (1..32)
  \return             Saturated value
 */
__STATIC_FORCEINLINE int32_t __SSAT(int32_t val, uint32_t sat)
{
  if ((sat >= 1U) && (sat <= 32U))
  {
    const int32_t max = (int32_t)((1U << (sat - 1U)) - 1U);
    const int32_t min = -1 - max ;
    if (val > max)
    {
      return (max);
    }
    else if (val < min)
    {
      return (min);
    }
  }
  return (val);
}


/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (0..31)
  \return             Saturated value
 */
__STATIC_FORCEINLINE uint32_t __USAT(int32_t val, uint32_t sat)
{
  if (sat <= 31U)
  {
    const uint32_t max = ((1U << sat) - 1U);
    if (val > (int32_t)max)
    {
      return (max);
    }
    else if (val < 0)
    {
      return (0U);
    }
  }
  return ((uint32_t)val);
}
#endif /* (__ARM_FEATURE_SAT >= 1) */


#if (__ARM_FEATURE_LDREX >= 1)
/**
  \brief   Remove the exclusive lock
  \details Removes the exclusive lock which is created by LDREX.
 */
__STATIC_FORCEINLINE void __CLREX(void)
{
  __ASM volatile ("clrex" ::: "memory");
}


/**
  \brief   LDR Exclusive (8 bit)
  \details Executes a exclusive LDR instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
__STATIC_FORCEINLINE uint8_t __LDREXB(volatile uint8_t *addr)
{
  uint32_t result;

  __ASM volatile ("ldrexb %0, %1" : "=r" (result) : "Q" (*addr) );
  return ((uint8_t) result);    /* Add explicit type cast here */
}


/**
  \brief   STR Exclusive (8 bit)
  \details Executes a exclusive STR instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
__STATIC_FORCEINLINE uint32_t __STREXB(uint8_t value, volatile uint8_t *addr)
{
  uint32_t result;

  __ASM volatile ("strexb %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" ((uint32_t)value) );
  return (result);
}
#endif /* (__ARM_FEATURE_LDREX >= 1) */


#if (__ARM_FEATURE_LDREX >= 2)
/**
  \brief   LDR Exclusive (16 bit)
  \details Executes a exclusive LDR instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
__STATIC_FORCEINLINE uint16_t __LDREXH(volatile uint16_t *addr)
{
  uint32_t result;

  __ASM volatile ("ldrexh %0, %1" : "=r" (result) : "Q" (*addr) );
  return ((uint16_t)result);    /* Add explicit type cast here */
}


/**
  \brief   STR Exclusive (16 bit)
  \details Executes a exclusive STR instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
__STATIC_FORCEINLINE uint32_t __STREXH(uint16_t value, volatile uint16_t *addr)
{
  uint32_t result;

  __ASM volatile ("strexh %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" ((uint32_t)value) );
  return (result);
}
#endif /* (__ARM_FEATURE_LDREX >= 2) */


#if (__ARM_FEATURE_LDREX >= 4)
/**
  \brief   LDR Exclusive (32 bit)
  \details Executes a exclusive LDR instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
__STATIC_FORCEINLINE uint32_t __LDREXW(volatile uint32_t *addr)
{
  uint32_t result;

  __ASM volatile ("ldrex %0, %1" : "=r" (result) : "Q" (*addr) );
  return (result);
}


/**
  \brief   STR Exclusive (32 bit)
  \details Executes a exclusive STR instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
__STATIC_FORCEINLINE uint32_t __STREXW(uint32_t value, volatile uint32_t *addr)
{
  uint32_t result;

  __ASM volatile ("strex %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" (value) );
  return (result);
}
#endif /* (__ARM_FEATURE_LDREX >= 4) */


#if (__ARM_ARCH_ISA_THUMB >= 2)
/**
  \brief   Rotate Right with Extend (32 bit)
  \details Moves each bit of a bitstring right by one bit.
           The carry input is shifted in at the left end of the bitstring.
  \param [in]    value  Value to rotate
  \return               Rotated value
 */
__STATIC_FORCEINLINE uint32_t __RRX(uint32_t value)
{
  uint32_t result;

  __ASM volatile ("rrx %0, %1" : "=r" (result) : "r" (value));
  return (result);
}


/**
  \brief   LDRT Unprivileged (8 bit)
  \details Executes a Unprivileged LDRT instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
__STATIC_FORCEINLINE uint8_t __LDRBT(volatile uint8_t *ptr)
{
  uint32_t result;

  __ASM volatile ("ldrbt %0, %1" : "=r" (result) : "Q" (*ptr) );
  return ((uint8_t)result);    /* Add explicit type cast here */
}


/**
  \brief   LDRT Unprivileged (16 bit)
  \details Executes a Unprivileged LDRT instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
__STATIC_FORCEINLINE uint16_t __LDRHT(volatile uint16_t *ptr)
{
  uint32_t result;

  __ASM volatile ("ldrht %0, %1" : "=r" (result) : "Q" (*ptr) );
  return ((uint16_t)result);    /* Add explicit type cast here */
}


/**
  \brief   LDRT Unprivileged (32 bit)
  \details Executes a Unprivileged LDRT instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
__STATIC_FORCEINLINE uint32_t __LDRT(volatile uint32_t *ptr)
{
  uint32_t result;

  __ASM volatile ("ldrt %0, %1" : "=r" (result) : "Q" (*ptr) );
  return (result);
}


/**
  \brief   STRT Unprivileged (8 bit)
  \details Executes a Unprivileged STRT instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
__STATIC_FORCEINLINE void __STRBT(uint8_t value, volatile uint8_t *ptr)
{
  __ASM volatile ("strbt %1, %0" : "=Q" (*ptr) : "r" ((uint32_t)value) );
}


/**
  \brief   STRT Unprivileged (16 bit)
  \details Executes a Unprivileged STRT instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
__STATIC_FORCEINLINE void __STRHT(uint16_t value, volatile uint16_t *ptr)
{
  __ASM volatile ("strht %1, %0" : "=Q" (*ptr) : "r" ((uint32_t)value) );
}


/**
  \brief   STRT Unprivileged (32 bit)
  \details Executes a Unprivileged STRT instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
__STATIC_FORCEINLINE void __STRT(uint32_t value, volatile uint32_t *ptr)
{
  __ASM volatile ("strt %1, %0" : "=Q" (*ptr) : "r" (value) );
}
#endif /* (__ARM_ARCH_ISA_THUMB >= 2) */


#if (__ARM_ARCH >= 8)
/**
  \brief   Load-Acquire (8 bit)
  \details Executes a LDAB instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
__STATIC_FORCEINLINE uint8_t __LDAB(volatile uint8_t *ptr)
{
  uint32_t result;

  __ASM volatile ("ldab %0, %1" : "=r" (result) : "Q" (*ptr) : "memory" );
  return ((uint8_t)result);    /* Add explicit type cast here */
}


/**
  \brief   Load-Acquire (16 bit)
  \details Executes a LDAH instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
__STATIC_FORCEINLINE uint16_t __LDAH(volatile uint16_t *ptr)
{
  uint32_t result;

  __ASM volatile ("ldah %0, %1" : "=r" (result) : "Q" (*ptr) : "memory" );
  return ((uint16_t)result);    /* Add explicit type cast here */
}


/**
  \brief   Load-Acquire (32 bit)
  \details Executes a LDA instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
__STATIC_FORCEINLINE uint32_t __LDA(volatile uint32_t *ptr)
{
  uint32_t result;

  __ASM volatile ("lda %0, %1" : "=r" (result) : "Q" (*ptr) : "memory" );
  return (result);
}


/**
  \brief   Store-Release (8 bit)
  \details Executes a STLB instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
__STATIC_FORCEINLINE void __STLB(uint8_t value, volatile uint8_t *ptr)
{
  __ASM volatile ("stlb %1, %0" : "=Q" (*ptr) : "r" ((uint32_t)value) : "memory" );
}


/**
  \brief   Store-Release (16 bit)
  \details Executes a STLH instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
__STATIC_FORCEINLINE void __STLH(uint16_t value, volatile uint16_t *ptr)
{
  __ASM volatile ("stlh %1, %0" : "=Q" (*ptr) : "r" ((uint32_t)value) : "memory" );
}


/**
  \brief   Store-Release (32 bit)
  \details Executes a STL instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
__STATIC_FORCEINLINE void __STL(uint32_t value, volatile uint32_t *ptr)
{
  __ASM volatile ("stl %1, %0" : "=Q" (*ptr) : "r" ((uint32_t)value) : "memory" );
}


/**
  \brief   Load-Acquire Exclusive (8 bit)
  \details Executes a LDAB exclusive instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
__STATIC_FORCEINLINE uint8_t __LDAEXB(volatile uint8_t *ptr)
{
  uint32_t result;

  __ASM volatile ("ldaexb %0, %1" : "=r" (result) : "Q" (*ptr) : "memory" );
  return ((uint8_t)result);    /* Add explicit type cast here */
}


/**
  \brief   Load-Acquire Exclusive (16 bit)
  \details Executes a LDAH exclusive instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
__STATIC_FORCEINLINE uint16_t __LDAEXH(volatile uint16_t *ptr)
{
  uint32_t result;

  __ASM volatile ("ldaexh %0, %1" : "=r" (result) : "Q" (*ptr) : "memory" );
  return ((uint16_t)result);    /* Add explicit type cast here */
}


/**
  \brief   Load-Acquire Exclusive (32 bit)
  \details Executes a LDA exclusive instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
__STATIC_FORCEINLINE uint32_t __LDAEX(volatile uint32_t *ptr)
{
  uint32_t result;

  __ASM volatile ("ldaex %0, %1" : "=r" (result) : "Q" (*ptr) : "memory" );
  return (result);
}


/**
  \brief   Store-Release Exclusive (8 bit)
  \details Executes a STLB exclusive instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
__STATIC_FORCEINLINE uint32_t __STLEXB(uint8_t value, volatile uint8_t *ptr)
{
  uint32_t result;

  __ASM volatile ("stlexb %0, %2, %1" : "=&r" (result), "=Q" (*ptr) : "r" ((uint32_t)value) : "memory" );
  return (result);
}


/**
  \brief   Store-Release Exclusive (16 bit)
  \details Executes a STLH exclusive instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
__STATIC_FORCEINLINE uint32_t __STLEXH(uint16_t value, volatile uint16_t *ptr)
{
  uint32_t result;

  __ASM volatile ("stlexh %0, %2, %1" : "=&r" (result), "=Q" (*ptr) : "r" ((uint32_t)value) : "memory" );
  return (result);
}


/**
  \brief   Store-Release Exclusive (32 bit)
  \details Executes a STL exclusive instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
__STATIC_FORCEINLINE uint32_t __STLEX(uint32_t value, volatile uint32_t *ptr)
{
  uint32_t result;

  __ASM volatile ("stlex %0, %2, %1" : "=&r" (result), "=Q" (*ptr) : "r" ((uint32_t)value) : "memory" );
  return (result);
}

#endif /* (__ARM_ARCH >= 8) */

/** @}*/ /* end of group CMSIS_Core_InstructionInterface */


/* ###########################  Core Function Access  ########################### */
/** \ingroup  CMSIS_Core_FunctionInterface
    \defgroup CMSIS_Core_RegAccFunctions CMSIS Core Register Access Functions
  @{
 */

/**
  \brief   Enable IRQ Interrupts
  \details Enables IRQ interrupts by clearing special-purpose register PRIMASK.
           Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __enable_irq(void)
{
  __ASM volatile ("cpsie i" : : : "memory");
}


/**
  \brief   Disable IRQ Interrupts
  \details Disables IRQ interrupts by setting special-purpose register PRIMASK.
           Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_irq(void)
{
  __ASM volatile ("cpsid i" : : : "memory");
}

#if (__ARM_ARCH_ISA_THUMB >= 2)
  /**
    \brief   Enable FIQ
  \details Enables FIQ interrupts by clearing special-purpose register FAULTMASK.
             Can only be executed in Privileged modes.
   */
  __STATIC_FORCEINLINE void __enable_fault_irq(void)
  {
    __ASM volatile ("cpsie f" : : : "memory");
  }


  /**
    \brief   Disable FIQ
    \details Disables FIQ interrupts by setting special-purpose register FAULTMASK.
             Can only be executed in Privileged modes.
   */
  __STATIC_FORCEINLINE void __disable_fault_irq(void)
  {
    __ASM volatile ("cpsid f" : : : "memory");
  }
#endif


/**
  \brief   Get FPSCR
  \details Returns the current value of the Floating Point Status/Control register.
  \return               Floating Point Status/Control register value
 */
__STATIC_FORCEINLINE uint32_t __get_FPSCR(void)
{
#if (defined(__ARM_FP) && (__ARM_FP >= 1))
  return (__builtin_arm_get_fpscr());
#else
  return (0U);
#endif
}


/**
  \brief   Set FPSCR
  \details Assigns the given value to the Floating Point Status/Control register.
  \param [in]    fpscr  Floating Point Status/Control value to set
 */
__STATIC_FORCEINLINE void __set_FPSCR(uint32_t fpscr)
{
#if (defined(__ARM_FP) && (__ARM_FP >= 1))
  __builtin_arm_set_fpscr(fpscr);
#else
  (void)fpscr;
#endif
}


/** @} end of CMSIS_Core_RegAccFunctions */


/* ###################  Compiler specific Intrinsics  ########################### */
/** \defgroup CMSIS_SIMD_intrinsics CMSIS SIMD Intrinsics
  Access to dedicated SIMD instructions
  @{
*/

#if (defined (__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1))
  #define     __SADD8                 __sadd8
  #define     __QADD8                 __qadd8
  #define     __SHADD8                __shadd8
  #define     __UADD8                 __uadd8
  #define     __UQADD8                __uqadd8
  #define     __UHADD8                __uhadd8
  #define     __SSUB8                 __ssub8
  #define     __QSUB8                 __qsub8
  #define     __SHSUB8                __shsub8
  #define     __USUB8                 __usub8
  #define     __UQSUB8                __uqsub8
  #define     __UHSUB8                __uhsub8
  #define     __SADD16                __sadd16
  #define     __QADD16                __qadd16
  #define     __SHADD16               __shadd16
  #define     __UADD16                __uadd16
  #define     __UQADD16               __uqadd16
  #define     __UHADD16               __uhadd16
  #define     __SSUB16                __ssub16
  #define     __QSUB16                __qsub16
  #define     __SHSUB16               __shsub16
  #define     __USUB16                __usub16
  #define     __UQSUB16               __uqsub16
  #define     __UHSUB16               __uhsub16
  #define     __SASX                  __sasx
  #define     __QASX                  __qasx
  #define     __SHASX                 __shasx
  #define     __UASX                  __uasx
  #define     __UQASX                 __uqasx
  #define     __UHASX                 __uhasx
  #define     __SSAX                  __ssax
  #define     __QSAX                  __qsax
  #define     __SHSAX                 __shsax
  #define     __USAX                  __usax
  #define     __UQSAX                 __uqsax
  #define     __UHSAX                 __uhsax
  #define     __USAD8                 __usad8
  #define     __USADA8                __usada8
  #define     __SSAT16                __ssat16
  #define     __USAT16                __usat16
  #define     __UXTB16                __uxtb16
  #define     __UXTAB16               __uxtab16
  #define     __SXTB16                __sxtb16
  #define     __SXTAB16               __sxtab16
  #define     __SMUAD                 __smuad
  #define     __SMUADX                __smuadx
  #define     __SMLAD                 __smlad
  #define     __SMLADX                __smladx
  #define     __SMLALD                __smlald
  #define     __SMLALDX               __smlaldx
  #define     __SMUSD                 __smusd
  #define     __SMUSDX                __smusdx
  #define     __SMLSD                 __smlsd
  #define     __SMLSDX                __smlsdx
  #define     __SMLSLD                __smlsld
  #define     __SMLSLDX               __smlsldx
  #define     __SEL                   __sel
  #define     __QADD                  __qadd
  #define     __QSUB                  __qsub
  
  #define __PKHBT(ARG1,ARG2,ARG3) \
  __extension__ \
  ({                          \
    uint32_t __RES, __ARG1 = (ARG1), __ARG2 = (ARG2); \
    __ASM ("pkhbt %0, %1, %2, lsl %3" : "=r" (__RES) :  "r" (__ARG1), "r" (__ARG2), "I" (ARG3)  ); \
    __RES; \
   })
  
  #define __PKHTB(ARG1,ARG2,ARG3) \
  __extension__ \
  ({                          \
    uint32_t __RES, __ARG1 = (ARG1), __ARG2 = (ARG2); \
    if (ARG3 == 0) \
      __ASM ("pkhtb %0, %1, %2" : "=r" (__RES) :  "r" (__ARG1), "r" (__ARG2)  ); \
    else \
      __ASM ("pkhtb %0, %1, %2, asr %3" : "=r" (__RES) :  "r" (__ARG1), "r" (__ARG2), "I" (ARG3)  ); \
    __RES; \
   })
  
  __STATIC_FORCEINLINE uint32_t __SXTB16_RORn(uint32_t op1, uint32_t rotate)
  {
      uint32_t result;
      if (__builtin_constant_p(rotate) && ((rotate == 8U) || (rotate == 16U) || (rotate == 24U)))
      {
          __ASM volatile("sxtb16 %0, %1, ROR %2" : "=r"(result) : "r"(op1), "i"(rotate));
      }
      else
      {
          result = __SXTB16(__ROR(op1, rotate));
      }
      return result;
  }
  
  __STATIC_FORCEINLINE uint32_t __SXTAB16_RORn(uint32_t op1, uint32_t op2, uint32_t rotate)
  {
      uint32_t result;
      if (__builtin_constant_p(rotate) && ((rotate == 8U) || (rotate == 16U) || (rotate == 24U)))
      {
          __ASM volatile("sxtab16 %0, %1, %2, ROR %3" : "=r"(result) : "r"(op1), "r"(op2), "i"(rotate));
      }
      else
      {
          result = __SXTAB16(op1, __ROR(op2, rotate));
      }
      return result;
  }
  
  __STATIC_FORCEINLINE int32_t __SMMLA (int32_t op1, int32_t op2, int32_t op3)
  {
    int32_t result;
  
    __ASM volatile ("smmla %0, %1, %2, %3" : "=r" (result): "r"  (op1), "r" (op2), "r" (op3) );
    return (result);
  }
#endif /* (__ARM_FEATURE_DSP == 1) */
/** @} end of group CMSIS_SIMD_intrinsics */

// Include the profile specific settings:
#if __ARM_ARCH_PROFILE == 'A'
  #include "a-profile/cmsis_gcc_a.h"
#elif __ARM_ARCH_PROFILE == 'R'
  #include "r-profile/cmsis_gcc_r.h"
#elif __ARM_ARCH_PROFILE == 'M'
  #include "m-profile/cmsis_gcc_m.h"
#else
  #error "Unknown Arm architecture profile"
#endif

#endif /* __CMSIS_GCC_H */
