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

/**
 * \file  mcuxClBuffer_Pointer.h
 * \brief Provides the implementation for the basic C pointer buffer types.
 */

#ifndef MCUXCLBUFFER_POINTER_H_
#define MCUXCLBUFFER_POINTER_H_

#include <mcuxClCore_Platform.h>
#include <mcuxCsslFlowProtection.h>

/**
 * \addtogroup mcuxClAPI MCUX CL -- API
 *
 * \addtogroup mcuxClBuffer Buffer API
 * \brief Essential types and functionality.
 * \ingroup mcuxClAPI
 */

/**
 * \defgroup clBufferTypes Buffer type definitions
 * \brief Types used by the buffer operations.
 * \ingroup mcuxClBuffer
 */

/**
 * \brief Input buffer type
 * \ingroup clBufferTypes
 *
 * This type provides a pointer to the memory location that should be used to
 * read input data from.
 */
typedef const uint8_t * mcuxCl_InputBuffer_t;

/**
 * \brief Input/output buffer type
 * \ingroup clBufferTypes
 *
 * This type provides a pointer to the memory location that can be used for
 * both reading input data and writing output data.
 */
typedef uint8_t * mcuxCl_Buffer_t;

/**
 * \brief Perform a read from the buffer
 *
 * \param bufSrc     Input buffer from which the data shall be read.
 * \param offset     Offset into the buffer at which the read operation shall start.
 * \param pDst       Pointer to the memory location where the data will be stored.
 * \param byteLength Amount of bytes that will be read.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClBuffer_import)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_import(mcuxCl_InputBuffer_t bufSrc, uint32_t offset, uint8_t *pDst, uint32_t byteLength);

/**
 * \brief Perform a write to the buffer
 *
 * \param bufDst     Output buffer to which the data shall be written.
 * \param offset     Offset into the buffer at which the write operation shall start.
 * \param pSrc       Pointer to the memory location from where the data will be read.
 * \param byteLength Amount of bytes that will be written.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClBuffer_export)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_export(mcuxCl_Buffer_t bufDst, uint32_t offset, const uint8_t *pSrc, uint32_t byteLength);

/**
 * \def MCUXCLBUFFER_INIT_PLAIN_RO_IMPL
 * \brief Initialize an input buffer with plain CPU handling.
 * \ingroup clBufInit
 *
 * This macro can be used to initialize an input buffer that will be handled
 * using plain CPU operations.
 *
 * \param _name the name to be used for the buffer variable
 * \param _info unused for the current implementation
 * \param _ptr  pointer to the memory location of the buffer
 * \param _size unused for the current implementation
 */
#define MCUXCLBUFFER_INIT_PLAIN_RO_IMPL(_name, _info, _ptr, _size) \
  mcuxCl_InputBuffer_t (_name) = (mcuxCl_InputBuffer_t)(_ptr)

/**
 * \def MCUXCLBUFFER_INIT_PLAIN_RW_IMPL
 * \brief Initialize an input/output buffer with plain CPU handling.
 * \ingroup clBufInit
 *
 * This macro can be used to initialize an input/output buffer that will be
 * handled using plain CPU operations.
 *
 * \param _name the name to be used for the buffer variable
 * \param _info unused for the current implementation
 * \param _ptr  pointer to the memory location of the buffer
 * \param _size unused for the current implementation
 */
#define MCUXCLBUFFER_INIT_PLAIN_RW_IMPL(_name, _info, _ptr, _size) \
  mcuxCl_Buffer_t (_name) = (mcuxCl_Buffer_t)(_ptr)

/**
 * \def MCUXCLBUFFER_INIT_DMA_RO_IMPL
 * \brief DMA handling is not supported. Initialize an input buffer with plain CPU handling.
 * \ingroup clBufInit
 *
 * This buffer implementation does not support DMA handling, so CPU handling is used as a fallback.
 * This macro is an alias for \p MCUXCLBUFFER_INIT_PLAIN_RO_IMPL.
 *
 * \param _name the name to be used for the buffer variable
 * \param _info unused for the current implementation
 * \param _ptr  pointer to the memory location of the buffer
 * \param _size unused for the current implementation
 */
#define MCUXCLBUFFER_INIT_DMA_RO_IMPL(_name, _info, _ptr, _size) \
  MCUXCLBUFFER_INIT_PLAIN_RO_IMPL(_name, _info, _ptr, _size)

