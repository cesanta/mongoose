#include "driver_teensy41.h"

// Author: Jean-Charles Mammana
// Email: jc_mammana@hotmail.com
//
// Code based from :
// https://github.com/ssilverman/QNEthernet
// https://github.com/PaulStoffregen/teensy41_ethernet

#if defined(MG_ENABLE_MIP) && (MG_ARCH == MG_ARCH_TEENSY41)

//#define DRIVER_TEENSY41_PROMISCUOUS_MODE  1
//#define DRIVER_TEENSY41_BUFFERS_IN_RAM1   1

#define MAX_FRAME_LEN 1522
#define ETH_PAD_SIZE 2

// Hardware checksums computation needs the checksum's fields cleared (value = 0) 
// otherwise the MAC will set the values to 0xFFFF
/*
#define CHECKSUM_HW_CHECK_PROTO
#define CHECKSUM_HW_CHECK_IP
#define CHECKSUM_HW_GEN_PROTO
#define CHECKSUM_HW_GEN_IP
*/

#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define RMII_PAD_INPUT_PULLDOWN 0x30E9
#define RMII_PAD_INPUT_PULLUP 0xB0E9
#define RMII_PAD_CLOCK 0x0031

#define RX_SIZE 5
#define TX_SIZE 5
#define IRQ_PRIORITY 64

// Buffer size for transferring to and from the Ethernet MAC. The frame size is
// either 1518 or 1522, assuming a 1500-byte payload, depending on whether VLAN
// support is desired. VLAN support requires an extra 4 bytes. The ARM cache
// management functions require 32-bit alignment, but the ENETx_MRBR max.
// receive buffer size register says that the RX buffer size must be a multiple
// of 64 and >= 256.
//
// [1518 or 1522 made into a multiple of 32 for ARM cache flush sizing and a
// multiple of 64 for ENETx_MRBR.]
#define BUF_SIZE ((1522 + 63) & (uintptr_t) (-64))

#ifndef DRIVER_TEENSY41_BUFFERS_IN_RAM1
#define MULTIPLE_OF_32(x) (((x) + 31) & (uintptr_t) (-32))
#define BUFFER_DMAMEM DMAMEM
#else
#define BUFFER_DMAMEM
#endif  // !DRIVER_TEENSY41_BUFFERS_IN_RAM1

// Defines the control and status region of the receive buffer descriptor.
typedef enum _enet_rx_bd_control_status {
  kEnetRxBdEmpty = 0x8000U,            // Empty bit
  kEnetRxBdRxSoftOwner1 = 0x4000U,     // Receive software ownership
  kEnetRxBdWrap = 0x2000U,             // Update buffer descriptor
  kEnetRxBdRxSoftOwner2 = 0x1000U,     // Receive software ownership
  kEnetRxBdLast = 0x0800U,             // Last BD in the frame (L bit)
  kEnetRxBdMiss = 0x0100U,             // In promiscuous mode; needs L
  kEnetRxBdBroadcast = 0x0080U,        // Broadcast
  kEnetRxBdMulticast = 0x0040U,        // Multicast
  kEnetRxBdLengthViolation = 0x0020U,  // Receive length violation; needs L
  kEnetRxBdNonOctet = 0x0010U,  // Receive non-octet aligned frame; needs L
  kEnetRxBdCrc = 0x0004U,       // Receive CRC or frame error; needs L
  kEnetRxBdOverrun = 0x0002U,   // Receive FIFO overrun; needs L
  kEnetRxBdTrunc = 0x0001U      // Frame is truncated
} enet_rx_bd_control_status_t;

// Defines the control extended region1 of the receive buffer descriptor.
typedef enum _enet_rx_bd_control_extend0 {
  kEnetRxBdIpHeaderChecksumErr = 0x0020U,  // IP header checksum error; needs L
  kEnetRxBdProtocolChecksumErr = 0x0010U,  // Protocol checksum error; needs L
  kEnetRxBdVlan = 0x0004U,                 // VLAN; needs L
  kEnetRxBdIpv6 = 0x0002U,                 // Ipv6 frame; needs L
  kEnetRxBdIpv4Fragment = 0x0001U,         // Ipv4 fragment; needs L
} enet_rx_bd_control_extend0_t;

