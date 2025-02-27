#pragma once

// Windows only supports RNDIS
// Mac only supports CDC-ECM
// Linux supports either RNDIS, CDC-ECM or CDC-NCM
#define CFG_TUD_ECM_RNDIS         1
#define BOARD_DEVICE_RHPORT_NUM   0
#define BOARD_DEVICE_RHPORT_SPEED OPT_MODE_FULL_SPEED
#define CFG_TUSB_RHPORT0_MODE     (OPT_MODE_DEVICE | BOARD_DEVICE_RHPORT_SPEED)
