/*--------------------------------------------------------------------------*/
/* Copyright 2020-2021 NXP                                                  */
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

/** @file  mcuxClMemory_Endianness.h
 *  @brief Memory header for endianness support functions.
 * This header exposes macros that enable using endianness support functions.
 */

/**
 * @defgroup mcuxClMemory_Endianness mcuxClMemory_Endianness
 * @brief These macros implement endianess management on integers
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_ENDIANNESS_H_
#define MCUXCLMEMORY_ENDIANNESS_H_

#include <mcuxClMemory_Types.h>
#include <mcuxClConfig.h> // Exported features flags header


/**********************************************
 * MACROS
 **********************************************/

/**
 * @brief Converts a 32-bit unsigned integer to a little-endian order @c uint8_t array .
 *
 * @note Implementation is platform independent.
 *
 * @param[out]      destination     pointer to a 4 byte buffer were 32-bit integer in little-endian will be encoded.
 * @param[in]       value           pointer to the 32-bit integer to be encoded.
 *
 */
#define mcuxClMemory_StoreLittleEndian32( destination, value )                       \
do                                                                                  \
{                                                                                   \
    uint32_t local_value = (uint32_t)(value);                                       \
    ((uint8_t*)(destination))[0] = (uint8_t) (((local_value) & 0x000000FFU) >>  0u);\
    ((uint8_t*)(destination))[1] = (uint8_t) (((local_value) & 0x0000FF00U) >>  8u);\
    ((uint8_t*)(destination))[2] = (uint8_t) (((local_value) & 0x00FF0000U) >> 16u);\
    ((uint8_t*)(destination))[3] = (uint8_t) (((local_value) & 0xFF000000U) >> 24u);\
} while (false)

/**
 * @brief Converts a 32-bit unsigned integer to a big-endian order @c uint8_t array.
 *
 * @note Implementation is platform independent.
 *
 * @param[in]       source          pointer to a 4 byte big-endian order @c uint8_t buffer that will be converted to an unsigned integer
 *
 */
#define mcuxClMemory_StoreBigEndian32( destination, value )                          \
do                                                                                  \
{                                                                                   \
    uint32_t local_value = (uint32_t)(value);                                       \
    ((uint8_t*)(destination))[0] = (uint8_t) (((local_value) & 0xFF000000U) >> 24u);\
    ((uint8_t*)(destination))[1] = (uint8_t) (((local_value) & 0x00FF0000U) >> 16u);\
    ((uint8_t*)(destination))[2] = (uint8_t) (((local_value) & 0x0000FF00U) >>  8u);\
    ((uint8_t*)(destination))[3] = (uint8_t) (((local_value) & 0x000000FFU) >>  0u);\
} while (false)

/**
 * @brief Converts a little-endian order @c uint8_t array to a 32-bit unsigned integer.
 *
 * @note Implementation is platform independent.
 *
 * @param[in]       source          pointer to a 4 byte little-endian order @c uint8_t buffer that will be converted to an unsigned integer
 *
 */
#define mcuxClMemory_LoadLittleEndian32( source )        \
    ( (((uint32_t) ((const uint8_t*)(source))[0]) <<  0u) |   \
      (((uint32_t) ((const uint8_t*)(source))[1]) <<  8u) |   \
      (((uint32_t) ((const uint8_t*)(source))[2]) << 16u) |   \
      (((uint32_t) ((const uint8_t*)(source))[3]) << 24u) )


/**
 * @brief Converts a big-endian order @c uint8_t array to a 32-bit unsigned integer.
 *
 * @param[in]       destination     pointer to a 4 byte buffer were 32-bit integer in big-endian will be decoded.
 *
 * @return a 32-bit unsigned integer
 */
#define mcuxClMemory_LoadBigEndian32( source )           \
    ( (((uint32_t) ((const uint8_t*)(source))[0]) << 24u) |   \
      (((uint32_t) ((const uint8_t*)(source))[1]) << 16u) |   \
      (((uint32_t) ((const uint8_t*)(source))[2]) <<  8u) |   \
      (((uint32_t) ((const uint8_t*)(source))[3]) <<  0u) )

/**
 * @brief MACRO that switches byte endianness of given CPU word.
 *
 * @param[in]       input           a 32-bit unsigned integer whose endianness will be reversed.
 *
 */

#ifdef __REV
#define MCUXCLMEMORY_SWITCH_4BYTE_ENDIANNESS(input)  __REV(input)
#else
#define MCUXCLMEMORY_SWITCH_4BYTE_ENDIANNESS(input)  ((((input) & 0xffu) << 24u) | (((input) & 0xff00u) << 8u) | (((input) & 0xff0000u) >> 8u) | (((input) & 0xff000000u) >> 24u))
#endif

#endif /* MCUXCLMEMORY_ENDIANNESS_H_ */
/**
 * @}
 */
