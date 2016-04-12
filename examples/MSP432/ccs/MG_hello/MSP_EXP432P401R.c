/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== MSP_EXP432P401R.c ========
 *  This file is responsible for setting up the board specific items for the
 *  MSP_EXP432P401R board.
 */

#include <stdbool.h>

#include <ti/drivers/ports/DebugP.h>
#include <ti/drivers/ports/HwiP.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>

#include <msp.h>
#include <rom.h>
#include <rom_map.h>
#include <dma.h>
#include <gpio.h>
#include <i2c.h>
#include <pmap.h>
#include <spi.h>
#include <timer_a.h>
#include <uart.h>
#include <wdt_a.h>

#include <interrupt.h>

#include "MSP_EXP432P401R.h"

/*
 *  =============================== DMA ===============================
 */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(dmaControlTable, 256)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=256
#elif defined(__GNUC__)
__attribute__ ((aligned (256)))
#endif
static DMA_ControlTable dmaControlTable[8];
static bool dmaInitialized = false;

/*
 *  ======== dmaErrorHwi ========
 */
static void dmaErrorHwi(uintptr_t arg)
{
    DebugP_log1("DMA error code: %d\n", MAP_DMA_getErrorStatus());
    MAP_DMA_clearErrorStatus();
    DebugP_log0("DMA error!!\n");
    while(1);
}

/*
 *  ======== MSP_EXP432P401R_initDMA ========
 */
void MSP_EXP432P401R_initDMA(void)
{
    HwiP_Params hwiParams;
    HwiP_Handle dmaErrorHwiHandle;

    if (!dmaInitialized) {
        HwiP_Params_init(&hwiParams);
        dmaErrorHwiHandle = HwiP_create(INT_DMA_ERR, dmaErrorHwi, &hwiParams);
        if (dmaErrorHwiHandle == NULL) {
            DebugP_log0("Failed to create DMA error Hwi!!\n");
            while (1);
        }

        MAP_DMA_enableModule();
        MAP_DMA_setControlBase(dmaControlTable);

        dmaInitialized = true;
    }
}

/*
 *  ======== MSP_EXP432P401R_initGeneral ========
 */
void MSP_EXP432P401R_initGeneral(void)
{
    Power_init();
}

/*
 *  =============================== GPIO ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(GPIOMSP432_config, ".const:GPIOMSP432_config")
#endif

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP432.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP432P401R.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Input pins */
    /* MSP_EXP432P401R_S1 */
    GPIOMSP432_P1_1 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,
    /* MSP_EXP432P401R_S2 */
    GPIOMSP432_P1_4 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,

    /* Output pins */
    /* MSP_EXP432P401R_LED1 */
    GPIOMSP432_P1_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* MSP_EXP432P401R_LED_RED */
    GPIOMSP432_P2_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,

    /*
     * MSP_EXP432P401R_LED_GREEN & MSP_EXP432P401R_LED_BLUE are used for
     * PWM examples.  Uncomment the following lines if you would like to control
     * the LEDs with the GPIO driver.
     */
    /* MSP_EXP432P401R_LED_GREEN */
    //GPIOMSP432_P2_1 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* MSP_EXP432P401R_LED_BLUE */
    //GPIOMSP432_P2_2 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP432P401R.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    /* MSP_EXP432P401R_S1 */
    NULL,
    /* MSP_EXP432P401R_S2 */
    NULL
};

const GPIOMSP432_Config GPIOMSP432_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)
};

/*
 *  ======== MSP_EXP432P401R_initGPIO ========
 */
void MSP_EXP432P401R_initGPIO(void)
{
    /* Initialize peripheral and pins */
    GPIO_init();
}

/*
 *  =============================== I2C ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cMSP432HWAttrs, ".const:i2cMSP432HWAttrs")
#endif

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CMSP432.h>

I2CMSP432_Object i2cMSP432Objects[MSP_EXP432P401R_I2CCOUNT];

const I2CMSP432_HWAttrs i2cMSP432HWAttrs[MSP_EXP432P401R_I2CCOUNT] = {
    {
        .baseAddr = EUSCI_B0_BASE,
        .intNum = INT_EUSCIB0,
        .intPriority = (~0),
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK
    }
};

const I2C_Config I2C_config[] = {
    {
        .fxnTablePtr = &I2CMSP432_fxnTable,
        .object = &i2cMSP432Objects[0],
        .hwAttrs = &i2cMSP432HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401R_initI2C ========
 */
