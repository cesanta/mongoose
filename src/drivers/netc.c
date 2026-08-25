#include "net_builtin.h"

#if defined(MG_ENABLE_TCPIP) && MG_ENABLE_TCPIP && \
    defined(MG_ENABLE_DRIVER_NETC) && MG_ENABLE_DRIVER_NETC

// IMXRT1180 RM - Chapter 53 - Ethernet Controller (NETC)

#define ETH_DESC_CNT 8 // Descriptor count must be multiple of 8 (53.4.6.11.97)
#define ETH_PKT_SIZE 1536

struct netc_pci_function { // 53.4.6.1.1
  volatile uint32_t DID_VID;
  volatile uint16_t CMD, STAT;
  volatile uint8_t reserved0[0x40];
  volatile uint16_t DEV_CTL, DEV_STAT;
  volatile uint8_t reserved1[0x34];
  volatile uint16_t MSIX_CAP_LIST, MSIX_MSG_CTL;
};

struct netc_emdio { // 53.4.6.12
  volatile uint32_t EMDIO_CFG, EMDIO_CTL, EMDIO_DATA, EMDIO_ADDR, EMDIO_STAT;
};

struct netc_enetc { // 53.4.6.6 
  volatile uint32_t ECAPR0, ECAPR1, ECAPR2, reserved0, PMR;
  volatile uint8_t reserved1[0x1ec];
  volatile uint32_t PSIPMMR, PSIPVMR;
  volatile uint8_t reserved2[0x5f8];
  volatile uint32_t SMCAPR;
  volatile uint8_t reserved3[0x17fc];
  struct {
    volatile uint32_t PSIPMAR0, PSIPMAR1, PSIVLANR, reserved0, PSICFGR0,
      PSICFGR1, PSICFGR2;
    volatile uint8_t reserved1[0x3c];
    volatile uint32_t PSIMMHFR0, PSIMMHFR1;
    volatile uint8_t reserved2[0x20];
  } PSI[1];
};

struct netc_port { // 53.4.6.8
  volatile uint32_t PCAPR, PMCAPR, PIOCAPR, reserved0, PCR;
  volatile uint8_t reserved1[0x0c];
  volatile uint32_t PMAR0, PMAR1;
  volatile uint8_t reserved2[0xd8];
  volatile uint32_t POR, PSR;
  volatile uint8_t reserved3[0x30];
  volatile uint32_t PDGSR;
  volatile uint8_t reserved4[0xc4];
  struct {
    volatile uint32_t PTGSTCSR, reserved0, PTCTMSDUR, reserved1, PTCCBSR0,
      PTCCBSR1;
    volatile uint8_t reserved2[8];
  } TCT[8];
  volatile uint8_t reserved5[0x220];
  volatile uint32_t BPSTGSR;
};

struct netc_port_mac { // 53.4.6.9
  volatile uint8_t reserved0[8];
  volatile uint32_t COMMAND_CONFIG, MAC_ADDR_0, MAC_ADDR_1, MAXFRM, MINFRM;
  volatile uint8_t reserved1[0x24];
  volatile uint32_t IEVENT;
  volatile uint8_t reserved2[0x1dc];
  volatile uint64_t TFRMN;
  volatile uint8_t reserved3[0xd8];
  volatile uint32_t IF_MODE;
  volatile uint8_t reserved4[0xfc];
};

struct netc_eth_link {
  struct netc_port_mac PM[2];
};

struct netc_pseudo_link {
  volatile uint32_t PPMSR;
  volatile uint8_t reserved0[0x0c];
  volatile uint32_t PPMCR;
};

struct netc_switch { // 53.4.6.5
  volatile uint8_t reserved0[0x400];
  volatile uint32_t MPCR;
  volatile uint8_t reserved1[0x1c0c];
  volatile uint32_t VFHTDECR0, VFHTDECR1, VFHTDECR2;
};

struct netc_bdr {
  volatile uint32_t TBMR, TBSR;
  volatile uint8_t reserved0[8];
  volatile uint32_t TBBAR0, TBBAR1, TBPIR, TBCIR, TBLENR;
  volatile uint8_t reserved1[0x7c];
  volatile uint32_t TBIER, TBIDR, TBICR0, TBICR1;
  volatile uint8_t reserved2[0x50];
  volatile uint32_t RBMR, RBSR, RBBSR, RBCIR, RBBAR0, RBBAR1, RBPIR,
    reserved3, RBLENR;
  volatile uint8_t reserved4[0x5c];
  volatile uint32_t RBDCR;
  volatile uint8_t reserved5[0x1c];
  volatile uint32_t RBIER, RBIDR, RBICR0, RBICR1;
  volatile uint8_t reserved6[0x50];
};

