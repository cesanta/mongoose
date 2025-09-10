# SPDX-License-Identifier: Apache-2.0
# Copyright 2022 NXP

# define part number for this driver

zephyr_compile_definitions_ifdef(CONFIG_88W8987
    SD8987
)

zephyr_compile_definitions_ifdef(CONFIG_IW416
    SD8978
)

zephyr_compile_definitions_ifdef(CONFIG_IW61X
    SD9177
)

zephyr_compile_definitions_ifdef(CONFIG_88W8801
    SD8801
)

zephyr_compile_definitions_ifdef(CONFIG_RW610
    RW610
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_WW_rw610.h"
)

zephyr_compile_definitions_ifdef(CONFIG_88W8987_AW_CM358_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x09
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0C
    WIFI_BT_USE_USD_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_88W8987_AW_CM358MA_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x09
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0C
    WIFI_BT_USE_M2_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_88W8987_MURATA_1ZM_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_1ZM_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x06
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x06
    WIFI_BT_USE_USD_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_88W8987_MURATA_1ZM_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_1ZM_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x06
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x06
    WIFI_BT_USE_M2_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW416_AW_AM457_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x09
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0C
    WIFI_BT_USE_USD_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW416_AW_AM457MA_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x09
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0C
    WIFI_BT_USE_M2_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW416_AW_AM510_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x09
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0C
    WIFI_BT_USE_USD_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW416_AW_AM510MA_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_1XK_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x09
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0C
    WIFI_BT_USE_M2_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW416_MURATA_1XK_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_1XK_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x00
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0C
    WIFI_BT_USE_USD_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW416_MURATA_1XK_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_1XK_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x00
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0C
    WIFI_BT_USE_M2_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW612_MURATA_2EL_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_2EL_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x0A
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0A
    WIFI_BT_USE_USD_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW612_MURATA_2EL_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_2EL_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x0A
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0A
    WIFI_BT_USE_M2_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW611_MURATA_2DL_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_2EL_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x0A
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0A
    WIFI_BT_USE_USD_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_IW611_MURATA_2DL_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_2EL_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x0A
    CONFIG_NXP_WIFI_ED_OFFSET_5G=0x0A
    WIFI_BT_USE_M2_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_88W8801_AW_NM191_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x1B
)

zephyr_compile_definitions_ifdef(CONFIG_88W8801_AW_NM191MA_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x1B
)

zephyr_compile_definitions_ifdef(CONFIG_88W8801_MURATA_2DS_USD
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_2DS_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x0E
)

zephyr_compile_definitions_ifdef(CONFIG_88W8801_MURATA_2DS_M2
    WIFI_BT_TX_PWR_LIMITS="wlan_txpwrlimit_cfg_murata_2DS_WW.h"
    CONFIG_NXP_WIFI_ED_OFFSET_2G=0x0E
)

zephyr_compile_definitions_ifdef(CONFIG_NXP_WIFI_BT_USE_USD_INTERFACE
    WIFI_BT_USE_USD_INTERFACE
)

zephyr_compile_definitions_ifdef(CONFIG_NXP_WIFI_BT_USE_M2_INTERFACE
    WIFI_BT_USE_M2_INTERFACE
)

zephyr_library_compile_definitions(
    FSL_OSA_TASK_ENABLE=1
)

