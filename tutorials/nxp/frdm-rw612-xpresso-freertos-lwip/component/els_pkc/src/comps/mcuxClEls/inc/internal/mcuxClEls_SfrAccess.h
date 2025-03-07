/*--------------------------------------------------------------------------*/
/* Copyright 2022-2024 NXP                                                  */
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

/** @file  mcuxClEls_SfrAccess.h
 *  @brief Provide macros for mcuxClEls internal use.
 * This header declares internal macros to deduplicate code and support for internal use only. */

#ifndef MCUXCLELS_SFRACCESS_H_
#define MCUXCLELS_SFRACCESS_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <platform_specific_headers.h>
#include <stdint.h>
#include <stdbool.h>

#include <internal/mcuxClEls_Internal_mapping.h>

#ifdef __cplusplus
extern "C" {
#endif

/****                                ****/
/**** ELS Hardware Abstraction Layer ****/
/****                                ****/

/**
 * Definitions for accessing ELS SFRs via, e.g., IP_ELS->STATUS.
 */

/** Helper macros for constructing SFR field name constants */
#define MCUXCLELS_PASTE(a,b)  a ## b
#define MCUXCLELS_CONCAT(a,b) MCUXCLELS_PASTE(a,b)
#define MCUXCLELS_SFR_FIELD(prefix,sfr,field)        MCUXCLELS_CONCAT(prefix,MCUXCLELS_CONCAT(MCUXCLELS_CONCAT(sfr,_),field))

/** Helper macros to get the mask and shift values for a specific ELS SFR field */
#define MCUXCLELS_SFR_FIELD_MASK(sfr, field)         MCUXCLELS_CONCAT(MCUXCLELS_SFR_FIELD(ELS_SFR_PREFIX,sfr,field), _MASK)
#define MCUXCLELS_SFR_FIELD_SHIFT(sfr, field)        MCUXCLELS_CONCAT(MCUXCLELS_SFR_FIELD(ELS_SFR_PREFIX,sfr,field), _SHIFT)
#define MCUXCLELS_SFR_FIELD_FORMAT(sfr, field, val)  (MCUXCLELS_SFR_FIELD(ELS_SFR_PREFIX,sfr,field) (val))

/**********************************************************/
/* Helper macros for ELS SFR access                       */
/**********************************************************/

/** Read from ELS SFR */
#define MCUXCLELS_SFR_READ(sfr) \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_INTEGER_TO_POINTER("Converting address defined externally to SFR pointer") \
    (ELS_SFR_BASE->ELS_SFR_NAME(sfr)) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_INTEGER_TO_POINTER()

/** Write to ELS SFR */
#define MCUXCLELS_SFR_WRITE(sfr, value) \
    do{ \
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Converting address defined externally to SFR pointer") \
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_HARDWARE_ACCESS("Sfr offset from address") \
        ELS_SFR_BASE->ELS_SFR_NAME(sfr) = (value); \
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_HARDWARE_ACCESS() \
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER() \
    } while(false)

/** Read from ELS SFR bit field */
#define MCUXCLELS_SFR_BITREAD(sfr, bit)  \
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Converting address defined externally to SFR pointer") \
    ((ELS_SFR_BASE->ELS_SFR_NAME(sfr) & MCUXCLELS_SFR_FIELD_MASK(sfr, bit)) >> MCUXCLELS_SFR_FIELD_SHIFT(sfr, bit)) \
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER() \

/** Set bit field of ELS SFR (read-modify-write) */
#define MCUXCLELS_SFR_BITSET(sfr, bit)  \
    do{ ELS_SFR_BASE->ELS_SFR_NAME(sfr) |= MCUXCLELS_SFR_FIELD_MASK(sfr, bit); } while(false)

/** Clear bit field of ELS SFR (read-modify-write) */
#define MCUXCLELS_SFR_BITCLEAR(sfr, bit)  \
    do{ ELS_SFR_BASE->ELS_SFR_NAME(sfr) &= (~ (uint32_t) MCUXCLELS_SFR_FIELD_MASK(sfr, bit)); } while(false)

/** Set value of multi-bit field of ELS SFR (read-modify-write) */
#define MCUXCLELS_SFR_BITVALSET(sfr, bit, val)  \
    do{                                                                                                                             \
        uint32_t temp = ELS_SFR_BASE->ELS_SFR_NAME(sfr) & (~ (uint32_t) MCUXCLELS_SFR_FIELD_MASK(sfr, bit));                         \
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_HARDWARE_ACCESS("Sfr offset from address")                                                \
        ELS_SFR_BASE->ELS_SFR_NAME(sfr) = temp | ((val) << MCUXCLELS_SFR_FIELD_SHIFT(sfr, bit)) & MCUXCLELS_SFR_FIELD_MASK(sfr, bit); \
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_HARDWARE_ACCESS()                                                                          \
    } while(false)

/**** ------------------------------ ****/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_SFRACCESS_H_ */