struct netc_si { // 53.4.6.11
  volatile uint32_t SIMR, SISR;
  volatile uint8_t reserved0[0x30];
  volatile uint32_t SIRBGCR;
  volatile uint8_t reserved1[0x2ec];
  volatile uint32_t SITFRM0;
  volatile uint8_t reserved2[0x6fc];
  volatile uint32_t SIRXIDR0;
  volatile uint8_t reserved3[0x154];
  volatile uint32_t SIMSIRRVR[1];
  volatile uint8_t reserved4[0x747c];
  struct netc_bdr BDR[1];
};

struct netc_msix_entry { // 53.4.2.11
  volatile uint32_t MSG_ADDR, MSG_UPPER_ADDR, MSG_DATA, VECTOR_CTRL;
};

struct netc_msgintr { // 54.4
  struct {
    volatile uint32_t MSIIR, MSIR;
  } MSI[3];
};

#define NETC_EMDIO_PCI \
  ((struct netc_pci_function *) (uintptr_t) 0x60001000U)
#define NETC_SWITCH_PCI \
  ((struct netc_pci_function *) (uintptr_t) 0x60002000U)
#define NETC_ENETC1_PCI \
  ((struct netc_pci_function *) (uintptr_t) 0x60004000U)
#define NETC_EMDIO ((struct netc_emdio *) (uintptr_t) 0x60ba1c00U)

#define NETC_ENETC1 ((struct netc_enetc *) (uintptr_t) 0x60b50000U)
#define NETC_ENETC1_PORT ((struct netc_port *) (uintptr_t) 0x60b54000U)
#define NETC_ENETC1_PSEUDO_LINK \
  ((struct netc_pseudo_link *) (uintptr_t) 0x60b55000U)
#define NETC_ENETC1_SI0 ((struct netc_si *) (uintptr_t) 0x60b40000U)
#define NETC_ENETC1_MSIX \
  ((struct netc_msix_entry *) (uintptr_t) 0x60c00000U)

#define NETC_SWITCH ((struct netc_switch *) (uintptr_t) 0x60a00000U)
#define NETC_SW0_PORT(link)                                   \
  ((struct netc_port *) (uintptr_t) (0x60a04000U +            \
                                     (uint32_t) (link) * 0x4000U))
#define NETC_SW0_ETH(link)                                    \
  ((struct netc_eth_link *) (uintptr_t) (0x60a05000U +        \
                                         (uint32_t) (link) * 0x4000U))
#define NETC_SW0_PORT4 ((struct netc_port *) (uintptr_t) 0x60a14000U)
#define NETC_MSGINTR1 \
  ((struct netc_msgintr *) (uintptr_t) 0x428a0000U)

enum {
  NETC_RX_MSIX_ENTRY = 0,
  NETC_RX_MSG_INDEX = 2,
};

struct netc_tx_bd {
  uint64_t buffer_address;
  uint16_t buffer_length, frame_length;
  uint32_t control;
};

struct netc_rx_descriptor {
  uint64_t buffer_address, reserved;
};

struct netc_rx_writeback {
  uint16_t internet_checksum, parser_summary;
  uint32_t rss_hash;
  uint16_t buffer_length, vlan;
  uint32_t status;
};

union netc_rx_bd {
  struct netc_rx_descriptor descriptor;
  struct netc_rx_writeback writeback;
};

static volatile struct netc_tx_bd s_txdesc[ETH_DESC_CNT] MG_ETH_RAM
    __attribute__((aligned(128)));
static volatile union netc_rx_bd s_rxdesc[ETH_DESC_CNT] MG_ETH_RAM
    __attribute__((aligned(128)));
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_ETH_RAM
    __attribute__((aligned(64)));
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_ETH_RAM
    __attribute__((aligned(64)));

static uint16_t s_tx_prod;
static uint16_t s_tx_clean;
static uint16_t s_rx_cons;
static struct mg_tcpip_if *s_ifp;

static uint16_t enet_read_phy(uint8_t addr, uint8_t reg) {
  NETC_EMDIO->EMDIO_CFG &= ~MG_BIT(6);  // ENC45: use Clause 22
  NETC_EMDIO->EMDIO_CTL = MG_BIT(15) | ((uint32_t) (addr & 0x1fU) << 5) |
                           (reg & 0x1fU);  // READ, PORT_ADDR, DEV_ADDR
  while (NETC_EMDIO->EMDIO_CFG & MG_BIT(31)) (void) 0;  // BSY1
  return (uint16_t) (NETC_EMDIO->EMDIO_DATA & 0xffffU);
}

