// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/dma.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"

#include "driver_rp2040_rmii.h"
#include "mongoose.h"

#define RXSM 0
#define TXSM 1
#define SMITXSM 2
#define SMIRXSM 0  // pio1

static uint smi_wr_addr, smi_rd_addr;
static int dma_rx;
static int dma_tx;

static dma_channel_config dmacfg_rx;
static dma_channel_config dmacfg_tx;

// Max frame size with VLAN tag + CRC and excluding preamble
#define ETH_PKT_SIZE 1522

static uint8_t s_rxbuf[2][ETH_PKT_SIZE];  // ping-pong buffer
static uint8_t s_txbuf[ETH_PKT_SIZE];
static struct mg_tcpip_if *s_ifp;  // MIP interface

#define rmii_tx_wrap_target 0
#define rmii_tx_wrap 7

static const uint16_t rmii_tx_program_instructions[] = {
    //     .wrap_target
    0xe000,  //  0: set    pins, 0         side 0
    0x80a0,  //  1: pull   block           side 0
    0xe03d,  //  2: set    x, 29           side 0
    0xf101,  //  3: set    pins, 1         side 1 [1]
    0x1144,  //  4: jmp    x--, 4          side 1 [1]
    0xf103,  //  5: set    pins, 3         side 1 [1]
    0x7002,  //  6: out    pins, 2         side 1
    0x10e6,  //  7: jmp    !osre, 6        side 1
             //     .wrap
};

static const struct pio_program rmii_tx_program = {
    .instructions = rmii_tx_program_instructions,
    .length = 8,
    .origin = -1,
};

static inline pio_sm_config rmii_tx_program_get_default_config(uint addr) {
  pio_sm_config c = pio_get_default_sm_config();
  sm_config_set_wrap(&c, addr + rmii_tx_wrap_target, addr + rmii_tx_wrap);
  sm_config_set_sideset(&c, 1, false, false);
  return c;
}

static inline void rmii_tx_init(PIO pio, uint sm, uint addr, uint gpio) {
  pio_sm_config c = rmii_tx_program_get_default_config(addr);
  pio_gpio_init(pio, gpio);
  pio_gpio_init(pio, gpio + 1);
  pio_gpio_init(pio, gpio + 2);
  sm_config_set_out_pins(&c, gpio, 2);
  sm_config_set_set_pins(&c, gpio, 2);
  sm_config_set_sideset_pins(&c, gpio + 2);
  pio_sm_set_consecutive_pindirs(pio, sm, gpio, 3, true);
  sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
  sm_config_set_out_shift(&c, true, true, 8);  // pull bytes, LSB first, auto
  sm_config_set_clkdiv(&c,
                       5);  // Run at 50/5 = 10MHz (2x 2-bit nibble data rate)
  pio_sm_init(pio, sm, addr, &c);
  pio_sm_set_enabled(pio, sm, true);
}

#define rmii_rx_wrap_target 0
#define rmii_rx_wrap 10

static const uint16_t rmii_rx_program_instructions[] = {
    //     .wrap_target
    0x2122,  //  0: wait   0 pin, 2               [1]
    0x21a2,  //  1: wait   1 pin, 2               [1]
    0x21a0,  //  2: wait   1 pin, 0               [1]
    0x2121,  //  3: wait   0 pin, 1               [1]
    0x21a1,  //  4: wait   1 pin, 1               [1]
    0x4002,  //  5: in     pins, 2
    0x00c5,  //  6: jmp    pin, 5
    0x4002,  //  7: in     pins, 2
    0x00c5,  //  8: jmp    pin, 5
    0xa0c3,  //  9: mov    isr, null
    0xc020,  // 10: irq    wait 0
             //     .wrap
};

static const struct pio_program rmii_rx_program = {
    .instructions = rmii_rx_program_instructions,
    .length = 11,
    .origin = -1,
};

