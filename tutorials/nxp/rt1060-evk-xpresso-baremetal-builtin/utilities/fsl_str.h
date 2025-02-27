/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _FSL_STR_H
#define _FSL_STR_H

#include "fsl_common.h"

/*!
 * @addtogroup debugconsole
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Definition to printf the float number. */
#ifndef PRINTF_FLOAT_ENABLE
#define PRINTF_FLOAT_ENABLE 0U
#endif /* PRINTF_FLOAT_ENABLE */

/*! @brief Definition to scanf the float number. */
#ifndef SCANF_FLOAT_ENABLE
#define SCANF_FLOAT_ENABLE 0U
#endif /* SCANF_FLOAT_ENABLE */

/*! @brief Definition to support advanced format specifier for printf. */
#ifndef PRINTF_ADVANCED_ENABLE
#define PRINTF_ADVANCED_ENABLE 0U
#endif /* PRINTF_ADVANCED_ENABLE */

/*! @brief Definition to support advanced format specifier for scanf. */
#ifndef SCANF_ADVANCED_ENABLE
#define SCANF_ADVANCED_ENABLE 0U
#endif /* SCANF_ADVANCED_ENABLE */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
/*! @brief Specification modifier flags for printf. */
enum _debugconsole_printf_flag
{
    kPRINTF_Minus             = 0x01U,  /*!< Minus FLag. */
    kPRINTF_Plus              = 0x02U,  /*!< Plus Flag. */
    kPRINTF_Space             = 0x04U,  /*!< Space Flag. */
    kPRINTF_Zero              = 0x08U,  /*!< Zero Flag. */
    kPRINTF_Pound             = 0x10U,  /*!< Pound Flag. */
    kPRINTF_LengthChar        = 0x20U,  /*!< Length: Char Flag. */
    kPRINTF_LengthShortInt    = 0x40U,  /*!< Length: Short Int Flag. */
    kPRINTF_LengthLongInt     = 0x80U,  /*!< Length: Long Int Flag. */
    kPRINTF_LengthLongLongInt = 0x100U, /*!< Length: Long Long Int Flag. */
};
#endif /* PRINTF_ADVANCED_ENABLE */

/*! @brief Specification modifier flags for scanf. */
enum _debugconsole_scanf_flag
{
    kSCANF_Suppress   = 0x2U,    /*!< Suppress Flag. */
    kSCANF_DestMask   = 0x7cU,   /*!< Destination Mask. */
    kSCANF_DestChar   = 0x4U,    /*!< Destination Char Flag. */
    kSCANF_DestString = 0x8U,    /*!< Destination String FLag. */
    kSCANF_DestSet    = 0x10U,   /*!< Destination Set Flag. */
    kSCANF_DestInt    = 0x20U,   /*!< Destination Int Flag. */
    kSCANF_DestFloat  = 0x30U,   /*!< Destination Float Flag. */
    kSCANF_LengthMask = 0x1f00U, /*!< Length Mask Flag. */
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
    kSCANF_LengthChar        = 0x100U, /*!< Length Char Flag. */
    kSCANF_LengthShortInt    = 0x200U, /*!< Length ShortInt Flag. */
    kSCANF_LengthLongInt     = 0x400U, /*!< Length LongInt Flag. */
    kSCANF_LengthLongLongInt = 0x800U, /*!< Length LongLongInt Flag. */
#endif                                 /* SCANF_ADVANCED_ENABLE */
#if (defined(SCANF_FLOAT_ENABLE) && (SCANF_FLOAT_ENABLE > 0))
    kSCANF_LengthLongLongDouble = 0x1000U, /*!< Length LongLongDuoble Flag. */
#endif                                     /*PRINTF_FLOAT_ENABLE */
    kSCANF_TypeSinged = 0x2000U,           /*!< TypeSinged Flag. */
};

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief A function pointer which is used when format printf log.
 */
typedef void (*printfCb)(char *buf, int32_t *indicator, char val, int len);

/*!
 * @brief This function outputs its parameters according to a formatted string.
 *
 * @note I/O is performed by calling given function pointer using following
 * (*func_ptr)(c);
 *
 * @param[in] fmt   Format string for printf.
 * @param[in] ap  Arguments to printf.
 * @param[in] buf  pointer to the buffer
 * @param cb print callbck function pointer
 *
 * @return Number of characters to be print
 */
int StrFormatPrintf(const char *fmt, va_list ap, char *buf, printfCb cb);

/*!
 * @brief Converts an input line of ASCII characters based upon a provided
 * string format.
 *
 * @param[in] line_ptr The input line of ASCII data.
 * @param[in] format   Format first points to the format string.
 * @param[in] args_ptr The list of parameters.
 *
 * @return Number of input items converted and assigned.
 * @retval IO_EOF When line_ptr is empty string "".
 */
int StrFormatScanf(const char *line_ptr, char *format, va_list args_ptr);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /* _FSL_STR_H */