static void enet_write_phy(uint8_t addr, uint8_t reg, uint16_t value) {
  NETC_EMDIO->EMDIO_CFG &= ~MG_BIT(6);  // ENC45: use Clause 22
  NETC_EMDIO->EMDIO_CTL = ((uint32_t) (addr & 0x1fU) << 5) |
                           (reg & 0x1fU);  // PORT_ADDR, DEV_ADDR
  NETC_EMDIO->EMDIO_DATA = value;
  while (NETC_EMDIO->EMDIO_CFG & MG_BIT(31)) (void) 0;  // BSY1
}

static bool mg_tcpip_driver_netc_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_netc_data *d =
      (struct mg_tcpip_driver_netc_data *) ifp->driver_data;
  struct mg_phy phy = {enet_read_phy, enet_write_phy};
  struct netc_port *switch_port;
  struct netc_eth_link *eth_link;
  uint64_t tx_base = (uint64_t) (uintptr_t) s_txdesc;
  uint64_t rx_base = (uint64_t) (uintptr_t) s_rxdesc;
  uint32_t mac0 = (uint32_t) ifp->mac[0] |
                  ((uint32_t) ifp->mac[1] << 8) |
                  ((uint32_t) ifp->mac[2] << 16) |
                  ((uint32_t) ifp->mac[3] << 24);
  uint32_t mac1 = (uint32_t) ifp->mac[4] |
                  ((uint32_t) ifp->mac[5] << 8);
  uint32_t mdc_cr =
      (uint32_t) (d->mdc_cr < 0 ? MG_DRIVER_MDC_CR : d->mdc_cr);

  switch_port = NETC_SW0_PORT(d->link);
  eth_link = NETC_SW0_ETH(d->link);
  s_ifp = ifp;
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i].buffer_address = (uint64_t) (uintptr_t) s_txbuf[i];
    s_rxdesc[i].descriptor.buffer_address =
        (uint64_t) (uintptr_t) s_rxbuf[i];
  }

  NETC_EMDIO_PCI->DEV_CTL |= MG_BIT(15);  // INIT_FLR
  while (NETC_EMDIO_PCI->DEV_CTL & MG_BIT(15)) (void) 0;
  NETC_EMDIO_PCI->CMD |= MG_BIT(2) | MG_BIT(1);  // BUS_MASTER_EN, MEM_ACCESS
  NETC_EMDIO->EMDIO_CFG =
      ((mdc_cr & 0x1ffU) << 7) | MG_BIT(2);  // MDIO_CLK_DIV, MDIO_HOLD
  mg_phy_init(&phy, d->phy_addr, 0);

  // Initialize ENETC1 PSI0 and port
  NETC_ENETC1_PCI->CMD |= MG_BIT(2) | MG_BIT(1);  // BUS_MASTER_EN, MEM_ACCESS
  NETC_ENETC1_PCI->MSIX_MSG_CTL = MG_BIT(14);  // FUNC_MASK
  NETC_ENETC1_PORT->POR |= MG_BIT(1) | MG_BIT(0);  // RXDIS, TXDIS
  NETC_ENETC1_PCI->DEV_CTL |= MG_BIT(15);  // INIT_FLR
  while (NETC_ENETC1_PCI->DEV_CTL & MG_BIT(15)) (void) 0;
  NETC_ENETC1_PCI->CMD |= MG_BIT(2) | MG_BIT(1);  // BUS_MASTER_EN, MEM_ACCESS
  NETC_ENETC1_PCI->MSIX_MSG_CTL = MG_BIT(14);  // FUNC_MASK
  NETC_ENETC1_PORT->PCR = 99U << 16;  // PSPEED: 1 Gbps
  NETC_ENETC1_PORT->PMAR0 = mac0;
  NETC_ENETC1_PORT->PMAR1 = mac1;
  NETC_ENETC1_PSEUDO_LINK->PPMCR = MG_BIT(16);  // TXPAD
  NETC_ENETC1->PSIPMMR &= ~(MG_BIT(16) | MG_BIT(0));
  NETC_ENETC1_SI0->SIMR &=
      ~(MG_BIT(31) | MG_BIT(3) | MG_BIT(2) | MG_BIT(1));
  NETC_ENETC1_SI0->SIRBGCR = 0;
  NETC_ENETC1->PSI[0].PSICFGR0 =
      MG_BIT(16) | MG_BIT(0);  // one Rx BDR, one Tx BDR

  NETC_ENETC1->PSI[0].PSICFGR2 = 0;  // NUM_MSIX: one entry
  NETC_ENETC1_MSIX[NETC_RX_MSIX_ENTRY].VECTOR_CTRL = MG_BIT(0);  // MASK
  NETC_ENETC1_MSIX[NETC_RX_MSIX_ENTRY].MSG_ADDR =
      (uint32_t) (uintptr_t) &NETC_MSGINTR1->MSI[0].MSIIR;
  NETC_ENETC1_MSIX[NETC_RX_MSIX_ENTRY].MSG_UPPER_ADDR = 0;
  NETC_ENETC1_MSIX[NETC_RX_MSIX_ENTRY].MSG_DATA = NETC_RX_MSG_INDEX;
  NETC_ENETC1_SI0->SIMSIRRVR[0] = NETC_RX_MSIX_ENTRY;  // Rx ring 0 -> entry 0

  // Initialize switch function 2 to drive the selected physical port
  NETC_SWITCH_PCI->CMD |= MG_BIT(2) | MG_BIT(1);
  NETC_SWITCH_PCI->MSIX_MSG_CTL = MG_BIT(14);  // FUNC_MASK
  switch_port->POR |= MG_BIT(1) | MG_BIT(0);
  eth_link->PM[0].COMMAND_CONFIG |= MG_BIT(26);  // SWR
  while (eth_link->PM[0].COMMAND_CONFIG & MG_BIT(26)) (void) 0;
  eth_link->PM[1].COMMAND_CONFIG |= MG_BIT(26);  // SWR
  while (eth_link->PM[1].COMMAND_CONFIG & MG_BIT(26)) (void) 0;
  NETC_SWITCH_PCI->DEV_CTL |= MG_BIT(15);  // INIT_FLR
  while (NETC_SWITCH_PCI->DEV_CTL & MG_BIT(15)) (void) 0;
  NETC_SWITCH_PCI->CMD |= MG_BIT(2) | MG_BIT(1);
  NETC_SWITCH_PCI->MSIX_MSG_CTL = MG_BIT(14);  // FUNC_MASK

  // Configure switch physical port and ethernet link (ETH_X)
  switch_port->PCR = 99U << 16;  // PSPEED: 1 Gbps
  switch_port->PMAR0 = mac0;
  switch_port->PMAR1 = mac1;
  switch_port->TCT[0].PTCTMSDUR = MG_BIT(24) | MG_BIT(16) | ETH_PKT_SIZE;
  eth_link->PM[0].MAXFRM = ETH_PKT_SIZE;
  eth_link->PM[0].MINFRM = 64;
  eth_link->PM[0].IF_MODE = MG_BIT(14) | 4U;  // 1G RGMII
  eth_link->PM[0].COMMAND_CONFIG =
      MG_BIT(15) | MG_BIT(1) | MG_BIT(0);  // TXP, RX_EN, TX_EN
  eth_link->PM[1].MAXFRM = ETH_PKT_SIZE;
  eth_link->PM[1].MINFRM = 64;
  eth_link->PM[1].IF_MODE = MG_BIT(14) | 4U;
  eth_link->PM[1].COMMAND_CONFIG =
      MG_BIT(15) | MG_BIT(1) | MG_BIT(0);

  // Bridge the selected physical port with management port 4 (54.4.1.1)
  NETC_SWITCH->VFHTDECR0 = MG_BIT(4) | MG_BIT(d->link);
  NETC_SWITCH->VFHTDECR2 = (1UL << 27) | (1UL << 24);  // MFO=1, MLO=1
  switch_port->BPSTGSR = 2U;  // STG0: forwarding
  NETC_SW0_PORT4->BPSTGSR = 2U;  // STG0: forwarding
  switch_port->POR &= ~(MG_BIT(1) | MG_BIT(0));
  NETC_SW0_PORT4->POR &= ~(MG_BIT(1) | MG_BIT(0));

  // Initialize Tx BD ring 0 (direct switch enqueue)
  NETC_ENETC1_SI0->BDR[0].TBBAR0 = (uint32_t) tx_base;
  NETC_ENETC1_SI0->BDR[0].TBBAR1 = (uint32_t) (tx_base >> 32);
  NETC_ENETC1_SI0->BDR[0].TBLENR = ETH_DESC_CNT;

  // Initialize Rx BD ring 0
  NETC_ENETC1_SI0->BDR[0].RBICR0 =
      MG_BIT(31) | MG_BIT(0);  // ICEN, interrupt after one frame
  NETC_ENETC1_SI0->BDR[0].RBBAR0 = (uint32_t) rx_base;
  NETC_ENETC1_SI0->BDR[0].RBBAR1 = (uint32_t) (rx_base >> 32);
  NETC_ENETC1_SI0->BDR[0].RBBSR = ETH_PKT_SIZE;
  NETC_ENETC1_SI0->BDR[0].RBLENR = ETH_DESC_CNT;

  // Clear stale device and MSGINTR state before the ring becomes active
  NETC_ENETC1_SI0->SIRXIDR0 = MG_BIT(0);  // RX0, W1C
  (void) NETC_MSGINTR1->MSI[0].MSIR;  // clear pending messages
  NETC_ENETC1_SI0->BDR[0].RBIER = MG_BIT(0);  // RXTIE
  NETC_ENETC1_SI0->BDR[0].TBMR = MG_BIT(31);
  NETC_ENETC1_SI0->BDR[0].RBMR = MG_BIT(31);  // EN, CRC=0: strip FCS
  NETC_ENETC1_PORT->POR &= ~(MG_BIT(1) | MG_BIT(0));
  NETC_ENETC1->PMR |= MG_BIT(16); // SI0EN
  NETC_ENETC1_SI0->SIMR |= MG_BIT(31);  // EN
  MG_DSB();
  if ((NETC_ENETC1->SMCAPR & MG_BIT(0)) == 0U ||
      (NETC_SWITCH->MPCR & 0x1fU) != 4U) {
    MG_ERROR(("NETC fixed management path unavailable: SMCAPR=%08lx MPCR=%08lx",
              (unsigned long) NETC_ENETC1->SMCAPR,
              (unsigned long) NETC_SWITCH->MPCR));
    return false;
  }

  NETC_ENETC1_PCI->MSIX_MSG_CTL =
    MG_BIT(15);  // Enable RX interrupt, MSIX_EN=1, FUNC_MASK=0
  MG_DSB();
  NETC_ENETC1_MSIX[NETC_RX_MSIX_ENTRY].VECTOR_CTRL = 0;  // MASK=0, last
  MG_DSB();
  return true;
}

