/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "fsl_loader_utils.h"
#include "api_tree_root.h"
#include "fsl_power.h"
#ifndef __ZEPHYR__
#include "board.h"
#include "fsl_debug_console.h"
#else
#include "fsl_clock.h"
#endif

#ifdef MCUBOOT_APPLICATION
#include "mcuboot_app_support.h"
#endif

#include "mflash_drv.h"

#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
#if defined(PSA_CRYPTO_DRIVER_THREAD_EN)
#include "mcux_psa_els_pkc_common_init.h"
#else
#include "els_pkc_mbedtls.h"
#endif /* defined(PSA_CRYPTO_DRIVER_THREAD_EN) */
#endif /* defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT) */

//! @addtogroup sbloader
//! @{

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if 0
#define SBLOADER_PRINTF(...)     \
    do                           \
    {                            \
        PLOG_DEBUG(__VA_ARGS__); \
    } while (0)
#else
#define SBLOADER_PRINTF(...)
#endif

/*!
 * @brief load cpu1/cpu2 firmware.
 */
#define CIU_RST_SW2                (0x41240184U)
#define CIU_RST_SW2_CPU1_RST_MASK  (0x40000000U)
#define CIU2_RST_SW3               (0x4424011cU)
#define CIU2_RST_SW3_CPU2_RST_MASK (0x00000010U)

#define IMU_SYNC_MAGIC_PATTERN  (0xABCDEF89U)
#define CPU1_MAGIC_PATTERN_ADDR (0x41380000U)
#define CPU2_MAGIC_PATTERN_ADDR (0x443c0000U)

#define SOCCTRL_CHIP_INFO_REV_NUM_MASK (0xFU)


#define CLKCTL0_PSCCTL1_OTP_MASK  (0x20000U)
#define RSTCTL0_PRSTCTL1_OTP_MASK (0x20000U)

#define STAGING_BUF_SZ 256u
typedef struct sb3_desc {
    uint32_t fmt;
    uint32_t sub_fmt;
    uint32_t dst_addr;
    uint32_t area_sz;
} sb3_load_desc_t;

/*******************************************************************************
 * Prototype
 ******************************************************************************/
static status_t ldr_DoHeader_v3(fsl_api_core_context_t *ctx);
static status_t ldr_DoDataRead(fsl_api_core_context_t *ctx);
static status_t ldr_DoBlock(fsl_api_core_context_t *ctx);
static status_t ldr_DoLoadCmd(fsl_api_core_context_t *ctx);
static status_t ldr_DoExecuteCmd(fsl_api_core_context_t *ctx);
static status_t ldr_ReadFromFlash(uint8_t * buf, uint32_t src_flash_offset, size_t read_sz);

static status_t load_service_monolithic(LOAD_Target_Type loadTarget, uint32_t sourceAddr);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief nboot library context. */
static fsl_api_core_context_t s_fsl_api_core_context = {0};
static fsl_ldr_Context_v3_t s_sbloader_context;
static uint8_t packetBuf[512]  = {0};
static fsl_nboot_context_t g_nbootCtx = {0};
#ifdef CONFIG_FW_VDLLV2
static uint32_t vdll_image_base = 0;
#endif

static bootloader_tree_v0_t *g_bootloaderTree_v0;
static bootloader_tree_v1_t *g_bootloaderTree_v1;

/*******************************************************************************
 * Codes
 ******************************************************************************/
__attribute__((__noinline__))
static void sb3_Delay(uint32_t loop)
{
    if (loop > 0U)
    {
        __ASM volatile(
            "1:                             \n"
            "    SUBS   %0, %0, #1          \n"
            "    CMP    %0, #0              \n"
            "    BNE    1b                  \n"
            :
            : "r"(loop));
    }
}

static void sb3_DelayUs(uint32_t us)
{
    uint32_t instNum;

    instNum = ((SystemCoreClock + 999999UL) / 1000000UL) * us;
    sb3_Delay((instNum + 2U) / 3U);
}

static uint32_t _ActiveApplicationRemapOffset(void)
{
    return (MFLASH_FLEXSPI->HADDROFFSET);
}
////////////////////////////////////////////////////////////////////////////
//! @brief power on device implementation
////////////////////////////////////////////////////////////////////////////
void power_on_device_impl(LOAD_Target_Type loadTarget)
{
    uint8_t target_type = ((uint8_t)loadTarget & ~0x80);

    if (LOAD_WIFI_FIRMWARE == target_type)
    {
        POWER_PowerOnWlan();
    }
    else if ((LOAD_BLE_FIRMWARE == target_type) || (LOAD_15D4_FIRMWARE == target_type))
    {
        POWER_PowerOnBle();
    }
    else
    {
        ; /* none to do */
    }
    // There's 2.6us gap from device Power-On till device sub-system power up.
    // Do a time delay which >2.6us for device sub-system here.
    sb3_DelayUs(5U);
}

////////////////////////////////////////////////////////////////////////////
//! @brief power off device implementation
////////////////////////////////////////////////////////////////////////////
void power_off_device_impl(LOAD_Target_Type loadTarget)
{
    uint8_t target_type = ((uint8_t)loadTarget & ~0x80);

    if (LOAD_WIFI_FIRMWARE == target_type)
    {
        POWER_PowerOffWlan();
    }
    else if ((LOAD_BLE_FIRMWARE == target_type) || (LOAD_15D4_FIRMWARE == target_type))
    {
        POWER_PowerOffBle();
    }
    else
    {
        ; /* none to do */
    }
    sb3_DelayUs(5U);
}

////////////////////////////////////////////////////////////////////////////
//! @brief reset device
////////////////////////////////////////////////////////////////////////////
void reset_device(LOAD_Target_Type loadTarget)
{
    uint8_t target_type = ((uint8_t)loadTarget & ~0x80);
    if (LOAD_WIFI_FIRMWARE == target_type)
    {
        *((uint32_t *)CIU_RST_SW2) = *((uint32_t *)CIU_RST_SW2) | CIU_RST_SW2_CPU1_RST_MASK;
    }
    else if ((LOAD_BLE_FIRMWARE == target_type) || (LOAD_15D4_FIRMWARE == target_type))
    {
        *((uint32_t *)CIU2_RST_SW3) = *((uint32_t *)CIU2_RST_SW3) | CIU2_RST_SW3_CPU2_RST_MASK;
    }
    else
    {
        ; /* none to do */
    }
}

static fsl_ldr_Context_v3_t *get_sbloader_v3_context(fsl_api_core_context_t *ctx)
{
    return ctx->sbloaderCtx;
}

