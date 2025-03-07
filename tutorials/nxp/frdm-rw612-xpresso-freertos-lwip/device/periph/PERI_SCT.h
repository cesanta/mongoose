/*
** ###################################################################
**     Processors:          RW610ETA2I
**                          RW610HNA2I
**                          RW610UKA2I
**                          RW612ETA2I
**                          RW612HNA2I
**                          RW612UKA2I
**
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b240715
**
**     Abstract:
**         CMSIS Peripheral Access Layer for SCT
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
 * @file SCT.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SCT
 *
 * CMSIS Peripheral Access Layer for SCT
 */

#if !defined(SCT_H_)
#define SCT_H_                                   /**< Symbol preventing repeated inclusion */

#if (defined(CPU_RW610ETA2I) || defined(CPU_RW610HNA2I) || defined(CPU_RW610UKA2I))
#include "RW610_COMMON.h"
#elif (defined(CPU_RW612ETA2I) || defined(CPU_RW612HNA2I) || defined(CPU_RW612UKA2I))
#include "RW612_COMMON.h"
#else
  #error "No valid CPU defined!"
#endif

/* ----------------------------------------------------------------------------
   -- Device Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_access_layer Device Peripheral Access Layer
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic push
  #else
    #pragma push
    #pragma anon_unions
  #endif
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- SCT Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCT_Peripheral_Access_Layer SCT Peripheral Access Layer
 * @{
 */

/** SCT - Size of Registers Arrays */
#define SCT_CAP_MATCH_CAP_CAP_COUNT               16u
#define SCT_CAP_MATCH_MATCH_MATCH_COUNT           16u
#define SCT_CAPCTRL_MATCHREL_CAPCTRL_SCTCAPCTRL_COUNT 16u
#define SCT_CAPCTRL_MATCHREL_MATCHREL_MATCHREL_COUNT 16u
#define SCT_EV_COUNT                              16u
#define SCT_OUT_COUNT                             10u

/** SCT - Register Layout Typedef */
typedef struct {
  __IO uint32_t CONFIG;                            /**< SCTimer Configuration, offset: 0x0 */
  union {                                          /* offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      __IO uint16_t CTRLL;                             /**< SCT_CTRLL register, offset: 0x4 */
      __IO uint16_t CTRLH;                             /**< SCT_CTRLH register, offset: 0x6 */
    } CTRL_ACCESS16BIT;
    __IO uint32_t CTRL;                              /**< SCT Control, offset: 0x4 */
  };
  union {                                          /* offset: 0x8 */
    struct {                                         /* offset: 0x8 */
      __IO uint16_t LIMITL;                            /**< SCT_LIMITL register, offset: 0x8 */
      __IO uint16_t LIMITH;                            /**< SCT_LIMITH register, offset: 0xA */
    } LIMIT_ACCESS16BIT;
    __IO uint32_t LIMIT;                             /**< SCT Limit Event Select, offset: 0x8 */
  };
  union {                                          /* offset: 0xC */
    struct {                                         /* offset: 0xC */
      __IO uint16_t HALTL;                             /**< SCT_HALTL register, offset: 0xC */
      __IO uint16_t HALTH;                             /**< SCT_HALTH register, offset: 0xE */
    } HALT_ACCESS16BIT;
    __IO uint32_t HALT;                              /**< Halt Event Select, offset: 0xC */
  };
  union {                                          /* offset: 0x10 */
    struct {                                         /* offset: 0x10 */
      __IO uint16_t STOPL;                             /**< SCT_STOPL register, offset: 0x10 */
      __IO uint16_t STOPH;                             /**< SCT_STOPH register, offset: 0x12 */
    } STOP_ACCESS16BIT;
    __IO uint32_t STOP;                              /**< Stop Event Select, offset: 0x10 */
  };
  union {                                          /* offset: 0x14 */
    struct {                                         /* offset: 0x14 */
      __IO uint16_t STARTL;                            /**< SCT_STARTL register, offset: 0x14 */
      __IO uint16_t STARTH;                            /**< SCT_STARTH register, offset: 0x16 */
    } START_ACCESS16BIT;
    __IO uint32_t START;                             /**< Start Event Select, offset: 0x14 */
  };
       uint8_t RESERVED_0[40];
  union {                                          /* offset: 0x40 */
    struct {                                         /* offset: 0x40 */
      __IO uint16_t COUNTL;                            /**< SCT_COUNTL register, offset: 0x40 */
      __IO uint16_t COUNTH;                            /**< SCT_COUNTH register, offset: 0x42 */
    } COUNT_ACCESS16BIT;
    __IO uint32_t COUNT;                             /**< Counter, offset: 0x40 */
  };
  union {                                          /* offset: 0x44 */
    struct {                                         /* offset: 0x44 */
      __IO uint16_t STATEL;                            /**< SCT_STATEL register, offset: 0x44 */
      __IO uint16_t STATEH;                            /**< SCT_STATEH register, offset: 0x46 */
    } STATE_ACCESS16BIT;
    __IO uint32_t STATE;                             /**< State, offset: 0x44 */
  };
  __I  uint32_t INPUT;                             /**< Input, offset: 0x48 */
  union {                                          /* offset: 0x4C */
    struct {                                         /* offset: 0x4C */
      __IO uint16_t REGMODEL;                          /**< SCT_REGMODEL register, offset: 0x4C */
      __IO uint16_t REGMODEH;                          /**< SCT_REGMODEH register, offset: 0x4E */
    } REGMODE_ACCESS16BIT;
    __IO uint32_t REGMODE;                           /**< Match/Capture Mode, offset: 0x4C */
  };
  __IO uint32_t OUTPUT;                            /**< Output, offset: 0x50 */
  __IO uint32_t OUTPUTDIRCTRL;                     /**< Output Counter Direction Control, offset: 0x54 */
  __IO uint32_t RES;                               /**< Output Conflict Resolution, offset: 0x58 */
  __IO uint32_t DMAREQ0;                           /**< DMA Request 0, offset: 0x5C */
  __IO uint32_t DMAREQ1;                           /**< DMA Request 1, offset: 0x60 */
       uint8_t RESERVED_1[140];
  __IO uint32_t EVEN;                              /**< Event Interrupt Enable, offset: 0xF0 */
  __IO uint32_t EVFLAG;                            /**< Event Flag, offset: 0xF4 */
  __IO uint32_t CONEN;                             /**< Conflict Interrupt Enable, offset: 0xF8 */
  __IO uint32_t CONFLAG;                           /**< Conflict Flag, offset: 0xFC */
  union {                                          /* offset: 0x100 */
    union {                                          /* offset: 0x100, array step: 0x4 */
      struct {                                         /* offset: 0x100, array step: 0x4 */
        __IO uint16_t CAPL;                              /**< SCT_CAPL register, array offset: 0x100, array step: 0x4 */
        __IO uint16_t CAPH;                              /**< SCT_CAPH register, array offset: 0x102, array step: 0x4 */
      } CAP_ACCESS16BIT[SCT_CAP_MATCH_CAP_CAP_COUNT];
      __IO uint32_t CAP[SCT_CAP_MATCH_CAP_CAP_COUNT];     /**< Capture Value, array offset: 0x100, array step: 0x4 */
    };
    union {                                          /* offset: 0x100, array step: 0x4 */
      struct {                                         /* offset: 0x100, array step: 0x4 */
        __IO uint16_t MATCHL;                            /**< SCT_MATCHL register, array offset: 0x100, array step: 0x4 */
        __IO uint16_t MATCHH;                            /**< SCT_MATCHH register, array offset: 0x102, array step: 0x4 */
      } MATCH_ACCESS16BIT[SCT_CAP_MATCH_MATCH_MATCH_COUNT];
      __IO uint32_t MATCH[SCT_CAP_MATCH_MATCH_MATCH_COUNT];     /**< Match Value, array offset: 0x100, array step: 0x4 */
    };
  };
       uint8_t RESERVED_2[192];
  union {                                          /* offset: 0x200 */
    union {                                          /* offset: 0x200, array step: 0x4 */
      struct {                                         /* offset: 0x200, array step: 0x4 */
        __IO uint16_t CAPCTRLL;                          /**< SCT_CAPCTRLL register, array offset: 0x200, array step: 0x4 */
        __IO uint16_t CAPCTRLH;                          /**< SCT_CAPCTRLH register, array offset: 0x202, array step: 0x4 */
      } CAPCTRL_ACCESS16BIT[SCT_CAPCTRL_MATCHREL_CAPCTRL_SCTCAPCTRL_COUNT];
      __IO uint32_t CAPCTRL[SCT_CAPCTRL_MATCHREL_CAPCTRL_SCTCAPCTRL_COUNT];     /**< Capture Control, array offset: 0x200, array step: 0x4 */
    };
    union {                                          /* offset: 0x200, array step: 0x4 */
      struct {                                         /* offset: 0x200, array step: 0x4 */
        __IO uint16_t MATCHRELL;                         /**< SCT_MATCHRELL register, array offset: 0x200, array step: 0x4 */
        __IO uint16_t MATCHRELH;                         /**< SCT_MATCHRELH register, array offset: 0x202, array step: 0x4 */
      } MATCHREL_ACCESS16BIT[SCT_CAPCTRL_MATCHREL_MATCHREL_MATCHREL_COUNT];
      __IO uint32_t MATCHREL[SCT_CAPCTRL_MATCHREL_MATCHREL_MATCHREL_COUNT];     /**< Match Reload Value, array offset: 0x200, array step: 0x4 */
    };
  };
       uint8_t RESERVED_3[192];
  struct {                                         /* offset: 0x300, array step: 0x8 */
    __IO uint32_t STATE;                             /**< Event n State, array offset: 0x300, array step: 0x8 */
    __IO uint32_t CTRL;                              /**< Event n Control, array offset: 0x304, array step: 0x8 */
  } EV[SCT_EV_COUNT];
       uint8_t RESERVED_4[384];
  struct {                                         /* offset: 0x500, array step: 0x8 */
    __IO uint32_t SET;                               /**< Output n Set, array offset: 0x500, array step: 0x8 */
    __IO uint32_t CLR;                               /**< Output n Clear, array offset: 0x504, array step: 0x8 */
  } OUT[SCT_OUT_COUNT];
} SCT_Type;

/* ----------------------------------------------------------------------------
   -- SCT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCT_Register_Masks SCT Register Masks
 * @{
 */

/*! @name CONFIG - SCTimer Configuration */
/*! @{ */

#define SCT_CONFIG_UNIFY_MASK                    (0x1U)
#define SCT_CONFIG_UNIFY_SHIFT                   (0U)
/*! UNIFY - SCT Operation
 *  0b0..Dual counter. The SCT operates as two 16-bit counters named COUNTER_L and COUNTER_H.
 *  0b1..Unified counter. The SCT operates as a unified 32-bit counter.
 */
#define SCT_CONFIG_UNIFY(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_CONFIG_UNIFY_SHIFT)) & SCT_CONFIG_UNIFY_MASK)

#define SCT_CONFIG_CLKMODE_MASK                  (0x6U)
#define SCT_CONFIG_CLKMODE_SHIFT                 (1U)
/*! CLKMODE - SCT Clock Mode
 *  0b00..System Clock Mode. The system clock clocks the entire SCT module including all counters and counter prescalers.
 *  0b01..Sampled System Clock Mode. The system clock clocks the SCT module, but the counter and prescalers are
 *        only enabled to count when the designated edge is detected on the input selected by the CKSEL field. The
 *        minimum pulse width on the selected clock-gate input is 1 bus clock period. This mode is the
 *        high-performance, sampled-clock mode.
 *  0b10..SCT Input Clock Mode. The input/edge selected by the CKSEL field clocks the SCT module, including all
 *        counters and prescalers, after first being synchronized to the system clock. The minimum pulse width on the
 *        clock input is 1 bus clock period. This mode is the low-power, sampled-clock mode.
 *  0b11..Asynchronous Mode. The entire SCT module is clocked directly by the input/edge selected by the CKSEL
 *        field. In this mode, the SCT outputs are switched synchronously to the SCT input clock - not the system
 *        clock. The input clock rate must be at least half the system clock rate and can be the same or faster than
 *        the system clock.
 */
#define SCT_CONFIG_CLKMODE(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_CONFIG_CLKMODE_SHIFT)) & SCT_CONFIG_CLKMODE_MASK)

#define SCT_CONFIG_CKSEL_MASK                    (0x78U)
#define SCT_CONFIG_CKSEL_SHIFT                   (3U)
/*! CKSEL - SCT Clock Select. The specific functionality of the designated input/edge is dependent
 *    on the CLKMODE bit selection in this register.
 *  0b0000..Rising edges on input 0
 *  0b0001..Falling edges on input 0
 *  0b0010..Rising edges on input 1
 *  0b0011..Falling edges on input 1
 *  0b0100..Rising edges on input 2
 *  0b0101..Falling edges on input 2
 *  0b0110..Rising edges on input 3
 *  0b0111..Falling edges on input 3
 *  0b1000..Rising edges on input 4
 *  0b1001..Falling edges on input 4
 *  0b1010..Rising edges on input 5
 *  0b1011..Falling edges on input 5
 *  0b1100..Rising edges on input 6
 *  0b1101..Falling edges on input 6
 *  0b1110..Rising edges on input 7
 *  0b1111..Falling edges on input 7
 */
#define SCT_CONFIG_CKSEL(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_CONFIG_CKSEL_SHIFT)) & SCT_CONFIG_CKSEL_MASK)

#define SCT_CONFIG_NORELOAD_L_MASK               (0x80U)
#define SCT_CONFIG_NORELOAD_L_SHIFT              (7U)
/*! NORELOAD_L - No Reload Lower Match
 *  0b0..Reload. The default setting.
 *  0b1..No Reload. Prevents the lower match registers from being reloaded from their respective reload registers.
 */