// Defines the control extended region2 of the receive buffer descriptor.
typedef enum _enet_rx_bd_control_extend1 {
  kEnetRxBdMacErr = 0x8000U,     // MAC error; needs L
  kEnetRxBdPhyErr = 0x0400U,     // PHY error; needs L
  kEnetRxBdCollision = 0x0200U,  // Collision; needs L
  kEnetRxBdUnicast = 0x0100U,    // Unicast frame; valid even if L is not set
  kEnetRxBdInterrupt = 0x0080U,  // Generate RXB/RXF interrupt
} enet_rx_bd_control_extend1_t;

// Defines the control status of the transmit buffer descriptor.
typedef enum _enet_tx_bd_control_status {
  kEnetTxBdReady = 0x8000U,         // Ready bit
  kEnetTxBdTxSoftOwner1 = 0x4000U,  // Transmit software ownership
  kEnetTxBdWrap = 0x2000U,          // Wrap buffer descriptor
  kEnetTxBdTxSoftOwner2 = 0x1000U,  // Transmit software ownership
  kEnetTxBdLast = 0x0800U,          // Last BD in the frame (L bit)
  kEnetTxBdTransmitCrc = 0x0400U,   // Transmit CRC; needs L
} enet_tx_bd_control_status_t;

// Defines the control extended region1 of the transmit buffer descriptor.
typedef enum _enet_tx_bd_control_extend0 {
  kEnetTxBdTxErr = 0x8000U,               // Transmit error; needs L
  kEnetTxBdTxUnderflowErr = 0x2000U,      // Underflow error; needs L
  kEnetTxBdExcessCollisionErr = 0x1000U,  // Excess collision error; needs L
  kEnetTxBdTxFrameErr = 0x0800U,          // Frame with error; needs L
  kEnetTxBdLatecollisionErr = 0x0400U,    // Late collision error; needs L
  kEnetTxBdOverflowErr = 0x0200U,         // Overflow error; needs L
  kEnetTxTimestampErr = 0x0100U,          // Timestamp error; needs L
} enet_tx_bd_control_extend0_t;

// Defines the control extended region2 of the transmit buffer descriptor.
typedef enum _enet_tx_bd_control_extend1 {
  kEnetTxBdTxInterrupt = 0x4000U,  // Transmit interrupt; all BDs
  kEnetTxBdTimestamp = 0x2000U,    // Transmit timestamp flag; all BDs
  kEnetTxBdProtChecksum =
      0x1000U,  // Insert protocol specific checksum; all BDs
  kEnetTxBdIpHdrChecksum = 0x0800U,  // Insert IP header checksum; all BDs
} enet_tx_bd_control_extend1_t;

typedef struct {
  uint16_t length;
  uint16_t status;
  void *buffer;
  uint16_t extend0;
  uint16_t extend1;
  uint16_t checksum;
  uint8_t prototype;
  uint8_t headerlen;
  uint16_t unused0;
  uint16_t extend2;
  uint32_t timestamp;
  uint16_t unused1;
  uint16_t unused2;
  uint16_t unused3;
  uint16_t unused4;
} enetbufferdesc_t;

static enetbufferdesc_t rx_ring[RX_SIZE] __attribute__((aligned(64)));
static enetbufferdesc_t tx_ring[TX_SIZE] __attribute__((aligned(64)));
static BUFFER_DMAMEM uint8_t rxbufs[RX_SIZE * BUF_SIZE]
    __attribute__((aligned(64)));
static BUFFER_DMAMEM uint8_t txbufs[TX_SIZE * BUF_SIZE]
    __attribute__((aligned(64)));
static volatile uint32_t rx_ready;
volatile static enetbufferdesc_t *p_rxbd = &rx_ring[0];
volatile static enetbufferdesc_t *p_txbd = &tx_ring[0];

void _teensy41_enet_isr() {
  if (ENET_EIR & ENET_EIR_RXF) {
    ENET_EIR = ENET_EIR_RXF;
    rx_ready = 1;
  }
}

