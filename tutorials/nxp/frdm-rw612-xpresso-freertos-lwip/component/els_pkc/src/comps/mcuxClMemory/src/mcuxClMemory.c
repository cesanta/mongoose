/*--------------------------------------------------------------------------*/
/* Copyright 2020-2021, 2023-2024 NXP                                       */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms.  If you do not agree to be bound by the applicable        */
/* license terms, then you may not retain, install, activate or otherwise   */
/* use the software.                                                        */
/*--------------------------------------------------------------------------*/

#include <mcuxClMemory.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClToolchain.h>
#include <mcuxCsslAnalysis.h>


#define WORDSIZE  (sizeof(uint32_t))


/**
 * [DESIGN]
 *
 * This function considers the following cases of alignment of source and
 * destination addresses and length:
 *
 *  Src Addr. | Des Addr. | Length    |
 *  ----------+-----------+-----------+-------------------------------
 *  aligned   | aligned   | aligned   | Case A: read word, write word
 *  ----------+-----------+-----------+-------------------------------
 *  aligned   | unaligned | aligned   | Case B: read word, write byte
 *  ----------+-----------+-----------+-------------------------------
 *  unaligned | aligned   | aligned   | Case C: read byte, write word
 *  ----------+-----------+-----------+-------------------------------
 *  unaligned | unaligned | aligned   | Case D: read byte,
 *  any       | any       | unaligned |         write byte-word-byte
 *
 * Since SFR address and length shall be aligned,
 * Cases A and B cover the usecases of SFR reading; and
 * Cases A and C cover the useceses of SFR writing.
 *
 * If length > bufLength, and bufLength is not aligned, in cases A and B,
 * the word containing last byte(s) is read in word, and last byte(s) is written byte-wisely.
 * Ps, since length is aligned, last word in source shall be in valid address range.
 *
 * Case C is a special case of Case D, because the byte-wisely writing will be ignored.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMemory_copy)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_copy (uint8_t *pDst, uint8_t const *pSrc, size_t length, size_t bufLength)
{
    /* This function assumes caller providing valid addresses and length. */

    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMemory_copy);
    MCUX_CSSL_FP_LOOP_DECL(mcuxClMemory_copy_loop);

    uint8_t *pDstX = pDst;
    const uint8_t *pSrcX = pSrc;
    uint32_t copiedLength = 0u;

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("casting pointer to integer to check alignment.");
    MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
    const uint32_t srcAddress = (uint32_t) pSrc;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
    const uint32_t dstAddress = (uint32_t) pDst;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER();

    const uint32_t srcAddrOrLength = srcAddress | length;
    const uint32_t minLength = (length <= bufLength) ? length : bufLength;
    if (0u == (srcAddrOrLength % WORDSIZE))  /* source address and length are both aligned. */
    {
        uint32_t temp = 0u;

        if (0u == (dstAddress % WORDSIZE))   /* destination address is aligned. */
        {
            /* Case A: copy word-wisely. */
            while ((copiedLength + WORDSIZE) <= minLength)
            {
                MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
                MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("Caller shall provide valid buffer pSrc[] of length.")
                MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("source pointer is aligned in Case A.")
                temp = *(const uint32_t *) pSrcX;
                MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()
                pSrcX += WORDSIZE;
                MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()

                MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
                copiedLength += WORDSIZE;

                MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
                MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("Caller shall provide valid buffer pDst[] of bufLength.")
                MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("destination pointer is aligned in Case A.")
                *(uint32_t *) pDstX = temp;
                MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()
                pDstX += WORDSIZE;
                MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()

                MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
            }
        }

        /* Case A: remaining byte(s) when (length < bufLength). */
        /* Case B: read word-wisely, write byte-wisely. */
        while (copiedLength < minLength)
        {
            if (0u == (copiedLength % WORDSIZE))
            {
                MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("Caller shall provide valid buffer pSrc[] of length.")
                MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("source pointer is aligned in Cases A and B.")
                temp = *(const uint32_t *) pSrcX;
                MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()
                pSrcX += WORDSIZE;
                MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
            }
            else
            {
                temp >>= 8u;
            }

            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("Caller shall provide valid buffer pDst[] of bufLength.")
            *pDstX = (uint8_t) (temp & 0xFFu);
            pDstX++;
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()

            copiedLength++;
        }

    }
    else
    {
        /* Cases C & D: read byte-wisely, write (byte-word-byte)-wisely. */
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("modular arithmetic, mod 4")
        const uint32_t unalignedBytes = (0u - dstAddress) % WORDSIZE;
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()

        // Loop on unaligned bytes if any.
        // Loop on words
        // Start at first aligned address, increment by 4 bytes. To understand the loop condition, consider without loss of generality a
        // byte array b_i of length=4 and bufLength=4.
        //
        // |0                             3|4     4|
        // +-------+-------+-------+-------+-------+
        // |  b_0  |  b_1  |  b_2  |  b_3  |       |
        // +-------+-------+-------+-------+-------+
        //
        // In order to determine whether a full word can be copied, check with regard to the copying position i:
        // * Starting from i=0, a full word can be copied. i+4 is the first position that is outside of the valid range,
        //   and it is equal to length.
        // Therefore, checking that i+4 <= length and i+4 <= bufLength is a valid condition to check whether a full word can be
        // copied.
        // Loop on remaining bytes.

        //copy unaligned bytes first, if any
        for (; (copiedLength < minLength) && (copiedLength < unalignedBytes); copiedLength++)
        {
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("Caller shall provide valid buffers pSrc[] of length and pDst[] of bufLength.")
            *pDstX = *pSrcX;
            pDstX++;
            pSrcX++;
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
        }

        MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("The pointer is CPU word aligned after the byte-loop above.");
        uint32_t* p32Dst = (uint32_t *) pDstX;
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING();

        //loop on words
        for (; (copiedLength + WORDSIZE) <= minLength; copiedLength += WORDSIZE)
        {
            /* Volatile keyword is added to avoid any chance of optimization (i.e. full word read) */
            /* The idea is to read byte-wise from SRC to avoid unaligned word reads, but write aligned and word-wise to DST */
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("Caller shall provide valid buffers pSrc[] of length and pDst[] of bufLength.")
            uint32_t crtWordVal = (uint32_t)*(volatile const uint8_t *)pSrcX;
            pSrcX++;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
            crtWordVal |= (uint32_t)*(volatile const uint8_t *)pSrcX << 8u;
            pSrcX++;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
            crtWordVal |= (uint32_t)*(volatile const uint8_t *)pSrcX << 16u;
            pSrcX++;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
            crtWordVal |= (uint32_t)*(volatile const uint8_t *)pSrcX << 24u;
            pSrcX++;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("The pointer is CPU word aligned after the byte-loop above.");
            *p32Dst = crtWordVal;
            p32Dst++;
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE();
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
        }

        pDstX = (uint8_t *) p32Dst;
        //loop on remaining bytes
        for (; (copiedLength < minLength); copiedLength++)
        {
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("Caller shall provide valid buffers pSrc[] of length and pDst[] of bufLength.")
            *pDstX = *pSrcX;
            pDstX++;
            pSrcX++;
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_loop);
        }
    }

    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClMemory_copy,
        ((length <= bufLength) ? length : bufLength) - copiedLength,
        MCUX_CSSL_FP_LOOP_ITERATIONS(mcuxClMemory_copy_loop,
                                    ((length <= bufLength) ? length : bufLength)) );
}


MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMemory_copy_reversed)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_copy_reversed (uint8_t *pDst, uint8_t const *pSrc, size_t length, size_t bufLength)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMemory_copy_reversed);
    uint32_t len = length;
    uint32_t diff;

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("diff is non-negative distance between pSrc and pDst, caculated according to platform architecture.")
    if (pDst > pSrc)
    {
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Casting pSrc and pDst to unsigned integer to calculate difference");
        MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
        diff = (uint32_t)pDst - (uint32_t)pSrc;
        MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER();
    }
    else
    {
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Casting pSrc and pDst to unsigned integer to calculate difference");
        MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
        diff = (uint32_t)pSrc - (uint32_t)pDst;
        MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER();
    }
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()

    if (bufLength < length)
    {
        length = bufLength;
        len = bufLength;
    }

    MCUX_CSSL_FP_LOOP_DECL(mcuxClMemory_copy_reversed_loop);

    //non-overlap case
    if (diff >= length)
    {
        diff = length;
    }

    uint8_t *pDstBt;
    const uint8_t *pSrcBt;
    if (pSrc > pDst)
    {
        // first copy the non-overlop part
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("pDstBt will be in the valid range pDst[0 ~ bufLength] and pSrc will be in the valid range pSrc[0 ~ length].")
        pSrcBt = pSrc + len - 1U;
        pDstBt = (uint8_t *)pDst;

        while (len > length - diff)
        {
            *pDstBt = *pSrcBt;
            pDstBt++;
            pSrcBt--;
            len--;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_reversed_loop);
        }
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
        //then swap the overlap part
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("pDstBt2 will be in the valid range pDst[0 ~ bufLength].")
        uint8_t *pDstBt2 = pDstBt + len - 1u;
        while (len > 1U)
        {
            uint8_t tempByte = *pDstBt2;
            *pDstBt2 = *pDstBt;
            pDstBt2--;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_reversed_loop);
            *pDstBt = tempByte;
            pDstBt++;
            len -= 2U;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_reversed_loop);
        }
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
    }
    else
    {
        // first copy the non-overlop part
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("pDstBt will be in the valid range pDst[0 ~ bufLength].")
        pDstBt = (uint8_t *)pDst + len - 1U;
        pSrcBt = pSrc;

        while (len > length - diff)
        {
            *pDstBt = *pSrcBt;
            pDstBt--;
            pSrcBt++;
            len--;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_reversed_loop);
        }
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
        //then swap the overlap part
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("pDstBt2 will be in the valid range pDst[0 ~ bufLength], pDstBt will be in the valid range pDst[0 ~ bufLength].")
        uint8_t *pDstBt2 = pDstBt - len + 1u;
        while (len > 1U)
        {
            uint8_t tempByte = *pDstBt2;
            *pDstBt2 = *pDstBt;
            pDstBt2++;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_reversed_loop);
            *pDstBt = tempByte;
            pDstBt--;
            len -= 2U;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_copy_reversed_loop);
        }
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
    }

    /* update SC and return */
    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClMemory_copy_reversed,
                              MCUX_CSSL_FP_LOOP_ITERATIONS(mcuxClMemory_copy_reversed_loop, (length - len)));
}


MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMemory_set)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_set (uint8_t *pDst, uint8_t val, size_t length, size_t bufLength)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMemory_set);

    MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(INTEGER_OVERFLOW, "modular arithmetic, mod 4")
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("casting to unsigned integer to calculate unaligned bytes");
    uint32_t unalignedBytes = (0u - (uint32_t)pDst) % (sizeof(uint32_t));
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER();
    MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(INTEGER_OVERFLOW)
    MCUX_CSSL_FP_LOOP_DECL(mcuxClMemory_set_loop);
    uint32_t wordVal = ((uint32_t)val << 24) | ((uint32_t)val << 16) | ((uint32_t)val << 8) | (uint32_t)val;

    //clear unaligned bytes first, if any
    size_t i = 0u;
    for(i = 0u; (i < length) && (i < bufLength) && (i < unalignedBytes); i++)
    {
        MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(INTEGER_OVERFLOW, "pDst will be in the valid range pDst[0 ~ bufLength].")
        *pDst = val;
        pDst++;
        MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_set_loop);
        MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(INTEGER_OVERFLOW)
    }

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("The pointer is CPU word aligned. So, it's safe to cast it to uint32_t*");
    uint32_t* p32Dst = (uint32_t *) pDst;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING();

    //loop on words. See mcuxClMemory_copy for an explanation of the condition
    while(((i + sizeof(uint32_t)) <= length) && ((i + sizeof(uint32_t)) <= bufLength))
    {
        MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(INTEGER_OVERFLOW, "p32Dst will be in the valid range pDst[0 ~ bufLength] and pSrc will be in the valid range pSrc[0 ~ length].")
        MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_set_loop);
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("This assignment never overflows because the pointer p32Dst points to pDst[i] where i <= length - 4");
        *p32Dst = wordVal;
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE();
        MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_set_loop);
        p32Dst++;
        MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_set_loop);
        i += sizeof(uint32_t);
        MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_set_loop);
        MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(INTEGER_OVERFLOW)
    }

    pDst = (uint8_t *) p32Dst;
    //loop on remaining bytes
    for(; (i < length) && (i < bufLength); i++)
    {
        MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(INTEGER_OVERFLOW, "pDst will be in the valid range pDst[0 ~ bufLength].")
        *pDst = val;
        pDst++;
        MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_set_loop);
        MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(INTEGER_OVERFLOW)
    }

    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClMemory_set,
                              ((length <= bufLength) ? length : bufLength) - i,
                              MCUX_CSSL_FP_LOOP_ITERATIONS(mcuxClMemory_set_loop,
                                                          ((length <= bufLength) ? length : bufLength)));
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMemory_clear)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_clear (uint8_t *pDst, size_t length, size_t bufLength)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMemory_clear, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set));

    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_set(pDst, 0U, length, bufLength));

    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClMemory_clear);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMemory_xor)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_xor(uint8_t *pDst, const uint8_t *pSrc1, const uint8_t *pSrc2, uint32_t length, size_t bufLength)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMemory_xor);
    uint32_t remainingLen = length;

    if (bufLength < remainingLen)
    {
        remainingLen = bufLength;
    }

    MCUX_CSSL_FP_LOOP_DECL(mcuxClMemory_xor_loop);

    /* xor by word if aligned */
    MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Typecasting pointer to integer is intentional")
    if ((remainingLen >= WORDSIZE) && (0u == ((uint32_t)pDst & (WORDSIZE - 1u)))
                && (0u == ((uint32_t)pSrc1 & (WORDSIZE - 1u)))
                && (0u == ((uint32_t)pSrc2 & (WORDSIZE - 1u))))
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER()
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
    {
        do
        {
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_xor_loop);
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_CASTING("pSrc1, pSrc2 and pDst are word aligned.")
            const uint32_t temp1 = *(const uint32_t *)pSrc1;
            const uint32_t temp2 = *(const uint32_t *)pSrc2;
            *(uint32_t *)pDst = temp1 ^ temp2;
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_CASTING()

            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_xor_loop);
            pSrc1 += WORDSIZE;
            pSrc2 += WORDSIZE;
            pDst += WORDSIZE;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_xor_loop);
            remainingLen -= WORDSIZE;
            MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_xor_loop);
        } while (remainingLen >= WORDSIZE);
    }

    /* xor the remaining bytes */
    while (remainingLen > 0u)
    {
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_OVERFLOW("Caller should set length and bufLength properly to make sure not to overflow.")
        const uint8_t temp1 = *pSrc1++;
        const uint8_t temp2 = *pSrc2++;
        *pDst++ = temp1 ^ temp2;
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_OVERFLOW()
        remainingLen--;
        MCUX_CSSL_FP_LOOP_ITERATION(mcuxClMemory_xor_loop);
    }

    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClMemory_xor,
                              MCUX_CSSL_FP_LOOP_ITERATIONS(mcuxClMemory_xor_loop,
                                                          ((length <= bufLength) ? length : bufLength)));
}