#define SCT_CONFIG_NORELOAD_L(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_CONFIG_NORELOAD_L_SHIFT)) & SCT_CONFIG_NORELOAD_L_MASK)

#define SCT_CONFIG_NORELOAD_H_MASK               (0x100U)
#define SCT_CONFIG_NORELOAD_H_SHIFT              (8U)
/*! NORELOAD_H - No Reload Higher Match
 *  0b0..Reload. The default setting.
 *  0b1..No Reload. Prevents the higher match registers from being reloaded from their respective reload registers.
 */
#define SCT_CONFIG_NORELOAD_H(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_CONFIG_NORELOAD_H_SHIFT)) & SCT_CONFIG_NORELOAD_H_MASK)

#define SCT_CONFIG_INSYNC_MASK                   (0x1FE00U)
#define SCT_CONFIG_INSYNC_SHIFT                  (9U)
/*! INSYNC - Input Synchronization */
#define SCT_CONFIG_INSYNC(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_CONFIG_INSYNC_SHIFT)) & SCT_CONFIG_INSYNC_MASK)

#define SCT_CONFIG_AUTOLIMIT_L_MASK              (0x20000U)
#define SCT_CONFIG_AUTOLIMIT_L_SHIFT             (17U)
/*! AUTOLIMIT_L - Auto Limit Lower
 *  0b0..Disable.
 *  0b1..Enable. A match on match register 0 is the LIMIT condition. No need to define an associated event.
 */
#define SCT_CONFIG_AUTOLIMIT_L(x)                (((uint32_t)(((uint32_t)(x)) << SCT_CONFIG_AUTOLIMIT_L_SHIFT)) & SCT_CONFIG_AUTOLIMIT_L_MASK)

#define SCT_CONFIG_AUTOLIMIT_H_MASK              (0x40000U)
#define SCT_CONFIG_AUTOLIMIT_H_SHIFT             (18U)
/*! AUTOLIMIT_H - Auto Limit Higher
 *  0b0..Disable.
 *  0b1..Enable. A match on match register 0 is the LIMIT condition. No need to define an associated event.
 */
#define SCT_CONFIG_AUTOLIMIT_H(x)                (((uint32_t)(((uint32_t)(x)) << SCT_CONFIG_AUTOLIMIT_H_SHIFT)) & SCT_CONFIG_AUTOLIMIT_H_MASK)
/*! @} */

/*! @name CTRLL - SCT_CTRLL register */
/*! @{ */

#define SCT_CTRLL_DOWN_L_MASK                    (0x1U)
#define SCT_CTRLL_DOWN_L_SHIFT                   (0U)
/*! DOWN_L - Down Counter Low
 *  0b0..Up. The L or unified counter is counting up.
 *  0b1..Down. The L or unified counter is counting down.
 */
#define SCT_CTRLL_DOWN_L(x)                      (((uint16_t)(((uint16_t)(x)) << SCT_CTRLL_DOWN_L_SHIFT)) & SCT_CTRLL_DOWN_L_MASK)

#define SCT_CTRLL_STOP_L_MASK                    (0x2U)
#define SCT_CTRLL_STOP_L_SHIFT                   (1U)
/*! STOP_L - Stop Counter Low
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_CTRLL_STOP_L(x)                      (((uint16_t)(((uint16_t)(x)) << SCT_CTRLL_STOP_L_SHIFT)) & SCT_CTRLL_STOP_L_MASK)

#define SCT_CTRLL_HALT_L_MASK                    (0x4U)
#define SCT_CTRLL_HALT_L_SHIFT                   (2U)
/*! HALT_L - Halt Counter Low
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_CTRLL_HALT_L(x)                      (((uint16_t)(((uint16_t)(x)) << SCT_CTRLL_HALT_L_SHIFT)) & SCT_CTRLL_HALT_L_MASK)

#define SCT_CTRLL_CLRCTR_L_MASK                  (0x8U)
#define SCT_CTRLL_CLRCTR_L_SHIFT                 (3U)
/*! CLRCTR_L - Clear Counter Low */
#define SCT_CTRLL_CLRCTR_L(x)                    (((uint16_t)(((uint16_t)(x)) << SCT_CTRLL_CLRCTR_L_SHIFT)) & SCT_CTRLL_CLRCTR_L_MASK)

#define SCT_CTRLL_BIDIR_L_MASK                   (0x10U)
#define SCT_CTRLL_BIDIR_L_SHIFT                  (4U)
/*! BIDIR_L - Bidirectional Select Low
 *  0b0..Up. The counter counts up to a limit condition, then is cleared to zero.
 *  0b1..Up-down. The counter counts up to a limit, then counts down to a limit condition or to 0.
 */
#define SCT_CTRLL_BIDIR_L(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_CTRLL_BIDIR_L_SHIFT)) & SCT_CTRLL_BIDIR_L_MASK)

#define SCT_CTRLL_PRE_L_MASK                     (0x1FE0U)
#define SCT_CTRLL_PRE_L_SHIFT                    (5U)
/*! PRE_L - Prescaler for Low Counter */
#define SCT_CTRLL_PRE_L(x)                       (((uint16_t)(((uint16_t)(x)) << SCT_CTRLL_PRE_L_SHIFT)) & SCT_CTRLL_PRE_L_MASK)
/*! @} */

/*! @name CTRLH - SCT_CTRLH register */
/*! @{ */

#define SCT_CTRLH_DOWN_H_MASK                    (0x1U)
#define SCT_CTRLH_DOWN_H_SHIFT                   (0U)
/*! DOWN_H - Down Counter High
 *  0b0..Up. The H counter is counting up.
 *  0b1..Down. The H counter is counting down.
 */
#define SCT_CTRLH_DOWN_H(x)                      (((uint16_t)(((uint16_t)(x)) << SCT_CTRLH_DOWN_H_SHIFT)) & SCT_CTRLH_DOWN_H_MASK)

#define SCT_CTRLH_STOP_H_MASK                    (0x2U)
#define SCT_CTRLH_STOP_H_SHIFT                   (1U)
/*! STOP_H - Stop Counter High
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_CTRLH_STOP_H(x)                      (((uint16_t)(((uint16_t)(x)) << SCT_CTRLH_STOP_H_SHIFT)) & SCT_CTRLH_STOP_H_MASK)

#define SCT_CTRLH_HALT_H_MASK                    (0x4U)
#define SCT_CTRLH_HALT_H_SHIFT                   (2U)
/*! HALT_H - Halt Counter High
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_CTRLH_HALT_H(x)                      (((uint16_t)(((uint16_t)(x)) << SCT_CTRLH_HALT_H_SHIFT)) & SCT_CTRLH_HALT_H_MASK)

#define SCT_CTRLH_CLRCTR_H_MASK                  (0x8U)
#define SCT_CTRLH_CLRCTR_H_SHIFT                 (3U)
/*! CLRCTR_H - Clear Counter High */
#define SCT_CTRLH_CLRCTR_H(x)                    (((uint16_t)(((uint16_t)(x)) << SCT_CTRLH_CLRCTR_H_SHIFT)) & SCT_CTRLH_CLRCTR_H_MASK)

#define SCT_CTRLH_BIDIR_H_MASK                   (0x10U)
#define SCT_CTRLH_BIDIR_H_SHIFT                  (4U)
/*! BIDIR_H - Bidirectional Select High
 *  0b0..Up. The H counter counts up to its limit condition, then is cleared to zero.
 *  0b1..Up-down. The H counter counts up to its limit, then counts down to a limit condition or to 0.
 */
#define SCT_CTRLH_BIDIR_H(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_CTRLH_BIDIR_H_SHIFT)) & SCT_CTRLH_BIDIR_H_MASK)

#define SCT_CTRLH_PRE_H_MASK                     (0x1FE0U)
#define SCT_CTRLH_PRE_H_SHIFT                    (5U)
/*! PRE_H - Prescaler for High Counter */
#define SCT_CTRLH_PRE_H(x)                       (((uint16_t)(((uint16_t)(x)) << SCT_CTRLH_PRE_H_SHIFT)) & SCT_CTRLH_PRE_H_MASK)
/*! @} */

/*! @name CTRL - SCT Control */
/*! @{ */

#define SCT_CTRL_DOWN_L_MASK                     (0x1U)
#define SCT_CTRL_DOWN_L_SHIFT                    (0U)
/*! DOWN_L - Down Counter Low
 *  0b0..Up. The L or unified counter is counting up.
 *  0b1..Down. The L or unified counter is counting down.
 */
#define SCT_CTRL_DOWN_L(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_DOWN_L_SHIFT)) & SCT_CTRL_DOWN_L_MASK)

#define SCT_CTRL_STOP_L_MASK                     (0x2U)
#define SCT_CTRL_STOP_L_SHIFT                    (1U)
/*! STOP_L - Stop Counter Low
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_CTRL_STOP_L(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_STOP_L_SHIFT)) & SCT_CTRL_STOP_L_MASK)

#define SCT_CTRL_HALT_L_MASK                     (0x4U)
#define SCT_CTRL_HALT_L_SHIFT                    (2U)
/*! HALT_L - Halt Counter Low
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_CTRL_HALT_L(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_HALT_L_SHIFT)) & SCT_CTRL_HALT_L_MASK)

#define SCT_CTRL_CLRCTR_L_MASK                   (0x8U)
#define SCT_CTRL_CLRCTR_L_SHIFT                  (3U)
/*! CLRCTR_L - Clear Counter Low */
#define SCT_CTRL_CLRCTR_L(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_CLRCTR_L_SHIFT)) & SCT_CTRL_CLRCTR_L_MASK)

#define SCT_CTRL_BIDIR_L_MASK                    (0x10U)
#define SCT_CTRL_BIDIR_L_SHIFT                   (4U)
/*! BIDIR_L - Bidirectional Select Low
 *  0b0..Up. The counter counts up to a limit condition, then is cleared to zero.
 *  0b1..Up-down. The counter counts up to a limit, then counts down to a limit condition or to 0.
 */
#define SCT_CTRL_BIDIR_L(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_BIDIR_L_SHIFT)) & SCT_CTRL_BIDIR_L_MASK)

#define SCT_CTRL_PRE_L_MASK                      (0x1FE0U)
#define SCT_CTRL_PRE_L_SHIFT                     (5U)
/*! PRE_L - Prescaler for Low Counter */
#define SCT_CTRL_PRE_L(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_PRE_L_SHIFT)) & SCT_CTRL_PRE_L_MASK)

#define SCT_CTRL_DOWN_H_MASK                     (0x10000U)
#define SCT_CTRL_DOWN_H_SHIFT                    (16U)
/*! DOWN_H - Down Counter High
 *  0b0..Up. The H counter is counting up.
 *  0b1..Down. The H counter is counting down.
 */
#define SCT_CTRL_DOWN_H(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_DOWN_H_SHIFT)) & SCT_CTRL_DOWN_H_MASK)

#define SCT_CTRL_STOP_H_MASK                     (0x20000U)
#define SCT_CTRL_STOP_H_SHIFT                    (17U)
/*! STOP_H - Stop Counter High
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_CTRL_STOP_H(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_STOP_H_SHIFT)) & SCT_CTRL_STOP_H_MASK)

#define SCT_CTRL_HALT_H_MASK                     (0x40000U)
#define SCT_CTRL_HALT_H_SHIFT                    (18U)
/*! HALT_H - Halt Counter High
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_CTRL_HALT_H(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_HALT_H_SHIFT)) & SCT_CTRL_HALT_H_MASK)

#define SCT_CTRL_CLRCTR_H_MASK                   (0x80000U)
#define SCT_CTRL_CLRCTR_H_SHIFT                  (19U)
/*! CLRCTR_H - Clear Counter High */
#define SCT_CTRL_CLRCTR_H(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_CLRCTR_H_SHIFT)) & SCT_CTRL_CLRCTR_H_MASK)

#define SCT_CTRL_BIDIR_H_MASK                    (0x100000U)
#define SCT_CTRL_BIDIR_H_SHIFT                   (20U)
/*! BIDIR_H - Bidirectional Select High
 *  0b0..Up. The H counter counts up to its limit condition, then is cleared to zero.
 *  0b1..Up-down. The H counter counts up to its limit, then counts down to a limit condition or to 0.
 */
#define SCT_CTRL_BIDIR_H(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_BIDIR_H_SHIFT)) & SCT_CTRL_BIDIR_H_MASK)

#define SCT_CTRL_PRE_H_MASK                      (0x1FE00000U)
#define SCT_CTRL_PRE_H_SHIFT                     (21U)
/*! PRE_H - Prescaler for High Counter */
#define SCT_CTRL_PRE_H(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_CTRL_PRE_H_SHIFT)) & SCT_CTRL_PRE_H_MASK)
/*! @} */

/*! @name LIMITL - SCT_LIMITL register */
/*! @{ */

#define SCT_LIMITL_LIMITL_MASK                   (0xFFFFU)
#define SCT_LIMITL_LIMITL_SHIFT                  (0U)
#define SCT_LIMITL_LIMITL(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_LIMITL_LIMITL_SHIFT)) & SCT_LIMITL_LIMITL_MASK)
/*! @} */

/*! @name LIMITH - SCT_LIMITH register */
/*! @{ */

#define SCT_LIMITH_LIMITH_MASK                   (0xFFFFU)
#define SCT_LIMITH_LIMITH_SHIFT                  (0U)
#define SCT_LIMITH_LIMITH(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_LIMITH_LIMITH_SHIFT)) & SCT_LIMITH_LIMITH_MASK)
/*! @} */

/*! @name LIMIT - SCT Limit Event Select */
/*! @{ */