// Get the built-in Ethernet MAC address.
void _teensy41_getmac(uint8_t *mac) {
  for (uint8_t by = 0; by < 2; by++)
    mac[by] = (HW_OCOTP_MAC1 >> ((1 - by) * 8)) & 0xFF;
  for (uint8_t by = 0; by < 4; by++)
    mac[by + 2] = (HW_OCOTP_MAC0 >> ((3 - by) * 8)) & 0xFF;
}

// Read a PHY register (using MDIO & MDC signals).
uint16_t _teensy41_mdio_read(int phyaddr, int regaddr) {
  ENET_MMFR = ENET_MMFR_ST(1) | ENET_MMFR_OP(2) | ENET_MMFR_TA(2) |
              ENET_MMFR_PA(phyaddr) | ENET_MMFR_RA(regaddr);
  // int count=0;
  while ((ENET_EIR & ENET_EIR_MII) == 0) {
    // count++; // wait
  }
  // print("mdio read waited ", count);
  uint16_t data = ENET_MMFR;
  ENET_EIR = ENET_EIR_MII;
  // printhex("mdio read:", data);
  return data;
}

// Write a PHY register (using MDIO & MDC signals).
void _teensy41_mdio_write(int phyaddr, int regaddr, uint16_t data) {
  ENET_MMFR = ENET_MMFR_ST(1) | ENET_MMFR_OP(1) | ENET_MMFR_TA(2) |
              ENET_MMFR_PA(phyaddr) | ENET_MMFR_RA(regaddr) |
              ENET_MMFR_DATA(data);
  // int count = 0;
  while ((ENET_EIR & ENET_EIR_MII) == 0) {
    // count++;  // wait
  }
  ENET_EIR = ENET_EIR_MII;
  // print("mdio write waited ", count);
  // printhex("mdio write :", data);
}

/*
static void _teensy41_low_level_deinit() {
  // Gracefully stop any transmission before disabling the Ethernet MAC
  ENET_TCR |= ENET_TCR_GTS;
  while ((ENET_EIR & ENET_EIR_GRA) == 0) {
    // Wait until it's gracefully stopped
  }
  ENET_EIR = ENET_EIR_GRA;

  // Disable the Ethernet MAC
  // Note: All interrupts are cleared when Ethernet is reinitialized,
  //       so nothing will be pending
  ENET_ECR &= ~ENET_ECR_ETHEREN;

  // Power down the PHY
  GPIO7_GDIR |= (1 << 15);
  GPIO7_DR_CLEAR = (1 << 15);

  // Stop the PLL
  CCM_ANALOG_PLL_ENET = CCM_ANALOG_PLL_ENET_POWERDOWN;

  // Disable the clock for ENET
  CCM_CCGR1 &= ~CCM_CCGR1_ENET(CCM_CCGR_ON);
}
*/

