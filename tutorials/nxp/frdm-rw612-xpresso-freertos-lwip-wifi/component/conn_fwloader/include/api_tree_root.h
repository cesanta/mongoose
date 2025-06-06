/*
 * Copyright 2016-2019, 2021,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __API_TREE_ROOT_H__
#define __API_TREE_ROOT_H__

#include "fsl_loader_utils.h"
#include "nboot_rom_api_table.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
// Load and set user appplication boot options stored in specific register
#define LOAD_USER_APP_BOOT_OPTIONS()        (SYSCON_USER_APP_BOOT_OPTIONS)
#define CLEAR_USER_APP_BOOT_OPTIONS()       (SYSCON_USER_APP_BOOT_OPTIONS = 0u)
#define SET_USER_APP_BOOT_OPTIONS(value)    (SYSCON_USER_APP_BOOT_OPTIONS = value)

//! @brief Boot parameters of the user application
//!  WORD    OFFSET      FIELD                              DESCRIPTION
//!          [31:24]     TAG                                Must be '0xEB'
//!          [23:20]     Boot mode                          0:Master boot mode; 1: ISP boot
//!          [19:16]     Boot interface                     0:USART 1:I2C 2:SPI 3:USB HID 4:QSPI 5:USB DFU
//!          [15:12]     Boot instance(Channel)             0 or 1; For SD or MMC,this is to select the instance
//!                                                         For FLEXSPI boot, this select the Channel A or Channel B
//!          [11:08]     Redundant boot image index         Redundant boot image index for FlexSPI NOR flash
//!          [07:00]     Reserved
//!
//!  TAG[31:24]	    BOOT MODE[23:20]    INTERFACE[19:16]    INSTANCE[15:12]     RBII    Reserved[07:00]     COMBINATION     BOOT ACTION
//!  0xEB  	        0                   0	                X	                X       X	                0xEB00XXXX	    MASTR BOOT: USART
//!	                0	                1	                X	                X       X	                0xEB01XXXX	    MASTR BOOT: I2C
//!	                0	                2	                X	                X       X	                0xEB02XXXX	    MASTR BOOT: SPI
//!	                0	                3	                X	                X       X	                0xEB03XXXX	    MASTR BOOT: USB HID
//!	                0	                4	                X	                0       X	                0xEB0400XX	    MASTR BOOT: FlexSPI Channel A:boot image index 0
//!	                0	                4	                X	                1       X	                0xEB0401XX	    MASTR BOOT: FlexSPI Channel A:boot image index 1
//!	                0                   4	                X	                0       X	                0xEB0410XX	    MASTR BOOT: FlexSPI Channel B:boot image index 0
//!	                0	                4	                X	                1       X	                0xEB0411XX	    MASTR BOOT: FlexSPI Channel B:boot image index 1
//!	                0	                5	                X	                X       X	                0xEB05XXXX	    MASTR BOOT: USB DFU
//!	                1	                0	                X	                X       X	                0xEB10XXXX	    ISP BOOT: USART
//!	                1	                1	                X	                X       X	                0xEB11XXXX	    ISP BOOT: I2C
//!	                1	                2	                X	                X       X	                0xEB12XXXX	    ISP BOOT: SPI
//!

typedef struct _fsl_user_app_boot_invoke_option
{
    union
    {
        struct
        {
            uint32_t reserved           : 8;
            uint32_t boot_image_index   : 4;
            uint32_t instance           : 4;
            uint32_t boot_interface     : 4;
            uint32_t mode               : 4;
            uint32_t tag                : 8;
        } B;
        uint32_t U;
    } option;
} fsl_user_app_boot_invoke_option_t;

//! @brief Boot interface can be selected by user application
//! @note  For USB-HID QSPI USB-DFU SD MMC, these interfaces are invalid for ISP boot
enum
{
    kUserAppBootPeripheral_UART     = 0u,
    kUserAppBootPeripheral_I2C      = 1u,
    kUserAppBootPeripheral_SPI      = 2u,
    kUserAppBootPeripheral_USB_HID  = 3u,
    kUserAppBootPeripheral_FLEXSPI  = 4u,
    kUserAppBootPeripheral_DFU      = 5u
};

//! @brief Boot mode can be selected by user application
//! @note  For master boot, valid boot insterfaces for user application are USART I2C SPI USB-HID USB-DFU SD MMC
//!        For ISP boot, valid boot interfaces for user application are USART I2C SPI
enum
{
    kUserAppBootMode_MasterBoot = 0,
    kUserAppBootMode_IspBoot    = 1,
};


//!@brief OTP driver API Interface for A0
typedef struct
{
    uint32_t version;
    status_t (*init)(uint32_t src_clk_freq);
    status_t (*deinit)(void);
    status_t (*fuse_read)(uint32_t addr, uint32_t *data);
    status_t (*fuse_program)(uint32_t addr, uint32_t data, bool lock);
    status_t (*reload)(void);
    status_t (*crc_check)(uint32_t start_addr, uint32_t end_addr, uint32_t crc_addr);
    status_t (*crc_calc)(uint32_t *src, uint32_t numberOfWords, uint32_t *crcChecksum);
    status_t (*crc_check_sw)(uint32_t *src, uint32_t numberOfWords, uint32_t crc_fuse_idx);
} ocotp_driver_v0_t;

//!@brief OTP driver API Interface for A1/A2
typedef struct
{
    uint32_t version;
    status_t (*init)(uint32_t src_clk_freq);
    status_t (*deinit)(void);
    status_t (*fuse_read)(uint32_t addr, uint32_t *data, uint32_t argChk);
    status_t (*fuse_program)(uint32_t addr, uint32_t data, bool lock);
    status_t (*reload)(void);
    status_t (*crc_check)(uint32_t start_addr, uint32_t end_addr, uint32_t crc_addr);
    status_t (*crc_calc)(uint32_t *src, uint32_t numberOfWords, uint32_t *crcChecksum);
    status_t (*crc_check_sw)(uint32_t *src, uint32_t numberOfWords, uint32_t crc_fuse_idx);
} ocotp_driver_v1_t;

//! @brief Root of the bootloader API tree for A0.
//!
//! An instance of this struct resides in read-only memory in the bootloader. It
//! provides a user application access to APIs exported by the bootloader.
//!
//! @note The order of existing fields must not be changed.
//!
//! @ingroup context
typedef struct BootloaderTree_v0
{
    void (*runBootloader)(void *arg);                               //!< Function to start the bootloader executing.
    fsl_standard_version_t                      version;                //!< Bootloader version number.
    const char                              *copyright;             //!< Copyright string.
    const uint32_t                          reservedBootloader2;
    const nboot_interface_v0_t              *nbootDriver;           //!< Image authentication API.
    const uint32_t                          reservedBootloader3;
    const ocotp_driver_v0_t                 *otpDriver;             //!< OTP driver API.
    const fsl_iap_api_interface_t               *iapApiDriver;
} bootloader_tree_v0_t;

//! @brief Root of the bootloader API tree for A1/A2.
//!
//! An instance of this struct resides in read-only memory in the bootloader. It
//! provides a user application access to APIs exported by the bootloader.
//!
//! @note The order of existing fields must not be changed.
//!
//! @ingroup context
typedef struct BootloaderTree_v1
{
    void (*runBootloader)(void *arg);                               //!< Function to start the bootloader executing.
    fsl_standard_version_t                      version;                //!< Bootloader version number.
    const char                              *copyright;             //!< Copyright string.
    const uint32_t                          reservedBootloader2;
    const nboot_interface_v1_t              *nbootDriver;           //!< Image authentication API.
    const uint32_t                          reservedBootloader3;
    const ocotp_driver_v1_t                 *otpDriver;             //!< OTP driver API.
    const fsl_iap_api_interface_t               *iapApiDriver;
} bootloader_tree_v1_t;

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif // __API_TREE_ROOT_H__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