#define SCT_LIMIT_LIMMSK_L_MASK                  (0xFFFFU)
#define SCT_LIMIT_LIMMSK_L_SHIFT                 (0U)
/*! LIMMSK_L - Limit Event Counter Low */
#define SCT_LIMIT_LIMMSK_L(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_LIMIT_LIMMSK_L_SHIFT)) & SCT_LIMIT_LIMMSK_L_MASK)

#define SCT_LIMIT_LIMMSK_H_MASK                  (0xFFFF0000U)
#define SCT_LIMIT_LIMMSK_H_SHIFT                 (16U)
/*! LIMMSK_H - Limit Event Counter High */
#define SCT_LIMIT_LIMMSK_H(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_LIMIT_LIMMSK_H_SHIFT)) & SCT_LIMIT_LIMMSK_H_MASK)
/*! @} */

/*! @name HALTL - SCT_HALTL register */
/*! @{ */

#define SCT_HALTL_HALTL_MASK                     (0xFFFFU)
#define SCT_HALTL_HALTL_SHIFT                    (0U)
#define SCT_HALTL_HALTL(x)                       (((uint16_t)(((uint16_t)(x)) << SCT_HALTL_HALTL_SHIFT)) & SCT_HALTL_HALTL_MASK)
/*! @} */

/*! @name HALTH - SCT_HALTH register */
/*! @{ */

#define SCT_HALTH_HALTH_MASK                     (0xFFFFU)
#define SCT_HALTH_HALTH_SHIFT                    (0U)
#define SCT_HALTH_HALTH(x)                       (((uint16_t)(((uint16_t)(x)) << SCT_HALTH_HALTH_SHIFT)) & SCT_HALTH_HALTH_MASK)
/*! @} */

/*! @name HALT - Halt Event Select */
/*! @{ */

#define SCT_HALT_HALTMSK_L_MASK                  (0xFFFFU)
#define SCT_HALT_HALTMSK_L_SHIFT                 (0U)
/*! HALTMSK_L - Halt Event Low */
#define SCT_HALT_HALTMSK_L(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_HALT_HALTMSK_L_SHIFT)) & SCT_HALT_HALTMSK_L_MASK)

#define SCT_HALT_HALTMSK_H_MASK                  (0xFFFF0000U)
#define SCT_HALT_HALTMSK_H_SHIFT                 (16U)
/*! HALTMSK_H - Halt Event High */
#define SCT_HALT_HALTMSK_H(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_HALT_HALTMSK_H_SHIFT)) & SCT_HALT_HALTMSK_H_MASK)
/*! @} */

/*! @name STOPL - SCT_STOPL register */
/*! @{ */

#define SCT_STOPL_STOPL_MASK                     (0xFFFFU)
#define SCT_STOPL_STOPL_SHIFT                    (0U)
#define SCT_STOPL_STOPL(x)                       (((uint16_t)(((uint16_t)(x)) << SCT_STOPL_STOPL_SHIFT)) & SCT_STOPL_STOPL_MASK)
/*! @} */

/*! @name STOPH - SCT_STOPH register */
/*! @{ */

#define SCT_STOPH_STOPH_MASK                     (0xFFFFU)
#define SCT_STOPH_STOPH_SHIFT                    (0U)
#define SCT_STOPH_STOPH(x)                       (((uint16_t)(((uint16_t)(x)) << SCT_STOPH_STOPH_SHIFT)) & SCT_STOPH_STOPH_MASK)
/*! @} */

/*! @name STOP - Stop Event Select */
/*! @{ */

#define SCT_STOP_STOPMSK_L_MASK                  (0xFFFFU)
#define SCT_STOP_STOPMSK_L_SHIFT                 (0U)
/*! STOPMSK_L - Stop Event Low */
#define SCT_STOP_STOPMSK_L(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_STOP_STOPMSK_L_SHIFT)) & SCT_STOP_STOPMSK_L_MASK)

#define SCT_STOP_STOPMSK_H_MASK                  (0xFFFF0000U)
#define SCT_STOP_STOPMSK_H_SHIFT                 (16U)
/*! STOPMSK_H - Stop Event High */
#define SCT_STOP_STOPMSK_H(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_STOP_STOPMSK_H_SHIFT)) & SCT_STOP_STOPMSK_H_MASK)
/*! @} */

/*! @name STARTL - SCT_STARTL register */
/*! @{ */

#define SCT_STARTL_STARTL_MASK                   (0xFFFFU)
#define SCT_STARTL_STARTL_SHIFT                  (0U)
#define SCT_STARTL_STARTL(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_STARTL_STARTL_SHIFT)) & SCT_STARTL_STARTL_MASK)
/*! @} */

/*! @name STARTH - SCT_STARTH register */
/*! @{ */

#define SCT_STARTH_STARTH_MASK                   (0xFFFFU)
#define SCT_STARTH_STARTH_SHIFT                  (0U)
#define SCT_STARTH_STARTH(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_STARTH_STARTH_SHIFT)) & SCT_STARTH_STARTH_MASK)
/*! @} */

/*! @name START - Start Event Select */
/*! @{ */

#define SCT_START_STARTMSK_L_MASK                (0xFFFFU)
#define SCT_START_STARTMSK_L_SHIFT               (0U)
/*! STARTMSK_L - If bit n is one, event n clears the CTRL[STOP_L] = 0 (event 0 = bit 0, event 1 =
 *    bit 1, etc.). The number of bits = number of events in this SCT.
 */
#define SCT_START_STARTMSK_L(x)                  (((uint32_t)(((uint32_t)(x)) << SCT_START_STARTMSK_L_SHIFT)) & SCT_START_STARTMSK_L_MASK)

#define SCT_START_STARTMSK_H_MASK                (0xFFFF0000U)
#define SCT_START_STARTMSK_H_SHIFT               (16U)
/*! STARTMSK_H - If bit n is one, event n clears the CTRL[STOP_H] = 0 (event 0 = bit 16, event 1 =
 *    bit 17, etc.). The number of bits = number of events in this SCT.
 */
#define SCT_START_STARTMSK_H(x)                  (((uint32_t)(((uint32_t)(x)) << SCT_START_STARTMSK_H_SHIFT)) & SCT_START_STARTMSK_H_MASK)
/*! @} */

/*! @name COUNTL - SCT_COUNTL register */
/*! @{ */

#define SCT_COUNTL_COUNTL_MASK                   (0xFFFFU)
#define SCT_COUNTL_COUNTL_SHIFT                  (0U)
#define SCT_COUNTL_COUNTL(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_COUNTL_COUNTL_SHIFT)) & SCT_COUNTL_COUNTL_MASK)
/*! @} */

/*! @name COUNTH - SCT_COUNTH register */
/*! @{ */

#define SCT_COUNTH_COUNTH_MASK                   (0xFFFFU)
#define SCT_COUNTH_COUNTH_SHIFT                  (0U)
#define SCT_COUNTH_COUNTH(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_COUNTH_COUNTH_SHIFT)) & SCT_COUNTH_COUNTH_MASK)
/*! @} */

/*! @name COUNT - Counter */
/*! @{ */

#define SCT_COUNT_CTR_L_MASK                     (0xFFFFU)
#define SCT_COUNT_CTR_L_SHIFT                    (0U)
/*! CTR_L - Counter Low */
#define SCT_COUNT_CTR_L(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_COUNT_CTR_L_SHIFT)) & SCT_COUNT_CTR_L_MASK)

#define SCT_COUNT_CTR_H_MASK                     (0xFFFF0000U)
#define SCT_COUNT_CTR_H_SHIFT                    (16U)
/*! CTR_H - Counter High */
#define SCT_COUNT_CTR_H(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_COUNT_CTR_H_SHIFT)) & SCT_COUNT_CTR_H_MASK)
/*! @} */

/*! @name STATEL - SCT_STATEL register */
/*! @{ */

#define SCT_STATEL_STATEL_MASK                   (0xFFFFU)
#define SCT_STATEL_STATEL_SHIFT                  (0U)
#define SCT_STATEL_STATEL(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_STATEL_STATEL_SHIFT)) & SCT_STATEL_STATEL_MASK)
/*! @} */

/*! @name STATEH - SCT_STATEH register */
/*! @{ */

#define SCT_STATEH_STATEH_MASK                   (0xFFFFU)
#define SCT_STATEH_STATEH_SHIFT                  (0U)
#define SCT_STATEH_STATEH(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_STATEH_STATEH_SHIFT)) & SCT_STATEH_STATEH_MASK)
/*! @} */

/*! @name STATE - State */
/*! @{ */

#define SCT_STATE_STATE_L_MASK                   (0x1FU)
#define SCT_STATE_STATE_L_SHIFT                  (0U)
/*! STATE_L - State variable */
#define SCT_STATE_STATE_L(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_STATE_STATE_L_SHIFT)) & SCT_STATE_STATE_L_MASK)

#define SCT_STATE_STATE_H_MASK                   (0x1F0000U)
#define SCT_STATE_STATE_H_SHIFT                  (16U)
/*! STATE_H - State variable */
#define SCT_STATE_STATE_H(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_STATE_STATE_H_SHIFT)) & SCT_STATE_STATE_H_MASK)
/*! @} */

/*! @name INPUT - Input */
/*! @{ */

#define SCT_INPUT_AIN0_MASK                      (0x1U)
#define SCT_INPUT_AIN0_SHIFT                     (0U)
/*! AIN0 - Input 0 state. Input 0 state on the last SCT clock edge. */
#define SCT_INPUT_AIN0(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN0_SHIFT)) & SCT_INPUT_AIN0_MASK)

#define SCT_INPUT_AIN1_MASK                      (0x2U)
#define SCT_INPUT_AIN1_SHIFT                     (1U)
/*! AIN1 - Input 1 state. Input 1 state on the last SCT clock edge. */
#define SCT_INPUT_AIN1(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN1_SHIFT)) & SCT_INPUT_AIN1_MASK)

#define SCT_INPUT_AIN2_MASK                      (0x4U)
#define SCT_INPUT_AIN2_SHIFT                     (2U)
/*! AIN2 - Input 2 state. Input 2 state on the last SCT clock edge. */
#define SCT_INPUT_AIN2(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN2_SHIFT)) & SCT_INPUT_AIN2_MASK)

#define SCT_INPUT_AIN3_MASK                      (0x8U)
#define SCT_INPUT_AIN3_SHIFT                     (3U)
/*! AIN3 - Input 3 state. Input 3 state on the last SCT clock edge. */
#define SCT_INPUT_AIN3(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN3_SHIFT)) & SCT_INPUT_AIN3_MASK)

#define SCT_INPUT_AIN4_MASK                      (0x10U)
#define SCT_INPUT_AIN4_SHIFT                     (4U)
/*! AIN4 - Input 4 state. Input 4 state on the last SCT clock edge. */
#define SCT_INPUT_AIN4(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN4_SHIFT)) & SCT_INPUT_AIN4_MASK)

#define SCT_INPUT_AIN5_MASK                      (0x20U)
#define SCT_INPUT_AIN5_SHIFT                     (5U)
/*! AIN5 - Input 5 state. Input 5 state on the last SCT clock edge. */
#define SCT_INPUT_AIN5(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN5_SHIFT)) & SCT_INPUT_AIN5_MASK)

#define SCT_INPUT_AIN6_MASK                      (0x40U)
#define SCT_INPUT_AIN6_SHIFT                     (6U)
/*! AIN6 - Input 6 state. Input 6 state on the last SCT clock edge. */
#define SCT_INPUT_AIN6(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN6_SHIFT)) & SCT_INPUT_AIN6_MASK)

#define SCT_INPUT_AIN7_MASK                      (0x80U)
#define SCT_INPUT_AIN7_SHIFT                     (7U)
/*! AIN7 - Input 7 state. Input 7 state on the last SCT clock edge. */
#define SCT_INPUT_AIN7(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN7_SHIFT)) & SCT_INPUT_AIN7_MASK)

#define SCT_INPUT_AIN8_MASK                      (0x100U)
#define SCT_INPUT_AIN8_SHIFT                     (8U)
/*! AIN8 - Input 8 state. Input 8 state on the last SCT clock edge. */
#define SCT_INPUT_AIN8(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN8_SHIFT)) & SCT_INPUT_AIN8_MASK)

#define SCT_INPUT_AIN9_MASK                      (0x200U)
#define SCT_INPUT_AIN9_SHIFT                     (9U)
/*! AIN9 - Input 9 state. Input 9 state on the last SCT clock edge. */
#define SCT_INPUT_AIN9(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN9_SHIFT)) & SCT_INPUT_AIN9_MASK)

#define SCT_INPUT_AIN10_MASK                     (0x400U)
#define SCT_INPUT_AIN10_SHIFT                    (10U)
/*! AIN10 - Input 10 state. Input 10 state on the last SCT clock edge. */
#define SCT_INPUT_AIN10(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN10_SHIFT)) & SCT_INPUT_AIN10_MASK)

#define SCT_INPUT_AIN11_MASK                     (0x800U)
#define SCT_INPUT_AIN11_SHIFT                    (11U)
/*! AIN11 - Input 11 state. Input 11 state on the last SCT clock edge. */
#define SCT_INPUT_AIN11(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN11_SHIFT)) & SCT_INPUT_AIN11_MASK)

#define SCT_INPUT_AIN12_MASK                     (0x1000U)
#define SCT_INPUT_AIN12_SHIFT                    (12U)
/*! AIN12 - Input 12 state. Input 12 state on the last SCT clock edge. */
#define SCT_INPUT_AIN12(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN12_SHIFT)) & SCT_INPUT_AIN12_MASK)