static size_t mg_tcpip_driver_netc_tx(const void *buf, size_t len,
                                      struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_netc_data *d =
      (struct mg_tcpip_driver_netc_data *) ifp->driver_data;
  uint16_t next;
  s_tx_clean =
      (uint16_t) (NETC_ENETC1_SI0->BDR[0].TBCIR & 0xffffU);  // BDR_INDEX
  next = (uint16_t) (s_tx_prod + 1U);
  if (next >= ETH_DESC_CNT) next = 0;
  if (len > sizeof(s_txbuf[0])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = (size_t) -1;  // fail
  } else if (next == s_tx_clean) {
    ifp->nerr++;
    MG_ERROR(("No descriptors available"));
    len = 0;  // retry later
  } else {
    memcpy(s_txbuf[s_tx_prod], buf, len);  // Copy data
    s_txdesc[s_tx_prod].buffer_address =
        (uint64_t) (uintptr_t) s_txbuf[s_tx_prod];
    s_txdesc[s_tx_prod].buffer_length = (uint16_t) len;  // BUFF_LEN
    s_txdesc[s_tx_prod].frame_length = (uint16_t) len;   // FRAME_LEN
    s_txdesc[s_tx_prod].control =
        MG_BIT(31) | (2UL << 24) | MG_BIT(23) | ((uint32_t) d->link << 16);
    // F=1, FLQ=2, SMSO=1, EGR_PORT=link, IPV=0, DR=0
    MG_DSB();
    if (++s_tx_prod >= ETH_DESC_CNT) s_tx_prod = 0;
    NETC_ENETC1_SI0->BDR[0].TBPIR = s_tx_prod;  // BDR_INDEX
  }
  return len;
}