void MSP_EXP432P401R_initI2C(void)
{
    /*
     * NOTE: TI-RTOS examples configure EUSCIB0 as either SPI or I2C.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */
    /* Configure Pins 1.6 & 1.7 as SDA & SCL, respectively. */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initialize the I2C driver */
    I2C_init();
}

/*
 *  =============================== Power ===============================
 */
const PowerMSP432_Config PowerMSP432_config = {
    .policyInitFxn = &PowerMSP432_initPolicy,
    .policyFxn = &PowerMSP432_sleepPolicy,
    .initialPerfLevel = 2,
    .enablePolicy = false,
    .enablePerf = true
};

/*
 *  =============================== PWM ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(PWM_config, ".const:PWM_config")
#pragma DATA_SECTION(pwmTimerMSP432HWAttrs, ".const:pwmTimerMSP432HWAttrs")
#endif

#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerMSP432.h>

PWMTimerMSP432_Object pwmTimerMSP432Objects[MSP_EXP432P401R_PWMCOUNT];

const PWMTimerMSP432_HWAttrs pwmTimerMSP432HWAttrs[MSP_EXP432P401R_PWMCOUNT] = {
    {
        .baseAddr = TIMER_A1_BASE,
        .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1
    },
    {
        .baseAddr = TIMER_A1_BASE,
        .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2
    }
};

const PWM_Config PWM_config[] = {
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[0],
        .hwAttrs = &pwmTimerMSP432HWAttrs[0]
    },
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[1],
        .hwAttrs = &pwmTimerMSP432HWAttrs[1]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401R_initPWM ========
 */
void MSP_EXP432P401R_initPWM(void)
{
    /* Use Port Map on Port2 get Timer outputs on pins with LEDs (2.1, 2.2) */
    const uint8_t portMap [] = {
        PM_NONE, PM_TA1CCR1A, PM_TA1CCR2A, PM_NONE,
        PM_NONE, PM_NONE,     PM_NONE,     PM_NONE
    };

    /* Mapping capture compare registers to Port 2 */
    MAP_PMAP_configurePorts((const uint8_t *) portMap, PMAP_P2MAP, 1,
        PMAP_DISABLE_RECONFIGURATION);

    /* Enable PWM output on GPIO pins */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
        GPIO_PIN1 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    PWM_init();
}

/*
 *  =============================== SDSPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SDSPI_config, ".const:SDSPI_config")
#pragma DATA_SECTION(sdspiMSP432HWAttrs, ".const:sdspiMSP432HWAttrs")
#endif

#include <ti/drivers/SDSPI.h>
#include <ti/drivers/sdspi/SDSPIMSP432.h>

SDSPIMSP432_Object sdspiMSP432Objects[MSP_EXP432P401R_SDSPICOUNT];

const SDSPIMSP432_HWAttrs sdspiMSP432HWAttrs[MSP_EXP432P401R_SDSPICOUNT] = {
    {
        .baseAddr = EUSCI_B0_BASE,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,

        /* CLK, MOSI & MISO ports & pins */
        .portSCK = GPIO_PORT_P1,
        .pinSCK = GPIO_PIN5,
        .sckMode = GPIO_PRIMARY_MODULE_FUNCTION,

        .portMISO = GPIO_PORT_P1,
        .pinMISO = GPIO_PIN7,
        .misoMode = GPIO_PRIMARY_MODULE_FUNCTION,

        .portMOSI = GPIO_PORT_P1,
        .pinMOSI = GPIO_PIN6,
        .mosiMode = GPIO_PRIMARY_MODULE_FUNCTION,

        /* Chip select port & pin */
        .portCS = GPIO_PORT_P4,
        .pinCS = GPIO_PIN6
    }
};

