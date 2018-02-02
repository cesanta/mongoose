.text
.org 0
.globl _start

// xtensa-esp108-elf-gcc -Wl,-N,-Ttext,0x40000000 -nostdlib rom.S -o rom.elf

here = .
#define PROVIDE(name, addr) name = here + addr - 0x40000000

#include "rom_functions.S"

PROVIDE(SPI_chip_erase, 0x40004080)
PROVIDE(SPIFlashModeConfig, 0x40004568)

PROVIDE(_c_0x80000000, 0x400003a4)
PROVIDE(_c_0x00400000, 0x40000414)
PROVIDE(_c_0x00ffffff, 0x40000418)
PROVIDE(_p_user_start, 0x40000fb8)  // 0x3fffdcd0
PROVIDE(_c_0x60000200, 0x40000fc4)
PROVIDE(_s_ets_build, 0x40000fc8)   // Jan  8 2013
PROVIDE(_s_ets_banner, 0x40000fcc)  // ets %s,rst cause:%d, boot mode:(%d,%d)
PROVIDE(_s_format_s_s, 0x40000fd0)  // %s %s
PROVIDE(_s_ets_main_c, 0x40000fd4)  // ets_main.c
PROVIDE(_s_user_code_done, 0x40000fe0)   // user code done
PROVIDE(_s_wdt_reset, 0x40000fe4)   // wdt reset
PROVIDE(_s_unk_reset, 0x40000fe8)   // unknown reset
PROVIDE(_l_boot_mode_6, 0x40001044)
PROVIDE(_l_boot_mode_0_1_2_3, 0x4000104a)
PROVIDE(_l_rst_cause_ge_3, 0x40001075)
PROVIDE(_l_rst_cause_ge_3_lt_7, 0x40001084)
PROVIDE(_l_rst_cause_3_soft_wdt, 0x4000108a)
PROVIDE(_l_load_from_flash, 0x400010a8)
PROVIDE(_l_run_user_code, 0x400010be)
PROVIDE(_l_wdt_reset, 0x40001118)
PROVIDE(_l_rst_cause_1_2, 0x40001121)
PROVIDE(_l_rst_cause_unknown, 0x40001130)
PROVIDE(_l_boot_mode_7, 0x40001148)
PROVIDE(_l_boot_mode_4, 0x40001150)
PROVIDE(_c_0x60000600, 0x4000115c)
PROVIDE(_s_waiting_for_host, 0x4000119c)  // waiting for host
PROVIDE(_x_unk1160, 0x40001160)
PROVIDE(_c_0x3fffa000_uart_buf, 0x400011a0)
PROVIDE(_c_0x2000, 0x400011a4)
PROVIDE(_c_0x40100000, 0x400011a8)  // default user_start
PROVIDE(_x_boot2, 0x400011ac)  // arg: 0x3fffdcd0 (&user_start)
PROVIDE(_l_strapping_2_eq_0, 0x400011d2)
PROVIDE(_l_124d, 0x4000124d)
PROVIDE(_l_boot_mode2_eq_2, 0x40001265)
PROVIDE(_l_boot_mode_eq_2, 0x40001268)
PROVIDE(_l_boot_mode_eq_1, 0x4000127a)
PROVIDE(_l_setup_uart, 0x40001291)
PROVIDE(_l_boot_mode_eq_3, 0x400012b0)
PROVIDE(_x_load_from_flash, 0x40001308)
PROVIDE(_c_0x00001000, 0x40001994)
PROVIDE(_c_0xffdfffff, 0x400025dc)
PROVIDE(_l_rr_not_dsleep, 0x40002624)
PROVIDE(_c_100000, 0x40002664)
PROVIDE(_c_0x3feffe00, 0x40002e5c)
PROVIDE(_p_cpu_freq, 0x40002ec8)
PROVIDE(_x_wdt_interval, 0x40002f14)  // arg: 3 -> 11, 6 -> 12, 12 -> 13
PROVIDE(_p_wdt_cfg, 0x40002f30)
PROVIDE(_wdt_soft_timer_fn2, 0x40002f3c)
PROVIDE(_l_wdt_soft_timer_fn2_exit, 0x40002f59)
PROVIDE(_l_wdt_soft_timer_fn2_feed, 0x40002f60)
PROVIDE(_l_wdt_soft_timer_fn2_mode2_stage1_feed, 0x40002f74)
PROVIDE(_wdt_soft_timer_fn, 0x40002f88)
PROVIDE(_p_wdt_soft_timer_fn, 0x40002f98)
PROVIDE(_p_wdt_soft_timer, 0x40002f9c)  // 0x3fffdde0
PROVIDE(_l_skip_wdt_imask, 0x40002fc4)
PROVIDE(_l_wdt_enable_and_exit, 0x40002fea)
PROVIDE(_l_wdt_mode_1, 0x4000309c)
PROVIDE(_l_wdt_mode_2_4, 0x4000300a)
PROVIDE(_l_wdt_mode_3, 0x40003060)
PROVIDE(_l_recv_req, 0x400033a1)
PROVIDE(_l_send_response, 0x400033be)
PROVIDE(_l_send_resp_pkt, 0x400033da)
PROVIDE(_l_process_req, 0x40003424)
PROVIDE(_l_34a3, 0x400034a3)
PROVIDE(_l_34b9, 0x400034b9)
PROVIDE(_c_0x00001800, 0x40003534)
PROVIDE(_c_0x60000a00, 0x40003f54)
PROVIDE(_c_0x01000000, 0x400040bc)
PROVIDE(_x_SPI_erase_sector, 0x400040c0)
PROVIDE(_l_SPI_erase_sector_align_ok, 0x400040dc)
PROVIDE(_c_0x00800000, 0x4000411c)
PROVIDE(_x_SPI_erase_block, 0x40004120)
PROVIDE(_c_0x20000000, 0x4000416c)
PROVIDE(_c_0x08000000, 0x400043c4)
PROVIDE(_c_0x04000000, 0x400043fc)
PROVIDE(_c_0x40000000, 0x40004438)
PROVIDE(_p_flashchip, 0x40004874)
PROVIDE(_s_bootup, 0x400054cc)      // bootup , addr 0x%08x
PROVIDE(_p_sip_ctx, 0x40005130)
PROVIDE(_l_sip_cmd_out, 0x4000550a)
PROVIDE(_c_0x00010000, 0x40005de0)

.text

_start:
.incbin "rom.bin"
_end:
