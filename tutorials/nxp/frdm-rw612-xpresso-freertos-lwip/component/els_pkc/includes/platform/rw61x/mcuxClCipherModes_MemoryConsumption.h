/*--------------------------------------------------------------------------*/
/* Copyright 2021-2024 NXP                                                  */
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

/** @file  mcuxClCipher_MemoryConsumption.h
 *  @brief Memory consumption of the mcuxClCipher component
 *         All work area sizes in bytes are a multiple of CPU wordsize.
 */

#ifndef MCUXCLCIPHER_MEMORYCONSUMPTION_H_
#define MCUXCLCIPHER_MEMORYCONSUMPTION_H_

#include <mcuxClCore_Macros.h>

/* Workarea sizes */
#define MCUXCLCIPHER_AES_CRYPT_CPU_WA_BUFFER_SIZE                 (48u)
#define MCUXCLCIPHER_AES_CRYPT_CPU_WA_BUFFER_SIZE_IN_WORDS        MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLCIPHER_AES_CRYPT_CPU_WA_BUFFER_SIZE)

#define MCUXCLCIPHER_AES_INIT_CPU_WA_BUFFER_SIZE                  (4u)
#define MCUXCLCIPHER_AES_PROCESS_CPU_WA_BUFFER_SIZE               (4u)
#define MCUXCLCIPHER_AES_FINISH_CPU_WA_BUFFER_SIZE                (4u)
#define MCUXCLCIPHER_AES_INIT_CPU_WA_BUFFER_SIZE_IN_WORDS         MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLCIPHER_AES_INIT_CPU_WA_BUFFER_SIZE)
#define MCUXCLCIPHER_AES_PROCESS_CPU_WA_BUFFER_SIZE_IN_WORDS      MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLCIPHER_AES_PROCESS_CPU_WA_BUFFER_SIZE)
#define MCUXCLCIPHER_AES_FINISH_CPU_WA_BUFFER_SIZE_IN_WORDS       MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLCIPHER_AES_FINISH_CPU_WA_BUFFER_SIZE)

#define MCUXCLCIPHER_MAX_AES_CPU_WA_BUFFER_SIZE                   (MCUXCLCIPHER_AES_CRYPT_CPU_WA_BUFFER_SIZE)
#define MCUXCLCIPHER_MAX_AES_CPU_WA_BUFFER_SIZE_IN_WORDS          (MCUXCLCIPHER_AES_CRYPT_CPU_WA_BUFFER_SIZE_IN_WORDS)

/* Context sizes */
#define MCUXCLCIPHER_AES_CONTEXT_SIZE                             (48u)
#define MCUXCLCIPHER_AES_CONTEXT_SIZE_IN_WORDS                    (MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLCIPHER_AES_CONTEXT_SIZE))


#endif /* MCUXCLCIPHER_MEMORYCONSUMPTION_H_ */
