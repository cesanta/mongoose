#include "tcpip.h"

/*
 * Todo
 * This driver doesn't support 10M line autoconfiguration yet.
 * Packets aren't sent if the link negociated 10M line.
 * todo: MAC back auto reconfiguration.
 */

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_IMXRT1020)
struct imx_rt1020_enet {
volatile uint32_t RESERVED0, EIR, EIMR, RESERVED1, RDAR, TDAR, RESERVED2[3], ECR, RESERVED3[6], MMFR, MSCR, RESERVED4[7], MIBC, RESERVED5[7], RCR, RESERVED6[15], TCR, RESERVED7[7], PALR, PAUR, OPD, TXIC0, TXIC1, TXIC2, RESERVED8, RXIC0, RXIC1, RXIC2, RESERVED9[3], IAUR, IALR, GAUR, GALR, RESERVED10[7], TFWR, RESERVED11[14], RDSR, TDSR, MRBR[2], RSFL, RSEM, RAEM, RAFL, TSEM, TAEM, TAFL, TIPG, FTRL, RESERVED12[3], TACC, RACC, RESERVED13[15], RMON_T_PACKETS, RMON_T_BC_PKT, RMON_T_MC_PKT, RMON_T_CRC_ALIGN, RMON_T_UNDERSIZE, RMON_T_OVERSIZE, RMON_T_FRAG, RMON_T_JAB, RMON_T_COL, RMON_T_P64, RMON_T_P65TO127, RMON_T_P128TO255, RMON_T_P256TO511, RMON_T_P512TO1023, RMON_T_P1024TO2048, RMON_T_GTE2048, RMON_T_OCTETS, IEEE_T_DROP, IEEE_T_FRAME_OK, IEEE_T_1COL, IEEE_T_MCOL, IEEE_T_DEF, IEEE_T_LCOL, IEEE_T_EXCOL, IEEE_T_MACERR, IEEE_T_CSERR, IEEE_T_SQE, IEEE_T_FDXFC, IEEE_T_OCTETS_OK, RESERVED14[3], RMON_R_PACKETS, RMON_R_BC_PKT, RMON_R_MC_PKT, RMON_R_CRC_ALIGN, RMON_R_UNDERSIZE, RMON_R_OVERSIZE, RMON_R_FRAG, RMON_R_JAB, RESERVED15, RMON_R_P64, RMON_R_P65TO127, RMON_R_P128TO255, RMON_R_P256TO511, RMON_R_P512TO1023, RMON_R_P1024TO2047, RMON_R_GTE2048, RMON_R_OCTETS, IEEE_R_DROP, IEEE_R_FRAME_OK, IEEE_R_CRC, IEEE_R_ALIGN, IEEE_R_MACERR, IEEE_R_FDXFC, IEEE_R_OCTETS_OK, RESERVED16[71], ATCR, ATVR, ATOFF, ATPER, ATCOR, ATINC, ATSTMP, RESERVED17[122], TGSR, TCSR0, TCCR0, TCSR1, TCCR1, TCSR2, TCCR2, TCSR3;
};

#undef ENET
#define ENET ((struct imx_rt1020_enet *) (uintptr_t) 0x402D8000u)

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))

#define ENET_RXBUFF_SIZE 1536 // 1522 Buffer must be 64bits aligned
#define ENET_TXBUFF_SIZE 1536 // 1522 hence set to 0x600 (1536)
#define ENET_RXBD_NUM          (4)
#define ENET_TXBD_NUM          (4)

const uint32_t EIMR_RX_ERR = 0x2400000;              // Intr mask RXF+EBERR

void ETH_IRQHandler(void);
static bool mg_tcpip_driver_imxrt1020_init(struct mg_tcpip_if *ifp);
static void wait_phy_complete(void);
static struct mg_tcpip_if *s_ifp;                         // MIP interface

static size_t mg_tcpip_driver_imxrt1020_tx(const void *, size_t , struct mg_tcpip_if *);
static bool mg_tcpip_driver_imxrt1020_up(struct mg_tcpip_if *ifp);

