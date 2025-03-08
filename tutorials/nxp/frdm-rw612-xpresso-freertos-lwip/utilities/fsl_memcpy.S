/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

    .syntax unified

    .text
    .thumb

    .align 2

#ifndef MSDK_MISC_OVERRIDE_MEMCPY
#define MSDK_MISC_OVERRIDE_MEMCPY 1
#endif

/*
   This mempcy function is used to replace the GCC newlib function for these purposes:
   1. The newlib nano memcpy function use byte by byte copy, it is slow.
   2. The newlib memcpy function for CM4, CM7, CM33 does't check address alignment,
      so it may run to fault when the address is unaligned, and the memory region
      is device memory, which does not support unaligned access.

   This function is manually optimized base on assembly result of the c function.
   The workflow is:
   1. Return directly if length is 0.
   2. If the source address is not 4-byte aligned, copy the unaligned part first byte by byte.
   3. If the destination address is 4-byte aligned, then copy the 16-byte aligned part first,
      copy 16-byte each loop, and then copy 8-byte, 4-byte, 2-byte and 1-byte.
   4. If the destination address is not 4-byte aligned, load source data into register word
      by word first, then store to memory based on alignement requirement. For the left part,
      copy them byte by byte.

   The source code of the c function is:

   #define __CPY_WORD(dst, src) \
       *(uint32_t *)(dst) = *(uint32_t *)(src); \
       (dst) = ((uint32_t *)dst) + 1; \
       (src) = ((uint32_t *)src) + 1

   #define __CPY_HWORD(dst, src) \
       *(uint16_t *)(dst) = *(uint16_t *)(src); \
       (dst) = ((uint16_t *)dst) + 1; \
       (src) = ((uint16_t *)src) + 1

   #define __CPY_BYTE(dst, src) \
       *(uint8_t *)(dst) = *(uint8_t *)(src); \
       (dst) = ((uint8_t *)dst) + 1; \
       (src) = ((uint8_t *)src) + 1

   void * memcpy(void *restrict  dst, const void * restrict src, size_t n)
   {
       void *ret = dst;
       uint32_t tmp;

       if (0 == n) return ret;

       while (((uintptr_t)src & 0x03UL) != 0UL)
       {
           __CPY_BYTE(dst, src);
           n--;

           if (0 == n) return ret;
       }

       if (((uintptr_t)dst & 0x03UL) == 0UL)
       {
           while (n >= 16UL)
           {
               __CPY_WORD(dst, src);
               __CPY_WORD(dst, src);
               __CPY_WORD(dst, src);
               __CPY_WORD(dst, src);
               n-= 16UL;
           }

           if ((n & 0x08UL) != 0UL)
           {
               __CPY_WORD(dst, src);
               __CPY_WORD(dst, src);
           }

           if ((n & 0x04UL) != 0UL)
           {
               __CPY_WORD(dst, src);
           }

           if ((n & 0x02UL) != 0UL)
           {
               __CPY_HWORD(dst, src);
           }

           if ((n & 0x01UL) != 0UL)
           {
               __CPY_BYTE(dst, src);
           }
       }
       else
       {
           if (((uintptr_t)dst & 1UL) == 0UL)
           {
               while (n >= 4)
               {
                   tmp = *(uint32_t *)src;
                   src = ((uint32_t *)src) + 1;

                   *(volatile uint16_t *)dst = (uint16_t)tmp;
                   dst = ((uint16_t *)dst) + 1;
                   *(volatile uint16_t *)dst = (uint16_t)(tmp>>16U);
                   dst = ((uint16_t *)dst) + 1;

                   n-=4;
               }
           }
           else
           {
               while (n >= 4)
               {
                   tmp = *(uint32_t *)src;
                   src = ((uint32_t *)src) + 1;

                   *(volatile uint8_t *)dst  = (uint8_t)tmp;
                   dst = ((uint8_t *)dst) + 1;
                   *(volatile uint16_t *)dst = (uint16_t)(tmp>>8U);
                   dst = ((uint16_t *)dst) + 1;
                   *(volatile uint8_t *)dst = (uint8_t)(tmp>>24U);
                   dst = ((uint8_t *)dst) + 1;
                   n-=4;
               }
           }

           while (n > 0)
           {
               __CPY_BYTE(dst, src);
               n--;
           }
       }

       return ret;
   }

   The test function is:

   void test_memcpy(uint8_t *dst, const uint8_t * src, size_t n)
   {
       uint8_t * ds;
       uint8_t * de;
       const uint8_t *ss;
       const uint8_t *se;
       uint8_t * ret;

       for (ss = src; ss < src+n; ss++)
       {
           for (se = ss; se < src + n; se ++)
           {
               size_t nn = (uintptr_t)se - (uintptr_t)ss;

               for (ds = dst; ds + nn < dst+n; ds++)
               {
                   de = ds + nn;

                   memset(dst, 0, n);

                   ret = memcpy(ds, ss, nn);

                   assert(ret == ds);

                   for (const uint8_t *data = dst; data < ds; data++)
                   {
                       assert(0 == *data);
                   }

                   for (const uint8_t *data = de; data < dst+n; data++)
                   {
                       assert(0 == *data);
                   }

                   assert(memcmp(ds, ss, nn) == 0);
               }
           }
       }
   }

   test_memcpy((uint8_t *)0x20240000, (const uint8_t *)0x202C0000, 48);

 */

