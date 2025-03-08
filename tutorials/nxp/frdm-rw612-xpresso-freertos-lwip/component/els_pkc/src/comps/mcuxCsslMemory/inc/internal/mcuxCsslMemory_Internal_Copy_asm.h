/*--------------------------------------------------------------------------*/
/* Copyright 2021-2023 NXP                                                  */
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

/**
 * @file  mcuxCsslMemory_Internal_Copy_asm.h
 * @brief Internal header of mcuxCsslMemory_Copy inline-asm macro
 */


#ifndef MCUXCSSLMEMORY_INTERNAL_COPY_ASM_H_ 
#define MCUXCSSLMEMORY_INTERNAL_COPY_ASM_H_ 


#if defined( __ICCARM__ ) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
#define MCUXCSSLMEMORY_COPY_ASM(word, byte, cha, chb, xorword, retval, datareg, src, dst, nwords, cnt, success)  \
do{  \
    __asm (  \
        "MOV    %[_word], #0\n"  \
        "MOV    %[_datareg], #0\n"  \
        "MOV    %[_xorword], #0\n"  \
        "CMP    %[_word], %[_nwords]\n"  \
        "BLT    mcuxCsslMemory_Copy_word_loop\n"  \
        "BGE    mcuxCsslMemory_Copy_word_loop_end\n"  \
        "B      mcuxCsslMemory_Copy_fault\n"  \
        "mcuxCsslMemory_Copy_word_loop:\n"  \
        "LDR    %[_datareg], [%[_src], %[_word], LSL #2]\n"  \
        "EORS   %[_cha], %[_cha], %[_datareg]\n"  \
        "STR    %[_datareg], [%[_dst], %[_word], LSL #2]\n"  \
        "EORS   %[_chb], %[_chb], %[_datareg]\n"  \
        "ADDS   %[_word], %[_word], #+1\n"  \
        "EORS   %[_xorword], %[_xorword], %[_word]\n"  \
        "CMP    %[_word], %[_nwords]\n"  \
        "BLT    mcuxCsslMemory_Copy_word_loop\n"  \
        "mcuxCsslMemory_Copy_word_loop_end:\n"  \
        "LSLS   %[_byte], %[_word], #2\n"  \
        "MOV    %[_datareg], #0\n"  \
        "CMP    %[_byte], %[_nbytes]\n"  \
        "BLT    mcuxCsslMemory_Copy_byte_loop\n"  \
        "BGE    mcuxCsslMemory_Copy_byte_loop_end\n"  \
        "B      mcuxCsslMemory_Copy_fault\n"  \
        "mcuxCsslMemory_Copy_byte_loop:\n"  \
        "LDRB   %[_datareg], [%[_src], %[_byte]]\n"  \
        "EORS   %[_cha], %[_cha], %[_datareg]\n"  \
        "STRB   %[_datareg], [%[_dst], %[_byte]]\n"  \
        "EORS   %[_chb], %[_chb], %[_datareg]\n"  \
        "ADDS   %[_byte], %[_byte], #+1\n"  \
        "CMP    %[_byte], %[_nbytes]\n"  \
        "BLT    mcuxCsslMemory_Copy_byte_loop\n"  \
        "mcuxCsslMemory_Copy_byte_loop_end:\n"  \
        "MOV    %[_datareg], #0\n"  \
        "EORS   %[_retval], %[_retval], %[_success]\n"  \
        "mcuxCsslMemory_Copy_fault:\n"  \
        : [_word] "+&r" (word)  \
        , [_byte] "+&r" (byte)  \
        , [_cha] "+&r" (cha)  \
        , [_chb] "+&r" (chb)  \
        , [_xorword] "+&r" (xorword)  \
        , [_retval] "+r" (retval)  \
        , [_datareg] "+&r" (datareg)  \
        : [_src] "r" (src)  \
        , [_dst] "r" (dst)  \
        , [_nwords] "r" (nwords)  \
        , [_nbytes] "r" (cnt)  \
        , [_success] "r" (success)  \
        : "cc", "memory"  \
    );  \
    (void)datareg;  \
    (void)success;  \
}while(false)

