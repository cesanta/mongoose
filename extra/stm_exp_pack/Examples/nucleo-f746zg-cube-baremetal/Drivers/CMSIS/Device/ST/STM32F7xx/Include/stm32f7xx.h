/**
  ******************************************************************************
  * @file    stm32f7xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32F7xx Device Peripheral Access Layer Header File.
  *
  *          The file is the unique include file that the application programmer
  *          is using in the C source code, usually in main.c. This file contains:
  *           - Configuration section that allows to select:
  *              - The STM32F7xx device used in the target application
  *              - To use or not the peripheral's drivers in application code(i.e.
  *                code will be based on direct access to peripheral's registers
  *                rather than drivers API), this option is controlled by
  *                "#define USE_HAL_DRIVER"
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f7xx
  * @{
  */

#ifndef __STM32F7xx_H
#define __STM32F7xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup Library_configuration_section
  * @{
  */

/**
  * @brief STM32 Family
  */
#if !defined  (STM32F7)
#define STM32F7
#endif /* STM32F7 */

/* Uncomment the line below according to the target STM32 device used in your
   application
  */
#if !defined (STM32F756xx) && !defined (STM32F746xx) && !defined (STM32F745xx) && !defined (STM32F765xx) && \
    !defined (STM32F767xx) && !defined (STM32F769xx) && !defined (STM32F777xx) && !defined (STM32F779xx) && \
    !defined (STM32F722xx) && !defined (STM32F723xx) && !defined (STM32F732xx) && !defined (STM32F733xx) && \
    !defined (STM32F730xx) && !defined (STM32F750xx)

  /* #define STM32F756xx */   /*!< STM32F756VG, STM32F756ZG, STM32F756ZG, STM32F756IG, STM32F756BG,
                                   STM32F756NG Devices */
  /* #define STM32F746xx */   /*!< STM32F746VE, STM32F746VG, STM32F746ZE, STM32F746ZG, STM32F746IE, STM32F746IG,
                                   STM32F746BE, STM32F746BG, STM32F746NE, STM32F746NG Devices */
  /* #define STM32F745xx */   /*!< STM32F745VE, STM32F745VG, STM32F745ZG, STM32F745ZE, STM32F745IE, STM32F745IG Devices */
  /* #define STM32F765xx */   /*!< STM32F765BI, STM32F765BG, STM32F765NI, STM32F765NG, STM32F765II, STM32F765IG,
                                   STM32F765ZI, STM32F765ZG, STM32F765VI, STM32F765VG Devices */
  /* #define STM32F767xx */   /*!< STM32F767BG, STM32F767BI, STM32F767IG, STM32F767II, STM32F767NG, STM32F767NI,
                                   STM32F767VG, STM32F767VI, STM32F767ZG, STM32F767ZI Devices */
  /* #define STM32F769xx */   /*!< STM32F769AG, STM32F769AI, STM32F769BG, STM32F769BI, STM32F769IG, STM32F769II,
                                   STM32F769NG, STM32F769NI, STM32F768AI Devices */
  /* #define STM32F777xx */   /*!< STM32F777VI, STM32F777ZI, STM32F777II, STM32F777BI, STM32F777NI Devices */
  /* #define STM32F779xx */   /*!< STM32F779II, STM32F779BI, STM32F779NI, STM32F779AI, STM32F778AI Devices */
  /* #define STM32F722xx */   /*!< STM32F722IE, STM32F722ZE, STM32F722VE, STM32F722RE, STM32F722IC, STM32F722ZC,
                                   STM32F722VC, STM32F722RC Devices */
  /* #define STM32F723xx */   /*!< STM32F723IE, STM32F723ZE, STM32F723VE, STM32F723IC, STM32F723ZC, STM32F723VC Devices */
  /* #define STM32F732xx */   /*!< STM32F732IE, STM32F732ZE, STM32F732VE, STM32F732RE Devices */
  /* #define STM32F733xx */   /*!< STM32F733IE, STM32F733ZE, STM32F733VE Devices */
  /* #define STM32F730xx */   /*!< STM32F730R, STM32F730V, STM32F730Z, STM32F730I Devices */
  /* #define STM32F750xx */   /*!< STM32F750V, STM32F750Z, STM32F750N Devices */
#endif

/*  Tip: To avoid modifying this file each time you need to switch between these
        devices, you can define the device in your toolchain compiler preprocessor.
  */

#if !defined  (USE_HAL_DRIVER)
/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will
   be based on direct access to peripherals registers
   */
  /*#define USE_HAL_DRIVER */
#endif /* USE_HAL_DRIVER */

/**
  * @brief CMSIS Device version number V1.2.8
  */
#define __STM32F7_CMSIS_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32F7_CMSIS_VERSION_SUB1   (0x02) /*!< [23:16] sub1 version */
#define __STM32F7_CMSIS_VERSION_SUB2   (0x08) /*!< [15:8]  sub2 version */
#define __STM32F7_CMSIS_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32F7_CMSIS_VERSION        ((__STM32F7_CMSIS_VERSION_MAIN << 24)\
                                       |(__STM32F7_CMSIS_VERSION_SUB1 << 16)\
                                       |(__STM32F7_CMSIS_VERSION_SUB2 << 8 )\
                                       |(__STM32F7_CMSIS_VERSION_RC))
