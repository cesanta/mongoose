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
 * \file  mcuxClBuffer_Internal_Pointer.h
 * \brief Provides the internal API & implementation for the plain C pointer buffer types.
 */

#ifndef MCUXCLBUFFER_INTERNAL_POINTER_H_
#define MCUXCLBUFFER_INTERNAL_POINTER_H_

#include <mcuxClToolchain.h>
#include <mcuxClCore_Platform.h>

#include <mcuxClBuffer.h>
#include <mcuxClBuffer_Pointer.h>

#include <mcuxCsslDataIntegrity.h>
#include <mcuxCsslMemory.h>

#include <mcuxClMemory_Copy.h>
#include <mcuxClMemory_Copy_Reversed.h>



#ifdef __cplusplus
extern "C" {
#endif


/**
 * \defgroup clBufferUsage Buffer read/write functionality
 * \brief Buffer read/write functionality.
 * \ingroup mcuxClBuffer
 */

/* TODO CLNS-10260: finalize the implementation and especially update all the memory functions that are called from here, once CLNS-9401 is finished.
 * Appropriate memory functions should be called, to properly handle the security and the different cases of length/alignment (e.g. length being a multiple of 4 or power of 2).
 */

/**
 * \brief Writes the pointer of \p bufSrc plus the \p offset in \p ppDest.
 * 
 * \param bufSrc      Input buffer
 * \param offset      Offset of the input buffer
 * \param bufCpuWa    Not used
 * \param ppDest      Pointer to the address where the result is written to
 * \param byteLength  Not used
 * 
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_inputBufferToCPU)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_inputBufferToCPU(mcuxCl_InputBuffer_t bufSrc, uint32_t offset, uint8_t *bufCpuWa UNUSED_PARAM, const uint8_t **ppDest, uint32_t byteLength UNUSED_PARAM)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_inputBufferToCPU);
  *ppDest = (const uint8_t *)bufSrc + offset;
  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_inputBufferToCPU, MCUXCLBUFFER_STATUS_OK);

}


/**
 * \brief Perform a read from the buffer
 *
 * \param bufSrc     Input buffer from which the data shall be read.
 * \param offset     Offset into the buffer at which the read operation shall start.
 * \param pDst       Pointer to the memory location where the data will be stored.
 * \param byteLength Amount of bytes that will be read.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_read)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_read(mcuxCl_InputBuffer_t bufSrc, uint32_t offset, uint8_t *pDst, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_read);

  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW()
  MCUX_CSSL_DI_EXPUNGE(memCpyParams, ((uint32_t) pDst) + ((uint32_t) bufSrc) + byteLength + offset);
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()
  MCUXCLMEMORY_FP_MEMORY_COPY(pDst, &bufSrc[offset], byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_read, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy)
      );
}

/**
 * \brief Perform a word-wise read from the buffer
 *
 * \param bufSrc     Input buffer from which the data shall be read.
 * \param offset     Offset into the buffer at which the read operation shall start.
 * \param pDst       Pointer to the memory location where the data will be stored.
 * \param byteLength Amount of bytes that will be read.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_read_word)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_read_word(mcuxCl_InputBuffer_t bufSrc, uint32_t offset, uint8_t *pDst, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_read_word);

  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW()
  MCUX_CSSL_DI_EXPUNGE(memCpyParams, ((uint32_t) pDst) + ((uint32_t) bufSrc) + byteLength + offset);
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()
  MCUXCLMEMORY_FP_MEMORY_COPY(pDst, &bufSrc[offset], byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_read_word, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy)
      );
}

/**
 * \brief Perform an read with endianess reversal from the buffer
 *
 * \param bufSrc     Input buffer from which the data shall be read.
 * \param offset     Offset into the buffer at which the read operation shall start.
 * \param pDst       Pointer to the memory location where the data will be stored.
 * \param byteLength Amount of bytes that will be read.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_read_reverse)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_read_reverse(mcuxCl_InputBuffer_t bufSrc, uint32_t offset, uint8_t *pDst, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_read_reverse);

  MCUXCLMEMORY_FP_MEMORY_COPY_REVERSED(pDst, &bufSrc[offset], byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_read_reverse, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy_reversed));
}

/**
 * \brief Perform a secure read from the buffer
 *
 * \param bufSrc     Input buffer from which the data shall be read.
 * \param offset     Offset into the buffer at which the read operation shall start.
 * \param pDst       Pointer to the memory location where the data will be stored.
 * \param byteLength Amount of bytes that will be read.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_read_secure)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_read_secure(mcuxCl_InputBuffer_t bufSrc, uint32_t offset, uint8_t *pDst, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_read_secure);

  MCUXCLMEMORY_FP_MEMORY_COPY(pDst, &bufSrc[offset], byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_read_secure, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy));
}

/**
 * \brief Perform a secure read with endianess reversal from the buffer
 *
 * \param bufSrc     Input buffer from which the data shall be read.
 * \param offset     Offset into the buffer at which the read operation shall start.
 * \param pDst       Pointer to the memory location where the data will be stored.
 * \param byteLength Amount of bytes that will be read.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_read_secure_reverse)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_read_secure_reverse(mcuxCl_InputBuffer_t bufSrc, uint32_t offset, uint8_t *pDst, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_read_secure_reverse);

  MCUXCLMEMORY_FP_MEMORY_COPY_REVERSED(pDst, &bufSrc[offset], byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_read_secure_reverse, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy_reversed));
}


/**
 * \brief Perform a write to the buffer
 *
 * \param bufDst     Output buffer to which the data shall be written.
 * \param offset     Offset into the buffer at which the write operation shall start.
 * \param pSrc       Pointer to the memory location from where the data will be read.
 * \param byteLength Amount of bytes that will be written.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_write)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_write(mcuxCl_Buffer_t bufDst, uint32_t offset, const uint8_t *pSrc, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_write);

  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW()
  MCUX_CSSL_DI_EXPUNGE(memCpyParams, ((uint32_t) pSrc) + ((uint32_t) bufDst) + byteLength + offset);
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()
  MCUXCLMEMORY_FP_MEMORY_COPY(&bufDst[offset], pSrc, byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_write, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy)
      );
}

/**
 * \brief Perform a word-wise write to the buffer
 *
 * \param bufDst     Output buffer to which the data shall be written.
 * \param offset     Offset into the buffer at which the write operation shall start.
 * \param pSrc       Pointer to the memory location from where the data will be read.
 * \param byteLength Amount of bytes that will be written.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_write_word)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_write_word(mcuxCl_Buffer_t bufDst, uint32_t offset, const uint8_t *pSrc, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_write_word);

  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW()
  MCUX_CSSL_DI_EXPUNGE(memCpyParams, ((uint32_t) pSrc) + ((uint32_t) bufDst) + byteLength + offset);
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()
  MCUXCLMEMORY_FP_MEMORY_COPY(&bufDst[offset], pSrc, byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_write_word, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy)
      );
}


/**
 * \brief Perform a write with endianess reversal to the buffer
 *
 * \param bufDst     Output buffer to which the data shall be written.
 * \param offset     Offset into the buffer at which the write operation shall start.
 * \param pSrc       Pointer to the memory location from where the data will be read.
 * \param byteLength Amount of bytes that will be written.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_write_reverse)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_write_reverse(mcuxCl_Buffer_t bufDst, uint32_t offset, const uint8_t *pSrc, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_write_reverse);

  MCUXCLMEMORY_FP_MEMORY_COPY_REVERSED(&bufDst[offset], pSrc, byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_write_reverse, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy_reversed));
}

/**
 * \brief Perform a secure write to the buffer
 *
 * \param bufDst     Output buffer to which the data shall be written.
 * \param offset     Offset into the buffer at which the write operation shall start.
 * \param pSrc       Pointer to the memory location from where the data will be read.
 * \param byteLength Amount of bytes that will be written.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_write_secure)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_write_secure(mcuxCl_Buffer_t bufDst, uint32_t offset, const uint8_t *pSrc, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_write_secure);

  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW()
  MCUX_CSSL_DI_EXPUNGE(memCpyParams, ((uint32_t) pSrc) + ((uint32_t) bufDst) + byteLength + offset);
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

  MCUXCLMEMORY_FP_MEMORY_COPY(&bufDst[offset], pSrc, byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_write_secure, MCUXCLBUFFER_STATUS_OK,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy));
}

/**
 * \brief Perform a secure write with endianess reversal to the buffer
 *
 * \param bufDst     Output buffer to which the data shall be written.
 * \param offset     Offset into the buffer at which the write operation shall start.
 * \param pSrc       Pointer to the memory location from where the data will be read.
 * \param byteLength Amount of bytes that will be written.
 * \return Status of the operation.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_write_secure_reverse)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_write_secure_reverse(mcuxCl_Buffer_t bufDst, uint32_t offset, const uint8_t *pSrc, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_write_secure_reverse);

  MCUXCLMEMORY_FP_MEMORY_COPY_REVERSED(&bufDst[offset], pSrc, byteLength);

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_write_secure_reverse, MCUXCLBUFFER_STATUS_OK, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy_reversed));
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLBUFFER_INTERNAL_POINTER_H_ */