static void _teensy41_low_level_init(uint8_t *mac) {
  CCM_CCGR1 |= CCM_CCGR1_ENET(CCM_CCGR_ON);
  // Configure PLL6 for 50 MHz, pg 1118 (Rev. 2, 1173 Rev. 1)
  CCM_ANALOG_PLL_ENET_SET = CCM_ANALOG_PLL_ENET_BYPASS;
  CCM_ANALOG_PLL_ENET_CLR = CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC(3) |
                            CCM_ANALOG_PLL_ENET_ENET2_DIV_SELECT(3) |
                            CCM_ANALOG_PLL_ENET_DIV_SELECT(3);
  CCM_ANALOG_PLL_ENET_SET = CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN |
                            // CCM_ANALOG_PLL_ENET_ENET2_REF_EN |
                            CCM_ANALOG_PLL_ENET_ENABLE |
                            // CCM_ANALOG_PLL_ENET_ENET2_DIV_SELECT(1) |
                            CCM_ANALOG_PLL_ENET_DIV_SELECT(1);
  CCM_ANALOG_PLL_ENET_CLR = CCM_ANALOG_PLL_ENET_POWERDOWN;
  while ((CCM_ANALOG_PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK) == 0) {
    // Wait for PLL lock
  }
  CCM_ANALOG_PLL_ENET_CLR = CCM_ANALOG_PLL_ENET_BYPASS;
  // printf("PLL6 = %08" PRIX32 "h (should be 80202001h)\n",
  // CCM_ANALOG_PLL_ENET);

  // Configure REFCLK to be driven as output by PLL6, pg 329 (Rev. 2, 326 Rev.
  // 1)
  CLRSET(IOMUXC_GPR_GPR1,
         IOMUXC_GPR_GPR1_ENET1_CLK_SEL | IOMUXC_GPR_GPR1_ENET_IPG_CLK_S_EN,
         IOMUXC_GPR_GPR1_ENET1_TX_CLK_DIR);

  // Configure pins
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_14 = 5;  // Reset    B0_14 Alt5 GPIO7.15
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_15 = 5;  // Power    B0_15 Alt5 GPIO7.14
  GPIO7_GDIR |= (1 << 14) | (1 << 15);
  GPIO7_DR_SET = (1 << 15);                                    // Power on
  GPIO7_DR_CLEAR = (1 << 14);                                  // Reset PHY chip
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_04 = RMII_PAD_INPUT_PULLDOWN;  // PhyAdd[0] = 0
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_06 = RMII_PAD_INPUT_PULLDOWN;  // PhyAdd[1] = 1
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_05 =
      RMII_PAD_INPUT_PULLUP;  // Master/Slave = slave mode
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_11 =
      RMII_PAD_INPUT_PULLDOWN;  // Auto MDIX Enable
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_07 = RMII_PAD_INPUT_PULLUP;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_08 = RMII_PAD_INPUT_PULLUP;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_09 = RMII_PAD_INPUT_PULLUP;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_10 = RMII_PAD_CLOCK;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_05 =
      3;  // RXD1    B1_05 Alt3, pg 529 (Rev. 2, 525 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_04 =
      3;  // RXD0    B1_04 Alt3, pg 528 (Rev. 2, 524 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_10 =
      6 | 0x10;  // REFCLK    B1_10 Alt6, pg 534 (Rev. 2, 530 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_11 =
      3;  // RXER    B1_11 Alt3, pg 535 (Rev. 2, 531 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_06 =
      3;  // RXEN    B1_06 Alt3, pg 530 (Rev. 2, 526 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_09 =
      3;  // TXEN    B1_09 Alt3, pg 533 (Rev. 2, 529 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_07 =
      3;  // TXD0    B1_07 Alt3, pg 531 (Rev. 2, 527 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_08 =
      3;  // TXD1    B1_08 Alt3, pg 532 (Rev. 2, 528 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_15 =
      0;  // MDIO    B1_15 Alt0, pg 539 (Rev. 2, 535 Rev. 1)
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_14 =
      0;  // MDC     B1_14 Alt0, pg 538 (Rev. 2, 534 Rev. 1)
  IOMUXC_ENET_MDIO_SELECT_INPUT =
      2;  // GPIO_B1_15_ALT0, pg 796 (Rev. 2, 792 Rev. 1)
  IOMUXC_ENET0_RXDATA_SELECT_INPUT =
      1;  // GPIO_B1_04_ALT3, pg 796 (Rev. 2, 792 Rev. 1)
  IOMUXC_ENET1_RXDATA_SELECT_INPUT =
      1;  // GPIO_B1_05_ALT3, pg 797 (Rev. 2, 793 Rev. 1)
  IOMUXC_ENET_RXEN_SELECT_INPUT =
      1;  // GPIO_B1_06_ALT3, pg 798 (Rev. 2, 794 Rev. 1)
  IOMUXC_ENET_RXERR_SELECT_INPUT =
      1;  // GPIO_B1_11_ALT3, pg 799 (Rev. 2, 795 Rev. 1)
  IOMUXC_ENET_IPG_CLK_RMII_SELECT_INPUT =
      1;  // GPIO_B1_10_ALT6, pg 795 (Rev. 2, 791 Rev. 1)
  delayMicroseconds(2);
  GPIO7_DR_SET = (1 << 14);  // Start PHY chip
  ENET_MSCR = ENET_MSCR_MII_SPEED(9);
  delayMicroseconds(5);

  // LEDCR offset 0x18, set LED_Link_Polarity, pg 62
  _teensy41_mdio_write(0, 0x18, 0x0280);  // LED shows link status, active high
  // RCSR offset 0x17, set RMII_Clock_Select, pg 61
  _teensy41_mdio_write(0, 0x17, 0x0081);  // Config for 50 MHz clock input

  memset(rx_ring, 0, sizeof(rx_ring));
  memset(tx_ring, 0, sizeof(tx_ring));

  for (int i = 0; i < RX_SIZE; i++) {
    rx_ring[i].buffer = &rxbufs[i * BUF_SIZE];
    rx_ring[i].status = kEnetRxBdEmpty;
    rx_ring[i].extend1 = kEnetRxBdInterrupt;
  }
  // The last buffer descriptor should be set with the wrap flag
  rx_ring[RX_SIZE - 1].status |= kEnetRxBdWrap;

  for (int i = 0; i < TX_SIZE; i++) {
    tx_ring[i].buffer = &txbufs[i * BUF_SIZE];
    tx_ring[i].status = kEnetTxBdTransmitCrc;
    tx_ring[i].extend1 = kEnetTxBdTxInterrupt
#ifdef CHECKSUM_HW_CHECK_PROTO
      | kEnetTxBdProtChecksum 
#endif
#ifdef CHECKSUM_HW_CHECK_IP
      | kEnetTxBdIpHdrChecksum
#endif
      ;
  }
  tx_ring[TX_SIZE - 1].status |= kEnetTxBdWrap;

  ENET_EIMR = 0;  // This also deasserts all interrupts

  ENET_RCR = ENET_RCR_NLC |  // Payload length is checked
             ENET_RCR_MAX_FL(MAX_FRAME_LEN) |
             ENET_RCR_CFEN |    // Discard non-pause MAC control frames
             ENET_RCR_CRCFWD |  // CRC is stripped (ignored if PADEN)
             ENET_RCR_PADEN |   // Padding is removed
             ENET_RCR_RMII_MODE | ENET_RCR_FCE |  // Flow control enable
#ifdef DRIVER_TEENSY41_PROMISCUOUS_MODE
             ENET_RCR_PROM |  // Promiscuous mode
#endif                        // DRIVER_TEENSY41_PROMISCUOUS_MODE
             ENET_RCR_MII_MODE;
  ENET_TCR = ENET_TCR_ADDINS |  // Overwrite with programmed MAC address
             ENET_TCR_ADDSEL(0) |
             // ENET_TCR_RFC_PAUSE |
             // ENET_TCR_TFC_PAUSE |
             ENET_TCR_FDEN;  // Enable full-duplex

  ENET_TACC = 0
#ifdef CHECKSUM_HW_GEN_PROTO
              | ENET_TACC_PROCHK  // Insert protocol checksum
#endif
#ifdef CHECKSUM_HW_GEN_IP
              | ENET_TACC_IPCHK  // Insert IP header checksum
#endif
#if ETH_PAD_SIZE == 2
              | ENET_TACC_SHIFT16
#endif
      ;

  ENET_RACC = 0
#if ETH_PAD_SIZE == 2
              | ENET_RACC_SHIFT16
#endif
              | ENET_RACC_LINEDIS  // Discard bad frames
#ifdef CHECKSUM_HW_CHECK_PROTO
              | ENET_RACC_PRODIS  // Discard frames with incorrect protocol
                                  // checksum Requires RSFL == 0
#endif
#ifdef CHECKSUM_HW_CHECK_IP
              | ENET_RACC_IPDIS  // Discard frames with incorrect IPv4 header
                                 // checksum Requires RSFL == 0
#endif
              | ENET_RACC_PADREM;

  ENET_TFWR = 0
#if defined(CHECKSUM_HW_GEN_IP) || defined(CHECKSUM_HW_GEN_PROTO)
              | ENET_TFWR_STRFWD
#endif
    ;

  ENET_RSFL = 0;

  ENET_RDSR = (uint32_t) rx_ring;
  ENET_TDSR = (uint32_t) tx_ring;
  ENET_MRBR = BUF_SIZE;

  ENET_RXIC = 0;
  ENET_TXIC = 0;
  ENET_PALR = mac[0] << 24 | mac[1] << 16 | mac[2] << 8 | mac[3];
  ENET_PAUR = mac[4] << 24 | mac[5] << 16 | 0x8808;

  ENET_OPD = 0x10014;
  ENET_RSEM = 0;
  ENET_MIBC = 0;

  ENET_IAUR = 0;
  ENET_IALR = 0;
  ENET_GAUR = 0;
  ENET_GALR = 0;

  ENET_EIMR = ENET_EIMR_RXF;
  attachInterruptVector(IRQ_ENET, _teensy41_enet_isr);
  NVIC_ENABLE_IRQ(IRQ_ENET);

  // Last, enable the Ethernet MAC
  //ENET_ECR = 0x70000000 | ENET_ECR_DBSWP | ENET_ECR_EN1588 | ENET_ECR_ETHEREN;
  ENET_ECR |= ENET_ECR_DBSWP | ENET_ECR_EN1588 | ENET_ECR_ETHEREN;

  // Indicate there are empty RX buffers and available ready TX buffers
  ENET_RDAR = ENET_RDAR_RDAR;
  ENET_TDAR = ENET_TDAR_TDAR;

  // phy soft reset
  // phy_mdio_write(0, 0x00, 1 << 15);
}