#define SCT_INPUT_AIN13_MASK                     (0x2000U)
#define SCT_INPUT_AIN13_SHIFT                    (13U)
/*! AIN13 - Input 13 state. Input 13 state on the last SCT clock edge. */
#define SCT_INPUT_AIN13(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN13_SHIFT)) & SCT_INPUT_AIN13_MASK)

#define SCT_INPUT_AIN14_MASK                     (0x4000U)
#define SCT_INPUT_AIN14_SHIFT                    (14U)
/*! AIN14 - Input 14 state. Input 14 state on the last SCT clock edge. */
#define SCT_INPUT_AIN14(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN14_SHIFT)) & SCT_INPUT_AIN14_MASK)

#define SCT_INPUT_AIN15_MASK                     (0x8000U)
#define SCT_INPUT_AIN15_SHIFT                    (15U)
/*! AIN15 - Input 15 state. Input 15 state on the last SCT clock edge. */
#define SCT_INPUT_AIN15(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_AIN15_SHIFT)) & SCT_INPUT_AIN15_MASK)

#define SCT_INPUT_SIN0_MASK                      (0x10000U)
#define SCT_INPUT_SIN0_SHIFT                     (16U)
/*! SIN0 - Input 0 state. Input 0 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN0(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN0_SHIFT)) & SCT_INPUT_SIN0_MASK)

#define SCT_INPUT_SIN1_MASK                      (0x20000U)
#define SCT_INPUT_SIN1_SHIFT                     (17U)
/*! SIN1 - Input 1 state. Input 1 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN1(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN1_SHIFT)) & SCT_INPUT_SIN1_MASK)

#define SCT_INPUT_SIN2_MASK                      (0x40000U)
#define SCT_INPUT_SIN2_SHIFT                     (18U)
/*! SIN2 - Input 2 state. Input 2 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN2(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN2_SHIFT)) & SCT_INPUT_SIN2_MASK)

#define SCT_INPUT_SIN3_MASK                      (0x80000U)
#define SCT_INPUT_SIN3_SHIFT                     (19U)
/*! SIN3 - Input 3 state. Input 3 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN3(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN3_SHIFT)) & SCT_INPUT_SIN3_MASK)

#define SCT_INPUT_SIN4_MASK                      (0x100000U)
#define SCT_INPUT_SIN4_SHIFT                     (20U)
/*! SIN4 - Input 4 state. Input 4 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN4(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN4_SHIFT)) & SCT_INPUT_SIN4_MASK)

#define SCT_INPUT_SIN5_MASK                      (0x200000U)
#define SCT_INPUT_SIN5_SHIFT                     (21U)
/*! SIN5 - Input 5 state. Input 5 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN5(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN5_SHIFT)) & SCT_INPUT_SIN5_MASK)

#define SCT_INPUT_SIN6_MASK                      (0x400000U)
#define SCT_INPUT_SIN6_SHIFT                     (22U)
/*! SIN6 - Input 6 state. Input 6 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN6(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN6_SHIFT)) & SCT_INPUT_SIN6_MASK)

#define SCT_INPUT_SIN7_MASK                      (0x800000U)
#define SCT_INPUT_SIN7_SHIFT                     (23U)
/*! SIN7 - Input 7 state. Input 7 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN7(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN7_SHIFT)) & SCT_INPUT_SIN7_MASK)

#define SCT_INPUT_SIN8_MASK                      (0x1000000U)
#define SCT_INPUT_SIN8_SHIFT                     (24U)
/*! SIN8 - Input 8 state. Input 8 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN8(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN8_SHIFT)) & SCT_INPUT_SIN8_MASK)

#define SCT_INPUT_SIN9_MASK                      (0x2000000U)
#define SCT_INPUT_SIN9_SHIFT                     (25U)
/*! SIN9 - Input 9 state. Input 9 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN9(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN9_SHIFT)) & SCT_INPUT_SIN9_MASK)

#define SCT_INPUT_SIN10_MASK                     (0x4000000U)
#define SCT_INPUT_SIN10_SHIFT                    (26U)
/*! SIN10 - Input 10 state. Input 10 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN10(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN10_SHIFT)) & SCT_INPUT_SIN10_MASK)

#define SCT_INPUT_SIN11_MASK                     (0x8000000U)
#define SCT_INPUT_SIN11_SHIFT                    (27U)
/*! SIN11 - Input 11 state. Input 11 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN11(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN11_SHIFT)) & SCT_INPUT_SIN11_MASK)

#define SCT_INPUT_SIN12_MASK                     (0x10000000U)
#define SCT_INPUT_SIN12_SHIFT                    (28U)
/*! SIN12 - Input 12 state. Input 12 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN12(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN12_SHIFT)) & SCT_INPUT_SIN12_MASK)

#define SCT_INPUT_SIN13_MASK                     (0x20000000U)
#define SCT_INPUT_SIN13_SHIFT                    (29U)
/*! SIN13 - Input 13 state. Input 13 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN13(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN13_SHIFT)) & SCT_INPUT_SIN13_MASK)

#define SCT_INPUT_SIN14_MASK                     (0x40000000U)
#define SCT_INPUT_SIN14_SHIFT                    (30U)
/*! SIN14 - Input 14 state. Input 14 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN14(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN14_SHIFT)) & SCT_INPUT_SIN14_MASK)

#define SCT_INPUT_SIN15_MASK                     (0x80000000U)
#define SCT_INPUT_SIN15_SHIFT                    (31U)
/*! SIN15 - Input 15 state. Input 15 state following the synchronization specified by INSYNC. */
#define SCT_INPUT_SIN15(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_INPUT_SIN15_SHIFT)) & SCT_INPUT_SIN15_MASK)
/*! @} */

/*! @name REGMODEL - SCT_REGMODEL register */
/*! @{ */

#define SCT_REGMODEL_REGMODEL_MASK               (0xFFFFU)
#define SCT_REGMODEL_REGMODEL_SHIFT              (0U)
/*! REGMODEL
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODEL_REGMODEL(x)                 (((uint16_t)(((uint16_t)(x)) << SCT_REGMODEL_REGMODEL_SHIFT)) & SCT_REGMODEL_REGMODEL_MASK)
/*! @} */

/*! @name REGMODEH - SCT_REGMODEH register */
/*! @{ */

#define SCT_REGMODEH_REGMODEH_MASK               (0xFFFFU)
#define SCT_REGMODEH_REGMODEH_SHIFT              (0U)
/*! REGMODEH
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODEH_REGMODEH(x)                 (((uint16_t)(((uint16_t)(x)) << SCT_REGMODEH_REGMODEH_SHIFT)) & SCT_REGMODEH_REGMODEH_MASK)
/*! @} */

/*! @name REGMODE - Match/Capture Mode */
/*! @{ */

#define SCT_REGMODE_REGMOD_L0_MASK               (0x1U)
#define SCT_REGMODE_REGMOD_L0_SHIFT              (0U)
/*! REGMOD_L0 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L0(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L0_SHIFT)) & SCT_REGMODE_REGMOD_L0_MASK)

#define SCT_REGMODE_REGMOD_L1_MASK               (0x2U)
#define SCT_REGMODE_REGMOD_L1_SHIFT              (1U)
/*! REGMOD_L1 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L1(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L1_SHIFT)) & SCT_REGMODE_REGMOD_L1_MASK)

#define SCT_REGMODE_REGMOD_L2_MASK               (0x4U)
#define SCT_REGMODE_REGMOD_L2_SHIFT              (2U)
/*! REGMOD_L2 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L2(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L2_SHIFT)) & SCT_REGMODE_REGMOD_L2_MASK)

#define SCT_REGMODE_REGMOD_L3_MASK               (0x8U)
#define SCT_REGMODE_REGMOD_L3_SHIFT              (3U)
/*! REGMOD_L3 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L3(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L3_SHIFT)) & SCT_REGMODE_REGMOD_L3_MASK)

#define SCT_REGMODE_REGMOD_L4_MASK               (0x10U)
#define SCT_REGMODE_REGMOD_L4_SHIFT              (4U)
/*! REGMOD_L4 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L4(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L4_SHIFT)) & SCT_REGMODE_REGMOD_L4_MASK)

#define SCT_REGMODE_REGMOD_L5_MASK               (0x20U)
#define SCT_REGMODE_REGMOD_L5_SHIFT              (5U)
/*! REGMOD_L5 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L5(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L5_SHIFT)) & SCT_REGMODE_REGMOD_L5_MASK)

#define SCT_REGMODE_REGMOD_L6_MASK               (0x40U)
#define SCT_REGMODE_REGMOD_L6_SHIFT              (6U)
/*! REGMOD_L6 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L6(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L6_SHIFT)) & SCT_REGMODE_REGMOD_L6_MASK)

#define SCT_REGMODE_REGMOD_L7_MASK               (0x80U)
#define SCT_REGMODE_REGMOD_L7_SHIFT              (7U)
/*! REGMOD_L7 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L7(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L7_SHIFT)) & SCT_REGMODE_REGMOD_L7_MASK)

#define SCT_REGMODE_REGMOD_L8_MASK               (0x100U)
#define SCT_REGMODE_REGMOD_L8_SHIFT              (8U)
/*! REGMOD_L8 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L8(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L8_SHIFT)) & SCT_REGMODE_REGMOD_L8_MASK)

#define SCT_REGMODE_REGMOD_L9_MASK               (0x200U)
#define SCT_REGMODE_REGMOD_L9_SHIFT              (9U)
/*! REGMOD_L9 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L9(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L9_SHIFT)) & SCT_REGMODE_REGMOD_L9_MASK)

#define SCT_REGMODE_REGMOD_L10_MASK              (0x400U)
#define SCT_REGMODE_REGMOD_L10_SHIFT             (10U)
/*! REGMOD_L10 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L10(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L10_SHIFT)) & SCT_REGMODE_REGMOD_L10_MASK)

#define SCT_REGMODE_REGMOD_L11_MASK              (0x800U)
#define SCT_REGMODE_REGMOD_L11_SHIFT             (11U)
/*! REGMOD_L11 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L11(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L11_SHIFT)) & SCT_REGMODE_REGMOD_L11_MASK)

#define SCT_REGMODE_REGMOD_L12_MASK              (0x1000U)
#define SCT_REGMODE_REGMOD_L12_SHIFT             (12U)
/*! REGMOD_L12 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L12(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L12_SHIFT)) & SCT_REGMODE_REGMOD_L12_MASK)

#define SCT_REGMODE_REGMOD_L13_MASK              (0x2000U)
#define SCT_REGMODE_REGMOD_L13_SHIFT             (13U)
/*! REGMOD_L13 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L13(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L13_SHIFT)) & SCT_REGMODE_REGMOD_L13_MASK)

#define SCT_REGMODE_REGMOD_L14_MASK              (0x4000U)
#define SCT_REGMODE_REGMOD_L14_SHIFT             (14U)
/*! REGMOD_L14 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L14(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L14_SHIFT)) & SCT_REGMODE_REGMOD_L14_MASK)

#define SCT_REGMODE_REGMOD_L15_MASK              (0x8000U)
#define SCT_REGMODE_REGMOD_L15_SHIFT             (15U)
/*! REGMOD_L15 - Register Mode Low n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_L15(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_L15_SHIFT)) & SCT_REGMODE_REGMOD_L15_MASK)

#define SCT_REGMODE_REGMOD_H0_MASK               (0x10000U)
#define SCT_REGMODE_REGMOD_H0_SHIFT              (16U)
/*! REGMOD_H0 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H0(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H0_SHIFT)) & SCT_REGMODE_REGMOD_H0_MASK)

#define SCT_REGMODE_REGMOD_H1_MASK               (0x20000U)
#define SCT_REGMODE_REGMOD_H1_SHIFT              (17U)
/*! REGMOD_H1 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H1(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H1_SHIFT)) & SCT_REGMODE_REGMOD_H1_MASK)

#define SCT_REGMODE_REGMOD_H2_MASK               (0x40000U)
#define SCT_REGMODE_REGMOD_H2_SHIFT              (18U)
/*! REGMOD_H2 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H2(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H2_SHIFT)) & SCT_REGMODE_REGMOD_H2_MASK)

#define SCT_REGMODE_REGMOD_H3_MASK               (0x80000U)
#define SCT_REGMODE_REGMOD_H3_SHIFT              (19U)
/*! REGMOD_H3 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H3(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H3_SHIFT)) & SCT_REGMODE_REGMOD_H3_MASK)

#define SCT_REGMODE_REGMOD_H4_MASK               (0x100000U)
#define SCT_REGMODE_REGMOD_H4_SHIFT              (20U)
/*! REGMOD_H4 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H4(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H4_SHIFT)) & SCT_REGMODE_REGMOD_H4_MASK)

#define SCT_REGMODE_REGMOD_H5_MASK               (0x200000U)
#define SCT_REGMODE_REGMOD_H5_SHIFT              (21U)
/*! REGMOD_H5 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H5(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H5_SHIFT)) & SCT_REGMODE_REGMOD_H5_MASK)

#define SCT_REGMODE_REGMOD_H6_MASK               (0x400000U)
#define SCT_REGMODE_REGMOD_H6_SHIFT              (22U)
/*! REGMOD_H6 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H6(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H6_SHIFT)) & SCT_REGMODE_REGMOD_H6_MASK)

#define SCT_REGMODE_REGMOD_H7_MASK               (0x800000U)
#define SCT_REGMODE_REGMOD_H7_SHIFT              (23U)
/*! REGMOD_H7 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H7(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H7_SHIFT)) & SCT_REGMODE_REGMOD_H7_MASK)

#define SCT_REGMODE_REGMOD_H8_MASK               (0x1000000U)
#define SCT_REGMODE_REGMOD_H8_SHIFT              (24U)
/*! REGMOD_H8 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H8(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H8_SHIFT)) & SCT_REGMODE_REGMOD_H8_MASK)

#define SCT_REGMODE_REGMOD_H9_MASK               (0x2000000U)
#define SCT_REGMODE_REGMOD_H9_SHIFT              (25U)
/*! REGMOD_H9 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H9(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H9_SHIFT)) & SCT_REGMODE_REGMOD_H9_MASK)

#define SCT_REGMODE_REGMOD_H10_MASK              (0x4000000U)
#define SCT_REGMODE_REGMOD_H10_SHIFT             (26U)
/*! REGMOD_H10 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H10(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H10_SHIFT)) & SCT_REGMODE_REGMOD_H10_MASK)

#define SCT_REGMODE_REGMOD_H11_MASK              (0x8000000U)
#define SCT_REGMODE_REGMOD_H11_SHIFT             (27U)
/*! REGMOD_H11 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H11(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H11_SHIFT)) & SCT_REGMODE_REGMOD_H11_MASK)

#define SCT_REGMODE_REGMOD_H12_MASK              (0x10000000U)
#define SCT_REGMODE_REGMOD_H12_SHIFT             (28U)
/*! REGMOD_H12 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H12(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H12_SHIFT)) & SCT_REGMODE_REGMOD_H12_MASK)

#define SCT_REGMODE_REGMOD_H13_MASK              (0x20000000U)
#define SCT_REGMODE_REGMOD_H13_SHIFT             (29U)
/*! REGMOD_H13 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H13(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H13_SHIFT)) & SCT_REGMODE_REGMOD_H13_MASK)

#define SCT_REGMODE_REGMOD_H14_MASK              (0x40000000U)
#define SCT_REGMODE_REGMOD_H14_SHIFT             (30U)
/*! REGMOD_H14 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H14(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H14_SHIFT)) & SCT_REGMODE_REGMOD_H14_MASK)

#define SCT_REGMODE_REGMOD_H15_MASK              (0x80000000U)
#define SCT_REGMODE_REGMOD_H15_SHIFT             (31U)
/*! REGMOD_H15 - Register Mode High n
 *  0b0..Match. Register n operates as a match register
 *  0b1..Capture. Register n operates as a capture register
 */
