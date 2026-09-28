/**
  ******************************************************************************
  * @file    stm32h5xx_hal_flash.h
  * @author  MCD Application Team
  * @brief   Header file of FLASH HAL module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H5xx_HAL_FLASH_H
#define STM32H5xx_HAL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup FLASH_Exported_Types FLASH Exported Types
  * @{
  */

/**
  * @brief  FLASH handle Structure definition
  */
typedef struct
{
  HAL_LockTypeDef        Lock;               /*!< FLASH locking object                                                */

  uint32_t               ErrorCode;          /*!< FLASH error code                                                    */

  uint32_t               ProcedureOnGoing;   /*!< Internal variable to indicate which procedure is ongoing or not
                                                  in IT context */

  uint32_t               Address;            /*!< Internal variable to save address selected for program              */

  uint32_t               Bank;               /*!< Internal variable to save current bank selected during erase in
                                                  IT context */

  uint32_t               Sector;             /*!< Internal variable to define the current sector which is erasing     */

  uint32_t               NbSectorsToErase;   /*!< Internal variable to save the remaining sectors to erase in
                                                  IT context  */

} FLASH_ProcessTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */

/** @defgroup FLASH_Flag_definition FLASH Flag definition
  * @brief Flag definition
  * @{
  */
#define FLASH_FLAG_BSY                     FLASH_SR_BSY             /*!< FLASH Busy flag */
#define FLASH_FLAG_WBNE                    FLASH_SR_WBNE            /*!< FLASH Write Buffer Not Empty flag */
#define FLASH_FLAG_DBNE                    FLASH_SR_DBNE            /*!< FLASH data Buffer Not Empty flag */
#define FLASH_FLAG_EOP                     FLASH_SR_EOP             /*!< FLASH End Of operation flag */
#define FLASH_FLAG_WRPERR                  FLASH_SR_WRPERR          /*!< FLASH Write Protection Error flag */
#define FLASH_FLAG_PGSERR                  FLASH_SR_PGSERR          /*!< FLASH Program Sequence Error flag */
#define FLASH_FLAG_STRBERR                 FLASH_SR_STRBERR         /*!< FLASH Strobe Error flag */
#define FLASH_FLAG_INCERR                  FLASH_SR_INCERR          /*!< FLASH Inconsistency Error flag */
#if defined (FLASH_SR_OBKERR)
#define FLASH_FLAG_OBKERR                  FLASH_SR_OBKERR          /*!< FLASH OBK Error flag */
#define FLASH_FLAG_OBKWERR                 FLASH_SR_OBKWERR         /*!< FLASH OBK Write Error flag */
#endif /* FLASH_SR_OBKERR */
#define FLASH_FLAG_OPTCHANGEERR            FLASH_SR_OPTCHANGEERR    /*!< FLASH Option Byte change Error flag */
#define FLASH_FLAG_ECCC                    FLASH_ECCR_ECCC          /*!< FLASH ECC Correction flag */
#define FLASH_FLAG_ECCD                    FLASH_ECCR_ECCD          /*!< FLASH ECC Detection flag */

#if defined (FLASH_SR_OBKERR)
#define FLASH_FLAG_SR_ERRORS               (FLASH_SR_WRPERR      | FLASH_SR_PGSERR    | \
                                            FLASH_SR_STRBERR     | FLASH_SR_INCERR    | \
                                            FLASH_SR_OBKERR      | FLASH_SR_OBKWERR   | \
                                            FLASH_SR_OPTCHANGEERR)
#else
#define FLASH_FLAG_SR_ERRORS               (FLASH_SR_WRPERR      | FLASH_SR_PGSERR    | \
                                            FLASH_SR_STRBERR     | FLASH_SR_INCERR    | \
                                            FLASH_SR_OPTCHANGEERR)
#endif /* FLASH_SR_OBKERR */
#define FLASH_FLAG_ECCR_ERRORS             (FLASH_FLAG_ECCC      | FLASH_FLAG_ECCD)
#define FLASH_FLAG_ALL_ERRORS              (FLASH_FLAG_SR_ERRORS | FLASH_FLAG_ECCR_ERRORS) /*!< All FLASH error flags */
/**
  * @}
  */

/** @defgroup FLASH_Interrupt_definition FLASH Interrupts definition
  * @brief FLASH Interrupt definition
  * @{
  */
#define FLASH_IT_EOP                  FLASH_CR_EOPIE        /*!< End of FLASH Operation interrupt enable */
#define FLASH_IT_WRPERR               FLASH_CR_WRPERRIE     /*!< Write Protection Error interrupt enable */
#define FLASH_IT_PGSERR               FLASH_CR_PGSERRIE     /*!< Program Sequence Error interrupt enable */
#define FLASH_IT_STRBERR              FLASH_CR_STRBERRIE    /*!< Strobe Error interrupt enable */
#define FLASH_IT_INCERR               FLASH_CR_INCERRIE     /*!< Inconsistency Error interrupt enable */
#if defined (FLASH_SR_OBKERR)
#define FLASH_IT_OBKERR               FLASH_CR_OBKERRIE     /*!< OBK Error interrupt enable */
#define FLASH_IT_OBKWERR              FLASH_CR_OBKWERRIE    /*!< OBK Write Error interrupt enable */
#endif /* FLASH_SR_OBKERR */
#define FLASH_IT_OPTCHANGEERR         FLASH_CR_OPTCHANGEERRIE /*!< Option Byte change Error interrupt enable */
#define FLASH_IT_ECCC                 FLASH_ECCR_ECCIE      /*!< Single ECC Error Correction interrupt enable */

#if defined (FLASH_SR_OBKERR)
#define FLASH_IT_ALL                 (FLASH_IT_EOP       | FLASH_IT_WRPERR   | \
                                      FLASH_IT_PGSERR    | FLASH_IT_STRBERR  | \
                                      FLASH_IT_INCERR    | FLASH_IT_OBKERR   | \
                                      FLASH_IT_OBKWERR   | FLASH_IT_OPTCHANGEERR   | \
                                      FLASH_IT_ECCC) /*!< All Flash interrupt sources */
#else
#define FLASH_IT_ALL                 (FLASH_IT_EOP       | FLASH_IT_WRPERR       | \
                                      FLASH_IT_PGSERR    | FLASH_IT_STRBERR      | \
                                      FLASH_IT_INCERR    | FLASH_IT_OPTCHANGEERR | \
                                      FLASH_IT_ECCC) /*!< All Flash interrupt sources */
#endif /* FLASH_SR_OBKERR */

/**
  * @}
  */

/** @defgroup FLASH_Error_Code FLASH Error Code
  * @brief    FLASH Error Code
  * @{
  */
#define HAL_FLASH_ERROR_NONE         0x00000000U               /*!< No error                              */
#define HAL_FLASH_ERROR_WRP          FLASH_FLAG_WRPERR         /*!< Write Protection Error                */
#define HAL_FLASH_ERROR_PGS          FLASH_FLAG_PGSERR         /*!< Program Sequence Error                */
#define HAL_FLASH_ERROR_STRB         FLASH_FLAG_STRBERR        /*!< Strobe Error                          */
#define HAL_FLASH_ERROR_INC          FLASH_FLAG_INCERR         /*!< Inconsistency Error                   */
#if defined (FLASH_SR_OBKERR)
#define HAL_FLASH_ERROR_OBK          FLASH_FLAG_OBKERR         /*!< OBK Error                             */
#define HAL_FLASH_ERROR_OBKW         FLASH_FLAG_OBKWERR        /*!< OBK Write Error                       */
#endif /* FLASH_SR_OBKERR */
#define HAL_FLASH_ERROR_OB_CHANGE    FLASH_FLAG_OPTCHANGEERR   /*!< Option Byte Change Error              */
#define HAL_FLASH_ERROR_ECCC         FLASH_FLAG_ECCC           /*!< ECC Single Correction Error           */
#define HAL_FLASH_ERROR_ECCD         FLASH_FLAG_ECCD           /*!< ECC Double Detection Error            */
/**
  * @}
  */

