#pragma once

#define CFG_TUD_ECM_RNDIS         1
#if 0
#define CFG_TUD_MSC               1     // This enables mass storage
#define CFG_TUD_MSC_EP_BUFSIZE    512   // Also Add msc_disk.c and descritprs
#endif
#define CFG_TUD_NCM               0
#define BOARD_DEVICE_RHPORT_NUM   0
#define BOARD_DEVICE_RHPORT_SPEED OPT_MODE_FULL_SPEED
#define CFG_TUSB_RHPORT0_MODE     (OPT_MODE_DEVICE | BOARD_DEVICE_RHPORT_SPEED)