static void mg_tcpip_driver_netc_update_hash_table(struct mg_tcpip_if *ifp) {
  // mcast address has NETC XOR hash index 56 (RM 53.4.2.3.1.3.8.4.1)
  NETC_ENETC1->PSI[0].PSIMMHFR0 = 0;
  NETC_ENETC1->PSI[0].PSIMMHFR1 = MG_BIT(24);
  (void) ifp;
}

static bool mg_tcpip_driver_netc_poll(struct mg_tcpip_if *ifp, bool s1) {
  if (ifp->update_mac_hash_table) {
    mg_tcpip_driver_netc_update_hash_table(ifp);
    ifp->update_mac_hash_table = false;
  }
  if (!s1) return false;
  struct mg_tcpip_driver_netc_data *d =
      (struct mg_tcpip_driver_netc_data *) ifp->driver_data;
  struct netc_port *switch_port = NETC_SW0_PORT(d->link);
  struct netc_eth_link *eth_link = NETC_SW0_ETH(d->link);
  uint8_t speed = MG_PHY_SPEED_10M;
  bool up, full_duplex = false;
  struct mg_phy phy = {enet_read_phy, enet_write_phy};

  up = mg_phy_up(&phy, d->phy_addr, &full_duplex, &speed);
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {
    uint16_t port_speed = speed == MG_PHY_SPEED_1000M ? 99U
                          : speed == MG_PHY_SPEED_100M ? 9U
                                                       : 0U;
    uint32_t config = switch_port->PCR;
    uint32_t mode = eth_link->PM[0].IF_MODE;
    config &= ~(0x3fffU << 16);  // PCR[29:16] PSPEED
    config |= (uint32_t) port_speed << 16;
    mode &= ~(MG_BIT(14) | MG_BIT(13) | MG_BIT(6));  // SSP, HD
    if (speed == MG_PHY_SPEED_1000M) {
      mode |= MG_BIT(14);  // SSP: 1 Gbps, full duplex
    } else {
      if (speed == MG_PHY_SPEED_10M) mode |= MG_BIT(13);  // SSP
      if (!full_duplex) mode |= MG_BIT(6);                // HD
    }
    switch_port->PCR = config;
    eth_link->PM[0].IF_MODE = mode;
    eth_link->PM[1].IF_MODE = mode;

    // TX_CSD, RX_CSD, and SPD_DUP are latched while the PHY is reset/down.
    eth_link->PM[0].IEVENT = MG_BIT(23) | MG_BIT(22) | MG_BIT(21);
    MG_DSB();
    MG_DEBUG(("Link is %uM %s-duplex",
              speed == MG_PHY_SPEED_1000M ? 1000U
              : speed == MG_PHY_SPEED_100M ? 100U
                                           : 10U,
              full_duplex ? "full" : "half"));
  }
  return up;
}