zephyr_library_sources(${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/utilities/misc_utilities/fsl_memcpy.S)

zephyr_library_sources(${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/lists/fsl_component_generic_list.c)

zephyr_library_sources(${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/osa/fsl_os_abstraction_zephyr.c)

zephyr_include_directories(
    incl
    incl/wifidriver
    incl/wlcmgr
    incl/port/net
    incl/port/osa
    incl/port/net/zephyr
    port/osa
    port/net/zephyr
    sdio_nxp_abs/incl
    firmware_dnld
    fwdnld_intf_abs
    wifidriver
    wifidriver/incl
    wifi_bt_firmware
    wifi_bt_firmware/8987
    wifi_bt_firmware/IW416
    wifi_bt_firmware/nw61x
    wifi_bt_firmware/8801
    cli
    ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/osa/
    ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/lists/
    ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/wifi_bt_module/AzureWave/tx_pwr_limits/
    ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/wifi_bt_module/Murata/tx_pwr_limits/
)

# OS abstraction layer source
zephyr_library_sources(
    port/osa/osa.c
    port/osa/osa_zephyr.c
)
# net stack abstraction layer source
zephyr_library_sources(port/net/zephyr/net.c)

file(GLOB WIFI_SRC ./wifidriver/*.c)
list(FILTER WIFI_SRC EXCLUDE REGEX ".*imu.c$")
list(FILTER WIFI_SRC EXCLUDE REGEX ".*sdio.c$")
list(FILTER WIFI_SRC EXCLUDE REGEX ".*firmware_dnld.c$")

zephyr_library_sources_ifdef(CONFIG_RW610
    wifidriver/wifi-imu.c
)

zephyr_library_sources_ifdef(CONFIG_SDIO_STACK
    wifidriver/sdio.c
    sdio_nxp_abs/fwdnld_sdio.c
    sdio_nxp_abs/mlan_sdio.c
    wifidriver/wifi-sdio.c
    firmware_dnld/firmware_dnld.c
    fwdnld_intf_abs/fwdnld_intf_abs.c
)

zephyr_library_sources(${WIFI_SRC})

file(GLOB WLCMGR_SRC ./wlcmgr/*.c)
#list(FILTER WLCMGR_SRC EXCLUDE REGEX ".*tests*")
#list(FILTER WLCMGR_SRC EXCLUDE REGEX ".*cli*")

zephyr_library_sources(${WLCMGR_SRC})

zephyr_library_sources_ifdef(CONFIG_NXP_WIFI_SMOKE_TESTS
    nw_utils/init_enet.c
)

zephyr_include_directories_ifdef(CONFIG_NXP_WIFI_SMOKE_TESTS
    nw_utils
)

if(CONFIG_NXP_WIFI_SIGMA_AGENT)
zephyr_library_sources(
    nw_utils/wifi_ping.c
    sigma_agent/sigma_agent.c
    sigma_agent/dut/wfa_dut.c
    sigma_agent/dut/wfa_dut_init.c
)

file(GLOB SIGMA_SRC ./sigma_agent/lib/*.c)
list(FILTER SIGMA_SRC EXCLUDE REGEX ".*cmdproc*")
list(FILTER SIGMA_SRC EXCLUDE REGEX ".*ca_resp*")
zephyr_library_sources(${SIGMA_SRC})
endif()

zephyr_library_include_directories_ifdef(CONFIG_NXP_WIFI_SIGMA_AGENT
    PRIVATE ${ZEPHYR_BASE}/subsys/net/ip)

zephyr_library_include_directories_ifdef(CONFIG_NXP_WIFI_SIGMA_AGENT
    sigma_agent/inc
    sigma_agent/dut
)

file(GLOB DHCPD_SRC ./dhcpd/*.c)
list(FILTER DHCPD_SRC EXCLUDE REGEX ".*cli*")

zephyr_library_sources(${DHCPD_SRC})
zephyr_library_include_directories(dhcpd)

# nxp wifi shell
zephyr_library_sources(cli/wifi_shell.c)
zephyr_library_sources(cli/cli_utils.c)

# macros for wpa_supplicant
if(CONFIG_WPA_SUPP)
zephyr_include_directories(certs)

zephyr_library_include_directories(wifidriver/wpa_supp_if wifidriver/wpa_supp_if/incl)
file(GLOB WIFI_SUPP_SRC ./wifidriver/wpa_supp_if/*.c)
zephyr_library_sources(${WIFI_SUPP_SRC})
endif()

if(CONFIG_SPEED_OPTIMIZATIONS OR CONFIG_SIZE_OPTIMIZATIONS)
# critical path code relocated to SRAM
zephyr_code_relocate(FILES
                     port/net/net.c
                     port/osa/osa.c
                     port/osa/osa_zephyr.c
                     port/net/zephyr/net.c
                     sdio_nxp_abs/mlan_sdio_zephyr.c
                     wifidriver/mlan_11n.c
                     wifidriver/mlan_11n_aggr.c
                     wifidriver/mlan_11n_rxreorder.c
                     wifidriver/mlan_wmm.c
                     wifidriver/wifi.c
                     LOCATION RAM_TEXT)

if(CONFIG_SDIO_STACK)
zephyr_code_relocate(FILES
                     sdio_nxp_abs/mlan_sdio.c
                     wifidriver/wifi-sdio.c
                     LOCATION RAM_TEXT)
endif()

if(CONFIG_RW610)
zephyr_code_relocate(FILES
                     # fsl_cache.c
                     # ${ZEPHYR_BASE}/modules/hal_nxp/fsl_memcpy.S
                     wifidriver/wifi-imu.c
                     ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/osa/fsl_os_abstraction_zephyr.c
                     ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/drivers/imu/fsl_imu.c
                     ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/rpmsg/fsl_adapter_rfimu.c
                     LOCATION RAM_TEXT)
endif()

zephyr_code_relocate(FILES
                     # fsl_cache.c
                     # ${ZEPHYR_BASE}/modules/hal_nxp/fsl_memcpy.S
                     ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/osa/fsl_os_abstraction_zephyr.c
                     ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/utilities/misc_utilities/fsl_memcpy.S
                     LOCATION RAM_TEXT)

file(GLOB ZPERF_SRC ${ZEPHYR_BASE}/subsys/net/lib/zperf/*.c)
zephyr_code_relocate(FILES ${ZPERF_SRC} LOCATION RAM_TEXT)

file(GLOB SOCKET_SRC ${ZEPHYR_BASE}/subsys/net/lib/sockets/*.c)
zephyr_code_relocate(FILES ${SOCKET_SRC} LOCATION RAM_TEXT)

file(GLOB NET_SRC ${ZEPHYR_BASE}/subsys/net/ip/*.c)
zephyr_code_relocate(FILES ${NET_SRC} LOCATION RAM_TEXT)

file(GLOB ETH_SRC ${ZEPHYR_BASE}/subsys/net/l2/ethernet/*.c)
zephyr_code_relocate(FILES ${ETH_SRC} LOCATION RAM_TEXT)

zephyr_code_relocate(FILES ${ZEPHYR_BASE}/subsys/net/buf.c LOCATION RAM_TEXT)

zephyr_code_relocate(FILES
                     ${ZEPHYR_BASE}/kernel/mem_slab.c
                     ${ZEPHYR_BASE}/kernel/mempool.c
                     ${ZEPHYR_BASE}/kernel/msg_q.c
                     ${ZEPHYR_BASE}/kernel/mutex.c
                     ${ZEPHYR_BASE}/kernel/queue.c
                     ${ZEPHYR_BASE}/kernel/sched.c
                     ${ZEPHYR_BASE}/kernel/sem.c
                     ${ZEPHYR_BASE}/kernel/thread.c
                     ${ZEPHYR_BASE}/kernel/work.c
                     LOCATION RAM_TEXT)
endif()

zephyr_library_link_libraries_ifdef(CONFIG_MBEDTLS mbedTLS)