static bool _teensy41_low_level_input(volatile enetbufferdesc_t *bdPtr,
                                      void *buffer, size_t buflen) {
  const uint16_t err_mask = kEnetRxBdTrunc | kEnetRxBdOverrun | kEnetRxBdCrc |
                            kEnetRxBdNonOctet | kEnetRxBdLengthViolation;

  bool ret = false;

  // Determine if a frame has been received
  if (bdPtr->status & err_mask) {
    if (bdPtr->status & kEnetRxBdTrunc) {
      MG_ERROR(("Teensy41 driver error : kEnetRxBdTrunc"));
    } else {  // Either truncated or others
      if (bdPtr->status & kEnetRxBdOverrun) {
        MG_ERROR(("Teensy41 driver error : kEnetRxBdOverrun"));
      } else {  // Either overrun and others zero, or others
        if (bdPtr->status & kEnetRxBdNonOctet) {
          MG_ERROR(("Teensy41 driver error : kEnetRxBdNonOctet"));
        } else if (bdPtr->status & kEnetRxBdCrc) {  // Non-octet or CRC
          MG_ERROR(("Teensy41 driver error : kEnetRxBdCrc"));
        }
        if (bdPtr->status & kEnetRxBdLengthViolation) {
          MG_ERROR(("Teensy41 driver error : kEnetRxBdLengthViolation"));
        }
      }
    }
  } else {
#ifdef ETH_PAD_SIZE
#ifndef DRIVER_TEENSY41_BUFFERS_IN_RAM1
    arm_dcache_delete(bdPtr->buffer, MULTIPLE_OF_32(buflen + ETH_PAD_SIZE));
#endif  // !DRIVER_TEENSY41_BUFFERS_IN_RAM1
    memcpy(buffer, bdPtr->buffer + ETH_PAD_SIZE, buflen);
#else
#ifndef DRIVER_TEENSY41_BUFFERS_IN_RAM1
    arm_dcache_delete(bdPtr->buffer, MULTIPLE_OF_32(buflen ));
#endif  // !DRIVER_TEENSY41_BUFFERS_IN_RAM1
    memcpy(buffer, bdPtr->buffer, buflen);
#endif
    ret = true;
  }

  // Set rx bd empty
  bdPtr->status = (bdPtr->status & kEnetRxBdWrap) | kEnetRxBdEmpty;

  ENET_RDAR = ENET_RDAR_RDAR;

  return ret;
}