void MSGINTR1_IRQHandler(void);
void MSGINTR1_IRQHandler(void) {
  if (NETC_MSGINTR1->MSI[0].MSIR & MG_BIT(NETC_RX_MSG_INDEX)) {
    NETC_ENETC1_SI0->SIRXIDR0 = MG_BIT(0);  // RX0, W1C
    MG_DSB();
    for (int i = 0; i < 10; i++) {  // Read as they arrive but not forever
      uint32_t status;
      size_t len;
      if (s_rx_cons ==
          (uint16_t) (NETC_ENETC1_SI0->BDR[0].RBPIR & 0xffffU))
        break;
      status = s_rxdesc[s_rx_cons].writeback.status;
      if (!(status & MG_BIT(30))) break;  // R: writeback is ready
      len = s_rxdesc[s_rx_cons].writeback.buffer_length;
      if ((status & (MG_BIT(31) | MG_BIT(30))) ==
              (MG_BIT(31) | MG_BIT(30)) &&  // F, R
          !(status & (0xffU << 16)) &&       // ERROR
          !(status & (0xfU << 2)) &&         // HR=0: regular frame
          len > 0 && len <= sizeof(s_rxbuf[0])) {
        mg_tcpip_qwrite(s_rxbuf[s_rx_cons], len, s_ifp);  // FCS stripped
      }
      s_rxdesc[s_rx_cons].descriptor.buffer_address =
          (uint64_t) (uintptr_t) s_rxbuf[s_rx_cons];
      s_rxdesc[s_rx_cons].descriptor.reserved = 0;
      MG_DSB();
      if (++s_rx_cons >= ETH_DESC_CNT) s_rx_cons = 0;
      NETC_ENETC1_SI0->BDR[0].RBCIR = s_rx_cons;
    }
  }
  MG_DSB();
}

struct mg_tcpip_driver mg_tcpip_driver_netc = {mg_tcpip_driver_netc_init,
                                               mg_tcpip_driver_netc_tx, NULL,
                                               mg_tcpip_driver_netc_poll};

#endif
