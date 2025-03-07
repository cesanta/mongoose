/*--------------------------------------------------------------------------*/
/* Copyright 2021-2022 NXP                                                  */
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
 * @file  mcuxCsslMemory_Internal_Compare_asm.h
 * @brief Internal header of mcuxCsslMemory_Compare inline-asm macro
 */


#ifndef MCUXCSSLMEMORY_INTERNAL_COMPARE_ASM_H_
#define MCUXCSSLMEMORY_INTERNAL_COMPARE_ASM_H_

#include <stdint.h>
#include <stdbool.h>


#if defined(__ghs__) || defined( __ICCARM__ ) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || (defined(__GNUC__) && defined(__arm__))


#define MCUXCSSLMEMORY_COMPARE_ASM_COMPARISON(retval_, cur_lhs_, cur_rhs_, nwords_, cnt_, notValid_, result_) \
do{  \
    uint32_t dat_lhs, dat_rhs;  \
    __asm volatile (  \
        "EOR    %[_retval], %[_result], %[_notValid]\n" /* retval should now be 0xFFFFFFFF */\
        "LSRS   %[_nwords], %[_cnt], #2\n"                \
        "CMP    %[_nwords], #0\n"                         \
        "BGT    mcuxCsslMemory_Compare_word_loop\n"        \
        "BEQ    mcuxCsslMemory_Compare_word_loop_end\n"    \
        "B      mcuxCsslMemory_Compare_fault\n"            \
        "mcuxCsslMemory_Compare_word_loop:\n"              \
        "LDR    %[_dat_lhs], [%[_cur_lhs]], #+4\n"        \
        "LDR    %[_dat_rhs], [%[_cur_rhs]], #+4\n"        \
        "SUBS   %[_cnt], %[_cnt], #+4\n"                  \
        "EORS   %[_dat_lhs], %[_dat_lhs], %[_dat_rhs]\n"  \
        "BICS   %[_retval], %[_retval], %[_dat_lhs]\n"    \
        "SUBS   %[_nwords], %[_nwords], #+1\n"            \
        "MVN    %[_dat_rhs], %[_dat_lhs]\n"               \
        "AND    %[_retval], %[_retval], %[_dat_rhs]\n"    \
        "BNE     mcuxCsslMemory_Compare_word_loop\n"       \
        "mcuxCsslMemory_Compare_word_loop_end:\n"          \
        "MOVS   %[_dat_lhs], #0\n"                        \
        "MOVS   %[_dat_rhs], #0\n"                        \
        "CMP    %[_cnt], #0\n"                            \
        "BGT    mcuxCsslMemory_Compare_byte_loop\n"        \
        "BEQ    mcuxCsslMemory_Compare_fault\n"            \
        "mcuxCsslMemory_Compare_byte_loop:\n"              \
        "LDRB   %[_dat_lhs], [%[_cur_lhs]], #+1\n"        \
        "LDRB   %[_dat_rhs], [%[_cur_rhs]], #+1\n"        \
        "EORS   %[_dat_lhs], %[_dat_lhs], %[_dat_rhs]\n"  \
        "BICS   %[_retval], %[_retval], %[_dat_lhs]\n"    \
        "SUBS   %[_cnt], %[_cnt], #+1\n"                  \
        "MVN    %[_dat_rhs], %[_dat_lhs]\n"               \
        "AND    %[_retval], %[_retval], %[_dat_rhs]\n"    \
        "BNE    mcuxCsslMemory_Compare_byte_loop\n"        \
        "mcuxCsslMemory_Compare_fault:\n"                  \
        : [_retval] "=r" (retval_),    \
          [_cur_lhs] "+r" (cur_lhs_),  \
          [_cur_rhs] "+r" (cur_rhs_),  \
          [_cnt] "+r" (cnt_),          \
          [_nwords] "+r" (nwords_),    \
          [_dat_lhs] "=r" (dat_lhs),   \
          [_dat_rhs] "=r" (dat_rhs)    \
        : [_notValid] "r" (notValid_), \
          [_result] "r" (result_)      \
    );  \
    (void)dat_lhs;  \
    (void)dat_rhs;  \
    (void)cnt_;     \
    (void)notValid_;  \
}while(false)