#elif defined (__ghs__)
#define MCUXCSSLMEMORY_COPY_ASM(word, byte, cha, chb, xorword, retval, datareg, src, dst, nwords, cnt, success)  \
do{  \
    /* GHS compiler can only handle 10 registers for the usecase of this inline asm block. */  \
    /* Store retval and success in registers of word and datareg. */  \
    (word) = (retval);  \
    (datareg) = (success);    \
    __asm (  \
        /* store retval and success on stack. */  \
        "SUB    sp, #8\n"  \
        "STR    %[_word], [sp, #0]\n"  \
        "STR    %[_datareg], [sp, #4]\n"  \
        /* original asm macro. */  \
        "MOV    %[_word], #0\n"  \
        "MOV    %[_datareg], #0\n"  \
        "MOV    %[_xorword], #0\n"  \
        "CMP    %[_word], %[_nwords]\n"  \
        "BLT    mcuxCsslMemory_Copy_word_loop\n"  \
        "BGE    mcuxCsslMemory_Copy_word_loop_end\n"  \
        "B      mcuxCsslMemory_Copy_fault\n"  \
        "mcuxCsslMemory_Copy_word_loop:\n"  \
        "LDR    %[_datareg], [%[_src], %[_word], LSL #2]\n"  \
        "EORS   %[_cha], %[_cha], %[_datareg]\n"  \
        "STR    %[_datareg], [%[_dst], %[_word], LSL #2]\n"  \
        "EORS   %[_chb], %[_chb], %[_datareg]\n"  \
        "ADDS   %[_word], %[_word], #+1\n"  \
        "EORS   %[_xorword], %[_xorword], %[_word]\n"  \
        "CMP    %[_word], %[_nwords]\n"  \
        "BLT    mcuxCsslMemory_Copy_word_loop\n"  \
        "mcuxCsslMemory_Copy_word_loop_end:\n"  \
        "LSLS   %[_byte], %[_word], #2\n"  \
        "MOV    %[_datareg], #0\n"  \
        "CMP    %[_byte], %[_nbytes]\n"  \
        "BLT    mcuxCsslMemory_Copy_byte_loop\n"  \
        "BGE    mcuxCsslMemory_Copy_byte_loop_end\n"  \
        "B      mcuxCsslMemory_Copy_fault\n"  \
        "mcuxCsslMemory_Copy_byte_loop:\n"  \
        "LDRB   %[_datareg], [%[_src], %[_byte]]\n"  \
        "EORS   %[_cha], %[_cha], %[_datareg]\n"  \
        "STRB   %[_datareg], [%[_dst], %[_byte]]\n"  \
        "EORS   %[_chb], %[_chb], %[_datareg]\n"  \
        "ADDS   %[_byte], %[_byte], #+1\n"  \
        "CMP    %[_byte], %[_nbytes]\n"  \
        "BLT    mcuxCsslMemory_Copy_byte_loop\n"  \
        "mcuxCsslMemory_Copy_byte_loop_end:\n"  \
        /* load retval and success from stack. */  \
        "LDR    %[_datareg], [sp, #0]\n"  \
        /* use byte as temp. */  \
        "STR    %[_byte], [sp, #0]\n"  \
        "LDR    %[_byte], [sp, #4]\n"  \
        /* set datareg = retval ^ success, instead of datareg = 0 in the original design. */  \
        "EORS   %[_datareg], %[_datareg], %[_byte]\n"  \
        "LDR    %[_byte], [sp, #0]\n"  \
        "mcuxCsslMemory_Copy_fault:\n"  \
        "ADD    sp, #8\n"  \
        : [_word] "+&r" (word)  \
        , [_byte] "+&r" (byte)  \
        , [_cha] "+&r" (cha)  \
        , [_chb] "+&r" (chb)  \
        , [_xorword] "+&r" (xorword)  \
        , [_datareg] "+&r" (datareg)  \
        : [_src] "r" (src)  \
        , [_dst] "r" (dst)  \
        , [_nwords] "r" (nwords)  \
        , [_nbytes] "r" (cnt)  \
        : "cc", "memory"  \
    );  \
    (retval) = (datareg);  \
}while(false)

