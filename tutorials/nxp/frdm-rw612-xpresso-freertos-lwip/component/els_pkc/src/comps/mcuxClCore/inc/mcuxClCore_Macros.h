/*--------------------------------------------------------------------------*/
/* Copyright 2023-2024 NXP                                                  */
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
 * @file  mcuxClCore_Macros.h
 * @brief Definition of macros.
 *
 */

#ifndef MCUXCLCORE_MACROS_H_
#define MCUXCLCORE_MACROS_H_

/* Macro to calculate the rounded down number of words that fit into the specified size */
#define MCUXCLCORE_NUM_OF_WORDS_FLOOR(wordsize, size)  \
  ((size) / (wordsize))

/* Macro to calculate the rounded up number of words needed to fit an object of the specified size */
#define MCUXCLCORE_NUM_OF_WORDS_CEIL(wordsize, size)  \
  (((size) / (wordsize)) +  ((((size) % (wordsize)) + ((wordsize) - 1u)) / (wordsize)))

/* Macro to calculate the rounded down number of CPU words that fit into the specified size */
#define MCUXCLCORE_NUM_OF_CPUWORDS_FLOOR(size)  \
  MCUXCLCORE_NUM_OF_WORDS_FLOOR(sizeof(uint32_t), size)

/* Macro to calculate the rounded up number of CPU words needed to fit an object of the specified size */
#define MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(size)  \
  MCUXCLCORE_NUM_OF_WORDS_CEIL(sizeof(uint32_t), size)

/* Macro to round up a given size to the nearest multiple of a specified word size */
#define MCUXCLCORE_ALIGN_TO_WORDSIZE(wordsize, size)  \
  (MCUXCLCORE_NUM_OF_WORDS_CEIL(wordsize, size) * (wordsize))

/* Macro to round up a given size to the nearest multiple of the CPU word size */
#define MCUXCLCORE_ALIGN_TO_CPU_WORDSIZE(size)  \
  MCUXCLCORE_ALIGN_TO_WORDSIZE(sizeof(uint32_t), size)

/* Macro to calculate the maximum of two values */
#define MCUXCLCORE_MAX(a, b)  \
MCUX_CSSL_ANALYSIS_START_SUPPRESS_CONTROLLING_EXPRESSION_IS_INVARIANT("Fixed values are allowed as macro inputs") \
  (((a) > (b)) ? (a) : (b)) \
MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_CONTROLLING_EXPRESSION_IS_INVARIANT()

/* Macro to calculate the minimum of two values */
#define MCUXCLCORE_MIN(a, b)  \
  (((a) < (b)) ? (a) : (b))

#endif /* MCUXCLCORE_MACROS_H_ */
