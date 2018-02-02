// These come from linker script

PROVIDE ( Cache_Read_Disable , 0x4000444c );
PROVIDE ( Cache_Read_Enable , 0x4000438c );

PROVIDE ( ets_delay_us , 0x40002db4 );

PROVIDE ( bzero , 0x40002a54 );

PROVIDE ( memcmp , 0x400068ec );
PROVIDE ( memcpy , 0x40006974 );
PROVIDE ( memmove , 0x40006a6c );
PROVIDE ( memset , 0x40006be4 );

PROVIDE ( strcmp , 0x40005bb8 );
PROVIDE ( strcpy , 0x40005cdc );
PROVIDE ( strlen , 0x40005d6c );
PROVIDE ( strncmp , 0x40005dd0 );
PROVIDE ( strncpy , 0x40005e90 );
PROVIDE ( strstr , 0x40005f6c );

PROVIDE ( ets_install_putc1 , 0x40002774 );
PROVIDE ( ets_printf , 0x40002804 );
PROVIDE ( ets_putc , 0x40002b14 );

PROVIDE ( ets_str2macaddr , 0x40002a64 );

PROVIDE ( gpio_output_set , 0x400049d8 );
PROVIDE ( gpio_output_set_high , 0x400049f8 );

PROVIDE ( ets_get_cpu_frequency , 0x40002de8 );
PROVIDE ( ets_update_cpu_frequency , 0x40002ddc );

PROVIDE ( lldesc_build_chain , 0x40004c8c );

PROVIDE ( multofup , 0x400068a0 );
PROVIDE ( roundup2 , 0x40006890 );

PROVIDE (software_reset_cpu , 0x40002998 );

PROVIDE ( SPIEraseSector , 0x40004708 );
PROVIDE ( SPIRead , 0x40004898 );
PROVIDE ( SPIWrite , 0x40004738 );

PROVIDE ( uart_div_modify , 0x400034e8 );
PROVIDE ( uart_tx_one_char , 0x4000362c );

PROVIDE ( __divsi3 , 0x40006888 );
PROVIDE ( __udivdi3 , 0x40006c30 );
PROVIDE ( __umoddi3 , 0x40006e64 );

PROVIDE ( _xtos_set_intlevel , 0x40006670 );

// These have been reverse-engineered.

PROVIDE(_XX_Vec40, 0x40000040)
PROVIDE(_XX_ExcVec50, 0x40000050)
PROVIDE(_XX_ExcVec80, 0x40000080)

PROVIDE(_XX_Vec400, 0x40000300)

PROVIDE(_WindowOverflowHandler, 0x40000100)
PROVIDE(_WindowUnderflowHandler, 0x40000140)

PROVIDE(_X_ResetVector, 0x40000500)

PROVIDE(_c_stack, 0x40000700)
PROVIDE(_c_bss_start, 0x40000708)
PROVIDE(_c_bss_end, 0x4000070c)
PROVIDE(_c_0x3fffc210, 0x40000734)
PROVIDE(_c_0x80000000, 0x40000738)
PROVIDE(_c_0x40000000, 0x40000760)
PROVIDE(_c_0x7fffffff, 0x40000780)
PROVIDE(_c_0x00ff0000, 0x40000798)

PROVIDE(_X_start, 0x400007ac)

PROVIDE(_c_0x3fffd820, 0x40000f50)
PROVIDE(_X_ets_task, 0x40000f54)
PROVIDE(_XX_unk0f84, 0x40000f84)
PROVIDE(_XX_unk0f96, 0x40000f98)

PROVIDE(_c_ets_critical_level, 0x400010a4)
PROVIDE(_X_ets_enter_critical, 0x400010a8)
PROVIDE(_X_ets_exit_critical, 0x400010bc)
PROVIDE(_X_ets_exit_critical_and_wait_int, 0x400010d4)
PROVIDE(_X_ets_isr_attach, 0x400010e8)  // 3 args
PROVIDE(_X_ets_isr_mask, 0x400010f8)  // 1 arg
PROVIDE(_X_ets_isr_unmask, 0x40001104)  // 1 arg

