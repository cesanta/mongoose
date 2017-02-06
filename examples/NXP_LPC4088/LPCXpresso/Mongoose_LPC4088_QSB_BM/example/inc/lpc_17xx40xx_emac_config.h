/* clang-format off */
/*
 * @brief LPC17xx/LPC40xx EMAC and PHY driver configuration file for LWIP
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __LPC_17XX40XX_EMAC_CONFIG_H_
#define __LPC_17XX40XX_EMAC_CONFIG_H_

#include "lwip/opt.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* The PHY address connected the to MII/RMII */
#define LPC_PHYDEF_PHYADDR 1

/* Autonegotiation mode enable flag */
#define PHY_USE_AUTONEG 1

/* PHY interface full duplex operation or half duplex enable flag.
   Only applies if PHY_USE_AUTONEG = 0 */
#define PHY_USE_FULL_DUPLEX 1

/* PHY interface 100MBS or 10MBS enable flag.
   Only applies if PHY_USE_AUTONEG = 0 */
#define PHY_USE_100MBS 1

/* Defines the number of descriptors used for RX */
#define LPC_NUM_BUFF_RXDESCS 4

/* Defines the number of descriptors used for TX */
#define LPC_NUM_BUFF_TXDESCS 4

/* Disable slow speed memory buffering */
#define LPC_CHECK_SLOWMEM 0

/* Array of slow memory address ranges for LPC_CHECK_SLOWMEM */
#define LPC_SLOWMEM_ARRAY

#ifdef __cplusplus
}
#endif

#endif /* __LPC_17XX40XX_EMAC_CONFIG_H_ */