/** @defgroup FLASH_Type_Program FLASH Program Type
  * @{
  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define FLASH_TYPEPROGRAM_QUADWORD         FLASH_CR_PG                                        /*!< Program a quad-word
                                                                             (128-bit) at  a specified secure address */
#define FLASH_TYPEPROGRAM_QUADWORD_NS       (FLASH_CR_PG | FLASH_NON_SECURE_MASK)              /*!< Program a quad-word
                                                                          (128-bit) at a specified non-secure address */
#if defined (FLASH_SR_OBKERR)
#define FLASH_TYPEPROGRAM_QUADWORD_OBK      (FLASH_CR_PG | FLASH_OBK)                          /*!< Program a quad-word
                                                                                   (128-bit) of OBK to current sector */
#define FLASH_TYPEPROGRAM_QUADWORD_OBK_ALT  (FLASH_CR_PG | FLASH_OBK | FLASH_OBKCFGR_ALT_SECT) /*!< Program a quad-word
                                                                                 (128-bit) of OBK to alternate sector */
#endif /* FLASH_SR_OBKERR */
#if defined (FLASH_EDATAR_EDATA_EN)
#define FLASH_TYPEPROGRAM_HALFWORD_EDATA    (FLASH_CR_PG | FLASH_EDATA_HALFWORD)               /*!< Program a flash
                                                      high-cycle data half-word (16-bit)at a specified secure address */
#define FLASH_TYPEPROGRAM_HALFWORD_EDATA_NS (FLASH_CR_PG | FLASH_EDATA_HALFWORD | FLASH_NON_SECURE_MASK)  /*!< Program a flash
                                                  high-cycle data half-word (16-bit)at a specified non-secure address */
#define FLASH_TYPEPROGRAM_WORD_EDATA    (FLASH_CR_PG | FLASH_EDATA_WORD)                          /*!< Program a flash
                                                      high-cycle data word (32-bit)at a specified secure address */
#define FLASH_TYPEPROGRAM_WORD_EDATA_NS (FLASH_CR_PG | FLASH_EDATA_WORD | FLASH_NON_SECURE_MASK)  /*!< Program a flash
                                                  high-cycle data word (32-bit)at a specified non-secure address */
#endif /* FLASH_EDATAR_EDATA_EN */
#else
#define FLASH_TYPEPROGRAM_QUADWORD           FLASH_CR_PG                                        /*!< Program a quad-word
                                                                                     (128-bit) at a specified address */
#if defined (FLASH_SR_OBKERR)
#define FLASH_TYPEPROGRAM_QUADWORD_OBK     (FLASH_CR_PG | FLASH_OBK)                           /*!< Program a quad-word
                                                                                   (128-bit) of OBK to current sector */
#define FLASH_TYPEPROGRAM_QUADWORD_OBK_ALT (FLASH_CR_PG | FLASH_OBK | FLASH_OBKCFGR_ALT_SECT)  /*!< Program a quad-word
                                                                                 (128-bit) of OBK to alternate sector */
#endif /* FLASH_SR_OBKERR */
#if defined (FLASH_EDATAR_EDATA_EN)
#define FLASH_TYPEPROGRAM_HALFWORD_EDATA    (FLASH_CR_PG | FLASH_EDATA_HALFWORD)                /*!< Program a flash
                                                             high-cycle data half-word (16-bit)at a specified address */
#define FLASH_TYPEPROGRAM_WORD_EDATA        (FLASH_CR_PG | FLASH_EDATA_WORD)                          /*!< Program a flash
                                                             high-cycle data half-word (32-bit)at a specified address */
#endif /* FLASH_EDATAR_EDATA_EN */
#endif /* __ARM_FEATURE_CMSE */
#define FLASH_TYPEPROGRAM_HALFWORD_OTP      (FLASH_CR_PG | FLASH_OTP | FLASH_NON_SECURE_MASK)   /*!< Program an OTP
                                                                             half-word (16-bit)at a specified address */
/**
  * @}
  */

/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */
#define FLASH_LATENCY_0          FLASH_ACR_LATENCY_0WS   /*!< FLASH Zero wait cycle      */
#define FLASH_LATENCY_1          FLASH_ACR_LATENCY_1WS   /*!< FLASH One wait cycle       */
#define FLASH_LATENCY_2          FLASH_ACR_LATENCY_2WS   /*!< FLASH Two wait cycles      */
#define FLASH_LATENCY_3          FLASH_ACR_LATENCY_3WS   /*!< FLASH Three wait cycles    */
#define FLASH_LATENCY_4          FLASH_ACR_LATENCY_4WS   /*!< FLASH Four wait cycles     */
#define FLASH_LATENCY_5          FLASH_ACR_LATENCY_5WS   /*!< FLASH Five wait cycles     */
#define FLASH_LATENCY_6          FLASH_ACR_LATENCY_6WS   /*!< FLASH Six wait cycles      */
#define FLASH_LATENCY_7          FLASH_ACR_LATENCY_7WS   /*!< FLASH Seven wait cycles    */
#define FLASH_LATENCY_8          FLASH_ACR_LATENCY_8WS   /*!< FLASH Eight wait cycle     */
#define FLASH_LATENCY_9          FLASH_ACR_LATENCY_9WS   /*!< FLASH Nine wait cycle      */
#define FLASH_LATENCY_10         FLASH_ACR_LATENCY_10WS  /*!< FLASH Ten wait cycles      */
#define FLASH_LATENCY_11         FLASH_ACR_LATENCY_11WS  /*!< FLASH Eleven wait cycles   */
#define FLASH_LATENCY_12         FLASH_ACR_LATENCY_12WS  /*!< FLASH Twelve wait cycles   */
#define FLASH_LATENCY_13         FLASH_ACR_LATENCY_13WS  /*!< FLASH Thirteen wait cycles */
#define FLASH_LATENCY_14         FLASH_ACR_LATENCY_14WS  /*!< FLASH Fourteen wait cycles */
#define FLASH_LATENCY_15         FLASH_ACR_LATENCY_15WS  /*!< FLASH Fifteen wait cycles  */
/**
  * @}
  */

/** @defgroup FLASH_Keys FLASH Keys
  * @{
  */
#define FLASH_KEY1                 0x45670123U
#define FLASH_KEY2                 0xCDEF89ABU
#define FLASH_OPT_KEY1             0x08192A3BU
#define FLASH_OPT_KEY2             0x4C5D6E7FU
#if defined (FLASH_SR_OBKERR)
#define FLASH_OBK_KEY1             0x192A083BU
#define FLASH_OBK_KEY2             0x6E7F4C5DU
#endif /* FLASH_SR_OBKERR */
/**
  * @}
  */

/** @defgroup FLASH_Sectors FLASH Sectors
  * @{
  */
