/*--------------------------------------------------------------------------*/
/* Copyright 2020-2023 NXP                                                  */
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

#ifndef MCUXCLCORE_EXAMPLES_H_
#define MCUXCLCORE_EXAMPLES_H_

#include <mcuxClCore_Platform.h>
#include <mcuxClCore_Macros.h>
#include <mcuxCsslFlowProtection.h>


/**
 * \def MCUXCLEXAMPLE_FUNCTION
 * \brief Macro to indicate that the symbol is an example function.
 */
// TODO CLNS-3599: #define MCUXCLEXAMPLE_FUNCTION(_name) uint32_t _name(void)
#define MCUXCLEXAMPLE_FUNCTION(_name) \
MCUX_CSSL_ANALYSIS_START_PATTERN_EXAMPLE_FUNCTION() \
bool _name(void) \
MCUX_CSSL_ANALYSIS_STOP_PATTERN_EXAMPLE_FUNCTION()

/**
 * \def MCUXCLEXAMPLE_STATUS_OK
 * \brief Example execution completed successfully.
 */
#define MCUXCLEXAMPLE_STATUS_OK      true // TODO CLNS-3599: 0xC001C0DEu

/**
 * \def MCUXCLEXAMPLE_OK
 * \brief Example execution completed successfully.
 * \deprecated{Replaced by MCUXCLEXAMPLE_STATUS_OK}
 */
#define MCUXCLEXAMPLE_OK      MCUXCLEXAMPLE_STATUS_OK

/**
 * \def MCUXCLEXAMPLE_STATUS_ERROR
 * \brief Example execution resulted in an unexpected error.
 */
#define MCUXCLEXAMPLE_STATUS_ERROR   false // TODO CLNS-3599: 0xEEEEEEEEu

/**
 * \def MCUXCLEXAMPLE_ERROR
 * \brief Example execution resulted in an unexpected error.
 * \deprecated{Replaced by MCUXCLEXAMPLE_STATUS_ERROR}
 */
#define MCUXCLEXAMPLE_ERROR   MCUXCLEXAMPLE_STATUS_ERROR


/**
 * \def MCUXCLEXAMPLE_STATUS_FAILURE
 * \brief Example execution resulted in an expected failure.
 */
#define MCUXCLEXAMPLE_STATUS_FAILURE  false // TODO CLNS-3599: 0xFFFFFFFFu

/**
 * \def MCUXCLEXAMPLE_FAILURE
 * \brief Example execution resulted in an expected failure.
 * \deprecated{Replaced by MCUXCLEXAMPLE_STATUS_FAILURE}
 */
#define MCUXCLEXAMPLE_FAILURE  MCUXCLEXAMPLE_STATUS_FAILURE

/**
 * \brief Assert whether two buffers are equal.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClCore_assertEqual)
static inline bool mcuxClCore_assertEqual(const uint8_t * const x, const uint8_t * const y, uint32_t length)
{
  for (uint32_t i = 0; i < length; ++i)
  {
    if (x[i] != y[i])
    {
      return false;
    }
  }

  return true;
}

#endif /* MCUXCLCORE_EXAMPLES_H_ */
