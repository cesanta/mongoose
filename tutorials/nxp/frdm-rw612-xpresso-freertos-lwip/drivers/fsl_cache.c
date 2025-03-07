/*
 * Copyright 2016-2021, 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_cache.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.cache_cache64"
#endif

#if (FSL_FEATURE_SOC_CACHE64_CTRL_COUNT > 0)
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Array of CACHE64_CTRL peripheral base address. */
static CACHE64_CTRL_Type *const s_cache64ctrlBases[] = CACHE64_CTRL_BASE_PTRS;

#if (defined(FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT) && (FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT > 0))
/* Array of CACHE64_POLSEL peripheral base address. */
static CACHE64_POLSEL_Type *const s_cache64polselBases[] = CACHE64_POLSEL_BASE_PTRS;
#endif

#if (defined(CACHE64_CTRL_PHYMEM_BASE_ALIAS_COUNT))
#define CACHE64_PHYMEM_COLUM_COUNT CACHE64_CTRL_PHYMEM_BASE_ALIAS_COUNT
/* Array of CACHE64 physical memory base address, 
  it is a 2D array, the row indicate cache instance,
  the column indicate the alias of one instance.  */
static uint32_t const s_cache64PhymemBases[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT] = CACHE64_CTRL_PHYMEM_BASES;
/* Array of CACHE64 physical size base address, 
  it is a 2D array, the row indicate cache instance,
  the column indicate the alias of one instance.  */
static uint32_t const s_cache64PhymemSizes[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT] = CACHE64_CTRL_PHYMEM_SIZES;
#else
#define CACHE64_PHYMEM_COLUM_COUNT 1
static uint32_t const s_cache64PhymemBases[] = CACHE64_CTRL_PHYMEM_BASES;
static uint32_t const s_cache64PhymemSizes[] = CACHE64_CTRL_PHYMEM_SIZES;
#endif

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
#ifdef CACHE64_CLOCKS
/* Array of CACHE64_CTRL clock name. */
static const clock_ip_name_t s_cache64Clocks[] = CACHE64_CLOCKS;
#endif
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

volatile uint8_t g_cache64MemPhyAliasId = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (defined(FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT) && (FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT > 0))
/*!
 * brief Returns an instance number given peripheral base address.
 *
 * param base The peripheral base address.
 * return CACHE64_POLSEL instance number starting from 0.
 */
uint32_t CACHE64_GetInstance(CACHE64_POLSEL_Type *base)
{
    uint32_t i;

    for (i = 0; i < ARRAY_SIZE(s_cache64polselBases); i++)
    {
        if (MSDK_REG_SECURE_ADDR(base) == MSDK_REG_SECURE_ADDR(s_cache64polselBases[i]))
        {
            break;
        }
    }

    assert(i < ARRAY_SIZE(s_cache64polselBases));

    return i;
}
#endif

/*!
 * brief Returns an instance number given physical memory address.
 *
 * param address The physical memory address.
 * return CACHE64_CTRL instance number starting from 0.
 */