#define FLASH_SECTOR_0             0U       /*!< Sector Number 0   */
#define FLASH_SECTOR_1             1U       /*!< Sector Number 1   */
#define FLASH_SECTOR_2             2U       /*!< Sector Number 2   */
#define FLASH_SECTOR_3             3U       /*!< Sector Number 3   */
#define FLASH_SECTOR_4             4U       /*!< Sector Number 4   */
#define FLASH_SECTOR_5             5U       /*!< Sector Number 5   */
#define FLASH_SECTOR_6             6U       /*!< Sector Number 6   */
#define FLASH_SECTOR_7             7U       /*!< Sector Number 7   */
#if (FLASH_SECTOR_NB >= 32)
#define FLASH_SECTOR_8             8U       /*!< Sector Number 8   */
#define FLASH_SECTOR_9             9U       /*!< Sector Number 9   */
#define FLASH_SECTOR_10            10U      /*!< Sector Number 10  */
#define FLASH_SECTOR_11            11U      /*!< Sector Number 11  */
#define FLASH_SECTOR_12            12U      /*!< Sector Number 12  */
#define FLASH_SECTOR_13            13U      /*!< Sector Number 13  */
#define FLASH_SECTOR_14            14U      /*!< Sector Number 14  */
#define FLASH_SECTOR_15            15U      /*!< Sector Number 15  */
#define FLASH_SECTOR_16            16U      /*!< Sector Number 16  */
#define FLASH_SECTOR_17            17U      /*!< Sector Number 17  */
#define FLASH_SECTOR_18            18U      /*!< Sector Number 18  */
#define FLASH_SECTOR_19            19U      /*!< Sector Number 19  */
#define FLASH_SECTOR_20            20U      /*!< Sector Number 20  */
#define FLASH_SECTOR_21            21U      /*!< Sector Number 21  */
#define FLASH_SECTOR_22            22U      /*!< Sector Number 22  */
#define FLASH_SECTOR_23            23U      /*!< Sector Number 23  */
#define FLASH_SECTOR_24            24U      /*!< Sector Number 24  */
#define FLASH_SECTOR_25            25U      /*!< Sector Number 25  */
#define FLASH_SECTOR_26            26U      /*!< Sector Number 26  */
#define FLASH_SECTOR_27            27U      /*!< Sector Number 27  */
#define FLASH_SECTOR_28            28U      /*!< Sector Number 28  */
#define FLASH_SECTOR_29            29U      /*!< Sector Number 29  */
#define FLASH_SECTOR_30            30U      /*!< Sector Number 30  */
#define FLASH_SECTOR_31            31U      /*!< Sector Number 31  */
#endif /* (FLASH_SECTOR_NB >= 32) */
#if (FLASH_SECTOR_NB >= 64)
#define FLASH_SECTOR_32            32U      /*!< Sector Number 32  */
#define FLASH_SECTOR_33            33U      /*!< Sector Number 33  */
#define FLASH_SECTOR_34            34U      /*!< Sector Number 34  */
#define FLASH_SECTOR_35            35U      /*!< Sector Number 35  */
#define FLASH_SECTOR_36            36U      /*!< Sector Number 36  */
#define FLASH_SECTOR_37            37U      /*!< Sector Number 37  */
#define FLASH_SECTOR_38            38U      /*!< Sector Number 38  */
#define FLASH_SECTOR_39            39U      /*!< Sector Number 39  */
#define FLASH_SECTOR_40            40U      /*!< Sector Number 40  */
#define FLASH_SECTOR_41            41U      /*!< Sector Number 41  */
#define FLASH_SECTOR_42            42U      /*!< Sector Number 42  */
#define FLASH_SECTOR_43            43U      /*!< Sector Number 43  */
#define FLASH_SECTOR_44            44U      /*!< Sector Number 44  */
#define FLASH_SECTOR_45            45U      /*!< Sector Number 45  */
#define FLASH_SECTOR_46            46U      /*!< Sector Number 46  */
#define FLASH_SECTOR_47            47U      /*!< Sector Number 47  */
#define FLASH_SECTOR_48            48U      /*!< Sector Number 48  */
#define FLASH_SECTOR_49            49U      /*!< Sector Number 49  */
#define FLASH_SECTOR_50            50U      /*!< Sector Number 50  */
#define FLASH_SECTOR_51            51U      /*!< Sector Number 51  */
#define FLASH_SECTOR_52            52U      /*!< Sector Number 52  */
#define FLASH_SECTOR_53            53U      /*!< Sector Number 53  */
#define FLASH_SECTOR_54            54U      /*!< Sector Number 54  */
#define FLASH_SECTOR_55            55U      /*!< Sector Number 55  */
#define FLASH_SECTOR_56            56U      /*!< Sector Number 56  */
#define FLASH_SECTOR_57            57U      /*!< Sector Number 57  */
#define FLASH_SECTOR_58            58U      /*!< Sector Number 58  */
#define FLASH_SECTOR_59            59U      /*!< Sector Number 59  */
#define FLASH_SECTOR_60            60U      /*!< Sector Number 60  */
#define FLASH_SECTOR_61            61U      /*!< Sector Number 61  */
#define FLASH_SECTOR_62            62U      /*!< Sector Number 62  */
#define FLASH_SECTOR_63            63U      /*!< Sector Number 63  */
#endif /* (FLASH_SECTOR_NB >= 64) */
#if (FLASH_SECTOR_NB >= 128)
#define FLASH_SECTOR_64            64U      /*!< Sector Number 64  */
#define FLASH_SECTOR_65            65U      /*!< Sector Number 65  */
#define FLASH_SECTOR_66            66U      /*!< Sector Number 66  */
#define FLASH_SECTOR_67            67U      /*!< Sector Number 67  */
#define FLASH_SECTOR_68            68U      /*!< Sector Number 68  */
#define FLASH_SECTOR_69            69U      /*!< Sector Number 69  */
#define FLASH_SECTOR_70            70U      /*!< Sector Number 70  */
#define FLASH_SECTOR_71            71U      /*!< Sector Number 71  */
#define FLASH_SECTOR_72            72U      /*!< Sector Number 72  */
#define FLASH_SECTOR_73            73U      /*!< Sector Number 73  */
#define FLASH_SECTOR_74            74U      /*!< Sector Number 74  */
#define FLASH_SECTOR_75            75U      /*!< Sector Number 75  */
#define FLASH_SECTOR_76            76U      /*!< Sector Number 76  */
#define FLASH_SECTOR_77            77U      /*!< Sector Number 77  */
#define FLASH_SECTOR_78            78U      /*!< Sector Number 78  */
#define FLASH_SECTOR_79            79U      /*!< Sector Number 79  */
#define FLASH_SECTOR_80            80U      /*!< Sector Number 80  */
#define FLASH_SECTOR_81            81U      /*!< Sector Number 81  */
#define FLASH_SECTOR_82            82U      /*!< Sector Number 82  */
#define FLASH_SECTOR_83            83U      /*!< Sector Number 83  */
#define FLASH_SECTOR_84            84U      /*!< Sector Number 84  */
#define FLASH_SECTOR_85            85U      /*!< Sector Number 85  */
#define FLASH_SECTOR_86            86U      /*!< Sector Number 86  */
#define FLASH_SECTOR_87            87U      /*!< Sector Number 87  */
#define FLASH_SECTOR_88            88U      /*!< Sector Number 88  */
#define FLASH_SECTOR_89            89U      /*!< Sector Number 89  */
#define FLASH_SECTOR_90            90U      /*!< Sector Number 90  */
#define FLASH_SECTOR_91            91U      /*!< Sector Number 91  */
#define FLASH_SECTOR_92            92U      /*!< Sector Number 92  */
#define FLASH_SECTOR_93            93U      /*!< Sector Number 93  */
#define FLASH_SECTOR_94            94U      /*!< Sector Number 94  */
#define FLASH_SECTOR_95            95U      /*!< Sector Number 95  */
#define FLASH_SECTOR_96            96U      /*!< Sector Number 96  */
#define FLASH_SECTOR_97            97U      /*!< Sector Number 97  */
#define FLASH_SECTOR_98            98U      /*!< Sector Number 98  */
#define FLASH_SECTOR_99            99U      /*!< Sector Number 99  */
#define FLASH_SECTOR_100           100U     /*!< Sector Number 100 */
#define FLASH_SECTOR_101           101U     /*!< Sector Number 101 */
#define FLASH_SECTOR_102           102U     /*!< Sector Number 102 */
#define FLASH_SECTOR_103           103U     /*!< Sector Number 103 */
#define FLASH_SECTOR_104           104U     /*!< Sector Number 104 */
#define FLASH_SECTOR_105           105U     /*!< Sector Number 105 */
#define FLASH_SECTOR_106           106U     /*!< Sector Number 106 */
#define FLASH_SECTOR_107           107U     /*!< Sector Number 107 */
#define FLASH_SECTOR_108           108U     /*!< Sector Number 108 */
#define FLASH_SECTOR_109           109U     /*!< Sector Number 109 */
#define FLASH_SECTOR_110           110U     /*!< Sector Number 110 */
#define FLASH_SECTOR_111           111U     /*!< Sector Number 111 */
#define FLASH_SECTOR_112           112U     /*!< Sector Number 112 */
#define FLASH_SECTOR_113           113U     /*!< Sector Number 113 */
#define FLASH_SECTOR_114           114U     /*!< Sector Number 114 */
#define FLASH_SECTOR_115           115U     /*!< Sector Number 115 */
#define FLASH_SECTOR_116           116U     /*!< Sector Number 116 */
#define FLASH_SECTOR_117           117U     /*!< Sector Number 117 */
#define FLASH_SECTOR_118           118U     /*!< Sector Number 118 */
#define FLASH_SECTOR_119           119U     /*!< Sector Number 119 */
#define FLASH_SECTOR_120           120U     /*!< Sector Number 120 */
#define FLASH_SECTOR_121           121U     /*!< Sector Number 121 */
#define FLASH_SECTOR_122           122U     /*!< Sector Number 122 */
#define FLASH_SECTOR_123           123U     /*!< Sector Number 123 */
#define FLASH_SECTOR_124           124U     /*!< Sector Number 124 */
#define FLASH_SECTOR_125           125U     /*!< Sector Number 125 */
#define FLASH_SECTOR_126           126U     /*!< Sector Number 126 */
#define FLASH_SECTOR_127           127U     /*!< Sector Number 127 */
#endif /* (FLASH_SECTOR_NB >= 128) */
#if (FLASH_SECTOR_NB >= 256)
#define FLASH_SECTOR_128           128U    /*!< Sector Number 128 */
#define FLASH_SECTOR_129           129U    /*!< Sector Number 129 */
#define FLASH_SECTOR_130           130U    /*!< Sector Number 130 */
#define FLASH_SECTOR_131           131U    /*!< Sector Number 131 */
#define FLASH_SECTOR_132           132U    /*!< Sector Number 132 */
#define FLASH_SECTOR_133           133U    /*!< Sector Number 133 */
#define FLASH_SECTOR_134           134U    /*!< Sector Number 134 */
#define FLASH_SECTOR_135           135U    /*!< Sector Number 135 */
#define FLASH_SECTOR_136           136U    /*!< Sector Number 136 */
#define FLASH_SECTOR_137           137U    /*!< Sector Number 137 */
#define FLASH_SECTOR_138           138U    /*!< Sector Number 138 */
#define FLASH_SECTOR_139           139U    /*!< Sector Number 139 */
#define FLASH_SECTOR_140           140U    /*!< Sector Number 140 */
#define FLASH_SECTOR_141           141U    /*!< Sector Number 141 */
#define FLASH_SECTOR_142           142U    /*!< Sector Number 142 */
#define FLASH_SECTOR_143           143U    /*!< Sector Number 143 */
#define FLASH_SECTOR_144           144U    /*!< Sector Number 144 */
#define FLASH_SECTOR_145           145U    /*!< Sector Number 145 */
#define FLASH_SECTOR_146           146U    /*!< Sector Number 146 */
#define FLASH_SECTOR_147           147U    /*!< Sector Number 147 */
#define FLASH_SECTOR_148           148U    /*!< Sector Number 148 */
#define FLASH_SECTOR_149           149U    /*!< Sector Number 149 */
#define FLASH_SECTOR_150           150U    /*!< Sector Number 150 */
#define FLASH_SECTOR_151           151U    /*!< Sector Number 151 */
#define FLASH_SECTOR_152           152U    /*!< Sector Number 152 */
#define FLASH_SECTOR_153           153U    /*!< Sector Number 153 */
#define FLASH_SECTOR_154           154U    /*!< Sector Number 154 */
#define FLASH_SECTOR_155           155U    /*!< Sector Number 155 */
#define FLASH_SECTOR_156           156U    /*!< Sector Number 156 */
#define FLASH_SECTOR_157           157U    /*!< Sector Number 157 */
#define FLASH_SECTOR_158           158U    /*!< Sector Number 158 */
#define FLASH_SECTOR_159           159U    /*!< Sector Number 159 */
#define FLASH_SECTOR_160           160U    /*!< Sector Number 160 */
#define FLASH_SECTOR_161           161U    /*!< Sector Number 161 */
#define FLASH_SECTOR_162           162U    /*!< Sector Number 162 */
#define FLASH_SECTOR_163           163U    /*!< Sector Number 163 */
#define FLASH_SECTOR_164           164U    /*!< Sector Number 164 */
#define FLASH_SECTOR_165           165U    /*!< Sector Number 165 */
#define FLASH_SECTOR_166           166U    /*!< Sector Number 166 */
#define FLASH_SECTOR_167           167U    /*!< Sector Number 167 */
#define FLASH_SECTOR_168           168U    /*!< Sector Number 168 */
#define FLASH_SECTOR_169           169U    /*!< Sector Number 169 */
#define FLASH_SECTOR_170           170U    /*!< Sector Number 170 */
#define FLASH_SECTOR_171           171U    /*!< Sector Number 171 */
#define FLASH_SECTOR_172           172U    /*!< Sector Number 172 */
#define FLASH_SECTOR_173           173U    /*!< Sector Number 173 */
#define FLASH_SECTOR_174           174U    /*!< Sector Number 174 */
#define FLASH_SECTOR_175           175U    /*!< Sector Number 175 */
#define FLASH_SECTOR_176           176U    /*!< Sector Number 176 */
#define FLASH_SECTOR_177           177U    /*!< Sector Number 177 */
#define FLASH_SECTOR_178           178U    /*!< Sector Number 178 */
#define FLASH_SECTOR_179           179U    /*!< Sector Number 179 */
#define FLASH_SECTOR_180           180U    /*!< Sector Number 180 */
#define FLASH_SECTOR_181           181U    /*!< Sector Number 181 */
#define FLASH_SECTOR_182           182U    /*!< Sector Number 182 */
#define FLASH_SECTOR_183           183U    /*!< Sector Number 183 */
#define FLASH_SECTOR_184           184U    /*!< Sector Number 184 */
#define FLASH_SECTOR_185           185U    /*!< Sector Number 185 */
#define FLASH_SECTOR_186           186U    /*!< Sector Number 186 */
#define FLASH_SECTOR_187           187U    /*!< Sector Number 187 */
#define FLASH_SECTOR_188           188U    /*!< Sector Number 188 */
#define FLASH_SECTOR_189           189U    /*!< Sector Number 189 */
#define FLASH_SECTOR_190           190U    /*!< Sector Number 190 */
#define FLASH_SECTOR_191           191U    /*!< Sector Number 191 */
#define FLASH_SECTOR_192           192U    /*!< Sector Number 192 */
#define FLASH_SECTOR_193           193U    /*!< Sector Number 193 */
#define FLASH_SECTOR_194           194U    /*!< Sector Number 194 */
#define FLASH_SECTOR_195           195U    /*!< Sector Number 195 */
#define FLASH_SECTOR_196           196U    /*!< Sector Number 196 */
#define FLASH_SECTOR_197           197U    /*!< Sector Number 197 */
#define FLASH_SECTOR_198           198U    /*!< Sector Number 198 */
#define FLASH_SECTOR_199           199U    /*!< Sector Number 199 */
#define FLASH_SECTOR_200           200U    /*!< Sector Number 200 */
#define FLASH_SECTOR_201           201U    /*!< Sector Number 201 */
#define FLASH_SECTOR_202           202U    /*!< Sector Number 202 */
#define FLASH_SECTOR_203           203U    /*!< Sector Number 203 */
#define FLASH_SECTOR_204           204U    /*!< Sector Number 204 */
#define FLASH_SECTOR_205           205U    /*!< Sector Number 205 */
#define FLASH_SECTOR_206           206U    /*!< Sector Number 206 */
#define FLASH_SECTOR_207           207U    /*!< Sector Number 207 */
#define FLASH_SECTOR_208           208U    /*!< Sector Number 208 */
#define FLASH_SECTOR_209           209U    /*!< Sector Number 209 */
#define FLASH_SECTOR_210           210U    /*!< Sector Number 210 */
#define FLASH_SECTOR_211           211U    /*!< Sector Number 211 */
#define FLASH_SECTOR_212           212U    /*!< Sector Number 212 */
#define FLASH_SECTOR_213           213U    /*!< Sector Number 213 */
#define FLASH_SECTOR_214           214U    /*!< Sector Number 214 */
#define FLASH_SECTOR_215           215U    /*!< Sector Number 215 */
#define FLASH_SECTOR_216           216U    /*!< Sector Number 216 */
#define FLASH_SECTOR_217           217U    /*!< Sector Number 217 */
#define FLASH_SECTOR_218           218U    /*!< Sector Number 218 */
#define FLASH_SECTOR_219           219U    /*!< Sector Number 219 */
#define FLASH_SECTOR_220           220U    /*!< Sector Number 220 */
#define FLASH_SECTOR_221           221U    /*!< Sector Number 221 */
#define FLASH_SECTOR_222           222U    /*!< Sector Number 222 */
#define FLASH_SECTOR_223           223U    /*!< Sector Number 223 */
#define FLASH_SECTOR_224           224U    /*!< Sector Number 224 */
#define FLASH_SECTOR_225           225U    /*!< Sector Number 225 */
#define FLASH_SECTOR_226           226U    /*!< Sector Number 226 */
#define FLASH_SECTOR_227           227U    /*!< Sector Number 227 */
#define FLASH_SECTOR_228           228U    /*!< Sector Number 228 */
#define FLASH_SECTOR_229           229U    /*!< Sector Number 229 */
#define FLASH_SECTOR_230           230U    /*!< Sector Number 230 */
#define FLASH_SECTOR_231           231U    /*!< Sector Number 231 */
#define FLASH_SECTOR_232           232U    /*!< Sector Number 232 */
#define FLASH_SECTOR_233           233U    /*!< Sector Number 233 */
#define FLASH_SECTOR_234           234U    /*!< Sector Number 234 */
#define FLASH_SECTOR_235           235U    /*!< Sector Number 235 */
#define FLASH_SECTOR_236           236U    /*!< Sector Number 236 */
#define FLASH_SECTOR_237           237U    /*!< Sector Number 237 */
#define FLASH_SECTOR_238           238U    /*!< Sector Number 238 */
#define FLASH_SECTOR_239           239U    /*!< Sector Number 239 */
#define FLASH_SECTOR_240           240U    /*!< Sector Number 240 */
#define FLASH_SECTOR_241           241U    /*!< Sector Number 241 */
#define FLASH_SECTOR_242           242U    /*!< Sector Number 242 */
#define FLASH_SECTOR_243           243U    /*!< Sector Number 243 */
#define FLASH_SECTOR_244           244U    /*!< Sector Number 244 */
#define FLASH_SECTOR_245           245U    /*!< Sector Number 245 */
#define FLASH_SECTOR_246           246U    /*!< Sector Number 246 */
#define FLASH_SECTOR_247           247U    /*!< Sector Number 247 */
#define FLASH_SECTOR_248           248U    /*!< Sector Number 248 */
#define FLASH_SECTOR_249           249U    /*!< Sector Number 249 */
#define FLASH_SECTOR_250           251U    /*!< Sector Number 251 */
#define FLASH_SECTOR_251           251U    /*!< Sector Number 251 */
#define FLASH_SECTOR_252           252U    /*!< Sector Number 252 */
#define FLASH_SECTOR_253           253U    /*!< Sector Number 253 */
#define FLASH_SECTOR_254           254U    /*!< Sector Number 254 */
#define FLASH_SECTOR_255           255U    /*!< Sector Number 255 */
#endif /* (FLASH_SECTOR_NB >= 256) */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros ------------------------------------------------------------*/
/** @defgroup FLASH_Exported_Macros FLASH Exported Macros
  * @{
  */