#define SCT_REGMODE_REGMOD_H15(x)                (((uint32_t)(((uint32_t)(x)) << SCT_REGMODE_REGMOD_H15_SHIFT)) & SCT_REGMODE_REGMOD_H15_MASK)
/*! @} */

/*! @name OUTPUT - Output */
/*! @{ */

#define SCT_OUTPUT_OUT0_MASK                     (0x1U)
#define SCT_OUTPUT_OUT0_SHIFT                    (0U)
/*! OUT0 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT0(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT0_SHIFT)) & SCT_OUTPUT_OUT0_MASK)

#define SCT_OUTPUT_OUT1_MASK                     (0x2U)
#define SCT_OUTPUT_OUT1_SHIFT                    (1U)
/*! OUT1 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT1(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT1_SHIFT)) & SCT_OUTPUT_OUT1_MASK)

#define SCT_OUTPUT_OUT2_MASK                     (0x4U)
#define SCT_OUTPUT_OUT2_SHIFT                    (2U)
/*! OUT2 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT2(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT2_SHIFT)) & SCT_OUTPUT_OUT2_MASK)

#define SCT_OUTPUT_OUT3_MASK                     (0x8U)
#define SCT_OUTPUT_OUT3_SHIFT                    (3U)
/*! OUT3 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT3(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT3_SHIFT)) & SCT_OUTPUT_OUT3_MASK)

#define SCT_OUTPUT_OUT4_MASK                     (0x10U)
#define SCT_OUTPUT_OUT4_SHIFT                    (4U)
/*! OUT4 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT4(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT4_SHIFT)) & SCT_OUTPUT_OUT4_MASK)

#define SCT_OUTPUT_OUT5_MASK                     (0x20U)
#define SCT_OUTPUT_OUT5_SHIFT                    (5U)
/*! OUT5 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT5(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT5_SHIFT)) & SCT_OUTPUT_OUT5_MASK)

#define SCT_OUTPUT_OUT6_MASK                     (0x40U)
#define SCT_OUTPUT_OUT6_SHIFT                    (6U)
/*! OUT6 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT6(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT6_SHIFT)) & SCT_OUTPUT_OUT6_MASK)

#define SCT_OUTPUT_OUT7_MASK                     (0x80U)
#define SCT_OUTPUT_OUT7_SHIFT                    (7U)
/*! OUT7 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT7(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT7_SHIFT)) & SCT_OUTPUT_OUT7_MASK)

#define SCT_OUTPUT_OUT8_MASK                     (0x100U)
#define SCT_OUTPUT_OUT8_SHIFT                    (8U)
/*! OUT8 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT8(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT8_SHIFT)) & SCT_OUTPUT_OUT8_MASK)

#define SCT_OUTPUT_OUT9_MASK                     (0x200U)
#define SCT_OUTPUT_OUT9_SHIFT                    (9U)
/*! OUT9 - Output n
 *  0b0..Writing a 0 forces the corresponding output low
 *  0b1..Writing a 1 forces the corresponding output high
 */
#define SCT_OUTPUT_OUT9(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUT_OUT9_SHIFT)) & SCT_OUTPUT_OUT9_MASK)
/*! @} */

/*! @name OUTPUTDIRCTRL - Output Counter Direction Control */
/*! @{ */

#define SCT_OUTPUTDIRCTRL_SETCLR0_MASK           (0x3U)
#define SCT_OUTPUTDIRCTRL_SETCLR0_SHIFT          (0U)
/*! SETCLR0 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR0(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR0_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR0_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR1_MASK           (0xCU)
#define SCT_OUTPUTDIRCTRL_SETCLR1_SHIFT          (2U)
/*! SETCLR1 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR1(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR1_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR1_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR2_MASK           (0x30U)
#define SCT_OUTPUTDIRCTRL_SETCLR2_SHIFT          (4U)
/*! SETCLR2 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR2(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR2_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR2_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR3_MASK           (0xC0U)
#define SCT_OUTPUTDIRCTRL_SETCLR3_SHIFT          (6U)
/*! SETCLR3 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR3(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR3_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR3_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR4_MASK           (0x300U)
#define SCT_OUTPUTDIRCTRL_SETCLR4_SHIFT          (8U)
/*! SETCLR4 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR4(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR4_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR4_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR5_MASK           (0xC00U)
#define SCT_OUTPUTDIRCTRL_SETCLR5_SHIFT          (10U)
/*! SETCLR5 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR5(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR5_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR5_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR6_MASK           (0x3000U)
#define SCT_OUTPUTDIRCTRL_SETCLR6_SHIFT          (12U)
/*! SETCLR6 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR6(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR6_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR6_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR7_MASK           (0xC000U)
#define SCT_OUTPUTDIRCTRL_SETCLR7_SHIFT          (14U)
/*! SETCLR7 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR7(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR7_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR7_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR8_MASK           (0x30000U)
#define SCT_OUTPUTDIRCTRL_SETCLR8_SHIFT          (16U)
/*! SETCLR8 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR8(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR8_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR8_MASK)

#define SCT_OUTPUTDIRCTRL_SETCLR9_MASK           (0xC0000U)
#define SCT_OUTPUTDIRCTRL_SETCLR9_SHIFT          (18U)
/*! SETCLR9 - Set/Clear Operation on Output n
 *  0b00..Set and clear do not depend on the direction of any counter.
 *  0b01..Set and clear are reversed when counter L or the unified counter is counting down.
 *  0b10..Set and clear are reversed when counter H is counting down. Do not use if UNIFY = 1.
 *  0b11..Reserved. Do not program this value.
 */
#define SCT_OUTPUTDIRCTRL_SETCLR9(x)             (((uint32_t)(((uint32_t)(x)) << SCT_OUTPUTDIRCTRL_SETCLR9_SHIFT)) & SCT_OUTPUTDIRCTRL_SETCLR9_MASK)
/*! @} */

/*! @name RES - Output Conflict Resolution */
/*! @{ */

#define SCT_RES_O0RES_MASK                       (0x3U)
#define SCT_RES_O0RES_SHIFT                      (0U)
/*! O0RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O0RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O0RES_SHIFT)) & SCT_RES_O0RES_MASK)

#define SCT_RES_O1RES_MASK                       (0xCU)
#define SCT_RES_O1RES_SHIFT                      (2U)
/*! O1RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O1RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O1RES_SHIFT)) & SCT_RES_O1RES_MASK)

#define SCT_RES_O2RES_MASK                       (0x30U)
#define SCT_RES_O2RES_SHIFT                      (4U)
/*! O2RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O2RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O2RES_SHIFT)) & SCT_RES_O2RES_MASK)

#define SCT_RES_O3RES_MASK                       (0xC0U)
#define SCT_RES_O3RES_SHIFT                      (6U)
/*! O3RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O3RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O3RES_SHIFT)) & SCT_RES_O3RES_MASK)

#define SCT_RES_O4RES_MASK                       (0x300U)
#define SCT_RES_O4RES_SHIFT                      (8U)
/*! O4RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O4RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O4RES_SHIFT)) & SCT_RES_O4RES_MASK)

#define SCT_RES_O5RES_MASK                       (0xC00U)
#define SCT_RES_O5RES_SHIFT                      (10U)
/*! O5RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O5RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O5RES_SHIFT)) & SCT_RES_O5RES_MASK)

#define SCT_RES_O6RES_MASK                       (0x3000U)
#define SCT_RES_O6RES_SHIFT                      (12U)
/*! O6RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O6RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O6RES_SHIFT)) & SCT_RES_O6RES_MASK)

#define SCT_RES_O7RES_MASK                       (0xC000U)
#define SCT_RES_O7RES_SHIFT                      (14U)
/*! O7RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O7RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O7RES_SHIFT)) & SCT_RES_O7RES_MASK)

#define SCT_RES_O8RES_MASK                       (0x30000U)
#define SCT_RES_O8RES_SHIFT                      (16U)
/*! O8RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O8RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O8RES_SHIFT)) & SCT_RES_O8RES_MASK)

#define SCT_RES_O9RES_MASK                       (0xC0000U)
#define SCT_RES_O9RES_SHIFT                      (18U)
/*! O9RES - Effect of simultaneous set and clear on output n
 *  0b00..No change
 *  0b01..Set output (or clear based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b10..Clear output (or set based on the OUTPUTDIRCTRL[SETCLRn] field)
 *  0b11..Toggle output
 */
#define SCT_RES_O9RES(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_RES_O9RES_SHIFT)) & SCT_RES_O9RES_MASK)
/*! @} */

/*! @name DMAREQ0 - DMA Request 0 */
/*! @{ */

#define SCT_DMAREQ0_DEV_0_MASK                   (0x1U)
#define SCT_DMAREQ0_DEV_0_SHIFT                  (0U)
/*! DEV_0 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_0(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_0_SHIFT)) & SCT_DMAREQ0_DEV_0_MASK)

#define SCT_DMAREQ0_DEV_1_MASK                   (0x2U)
#define SCT_DMAREQ0_DEV_1_SHIFT                  (1U)
/*! DEV_1 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_1(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_1_SHIFT)) & SCT_DMAREQ0_DEV_1_MASK)

#define SCT_DMAREQ0_DEV_2_MASK                   (0x4U)
#define SCT_DMAREQ0_DEV_2_SHIFT                  (2U)
/*! DEV_2 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_2(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_2_SHIFT)) & SCT_DMAREQ0_DEV_2_MASK)

#define SCT_DMAREQ0_DEV_3_MASK                   (0x8U)
#define SCT_DMAREQ0_DEV_3_SHIFT                  (3U)
/*! DEV_3 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_3(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_3_SHIFT)) & SCT_DMAREQ0_DEV_3_MASK)

#define SCT_DMAREQ0_DEV_4_MASK                   (0x10U)
#define SCT_DMAREQ0_DEV_4_SHIFT                  (4U)
/*! DEV_4 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_4(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_4_SHIFT)) & SCT_DMAREQ0_DEV_4_MASK)

#define SCT_DMAREQ0_DEV_5_MASK                   (0x20U)
#define SCT_DMAREQ0_DEV_5_SHIFT                  (5U)
/*! DEV_5 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_5(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_5_SHIFT)) & SCT_DMAREQ0_DEV_5_MASK)

#define SCT_DMAREQ0_DEV_6_MASK                   (0x40U)
#define SCT_DMAREQ0_DEV_6_SHIFT                  (6U)
/*! DEV_6 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_6(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_6_SHIFT)) & SCT_DMAREQ0_DEV_6_MASK)

#define SCT_DMAREQ0_DEV_7_MASK                   (0x80U)
#define SCT_DMAREQ0_DEV_7_SHIFT                  (7U)
/*! DEV_7 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_7(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_7_SHIFT)) & SCT_DMAREQ0_DEV_7_MASK)

#define SCT_DMAREQ0_DEV_8_MASK                   (0x100U)
#define SCT_DMAREQ0_DEV_8_SHIFT                  (8U)
/*! DEV_8 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_8(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_8_SHIFT)) & SCT_DMAREQ0_DEV_8_MASK)

#define SCT_DMAREQ0_DEV_9_MASK                   (0x200U)
#define SCT_DMAREQ0_DEV_9_SHIFT                  (9U)
/*! DEV_9 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_9(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_9_SHIFT)) & SCT_DMAREQ0_DEV_9_MASK)

#define SCT_DMAREQ0_DEV_10_MASK                  (0x400U)
#define SCT_DMAREQ0_DEV_10_SHIFT                 (10U)
/*! DEV_10 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_10(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_10_SHIFT)) & SCT_DMAREQ0_DEV_10_MASK)

#define SCT_DMAREQ0_DEV_11_MASK                  (0x800U)
#define SCT_DMAREQ0_DEV_11_SHIFT                 (11U)
/*! DEV_11 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_11(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_11_SHIFT)) & SCT_DMAREQ0_DEV_11_MASK)

#define SCT_DMAREQ0_DEV_12_MASK                  (0x1000U)
#define SCT_DMAREQ0_DEV_12_SHIFT                 (12U)
/*! DEV_12 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_12(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_12_SHIFT)) & SCT_DMAREQ0_DEV_12_MASK)

#define SCT_DMAREQ0_DEV_13_MASK                  (0x2000U)
#define SCT_DMAREQ0_DEV_13_SHIFT                 (13U)
/*! DEV_13 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_13(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_13_SHIFT)) & SCT_DMAREQ0_DEV_13_MASK)

#define SCT_DMAREQ0_DEV_14_MASK                  (0x4000U)
#define SCT_DMAREQ0_DEV_14_SHIFT                 (14U)
/*! DEV_14 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_14(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_14_SHIFT)) & SCT_DMAREQ0_DEV_14_MASK)

#define SCT_DMAREQ0_DEV_15_MASK                  (0x8000U)
#define SCT_DMAREQ0_DEV_15_SHIFT                 (15U)
/*! DEV_15 - DMA Request Event n */
#define SCT_DMAREQ0_DEV_15(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DEV_15_SHIFT)) & SCT_DMAREQ0_DEV_15_MASK)

