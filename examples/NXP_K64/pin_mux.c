/* clang-format off */
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitPins(void)
{
    port_pin_config_t configENET = {0};

    /* Initialize UART0 pins below */
    /* Ungate the port clock */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Affects PORTB_PCR16 register */
    PORT_SetPinMux(PORTB, 16u, kPORT_MuxAlt3);
    /* Affects PORTB_PCR17 register */
    PORT_SetPinMux(PORTB, 17u, kPORT_MuxAlt3);

    CLOCK_EnableClock(kCLOCK_PortC);
    /* Affects PORTC_PCR16 register */
    PORT_SetPinMux(PORTC, 16u, kPORT_MuxAlt4);
    /* Affects PORTC_PCR17 register */
    PORT_SetPinMux(PORTC, 17u, kPORT_MuxAlt4);
    /* Affects PORTC_PCR18 register */
    PORT_SetPinMux(PORTC, 18u, kPORT_MuxAlt4);
    /* Affects PORTC_PCR19 register */
    PORT_SetPinMux(PORTC, 19u, kPORT_MuxAlt4);
    /* Affects PORTB_PCR1 register */
    PORT_SetPinMux(PORTB, 1u, kPORT_MuxAlt4);

    configENET.openDrainEnable = kPORT_OpenDrainEnable;
    configENET.mux = kPORT_MuxAlt4;
    configENET.pullSelect = kPORT_PullUp;
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Affects PORTB_PCR0 register */
    PORT_SetPinConfig(PORTB, 0u, &configENET);

    /* Affects PORTA_PCR13 register */
    PORT_SetPinMux(PORTA, 13u, kPORT_MuxAlt4);
    /* Affects PORTA_PCR12 register */
    PORT_SetPinMux(PORTA, 12u, kPORT_MuxAlt4);
    /* Affects PORTA_PCR14 register */
    PORT_SetPinMux(PORTA, 14u, kPORT_MuxAlt4);
    /* Affects PORTA_PCR5 register */
    PORT_SetPinMux(PORTA, 5u, kPORT_MuxAlt4);
    /* Affects PORTA_PCR16 register */
    PORT_SetPinMux(PORTA, 16u, kPORT_MuxAlt4);
    /* Affects PORTA_PCR17 register */
    PORT_SetPinMux(PORTA, 17u, kPORT_MuxAlt4);
    /* Affects PORTA_PCR15 register */
    PORT_SetPinMux(PORTA, 15u, kPORT_MuxAlt4);
    /* Affects PORTA_PCR28 register */
    PORT_SetPinMux(PORTA, 28u, kPORT_MuxAlt4);

    /* Enable SW port clock */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Affects PORTA_PCR4 register */
    port_pin_config_t config = {0};
    config.pullSelect = kPORT_PullUp;
    config.mux = kPORT_MuxAsGpio;
    PORT_SetPinConfig(PORTA, 4U, &config);
}