/**
  * @brief  Set the FLASH Latency.
  * @param  __LATENCY__: FLASH Latency
  *         This parameter can be one of the following values :
  *     @arg FLASH_LATENCY_0: FLASH Zero wait state
  *     @arg FLASH_LATENCY_1: FLASH One wait state
  *     @arg FLASH_LATENCY_2: FLASH Two wait states
  *     @arg FLASH_LATENCY_3: FLASH Three wait states
  *     @arg FLASH_LATENCY_4: FLASH Four wait states
  *     @arg FLASH_LATENCY_5: FLASH Five wait states
  *     @arg FLASH_LATENCY_6: FLASH Six wait states
  *     @arg FLASH_LATENCY_7: FLASH Seven wait states
  *     @arg FLASH_LATENCY_8: FLASH Eight wait states
  *     @arg FLASH_LATENCY_9: FLASH Nine wait states
  *     @arg FLASH_LATENCY_10: FLASH Ten wait states
  *     @arg FLASH_LATENCY_11: FLASH Eleven wait states
  *     @arg FLASH_LATENCY_12: FLASH Twelve wait states
  *     @arg FLASH_LATENCY_13: FLASH Thirteen wait states
  *     @arg FLASH_LATENCY_14: FLASH Fourteen wait states
  *     @arg FLASH_LATENCY_15: FLASH Fifteen wait states
  * @retval none
  */