#if MSDK_MISC_OVERRIDE_MEMCPY

    .thumb_func
    .align 2
    .global  memcpy
    .type    memcpy, %function

memcpy:
    push    {r0, r4, r5, r6, r7, lr}
    cmp     r2, #0
    beq     ret                    /* If copy size is 0, return. */

src_word_unaligned:
    ands    r3, r1, #3             /* Make src 4-byte align. */
    beq.n   src_word_aligned       /* src is 4-byte aligned, jump. */
    ldrb    r4, [r1], #1
    subs    r2, r2, #1             /* n-- */
    strb    r4, [r0], #1
    beq.n   ret                    /* n=0, return. */
    b.n     src_word_unaligned

src_word_aligned:
    ands    r3, r0, #3             /* Check dest 4-byte align. */
    bne.n   dst_word_unaligned

dst_word_aligned:
    cmp     r2, #16
    blt.n   size_ge_8
size_ge_16:                         /* size greater or equal than 16, use ldm and stm. */
    subs    r2, r2, #16             /* n -= 16 */
    ldmia   r1!, { r4, r5, r6, r7 }
    cmp     r2, #16
    stmia   r0!, { r4, r5, r6, r7 }
    bcs.n   size_ge_16
size_ge_8:                         /* size greater or equal than 8 */
    lsls    r3, r2, #28
    itt     mi
    ldmiami r1!, { r4, r5 }
    stmiami r0!, { r4, r5 }
size_ge_4:                         /* size greater or equal than 4 */
    lsls    r3, r2, #29
    itt     mi
    ldrmi   r4, [r1], #4
    strmi   r4, [r0], #4
size_ge_2:                         /* size greater or equal than 2 */
    lsls    r3, r2, #30
    itt     mi
    ldrhmi  r4, [r1], #2
    strhmi  r4, [r0], #2
size_ge_1:                         /* size greater or equal than 1 */
    lsls    r3, r2, #31
    itt     mi
    ldrbmi  r4, [r1]
    strbmi  r4, [r0]
    b.n     ret

dst_word_unaligned:
    lsls    r3, r0, #31
    bmi.n   dst_half_word_unaligned
dst_half_word_aligned:
    cmp     r2, #4
    bcc.n   size_lt_4
    ldr     r4, [r1], #4
    subs    r2, r2, #4
    strh    r4, [r0], #2
    lsrs    r5, r4, #16
    strh    r5, [r0], #2
    b  dst_half_word_aligned
dst_half_word_unaligned:
    cmp     r2, #4
    bcc.n   size_lt_4
    ldr     r4, [r1], #4
    subs    r2, r2, #4
    strb    r4, [r0], #1
    lsrs    r5, r4, #8
    strh    r5, [r0], #2
    lsrs    r6, r4, #24
    strb    r6, [r0], #1
    b  dst_half_word_unaligned
size_lt_4:                             /* size less than 4. */
    cmp     r2, #0
    ittt    ne
    ldrbne  r4, [r1], #1
    strbne  r4, [r0], #1
    subne   r2, r2, #1
    bne     size_lt_4
ret:
    pop    {r0, r4, r5, r6, r7, pc}

#endif /* MSDK_MISC_OVERRIDE_MEMCPY */