uint32_t CACHE64_GetInstanceByAddr(uint32_t address)
{
    uint32_t i = 0UL;
    uint32_t phyMemBase[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
    uint32_t phyMemSize[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
    memcpy(phyMemBase, s_cache64PhymemBases, sizeof(s_cache64PhymemBases));
    memcpy(phyMemSize, s_cache64PhymemSizes, sizeof(s_cache64PhymemSizes));

    while(i < ARRAY_SIZE(s_cache64ctrlBases))
    {
        g_cache64MemPhyAliasId = 0U;
        while(g_cache64MemPhyAliasId < CACHE64_PHYMEM_COLUM_COUNT)
        {
            if ((MSDK_REG_SECURE_ADDR(address) >= MSDK_REG_SECURE_ADDR(phyMemBase[i][g_cache64MemPhyAliasId])) && (MSDK_REG_SECURE_ADDR(address) < MSDK_REG_SECURE_ADDR(phyMemBase[i][g_cache64MemPhyAliasId] + phyMemSize[i][g_cache64MemPhyAliasId] - 0x01U)))
            {
                return i;
            }
            g_cache64MemPhyAliasId++;
        }
        i++;
    }
		
    return 0xFFFFFFFFUL;
}

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
status_t CACHE64_Init(CACHE64_POLSEL_Type *base, const cache64_config_t *config)
{
    volatile uint32_t *topReg = &base->REG0_TOP;
    uint32_t i;
    uint32_t polsel = 0;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
#ifdef CACHE64_CLOCKS
    uint32_t instance = CACHE64_GetInstance(base);

    /* Enable CACHE64 clock */
    CLOCK_EnableClock(s_cache64Clocks[instance]);
#endif
#endif

    for (i = 0; i < CACHE64_REGION_NUM - 1U; i++)
    {
        assert((config->boundaryAddr[i] & (CACHE64_REGION_ALIGNMENT - 1U)) == 0U);
        ((volatile uint32_t *)topReg)[i] = config->boundaryAddr[i] >= CACHE64_REGION_ALIGNMENT ?
                                               config->boundaryAddr[i] - CACHE64_REGION_ALIGNMENT :
                                               0U;
    }

    for (i = 0; i < CACHE64_REGION_NUM; i++)
    {
        polsel |= (((uint32_t)config->policy[i]) << (2U * i));
    }
    base->POLSEL = polsel;

    return kStatus_Success;
}

/*!
 * @brief Gets the default configuration structure.
 *
 * This function initializes the CACHE64 configuration structure to a default value. The default
 * values are first region covers whole cacheable area, and policy set to write back.
 *
 * @param config Pointer to a configuration structure.
 */
void CACHE64_GetDefaultConfig(cache64_config_t *config)
{
    uint32_t phyMemSize[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
    memcpy(phyMemSize, s_cache64PhymemSizes, sizeof(s_cache64PhymemSizes));
    (void)memset(config, 0, sizeof(cache64_config_t));

    config->boundaryAddr[0] = phyMemSize[0][g_cache64MemPhyAliasId];
    config->policy[0]       = kCACHE64_PolicyWriteBack;
}
#endif

/*!
 * brief Enables the cache.
 *
 */
void CACHE64_EnableCache(CACHE64_CTRL_Type *base)
{
    /* if CACHE is not enabled */
    if ((base->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) == 0x00U)
    {
        /* First, invalidate the entire cache. */
        CACHE64_InvalidateCache(base);

        /* Now enable the cache. */
        base->CCR |= CACHE64_CTRL_CCR_ENCACHE_MASK;
    }
}

/*!
 * brief Disables the cache.
 *
 */
void CACHE64_DisableCache(CACHE64_CTRL_Type *base)
{
    /* if CACHE is enabled */
    if ((base->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) != 0x00U)
    {
        /* First, push any modified contents. */
        CACHE64_CleanCache(base);

        /* Now disable the cache. */
        base->CCR &= ~CACHE64_CTRL_CCR_ENCACHE_MASK;
    }
}

/*!
 * brief Invalidates the cache.
 *
 */
void CACHE64_InvalidateCache(CACHE64_CTRL_Type *base)
{
    /* Invalidate all lines in both ways and initiate the cache command. */
    base->CCR |= CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK;

    /* Wait until the cache command completes. */
    while ((base->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->CCR &= ~(CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);
}

/*!
 * brief Invalidates cache by range.
 *
 * param address The physical address of cache.
 * param size_byte size of the memory to be invalidated, should be larger than 0.
 * note Address and size should be aligned to "L1CODCACHE_LINESIZE_BYTE".
 * The startAddr here will be forced to align to CACHE64_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void CACHE64_InvalidateCacheByRange(uint32_t address, uint32_t size_byte)
{
    if (size_byte > 0UL)
    {
        uint32_t endAddr = MSDK_REG_SECURE_ADDR(address + size_byte - 0x01U);
        uint32_t pccReg  = 0;
        /* Align address to cache line size. */
        uint32_t startAddr = MSDK_REG_SECURE_ADDR(address & ~((uint32_t)CACHE64_LINESIZE_BYTE - 1U));
        uint32_t instance  = CACHE64_GetInstanceByAddr(address);
        uint32_t endLim;
        CACHE64_CTRL_Type *base;
        uint32_t phyMemBase[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
        uint32_t phyMemSize[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
        memcpy(phyMemBase, s_cache64PhymemBases, sizeof(s_cache64PhymemBases));
        memcpy(phyMemSize, s_cache64PhymemSizes, sizeof(s_cache64PhymemSizes));

        if (instance >= ARRAY_SIZE(s_cache64ctrlBases))
        {
            return;
        }
        base    = s_cache64ctrlBases[instance];
        endLim  = MSDK_REG_SECURE_ADDR(phyMemBase[instance][g_cache64MemPhyAliasId] + phyMemSize[instance][g_cache64MemPhyAliasId] - 0x01U);
        endAddr = endAddr > endLim ? endLim : endAddr;

        /* Set the invalidate by line command and use the physical address. */
        pccReg = (base->CLCR & ~CACHE64_CTRL_CLCR_LCMD_MASK) | CACHE64_CTRL_CLCR_LCMD(1) | CACHE64_CTRL_CLCR_LADSEL_MASK;
        base->CLCR = pccReg;

        while (startAddr < endAddr)
        {
            /* Set the address and initiate the command. */
            base->CSAR = (startAddr & CACHE64_CTRL_CSAR_PHYADDR_MASK) | CACHE64_CTRL_CSAR_LGO_MASK;

            /* Wait until the cache command completes. */
            while ((base->CSAR & CACHE64_CTRL_CSAR_LGO_MASK) != 0x00U)
            {
            }
            startAddr += (uint32_t)CACHE64_LINESIZE_BYTE;
        }
    }
}

/*!
 * brief Cleans the cache.
 *
 */
void CACHE64_CleanCache(CACHE64_CTRL_Type *base)
{
    /* Enable the to push all modified lines. */
    base->CCR |= CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK | CACHE64_CTRL_CCR_GO_MASK;

    /* Wait until the cache command completes. */
    while ((base->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->CCR &= ~(CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK);
}

/*!
 * brief Cleans cache by range.
 *
 * param address The physical address of cache.
 * param size_byte size of the memory to be cleaned, should be larger than 0.
 * note Address and size should be aligned to "CACHE64_LINESIZE_BYTE".
 * The startAddr here will be forced to align to CACHE64_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void CACHE64_CleanCacheByRange(uint32_t address, uint32_t size_byte)
{
    if (size_byte > 0UL)
    {
        uint32_t endAddr = MSDK_REG_SECURE_ADDR(address + size_byte - 0x01U);
        uint32_t pccReg  = 0;
        /* Align address to cache line size. */
        uint32_t startAddr = MSDK_REG_SECURE_ADDR(address & ~((uint32_t)CACHE64_LINESIZE_BYTE - 1U));
        uint32_t instance  = CACHE64_GetInstanceByAddr(address);
        uint32_t endLim;
        CACHE64_CTRL_Type *base;
        uint32_t phyMemBase[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
        uint32_t phyMemSize[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
        memcpy(phyMemBase, s_cache64PhymemBases, sizeof(s_cache64PhymemBases));
        memcpy(phyMemSize, s_cache64PhymemSizes, sizeof(s_cache64PhymemSizes));

        if (instance >= ARRAY_SIZE(s_cache64ctrlBases))
        {
            return;
        }
        base    = s_cache64ctrlBases[instance];
        endLim  = MSDK_REG_SECURE_ADDR(phyMemBase[instance][g_cache64MemPhyAliasId] + phyMemSize[instance][g_cache64MemPhyAliasId] - 0x01U);
        endAddr = endAddr > endLim ? endLim : endAddr;

        /* Set the push by line command. */
        pccReg = (base->CLCR & ~CACHE64_CTRL_CLCR_LCMD_MASK) | CACHE64_CTRL_CLCR_LCMD(2) | CACHE64_CTRL_CLCR_LADSEL_MASK;
        base->CLCR = pccReg;

        while (startAddr < endAddr)
        {
            /* Set the address and initiate the command. */
            base->CSAR = (startAddr & CACHE64_CTRL_CSAR_PHYADDR_MASK) | CACHE64_CTRL_CSAR_LGO_MASK;

            /* Wait until the cache command completes. */
            while ((base->CSAR & CACHE64_CTRL_CSAR_LGO_MASK) != 0x00U)
            {
            }
            startAddr += (uint32_t)CACHE64_LINESIZE_BYTE;
        }
    }
}

/*!
 * brief Cleans and invalidates the cache.
 *
 */
void CACHE64_CleanInvalidateCache(CACHE64_CTRL_Type *base)
{
    /* Push and invalidate all. */
    base->CCR |= CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK | CACHE64_CTRL_CCR_INVW0_MASK |
                 CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK;

    /* Wait until the cache command completes. */
    while ((base->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->CCR &= ~(CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK | CACHE64_CTRL_CCR_INVW0_MASK |
                   CACHE64_CTRL_CCR_INVW1_MASK);
}

/*!
 * brief Cleans and invalidate cache by range.
 *
 * param address The physical address of cache.
 * param size_byte size of the memory to be Cleaned and Invalidated, should be larger than 0.
 * note Address and size should be aligned to "CACHE64_LINESIZE_BYTE".
 * The startAddr here will be forced to align to CACHE64_LINESIZE_BYTE if
 * startAddr is not aligned. For the size_byte, application should make sure the
 * alignment or make sure the right operation order if the size_byte is not aligned.
 */
void CACHE64_CleanInvalidateCacheByRange(uint32_t address, uint32_t size_byte)
{
    if (size_byte > 0UL)
    {
        uint32_t endAddr = MSDK_REG_SECURE_ADDR(address + size_byte - 0x01U);
        uint32_t pccReg  = 0;
        /* Align address to cache line size. */
        uint32_t startAddr = MSDK_REG_SECURE_ADDR(address & ~((uint32_t)CACHE64_LINESIZE_BYTE - 1U));
        uint32_t instance  = CACHE64_GetInstanceByAddr(address);
        uint32_t endLim;
        CACHE64_CTRL_Type *base;
        uint32_t phyMemBase[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
        uint32_t phyMemSize[FSL_FEATURE_SOC_CACHE64_CTRL_COUNT][CACHE64_PHYMEM_COLUM_COUNT];
        memcpy(phyMemBase, s_cache64PhymemBases, sizeof(s_cache64PhymemBases));
        memcpy(phyMemSize, s_cache64PhymemSizes, sizeof(s_cache64PhymemSizes));

        if (instance >= ARRAY_SIZE(s_cache64ctrlBases))
        {
            return;
        }
        base    = s_cache64ctrlBases[instance];
        endLim  = MSDK_REG_SECURE_ADDR(phyMemBase[instance][g_cache64MemPhyAliasId] + phyMemSize[instance][g_cache64MemPhyAliasId] - 0x01U);
        endAddr = endAddr > endLim ? endLim : endAddr;

        /* Set the push by line command. */
        pccReg = (base->CLCR & ~CACHE64_CTRL_CLCR_LCMD_MASK) | CACHE64_CTRL_CLCR_LCMD(3) | CACHE64_CTRL_CLCR_LADSEL_MASK;
        base->CLCR = pccReg;

        while (startAddr < endAddr)
        {
            /* Set the address and initiate the command. */
            base->CSAR = (startAddr & CACHE64_CTRL_CSAR_PHYADDR_MASK) | CACHE64_CTRL_CSAR_LGO_MASK;

            /* Wait until the cache command completes. */
            while ((base->CSAR & CACHE64_CTRL_CSAR_LGO_MASK) != 0x00U)
            {
            }
            startAddr += (uint32_t)CACHE64_LINESIZE_BYTE;
        }
    }
}

#if !(defined(FSL_FEATURE_CACHE64_CTRL_HAS_NO_WRITE_BUF) && FSL_FEATURE_CACHE64_CTRL_HAS_NO_WRITE_BUF)
/*!
 * brief Enable the cache write buffer.
 *
 */
void CACHE64_EnableWriteBuffer(CACHE64_CTRL_Type *base, bool enable)
{
    if (enable)
    {
        base->CCR |= CACHE64_CTRL_CCR_ENWRBUF_MASK;
    }
    else
    {
        base->CCR &= ~CACHE64_CTRL_CCR_ENWRBUF_MASK;
    }
}

#endif

#endif /* FSL_FEATURE_SOC_CACHE64_CTRL_COUNT > 0 */