enum { IMXRT1020_PHY_ADDR = 0x02, IMXRT1020_PHY_BCR = 0, IMXRT1020_PHY_BSR = 1 };     // PHY constants

void delay(uint32_t);
void delay (uint32_t di) {
  volatile int dno = 0; // Prevent optimization
  for (uint32_t i = 0; i < di; i++)
    for (int j=0; j<20; j++) // PLLx20 (500 MHz/24MHz)
      dno++;
}

static void wait_phy_complete(void) {
  delay(0x00010000);
  const uint32_t delay_max = 0x00100000;
  uint32_t delay_cnt = 0;
  while (!(ENET->EIR & BIT(23)) && (delay_cnt < delay_max))
  {delay_cnt++;}
  ENET->EIR |= BIT(23); // MII interrupt clear
}

static uint32_t imxrt1020_eth_read_phy(uint8_t addr, uint8_t reg) {
  ENET->EIR |= BIT(23); // MII interrupt clear
  uint32_t mask_phy_adr_reg = 0x1f; // 0b00011111: Ensure we write 5 bits (Phy address & register)
  uint32_t phy_transaction = 0x00;
  phy_transaction = (0x1 << 30) \
                  | (0x2 << 28) \
                  | ((uint32_t)(addr & mask_phy_adr_reg) << 23) \
                  | ((uint32_t)(reg & mask_phy_adr_reg)  << 18) \
                  | (0x2 << 16);

  ENET->MMFR = phy_transaction;
  wait_phy_complete();

  return (ENET->MMFR & 0x0000ffff);
}

static void imxrt1020_eth_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  ENET->EIR |= BIT(23); // MII interrupt clear
  uint8_t mask_phy_adr_reg = 0x1f; // 0b00011111: Ensure we write 5 bits (Phy address & register)
  uint32_t mask_phy_data = 0x0000ffff; // Ensure we write 16 bits (data)
  addr &= mask_phy_adr_reg;
  reg &= mask_phy_adr_reg;
  val &= mask_phy_data;
  uint32_t phy_transaction = 0x00;
  phy_transaction = (uint32_t)(0x1 << 30) \
                  | (uint32_t)(0x1 << 28) \
                  | (uint32_t)(addr << 23) \
                  | (uint32_t)(reg  << 18) \
                  | (uint32_t)(0x2 << 16) \
                  | (uint32_t)(val);
  ENET->MMFR = phy_transaction;
  wait_phy_complete();
}

// FEC RX/TX descriptors (Enhanced descriptor not enabled)
// Descriptor buffer structure, little endian

typedef struct enet_bd_struct_def
{
    uint16_t length;  // Data length
    uint16_t control; // Control and status
    uint32_t *buffer; // Data ptr
} enet_bd_struct_t;

// Descriptor and buffer globals, in non-cached area, 64 bits aligned.

__attribute__((section("NonCacheable,\"aw\",%nobits @"))) enet_bd_struct_t rx_buffer_descriptor[(ENET_RXBD_NUM)] __attribute__((aligned((64U))));
__attribute__((section("NonCacheable,\"aw\",%nobits @"))) enet_bd_struct_t tx_buffer_descriptor[(ENET_TXBD_NUM)] __attribute__((aligned((64U))));

uint8_t rx_data_buffer[(ENET_RXBD_NUM)][((unsigned int)(((ENET_RXBUFF_SIZE)) + (((64U))-1U)) & (unsigned int)(~(unsigned int)(((64U))-1U)))] __attribute__((aligned((64U))));
uint8_t tx_data_buffer[(ENET_TXBD_NUM)][((unsigned int)(((ENET_TXBUFF_SIZE)) + (((64U))-1U)) & (unsigned int)(~(unsigned int)(((64U))-1U)))] __attribute__((aligned((64U))));

// Initialise driver imx_rt1020