#define __HAL_FLASH_SET_LATENCY(__LATENCY__)    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (__LATENCY__))

/**
  * @brief  Get the FLASH Latency.
  * @retval FLASH Latency
  *         This return value can be one of the following values :
  *     @arg FLASH_LATENCY_0: FLASH Zero wait state
  *     @arg FLASH_LATENCY_1: FLASH One wait state
  *     @arg FLASH_LATENCY_2: FLASH Two wait states
  *     @arg FLASH_LATENCY_3: FLASH Three wait states
  *     @arg FLASH_LATENCY_4: FLASH Four wait states
  *     @arg FLASH_LATENCY_5: FLASH Five wait states
  *     @arg FLASH_LATENCY_6: FLASH Six wait states
  *     @arg FLASH_LATENCY_7: FLASH Seven wait states
  *     @arg FLASH_LATENCY_8: FLASH Eight wait states
  *     @arg FLASH_LATENCY_9: FLASH Nine wait states
  *     @arg FLASH_LATENCY_10: FLASH Ten wait states
  *     @arg FLASH_LATENCY_11: FLASH Eleven wait states
  *     @arg FLASH_LATENCY_12: FLASH Twelve wait states
  *     @arg FLASH_LATENCY_13: FLASH Thirteen wait states
  *     @arg FLASH_LATENCY_14: FLASH Fourteen wait states
  *     @arg FLASH_LATENCY_15: FLASH Fifteen wait states
  */