static inline pio_sm_config rmii_rx_program_get_default_config(uint addr) {
  pio_sm_config c = pio_get_default_sm_config();
  sm_config_set_wrap(&c, addr + rmii_rx_wrap_target, addr + rmii_rx_wrap);
  return c;
}

static inline void rmii_rx_init(PIO pio, uint sm, uint addr, uint gpio) {
  pio_sm_config c = rmii_rx_program_get_default_config(addr);
  pio_gpio_init(pio, gpio);
  pio_gpio_init(pio, gpio + 1);
  pio_gpio_init(pio, gpio + 2);
  sm_config_set_in_pins(&c, gpio);
  pio_sm_set_consecutive_pindirs(pio, sm, gpio, 3, false);
  sm_config_set_jmp_pin(&c, gpio + 2);
  sm_config_set_in_shift(&c, true, true, 8);  // push bytes, LSB first, auto
  sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
  sm_config_set_clkdiv(&c,
                       5);  // Run at 50/5 = 10MHz (2x 2-bit nibble data rate)
  pio_sm_init(pio, sm, addr, &c);
  pio_sm_set_enabled(pio, sm, true);
}

#define smi_wr_wrap_target 0
#define smi_wr_wrap 4

static const uint16_t smi_wr_program_instructions[] = {
    //     .wrap_target
    0xe03f,  //  0: set    x, 31           side 0
    0xe101,  //  1: set    pins, 1         side 0 [1]
    0x1141,  //  2: jmp    x--, 1          side 1 [1]
    0x6101,  //  3: out    pins, 1         side 0 [1]
    0x1103,  //  4: jmp    3               side 1 [1]
             //     .wrap
};

static const struct pio_program smi_wr_program = {
    .instructions = smi_wr_program_instructions,
    .length = 5,
    .origin = -1,
};

static inline pio_sm_config smi_wr_program_get_default_config(uint addr) {
  pio_sm_config c = pio_get_default_sm_config();
  sm_config_set_wrap(&c, addr + smi_wr_wrap_target, addr + smi_wr_wrap);
  sm_config_set_sideset(&c, 1, false, false);
  return c;
}

#define smi_rd_wrap_target 13
#define smi_rd_wrap 13

static const uint16_t smi_rd_program_instructions[] = {
    0xe03f,  //  0: set    x, 31           side 0
    0xe101,  //  1: set    pins, 1         side 0 [1]
    0x1141,  //  2: jmp    x--, 1          side 1 [1]
    0xf02d,  //  3: set    x, 13           side 1
    0x6101,  //  4: out    pins, 1         side 0 [1]
    0x1144,  //  5: jmp    x--, 4          side 1 [1]
    0xe180,  //  6: set    pindirs, 0      side 0 [1]
    0xf101,  //  7: set    pins, 1         side 1 [1]
    0xa142,  //  8: nop                    side 0 [1]
    0xf12f,  //  9: set    x, 15           side 1 [1]
    0xa042,  // 10: nop                    side 0
    0x4001,  // 11: in     pins, 1         side 0
    0x114a,  // 12: jmp    x--, 10         side 1 [1]
             //     .wrap_target
    0xf081,  // 13: set    pindirs, 1      side 1
             //     .wrap
};

static const struct pio_program smi_rd_program = {
    .instructions = smi_rd_program_instructions,
    .length = 14,
    .origin = -1,
};

static inline pio_sm_config smi_rd_program_get_default_config(uint addr) {
  pio_sm_config c = pio_get_default_sm_config();
  sm_config_set_wrap(&c, addr + smi_rd_wrap_target, addr + smi_rd_wrap);
  sm_config_set_sideset(&c, 1, false, false);
  return c;
}

