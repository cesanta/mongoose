/*
** ###################################################################
**     Processors:          RW612ETA2I
**                          RW612HNA2I
**                          RW612UKA2I
**
**     Compilers:           GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    RW61X User manual Rev. 0.95, June 2022
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b240715
**
**     Abstract:
**         CMSIS Peripheral Access Layer for RW612
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2024 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2021-03-16)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file RW612.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for RW612
 *
 * CMSIS Peripheral Access Layer for RW612
 */

#if !defined(RW612_H_)  /* Check if memory map has not been already included */
#define RW612_H_

/* IP Header Files List */
#include "PERI_ACOMP.h"
#include "PERI_ADC.h"
#include "PERI_AHB_SECURE_CTRL.h"
#include "PERI_AON_SOC_CIU.h"
#include "PERI_APU.h"
#include "PERI_BG.h"
#include "PERI_BLEAPU.h"
#include "PERI_BUCK11.h"
#include "PERI_BUCK18.h"
#include "PERI_CACHE64_CTRL.h"
#include "PERI_CACHE64_POLSEL.h"
#include "PERI_CAU.h"
#include "PERI_CDOG.h"
#include "PERI_CIU1.h"
#include "PERI_CIU2.h"
#include "PERI_CLKCTL0.h"
#include "PERI_CLKCTL1.h"
#include "PERI_CRC.h"
#include "PERI_CTIMER.h"
#include "PERI_DAC.h"
#include "PERI_DBGMAILBOX.h"
#include "PERI_DMA.h"
#include "PERI_DMIC.h"
#include "PERI_ELS.h"
#include "PERI_ENET.h"
#include "PERI_FLEXCOMM.h"
#include "PERI_FLEXSPI.h"
#include "PERI_FREQME.h"
#include "PERI_GDMA.h"
#include "PERI_GPIO.h"
#include "PERI_I2C.h"
#include "PERI_I2S.h"
#include "PERI_INPUTMUX.h"
#include "PERI_ITRC.h"
#include "PERI_LCDIC.h"
#include "PERI_MCI_IO_MUX.h"
#include "PERI_MRT.h"
#include "PERI_OCOTP.h"
#include "PERI_OSTIMER.h"
#include "PERI_PINT.h"
#include "PERI_PKC.h"
#include "PERI_PMU.h"
#include "PERI_POWERQUAD.h"
#include "PERI_PUF.h"
#include "PERI_RF_SYSCON.h"
#include "PERI_ROMCP.h"
#include "PERI_RSTCTL0.h"
#include "PERI_RSTCTL1.h"
#include "PERI_RTC.h"
#include "PERI_SCT.h"
#include "PERI_SDU_FBR_CARD.h"
#include "PERI_SDU_FN0_CARD.h"
#include "PERI_SDU_FN_CARD.h"
#include "PERI_SENSOR_CTRL.h"
#include "PERI_SOCCIU.h"
#include "PERI_SOC_OTP_CTRL.h"
#include "PERI_SPI.h"
#include "PERI_SYSCTL0.h"
#include "PERI_SYSCTL1.h"
#include "PERI_SYSCTL2.h"
#include "PERI_SYSPLL_T3.h"
#include "PERI_SYSPLL_TCPU.h"
#include "PERI_SYSPLL_TDDR.h"
#include "PERI_TRNG.h"
#include "PERI_USART.h"
#include "PERI_USBC.h"
#include "PERI_USIM.h"
#include "PERI_UTICK.h"
#include "PERI_WLAPU.h"
#include "PERI_WWDT.h"

#endif  /* #if !defined(RW612_H_) */
