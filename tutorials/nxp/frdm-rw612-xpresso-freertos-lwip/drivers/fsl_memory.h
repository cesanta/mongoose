/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_MEMORY_H_
#define _FSL_MEMORY_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.memory"
#endif

/* Clear bit28 secure indicator to get physical address */
#define FSL_MEM_PHY_ADDR(addr) ((addr)&0xEFFFFFFFUL)

/* The FLEXSPI AMBA PC start address, refer to Reference Manual for detailed information */
#define FSL_MEM_FLEXSPI_AMBA_PC_BEGIN FSL_MEM_PHY_ADDR(FlexSPI_AMBA_PC_CACHE_BASE)
/* The FLEXSPI AMBA PC end address, refer to Reference Manual for detailed information */
#define FSL_MEM_FLEXSPI_AMBA_PC_END (FSL_MEM_FLEXSPI_AMBA_PC_BEGIN + 0x08000000UL - 1UL)

/* The FLEXSPI AMBA PS start address */
#define FSL_MEM_FLEXSPI_AMBA_PS_BEGIN FSL_MEM_PHY_ADDR(FlexSPI_AMBA_PS_NCACHE_BASE)
/* The FLEXSPI AMBA PS end address */
#define FSL_MEM_FLEXSPI_AMBA_PS_END (FSL_MEM_FLEXSPI_AMBA_PS_BEGIN + 0x08000000UL - 1UL)
/*
  This alias allows the DMA to access PC flash memory with PS address.
*/
#define FSL_MEM_FLEXSPI_AMBA_OFFSET (FlexSPI_AMBA_PS_NCACHE_BASE - FlexSPI_AMBA_PC_CACHE_BASE)

typedef enum _mem_direction
{
    kMEMORY_Local2DMA = 0,
    kMEMORY_DMA2Local,
} mem_direction_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
/*!
 * @brief Convert the memory map address.
 *
 * This function convert the address between system mapped address and native mapped address.
 * There maybe offset between subsystem native address and system address for some memory,
 * this funciton convert the address to different memory map.
 * @param addr address need to be converted.
 * @param direction convert direction.
 * @return the converted address
 */
static inline uint32_t MEMORY_ConvertMemoryMapAddress(uint32_t addr, mem_direction_t direction)
{
    uint32_t dest;
    uint32_t paddr = FSL_MEM_PHY_ADDR(addr);

    switch (direction)
    {
        case kMEMORY_Local2DMA:
        {
            if ((paddr >= FSL_MEM_FLEXSPI_AMBA_PC_BEGIN) && (paddr <= FSL_MEM_FLEXSPI_AMBA_PC_END))
            {
                dest = addr + FSL_MEM_FLEXSPI_AMBA_OFFSET;
            }
            else
            {
                dest = addr;
            }
            break;
        }
        case kMEMORY_DMA2Local:
        {
            if ((paddr >= FSL_MEM_FLEXSPI_AMBA_PS_BEGIN) && (paddr <= FSL_MEM_FLEXSPI_AMBA_PS_END))
            {
                dest = addr - FSL_MEM_FLEXSPI_AMBA_OFFSET;
            }
            else
            {
                dest = addr;
            }
            break;
        }
        default:
            dest = addr;
            break;
    }

    return dest;
}
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _FSL_MEMORY_H_ */