// static bool mg_tcpip_driver_imxrt1020_init(uint8_t *mac, void *data) { // VO
static bool mg_tcpip_driver_imxrt1020_init(struct mg_tcpip_if *ifp) {

  struct mg_tcpip_driver_imxrt1020_data *d = (struct mg_tcpip_driver_imxrt1020_data *) ifp->driver_data;
  s_ifp = ifp;

  // ENET Reset, wait complete
  ENET->ECR |= BIT(0);
  while((ENET->ECR & BIT(0)) != 0) {}

  // Re-latches the pin strapping pin values
  ENET->ECR |= BIT(0);
  while((ENET->ECR & BIT(0)) != 0) {}

  // Setup MII/RMII MDC clock divider (<= 2.5MHz).
  ENET->MSCR = 0x130; // HOLDTIME 2 clk, Preamble enable, MDC MII_Speed Div 0x30
  imxrt1020_eth_write_phy(IMXRT1020_PHY_ADDR, IMXRT1020_PHY_BCR, 0x8000); // PHY W @0x00 D=0x8000 Soft reset
  while (imxrt1020_eth_read_phy(IMXRT1020_PHY_ADDR, IMXRT1020_PHY_BSR) & BIT(15)) {delay(0x5000);} // Wait finished poll 10ms

  // PHY: Start Link
  {
    imxrt1020_eth_write_phy(IMXRT1020_PHY_ADDR, IMXRT1020_PHY_BCR, 0x1200); // PHY W @0x00 D=0x1200 Autonego enable + start
    imxrt1020_eth_write_phy(IMXRT1020_PHY_ADDR, 0x1f, 0x8180);    // PHY W @0x1f D=0x8180 Ref clock 50 MHz at XI input

    uint32_t bcr = imxrt1020_eth_read_phy(IMXRT1020_PHY_ADDR, IMXRT1020_PHY_BCR);
    bcr &= ~BIT(10); // Isolation -> Normal
    imxrt1020_eth_write_phy(IMXRT1020_PHY_ADDR, IMXRT1020_PHY_BCR, bcr);
  }

  // Disable ENET
  ENET->ECR = 0x0; //  Disable before configuration

  // Configure ENET
  ENET->RCR = 0x05ee0104; // #CRCFWD=0 (CRC kept in frame) + RMII + MII Enable
  
  ENET->TCR = BIT(8) | BIT(2); // Addins (MAC address from PAUR+PALR) + Full duplex enable
  //ENET->TFWR = BIT(8); // Store And Forward Enable, 64 bytes (minimize tx latency)

  // Configure descriptors and buffers
  // RX
  for (int i = 0; i < ENET_RXBD_NUM; i++) {
    // Wrap last descriptor buffer ptr
    rx_buffer_descriptor[i].control = (BIT(15) | ((i<(ENET_RXBD_NUM-1))?0:BIT(13))); // E+(W*)
    rx_buffer_descriptor[i].buffer = (uint32_t *)rx_data_buffer[i];
  }

  // TX
  for (int i = 0; i < ENET_TXBD_NUM; i++) {
    // Wrap last descriptor buffer ptr
    tx_buffer_descriptor[i].control = ((i<(ENET_RXBD_NUM-1))?0:BIT(13)) | BIT(10); // (W*)+TC
    tx_buffer_descriptor[i].buffer = (uint32_t *)tx_data_buffer[i];
  }

  // Continue ENET configuration
  ENET->RDSR = (uint32_t)(uintptr_t)rx_buffer_descriptor;
  ENET->TDSR = (uint32_t)(uintptr_t)tx_buffer_descriptor;
  ENET->MRBR[0] = ENET_RXBUFF_SIZE; // Same size for RX/TX buffers

  // MAC address filtering (bytes in reversed order)
  ENET->PAUR = ((uint32_t) ifp->mac[4] << 24U) | (uint32_t) ifp->mac[5] << 16U;
  ENET->PALR = (uint32_t) (ifp->mac[0] << 24U) | ((uint32_t) ifp->mac[1] << 16U) |
                 ((uint32_t) ifp->mac[2] << 8U) | ifp->mac[3];

  // Init Hash tables (mac filtering)
  ENET->IAUR = 0; // Unicast
  ENET->IALR = 0;
  ENET->GAUR = 0; // Multicast
  ENET->GALR = 0;

  // Set ENET Online
  ENET->ECR |= BIT(8); // ENET Set Little-endian + (FEC buffer desc.)
  ENET->ECR |= BIT(1); // Enable

  // Set interrupt mask
  ENET->EIMR = EIMR_RX_ERR;

  // RX Descriptor activation
  ENET->RDAR = BIT(24); // Activate Receive Descriptor
  return true;
}