/**
 * \def MCUXCLBUFFER_INIT_DMA_RW_IMPL
 * \brief DMA handling is not supported. Initialize an input/output buffer with plain CPU handling.
 * \ingroup clBufInit
 *
 * This buffer implementation does not support DMA handling, so CPU handling is used as a fallback.
 * This macro is an alias for \p MCUXCLBUFFER_INIT_PLAIN_RW_IMPL.
 *
 * \param _name the name to be used for the buffer variable
 * \param _info unused for the current implementation
 * \param _ptr  pointer to the memory location of the buffer
 * \param _size unused for the current implementation
 */
#define MCUXCLBUFFER_INIT_DMA_RW_IMPL(_name, _info, _ptr, _size) \
  MCUXCLBUFFER_INIT_PLAIN_RW_IMPL(_name, _info, _ptr, _size)

/**
 * \def MCUXCLBUFFER_INIT_CUSTOM_IMPL
 * \brief Usage of custom handler is not supported. Initialize an input/output buffer with plain CPU handling.
 * \ingroup clBufInit
 *
 * This macro is an alias for \p MCUXCLBUFFER_INIT_PLAIN_RW_IMPL.
 *
 * \param _name    the name to be used for the buffer variable
 * \param _handler unused for the current implementation
 * \param _info    unused for the current implementation
 * \param _spec    unused for the current implementation
 * \param _ptr     pointer to the memory location of the buffer
 * \param _size    unused for the current implementation
 */
#define MCUXCLBUFFER_INIT_CUSTOM_IMPL(_name, _handler, _info, _spec, _ptr, _size) \
    MCUXCLBUFFER_INIT_PLAIN_RW_IMPL(_name, _info, _ptr, _size)


/**
 * \def MCUXCLBUFFER_UPDATE_IMPL
 * \brief Update the buffer pointer with the given offset.
 * \ingroup clBufUpdate
 *
 * \param _name   the name of the buffer variable to update
 * \param _offset the amount of bytes that the buffer pointer should be moved.
 */
#define MCUXCLBUFFER_UPDATE_IMPL(_name, _offset) \
  do { (_name) += (_offset); } while(false)

/**
 * \def MCUXCLBUFFER_DERIVE_RO_IMPL
 * \brief Derive a new input buffer object from an existing one (updated with the given offset).
 * \ingroup clBufUpdate
 *
 * \param _name     the name of the buffer variable to create.
 * \param _original the name of the buffer variable that will be cloned.
 * \param _offset   the amount of bytes that the buffer pointer should be moved.
 */
#define MCUXCLBUFFER_DERIVE_RO_IMPL(_name, _original, _offset) \
  mcuxCl_InputBuffer_t (_name) = (mcuxCl_InputBuffer_t) &(_original)[_offset]

/**
 * \def MCUXCLBUFFER_DERIVE_RW_IMPL
 * \brief Derive a new input/output buffer object from an existing one (updated with the given offset).
 * \ingroup clBufUpdate
 *
 * \param _name     the name of the buffer variable to create.
 * \param _original the name of the buffer variable that will be cloned.
 * \param _offset   the amount of bytes that the buffer pointer should be moved.
 */
#define MCUXCLBUFFER_DERIVE_RW_IMPL(_name, _original, _offset) \
  mcuxCl_Buffer_t (_name) = (mcuxCl_Buffer_t) &(_original)[_offset]

/**
 * \def MCUXCLBUFFER_SET_IMPL
 * \brief Update the buffer pointer the a new memory location.
 * \ingroup clBufUpdate
 *
 * \param _name the name of the buffer variable to update
 * \param _ptr  pointer to the memory location of the buffer
 * \param _size unused for the current implementation
 */
#define MCUXCLBUFFER_SET_IMPL(_name, _ptr, _size) \
  do { (_name) = (_ptr); } while(false)

/**
 * \def MCUXCLBUFFER_GET_IMPL
 * \brief Get the pointer of the buffer
 * \ingroup clBufUpdate
 *
 * \param _name the name of the buffer variable
 */
#define MCUXCLBUFFER_GET_IMPL(_name) \
  (_name)

/**
 * \def MCUXCLBUFFER_IS_NULL_IMPL
 * \brief Check the buffer pointer against NULL
 * \ingroup clBufUpdate
 *
 * \param _name the name of the buffer variable
 */
#define MCUXCLBUFFER_IS_NULL_IMPL(_name) \
  (NULL == MCUXCLBUFFER_GET_IMPL(_name))


#endif /* MCUXCLBUFFER_POINTER_H_ */
