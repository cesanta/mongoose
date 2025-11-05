#pragma once

#if MG_ENABLE_TCPIP
#if !defined(MG_ENABLE_DRIVER_STM32H)
#define MG_ENABLE_DRIVER_STM32H 0
#endif
#if !defined(MG_ENABLE_DRIVER_MCXN)
#define MG_ENABLE_DRIVER_MCXN 0
#endif
#if !defined(MG_ENABLE_DRIVER_STM32N)
#define MG_ENABLE_DRIVER_STM32N 0
#endif
#if MG_ENABLE_DRIVER_STM32H || MG_ENABLE_DRIVER_MCXN || MG_ENABLE_DRIVER_STM32N

struct mg_tcpip_driver_stm32h_data {
  // MDC clock divider. MDC clock is derived from HCLK, must not exceed 2.5MHz
  //    HCLK range    DIVIDER    mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    HCLK/42        0
  //    100-150 MHz   HCLK/62        1
  //    20-35 MHz     HCLK/16        2
  //    35-60 MHz     HCLK/26        3
  //    150-250 MHz   HCLK/102       4  <-- value for max speed HSI
  //    250-300 MHz   HCLK/124       5  <-- value for Nucleo-H* on CSI
  //    300-500 MHz   HCLK/204       6
  //    500-800 MHz   HCLK/324       7
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5

  uint8_t phy_addr;  // PHY address
  uint8_t phy_conf;  // PHY config
};

#ifndef MG_TCPIP_PHY_CONF
#define MG_TCPIP_PHY_CONF MG_PHY_CLOCKS_MAC
#endif

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 4
#endif

#if MG_ENABLE_DRIVER_STM32H && MG_ARCH == MG_ARCH_CUBE
#define MG_ENABLE_ETH_IRQ() NVIC_EnableIRQ(ETH_IRQn)
#else
#define MG_ENABLE_ETH_IRQ()
#endif

#if MG_ENABLE_IPV6
#define MG_IPV6_INIT(mif)                                         \
  do {                                                            \
    memcpy(mif.ip6ll, (uint8_t[16]) MG_TCPIP_IPV6_LINKLOCAL, 16);     \
    memcpy(mif.ip6, (uint8_t[16]) MG_TCPIP_GLOBAL, 16);           \
    memcpy(mif.gw6, (uint8_t[16]) MG_TCPIP_GW6, 16);              \
    mif.prefix_len = MG_TCPIP_PREFIX_LEN;                        \
  } while(0)
#else
#define MG_IPV6_INIT(mif)
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_stm32h_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                    \
    driver_data_.phy_conf = MG_TCPIP_PHY_CONF;                    \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_stm32h;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    MG_IPV6_INIT(mif_);                                           \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_ENABLE_ETH_IRQ();                                          \
    MG_INFO(("Driver: stm32h, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
#endif