////////////////////////////////////////////////////////////////////////////
//! @brief get firmware version from otp
////////////////////////////////////////////////////////////////////////////
static fsl_nboot_status_t nboot_hal_get_secure_firmware_version(uint32_t *fwVer, LOAD_Target_Type loadTarget)
{
    if (fwVer == NULL)
    {
        return kStatus_NBOOT_InvalidArgument;
    }

    uint32_t tmpVersion               = 0u;
    uint32_t trustedFwVersionFuses[4] = {0};
    uint32_t fuseIdxStart;

    uint8_t target_type = (uint8_t)loadTarget & ~0x80;
     if (LOAD_WIFI_FIRMWARE == target_type)
    {
        fuseIdxStart = OTP_WIFI_FW_VER0_FUSE_IDX;
    }
    else if (LOAD_BLE_FIRMWARE == target_type)
    {
        fuseIdxStart = OTP_BLE_FW_VER0_FUSE_IDX;
    }
    else if (LOAD_15D4_FIRMWARE == target_type)
    {
        fuseIdxStart = OTP_15_4_FW_VER0_FUSE_IDX;
    }
    else
    {
        return kStatus_NBOOT_InvalidArgument;
    }

    for (uint32_t i = 0u; i < ARRAY_SIZE(trustedFwVersionFuses); i++)
    {
        status_t status = OCOTP_OtpFuseRead(fuseIdxStart, &trustedFwVersionFuses[i]);
        if (status != kStatus_Success)
        {
            return kStatus_NBOOT_Fail;
        }
        ++fuseIdxStart;
    }

    for (uint32_t i = 0u; i < ARRAY_SIZE(trustedFwVersionFuses); i++)
    {
        // Only the low-half 16-bit is used for counter calculation
        for (uint8_t j = 0U; j < 16U; j++)
        {
            if ((trustedFwVersionFuses[i] & (uint32_t)((uint32_t)(1U) << j)) != 0U)
            {
                ++tmpVersion;
            }
        }
    }

    *fwVer = tmpVersion;

    return kStatus_NBOOT_Success;
}
////////////////////////////////////////////////////////////////////////////
//! @brief fw download implementation
////////////////////////////////////////////////////////////////////////////
status_t sb3_fw_download_impl(LOAD_Target_Type loadTarget, uint32_t flag, uint32_t sourceAddr)
{
    volatile uint32_t *magic_pattern_addr = NULL;
    status_t status                       = kStatus_Fail;
    int wait_count                        = 200;
    uint8_t target_type                   = ((uint8_t)loadTarget & ~0x80);

    if ((g_bootloaderTree_v1 == NULL) && ((get_chip_revision() == 1U) || (get_chip_revision() == 2U)))
    {
        g_bootloaderTree_v1 = ((bootloader_tree_v1_t *)0x13030000);
    }
    else if (g_bootloaderTree_v0 == NULL)
    {
        g_bootloaderTree_v0 = ((bootloader_tree_v0_t *)0x13024100);
    }
	else
    {
        ; /* none to do */
    }

    if (LOAD_WIFI_FIRMWARE == target_type)
    {
        magic_pattern_addr = (volatile uint32_t *)CPU1_MAGIC_PATTERN_ADDR;
    }
    else if ((LOAD_BLE_FIRMWARE == target_type) || (LOAD_15D4_FIRMWARE == target_type))
    {
        magic_pattern_addr = (volatile uint32_t *)CPU2_MAGIC_PATTERN_ADDR;
    }
#ifdef CONFIG_FW_VDLLV2
    else if (LOAD_WIFI_VDLL_FIRMWARE == loadTarget)
    {
        status = load_service(loadTarget, sourceAddr);
        return status;
    }
#endif
    else
    {
        return status;
    }

    // Check if fw already active, if active skip download fw
    if (IMU_SYNC_MAGIC_PATTERN == *((volatile uint32_t *)magic_pattern_addr))
    {
        status = kStatus_Success;
        return status;
    }

    if (loadTarget & 0x80)
    {
        status = load_service_monolithic(loadTarget, sourceAddr);
    }
    else
    {
        status = load_service(loadTarget, sourceAddr);
    }

    // Wait for fw activation for 1s. Return fail if wait_count is used up.
    while (wait_count != 0)
    {
        if (IMU_SYNC_MAGIC_PATTERN != *((volatile uint32_t *)magic_pattern_addr))
        {
            /* 5 ms delay */
            sb3_DelayUs(1000 * 5);
            wait_count--;
            if (wait_count == 0)
            {
                status = kStatus_Fail;
            }
        }
        else
        {
            status = kStatus_Success;
            break;
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief fw reset implementation
////////////////////////////////////////////////////////////////////////////
status_t sb3_fw_reset_impl(LOAD_Target_Type loadTarget, uint32_t flag, uint32_t sourceAddr)
{
    status_t status = kStatus_Fail;

    power_off_device_impl(loadTarget);

    status = sb3_fw_download_impl(loadTarget, flag, sourceAddr);

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief load command processing
////////////////////////////////////////////////////////////////////////////
static status_t ldr_DoLoadCmd(fsl_api_core_context_t *ctx)
{
    status_t status = kStatus_Fail;

    fsl_ldr_Context_v3_t *context = get_sbloader_v3_context(ctx);

    // check current data_block_position
    if (context->data_block_position == context->block_data_size)
    {
        // we reached end of data section, need to get the next block
        status = kStatus_Success;
    }
    else if (context->data_block_position + context->data_range_header.length <= context->block_data_size)
    {
        // the load data enough in data section (buffer)
        (void)memcpy((uint8_t *)context->data_range_header.startAddress,
                     (uint8_t *)&context->data_block[context->data_block_position], context->data_range_header.length);
        // this load command completed.
        // this data range section process finiskStatus_FLASH_Successhed.
        context->in_data_range      = false;
        context->data_range_handled = 0;
        context->data_block_position += context->data_range_header.length + context->data_range_gap;
        context->data_section_handled += context->data_range_header.length + context->data_range_gap;
        status = kStatus_Success;
    }
    else
    {
        // we have partial data to load
        (void)memcpy((uint8_t *)context->data_range_header.startAddress,
                     (uint8_t *)&context->data_block[context->data_block_position],
                     context->block_data_size - context->data_block_position);
        context->data_range_handled = context->block_data_size - context->data_block_position;
        context->data_range_header.startAddress += context->data_range_handled;
        context->data_range_header.length -= context->data_range_handled;
        context->data_block_position += context->data_range_handled;
        context->data_section_handled += context->data_range_handled;
        status = kStatus_Success;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief Execute command processing
////////////////////////////////////////////////////////////////////////////
static status_t ldr_DoExecuteCmd(fsl_api_core_context_t *ctx)
{
    fsl_ldr_Context_v3_t *context = get_sbloader_v3_context(ctx);
    // this data range section process finished.
    context->in_data_range      = false;
    context->data_range_handled = 0;

    // Actual jump is implemented in fsl_sbloader_finalize().
    return (status_t)kStatusRomLdrPendingJumpCommand;
}

////////////////////////////////////////////////////////////////////////////
//! @brief data block processing
////////////////////////////////////////////////////////////////////////////
static status_t ldr_DoBlock(fsl_api_core_context_t *ctx)
{
    fsl_ldr_Context_v3_t *context = get_sbloader_v3_context(ctx);

    status_t status = kStatus_Fail;
    // new data range with new data block
    fsl_sb3_data_range_header_t *data_range_header;
    fsl_sb3_section_header_t *data_section_header;

    while (context->in_data_block)
    {
        // check if we are in a data section
        if (context->in_data_section)
        {
            // in process of a data section
            data_section_header = &context->data_section_header;
        }
        else
        {
            // new data section started
            data_section_header = (fsl_sb3_section_header_t *)(void *)&context->data_block[context->data_block_position];

            // save data range section header
            (void)memcpy(&context->data_section_header, data_section_header, sizeof(fsl_sb3_section_header_t));

            // branch to section types (only data range is currently supported)
            switch (data_section_header->sectionType)
            {
                case ((uint32_t)kSectionNone):
                    status = kStatus_Success;
                    break;
                case ((uint32_t)kSectionDataRange):
                    context->in_data_section      = true;
                    context->data_section_handled = 0;
                    context->in_data_range        = false;
                    context->data_range_handled   = 0;
                    context->data_block_position += sizeof(fsl_sb3_section_header_t);
                    break;
                case ((uint32_t)kSectionDiffUpdate):
                case ((uint32_t)kSectionDDRConfig):
                case ((uint32_t)kSectionRegister):
                default:
                    // non-supported section type
                    SBLOADER_PRINTF("Bootloader: %s, invalid section type = %x", __func__,
                                    data_section_header->sectionType);
                    status = kStatus_Fail;
                    break;
            }
            if (data_section_header->sectionType != (uint32_t)kSectionDataRange)
            {
                return status;
            }
        }

        switch (data_section_header->sectionType)
        {
            case ((uint32_t)kSectionDataRange):
            {
                // check if we are in a data range
                if (context->in_data_range)
                {
                    // continue current data range process
                    data_range_header = &context->data_range_header;
                }
                else
                {
                    // started a new data range
                    data_range_header =
                        (fsl_sb3_data_range_header_t *)(void *)&context->data_block[context->data_block_position];

                    // check command tag
                    if (data_range_header->tag != SB3_DATA_RANGE_HEADER_TAG)
                    {
                        // bad data range section
                        status = kStatus_Fail;
                        SBLOADER_PRINTF("Bootloader: %s, invalid data range header tag = %x", __func__,
                                        data_range_header->tag);
                        return status;
                    }

                    // save data range section header
                    (void)memcpy(&context->data_range_header, data_range_header, sizeof(fsl_sb3_data_range_header_t));
                    context->in_data_range            = true;
                    context->has_data_range_expansion = false;
                    context->data_range_handled       = sizeof(fsl_sb3_data_range_header_t); // used anywhere?
                    context->data_block_position += sizeof(fsl_sb3_data_range_header_t);
                    context->data_section_handled += sizeof(fsl_sb3_data_range_header_t);

                    // 16 bytes alignmnent check and handling
                    context->data_range_gap = 0;

                    switch ((fsl_sb3_cmd_t)data_range_header->cmd)
                    {
                        case kSB3_CmdLoad:
                            context->data_range_gap =
                                (SB3_DATA_ALIGNMENT_SIZE_IN_BYTE - (data_range_header->length & 0xFU)) & 0xFU;
                            break;
                        default:
                            // Do nothing for the commands.
                            break;
                    }
                }

                switch (data_range_header->cmd)
                {
                    case ((uint32_t)kSB3_CmdLoad):
                        if (!context->has_data_range_expansion)
                        {
                            SBLOADER_PRINTF("Bootloader: %s, Copy data range expansion", __func__);
                            // check current data_block_position
                            if (context->data_block_position == context->block_data_size)
                            {
                                // we reached end of data section, need to get the next block
                                return kStatus_Success;
                            }
                            else if ((context->data_block_position + sizeof(fsl_sb3_data_range_expansion_t)) <=
                                     context->block_data_size)
                            {
                                // save data range section header expansion.
                                (void)memcpy(&context->data_range_expansion,
                                             (fsl_sb3_data_range_expansion_t *)&context
                                                 ->data_block[context->data_block_position],
                                             sizeof(fsl_sb3_data_range_expansion_t));
                                context->has_data_range_expansion = true;
                                context->data_block_position += sizeof(fsl_sb3_data_range_expansion_t);
                                context->data_section_handled += sizeof(fsl_sb3_data_range_expansion_t);
                            }
                            else
                            {
                                // Unaligned data range.
                                return kStatus_Fail;
                            }
                        }
                        else
                        {
                            SBLOADER_PRINTF("Bootloader: %s, Has data range expansion", __func__);
                        }
                        break;
                    default:
                        // Do nothing for the commands without header expansion.
                        break;
                }

                // branch to range commands
                switch (data_range_header->cmd)
                {
                    case ((uint32_t)kSB3_CmdLoad):
                        status = ldr_DoLoadCmd(ctx);
                        if (status != kStatus_Success)
                        {
                            return status;
                        }
                        break;
                    case ((uint32_t)kSB3_CmdExecute):
                        status = ldr_DoExecuteCmd(ctx);
                        break;
                    default:
                        // this data range section process finished.
                        context->in_data_range      = false;
                        context->data_range_handled = 0;
                        break;
                }
                if (data_range_header->cmd == (uint32_t)kSB3_CmdExecute)
                {
                    return status;
                }

                // check if we reach the end of this data section
                if (context->data_section_handled == context->data_section_header.length)
                {
                    // this data section completed
                    context->in_data_section = false;
                }
                else if (context->data_section_handled > context->data_section_header.length)
                {
                    // bad data section
                    // return error
                    SBLOADER_PRINTF("Bootloader: %s, bad data section.", __func__);
                    status = kStatus_Fail;
                    return status;
                }
                else
                {
                    ; /* none to do */
                }

                // check if we reach the end of this data block
                if (context->data_block_position == context->block_data_size)
                {
                    // This data block process finished.
                    context->block_buffer_position = 0;
                    context->in_data_block         = false;
                    context->data_block_position   = 0;
                    context->Action                = (fsl_pLdrFnc_v3_t)ldr_DoDataRead;
                    SBLOADER_PRINTF("Bootloader: %s, data blobck process done.", __func__);
                    status = kStatus_Success;
                }
                else if (context->data_block_position > context->block_data_size)
                {
                    // bad block position
                    // bad pointer
                    SBLOADER_PRINTF("Bootloader: %s, bad block position.", __func__);
                    status = kStatus_Fail;
                    return status;
                }
                else if (context->data_block_position + SB3_DATA_ALIGNMENT_SIZE_IN_BYTE > context->block_data_size)
                {
                    // data is not 16 bytes aligned.
                    // return error
                    status = kStatus_Fail;
                    SBLOADER_PRINTF("Bootloader: %s, data not aligned.", __func__);
                    return status;
                }
                else
                {
                    ; /* none to do */
                }
            }
            break;
            case ((uint32_t)kSectionDiffUpdate):
            case ((uint32_t)kSectionDDRConfig):
            case ((uint32_t)kSectionRegister):
            default:
                // non-supported section type
                SBLOADER_PRINTF("Bootloader: %s, non-supported section type = %x", __func__,
                                data_section_header->sectionType);
                status                 = kStatus_Fail;
                context->in_data_block = false;
                break;
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief data block decryption and handling
////////////////////////////////////////////////////////////////////////////
static status_t ldr_DoDataRead(fsl_api_core_context_t *ctx)
{
    status_t status                = kStatus_Fail;
    fsl_nboot_status_t nbootResult = kStatus_NBOOT_Fail;

    fsl_ldr_Context_v3_t *context = get_sbloader_v3_context(ctx);

    // check block integrity
    if (context->processedBlocks < ctx->nbootCtx->totalBlocks)
    {
        // call nboot lib to decrypt the data block
        if ((get_chip_revision() == 1U) || (get_chip_revision() == 2U))
        {
            nbootResult = (fsl_nboot_status_t)g_bootloaderTree_v1->nbootDriver->nboot_sb3_load_block(
                ctx->nbootCtx, (uint32_t *)&context->block_buffer[0]);
        }
        else
        {
            nbootResult = (fsl_nboot_status_t)g_bootloaderTree_v0->nbootDriver->nboot_sb3_load_block(
                ctx->nbootCtx, (uint32_t *)&context->block_buffer[0]);
        }
        if (nbootResult == kStatus_NBOOT_Success)
        {
            context->block_buffer_position = 0;
#if defined(NBOOT_IGNORE_SB3_COMMANDS)
            if (g_nboot_ctx.processData != NBOOT_IGNORE_SB3_COMMANDS)
#endif
            {
                context->in_data_block       = true;
                context->data_block          = &context->block_buffer[context->data_block_offset];
                context->data_block_position = 0;
                context->processedBlocks++;
                status = ldr_DoBlock(ctx);
            }
#if defined(NBOOT_IGNORE_SB3_COMMANDS)
            else
            {
                status = kStatus_Success;
            }
#endif
        }
        else
        {
            SBLOADER_PRINTF("ROM API: %s, nboot_sb3_load_block is failed, status = %x", __func__, nbootResult);
            status = kStatus_Fail;
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief header block handling
////////////////////////////////////////////////////////////////////////////
static status_t ldr_DoHeader_v3(fsl_api_core_context_t *ctx)
{
    status_t status = kStatus_Success;
    fsl_nboot_sb3_load_manifest_parms_t manifestParms;

    fsl_ldr_Context_v3_t *context = get_sbloader_v3_context(ctx);

    do
    {
        fsl_nboot_sb3_header_t *header = (fsl_nboot_sb3_header_t *)(void *)&context->block_buffer[0];
        if (context->block_buffer_size == sizeof(fsl_nboot_sb3_header_t))
        {
            // Update the buffer size to the size of Block 0.
            context->block_buffer_size = header->imageTotalLength;
            SBLOADER_PRINTF("ROM API: %s, manifest size = %x, but buffer size =%x", __func__,
                            context->block_buffer_size, sizeof(context->block_buffer));
            if (context->block_buffer_size > sizeof(context->block_buffer))
            {
                status = kStatus_Fail;
                break;
            }
            // Resume the cleared buffer position.
            context->block_buffer_position = sizeof(fsl_nboot_sb3_header_t);

            status = kStatus_Success;
            break;
        }

        if (context->block_buffer_size != header->imageTotalLength)
        {
            status = kStatus_Fail;
            break;
        }

        (void)memset(&manifestParms, 0, sizeof(fsl_nboot_sb3_load_manifest_parms_t));

        fsl_nboot_status_t nbootResult = nboot_hal_get_sb3_manifest_params(ctx->nbootCtx, &manifestParms);
        if (nbootResult != kStatus_NBOOT_Success)
        {
            SBLOADER_PRINTF("ROM API: %s, nboot_hal_get_sb3_manifest_params is failed, status = %x", __func__,
                            nbootResult);
            status = kStatus_Fail;
            break;
        }

        // call nboot lib to verify the block header
        if ((get_chip_revision() == 1U) || (get_chip_revision() == 2U))
        {
            nbootResult = (fsl_nboot_status_t)g_bootloaderTree_v1->nbootDriver->nboot_sb3_load_manifest(
                ctx->nbootCtx, (uint32_t *)(void *)header, &manifestParms);
        }
        else
        {
            nbootResult = (fsl_nboot_status_t)g_bootloaderTree_v0->nbootDriver->nboot_sb3_load_manifest(
                ctx->nbootCtx, (uint32_t *)(void *)header, &manifestParms);
        }
        if (nbootResult == kStatus_NBOOT_Success)
        {
            context->data_block_offset = (uint8_t)(sizeof(uint32_t) /* blockNumber*/ + header->certificateBlockOffset -
                                                   sizeof(fsl_nboot_sb3_header_t));
            context->block_buffer_size = (uint32_t)context->data_block_offset + NBOOT_SB3_CHUNK_SIZE_IN_BYTES;
            context->block_buffer_position = 0;
            context->block_size            = header->blockSize;
            context->block_data_size       = NBOOT_SB3_CHUNK_SIZE_IN_BYTES;
            context->block_data_total      = context->block_size * ctx->nbootCtx->totalBlocks;
            context->in_data_section       = false;
            context->data_section_handled  = 0;
            context->processedBlocks       = 0;
            context->Action                = (fsl_pLdrFnc_v3_t)ldr_DoDataRead;
            status                         = kStatus_Success;
        }
        else
        {
            SBLOADER_PRINTF("ROM API: %s, nboot_sb3_load_manifest is failed, status = %x", __func__, nbootResult);
            status = kStatus_Fail;
        }
    } while (false);

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief Initialize the loader state machine.
////////////////////////////////////////////////////////////////////////////
status_t fsl_sbloader_init(fsl_api_core_context_t *ctx)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if (ctx == NULL)
        {
            break;
        }

        fsl_ldr_Context_v3_t *context = ctx->sbloaderCtx;

        // Initialize the context
        (void)memset(context, 0, sizeof(fsl_ldr_Context_v3_t));
        context->block_buffer_size = sizeof(fsl_nboot_sb3_header_t);

        // Process the first chunk of the image header
        context->Action = (fsl_pLdrFnc_v3_t)ldr_DoHeader_v3;

        // Initialize the allowed command set
        context->commandSet = SBLOADER_V3_CMD_SET_ALL;

        status = kStatus_Success;

    } while (false);

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief Finalize the loader operations
////////////////////////////////////////////////////////////////////////////
status_t fsl_sbloader_finalize(fsl_api_core_context_t *ctx)
{
    status_t status = kStatus_Fail;

    fsl_ldr_Context_v3_t *context = get_sbloader_v3_context(ctx);

    if (context->data_range_header.cmd == (uint32_t)kSB3_CmdExecute)
    {
        status = kStatus_Success;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief Pump the loader state machine.///////////////////////////////////
static status_t fsl_sbloader_pump(fsl_api_core_context_t *ctx, uint8_t *data, uint32_t length)
{
    status_t status = kStatus_InvalidArgument;
    do
    {
        fsl_ldr_Context_v3_t *context = ctx->sbloaderCtx;
        uint32_t required             = 0U;
        uint32_t available            = 0U;
        uint32_t readPosition         = 0U;

        while (readPosition < length)
        {
            required  = context->block_buffer_size - context->block_buffer_position;
            available = length - readPosition;

            // copy what we need to complete a full chunk into the chunk buffer
            if ((required > 0U) && (available > 0U))
            {
                uint32_t toCopy = required > available ? available : required;
                if ((context->block_buffer_position < context->block_buffer_size) &&
                    (context->block_buffer_position + toCopy <= context->block_buffer_size) &&
                    (readPosition + toCopy <= length))
                {
                    if ((context->block_buffer_position + toCopy) >= sizeof(context->block_buffer))
                    {
                        // block buffer over-flow.
                        SBLOADER_PRINTF("ROM API: %s, block buffer is overflown", __func__);
                        status = kStatus_Fail;
                        break;
                    }
                    (void)memcpy(&context->block_buffer[context->block_buffer_position], &data[readPosition], toCopy);
                    required -= toCopy;
                    available -= toCopy;
                    readPosition += toCopy;
                    context->block_buffer_position += toCopy;
                    status = kStatus_Success;
                }
                else
                {
                    status = kStatus_Fail;
                    break;
                }
            }

            if (required == 0U)
            {
                // a full chunk was filled to process it
                context->block_buffer_position = 0U;
                status                         = (context->Action)(ctx);

                if (status != kStatus_Success)
                {
                    if (status != (status_t)kStatusRomLdrPendingJumpCommand)
                    {
                        SBLOADER_PRINTF("sbloader Action failed: 0x%08x", status);
                    }
                    break;
                }
            }
            else if (available == 0U)
            {
                // otherwise we are just going to wait for more data
                status = (status_t)kStatusRomLdrDataUnderrun;
                break;
            }
            else
            {
                ; /* None to do */
            }
        }
    } while (false);

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief Read flash area loading to RAM buffer.
// Direct read from flash is not allowed when remapping is active.
// buf             : pointer to RAM buffer, its size must be sufficient to receive
//                   the required number of bytes.
// src_flash_offset: 'virtual' address in flash relative to start of flash storage.
//                   Actual 'physical' address results from the addition of the remap offset.
// read_sz         : Number of bytes to be read.
////////////////////////////////////////////////////////////////////////////
static status_t ldr_ReadFromFlash(uint8_t * buf, uint32_t src_flash_offset, size_t read_sz)
{
    status_t st;
    static const uint32_t mflash_base = (1u << 27);
    uint32_t remap_offset = _ActiveApplicationRemapOffset();
    if (remap_offset == 0U)
    {
        memcpy(buf, (void*)src_flash_offset, read_sz);
        st = kStatus_Success;
    }
    else
    {
        // similar to mflash_drv_log2phys
        uint32_t phys_offset = (src_flash_offset + remap_offset) & ~mflash_base;
        st = mflash_drv_read(phys_offset, (uint32_t *)buf, read_sz);
    }
    return st;
}

////////////////////////////////////////////////////////////////////////////
//! @brief Read SB3 area descriptor.
// Direct read from flash is not allowed when remapping is active.
// hdr       : pointer to RAM fsl_nboot_sb3_header_t structure.
// sourceAddr: 'virtual' address where SB3 header is expected.
////////////////////////////////////////////////////////////////////////////
status_t read_nboot_sb3_header(fsl_nboot_sb3_header_t * hdr, uint32_t sourceAddr)
{
   return ldr_ReadFromFlash((uint8_t*)hdr, sourceAddr, sizeof(fsl_nboot_sb3_header_t));
}

////////////////////////////////////////////////////////////////////////////
//! @brief load service with format of sb3
// readOffset: image offset in flash
////////////////////////////////////////////////////////////////////////////
status_t loader_process_sb_file(uint32_t readOffset)
{
    status_t status                    = kStatus_Fail;
    uint32_t packetLength              = sizeof(packetBuf);
    s_fsl_api_core_context.sbloaderCtx = &s_sbloader_context;
    s_fsl_api_core_context.nbootCtx    = &g_nbootCtx;
    bool elsFlag                       = false;
    uint32_t CSS_CTRL_context          = 0;
#ifdef CONFIG_FW_VDLLV2
    uint32_t counter;
#endif

    do
    {
        (void)POWER_EnableGDetVSensors();
        if (((CLKCTL0->PSCCTL0 & CLKCTL0_PSCCTL0_ELS_MASK) == 0U) ||
            ((CLKCTL0->PSCCTL1 & CLKCTL0_PSCCTL1_ELS_APB_MASK) == 0U) ||
            ((RSTCTL0->PRSTCTL0 & RSTCTL0_PRSTCTL0_ELS_MASK) != 0U))
        {
            elsFlag = true;
            CLOCK_EnableClock(kCLOCK_Els);
            CLOCK_EnableClock(kCLOCK_ElsApb);
            RESET_PeripheralReset(kELS_RST_SHIFT_RSTn);
        }

#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
        (void)mcux_els_mutex_lock();
#endif

        if ((get_chip_revision() == 1U) || (get_chip_revision() == 2U))
        {
            status = (int32_t)g_bootloaderTree_v1->nbootDriver->nboot_context_init(s_fsl_api_core_context.nbootCtx);
        }
        else
        {
            status = (int32_t)g_bootloaderTree_v0->nbootDriver->nboot_context_init(s_fsl_api_core_context.nbootCtx);
        }
        if (status != (status_t)kStatus_NBOOT_Success)
        {
            break;
        }

        status = fsl_sbloader_init(&s_fsl_api_core_context);
        if (status != kStatus_Success)
        {
            break;
        }

        // Pump the sbloader content and do sbloader handling until ROM see the jump command and jump to the image
        while (true)
        {
            memcpy(packetBuf, (void*)readOffset, packetLength);

            if ((get_chip_revision() == 1U) || (get_chip_revision() == 2U))
            {
                status = fsl_sbloader_pump(&s_fsl_api_core_context, packetBuf, packetLength);
            }
            else
            {
                status = g_bootloaderTree_v0->iapApiDriver->fsl_sbloader_pump(&s_fsl_api_core_context, packetBuf,
                                                                              packetLength);
            }

            // kStatusRomLdrDataUnderrun means need more data
            // kStatusRomLdrSectionOverrun means we reached the end of the sb file processing
            // either of these are OK
            if ((status == (status_t)kStatusRomLdrDataUnderrun) || (status == (status_t)kStatusRomLdrSectionOverrun))
            {
                status = kStatus_Success;
            }
            else if (status == (status_t)kStatusRomLdrPendingJumpCommand)
            {
                status = fsl_sbloader_finalize(&s_fsl_api_core_context);
#ifdef CONFIG_FW_VDLLV2
                assert((readOffset & 0x3U) == 0U);
                for (counter = 0; counter < (packetLength + 7U) >> 2U; counter++)
                {
                    if (*(uint32_t *)readOffset == TAG_SB_V3)
                    {
                        vdll_image_base = readOffset;
                        break;
                    }
                    else
                    {
                        readOffset += 4U;
                    }
                }
#endif
                break;
            }
            else
            {
                ; /* No necessary actions. */
            }

            if (status != kStatus_Success)
            {
                break;
            }

            readOffset += packetLength;
        }
    } while (false);

    if (get_chip_revision() == 0U)
    {
        CSS_CTRL_context = ELS->ELS_CTRL;
    }

    if ((get_chip_revision() == 1U) || (get_chip_revision() == 2U))
    {
        (void)g_bootloaderTree_v1->nbootDriver->nboot_context_deinit(s_fsl_api_core_context.nbootCtx);
    }
    else
    {
        (void)g_bootloaderTree_v0->nbootDriver->nboot_context_deinit(s_fsl_api_core_context.nbootCtx);
    }

    if (get_chip_revision() == 0U)
    {
        ELS->ELS_CTRL = (CSS_CTRL_context & 0xFFU);
    }

#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
    (void)mcux_els_mutex_unlock();
#endif

    if (elsFlag == true)
    {
        RESET_SetPeripheralReset(kELS_RST_SHIFT_RSTn);
        CLOCK_DisableClock(kCLOCK_ElsApb);
        CLOCK_DisableClock(kCLOCK_Els);
    }
    POWER_DisableGDetVSensors();

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief load service with format of raw binary image
// readOffset: image offset in flash
////////////////////////////////////////////////////////////////////////////
static status_t loader_process_raw_file(uint32_t readOffset)
{
    status_t status = kStatus_Fail;
    uint32_t *src_addr;
    uint32_t *dst_addr;
    uint32_t code_size;
    uint32_t *data_ptr      = (uint32_t *)readOffset;
    uint32_t total_raw_size = 0;

#ifdef CONFIG_FW_VDLLV2
    if ((*data_ptr == LOADER_RAW_BINARY_FORMAT) && (*(data_ptr + 1) == LOADER_VDLL_RAW_BINARY_FORMAT))
    {
        src_addr  = data_ptr + 4;
        dst_addr  = (uint32_t *)*(data_ptr + 2);
        code_size = *(data_ptr + 3);
        (void)memcpy(dst_addr, src_addr, code_size);
        status = kStatus_Success;
    }
    else
#endif
    {
        do
        {
            if (*data_ptr != LOADER_RAW_BINARY_FORMAT)
            {
                break;
            }

            src_addr  = data_ptr + 4;
            dst_addr  = (uint32_t *)*(data_ptr + 2);
            code_size = *(data_ptr + 3);
            // Check for raw ending segment
            if (((uint32_t)src_addr == 0xffffffffU) || ((uint32_t)dst_addr == 0xffffffffU))
            {
                if (code_size == total_raw_size)
                {
                    status = kStatus_Success;
#ifdef CONFIG_FW_VDLLV2
                    vdll_image_base = (uint32_t)(data_ptr + 4);
#endif
                }
                break;
            }

            (void)memcpy(dst_addr, src_addr, code_size);
            data_ptr += 4U + (code_size >> 2U);
            total_raw_size += code_size;
        } while (true);
    }

    return status;
}

static status_t loader_process_raw_file_monolithic(uint32_t readOffset)
{
    status_t status = kStatus_Fail;
    uint32_t *dst_addr;
    uint32_t code_size;
    uint32_t sz;


    uint32_t total_raw_size = 0U;
    uint8_t staging_buf[STAGING_BUF_SZ] = { 0u };

    do {
        sb3_load_desc_t *p_desc = (sb3_load_desc_t*)&staging_buf[0];
        status_t flash_st;
        /* Firs read the SB3 area descriptor */
        flash_st = ldr_ReadFromFlash(&staging_buf[0], readOffset, sizeof(sb3_load_desc_t));
        if (flash_st != kStatus_Success)
        {
            break;
        }
        readOffset += sizeof(sb3_load_desc_t);

        if (p_desc->fmt != LOADER_RAW_BINARY_FORMAT)
        {
            break;
        }

        dst_addr  = (uint32_t*)p_desc->dst_addr;
        code_size = p_desc->area_sz;

        // Check for raw ending segment
        if (p_desc->dst_addr == 0xffffffffU)
        {
            if (code_size == total_raw_size)
            {
                status = kStatus_Success;
#ifdef CONFIG_FW_VDLLV2
                vdll_image_base = readOffset;
#endif
            }
            break;
        }

        /* start of indirect memcpy to destination */
        sz = code_size;
        while (sz >= STAGING_BUF_SZ)
        {
            flash_st = ldr_ReadFromFlash(&staging_buf[0], readOffset, STAGING_BUF_SZ);
            if (flash_st != kStatus_Success)
            {
                break;
            }
            (void)memcpy(dst_addr, &staging_buf[0], STAGING_BUF_SZ);
            readOffset += STAGING_BUF_SZ;
            dst_addr += STAGING_BUF_SZ/4;
            sz -= STAGING_BUF_SZ;
        }
        if (flash_st != kStatus_Success)
        {
            break;
        }
        /* last chunk smaller than staging buffer */
        if (sz > 0U)
        {
            flash_st = ldr_ReadFromFlash(&staging_buf[0], readOffset, sz);
            if (flash_st != kStatus_Success)
            {
                break;
            }
            (void)memcpy(dst_addr, &staging_buf[0], sz);
            readOffset += sz;
            dst_addr += (sz+3U)/4U;
        }
        /* at this point are is fully consumed and copied to destination : */
#ifdef CONFIG_FW_VDLLV2
        if ((p_desc->sub_fmt == LOADER_VDLL_RAW_BINARY_FORMAT))
        {
            status = kStatus_Success;
            break;
        }
#endif
        total_raw_size += code_size;

    } while (true);

    return status;
}

#ifndef __ZEPHYR__
static bool __FlexSpiFlashInit(void)
{
    bool ret = false;
    if (((CLKCTL0->PSCCTL0 & CLKCTL0_PSCCTL0_FLEXSPI0_MASK) == 0U) ||
        ((RSTCTL0->PRSTCTL0 & RSTCTL0_PRSTCTL0_FLEXSPI0_MASK) != 0U))
    {
        CLOCK_EnableClock(kCLOCK_Flexspi);
        RESET_PeripheralReset(kFLEXSPI_RST_SHIFT_RSTn);
        BOARD_SetFlexspiClock(FLEXSPI, 2U, 2U);
        BOARD_InitFlash(FLEXSPI);
        ret = true;
    }
    return ret;
}

static void __FlexSpiFlashDeInit(void)
{
    RESET_ClearPeripheralReset(kFLEXSPI_RST_SHIFT_RSTn);
    BOARD_DeinitFlash(FLEXSPI);
    CLOCK_AttachClk(kNONE_to_FLEXSPI_CLK);
    CLOCK_DisableClock(kCLOCK_Flexspi);
    RESET_SetPeripheralReset(kFLEXSPI_RST_SHIFT_RSTn);
}

#endif

static int __OtpInit(void)
{
    int ret = 0; /* will stay 0 if nothing to do */
    if (((CLKCTL0->PSCCTL1 & CLKCTL0_PSCCTL1_OTP_MASK) == 0U) ||
        ((RSTCTL0->PRSTCTL1 & RSTCTL0_PRSTCTL1_OTP_MASK) != 0U))
    {
        status_t st;
#ifdef USE_OCOTP_DRIVER_IN_LOAD_SERVICE
        st = OCOTP_OtpInit();
#else
        if ((get_chip_revision() == 1U) || (get_chip_revision() == 2U))
        {
            st = g_bootloaderTree_v1->otpDriver->init(0U);
        }
        else
        {
            RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);
            st = g_bootloaderTree_v0->otpDriver->init(0U);
        }
#endif
        if (st != kStatus_Success)
        {
            ret = -1;
        }
        else
        {
            ret = 1;
        }
    }
    return ret;
}

static int __OtpDeInit(void)
{
    int ret = -1;
#ifdef USE_OCOTP_DRIVER_IN_LOAD_SERVICE
        if (OCOTP_OtpDeinit() == kStatus_Success)
        {
            ret = 0;
        }
#else
        if ((get_chip_revision() == 1U) || (get_chip_revision() == 2U))
        {
            if (g_bootloaderTree_v1->otpDriver->deinit() == kStatus_Success)
            {
                ret = 0;
            }
        }
        else
        {
            if (g_bootloaderTree_v0->otpDriver->deinit() == kStatus_Success)
            {
                ret = 0;
            }
        }
#endif
    return ret;
}


////////////////////////////////////////////////////////////////////////////
//! @brief load service
// loadTarget: LOAD_WIFI_FIRMWARE / LOAD_BLE_FIRMWARE / LOAD_15D4_FIRMWARE
// sourceAddr: load firmware source address, if 0 load default address
////////////////////////////////////////////////////////////////////////////
status_t load_service(LOAD_Target_Type loadTarget, uint32_t sourceAddr)
{
    status_t status = kStatus_Fail;
    fsl_nboot_sb3_header_t *pt_a_ptr;
    fsl_nboot_sb3_header_t *pt_b_ptr;
    fsl_nboot_sb3_header_t *active_pt_ptr;
    uint32_t firmwareVersion = 0xFFFFFFFFU;
    int otp_status           = 0;
#ifndef __ZEPHYR__
    bool flexspiFlag         = __FlexSpiFlashInit();
#endif

    if (LOAD_WIFI_FIRMWARE == loadTarget)
    {
        if (sourceAddr == 0U)
        {
            pt_a_ptr = (fsl_nboot_sb3_header_t *)WIFI_IMAGE_A_OFFSET;
            pt_b_ptr = (fsl_nboot_sb3_header_t *)WIFI_IMAGE_B_OFFSET;
        }
        else
        {
            pt_a_ptr = (fsl_nboot_sb3_header_t *)sourceAddr;
            pt_b_ptr = (fsl_nboot_sb3_header_t *)(sourceAddr + WIFI_IMAGE_SIZE_MAX);
        }
    }
    else if (LOAD_BLE_FIRMWARE == loadTarget)
    {
        if (sourceAddr == 0U)
        {
            pt_a_ptr = (fsl_nboot_sb3_header_t *)BLE_IMAGE_A_OFFSET;
            pt_b_ptr = (fsl_nboot_sb3_header_t *)BLE_IMAGE_B_OFFSET;
        }
        else
        {
            pt_a_ptr = (fsl_nboot_sb3_header_t *)sourceAddr;
            pt_b_ptr = (fsl_nboot_sb3_header_t *)(sourceAddr + BLE_IMAGE_SIZE_MAX);
        }
    }
    else if (LOAD_15D4_FIRMWARE == loadTarget)
    {
        if (sourceAddr == 0U)
        {
            pt_a_ptr = (fsl_nboot_sb3_header_t *)Z154_IMAGE_A_OFFSET;
            pt_b_ptr = (fsl_nboot_sb3_header_t *)Z154_IMAGE_B_OFFSET;
        }
        else
        {
            pt_a_ptr = (fsl_nboot_sb3_header_t *)sourceAddr;
            pt_b_ptr = (fsl_nboot_sb3_header_t *)(sourceAddr + Z154_IMAGE_SIZE_MAX);
        }
    }
#ifdef CONFIG_FW_VDLLV2
    else if (LOAD_WIFI_VDLL_FIRMWARE == loadTarget)
    {
        assert(vdll_image_base != 0U);
        pt_a_ptr = (fsl_nboot_sb3_header_t *)(vdll_image_base + sourceAddr);
        pt_b_ptr = NULL;
    }
#endif
    else
    {
        return kStatus_Fail;
    }

#ifdef MCUBOOT_APPLICATION
    {
        /* Skip MCUBoot header if present */

        struct image_header *header;

        header = (void *)pt_a_ptr;
        if (header->ih_magic == IMAGE_MAGIC)
        {
            pt_a_ptr = (void *)(((uint8_t *)pt_a_ptr) + header->ih_hdr_size);
        }

        header = (void *)pt_b_ptr;
        if (header->ih_magic == IMAGE_MAGIC)
        {
            pt_b_ptr = (void *)(((uint8_t *)pt_b_ptr) + header->ih_hdr_size);
        }
    }
#endif

    otp_status = __OtpInit();
    if (otp_status < 0)
    {
        return kStatus_Fail;
    }

#ifdef CONFIG_FW_VDLLV2
    if (LOAD_WIFI_VDLL_FIRMWARE != loadTarget)
#endif
    {
        if (nboot_hal_get_secure_firmware_version(&firmwareVersion, loadTarget) != kStatus_NBOOT_Success)
        {
            return kStatus_Fail;
        }

        // imu init may be called before or after load_service(), not sure user will do in which sequence.
        // If imu init is before load_service(), it is not appropriate do Power-Off here, then comment out Power-Off.
        // power_off_device_impl(); // temporarily comment out for PDM Non-UPF version

        power_on_device_impl(loadTarget);
    }

    /* Check partition TAG and select active partition */
    if ((pt_a_ptr->magic != TAG_SB_V3) && (pt_b_ptr->magic != TAG_SB_V3))
    {
        active_pt_ptr = pt_a_ptr;
        status        = loader_process_raw_file((uint32_t)active_pt_ptr);
    }
    else if ((pt_a_ptr->magic == TAG_SB_V3) && (pt_b_ptr->magic != TAG_SB_V3))
    {
        active_pt_ptr = pt_a_ptr;
#ifdef CONFIG_FW_VDLLV2
        if (LOAD_WIFI_VDLL_FIRMWARE != loadTarget)
        {
#endif
            if (active_pt_ptr->firmwareVersion < firmwareVersion)
            {
                return kStatus_Fail;
            }
#ifdef CONFIG_FW_VDLLV2
        }
#endif
        status = loader_process_sb_file((uint32_t)active_pt_ptr);
    }
    else if ((pt_a_ptr->magic != TAG_SB_V3) && (pt_b_ptr->magic == TAG_SB_V3))
    {
        active_pt_ptr = pt_b_ptr;
        if (active_pt_ptr->firmwareVersion < firmwareVersion)
        {
            return kStatus_Fail;
        }
        status = loader_process_sb_file((uint32_t)active_pt_ptr);
    }
    else
    {
        if (pt_a_ptr->firmwareVersion >= pt_b_ptr->firmwareVersion)
        {
            active_pt_ptr = pt_a_ptr;
        }
        else
        {
            active_pt_ptr = pt_b_ptr;
        }

        if (active_pt_ptr->firmwareVersion < firmwareVersion)
        {
            return kStatus_Fail;
        }
        status = loader_process_sb_file((uint32_t)active_pt_ptr);
    }

    if (otp_status != 0)
    {
        /* OTP init was done here */
        (void)__OtpDeInit();
    }

    if (status == kStatus_Success)
    {
        reset_device(loadTarget);
    }

#ifndef __ZEPHYR__
    if (flexspiFlag)
    {
        __FlexSpiFlashDeInit();
    }
#endif
    return status;
}


static status_t load_service_monolithic(LOAD_Target_Type loadTarget, uint32_t sourceAddr)
{
    status_t status = kStatus_Fail;
    uint32_t hdr_a = 0UL;
    fsl_nboot_sb3_header_t boot_hdr;
    fsl_nboot_sb3_header_t *pt_a_ptr = &boot_hdr;
    uint32_t firmwareVersion = 0xFFFFFFFFU;
    uint32_t sel_fw_ver;
    int otp_status           = 0;

    memset(&boot_hdr, 0xff, sizeof(fsl_nboot_sb3_header_t));

#ifndef __ZEPHYR__
    bool flexspiFlag         = __FlexSpiFlashInit();
#endif
    (void)mflash_drv_init();
    do {
        status_t ret = kStatus_Fail;
        if ((LOAD_WIFI_FW_MONOLITHIC != loadTarget) && (LOAD_BLE_FW_MONOLITHIC != loadTarget) && (LOAD_15D4_FW_MONOLITHIC != loadTarget))
        {
            break;
        }
        if (sourceAddr == 0UL)
        {
            break;
        }
        hdr_a = sourceAddr;

        otp_status = __OtpInit();
        if (otp_status < 0)
        {
            break;
        }

        if (nboot_hal_get_secure_firmware_version(&firmwareVersion, loadTarget) != kStatus_NBOOT_Success)
        {
           break;
        }

        // imu init may be called before or after load_service(), not sure user will do in which sequence.
        // If imu init is before load_service(), it is not appropriate do Power-Off here, then comment out Power-Off.
        // power_off_device_impl(); // temporarily comment out for PDM Non-UPF version

        power_on_device_impl(loadTarget);

        ret = ldr_ReadFromFlash((uint8_t*)pt_a_ptr, hdr_a, sizeof(fsl_nboot_sb3_header_t ));
        if (ret != kStatus_Success)
        {
            break;
        }
        /* Check partition TAG and select active partition */
        if (pt_a_ptr->magic == TAG_SB_V3)
        {
            sel_fw_ver = pt_a_ptr->firmwareVersion;

            if (sel_fw_ver < firmwareVersion)
            {
                break;
            }
            status = loader_process_sb_file(hdr_a);
        }
        else
        {
            status = loader_process_raw_file_monolithic(hdr_a);
        }

    } while (false);

    if (otp_status != 0)
    {
        /* OTP init was done here so undo it here */
        (void)__OtpDeInit();
    }

    if (status == kStatus_Success)
    {
        reset_device(loadTarget);
    }
#ifndef __ZEPHYR__
    if (flexspiFlag)
    {
        __FlexSpiFlashDeInit();
    }
#endif
    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief get chip revision
////////////////////////////////////////////////////////////////////////////
uint8_t get_chip_revision(void)
{
    return (uint8_t)(SOCCTRL->CHIP_INFO & SOCCTRL_CHIP_INFO_REV_NUM_MASK);
}

//! @}
////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////