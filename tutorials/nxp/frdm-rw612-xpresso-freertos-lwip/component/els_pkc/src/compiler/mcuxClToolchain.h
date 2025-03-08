/*--------------------------------------------------------------------------*/
/* Copyright 2022-2023 NXP                                                  */
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

#ifndef COMPILER_TOOLCHAIN_H_
#define COMPILER_TOOLCHAIN_H_

/* for armclang */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #define CSS_IDATA_SEGMENT __attribute__((section("CSS_IDATA_SEGMENT")))
    #define CSS_CONST_SEGMENT __attribute__((section("CSS_CONST_SEGMENT")))
    #define MCUX_FUP_ATTRIBUTE __attribute__((aligned(4))) __attribute__((section("MCUX_OBFUSCATED_FUP_SEGMENT")))
    #define UNUSED_PARAM __attribute__((unused))
    /*  
        Use of UNALIGNED on ARMCLANG

        The __unaligned keyword is a type qualifier that tells the compiler to treat the pointer or variable as an unaligned pointer or variable.
        www.keil.com/support/man/docs/armclang_ref/armclang_ref_pfl1493130433688.htm

        The use of __attribute__((packed)) is incorrect on ARMCLANG

        The packed type attribute specifies that a type must have the smallest possible alignment. This attribute only applies to struct and union types.
        www.keil.com/support/man/docs/armclang_ref/armclang_ref_chr1393328521340.htm

        -munaligned-access is the default for architectures that support unaligned accesses to data. This default applies to all architectures supported by Arm Compiler for Embedded 6, except Armv6-M, and Armv8-M without the Main Extension.
    */
    #define UNALIGNED __unaligned
    #define MCUX_CSSL_UNUSED(p) ((void) (p))
    /* Macro for alligning buffers to cpu word */
    #define ALIGNED __attribute__((aligned(4))) 

/* using the gcc toolchain file for both gcc and armgcc */
#elif defined ( __GNUC__ )
    #define CSS_IDATA_SEGMENT
    #define CSS_CONST_SEGMENT
    #define MCUX_FUP_ATTRIBUTE __attribute__((aligned(4)))
    #define UNUSED_PARAM __attribute__((unused))
    #define UNALIGNED
    /* Macro for alligning buffers to cpu word */
    #define ALIGNED __attribute__((aligned(4))) 

/* for armcc compiler */
#elif defined ( __CC_ARM )
    #define CSS_IDATA_SEGMENT __attribute__((section("CSS_IDATA_SEGMENT")))
    #define CSS_CONST_SEGMENT __attribute__((section("CSS_CONST_SEGMENT")))
    #define MCUX_FUP_ATTRIBUTE __attribute__((aligned(4))) __attribute__((section("MCUX_OBFUSCATED_FUP_SEGMENT")))
    #define UNUSED_PARAM __attribute__((unused))
    #define UNALIGNED __packed
    /* Macro for alligning buffers to cpu word */
    #define ALIGNED __attribute__((aligned(4))) 

/* for ghs compiler */
#elif defined ( __ghs__ )
    #define CSS_IDATA_SEGMENT
    #define CSS_CONST_SEGMENT
    #define MCUX_FUP_ATTRIBUTE __attribute__((aligned(4)))
    #define UNUSED_PARAM __attribute__((unused))
    #define UNALIGNED
    /* Macro for alligning buffers to cpu word */
    #define ALIGNED __attribute__((aligned(4))) 

/* for iar compiler */
#elif defined ( __ICCARM__ )
    #define CSS_IDATA_SEGMENT __attribute__((section("CSS_IDATA_SEGMENT")))
    #define CSS_CONST_SEGMENT __attribute__((section("CSS_CONST_SEGMENT")))
    #define MCUX_FUP_ATTRIBUTE __attribute__((aligned(4))) __attribute__((section(".mcux_obfuscated_fup_segment")))
    #define UNUSED_PARAM __attribute__((unused))
    #define UNALIGNED __packed
    /* Macro for alligning buffers to cpu word */
    #define ALIGNED __attribute__((aligned(4))) 

/* for llvm */
#elif defined ( __clang__ )
    #define CSS_IDATA_SEGMENT
    #define CSS_CONST_SEGMENT
    #define MCUX_FUP_ATTRIBUTE __attribute__((aligned(4)))
    #define UNUSED_PARAM __attribute__((unused))
    #define UNALIGNED
    /* Macro for alligning buffers to cpu word */
    #define ALIGNED __attribute__((aligned(4))) 
    

#endif


#if defined(__ghs__) || defined( __ICCARM__ ) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || defined(__GNUC__)
    #define GHS_ICCARM_ARMCC_GT_10_5_GNUC
#endif

#if defined(__ghs__) || defined(__gcc__) || defined(__ICCARM__) || defined(__GNUC__)
    #define GHS_GCC_ICCARM_GNUC
#endif

#if defined(__ghs__) || defined(__gcc__) || defined(__ICCARM__)
    #define GHS_GCC_ICCARM
#endif

#if defined(__ICCARM__) || defined(__ARMCC_VERSION) || defined(__CC_ARM) || defined(__GNUC__)
    #define ICCARM_ARMCC_GNUC
#endif

#if defined(__ICCARM__) || defined(__ARMCC_VERSION) || defined(__GNUC__)
    #define ICCARM_ARMCLANG_GNUC
#endif

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
    #define ARMCC_LT_10_5 //6.01 build 0050
#endif


#if defined ( __CC_ARM )
/* Arm Compiler 4/5 */
#define MCUX_CL_COMPILER_ARMCC
#define MCUX_CL_COMPILER_ARM_COMPILER

#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) && (__ARMCC_VERSION < 6100100)
/* Arm Compiler 6.6 LTM (armclang) */
#define MCUX_CL_COMPILER_ARMCLANG_LTM
#define MCUX_CL_COMPILER_ARM_COMPILER

#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6100100)
/* Arm Compiler above 6.10.1 (armclang) */
#define MCUX_CL_COMPILER_ARMCLANG
#define MCUX_CL_COMPILER_ARM_COMPILER

#elif defined (_clang_)
#define MCUX_CL_COMPILER_ARM_COMPILER /* i.e. Version 6.01 build 0019  */
#endif


#if ( defined(__ARMCC_VERSION) || defined(_MSC_VER) ) && !defined(inline) && !defined(__cplusplus)
    #define ARMCC_MSC_VER_NOT_INLINE_NOT_CPP
#endif

#endif /* COMPILER_TOOLCHAIN_H_ */
