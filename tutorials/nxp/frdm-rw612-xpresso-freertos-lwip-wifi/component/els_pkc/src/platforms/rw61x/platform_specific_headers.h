 /*--------------------------------------------------------------------------*/
 /* Copyright 2021 NXP                                                       */
 /*                                                                          */
 /* NXP Confidential. This software is owned or controlled by NXP and may    */
 /* only be used strictly in accordance with the applicable license terms.   */
 /* By expressly accepting such terms or by downloading, installing,         */
 /* activating and/or otherwise using the software, you are agreeing that    */
 /* you have read, and that you agree to comply with and are bound by, such  */
 /* license terms. If you do not agree to be bound by the applicable license */
 /* terms, then you may not retain, install, activate or otherwise use the   */
 /* software.                                                                */
 /*--------------------------------------------------------------------------*/
 
 #ifndef PLATFORM_SPECIFIC_HEADERS_H_ 
 #define PLATFORM_SPECIFIC_HEADERS_H_ 
 #pragma once
 
 #include "mcuxClConfig.h"
 #include "mcuxCsslAnalysis.h"
 
 MCUX_CSSL_ANALYSIS_START_PATTERN_EXTERNAL_HEADER()
 #include "fsl_device_registers.h" 
 #include "ip_platform.h" 
 #include "ip_css_constants.h" 
 #include "ip_css_design_configuration.h" 
 MCUX_CSSL_ANALYSIS_STOP_PATTERN_EXTERNAL_HEADER()
 
 #endif /*PLATFORM_SPECIFIC_HEADERS_H_*/ 