#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
#define MCUXCSSLMEMORY_COMPARE_ASM_COMPARISON(retval_, cur_lhs_, cur_rhs_, nwords_, cnt_, notValid_, result_) \
do{  \
    uint32_t dat_lhs, dat_rhs;  \
    __asm {  \
        EOR    retval_, result_, notValid_;  \
        LSRS   nwords_, cnt_, 2;  \
        CMP    nwords_, 0;  \
        BGT    mcuxCsslMemory_Compare_word_loop;  \
        BEQ    mcuxCsslMemory_Compare_word_loop_end;  \
        B      mcuxCsslMemory_Compare_fault;  \
mcuxCsslMemory_Compare_word_loop:  \
        LDR    dat_lhs, [cur_lhs_], +4;  \
        LDR    dat_rhs, [cur_rhs_], +4;  \
        SUBS   cnt_, cnt_, 4;  \
        EORS   dat_lhs, dat_lhs, dat_rhs;  \
        BICS   retval_, retval_, dat_lhs;  \
        SUBS   nwords_, nwords_, 1;  \
        MVN    dat_rhs, dat_lhs;  \
        AND    retval_, retval_, dat_rhs;  \
        BNE    mcuxCsslMemory_Compare_word_loop;  \
mcuxCsslMemory_Compare_word_loop_end:  \
        MOVS   dat_lhs, 0;  \
        MOVS   dat_rhs, 0;  \
        CMP    cnt_, 0;  \
        BGT    mcuxCsslMemory_Compare_byte_loop;  \
        BEQ    mcuxCsslMemory_Compare_fault;  \
mcuxCsslMemory_Compare_byte_loop:  \
        LDRB   dat_lhs, [cur_lhs_], +1;  \
        LDRB   dat_rhs, [cur_rhs_], +1;  \
        EORS   dat_lhs, dat_lhs, dat_rhs;  \
        BICS   retval_, retval_, dat_lhs;  \
        SUBS   cnt_, cnt_, 1;  \
        MVN    dat_rhs, dat_lhs;  \
        AND    retval_, retval_, dat_rhs;  \
        BNE    mcuxCsslMemory_Compare_byte_loop;  \
mcuxCsslMemory_Compare_fault:  \
    }  \
    (void)dat_lhs;  \
    (void)dat_rhs;  \
    (void)cnt_;  \
    (void)notValid_;  \
}while(false)

#else
    #error Unsupported compiler. The above section must be manually adapted to support your compiler inline assembly syntax.
#endif


/* If the binary representation of retval contains a zero,
 * this sequence of instructions will turn retval to zero completely.
 * If retval is 0xFFFFFFFF, it will remain untouched.
 */
#if defined(__ghs__) || defined( __ICCARM__ ) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || (defined(__GNUC__) && defined(__arm__))
#define MCUXCSSLMEMORY_COMPARE_ASM_CALC_RETVAL(retval_, errCode_)  \
do{  \
    __asm volatile (  \
        "and %[_retval], %[_retval], %[_retval], ror  #1\n"  \
        "and %[_retval], %[_retval], %[_retval], ror  #2\n"  \
        "and %[_retval], %[_retval], %[_retval], ror  #4\n"  \
        "and %[_retval], %[_retval], %[_retval], ror  #8\n"  \
        "and %[_retval], %[_retval], %[_retval], ror #16\n"  \
        "eor %[_retval], %[_retval], %[_errCode]\n"  \
        : [_retval] "+r" (retval_)  \
        : [_errCode] "r" (errCode_)  \
    );  \
    (void)errCode_;  \
}while(false)

#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
#define MCUXCSSLMEMORY_COMPARE_ASM_CALC_RETVAL(retval_, errCode_)  \
do{  \
    __asm {  \
        AND retval_, retval_, retval_, ror 1;  \
        AND retval_, retval_, retval_, ror 2;  \
        AND retval_, retval_, retval_, ror 4;  \
        AND retval_, retval_, retval_, ror 8;  \
        AND retval_, retval_, retval_, ror 16;  \
        EOR retval_, retval_, errCode_;  \
    }  \
    (void)errCode_;  \
}while(false)

#else
    #error Unsupported compiler. The above section must be manually adapted to support your compiler inline assembly syntax.
#endif


#endif /* MCUXCSSLMEMORY_INTERNAL_COMPARE_ASM_H_ */
