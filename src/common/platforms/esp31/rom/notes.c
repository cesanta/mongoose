/* Just some notes scribbled while disassembling */

/*
 * RTC = 0x60008000
 * RTC+0x18: ??c????? ???????? ???????? ????????
 * RTC+0x34: ???????? ??bbbbbb bbbb???? ??aaaaaa
 */
int _X_get_rst_cause(void) {
  int ret;
  int a = GET_PERI_REG_BITS(RTC_STATE1, 6, 0);
  if (a == 5) {
    int b = (RTC_STATE1 >> 12) && 0xfff;
    if (b != 1) {
      ret = (b == 8 ? a : 0);
    } else {
      ret = 20;
    }
  } else {
    ret = a;
  }
  CLEAR_PERI_REG_MASK(RTC_STATE0, RTC_CNTL_SLP_WAKEUP);
  return ret;
}

/*
 * RTC = 0x60008000
 * RTC+0x38: ???????? ???????? ???????? ??cccccc
 * RTC+0x74: ???????? ???????? ???????? dddddddd
 * RTC+0x80: ???????? ??????a? ???b???? ????????
 */
void main(void) {
  uint32_t rst_cause = _X_get_rst_cause();
  CLEAR_PERI_REG_MASK(RTC+0x80, BIT(17));     // a
  SET_PERI_REG_MASK(RTC+0x80, BIT(12));       // b
  uint32_t boot_mode = GET_PERI_REG_BITS(GPIO_STRAP, 6, 0);  // c
  if (boot_mode & (BIT(5) | BIT(4)) == (BIT(5) | BIT(4)) || boot_mode == 24 || boot_mode == 26) {
    CLEAR_PERI_REG_MASK(RTC+0x74, 0xff);
  }
  if (boot_mode & (BIT(5) | BIT(4)) == BIT(5)) {
    CLEAR_PERI_REG_MASK(RTC+0x94, BIT(31));
    CLEAR_PERI_REG_MASK(RTC+0x98, BIT(31));
    CLEAR_PERI_REG_MASK(RTC+0x9c, BIT(31));
    CLEAR_PERI_REG_MASK(RTC+0xa0, BIT(31));
    CLEAR_PERI_REG_MASK(RTC+0xa4, BIT(31));
    CLEAR_PERI_REG_MASK(RTC+0xa8, BIT(31));
    CLEAR_PERI_REG_MASK(RTC+0xac, BIT(31));
  }
  if (boot_mode & (BIT(5) | BIT(3)) == 0) {
    // ... 1405
  }
  CLEAR_PERI_REG_MASK(RTC+0x74, 0xff);
  _X_uart_attach();
  _X_uart_init(0);
  // GPIO_STRAP ...
  ets_printf(boot_banner, fw_build, rst_cause, boot_mode);
  // rst_cause
  if (rst_cause == 1 || rst_cause == 2) {
    
  } else {
    // ...
  }
  ets_printf("%s %u", "ets_main.c", 305);
  while(1) {}
}

/*
 * GPIO strap mapping:
 *
 * 0011 1111 1011 0011
 *   || |||| |||| ||||
 *   || |||| |||| |||`- IO5
 *   || |||| |||| ||`-- IO15
 *   || |||| |||| |`--- IO4
 *   || |||| |||| `---- IO2
 *   || |||| |||`------ ?
 *   || |||| ||`------- IO0
 *   || |||| |`-------- IO12
 *   || |||| `--------- ?
 *   || |||`----------- CLK
 *   || ||`------------ ?
 *   || |`------------- SD0
 *   || `-------------- SD1
 *   |`---------------- ? SD2
 *   `----------------- SD3
 */

struct uartdev {
  uint32_t baud_rate;  // 0
  uint32_t ud4;
  uint32_t ud8;
  uint32_t ud12;
  uint32_t ud16;
  uint32_t ud20;
  uint8_t ud24;
  uint8_t ud25;
  uint32_t ud28;
  uint32_t ud32;
  uint32_t ud36;
  uint8_t ud40;
  uint32_t ud48;
  uint32_t ud52;
};

void _X_uart_attach(void) {
  // zero uartdev
  uartdev.baud_rate = 115200;
  _X_xtos_ints_off(1 << ETS_UART_INUM);
  // INTR_MAP_REG_C
  // 11111111 11111111 11111100 00011111 &
  // 00000000 00000000 00000000 10100000 |
  // PRODPORT_INTR_MAP_13 -> 5 = ETS_UART_INUM
  // 11111111 11111111 10000011 11111111 &
  // 00000000 00000000 00010100 11111111 |
  // PRODPORT_INTR_MAP_14 -> 5 = ETS_UART_INUM
  _xtos_set_interrupt_handler_arg(ETS_UART_INUM, uart_int_handler, _c_0x3fffdb2c_uart_int_handler_arg);
}

void _X_uart_init(uint32_t a) {
  // GPIO_FUNC_IN_SEL3
  // xx999999 88888877 77776666 66555555
  // 11111111 11111100 00001111 11111111 = 0xfffc0fff
  // 00000000 00000000 10010000 00000000 = 0x00009000
  // GPIO17 func => 9
  // 00000000 00000010 00000000 00000000
  uart_div_modify(13000000 / uartdev.baud_rate);
  // ...
}

struct _st_0x3fffdc90 {
};

struct _st_0x3fffdf70 {
  void *fp1;  // 20
  void *fp2;  // 24
  uint32_t d28;
  uint32_t d32;
  uint32_t d36;
  struct _st_0x3fffdc90 *st;  // 44
} stdf70;

void _X_slc_init_attach(void *fp1, void *fp2, struct _st_0x3fffdc90 *st, uint32_t gpio_mode) {
  stdf70.fp1 = fp1;
  stdf70.fp2 = fp2;
  stdf70.st = st;
  d28 = d32 = d36 = 0;
  SET_PERI_REG_MASK(WIFI_RST_EN, PRODPORT_SDIO_RST);
  CLEAR_PERI_REG_MASK(WIFI_RST_EN, PRODPORT_SDIO_RST);
  if (gpio_mode == 4) {
    SET_PERI_REG((READ_PERI_REG(PERIPHS_HINF_BASEADDR+4) & 0xf0000000) | 0x01110013);
  } else {
    SET_PERI_REG((READ_PERI_REG(PERIPHS_HINF_BASEADDR+4) & 0xf0000000) | 0x02320017);
  }
  SET_PERI_REG(PERIPHS_HINF_BASEADDR, 0x11116666);
  _X_slc_set_host_io_max_window();
  ...
}

#define SLC_TOKEN1 (PERIPHS_SLC_BASEADDR + 0x54)
#define SLC_BRIDGE_CONF (PERIPHS_SLC_BASEADDR + 0x74)

void _X_slc_set_host_io_max_window(void) {
  SET_PERI_REG(SLC_BRIDGE_CONF, (READ_PERI_REG(SLC_BRIDGE_CONF) & 0xfffff0c0) | 0x720);
}