static inline void smi_wr_init(PIO pio, uint sm, uint addr, uint gpio) {
  pio_gpio_init(pio, gpio);
  pio_gpio_init(pio, gpio + 1);
  pio_sm_config c = smi_wr_program_get_default_config(addr);
  sm_config_set_out_pins(&c, gpio, 1);
  sm_config_set_set_pins(&c, gpio, 1);
  sm_config_set_sideset_pins(&c, gpio + 1);
  pio_sm_set_consecutive_pindirs(pio, sm, gpio, 2, true);
  sm_config_set_out_shift(&c, false, true,
                          16);  // pull half-words, MSB first, auto
  sm_config_set_clkdiv(&c, 6);  // Run at 50/6 = <10MHz (4x data rate)
  pio_sm_init(pio, sm, addr, &c);
}

static inline void smi_rd_init(PIO pio, uint sm, uint addr, uint gpio) {
  pio_gpio_init(pio, gpio);
  pio_gpio_init(pio, gpio + 1);
  pio_sm_config c = smi_rd_program_get_default_config(addr);
  sm_config_set_in_pins(&c, gpio);
  sm_config_set_out_pins(&c, gpio, 1);
  sm_config_set_set_pins(&c, gpio, 1);
  sm_config_set_sideset_pins(&c, gpio + 1);
  pio_sm_set_consecutive_pindirs(pio, sm, gpio, 2, true);
  sm_config_set_out_shift(&c, false, true,
                          16);  // pull half-words, MSB first, auto
  sm_config_set_in_shift(&c, false, true,
                         16);   // push half-words, MSB first, auto
  sm_config_set_clkdiv(&c, 6);  // Run at 50/6 = <10MHz (4x data rate)
  pio_sm_init(pio, sm, addr, &c);
}

static void eth_write_phy(int addr, int reg, int val, uint8_t gpio) {
  uint16_t op = 0x5002 | (addr << 7) | (reg << 2);  // b0101aaaaarrrrr10
  smi_wr_init(pio0, SMITXSM, smi_wr_addr, gpio);
  pio_sm_put(pio0, SMITXSM, op << 16);
  pio_sm_put(pio0, SMITXSM, (uint16_t) val << 16);
  pio_sm_set_enabled(pio0, SMITXSM, true);
}

static uint16_t eth_read_phy(uint addr, uint reg, uint8_t gpio) {
  uint16_t op = 0x6000 | (addr << 7) | (reg << 2);  // b0110aaaaarrrrr00
  smi_rd_init(pio1, SMIRXSM, smi_rd_addr, gpio);
  pio_sm_put(pio1, SMIRXSM, op << 16);
  pio_sm_set_enabled(pio1, SMIRXSM, true);
  uint16_t val = (uint16_t) pio_sm_get_blocking(pio1, SMIRXSM);
  pio_sm_set_enabled(pio1, SMIRXSM, false);
  return val;
}

static void rx_irq(void);

static bool mg_tcpip_driver_rp2040_rmii_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_rp2040_rmii_data *d =
      (struct mg_tcpip_driver_rp2040_rmii_data *) ifp->driver_data;
  uint rx_sm_addr, tx_sm_addr;
  s_ifp = ifp;
  rx_sm_addr = pio_add_program(pio0, &rmii_rx_program);
  tx_sm_addr = pio_add_program(pio0, &rmii_tx_program);
  smi_wr_addr = pio_add_program(pio0, &smi_wr_program);
  smi_rd_addr = pio_add_program(pio1, &smi_rd_program);

  dma_tx = dma_claim_unused_channel(true);
  dmacfg_tx = dma_channel_get_default_config(dma_tx);
  channel_config_set_read_increment(&dmacfg_tx, true);    // read from memory
  channel_config_set_write_increment(&dmacfg_tx, false);  // write to FIFO
  channel_config_set_dreq(&dmacfg_tx, pio_get_dreq(pio0, TXSM, true));
  channel_config_set_transfer_data_size(&dmacfg_tx,
                                        DMA_SIZE_8);  // transfer bytes
  rmii_tx_init(pio0, TXSM, tx_sm_addr, d->tx0);

  eth_write_phy(d->phy_addr, 4, 0x61,
                d->mdio);  // Auto Negotiation Advertisement Register: enable
                           // only 10BASE-T full and half duplex (two-pair), use
                           // IEEE 802.3 format
  sleep_us(31);  // give some time for PIO to work (64 bits @2.5MHz = 25.6 us)
  eth_write_phy(d->phy_addr, 0, 0x1000,
                d->mdio);  // Basic Control Register: enable Auto-Negotiation
  sleep_us(31);

  dma_rx = dma_claim_unused_channel(true);
  dmacfg_rx = dma_channel_get_default_config(dma_rx);
  channel_config_set_read_increment(&dmacfg_rx, false);  // read from FIFO
  channel_config_set_write_increment(&dmacfg_rx,
                                     true);  // write to memory, increment
  channel_config_set_dreq(&dmacfg_rx, pio_get_dreq(pio0, RXSM, false));
  channel_config_set_transfer_data_size(&dmacfg_rx,
                                        DMA_SIZE_8);  // transfer bytes
  dma_channel_configure(dma_rx, &dmacfg_rx, s_rxbuf[0],
                        ((uint8_t *) &pio0->rxf[RXSM]) + 3, ETH_PKT_SIZE, true);
  pio_set_irq0_source_enabled(pio0, pis_interrupt0,
                              true);     // enable SM0 on PIO0_IRQ_0
  irq_set_exclusive_handler(7, rx_irq);  // set handler for PIO0_IRQ_0
  irq_set_enabled(7, true);              // enable it
  rmii_rx_init(pio0, RXSM, rx_sm_addr, d->rx0);

  return true;
}

