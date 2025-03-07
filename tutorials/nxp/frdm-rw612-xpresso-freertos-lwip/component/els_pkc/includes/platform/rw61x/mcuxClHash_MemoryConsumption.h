/*--------------------------------------------------------------------------*/
/* Copyright 2020-2024 NXP                                                  */
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

/** @file  mcuxClHash_MemoryConsumption.h
 *  @brief Defines the memory consumption for the mcuxClHash component */

#ifndef MCUXCLHASH_MEMORYCONSUMPTION_H_
#define MCUXCLHASH_MEMORYCONSUMPTION_H_

#define MCUXCLHASH_COMPUTE_CPU_WA_BUFFER_SIZE_MAX                (192u)                            ///< Defines the max workarea size required for mcuxClHash_compute
#define MCUXCLHASH_COMPARE_CPU_WA_BUFFER_SIZE_MAX                (192u)                            ///< Defines the max workarea size required for mcuxClHash_compare
#define MCUXCLHASH_INIT_CPU_WA_BUFFER_SIZE                       (4u)                                                ///< Defines the max workarea size required for mcuxClHash_init
#define MCUXCLHASH_PROCESS_CPU_WA_BUFFER_SIZE_MAX                (192u)                            ///< Defines the max workarea size required for mcuxClHash_process
#define MCUXCLHASH_FINISH_CPU_WA_BUFFER_SIZE_MAX                 (192u)                            ///< Defines the max workarea size required for mcuxClHash_finish
#define MCUXCLHASH_VERIFY_CPU_WA_BUFFER_SIZE_MAX                 (192u)                            ///< Defines the max workarea size required for mcuxClHash_verify

#define MCUXCLHASH_MAX_CPU_WA_BUFFER_SIZE                        (192u)  ///< Defines the max workarea size required for this component


/****************************************************************************/
/* Definitions of context sizes for the mcuxClHash multi-part functions.     */
/****************************************************************************/

#define MCUXCLHASH_CONTEXT_SIZE                                  (400u)    ///< Defines the maximum size a context might need.
#define MCUXCLHASH_CONTEXT_SIZE_IN_WORDS                         (400u / sizeof(uint32_t))


#endif /* MCUXCLHASH_MEMORYCONSUMPTION_H_ */
