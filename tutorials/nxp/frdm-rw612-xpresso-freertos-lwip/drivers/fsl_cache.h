/*
 * Copyright 2016-2021, 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_CACHE_H_
#define FSL_CACHE_H_

#include "fsl_common.h"

/*!
 * @addtogroup cache64
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*! @{ */
/*! @brief cache driver version. */
#define FSL_CACHE_DRIVER_VERSION (MAKE_VERSION(2, 0, 9))
/*! @} */

/*! @brief cache line size. */
#define CACHE64_LINESIZE_BYTE (FSL_FEATURE_CACHE64_CTRL_LINESIZE_BYTE)

#if (defined(FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT) && (FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT > 0))
/*! @brief cache region number. */
#define CACHE64_REGION_NUM (3U)
/*! @brief cache region alignment. */
#define CACHE64_REGION_ALIGNMENT (0x400U)

/*! @brief Level 2 cache controller way size. */
typedef enum _cache64_policy
{
    kCACHE64_PolicyNonCacheable = 0, /*!< Non-cacheable */
    kCACHE64_PolicyWriteThrough = 1, /*!< Write through */
    kCACHE64_PolicyWriteBack    = 2, /*!< Write back */
} cache64_policy_t;

/*! @brief CACHE64 configuration structure. */
typedef struct _cache64_config
{
    /*!< The cache controller can divide whole memory into 3 regions.
     * Boundary address is the FlexSPI internal address (start from 0) instead of system
     * address (start from FlexSPI AMBA base) to split adjacent regions and must be 1KB
     * aligned. The boundary address itself locates in upper region. */
    uint32_t boundaryAddr[CACHE64_REGION_NUM - 1];
    /*!< Cacheable policy for each region. */
    cache64_policy_t policy[CACHE64_REGION_NUM];
} cache64_config_t;
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name cache control for cache64
 *@{
 */

#if (defined(FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT) && (FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT > 0))
/*!
 * @brief Returns an instance number given peripheral base address.
 *
 * @param base The peripheral base address.
 * @return CACHE64_POLSEL instance number starting from 0.
 */
uint32_t CACHE64_GetInstance(CACHE64_POLSEL_Type *base);
#endif

/*!
 * brief Returns an instance number given physical memory address.
 *
 * param address The physical memory address.
 * @return CACHE64_CTRL instance number starting from 0.
 */
uint32_t CACHE64_GetInstanceByAddr(uint32_t address);

#if (defined(FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT) && (FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT > 0))
/*!
 * @brief Initializes an CACHE64 instance with the user configuration structure.
 *
 * This function configures the CACHE64 module with user-defined settings. Call the CACHE64_GetDefaultConfig() function
 * to configure the configuration structure and get the default configuration.
 *
 * @param base CACHE64_POLSEL peripheral base address.
 * @param config Pointer to a user-defined configuration structure.
 * @retval kStatus_Success CACHE64 initialize succeed
 */
status_t CACHE64_Init(CACHE64_POLSEL_Type *base, const cache64_config_t *config);

/*!
 * @brief Gets the default configuration structure.
 *
 * This function initializes the CACHE64 configuration structure to a default value. The default
 * values are first region covers whole cacheable area, and policy set to write back.
 *
 * @param config Pointer to a configuration structure.
 */
void CACHE64_GetDefaultConfig(cache64_config_t *config);
#endif

/*!
 * @brief Enables the cache.
 *
 * @param base CACHE64_CTRL peripheral base address.
 *
 */
void CACHE64_EnableCache(CACHE64_CTRL_Type *base);

/*!
 * @brief Disables the cache.
 *
 * @param base CACHE64_CTRL peripheral base address.
 *
 */
void CACHE64_DisableCache(CACHE64_CTRL_Type *base);

/*!
 * @brief Invalidates the cache.
 *
 * @param base CACHE64_CTRL peripheral base address.
 *
 */
void CACHE64_InvalidateCache(CACHE64_CTRL_Type *base);

/*!
 * @brief Invalidates cache by range.
 *
 * @param address The physical address of cache.
 * @param size_byte size of the memory to be invalidated, should be larger than 0.
 * @note Address and size should be aligned to "CACHE64_LINESIZE_BYTE".
 * The startAddr here will be forced to align to CACHE64_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void CACHE64_InvalidateCacheByRange(uint32_t address, uint32_t size_byte);

/*!
 * @brief Cleans the cache.
 *
 * @param base CACHE64_CTRL peripheral base address.
 *
 */
void CACHE64_CleanCache(CACHE64_CTRL_Type *base);

/*!
 * @brief Cleans cache by range.
 *
 * @param address The physical address of cache.
 * @param size_byte size of the memory to be cleaned, should be larger than 0.
 * @note Address and size should be aligned to "CACHE64_LINESIZE_BYTE".
 * The startAddr here will be forced to align to CACHE64_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void CACHE64_CleanCacheByRange(uint32_t address, uint32_t size_byte);

/*!
 * @brief Cleans and invalidates the cache.
 *
 * @param base CACHE64_CTRL peripheral base address.
 *
 */
void CACHE64_CleanInvalidateCache(CACHE64_CTRL_Type *base);

/*!
 * @brief Cleans and invalidate cache by range.
 *
 * @param address The physical address of cache.
 * @param size_byte size of the memory to be Cleaned and Invalidated, should be larger than 0.
 * @note Address and size should be aligned to "CACHE64_LINESIZE_BYTE".
 * The startAddr here will be forced to align to CACHE64_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void CACHE64_CleanInvalidateCacheByRange(uint32_t address, uint32_t size_byte);

#if !(defined(FSL_FEATURE_CACHE64_CTRL_HAS_NO_WRITE_BUF) && FSL_FEATURE_CACHE64_CTRL_HAS_NO_WRITE_BUF)
/*!
 * @brief Enables/disables the write buffer.
 *
 * @param base CACHE64_CTRL peripheral base address.
 * @param enable The enable or disable flag.
 *       true  - enable the write buffer.
 *       false - disable the write buffer.
 */
void CACHE64_EnableWriteBuffer(CACHE64_CTRL_Type *base, bool enable);
#endif

/*! @} */

/*!
 * @name Unified Cache Control for all caches
 *@{
 */

/*!
 * @brief Invalidates instruction cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be invalidated, should be larger than 0.
 * @note Address and size should be aligned to CACHE64_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_CACHE64_CTRL_LINESIZE_BYTE. The startAddr here will be forced to align to the cache line
 * size if startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
static inline void ICACHE_InvalidateByRange(uint32_t address, uint32_t size_byte)
{
    CACHE64_InvalidateCacheByRange(address, size_byte);
}

/*!
 * @brief Invalidates data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be invalidated, should be larger than 0.
 * @note Address and size should be aligned to CACHE64_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_CACHE64_CTRL_LINESIZE_BYTE. The startAddr here will be forced to align to the cache line
 * size if startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
static inline void DCACHE_InvalidateByRange(uint32_t address, uint32_t size_byte)
{
    CACHE64_InvalidateCacheByRange(address, size_byte);
}

/*!
 * @brief Clean data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be cleaned, should be larger than 0.
 * @note Address and size should be aligned to CACHE64_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_CACHE64_CTRL_LINESIZE_BYTE. The startAddr here will be forced to align to the cache line
 * size if startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
static inline void DCACHE_CleanByRange(uint32_t address, uint32_t size_byte)
{
    CACHE64_CleanCacheByRange(address, size_byte);
}

/*!
 * @brief Cleans and Invalidates data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be Cleaned and Invalidated, should be larger than 0.
 * @note Address and size should be aligned to CACHE64_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_CACHE64_CTRL_LINESIZE_BYTE. The startAddr here will be forced to align to the cache line
 * size if startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
static inline void DCACHE_CleanInvalidateByRange(uint32_t address, uint32_t size_byte)
{
    CACHE64_CleanInvalidateCacheByRange(address, size_byte);
}

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FSL_CACHE_H_*/