#define __HAL_FLASH_GET_LATENCY()               READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY)

/**
  * @brief  Enable the specified FLASH interrupt.
  * @param  __INTERRUPT__ : FLASH interrupt
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_IT_EOP       : End of FLASH Operation Interrupt
  *     @arg FLASH_IT_WRPERR    : Write Protection Error Interrupt
  *     @arg FLASH_IT_PGSERR    : Program Sequence Error Interrupt
  *     @arg FLASH_IT_STRBERR   : Strobe Error Interrupt
  *     @arg FLASH_IT_INCERR    : Inconsistency Error Interrupt
  *     @arg FLASH_IT_OBKERR    : OBK Error Interrupt
  *     @arg FLASH_IT_OBKWERR   : OBK Write Error Interrupt
  *     @arg FLASH_IT_OPTCHANGEERR : Option Byte Change Error Interrupt
  *     @arg FLASH_IT_ECCC      : Single ECC Error Correction Interrupt
  * @retval none
  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* Enable secure FLASH interrupts from the secure world */
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__) do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U)                          \
                                                    { SET_BIT(FLASH->ECCCORR, FLASH_IT_ECCC); }                        \
                                                  if(((__INTERRUPT__) & FLASH_IT_OPTCHANGEERR) != 0U)                  \
                                                    { SET_BIT(FLASH->NSCR, FLASH_IT_OPTCHANGEERR); }                   \
                                                  if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U)                       \
                                                    { SET_BIT(FLASH->SECCR, ((__INTERRUPT__) & (~(FLASH_IT_ECCC |      \
                                                      FLASH_IT_OPTCHANGEERR)))); }\
                                                } while(0)
/* Enable non-secure FLASH interrupts from the secure world */
#define __HAL_FLASH_ENABLE_IT_NS(__INTERRUPT__) do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U)                       \
                                                       { SET_BIT(FLASH->ECCCORR, FLASH_IT_ECCC); }                     \
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U)                    \
                                                       { SET_BIT(FLASH->NSCR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); } \
                                                   } while(0)
#else
/* Enable non-secure FLASH interrupts from the non-secure world */
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__)    do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U)                       \
                                                       { SET_BIT(FLASH->ECCCORR, FLASH_IT_ECCC); }                     \
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U)                    \
                                                       { SET_BIT(FLASH->NSCR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); } \
                                                   } while(0)
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Disable the specified FLASH interrupt.
  * @param  __INTERRUPT__ : FLASH interrupt
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_IT_EOP       : End of FLASH Operation Interrupt
  *     @arg FLASH_IT_WRPERR    : Write Protection Error Interrupt
  *     @arg FLASH_IT_PGSERR    : Program Sequence Error Interrupt
  *     @arg FLASH_IT_STRBERR   : Strobe Error Interrupt
  *     @arg FLASH_IT_INCERR    : Inconsistency Error Interrupt
  *     @arg FLASH_IT_OBKERR    : OBK Error Interrupt
  *     @arg FLASH_IT_OBKWERR   : OBK Write Error Interrupt
  *     @arg FLASH_IT_OPTCHANGEERR : Option Byte Change Error Interrupt
  *     @arg FLASH_IT_ECCC      : Single ECC Error Correction Interrupt
  * @retval none
  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* Disable secure FLASH interrupts from the secure world */
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__) do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U)                         \
                                                     { CLEAR_BIT(FLASH->ECCCORR, FLASH_IT_ECCC); }                     \
                                                   if(((__INTERRUPT__) & FLASH_IT_OPTCHANGEERR) != 0U)                 \
                                                     { CLEAR_BIT(FLASH->NSCR, FLASH_IT_OPTCHANGEERR); }                \
                                                   if(((__INTERRUPT__) & (~(FLASH_IT_ECCC | FLASH_IT_OPTCHANGEERR)))   \
                                                     != 0U){ CLEAR_BIT(FLASH->SECCR, ((__INTERRUPT__) &                \
                                                     (~(FLASH_IT_ECCC | FLASH_IT_OPTCHANGEERR)))); }\
                                                 } while(0)
/* Disable non-secure FLASH interrupts from the secure world */
#define __HAL_FLASH_DISABLE_IT_NS(__INTERRUPT__) do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U) { CLEAR_BIT          \
                                                        (FLASH->ECCCORR, FLASH_IT_ECCC); }                             \
                                                      if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U)                   \
                                                        { CLEAR_BIT(FLASH->NSCR, ((__INTERRUPT__) & (~FLASH_IT_ECCC)));\
                                                        }                                                              \
                                                    } while(0)
#else
/* Disable non-secure FLASH interrupts from the non-secure world */
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__)   do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U) { CLEAR_BIT           \
                                                       (FLASH->ECCCORR, FLASH_IT_ECCC); }                              \
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U) { CLEAR_BIT        \
                                                       (FLASH->NSCR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); }          \
                                                    } while(0)
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  __FLAG__: specifies the FLASH flag to check.
  *   This parameter can be one of the following values :
  *     @arg FLASH_FLAG_BSY      : FLASH Busy flag
  *     @arg FLASH_FLAG_WBNE     : Write Buffer Not Empty flag
  *     @arg FLASH_FLAG_EOP      : End Of Operation flag
  *     @arg FLASH_FLAG_WRPERR   : Write Protection Error flag
  *     @arg FLASH_FLAG_PGSERR   : Program Sequence Error flag
  *     @arg FLASH_FLAG_STRBERR  : Strobe Error flag
  *     @arg FLASH_FLAG_INCERR   : Inconsistency Error flag
  *     @arg FLASH_FLAG_OBKERR   : OBK Error flag
  *     @arg FLASH_FLAG_OBKWERR  : OBK Write Error flag
  *     @arg FLASH_FLAG_OPTCHANGEERR : Option Byte Change Error flag
  *     @arg FLASH_FLAG_ECCC     : Single ECC Error Correction flag
  *     @arg FLASH_FLAG_ECCD     : Double Detection ECC Error flag
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* Get secure FLASH flags from the secure world */
#define __HAL_FLASH_GET_FLAG(__FLAG__)          ((((__FLAG__) & (FLASH_FLAG_ECCC)) != 0U) ?                            \
                                                 (READ_BIT(FLASH->ECCCORR, (__FLAG__)) == (__FLAG__))  :               \
                                                 (((__FLAG__) & (FLASH_FLAG_ECCD)) != 0U) ?                            \
                                                 (READ_BIT(FLASH->ECCDETR, (__FLAG__)) == (__FLAG__))  :               \
                                                 ((((__FLAG__) & (FLASH_FLAG_OPTCHANGEERR)) != 0U) ?                   \
                                                  (READ_BIT(FLASH->NSSR, (__FLAG__)) == (__FLAG__)) :                  \
                                                  (READ_BIT(FLASH->SECSR, (__FLAG__)) == (__FLAG__))))
