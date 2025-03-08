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

/**
 * @file  mcuxClEls_Internal_Common.h
 * @brief ELS header for common internal functionality.
 */

#ifndef MCUXCLELS_INTERNAL_COMMON_H_
#define MCUXCLELS_INTERNAL_COMMON_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClEls_Common.h> // Common types
#include <platform_specific_headers.h>

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************
 * CONSTANTS
 **********************************************/

/**
 * @def MCUXCLELS_HW_VERSION
 * @ingroup mcuxClEls_Common
 * @brief Compatible ELS hardware IP version for the CLNS release that this header is part of.
 */
#ifndef MCUXCL_FEATURE_ELS_GET_FW_VERSION
#define MCUXCLELS_HW_VERSION ((mcuxClEls_HwVersion_t) { \
        .bits = { \
            .revision = (uint32_t) ELS_HW_VERSION_REVISION, \
            .minor = (uint32_t) ELS_HW_VERSION_MINOR, \
            .major = (uint32_t) ELS_HW_VERSION_MAJOR, \
            .level = (uint32_t) ELS_HW_VERSION_LEVEL \
        } \
    })
#else /* MCUXCL_FEATURE_ELS_GET_FW_VERSION */
#define MCUXCLELS_HW_VERSION ((mcuxClEls_HwVersion_t) { \
        .bits = { \
            .revision = (uint32_t) ELS_HW_VERSION_REVISION, \
            .minor = (uint32_t) ELS_HW_VERSION_MINOR, \
            .major = (uint32_t) ELS_HW_VERSION_MAJOR, \
            .fw_revision = (uint32_t) ELS_HW_VERSION_FW_REVISION, \
            .fw_minor = (uint32_t) ELS_HW_VERSION_FW_MINOR, \
            .fw_major = (uint32_t) ELS_HW_VERSION_FW_MAJOR \
        } \
    })
#endif /* MCUXCL_FEATURE_ELS_GET_FW_VERSION */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_INTERNAL_COMMON_H_ */