#define SCT_DMAREQ0_DRL0_MASK                    (0x40000000U)
#define SCT_DMAREQ0_DRL0_SHIFT                   (30U)
/*! DRL0 - A 1 in this bit triggers DMA request 0 when it loads the MATCH_L/Unified registers from the RELOAD_L/Unified registers. */
#define SCT_DMAREQ0_DRL0(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DRL0_SHIFT)) & SCT_DMAREQ0_DRL0_MASK)

#define SCT_DMAREQ0_DRQ0_MASK                    (0x80000000U)
#define SCT_DMAREQ0_DRQ0_SHIFT                   (31U)
/*! DRQ0 - DMA Request 0 State */
#define SCT_DMAREQ0_DRQ0(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ0_DRQ0_SHIFT)) & SCT_DMAREQ0_DRQ0_MASK)
/*! @} */

/*! @name DMAREQ1 - DMA Request 1 */
/*! @{ */

#define SCT_DMAREQ1_DEV_0_MASK                   (0x1U)
#define SCT_DMAREQ1_DEV_0_SHIFT                  (0U)
/*! DEV_0 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_0(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_0_SHIFT)) & SCT_DMAREQ1_DEV_0_MASK)

#define SCT_DMAREQ1_DEV_1_MASK                   (0x2U)
#define SCT_DMAREQ1_DEV_1_SHIFT                  (1U)
/*! DEV_1 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_1(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_1_SHIFT)) & SCT_DMAREQ1_DEV_1_MASK)

#define SCT_DMAREQ1_DEV_2_MASK                   (0x4U)
#define SCT_DMAREQ1_DEV_2_SHIFT                  (2U)
/*! DEV_2 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_2(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_2_SHIFT)) & SCT_DMAREQ1_DEV_2_MASK)

#define SCT_DMAREQ1_DEV_3_MASK                   (0x8U)
#define SCT_DMAREQ1_DEV_3_SHIFT                  (3U)
/*! DEV_3 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_3(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_3_SHIFT)) & SCT_DMAREQ1_DEV_3_MASK)

#define SCT_DMAREQ1_DEV_4_MASK                   (0x10U)
#define SCT_DMAREQ1_DEV_4_SHIFT                  (4U)
/*! DEV_4 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_4(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_4_SHIFT)) & SCT_DMAREQ1_DEV_4_MASK)

#define SCT_DMAREQ1_DEV_5_MASK                   (0x20U)
#define SCT_DMAREQ1_DEV_5_SHIFT                  (5U)
/*! DEV_5 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_5(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_5_SHIFT)) & SCT_DMAREQ1_DEV_5_MASK)

#define SCT_DMAREQ1_DEV_6_MASK                   (0x40U)
#define SCT_DMAREQ1_DEV_6_SHIFT                  (6U)
/*! DEV_6 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_6(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_6_SHIFT)) & SCT_DMAREQ1_DEV_6_MASK)

#define SCT_DMAREQ1_DEV_7_MASK                   (0x80U)
#define SCT_DMAREQ1_DEV_7_SHIFT                  (7U)
/*! DEV_7 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_7(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_7_SHIFT)) & SCT_DMAREQ1_DEV_7_MASK)

#define SCT_DMAREQ1_DEV_8_MASK                   (0x100U)
#define SCT_DMAREQ1_DEV_8_SHIFT                  (8U)
/*! DEV_8 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_8(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_8_SHIFT)) & SCT_DMAREQ1_DEV_8_MASK)

#define SCT_DMAREQ1_DEV_9_MASK                   (0x200U)
#define SCT_DMAREQ1_DEV_9_SHIFT                  (9U)
/*! DEV_9 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_9(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_9_SHIFT)) & SCT_DMAREQ1_DEV_9_MASK)

#define SCT_DMAREQ1_DEV_10_MASK                  (0x400U)
#define SCT_DMAREQ1_DEV_10_SHIFT                 (10U)
/*! DEV_10 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_10(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_10_SHIFT)) & SCT_DMAREQ1_DEV_10_MASK)

#define SCT_DMAREQ1_DEV_11_MASK                  (0x800U)
#define SCT_DMAREQ1_DEV_11_SHIFT                 (11U)
/*! DEV_11 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_11(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_11_SHIFT)) & SCT_DMAREQ1_DEV_11_MASK)

#define SCT_DMAREQ1_DEV_12_MASK                  (0x1000U)
#define SCT_DMAREQ1_DEV_12_SHIFT                 (12U)
/*! DEV_12 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_12(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_12_SHIFT)) & SCT_DMAREQ1_DEV_12_MASK)

#define SCT_DMAREQ1_DEV_13_MASK                  (0x2000U)
#define SCT_DMAREQ1_DEV_13_SHIFT                 (13U)
/*! DEV_13 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_13(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_13_SHIFT)) & SCT_DMAREQ1_DEV_13_MASK)

#define SCT_DMAREQ1_DEV_14_MASK                  (0x4000U)
#define SCT_DMAREQ1_DEV_14_SHIFT                 (14U)
/*! DEV_14 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_14(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_14_SHIFT)) & SCT_DMAREQ1_DEV_14_MASK)

#define SCT_DMAREQ1_DEV_15_MASK                  (0x8000U)
#define SCT_DMAREQ1_DEV_15_SHIFT                 (15U)
/*! DEV_15 - DMA Request Event n */
#define SCT_DMAREQ1_DEV_15(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DEV_15_SHIFT)) & SCT_DMAREQ1_DEV_15_MASK)

#define SCT_DMAREQ1_DRL1_MASK                    (0x40000000U)
#define SCT_DMAREQ1_DRL1_SHIFT                   (30U)
/*! DRL1 - A 1 in this bit triggers DMA request 1 when it loads the Match L/Unified registers from the Reload L/Unified registers. */
#define SCT_DMAREQ1_DRL1(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DRL1_SHIFT)) & SCT_DMAREQ1_DRL1_MASK)

#define SCT_DMAREQ1_DRQ1_MASK                    (0x80000000U)
#define SCT_DMAREQ1_DRQ1_SHIFT                   (31U)
/*! DRQ1 - DMA Request 1 State */
#define SCT_DMAREQ1_DRQ1(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_DMAREQ1_DRQ1_SHIFT)) & SCT_DMAREQ1_DRQ1_MASK)
/*! @} */

/*! @name EVEN - Event Interrupt Enable */
/*! @{ */

#define SCT_EVEN_IEN0_MASK                       (0x1U)
#define SCT_EVEN_IEN0_SHIFT                      (0U)
/*! IEN0 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN0(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN0_SHIFT)) & SCT_EVEN_IEN0_MASK)

#define SCT_EVEN_IEN1_MASK                       (0x2U)
#define SCT_EVEN_IEN1_SHIFT                      (1U)
/*! IEN1 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN1(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN1_SHIFT)) & SCT_EVEN_IEN1_MASK)

#define SCT_EVEN_IEN2_MASK                       (0x4U)
#define SCT_EVEN_IEN2_SHIFT                      (2U)
/*! IEN2 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN2(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN2_SHIFT)) & SCT_EVEN_IEN2_MASK)

#define SCT_EVEN_IEN3_MASK                       (0x8U)
#define SCT_EVEN_IEN3_SHIFT                      (3U)
/*! IEN3 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN3(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN3_SHIFT)) & SCT_EVEN_IEN3_MASK)

#define SCT_EVEN_IEN4_MASK                       (0x10U)
#define SCT_EVEN_IEN4_SHIFT                      (4U)
/*! IEN4 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN4(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN4_SHIFT)) & SCT_EVEN_IEN4_MASK)

#define SCT_EVEN_IEN5_MASK                       (0x20U)
#define SCT_EVEN_IEN5_SHIFT                      (5U)
/*! IEN5 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN5(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN5_SHIFT)) & SCT_EVEN_IEN5_MASK)

#define SCT_EVEN_IEN6_MASK                       (0x40U)
#define SCT_EVEN_IEN6_SHIFT                      (6U)
/*! IEN6 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN6(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN6_SHIFT)) & SCT_EVEN_IEN6_MASK)

#define SCT_EVEN_IEN7_MASK                       (0x80U)
#define SCT_EVEN_IEN7_SHIFT                      (7U)
/*! IEN7 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN7(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN7_SHIFT)) & SCT_EVEN_IEN7_MASK)

#define SCT_EVEN_IEN8_MASK                       (0x100U)
#define SCT_EVEN_IEN8_SHIFT                      (8U)
/*! IEN8 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN8(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN8_SHIFT)) & SCT_EVEN_IEN8_MASK)

#define SCT_EVEN_IEN9_MASK                       (0x200U)
#define SCT_EVEN_IEN9_SHIFT                      (9U)
/*! IEN9 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN9(x)                         (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN9_SHIFT)) & SCT_EVEN_IEN9_MASK)

#define SCT_EVEN_IEN10_MASK                      (0x400U)
#define SCT_EVEN_IEN10_SHIFT                     (10U)
/*! IEN10 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN10(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN10_SHIFT)) & SCT_EVEN_IEN10_MASK)

#define SCT_EVEN_IEN11_MASK                      (0x800U)
#define SCT_EVEN_IEN11_SHIFT                     (11U)
/*! IEN11 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN11(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN11_SHIFT)) & SCT_EVEN_IEN11_MASK)

#define SCT_EVEN_IEN12_MASK                      (0x1000U)
#define SCT_EVEN_IEN12_SHIFT                     (12U)
/*! IEN12 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN12(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN12_SHIFT)) & SCT_EVEN_IEN12_MASK)

#define SCT_EVEN_IEN13_MASK                      (0x2000U)
#define SCT_EVEN_IEN13_SHIFT                     (13U)
/*! IEN13 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN13(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN13_SHIFT)) & SCT_EVEN_IEN13_MASK)

#define SCT_EVEN_IEN14_MASK                      (0x4000U)
#define SCT_EVEN_IEN14_SHIFT                     (14U)
/*! IEN14 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN14(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN14_SHIFT)) & SCT_EVEN_IEN14_MASK)

#define SCT_EVEN_IEN15_MASK                      (0x8000U)
#define SCT_EVEN_IEN15_SHIFT                     (15U)
/*! IEN15 - Event Interrupt Enable n
 *  0b0..Disable
 *  0b1..Enable
 */
#define SCT_EVEN_IEN15(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_EVEN_IEN15_SHIFT)) & SCT_EVEN_IEN15_MASK)
/*! @} */

/*! @name EVFLAG - Event Flag */
/*! @{ */