const SDSPI_Config SDSPI_config[] = {
    {
        .fxnTablePtr = &SDSPIMSP432_fxnTable,
        .object = &sdspiMSP432Objects[0],
        .hwAttrs = &sdspiMSP432HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401R_initSDSPI ========
 */
void MSP_EXP432P401R_initSDSPI(void)
{
    SDSPI_init();
}

/*
 *  =============================== SPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SPI_config, ".const:SPI_config")
#pragma DATA_SECTION(spiMSP432DMAHWAttrs, ".const:spiMSP432DMAHWAttrs")
#endif

#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPIMSP432DMA.h>

SPIMSP432DMA_Object spiMSP432DMAObjects[MSP_EXP432P401R_SPICOUNT];

const SPIMSP432DMA_HWAttrs spiMSP432DMAHWAttrs[MSP_EXP432P401R_SPICOUNT] = {
    {
        .baseAddr = EUSCI_B0_BASE,
        .bitOrder = EUSCI_B_SPI_MSB_FIRST,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,

        .defaultTxBufValue = 0,

        .dmaIntNum = INT_DMA_INT1,
        .intPriority = (~0),
        .rxDMAChannelIndex = DMA_CH1_EUSCIB0RX0,
        .txDMAChannelIndex = DMA_CH0_EUSCIB0TX0
    },
    {
        .baseAddr = EUSCI_B2_BASE,
        .bitOrder = EUSCI_B_SPI_MSB_FIRST,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,

        .defaultTxBufValue = 0,

        .dmaIntNum = INT_DMA_INT2,
        .intPriority = (~0),
        .rxDMAChannelIndex = DMA_CH5_EUSCIB2RX0,
        .txDMAChannelIndex = DMA_CH4_EUSCIB2TX0
    }
};

const SPI_Config SPI_config[] = {
    {
        .fxnTablePtr = &SPIMSP432DMA_fxnTable,
        .object = &spiMSP432DMAObjects[0],
        .hwAttrs = &spiMSP432DMAHWAttrs[0]
    },
    {
        .fxnTablePtr = &SPIMSP432DMA_fxnTable,
        .object = &spiMSP432DMAObjects[1],
        .hwAttrs = &spiMSP432DMAHWAttrs[1]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== MSP_EXP432P401R_initSPI ========
 */
void MSP_EXP432P401R_initSPI(void)
{
    /*
     * NOTE: TI-RTOS examples configure EUSCIB0 as either SPI or I2C.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */
    /* Configure CLK, MOSI & MISO for SPI0 (EUSCI_B0) */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,
        GPIO_PIN5 | GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN7,
        GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configure CLK, MOSI & MISO for SPI1 (EUSCI_B2) */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3,
        GPIO_PIN5 | GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN7,
        GPIO_PRIMARY_MODULE_FUNCTION);

    MSP_EXP432P401R_initDMA();
    SPI_init();
}

/*
 *  =============================== UART ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartMSP432HWAttrs, ".const:uartMSP432HWAttrs")
#endif

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>

UARTMSP432_Object uartMSP432Objects[MSP_EXP432P401R_UARTCOUNT];
unsigned char uartMSP432RingBuffer[MSP_EXP432P401R_UARTCOUNT][32];

/*
 * The baudrate dividers were determined by using the MSP430 baudrate
 * calculator
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const UARTMSP432_BaudrateConfig uartMSP432Baudrates[] = {
    /* {baudrate, input clock, prescalar, UCBRFx, UCBRSx, oversampling} */
    {
        .outputBaudrate = 115200,
        .inputClockFreq = 12000000,
        .prescalar = 6,
        .hwRegUCBRFx = 8,
        .hwRegUCBRSx = 32,
        .oversampling = 1
    },
    {115200, 6000000,   3,  4,   2, 1},
    {115200, 3000000,   1, 10,   0, 1},
    {9600,   12000000, 78,  2,   0, 1},
    {9600,   6000000,  39,  1,   0, 1},
    {9600,   3000000,  19,  8,  85, 1},
    {9600,   32768,     3,  0, 146, 0}
};