PROVIDE(_c_0x3fffda30, 0x40001110)
PROVIDE(_XX_set_0x3fffda30_0, 0x40001114)
PROVIDE(_XX_set_0x3fffda30_4, 0x40001120)
PROVIDE(_c_0xfffdffff, 0x4000112c)
PROVIDE(_c_0x60003e00, 0x40001130)
PROVIDE(_c_0x60008200, 0x40001134)
PROVIDE(_c_0x60007e00, 0x40001138)
PROVIDE(_c_0x1000, 0x4000113c)
PROVIDE(_s_fw_build, 0x40001140)
PROVIDE(_s_boot_banner, 0x40001144)
PROVIDE(_s_pct_s_pct_u, 0x40001148)
PROVIDE(_s_ets_main_c, 0x4000114c)
PROVIDE(_X_main, 0x4000115c)

PROVIDE(_l_strap_0x0xxx, 0x4000125a)
PROVIDE(_l_strap_init_uart0, 0x40001269)
PROVIDE(_l_strap_0x0x00, 0x400012e2)
PROVIDE(_l_boot, 0x400012ea)
PROVIDE(_l_rst_cause_345, 0x40001336)
PROVIDE(_l_rst_cause_12, 0x40001342)
PROVIDE(_l_strap_NxNxxx, 0x40001405)
PROVIDE(_l_strap_0010xx, 0x4000144c)
PROVIDE(_l_strap_001000_0x110x, 0x400014b0)
PROVIDE(_l_strap_0x0x11_loader, 0x400014c9)  // loader
PROVIDE(_l_strap_0x0x01, 0x400014d4)
PROVIDE(_l_strap_0x0x10, 0x400014e6)
PROVIDE(_c_0xffff8fff, 0x400014f0)
PROVIDE(_c_0x60008e00, 0x400014f4)

PROVIDE(_s_waiting_for_host, 0x4000152c)
PROVIDE(_s_mem_banner, 0x40001cdc)
PROVIDE(_c_stack_sentry, 0x40001ce0)
PROVIDE(_XX_unk153c, 0x4000153c)

PROVIDE(_c_data_end, 0x40001cd8)
PROVIDE(_c_data_start, 0x40001ce4)
PROVIDE(_X_print_mem_banner, 0x40001ce8)

PROVIDE(_s_exc_sp_fmt, 0x40001d0c)
PROVIDE(_s_exc_sf_dump_fmt, 0x40001d10)
PROVIDE(_s_exc_regs_fmt, 0x40001d14)
PROVIDE(_X_exc_handler, 0x40001d18)

PROVIDE(_XX_unk1d90, 0x40001d90)

PROVIDE(_X_ets_memset, 0x40001db4)
PROVIDE(_X_ets_memcpy, 0x40001dc4)
PROVIDE(_X_ets_memmove, 0x40001dd4)
PROVIDE(_X_ets_memcmp, 0x40001de4)

PROVIDE(_st_0x3fffda9c, 0x40002150)  // struct

PROVIDE(_X_ets_uart_putc, 0x4000223c)
PROVIDE(_X_ets_unk225c, 0x4000225c)

PROVIDE(_c_0x4000223c_ets_uart_putc, 0x40002780)
PROVIDE(_X_ets_install_uart_printf, 0x40002784)
PROVIDE(_c_0x400027dc, 0x40002790)
PROVIDE(_X_ets_install_external_printf, 0x40002794)
PROVIDE(_X_ets_install_putc2, 0x400027b4)
PROVIDE(_X_ets_get_printf_buf_remain_len, 0x400027c0)
PROVIDE(_X_ets_reset_printf_buf_len, 0x400027cc)
PROVIDE(_X_ets_putc, 0x400027dc)