#define SCT_EVFLAG_FLAG0_MASK                    (0x1U)
#define SCT_EVFLAG_FLAG0_SHIFT                   (0U)
/*! FLAG0 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG0(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG0_SHIFT)) & SCT_EVFLAG_FLAG0_MASK)

#define SCT_EVFLAG_FLAG1_MASK                    (0x2U)
#define SCT_EVFLAG_FLAG1_SHIFT                   (1U)
/*! FLAG1 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG1(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG1_SHIFT)) & SCT_EVFLAG_FLAG1_MASK)

#define SCT_EVFLAG_FLAG2_MASK                    (0x4U)
#define SCT_EVFLAG_FLAG2_SHIFT                   (2U)
/*! FLAG2 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG2(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG2_SHIFT)) & SCT_EVFLAG_FLAG2_MASK)

#define SCT_EVFLAG_FLAG3_MASK                    (0x8U)
#define SCT_EVFLAG_FLAG3_SHIFT                   (3U)
/*! FLAG3 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG3(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG3_SHIFT)) & SCT_EVFLAG_FLAG3_MASK)

#define SCT_EVFLAG_FLAG4_MASK                    (0x10U)
#define SCT_EVFLAG_FLAG4_SHIFT                   (4U)
/*! FLAG4 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG4(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG4_SHIFT)) & SCT_EVFLAG_FLAG4_MASK)

#define SCT_EVFLAG_FLAG5_MASK                    (0x20U)
#define SCT_EVFLAG_FLAG5_SHIFT                   (5U)
/*! FLAG5 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG5(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG5_SHIFT)) & SCT_EVFLAG_FLAG5_MASK)

#define SCT_EVFLAG_FLAG6_MASK                    (0x40U)
#define SCT_EVFLAG_FLAG6_SHIFT                   (6U)
/*! FLAG6 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG6(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG6_SHIFT)) & SCT_EVFLAG_FLAG6_MASK)

#define SCT_EVFLAG_FLAG7_MASK                    (0x80U)
#define SCT_EVFLAG_FLAG7_SHIFT                   (7U)
/*! FLAG7 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG7(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG7_SHIFT)) & SCT_EVFLAG_FLAG7_MASK)

#define SCT_EVFLAG_FLAG8_MASK                    (0x100U)
#define SCT_EVFLAG_FLAG8_SHIFT                   (8U)
/*! FLAG8 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG8(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG8_SHIFT)) & SCT_EVFLAG_FLAG8_MASK)

#define SCT_EVFLAG_FLAG9_MASK                    (0x200U)
#define SCT_EVFLAG_FLAG9_SHIFT                   (9U)
/*! FLAG9 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG9(x)                      (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG9_SHIFT)) & SCT_EVFLAG_FLAG9_MASK)

#define SCT_EVFLAG_FLAG10_MASK                   (0x400U)
#define SCT_EVFLAG_FLAG10_SHIFT                  (10U)
/*! FLAG10 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG10(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG10_SHIFT)) & SCT_EVFLAG_FLAG10_MASK)

#define SCT_EVFLAG_FLAG11_MASK                   (0x800U)
#define SCT_EVFLAG_FLAG11_SHIFT                  (11U)
/*! FLAG11 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG11(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG11_SHIFT)) & SCT_EVFLAG_FLAG11_MASK)

#define SCT_EVFLAG_FLAG12_MASK                   (0x1000U)
#define SCT_EVFLAG_FLAG12_SHIFT                  (12U)
/*! FLAG12 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG12(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG12_SHIFT)) & SCT_EVFLAG_FLAG12_MASK)

#define SCT_EVFLAG_FLAG13_MASK                   (0x2000U)
#define SCT_EVFLAG_FLAG13_SHIFT                  (13U)
/*! FLAG13 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG13(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG13_SHIFT)) & SCT_EVFLAG_FLAG13_MASK)

#define SCT_EVFLAG_FLAG14_MASK                   (0x4000U)
#define SCT_EVFLAG_FLAG14_SHIFT                  (14U)
/*! FLAG14 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG14(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG14_SHIFT)) & SCT_EVFLAG_FLAG14_MASK)

#define SCT_EVFLAG_FLAG15_MASK                   (0x8000U)
#define SCT_EVFLAG_FLAG15_SHIFT                  (15U)
/*! FLAG15 - Event Flag n
 *  0b0..No Flag
 *  0b1..Event n Flag
 */
#define SCT_EVFLAG_FLAG15(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_EVFLAG_FLAG15_SHIFT)) & SCT_EVFLAG_FLAG15_MASK)
/*! @} */

/*! @name CONEN - Conflict Interrupt Enable */
/*! @{ */

#define SCT_CONEN_NCEN0_MASK                     (0x1U)
#define SCT_CONEN_NCEN0_SHIFT                    (0U)
/*! NCEN0 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN0(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN0_SHIFT)) & SCT_CONEN_NCEN0_MASK)

#define SCT_CONEN_NCEN1_MASK                     (0x2U)
#define SCT_CONEN_NCEN1_SHIFT                    (1U)
/*! NCEN1 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN1(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN1_SHIFT)) & SCT_CONEN_NCEN1_MASK)

#define SCT_CONEN_NCEN2_MASK                     (0x4U)
#define SCT_CONEN_NCEN2_SHIFT                    (2U)
/*! NCEN2 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN2(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN2_SHIFT)) & SCT_CONEN_NCEN2_MASK)

#define SCT_CONEN_NCEN3_MASK                     (0x8U)
#define SCT_CONEN_NCEN3_SHIFT                    (3U)
/*! NCEN3 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN3(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN3_SHIFT)) & SCT_CONEN_NCEN3_MASK)

#define SCT_CONEN_NCEN4_MASK                     (0x10U)
#define SCT_CONEN_NCEN4_SHIFT                    (4U)
/*! NCEN4 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN4(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN4_SHIFT)) & SCT_CONEN_NCEN4_MASK)

#define SCT_CONEN_NCEN5_MASK                     (0x20U)
#define SCT_CONEN_NCEN5_SHIFT                    (5U)
/*! NCEN5 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN5(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN5_SHIFT)) & SCT_CONEN_NCEN5_MASK)

#define SCT_CONEN_NCEN6_MASK                     (0x40U)
#define SCT_CONEN_NCEN6_SHIFT                    (6U)
/*! NCEN6 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN6(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN6_SHIFT)) & SCT_CONEN_NCEN6_MASK)

#define SCT_CONEN_NCEN7_MASK                     (0x80U)
#define SCT_CONEN_NCEN7_SHIFT                    (7U)
/*! NCEN7 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN7(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN7_SHIFT)) & SCT_CONEN_NCEN7_MASK)

#define SCT_CONEN_NCEN8_MASK                     (0x100U)
#define SCT_CONEN_NCEN8_SHIFT                    (8U)
/*! NCEN8 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN8(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN8_SHIFT)) & SCT_CONEN_NCEN8_MASK)

#define SCT_CONEN_NCEN9_MASK                     (0x200U)
#define SCT_CONEN_NCEN9_SHIFT                    (9U)
/*! NCEN9 - No Change Conflict Event/Interrupt Enable
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define SCT_CONEN_NCEN9(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_CONEN_NCEN9_SHIFT)) & SCT_CONEN_NCEN9_MASK)
/*! @} */

/*! @name CONFLAG - Conflict Flag */
/*! @{ */

#define SCT_CONFLAG_NCFLAG0_MASK                 (0x1U)
#define SCT_CONFLAG_NCFLAG0_SHIFT                (0U)
/*! NCFLAG0 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG0(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG0_SHIFT)) & SCT_CONFLAG_NCFLAG0_MASK)

#define SCT_CONFLAG_NCFLAG1_MASK                 (0x2U)
#define SCT_CONFLAG_NCFLAG1_SHIFT                (1U)
/*! NCFLAG1 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG1(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG1_SHIFT)) & SCT_CONFLAG_NCFLAG1_MASK)

#define SCT_CONFLAG_NCFLAG2_MASK                 (0x4U)
#define SCT_CONFLAG_NCFLAG2_SHIFT                (2U)
/*! NCFLAG2 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG2(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG2_SHIFT)) & SCT_CONFLAG_NCFLAG2_MASK)

#define SCT_CONFLAG_NCFLAG3_MASK                 (0x8U)
#define SCT_CONFLAG_NCFLAG3_SHIFT                (3U)
/*! NCFLAG3 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG3(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG3_SHIFT)) & SCT_CONFLAG_NCFLAG3_MASK)

#define SCT_CONFLAG_NCFLAG4_MASK                 (0x10U)
#define SCT_CONFLAG_NCFLAG4_SHIFT                (4U)
/*! NCFLAG4 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG4(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG4_SHIFT)) & SCT_CONFLAG_NCFLAG4_MASK)

#define SCT_CONFLAG_NCFLAG5_MASK                 (0x20U)
#define SCT_CONFLAG_NCFLAG5_SHIFT                (5U)
/*! NCFLAG5 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG5(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG5_SHIFT)) & SCT_CONFLAG_NCFLAG5_MASK)

#define SCT_CONFLAG_NCFLAG6_MASK                 (0x40U)
#define SCT_CONFLAG_NCFLAG6_SHIFT                (6U)
/*! NCFLAG6 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG6(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG6_SHIFT)) & SCT_CONFLAG_NCFLAG6_MASK)

#define SCT_CONFLAG_NCFLAG7_MASK                 (0x80U)
#define SCT_CONFLAG_NCFLAG7_SHIFT                (7U)
/*! NCFLAG7 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG7(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG7_SHIFT)) & SCT_CONFLAG_NCFLAG7_MASK)

#define SCT_CONFLAG_NCFLAG8_MASK                 (0x100U)
#define SCT_CONFLAG_NCFLAG8_SHIFT                (8U)
/*! NCFLAG8 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG8(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG8_SHIFT)) & SCT_CONFLAG_NCFLAG8_MASK)

#define SCT_CONFLAG_NCFLAG9_MASK                 (0x200U)
#define SCT_CONFLAG_NCFLAG9_SHIFT                (9U)
/*! NCFLAG9 - No Change Conflict Event Flag
 *  0b0..No Conflict Event
 *  0b1..A No Change Conflict Event occured
 */
#define SCT_CONFLAG_NCFLAG9(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_NCFLAG9_SHIFT)) & SCT_CONFLAG_NCFLAG9_MASK)

#define SCT_CONFLAG_BUSERRL_MASK                 (0x40000000U)
#define SCT_CONFLAG_BUSERRL_SHIFT                (30U)
/*! BUSERRL - Bus Error Low/Unified */
#define SCT_CONFLAG_BUSERRL(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_BUSERRL_SHIFT)) & SCT_CONFLAG_BUSERRL_MASK)

#define SCT_CONFLAG_BUSERRH_MASK                 (0x80000000U)
#define SCT_CONFLAG_BUSERRH_SHIFT                (31U)
/*! BUSERRH - Bus Error High */
#define SCT_CONFLAG_BUSERRH(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_CONFLAG_BUSERRH_SHIFT)) & SCT_CONFLAG_BUSERRH_MASK)
/*! @} */

/*! @name CAPL - SCT_CAPL register */
/*! @{ */

#define SCT_CAPL_CAPL_MASK                       (0xFFFFU)
#define SCT_CAPL_CAPL_SHIFT                      (0U)
#define SCT_CAPL_CAPL(x)                         (((uint16_t)(((uint16_t)(x)) << SCT_CAPL_CAPL_SHIFT)) & SCT_CAPL_CAPL_MASK)
/*! @} */

/* The count of SCT_CAPL */
#define SCT_CAPL_COUNT                           (16U)

/*! @name CAPH - SCT_CAPH register */
/*! @{ */

#define SCT_CAPH_CAPH_MASK                       (0xFFFFU)
#define SCT_CAPH_CAPH_SHIFT                      (0U)
#define SCT_CAPH_CAPH(x)                         (((uint16_t)(((uint16_t)(x)) << SCT_CAPH_CAPH_SHIFT)) & SCT_CAPH_CAPH_MASK)
/*! @} */

/* The count of SCT_CAPH */
#define SCT_CAPH_COUNT                           (16U)

/*! @name CAP - Capture Value */
/*! @{ */

#define SCT_CAP_CAPN_L_MASK                      (0xFFFFU)
#define SCT_CAP_CAPN_L_SHIFT                     (0U)
/*! CAPN_L - Capture n Low */
#define SCT_CAP_CAPN_L(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_CAP_CAPN_L_SHIFT)) & SCT_CAP_CAPN_L_MASK)

#define SCT_CAP_CAPN_H_MASK                      (0xFFFF0000U)
#define SCT_CAP_CAPN_H_SHIFT                     (16U)
/*! CAPN_H - Capture n High */
#define SCT_CAP_CAPN_H(x)                        (((uint32_t)(((uint32_t)(x)) << SCT_CAP_CAPN_H_SHIFT)) & SCT_CAP_CAPN_H_MASK)
/*! @} */

/* The count of SCT_CAP */
#define SCT_CAP_COUNT                            (16U)

/*! @name MATCHL - SCT_MATCHL register */
/*! @{ */

#define SCT_MATCHL_MATCHL_MASK                   (0xFFFFU)
#define SCT_MATCHL_MATCHL_SHIFT                  (0U)
#define SCT_MATCHL_MATCHL(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_MATCHL_MATCHL_SHIFT)) & SCT_MATCHL_MATCHL_MASK)
/*! @} */

/* The count of SCT_MATCHL */
#define SCT_MATCHL_COUNT                         (16U)

/*! @name MATCHH - SCT_MATCHH register */
/*! @{ */

#define SCT_MATCHH_MATCHH_MASK                   (0xFFFFU)
#define SCT_MATCHH_MATCHH_SHIFT                  (0U)
#define SCT_MATCHH_MATCHH(x)                     (((uint16_t)(((uint16_t)(x)) << SCT_MATCHH_MATCHH_SHIFT)) & SCT_MATCHH_MATCHH_MASK)
/*! @} */

/* The count of SCT_MATCHH */
#define SCT_MATCHH_COUNT                         (16U)

/*! @name MATCH - Match Value */
/*! @{ */

#define SCT_MATCH_MATCHN_L_MASK                  (0xFFFFU)
#define SCT_MATCH_MATCHN_L_SHIFT                 (0U)
/*! MATCHN_L - Match n Low */
#define SCT_MATCH_MATCHN_L(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_MATCH_MATCHN_L_SHIFT)) & SCT_MATCH_MATCHN_L_MASK)

#define SCT_MATCH_MATCHN_H_MASK                  (0xFFFF0000U)
#define SCT_MATCH_MATCHN_H_SHIFT                 (16U)
/*! MATCHN_H - Match n High */
#define SCT_MATCH_MATCHN_H(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_MATCH_MATCHN_H_SHIFT)) & SCT_MATCH_MATCHN_H_MASK)
/*! @} */

/* The count of SCT_MATCH */
#define SCT_MATCH_COUNT                          (16U)

/*! @name CAPCTRLL - SCT_CAPCTRLL register */
/*! @{ */

#define SCT_CAPCTRLL_CAPCTRLL_MASK               (0xFFFFU)
#define SCT_CAPCTRLL_CAPCTRLL_SHIFT              (0U)
#define SCT_CAPCTRLL_CAPCTRLL(x)                 (((uint16_t)(((uint16_t)(x)) << SCT_CAPCTRLL_CAPCTRLL_SHIFT)) & SCT_CAPCTRLL_CAPCTRLL_MASK)
/*! @} */