/**
  * @}
  */

/** @addtogroup Device_Included
  * @{
  */
#if defined(STM32F722xx)
  #include "stm32f722xx.h"
#elif defined(STM32F723xx)
  #include "stm32f723xx.h"
#elif defined(STM32F732xx)
  #include "stm32f732xx.h"
#elif defined(STM32F733xx)
  #include "stm32f733xx.h"
#elif defined(STM32F756xx)
  #include "stm32f756xx.h"
#elif defined(STM32F746xx)
  #include "stm32f746xx.h"
#elif defined(STM32F745xx)
  #include "stm32f745xx.h"
#elif defined(STM32F765xx)
  #include "stm32f765xx.h"
#elif defined(STM32F767xx)
  #include "stm32f767xx.h"
#elif defined(STM32F769xx)
  #include "stm32f769xx.h"
#elif defined(STM32F777xx)
  #include "stm32f777xx.h"
#elif defined(STM32F779xx)
  #include "stm32f779xx.h"
#elif defined(STM32F730xx)
  #include "stm32f730xx.h"
#elif defined(STM32F750xx)
  #include "stm32f750xx.h"
#else
 #error "Please select first the target STM32F7xx device used in your application (in stm32f7xx.h file)"
#endif

/**
  * @}
  */

/** @addtogroup Exported_types
  * @{
  */
typedef enum
{
  RESET = 0U,
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
  DISABLE = 0U,
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;

/**
  * @}
  */

/** @addtogroup Exported_macro
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))

/* Use of CMSIS compiler intrinsics for register exclusive access */
/* Atomic 32-bit register access macro to set one or several bits */
#define ATOMIC_SET_BIT(REG, BIT)                             \
  do {                                                       \
    uint32_t val;                                            \
    do {                                                     \
      val = __LDREXW((__IO uint32_t *)&(REG)) | (BIT);       \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 32-bit register access macro to clear one or several bits */
#define ATOMIC_CLEAR_BIT(REG, BIT)                           \
  do {                                                       \
    uint32_t val;                                            \
    do {                                                     \
      val = __LDREXW((__IO uint32_t *)&(REG)) & ~(BIT);      \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 32-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFY_REG(REG, CLEARMSK, SETMASK)                          \
  do {                                                                     \
    uint32_t val;                                                          \
    do {                                                                   \
      val = (__LDREXW((__IO uint32_t *)&(REG)) & ~(CLEARMSK)) | (SETMASK); \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U);               \
  } while(0)

/* Atomic 16-bit register access macro to set one or several bits */
#define ATOMIC_SETH_BIT(REG, BIT)                            \
  do {                                                       \
    uint16_t val;                                            \
    do {                                                     \
      val = __LDREXH((__IO uint16_t *)&(REG)) | (BIT);       \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 16-bit register access macro to clear one or several bits */
#define ATOMIC_CLEARH_BIT(REG, BIT)                          \
  do {                                                       \
    uint16_t val;                                            \
    do {                                                     \
      val = __LDREXH((__IO uint16_t *)&(REG)) & ~(BIT);      \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 16-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFYH_REG(REG, CLEARMSK, SETMASK)                         \
  do {                                                                     \
    uint16_t val;                                                          \
    do {                                                                   \
      val = (__LDREXH((__IO uint16_t *)&(REG)) & ~(CLEARMSK)) | (SETMASK); \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U);               \
  } while(0)

/**
  * @}
  */

#ifdef USE_HAL_DRIVER
 #include "stm32f7xx_hal.h"
#endif /* USE_HAL_DRIVER */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F7xx_H */

/**
  * @}
  */

/**
  * @}
  */