const UARTMSP432_HWAttrs uartMSP432HWAttrs[MSP_EXP432P401R_UARTCOUNT] = {
    {
        .baseAddr = EUSCI_A0_BASE,
        .intNum = INT_EUSCIA0,
        .intPriority = (~0),
        .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .bitOrder = EUSCI_A_UART_LSB_FIRST,
        .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
            sizeof(UARTMSP432_BaudrateConfig),
        .baudrateLUT = uartMSP432Baudrates,
        .ringBufPtr  = uartMSP432RingBuffer[0],
        .ringBufSize = sizeof(uartMSP432RingBuffer[0])
    },
    {
        .baseAddr = EUSCI_A2_BASE,
        .intNum = INT_EUSCIA2,
        .intPriority = (~0),
        .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .bitOrder = EUSCI_A_UART_LSB_FIRST,
        .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
            sizeof(UARTMSP432_BaudrateConfig),
        .baudrateLUT = uartMSP432Baudrates,
        .ringBufPtr  = uartMSP432RingBuffer[1],
        .ringBufSize = sizeof(uartMSP432RingBuffer[1])
    }
};

const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTMSP432_fxnTable,
        .object = &uartMSP432Objects[0],
        .hwAttrs = &uartMSP432HWAttrs[0]
    },
    {
        .fxnTablePtr = &UARTMSP432_fxnTable,
        .object = &uartMSP432Objects[1],
        .hwAttrs = &uartMSP432HWAttrs[1]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401R_initUART ========
 */
void MSP_EXP432P401R_initUART(void)
{
    /* Set P1.2 & P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Set P3.2 & P3.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
        GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initialize the UART driver */
    UART_init();
}

/*
 *  =============================== Watchdog ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(Watchdog_config, ".const:Watchdog_config")
#pragma DATA_SECTION(watchdogMSP432HWAttrs, ".const:watchdogMSP432HWAttrs")
#endif

#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogMSP432.h>

WatchdogMSP432_Object watchdogMSP432Objects[MSP_EXP432P401R_WATCHDOGCOUNT];

const WatchdogMSP432_HWAttrs
    watchdogMSP432HWAttrs[MSP_EXP432P401R_WATCHDOGCOUNT] = {
    {
        .baseAddr = WDT_A_BASE,
        .intNum = INT_WDT_A,
        .intPriority = (~0),
        .clockSource = WDT_A_CLOCKSOURCE_SMCLK,
        .clockDivider = WDT_A_CLOCKDIVIDER_8192K
    },
};

const Watchdog_Config Watchdog_config[] = {
    {
        .fxnTablePtr = &WatchdogMSP432_fxnTable,
        .object = &watchdogMSP432Objects[0],
        .hwAttrs = &watchdogMSP432HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401R_initWatchdog ========
 */
void MSP_EXP432P401R_initWatchdog(void)
{
    /* Initialize the Watchdog driver */
    Watchdog_init();
}

/*
 *  =============================== WiFi ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(WiFi_config, ".const:WiFi_config")
#pragma DATA_SECTION(wiFiCC3100HWAttrs, ".const:wiFiCC3100HWAttrs")
#endif

#include <ti/drivers/WiFi.h>
#include <ti/drivers/wifi/WiFiCC3100.h>

WiFiCC3100_Object wiFiCC3100Objects[MSP_EXP432P401R_WIFICOUNT];

const WiFiCC3100_HWAttrs wiFiCC3100HWAttrs[MSP_EXP432P401R_WIFICOUNT] = {
    {
        .irqPort = GPIO_PORT_P2,
        .irqPin = GPIO_PIN5,
        .irqIntNum = INT_PORT2,

        .csPort = GPIO_PORT_P3,
        .csPin = GPIO_PIN0,

        .enPort = GPIO_PORT_P4,
        .enPin = GPIO_PIN1
    }
};

const WiFi_Config WiFi_config[] = {
    {
        .fxnTablePtr = &WiFiCC3100_fxnTable,
        .object = &wiFiCC3100Objects[0],
        .hwAttrs = &wiFiCC3100HWAttrs[0]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== MSP_EXP432P401R_initWiFi ========
 */
void MSP_EXP432P401R_initWiFi(void)
{
    /* Configure EN & CS pins to disable CC3100 */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);

    /* Configure CLK, MOSI & MISO for SPI0 (EUSCI_B0) */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,
        GPIO_PIN5 | GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN7,
        GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configure IRQ pin */
    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P2, GPIO_PIN5,
        GPIO_LOW_TO_HIGH_TRANSITION);

    /* Initialize SPI and WiFi drivers */
    MSP_EXP432P401R_initDMA();
    SPI_init();
    WiFi_init();
}