PROVIDE(_c_0xdfffffff, 0x400028d4)
PROVIDE(_X_get_rst_cause, 0x400028d8)
PROVIDE(_XX_unk2948, 0x40002948)
PROVIDE(_l_2970, 0x40002970)
PROVIDE(_X_sw_sys_rst, 0x4000297c)
PROVIDE(_c_0x00400000, 0x400029b4)
PROVIDE(_c_0xffbfffff, 0x400029b8)
PROVIDE(_XX_apb_bridge_toggle, 0x400029bc)  // turns RTC_CNTL_APB2RTC_BRIDGE_SEL on and off
PROVIDE(_X_ets_strcpy, 0x400029ec)
PROVIDE(_X_ets_strncpy, 0x40002a00)
PROVIDE(_X_ets_strcmp, 0x40002a10)
PROVIDE(_X_ets_strncmp, 0x40002a24)
PROVIDE(_X_ets_strlen, 0x40002a34)
PROVIDE(_X_ets_strstr, 0x40002a40)

PROVIDE(_st_0x3fffdb10_uartdev, 0x40002e4c)  // some struct - uartdev?

PROVIDE(_c_0x3fffdb00, 0x40002e50)
PROVIDE(_c_0x3fffdb04, 0x40002f64)
PROVIDE(_XX_unk2e58, 0x40002e58)
PROVIDE(_X_UartDwnLdProc, 0x40002f6c)
PROVIDE(_c_0x00001800, 0x400030ec)
PROVIDE(_X_FlashDwnLdStartMsgProc, 0x400030f0)
PROVIDE(_XX_unk313c, 0x4000313c)
PROVIDE(_XX_unk31bc, 0x400031bc)
PROVIDE(_XX_unk31e4, 0x400031e4)
PROVIDE(_XX_unk3210, 0x40003210)
PROVIDE(_XX_unk3240, 0x40003240)
PROVIDE(_X_MemDwnLdStopReqMsgProc, 0x4000329c)
PROVIDE(_X_UartConnectProc, 0x400032c4)
PROVIDE(_X_UartRegWriteProc, 0x400032d4)
PROVIDE(_X_UartRegReadProc, 0x40003318)

PROVIDE(_c_115200, 0x4000332c)
PROVIDE(_c_0x3feffe00, 0x40003330)
PROVIDE(_c_0xffff83ff, 0x40003334)
PROVIDE(_c_0x00001400, 0x40003338)
PROVIDE(_c_0x40003728_uart_int_handler, 0x4000333c)
PROVIDE(_c_0x3fffdb2c_uart_int_handler_arg, 0x40003340)
PROVIDE(_X_uart_attach, 0x40003344)
PROVIDE(_XX_uart_set_unk33c0, 0x400033c0)
PROVIDE(_c_0x5ffffe00, 0x400033cc)
PROVIDE(_c_0x0000ffff, 0x400033d0)
PROVIDE(_c_0x000fffff, 0x40003448)
PROVIDE(_c_0x00060000, 0x400034e0)
PROVIDE(_c_0xfff9ffff, 0x400034e4)

PROVIDE(_c_0xfffc0fff, 0x40003520)
PROVIDE(_c_0x00009000, 0x40003524)
PROVIDE(_c_0x00020000, 0x40003528)
PROVIDE(_c_13000000, 0x4000352c)
PROVIDE(_c_0x08000000, 0x40003530)
PROVIDE(_X_uart_init, 0x40003534)
PROVIDE(_l_35f4, 0x400035f4)
PROVIDE(_X_uart_wait_tx_empty, 0x4000369c)

PROVIDE(_X_uart_int_handler, 0x40003728)

PROVIDE(_X_uart_tx_one_char2, 0x40003664)

PROVIDE(_X_send_packet, 0x400037d4)
PROVIDE(_X_SendMsg, 0x40003828)
PROVIDE(_X_recv_packet, 0x4000383c)
PROVIDE(_X_RcvMsg, 0x40003988)
PROVIDE(_X_uart_rx_readbuff, 0x400039a0)