// Transmit frame
static uint32_t s_rt1020_txno;

static size_t mg_tcpip_driver_imxrt1020_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {

  if (len > sizeof(tx_data_buffer[ENET_TXBD_NUM])) {
  //  MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((tx_buffer_descriptor[s_rt1020_txno].control & BIT(15))) {
  MG_ERROR(("No free descriptors"));
    // printf("D0 %lx SR %lx\n", (long) s_txdesc[0][0], (long) ETH->DMASR);
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(tx_data_buffer[s_rt1020_txno], buf, len);     // Copy data
    tx_buffer_descriptor[s_rt1020_txno].length = (uint16_t) len;  // Set data len
    tx_buffer_descriptor[s_rt1020_txno].control |= (uint16_t)(BIT(10)); // TC (transmit CRC)
    //  tx_buffer_descriptor[s_rt1020_txno].control &= (uint16_t)(BIT(14) | BIT(12)); // Own doesn't affect HW
    tx_buffer_descriptor[s_rt1020_txno].control |= (uint16_t)(BIT(15) | BIT(11)); // R+L (ready+last)
    ENET->TDAR = BIT(24); // Descriptor updated. Hand over to DMA.
    // INFO
    // Relevant Descriptor bits: 15(R)  Ready
    //                           11(L)  last in frame
    //                           10(TC) transmis CRC
    // __DSB(); // ARM errata 838869 Cortex-M4, M4F, M7, M7F: "store immediate overlapping
                // exception" return might vector to incorrect interrupt.
    if (++s_rt1020_txno >= ENET_TXBD_NUM) s_rt1020_txno = 0;
  }
  (void) ifp;
  return len;
}

// IRQ (RX)
static uint32_t s_rt1020_rxno;

void ENET_IRQHandler(void) {
  ENET->EIMR = 0;           // Mask interrupts.
  uint32_t eir = ENET->EIR; // Read EIR
  ENET->EIR = 0xffffffff;   // Clear interrupts

  if (eir & EIMR_RX_ERR) // Global mask used
  {
    if (rx_buffer_descriptor[s_rt1020_rxno].control & BIT(15)) {
      ENET->EIMR = EIMR_RX_ERR; // Enable interrupts
      return;  // Empty? -> exit.
    }
    // Read inframes
    else { // Frame received, loop
      for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
        if (rx_buffer_descriptor[s_rt1020_rxno].control & BIT(15)) break;  // exit when done
        // Process if CRC OK and frame not truncated
        if (!(rx_buffer_descriptor[s_rt1020_rxno].control & (BIT(2) | BIT(0)))) {
          uint32_t len = (rx_buffer_descriptor[s_rt1020_rxno].length);
          mg_tcpip_qwrite(rx_buffer_descriptor[s_rt1020_rxno].buffer, len > 4 ? len - 4 : len, s_ifp);
        }
        rx_buffer_descriptor[s_rt1020_rxno].control |= BIT(15); // Inform DMA RX is empty
        if (++s_rt1020_rxno >= ENET_RXBD_NUM) s_rt1020_rxno = 0;
      }
    }
  }
  ENET->EIMR = EIMR_RX_ERR; // Enable interrupts
}

// Up/down status
static bool mg_tcpip_driver_imxrt1020_up(struct mg_tcpip_if *ifp) {
  uint32_t bsr = imxrt1020_eth_read_phy(IMXRT1020_PHY_ADDR, IMXRT1020_PHY_BSR);
  (void) ifp;
  return bsr & BIT(2) ? 1 : 0;
}

// API
struct mg_tcpip_driver mg_tcpip_driver_imxrt1020 = {
  mg_tcpip_driver_imxrt1020_init, mg_tcpip_driver_imxrt1020_tx, NULL,
  mg_tcpip_driver_imxrt1020_up};

#endif