// Find the next non-empty BD.
static inline volatile enetbufferdesc_t *_teensy41_rxbd_next() {
  volatile enetbufferdesc_t *p_bd = p_rxbd;

  while (p_bd->status & kEnetRxBdEmpty) {
    if (p_bd->status & kEnetRxBdWrap) {
      p_bd = &rx_ring[0];
    } else {
      p_bd++;
    }
    if (p_bd == p_rxbd) {
      return NULL;
    }
  }

  if (p_rxbd->status & kEnetRxBdWrap) {
    p_rxbd = &rx_ring[0];
  } else {
    p_rxbd++;
  }
  return p_bd;
}

// Acquire a buffer descriptor. Meant to be used with
// _teensy41_update_bufdesc().
static inline volatile enetbufferdesc_t *_teensy41_get_bufdesc() {
  volatile enetbufferdesc_t *bdPtr = p_txbd;

  while (bdPtr->status & kEnetTxBdReady) {
    // Wait until BD is free
  }

  return bdPtr;
}

// Update a buffer descriptor. Meant to be used with get_bufdesc().
static inline void _teensy41_update_bufdesc(volatile enetbufferdesc_t *bdPtr,
                                            uint16_t len) {
  bdPtr->length = len;
  bdPtr->status = (bdPtr->status & kEnetTxBdWrap) | kEnetTxBdTransmitCrc |
                  kEnetTxBdLast | kEnetTxBdReady;

  ENET_TDAR = ENET_TDAR_TDAR;

  if (bdPtr->status & kEnetTxBdWrap) {
    p_txbd = &tx_ring[0];
  } else {
    p_txbd++;
  }

  //LINK_STATS_INC(link.xmit);
}