/* Get non-secure FLASH flags from the secure world */
#define __HAL_FLASH_GET_FLAG_NS(__FLAG__)       ((((__FLAG__) & (FLASH_FLAG_ECCC)) != 0U) ?                            \
                                                 (READ_BIT(FLASH->ECCCORR, (__FLAG__)) == (__FLAG__))  :               \
                                                 (((__FLAG__) & (FLASH_FLAG_ECCD)) != 0U) ?                            \
                                                 (READ_BIT(FLASH->ECCDETR, (__FLAG__)) == (__FLAG__))  :               \
                                                 (READ_BIT(FLASH->NSSR, (__FLAG__)) == (__FLAG__))))
#else
/* Get non-secure FLASH flags from the non-secure world */
#define __HAL_FLASH_GET_FLAG(__FLAG__)          ((((__FLAG__) & (FLASH_FLAG_ECCC)) != 0U) ?                            \
                                                 (READ_BIT(FLASH->ECCCORR, (__FLAG__)) == (__FLAG__))  :               \
                                                 (((__FLAG__) & (FLASH_FLAG_ECCD)) != 0U) ?                            \
                                                 (READ_BIT(FLASH->ECCDETR, (__FLAG__)) == (__FLAG__))  :               \
                                                 (READ_BIT(FLASH->NSSR, (__FLAG__)) == (__FLAG__)))
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Clear the specified FLASH flag.
  * @param  __FLAG__: specifies the FLASH flags to clear.
  *   This parameter can be one of the following values :
  *     @arg FLASH_FLAG_BSY       : FLASH Busy flag
  *     @arg FLASH_FLAG_WBNE      : Write Buffer Not Empty flag
  *     @arg FLASH_FLAG_EOP       : End Of Operation flag
  *     @arg FLASH_FLAG_WRPERR    : Write Protection Error flag
  *     @arg FLASH_FLAG_PGSERR    : Program Sequence Error flag
  *     @arg FLASH_FLAG_STRBERR   : Strobe Error flag
  *     @arg FLASH_FLAG_INCERR    : Inconsistency Error flag
  *     @arg FLASH_FLAG_OBKERR    : OBK Error flag
  *     @arg FLASH_FLAG_OBKWERR   : OBK Write Error flag
  *     @arg FLASH_FLAG_OPTCHANGEERR : Option Byte Change Error flag
  *     @arg FLASH_FLAG_ECCC      : Single ECC Error Correction flag
  *     @arg FLASH_FLAG_ECCD      : Double Detection ECC Error flag
  *     @arg FLASH_FLAG_ALL_ERRORS: All errors flags
  * @retval none
  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* Clear secure FLASH flags from the secure world */
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)        do { if(((__FLAG__) & FLASH_FLAG_ECCC) != 0U) { SET_BIT(FLASH->ECCCORR,\
                                                        ((__FLAG__) & FLASH_FLAG_ECCC)); }                             \
                                                     if(((__FLAG__) & FLASH_FLAG_ECCD) != 0U) { SET_BIT(FLASH->ECCDETR,\
                                                        ((__FLAG__) & FLASH_FLAG_ECCD)); }                             \
                                                     if(((__FLAG__) & FLASH_FLAG_OPTCHANGEERR) != 0U) { SET_BIT        \
                                                         (FLASH->NSCCR, ((__FLAG__) & (FLASH_FLAG_OPTCHANGEERR))); }   \
                                                     if(((__FLAG__) & ~(FLASH_FLAG_ECCR_ERRORS |                       \
                                                          FLASH_FLAG_OPTCHANGEERR)) != 0U) { WRITE_REG(FLASH->SECCCR,  \
                                                        ((__FLAG__) & ~(FLASH_FLAG_ECCR_ERRORS |                       \
                                                          FLASH_FLAG_OPTCHANGEERR))); }                                \
                                                   } while(0)
/* Clear non-secure FLASH flags from the secure world */
#define __HAL_FLASH_CLEAR_FLAG_NS(__FLAG__)     do { if(((__FLAG__) & FLASH_FLAG_ECCC) != 0U) { SET_BIT(FLASH->ECCCORR,\
                                                        ((__FLAG__) & FLASH_FLAG_ECCC)); }                             \
                                                     if(((__FLAG__) & FLASH_FLAG_ECCD) != 0U) { SET_BIT(FLASH->ECCDETR,\
                                                        ((__FLAG__) & FLASH_FLAG_ECCD)); }                             \
                                                     if(((__FLAG__) & (~FLASH_FLAG_ECCR_ERRORS)) != 0U) { WRITE_REG    \
                                                         (FLASH->NSCCR, ((__FLAG__) & (~FLASH_FLAG_ECCR_ERRORS))); }   \
                                                   } while(0)
#else
/* Clear non-secure FLASH flags from the non-secure world */
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)        do { if(((__FLAG__) & FLASH_FLAG_ECCC) != 0U) { SET_BIT(FLASH->ECCCORR,\
                                                        ((__FLAG__) & FLASH_FLAG_ECCC)); }                             \
                                                     if(((__FLAG__) & FLASH_FLAG_ECCD) != 0U) { SET_BIT(FLASH->ECCDETR,\
                                                        ((__FLAG__) & FLASH_FLAG_ECCD)); }                             \
                                                     if(((__FLAG__) & (~FLASH_FLAG_ECCR_ERRORS)) != 0U) { WRITE_REG    \
                                                         (FLASH->NSCCR, ((__FLAG__) & (~FLASH_FLAG_ECCR_ERRORS))); }   \
                                                   } while(0)
#endif /* __ARM_FEATURE_CMSE */

/**
  * @}
  */