#elif (defined (__GNUC__) && defined(__arm__))
#define MCUXCSSLMEMORY_COPY_SUCCESS_IMPL ((uint32_t)MCUXCSSLMEMORY_STATUS_OK ^ (uint32_t)MCUXCSSLMEMORY_STATUS_FAULT)
#define MCUXCSSLMEMORY_COPY_DST_STACK_OFFSET (0)
#define MCUXCSSLMEMORY_COPY_SRC_STACK_OFFSET (4)
#define MCUXCSSLMEMORY_COPY_RETVAL_STACK_OFFSET (8)
#define MCUXCSSLMEMORY_COPY_CNT_STACK_OFFSET (12)
#define MCUXCSSLMEMORY_COPY_ASM(word, byte, cha, chb, xorword, retval, datareg, src, dst, nwords, cnt, success)  \
do{  \
    /* [DESIGN]
     * GNU compiler can only handle 7 registers for the usecase of this inline asm block, when building with optimization level 0.
     * Thus, 5 registers need to be saved compared to the original asm macro.
     * success is a constant, so it is defined as an immediatte instead of an input register.
     * 4 registers can be used as temporary registers to push data to the stack: store retval, cnt, src, dst in registers of word, datareg, xorword, byte. */  \
    uint32_t nwords_cnt = (nwords);  \
    (word) = (retval);  \
    (datareg) = (cnt);    \
MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Typecast needed for specialized assembly routine") \
    (xorword) = (uint32_t)(src);  \
    (byte) = (uint32_t)(dst);    \
MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER() \
    __asm (  \
        /* [DESIGN] store retval, success, src, dst on stack. */  \
        "SUB    sp, #16\n"  \
        "STR    %[_word], [sp, %[_RETVAL_OFFSET]]\n"  \
        "STR    %[_datareg], [sp, %[_CNT_OFFSET]]\n"  \
        "STR    %[_byte], [sp, %[_DST_OFFSET]]\n"  \
        "STR    %[_xorword], [sp, %[_SRC_OFFSET]]\n" \
        /* [DESIGN] original asm macro. */  \
        "MOV    %[_word], #0\n"  \
        "MOV    %[_datareg], #0\n"  \
        "MOV    %[_xorword], #0\n"  \
        "CMP    %[_word], %[_nwords_cnt]\n"  \
        "BLT    mcuxCsslMemory_Copy_word_loop\n"  \
        "BGE    mcuxCsslMemory_Copy_word_loop_end\n"  \
        "B      mcuxCsslMemory_Copy_fault\n"  \
        "mcuxCsslMemory_Copy_word_loop:\n"  \
        /* [DESIGN] get src from the stack, _datareg is used as a temporary register */ \
        "LDR    %[_datareg], [sp, %[_SRC_OFFSET]]\n"  \
        "LDR    %[_datareg], [%[_datareg], %[_word], LSL #2]\n"  \
        "EORS   %[_cha], %[_cha], %[_datareg]\n"  \
        /* [DESIGN] _byte already contains the value of dst */ \
        "STR    %[_datareg], [%[_byte], %[_word], LSL #2]\n"  \
        "EORS   %[_chb], %[_chb], %[_datareg]\n"  \
        "ADDS   %[_word], %[_word], #+1\n"  \
        "EORS   %[_xorword], %[_xorword], %[_word]\n"  \
        "CMP    %[_word], %[_nwords_cnt]\n"  \
        "BLT    mcuxCsslMemory_Copy_word_loop\n"  \
        "mcuxCsslMemory_Copy_word_loop_end:\n"  \
        "LSLS   %[_byte], %[_word], #2\n"  \
        "MOV    %[_datareg], #0\n"  \
        "LDR    %[_nwords_cnt], [sp, %[_CNT_OFFSET]]\n"  \
        /* [DESIGN] store word to the stack, at the offset of cnt which is not needed on stack anymore */  \
        "STR    %[_word], [sp, %[_CNT_OFFSET]]\n"  \
        /* [DESIGN] get dst from the stack, _word is used as a temporary register */ \
        "LDR    %[_word], [sp, %[_DST_OFFSET]]\n"  \
        "CMP    %[_byte], %[_nwords_cnt]\n"  \
        "BLT    mcuxCsslMemory_Copy_byte_loop\n"  \
        "BGE    mcuxCsslMemory_Copy_byte_loop_end\n"  \
        "B      mcuxCsslMemory_Copy_fault\n"  \
        "mcuxCsslMemory_Copy_byte_loop:\n"  \
        /* [DESIGN] get src from the stack, _datareg is used as a temporary register */ \
        "LDR    %[_datareg], [sp, %[_SRC_OFFSET]]\n"  \
        "LDRB   %[_datareg], [%[_datareg], %[_byte]]\n"  \
        "EORS   %[_cha], %[_cha], %[_datareg]\n"  \
        /* [DESIGN] _word already contains the value of dst */ \
        "STRB   %[_datareg], [%[_word], %[_byte]]\n"  \
        "EORS   %[_chb], %[_chb], %[_datareg]\n"  \
        "ADDS   %[_byte], %[_byte], #+1\n"  \
        "CMP    %[_byte], %[_nwords_cnt]\n"  \
        "BLT    mcuxCsslMemory_Copy_byte_loop\n"  \
        "mcuxCsslMemory_Copy_byte_loop_end:\n"  \
        /* [DESIGN] load retval from stack. */  \
        "LDR    %[_datareg], [sp, %[_RETVAL_OFFSET]]\n"  \
        /* [DESIGN] use _byte as temp. */  \
        "STR    %[_byte], [sp, %[_RETVAL_OFFSET]]\n"  \
        "LDR    %[_byte], =%[_SUCCESS]\n"  \
        /* [DESIGN] set datareg = retval ^ success, instead of datareg = 0 in the original design. */  \
        "EORS   %[_datareg], %[_datareg], %[_byte]\n"  \
        "LDR    %[_byte], [sp, %[_RETVAL_OFFSET]]\n"  \
        "LDR    %[_word], [sp, %[_CNT_OFFSET]]\n"  \
        "mcuxCsslMemory_Copy_fault:\n"  \
        "ADD    sp, #16\n"  \
        : [_word] "+&r" (word)  \
        , [_byte] "+&r" (byte)  \
        , [_cha] "+&r" (cha)  \
        , [_chb] "+&r" (chb)  \
        , [_xorword] "+&r" (xorword)  \
        , [_datareg] "+&r" (datareg)  \
        , [_nwords_cnt] "+&r" (nwords_cnt)  \
        : [_SUCCESS] "i" MCUXCSSLMEMORY_COPY_SUCCESS_IMPL \
        , [_DST_OFFSET] "i" MCUXCSSLMEMORY_COPY_DST_STACK_OFFSET \
        , [_SRC_OFFSET] "i" MCUXCSSLMEMORY_COPY_SRC_STACK_OFFSET \
        , [_RETVAL_OFFSET] "i" MCUXCSSLMEMORY_COPY_RETVAL_STACK_OFFSET \
        , [_CNT_OFFSET] "i" MCUXCSSLMEMORY_COPY_CNT_STACK_OFFSET \
        : "cc", "memory"  \
    );  \
    (retval) = (datareg);  \
    (void) success; \
}while(false)

