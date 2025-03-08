/*
 * Copyright 2017-2021,2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MFLASH_DRV_H__
#define __MFLASH_DRV_H__

#include "mflash_common.h"

/* Flash constants */
#ifndef MFLASH_SECTOR_SIZE
#define MFLASH_SECTOR_SIZE (4096U)
#endif

#ifndef MFLASH_PAGE_SIZE
#define MFLASH_PAGE_SIZE (256U)
#endif

/* Device specific settings */
#ifndef MFLASH_FLEXSPI
#define MFLASH_FLEXSPI FLEXSPI
#endif

#define MFLASH_BASE_ADDRESS (FlexSPI_AMBA_PC_CACHE_BASE & 0x0fffffffU)
#ifndef MFLASH_BASE_ADDRESS
#define MFLASH_BASE_ADDRESS (FlexSPI_AMBA_PC_CACHE_BASE)
#endif

/* Flash size expressed in bytes: 64MB */
#define MFLASH_BSIZE 0x04000000UL

#define MFLASH_REMAP_OFFSET() (MFLASH_FLEXSPI->HADDROFFSET & FLEXSPI_HADDROFFSET_ADDROFFSET_MASK)
#define MFLASH_REMAP_START()  (MFLASH_FLEXSPI->HADDRSTART & FLEXSPI_HADDRSTART_ADDRSTART_MASK)
#define MFLASH_REMAP_END()    (MFLASH_FLEXSPI->HADDREND & FLEXSPI_HADDREND_ENDSTART_MASK)

#define MFLASH_REMAP_ACTIVE() (MFLASH_REMAP_OFFSET() != 0UL)

#endif