PROVIDE(_c_0x60004e00, 0x40003a18)
PROVIDE(_X_SelectSpiFunction, 0x40003a1c)  // 1 arg - SPI number
PROVIDE(_c_0x60002e00, 0x40003c78)
PROVIDE(_X_SPI_chip_erase, 0x40003c7c)
PROVIDE(_c_0x00ffffff, 0x40003cb4)
PROVIDE(_c_0x01000000, 0x40003cb8)
PROVIDE(_XX_unk3cbc, 0x40003cbc)
PROVIDE(_c_0x00800000, 0x40003d00)
PROVIDE(_c_0x02000000, 0x40003d4c)
PROVIDE(_XX_unk3e24, 0x40003e24)
PROVIDE(_X_SPI_read_status, 0x40003efc)
PROVIDE(_c_0x90000000, 0x40003f48)
PROVIDE(_c_0x70000035, 0x40003f4c)
PROVIDE(_c_0x00040000, 0x40003f50)
PROVIDE(_XX_unk3f54, 0x40003f54)
PROVIDE(_c_0x04000000, 0x40003fcc)
PROVIDE(_XX_unk4010, 0x40004010)
PROVIDE(_X_SPI_write_enable, 0x400041bc)
PROVIDE(_X_Wait_SPI_Idle, 0x40004208)
PROVIDE(_l_4228, 0x40004228)
PROVIDE(_l_4234, 0x40004234)
PROVIDE(_XX_unk4238, 0x40004238)
PROVIDE(_X_SPIFlashModeConfig, 0x400042d8)
PROVIDE(_X_spi_flash_attach, 0x40004370)  // 2 args: SPI num, ???
PROVIDE(_X_SPIReadModeConfig, 0x40004538)

PROVIDE(_X_SPIEraseArea, 0x400048b4)
PROVIDE(_XX_unk4940, 0x40004940)

PROVIDE(_st_0x3fffdc90, 0x40004d80)
PROVIDE(_XX_unk4d88, 0x40004d88)
PROVIDE(_XX_unk4f6c, 0x40004f6c)
PROVIDE(_XX_unk4fc8, 0x40004fc8)

PROVIDE(_c_0xbfffffff, 0x40004c80)
PROVIDE(_c_0xff000fff, 0x40004c88)
PROVIDE(_XX_unk4f14, 0x40004f14)
PROVIDE(_s_no_rds, 0x40005008)
PROVIDE(_XX_unk500c, 0x4000500c)

PROVIDE(_fp_0x40004f6c, 0x40005164)
PROVIDE(_fp_0x40004fc8, 0x40005168)
PROVIDE(_X_sip_init_attach, 0x40005170)  // 1 arg, boot_mode?
PROVIDE(_XX_unk51ac, 0x400051ac)
PROVIDE(_c_0x60017e00, 0x40005478)
PROVIDE(_st_0x3fffdf70, 0x400054a4)
PROVIDE(_c_0x6000ae00, 0x400054b8)
PROVIDE(_c_0xf0000000, 0x400054bc)
PROVIDE(_c_0x02320017, 0x400054c0)
PROVIDE(_c_0x11116666, 0x400054c4)
PROVIDE(_c_0x01110013, 0x400054e4)
PROVIDE(_X_slc_init_attach, 0x400054e8)  // 4 args - fp, fp, st, boot_mode; PRODPORT_SDIO_RST
PROVIDE(_l_slc_boot_mode_4, 0x40005654)
PROVIDE(_X_slc_enable, 0x40005678)
PROVIDE(_X_slc_select_tohost_gpio_mode, 0x400056fc)
PROVIDE(_X_slc_select_tohost_gpio, 0x40005708)
PROVIDE(_c_0xff300000, 0x40005730)
PROVIDE(_XX_unk5734, 0x40005734)
PROVIDE(_XX_unk57b8, 0x400057b8)
PROVIDE(_XX_unk57f4, 0x400057f4)
PROVIDE(_XX_unk5848, 0x40005848)

PROVIDE(_c_0xfffff0c0, 0x40005988)
PROVIDE(_X_slc_set_host_io_max_window, 0x4000598c)
PROVIDE(_X_slc_init_credit, 0x400059ac)
PROVIDE(_X_slc_add_credits, 0x400059c4)

PROVIDE(_X_xtos_set_interrupt_handler_arg, 0x400059d8)
PROVIDE(_X_xtos_set_interrupt_handler, 0x40005a24)
PROVIDE(_X_xtos_ints_on, 0x40005a34)
PROVIDE(_X_xtos_ints_off, 0x40005a58)

PROVIDE(_XX_xtos_exc_unk5a80, 0x40005a80)
PROVIDE(_XX_xtos_exc_unk5b94, 0x40005b94)