/* The count of SCT_CAPCTRLL */
#define SCT_CAPCTRLL_COUNT                       (16U)

/*! @name CAPCTRLH - SCT_CAPCTRLH register */
/*! @{ */

#define SCT_CAPCTRLH_CAPCTRLH_MASK               (0xFFFFU)
#define SCT_CAPCTRLH_CAPCTRLH_SHIFT              (0U)
#define SCT_CAPCTRLH_CAPCTRLH(x)                 (((uint16_t)(((uint16_t)(x)) << SCT_CAPCTRLH_CAPCTRLH_SHIFT)) & SCT_CAPCTRLH_CAPCTRLH_MASK)
/*! @} */

/* The count of SCT_CAPCTRLH */
#define SCT_CAPCTRLH_COUNT                       (16U)

/*! @name SCTCAPCTRL_CAPCTRL - Capture Control */
/*! @{ */

#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_L_MASK    (0xFFFFU)
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_L_SHIFT   (0U)
/*! CAPCONN_L - Capture Control n Low */
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_L(x)      (((uint32_t)(((uint32_t)(x)) << SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_L_SHIFT)) & SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_L_MASK)

#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_H_MASK    (0xFFFF0000U)
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_H_SHIFT   (16U)
/*! CAPCONN_H - Capture Control n High */
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_H(x)      (((uint32_t)(((uint32_t)(x)) << SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_H_SHIFT)) & SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_H_MASK)
/*! @} */

/* The count of SCT_SCTCAPCTRL_CAPCTRL */
#define SCT_SCTCAPCTRL_CAPCTRL_COUNT             (16U)

/*! @name MATCHRELL - SCT_MATCHRELL register */
/*! @{ */

#define SCT_MATCHRELL_MATCHRELL_MASK             (0xFFFFU)
#define SCT_MATCHRELL_MATCHRELL_SHIFT            (0U)
#define SCT_MATCHRELL_MATCHRELL(x)               (((uint16_t)(((uint16_t)(x)) << SCT_MATCHRELL_MATCHRELL_SHIFT)) & SCT_MATCHRELL_MATCHRELL_MASK)
/*! @} */

/* The count of SCT_MATCHRELL */
#define SCT_MATCHRELL_COUNT                      (16U)

/*! @name MATCHRELH - SCT_MATCHRELH register */
/*! @{ */

#define SCT_MATCHRELH_MATCHRELH_MASK             (0xFFFFU)
#define SCT_MATCHRELH_MATCHRELH_SHIFT            (0U)
#define SCT_MATCHRELH_MATCHRELH(x)               (((uint16_t)(((uint16_t)(x)) << SCT_MATCHRELH_MATCHRELH_SHIFT)) & SCT_MATCHRELH_MATCHRELH_MASK)
/*! @} */

/* The count of SCT_MATCHRELH */
#define SCT_MATCHRELH_COUNT                      (16U)

/*! @name MATCHREL - Match Reload Value */
/*! @{ */

#define SCT_MATCHREL_RELOADN_L_MASK              (0xFFFFU)
#define SCT_MATCHREL_RELOADN_L_SHIFT             (0U)
/*! RELOADN_L - Reload n Low */
#define SCT_MATCHREL_RELOADN_L(x)                (((uint32_t)(((uint32_t)(x)) << SCT_MATCHREL_RELOADN_L_SHIFT)) & SCT_MATCHREL_RELOADN_L_MASK)

#define SCT_MATCHREL_RELOADN_H_MASK              (0xFFFF0000U)
#define SCT_MATCHREL_RELOADN_H_SHIFT             (16U)
/*! RELOADN_H - Reload n High */
#define SCT_MATCHREL_RELOADN_H(x)                (((uint32_t)(((uint32_t)(x)) << SCT_MATCHREL_RELOADN_H_SHIFT)) & SCT_MATCHREL_RELOADN_H_MASK)
/*! @} */

/* The count of SCT_MATCHREL */
#define SCT_MATCHREL_COUNT                       (16U)

/*! @name EV_STATE - Event n State */
/*! @{ */

#define SCT_EV_STATE_STATEMSKN_MASK              (0xFFFFFFFFU)
#define SCT_EV_STATE_STATEMSKN_SHIFT             (0U)
/*! STATEMSKN - Event State Mask n */
#define SCT_EV_STATE_STATEMSKN(x)                (((uint32_t)(((uint32_t)(x)) << SCT_EV_STATE_STATEMSKN_SHIFT)) & SCT_EV_STATE_STATEMSKN_MASK)
/*! @} */

/* The count of SCT_EV_STATE */
#define SCT_EV_STATE_COUNT                       (16U)

/*! @name EV_CTRL - Event n Control */
/*! @{ */

#define SCT_EV_CTRL_MATCHSEL_MASK                (0xFU)
#define SCT_EV_CTRL_MATCHSEL_SHIFT               (0U)
/*! MATCHSEL - Match Select */
#define SCT_EV_CTRL_MATCHSEL(x)                  (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_MATCHSEL_SHIFT)) & SCT_EV_CTRL_MATCHSEL_MASK)

#define SCT_EV_CTRL_HEVENT_MASK                  (0x10U)
#define SCT_EV_CTRL_HEVENT_SHIFT                 (4U)
/*! HEVENT - High Event
 *  0b0..Low Counter
 *  0b1..High Counter
 */
#define SCT_EV_CTRL_HEVENT(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_HEVENT_SHIFT)) & SCT_EV_CTRL_HEVENT_MASK)

#define SCT_EV_CTRL_OUTSEL_MASK                  (0x20U)
#define SCT_EV_CTRL_OUTSEL_SHIFT                 (5U)
/*! OUTSEL - Input/Output Select
 *  0b0..Selects the inputs selected by IOSEL.
 *  0b1..Selects the outputs selected by IOSEL.
 */
#define SCT_EV_CTRL_OUTSEL(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_OUTSEL_SHIFT)) & SCT_EV_CTRL_OUTSEL_MASK)

#define SCT_EV_CTRL_IOSEL_MASK                   (0x3C0U)
#define SCT_EV_CTRL_IOSEL_SHIFT                  (6U)
/*! IOSEL - Input/Output Signal Select */
#define SCT_EV_CTRL_IOSEL(x)                     (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_IOSEL_SHIFT)) & SCT_EV_CTRL_IOSEL_MASK)

#define SCT_EV_CTRL_IOCOND_MASK                  (0xC00U)
#define SCT_EV_CTRL_IOCOND_SHIFT                 (10U)
/*! IOCOND - Input/Output Condition
 *  0b00..Low
 *  0b01..Rise
 *  0b10..Fall
 *  0b11..High
 */
#define SCT_EV_CTRL_IOCOND(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_IOCOND_SHIFT)) & SCT_EV_CTRL_IOCOND_MASK)

#define SCT_EV_CTRL_COMBMODE_MASK                (0x3000U)
#define SCT_EV_CTRL_COMBMODE_SHIFT               (12U)
/*! COMBMODE - Combination Mode
 *  0b00..OR. The event occurs when either the specified match or I/O condition occurs.
 *  0b01..MATCH. Uses the specified match only.
 *  0b10..IO. Uses the specified I/O condition only.
 *  0b11..AND. The event occurs when the specified match and I/O condition occur simultaneously.
 */
#define SCT_EV_CTRL_COMBMODE(x)                  (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_COMBMODE_SHIFT)) & SCT_EV_CTRL_COMBMODE_MASK)

#define SCT_EV_CTRL_STATELD_MASK                 (0x4000U)
#define SCT_EV_CTRL_STATELD_SHIFT                (14U)
/*! STATELD - State Load
 *  0b0..Add. STATEV value is added into STATE (the carry-out is ignored).
 *  0b1..Load. STATEV value is loaded into STATE.
 */
#define SCT_EV_CTRL_STATELD(x)                   (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_STATELD_SHIFT)) & SCT_EV_CTRL_STATELD_MASK)

#define SCT_EV_CTRL_STATEV_MASK                  (0xF8000U)
#define SCT_EV_CTRL_STATEV_SHIFT                 (15U)
/*! STATEV - State Value */
#define SCT_EV_CTRL_STATEV(x)                    (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_STATEV_SHIFT)) & SCT_EV_CTRL_STATEV_MASK)

#define SCT_EV_CTRL_MATCHMEM_MASK                (0x100000U)
#define SCT_EV_CTRL_MATCHMEM_SHIFT               (20U)
/*! MATCHMEM - Match Mem */
#define SCT_EV_CTRL_MATCHMEM(x)                  (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_MATCHMEM_SHIFT)) & SCT_EV_CTRL_MATCHMEM_MASK)

#define SCT_EV_CTRL_DIRECTION_MASK               (0x600000U)
#define SCT_EV_CTRL_DIRECTION_SHIFT              (21U)
/*! DIRECTION - Direction
 *  0b00..Direction independent. This event is triggered regardless of the count direction.
 *  0b01..Counting up. This event is triggered only during up-counting when BIDIR = 1.
 *  0b10..Counting down. This event is triggered only during down-counting when BIDIR = 1.
 *  0b11..Reserved
 */
#define SCT_EV_CTRL_DIRECTION(x)                 (((uint32_t)(((uint32_t)(x)) << SCT_EV_CTRL_DIRECTION_SHIFT)) & SCT_EV_CTRL_DIRECTION_MASK)
/*! @} */

/* The count of SCT_EV_CTRL */
#define SCT_EV_CTRL_COUNT                        (16U)

/*! @name OUT_SET - Output n Set */
/*! @{ */

#define SCT_OUT_SET_SET_MASK                     (0xFFFFU)
#define SCT_OUT_SET_SET_SHIFT                    (0U)
/*! SET - Set */
#define SCT_OUT_SET_SET(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUT_SET_SET_SHIFT)) & SCT_OUT_SET_SET_MASK)
/*! @} */

/* The count of SCT_OUT_SET */
#define SCT_OUT_SET_COUNT                        (10U)

/*! @name OUT_CLR - Output n Clear */
/*! @{ */

#define SCT_OUT_CLR_CLR_MASK                     (0xFFFFU)
#define SCT_OUT_CLR_CLR_SHIFT                    (0U)
/*! CLR - Clear */
#define SCT_OUT_CLR_CLR(x)                       (((uint32_t)(((uint32_t)(x)) << SCT_OUT_CLR_CLR_SHIFT)) & SCT_OUT_CLR_CLR_MASK)
/*! @} */

/* The count of SCT_OUT_CLR */
#define SCT_OUT_CLR_COUNT                        (10U)


/*!
 * @}
 */ /* end of group SCT_Register_Masks */

/* Backward compatibility */
#define SCT_CAP_CAPn_L_MASK                      SCT_CAP_CAPN_L_MASK
#define SCT_CAP_CAPn_L_SHIFT                     SCT_CAP_CAPN_L_SHIFT
#define SCT_CAP_CAPn_L(x)                        SCT_CAP_CAPN_L(x)
#define SCT_CAP_CAPn_H_MASK                      SCT_CAP_CAPN_H_MASK
#define SCT_CAP_CAPn_H_SHIFT                     SCT_CAP_CAPN_H_SHIFT
#define SCT_CAP_CAPn_H(x)                        SCT_CAP_CAPN_H(x)
#define SCT_MATCH_MATCHn_L_MASK                  SCT_MATCH_MATCHN_L_MASK
#define SCT_MATCH_MATCHn_L_SHIFT                 SCT_MATCH_MATCHN_L_SHIFT
#define SCT_MATCH_MATCHn_L(x)                    SCT_MATCH_MATCHN_L(x)
#define SCT_MATCH_MATCHn_H_MASK                  SCT_MATCH_MATCHN_H_MASK
#define SCT_MATCH_MATCHn_H_SHIFT                 SCT_MATCH_MATCHN_H_SHIFT
#define SCT_MATCH_MATCHn_H(x)                    SCT_MATCH_MATCHN_H(x)
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONn_L_MASK    SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_L_MASK
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONn_L_SHIFT   SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_L_SHIFT
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONn_L(x)      SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_L(x)
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONn_H_MASK    SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_H_MASK
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONn_H_SHIFT   SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_H_SHIFT
#define SCT_SCTCAPCTRL_CAPCTRL_CAPCONn_H(x)      SCT_SCTCAPCTRL_CAPCTRL_CAPCONN_H(x)
#define SCT_MATCHREL_RELOADn_L_MASK              SCT_MATCHREL_RELOADN_L_MASK
#define SCT_MATCHREL_RELOADn_L_SHIFT             SCT_MATCHREL_RELOADN_L_SHIFT
#define SCT_MATCHREL_RELOADn_L(x)                SCT_MATCHREL_RELOADN_L(x)
#define SCT_MATCHREL_RELOADn_H_MASK              SCT_MATCHREL_RELOADN_H_MASK
#define SCT_MATCHREL_RELOADn_H_SHIFT             SCT_MATCHREL_RELOADN_H_SHIFT
#define SCT_MATCHREL_RELOADn_H(x)                SCT_MATCHREL_RELOADN_H(x)
#define SCT_EV_STATE_STATEMSKn_MASK              SCT_EV_STATE_STATEMSKN_MASK
#define SCT_EV_STATE_STATEMSKn_SHIFT             SCT_EV_STATE_STATEMSKN_SHIFT
#define SCT_EV_STATE_STATEMSKn(x)                SCT_EV_STATE_STATEMSKN(x)


/*!
 * @}
 */ /* end of group SCT_Peripheral_Access_Layer */


/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic pop
  #else
    #pragma pop
  #endif
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

/*!
 * @}
 */ /* end of group Peripheral_access_layer */


#endif  /* SCT_H_ */