#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
#define MCUXCSSLMEMORY_COPY_ASM(word, byte, cha, chb, xorword, retval, datareg, src, dst, nwords, cnt, success)  \
do{  \
    __asm {  \
        MOV    word, 0;  \
        MOV    datareg, 0;  \
        MOV    xorword, 0;  \
        CMP    word, nwords;  \
        BLT    mcuxCsslMemory_Copy_word_loop;  \
        BGE    mcuxCsslMemory_Copy_word_loop_end;  \
        B      mcuxCsslMemory_Copy_fault;  \
mcuxCsslMemory_Copy_word_loop:  \
        LDR    datareg, [src, word, LSL 2];  \
        EORS   cha, cha, datareg;  \
        STR    datareg, [dst, word, LSL 2];  \
        EORS   chb, chb, datareg;  \
        ADDS   word, word, +1;  \
        EORS   xorword, xorword, word;  \
        CMP    word, nwords;  \
        BLT    mcuxCsslMemory_Copy_word_loop;  \
mcuxCsslMemory_Copy_word_loop_end:  \
        LSLS   byte, word, 2;  \
        MOV    datareg, 0;  \
        CMP    byte, cnt;  \
        BLT    mcuxCsslMemory_Copy_byte_loop;  \
        BGE    mcuxCsslMemory_Copy_byte_loop_end;  \
        B      mcuxCsslMemory_Copy_fault;  \
mcuxCsslMemory_Copy_byte_loop:  \
        LDRB   datareg, [src, byte];  \
        EORS   cha, cha, datareg;  \
        STRB   datareg, [dst, byte];  \
        EORS   chb, chb, datareg;  \
        ADDS   byte, byte, +1;  \
        CMP    byte, cnt;  \
        BLT    mcuxCsslMemory_Copy_byte_loop;  \
mcuxCsslMemory_Copy_byte_loop_end:  \
        MOV    datareg, 0;  \
        EORS   retval, retval, success;  \
mcuxCsslMemory_Copy_fault:  \
    }  \
    (void)datareg;  \
    (void)success;  \
}while(false)

#else
    #error Unsupported compiler. The above section must be manually adapted to support your compiler inline assembly syntax.
#endif /* Compiler selection */


#endif /* MCUXCSSLMEMORY_INTERNAL_COPY_ASM_H_  */