static size_t mg_tcpip_driver_rp2040_rmii_tx(const void *buf, size_t len,
                                             struct mg_tcpip_if *ifp) {
  dma_channel_wait_for_finish_blocking(dma_tx);
  memset(s_txbuf, 0, 60);  // pre-pad
  memcpy(s_txbuf, buf, len);
  if (len < 60) len = 60;                    // pad
  uint32_t crc = mg_crc32(0, s_txbuf, len);  // host is little-endian
  memcpy(s_txbuf + len, (uint8_t *) &crc, 4);
  len += 4;
  sleep_us(10);  // enforce IFG in case software has been lightning fast...
  dma_channel_configure(dma_tx, &dmacfg_tx, ((uint8_t *) &pio0->txf[TXSM]) + 3,
                        s_txbuf, len, true);
  return len;
}

static uint32_t s_rxno;

// The max amount of time we should keep the PIO state machine stopped is the
// IFG time (10us) and that includes irq latency
static void rx_irq(void) {
  dma_channel_hw_t *hw = dma_channel_hw_addr(dma_rx);
  uint32_t rxno =
      (s_rxno + 1) &
      1;  // 2 buffers, switch to the available one as fast as possible
  size_t len = ETH_PKT_SIZE - hw->transfer_count;
  dma_channel_abort(dma_rx);
  dma_channel_set_write_addr(dma_rx, s_rxbuf[rxno], true);  // restart DMA
  pio_interrupt_clear(pio0,
                      0);  // ACK PIO IRQ so the state machine resumes receiving
  // NOTE(scaprile) Here we could check addressing to avoid queuing frames not
  // for us. The max amount of time we can linger here is what it takes for the
  // other buffer to fill (<8us) and that includes irq chaining
  if (len >= 64 && len <= ETH_PKT_SIZE)
    mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);
  s_rxno = rxno;
}

static bool mg_tcpip_driver_rp2040_rmii_up(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_rp2040_rmii_data *d =
      (struct mg_tcpip_driver_rp2040_rmii_data *) ifp->driver_data;
  uint32_t bsr =
      eth_read_phy(d->phy_addr, 1, d->mdio);  // Basic Status Register
  return (bsr & (1 << 2)) ? 1 : 0;            // check Link Status flag
}

struct mg_tcpip_driver mg_tcpip_driver_rp2040_rmii = {
    mg_tcpip_driver_rp2040_rmii_init,
    mg_tcpip_driver_rp2040_rmii_tx,
    NULL,
    mg_tcpip_driver_rp2040_rmii_up,
};
