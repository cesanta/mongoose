/*
 *     Copyright 2023 NXP
 *      
 *
 *     SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_iped.h"

status_t IPED_SetRegionAddressRange(FLEXSPI_Type *base,
                                    iped_region_t region,
                                    uint32_t start_address,
                                    uint32_t end_address)
{
    /* Check if region is not locked */
    if (IPED_IsRegionLocked(base, region))
    {
        return (status_t)kStatus_IPED_RegionIsLocked;
    }

    /* Disable soft lock for given region first */
    base->IPEDCTXCTRL[0] = (base->IPEDCTXCTRL[0] & ~(FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_MASK << (region * 2UL))) |
                          (IPED_RW_ENABLE_VAL << (region * 2UL));

    __IO uint32_t *reg_start = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0START)) + (IPED_CTX_REG_OFFSET * region));
    __IO uint32_t *reg_end   = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0END)) + (IPED_CTX_REG_OFFSET * region));

    *reg_start = (*reg_start & (~FLEXSPI_IPEDCTX0START_START_ADDRESS_MASK)) |
                 (start_address & FLEXSPI_IPEDCTX0START_START_ADDRESS_MASK);
    *reg_end =
        (*reg_end & (~FLEXSPI_IPEDCTX0END_END_ADDRESS_MASK)) | (end_address & FLEXSPI_IPEDCTX0END_END_ADDRESS_MASK);

    /* Re-enable soft lock for given region */
    base->IPEDCTXCTRL[0] = (base->IPEDCTXCTRL[0] & ~(FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_MASK << (region * 2UL))) |
                           (IPED_RW_DISABLE_VAL << (region * 2UL));

    return kStatus_Success;
}

status_t IPED_SetRegionEnable(FLEXSPI_Type *base, iped_region_t region, bool enable)
{
    /* Check if region is not locked */
    if (IPED_IsRegionLocked(base, region))
    {
        return (status_t)kStatus_IPED_RegionIsLocked;
    }

    /* Disable soft lock for given region first */
    base->IPEDCTXCTRL[0] = (base->IPEDCTXCTRL[0] & ~(FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_MASK << (region * 2UL))) |
                           (IPED_RW_ENABLE_VAL << (region * 2UL));

    __IO uint32_t *reg_start = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0START)) + (IPED_CTX_REG_OFFSET * region));
    if (enable)
    {
        *reg_start = *reg_start | FLEXSPI_IPEDCTX0START_GCM_MASK;
    }
    else
    {
        *reg_start = *reg_start & (~FLEXSPI_IPEDCTX0START_GCM_MASK);
    }

    /* Re-enable soft lock for given region */
    base->IPEDCTXCTRL[0] = (base->IPEDCTXCTRL[0] & ~(FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_MASK << (region * 2UL))) |
                           (IPED_RW_DISABLE_VAL << (region * 2UL));

    return kStatus_Success;
}

void IPED_GetRegionAddressRange(FLEXSPI_Type *base,
                                iped_region_t region,
                                uint32_t *start_address,
                                uint32_t *end_address)
{
    __IO uint32_t *reg_start = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0START)) + (IPED_CTX_REG_OFFSET * region));
    __IO uint32_t *reg_end   = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0END)) + (IPED_CTX_REG_OFFSET * region));

    *start_address = *reg_start & FLEXSPI_IPEDCTX0START_START_ADDRESS_MASK;
    *end_address   = *reg_end & FLEXSPI_IPEDCTX0END_END_ADDRESS_MASK;
}

void IPED_SetRegionIV(FLEXSPI_Type *base, iped_region_t region, const uint8_t iv[8])
{
    __IO uint32_t *reg_iv0 = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0IV0)) + (IPED_CTX_REG_OFFSET * region));
    __IO uint32_t *reg_iv1 = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0IV1)) + (IPED_CTX_REG_OFFSET * region));

    *reg_iv0 = ((uint32_t *)(uintptr_t)iv)[0];
    *reg_iv1 = ((uint32_t *)(uintptr_t)iv)[1];
}

void IPED_GetRegionIV(FLEXSPI_Type *base, iped_region_t region, uint8_t iv[8])
{
    __IO uint32_t *reg_iv0 = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0IV0)) + (IPED_CTX_REG_OFFSET * region));
    __IO uint32_t *reg_iv1 = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0IV1)) + (IPED_CTX_REG_OFFSET * region));

    ((uint32_t *)(uintptr_t)iv)[0] = *reg_iv0;
    ((uint32_t *)(uintptr_t)iv)[1] = *reg_iv1;
}

void IPED_SetRegionAAD(FLEXSPI_Type *base, iped_region_t region, const uint8_t aad[8])
{
    __IO uint32_t *reg_aad0 = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0AAD0)) + (IPED_CTX_REG_OFFSET * region));
    __IO uint32_t *reg_aad1 = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0AAD1)) + (IPED_CTX_REG_OFFSET * region));

    *reg_aad0 = ((uint32_t *)(uintptr_t)aad)[0];
    *reg_aad1 = ((uint32_t *)(uintptr_t)aad)[1];
}

void IPED_GetRegionAAD(FLEXSPI_Type *base, iped_region_t region, uint8_t aad[8])
{
    __IO uint32_t *reg_aad0 = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0AAD0)) + (IPED_CTX_REG_OFFSET * region));
    __IO uint32_t *reg_aad1 = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0AAD1)) + (IPED_CTX_REG_OFFSET * region));

    ((uint32_t *)(uintptr_t)aad)[0] = *reg_aad0;
    ((uint32_t *)(uintptr_t)aad)[1] = *reg_aad1;
}

#if defined(__cplusplus)
}
#endif