static size_t teensy41_rx(void *buf, size_t buflen, struct mip_if *ifp) {
  if (!rx_ready) return 0;

  volatile enetbufferdesc_t *p_bd = _teensy41_rxbd_next();
  if (!p_bd) {
    rx_ready = 0;
    return 0;
  }

  if (!_teensy41_low_level_input(p_bd, buf, buflen)) {
    rx_ready = 0;
    return 0;
  }

  //MG_INFO(("recv %d", buflen));

  return buflen;
}

static size_t teensy41_tx(const void *buf, size_t buflen, struct mip_if *ifp) {
  //if (buf == NULL || buflen < 60 || MAX_FRAME_LEN - 4 < buflen)
  //  return 0;

  volatile enetbufferdesc_t *bdPtr = _teensy41_get_bufdesc();
#if ETH_PAD_SIZE
  memcpy(bdPtr->buffer + ETH_PAD_SIZE, buf, buflen);
#ifndef DRIVER_TEENSY41_BUFFERS_IN_RAM1
  arm_dcache_flush_delete(bdPtr->buffer, MULTIPLE_OF_32(buflen + ETH_PAD_SIZE));
#endif  // !DRIVER_TEENSY41_BUFFERS_IN_RAM1
  _teensy41_update_bufdesc(bdPtr, buflen + ETH_PAD_SIZE);
#else
  memcpy(bdPtr->buffer, buf, buflen);
#ifndef DRIVER_TEENSY41_BUFFERS_IN_RAM1
  arm_dcache_flush_delete(bdPtr->buffer, MULTIPLE_OF_32(buflen));
#endif  // !DRIVER_TEENSY41_BUFFERS_IN_RAM1
  _teensy41_update_bufdesc(bdPtr, buflen);
#endif  // ETH_PAD_SIZE

  //MG_INFO(("send %d", buflen));

  return buflen;
}

static bool teensy41_init(struct mip_if *ifp) {
  _teensy41_getmac(ifp->mac);

  /*
   MG_INFO(("teensy41_init() = mac : %02x:%02x:%02x:%02x:%02x:%02x\n",
          ifp->mac[0], ifp->mac[1], ifp->mac[2],
          ifp->mac[3], ifp->mac[4], ifp->mac[5]));
   */

  _teensy41_low_level_init(ifp->mac);

  return true;
}

static bool teensy41_up(struct mip_if *ifp) {
  uint16_t status = _teensy41_mdio_read(0, 0x01);
  uint8_t is_link_up = !!(status & (1 << 2));
  return is_link_up;
}

struct mip_driver mip_driver_teensy41 = {teensy41_init, teensy41_tx,
                                         teensy41_rx, teensy41_up};
#endif