/* Include FLASH HAL Extension module */
#include "stm32h5xx_hal_flash_ex.h"

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASH_Exported_Functions
  * @{
  */
/** @addtogroup FLASH_Exported_Functions_Group1
  * @{
  */
/* Program operation functions */
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t FlashAddress, uint32_t DataAddress);
HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t FlashAddress, uint32_t DataAddress);
/* FLASH IRQ handler method */
void HAL_FLASH_IRQHandler(void);
/* Callbacks in non blocking modes */
void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);
/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group2
  * @{
  */
/* Peripheral Control functions */
HAL_StatusTypeDef HAL_FLASH_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Lock(void);
/* Option bytes control */
HAL_StatusTypeDef HAL_FLASH_OB_Launch(void);
HAL_StatusTypeDef HAL_FLASHEx_PUF_Launch(void);

/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group3
  * @{
  */
/* Peripheral State functions */
uint32_t HAL_FLASH_GetError(void);
/**
  * @}
  */

/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup FLASH_Private_Variables FLASH Private Variables
  * @{
  */
extern FLASH_ProcessTypeDef pFlash;
/**
  * @}
  */
/* Private constants ---------------------------------------------------------*/
/** @defgroup FLASH_Private_Constants FLASH Private Constants
  * @{
  */
#define FLASH_TIMEOUT_VALUE             1000U   /*!< 1 s */

#if defined (FLASH_SR_OBKERR)
#define FLASH_OBK                       0x10000000U
#endif /* FLASH_SR_OBKERR */

#define FLASH_OTP                       0x20000000U

#if defined (FLASH_EDATAR_EDATA_EN)
#define FLASH_EDATA_HALFWORD            0x40000000U
#define FLASH_EDATA_WORD                0x60000000U
#endif /* FLASH_EDATAR_EDATA_EN */

#define FLASH_NON_SECURE_MASK           0x80000000U

#if !defined(STM32H5F5xx)
#define FLASH_EDATA_SECTOR_NB           8U       /*!< Maximum number of FLASH high-cycle data sectors */
#else
#define FLASH_EDATA_SECTOR_NB           16U      /*!< Maximum number of FLASH high-cycle data sectors */
#endif /* STM32H5F5xx */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup FLASH_Private_Macros FLASH Private Macros
  * @{
  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#if defined (FLASH_SR_OBKERR) && defined (FLASH_EDATAR_EDATA_EN)
#define IS_FLASH_TYPEPROGRAM(VALUE)      (((VALUE) == FLASH_TYPEPROGRAM_QUADWORD)                                   || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_QUADWORD_NS)                                || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_HALFWORD_OTP)                               || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_HALFWORD_EDATA)                             || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_HALFWORD_EDATA_NS)                          || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_WORD_EDATA)                                 || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_WORD_EDATA_NS)                              || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_QUADWORD_OBK)                               || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_QUADWORD_OBK_ALT))
#else
#define IS_FLASH_TYPEPROGRAM(VALUE)      (((VALUE) == FLASH_TYPEPROGRAM_QUADWORD)                                   || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_QUADWORD_NS)                                || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_HALFWORD_OTP))
#endif /* FLASH_SR_OBKERR && FLASH_EDATAR_EDATA_EN */
#else
#if defined (FLASH_SR_OBKERR) && defined (FLASH_EDATAR_EDATA_EN)
#define IS_FLASH_TYPEPROGRAM(VALUE)      (((VALUE) == FLASH_TYPEPROGRAM_QUADWORD)                                   || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_HALFWORD_OTP)                               || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_HALFWORD_EDATA)                             || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_WORD_EDATA)                                 || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_QUADWORD_OBK)                               || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_QUADWORD_OBK_ALT))
#else
#define IS_FLASH_TYPEPROGRAM(VALUE)      (((VALUE) == FLASH_TYPEPROGRAM_QUADWORD)                                   || \
                                          ((VALUE) == FLASH_TYPEPROGRAM_HALFWORD_OTP))
#endif /* FLASH_SR_OBKERR && FLASH_EDATAR_EDATA_EN */
#endif /* __ARM_FEATURE_CMSE */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_FLASH_USER_MEM_ADDRESS(ADDRESS) ((((ADDRESS) >= FLASH_BASE) && ((ADDRESS) < (FLASH_BASE+FLASH_SIZE)))    || \
                                            (((ADDRESS) >= FLASH_BASE_NS) && ((ADDRESS) < (FLASH_BASE_NS+FLASH_SIZE))))
#if defined (FLASH_SR_OBKERR)
#define IS_FLASH_OBK_ADDRESS(ADDRESS)      ((((ADDRESS) >= FLASH_OBK_BASE)                                          && \
                                             ((ADDRESS)  < (FLASH_OBK_BASE+FLASH_OBK_SIZE)))                        || \
                                            (((ADDRESS) >= FLASH_OBK_BASE_NS)                                       && \
                                             ((ADDRESS)  < (FLASH_OBK_BASE_NS+FLASH_OBK_SIZE))))
#endif /* FLASH_SR_OBKERR */
#if defined (FLASH_EDATAR_EDATA_EN)
#define IS_FLASH_EDATA_ADDRESS(ADDRESS)    ((((ADDRESS) >= FLASH_EDATA_BASE_S)                                      && \
                                             ((ADDRESS)  < (FLASH_EDATA_BASE_S+FLASH_EDATA_SIZE)))                  || \
                                            (((ADDRESS) >= FLASH_EDATA_BASE_NS)                                     && \
                                             ((ADDRESS)  < (FLASH_EDATA_BASE_NS+FLASH_EDATA_SIZE))))
#endif /* FLASH_EDATAR_EDATA_EN */
#else
#define IS_FLASH_USER_MEM_ADDRESS(ADDRESS) (((ADDRESS) >= FLASH_BASE)                                               && \
                                            ((ADDRESS) < (FLASH_BASE+FLASH_SIZE)))
#if defined (FLASH_SR_OBKERR)
#define IS_FLASH_OBK_ADDRESS(ADDRESS)      (((ADDRESS) >= FLASH_OBK_BASE)                                           && \
                                            ((ADDRESS) < (FLASH_OBK_BASE + FLASH_OBK_SIZE)))
#endif /* FLASH_SR_OBKERR */
#if defined (FLASH_EDATAR_EDATA_EN)
#define IS_FLASH_EDATA_ADDRESS(ADDRESS)    (((ADDRESS) >= FLASH_EDATA_BASE_NS)                                      && \
                                            ((ADDRESS) < (FLASH_EDATA_BASE_NS + FLASH_EDATA_SIZE)))
#endif /* FLASH_EDATAR_EDATA_EN */
#endif /* __ARM_FEATURE_CMSE */

#define IS_FLASH_OTP_ADDRESS(ADDRESS)      (((ADDRESS) >= FLASH_OTP_BASE)                                           && \
                                            ((ADDRESS) < (FLASH_OTP_BASE + FLASH_OTP_SIZE)))

#define IS_FLASH_BANK(BANK)              (((BANK) == FLASH_BANK_1)                                                  || \
                                          ((BANK) == FLASH_BANK_2)                                                  || \
                                          ((BANK) == FLASH_BANK_BOTH))

#define IS_FLASH_BANK_EXCLUSIVE(BANK)    (((BANK) == FLASH_BANK_1)                                                  || \
                                          ((BANK) == FLASH_BANK_2))

#define IS_FLASH_SECTOR(SECTOR)          ((SECTOR) < FLASH_SECTOR_NB)

#define IS_FLASH_LATENCY(LATENCY)        (((LATENCY) == FLASH_LATENCY_0)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_1)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_2)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_3)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_4)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_5)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_6)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_7)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_8)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_9)                                            || \
                                          ((LATENCY) == FLASH_LATENCY_10)                                           || \
                                          ((LATENCY) == FLASH_LATENCY_11)                                           || \
                                          ((LATENCY) == FLASH_LATENCY_12)                                           || \
                                          ((LATENCY) == FLASH_LATENCY_13)                                           || \
                                          ((LATENCY) == FLASH_LATENCY_14)                                           || \
                                          ((LATENCY) == FLASH_LATENCY_15))

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_FLASH_SECURE_OPERATION()        ((pFlash.ProcedureOnGoing & FLASH_NON_SECURE_MASK) == 0U)
#else
#define IS_FLASH_SECURE_OPERATION()        (1U == 0U)
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */
/* Private functions ---------------------------------------------------------*/
/** @defgroup FLASH_Private_Functions FLASH Private Functions
  * @{
  */
HAL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H5xx_HAL_FLASH_H */
