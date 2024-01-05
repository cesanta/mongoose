/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    rt1060-evk-xpresso-baremetal-builtin.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1062.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "hal.h"
#include "mongoose.h"
#include "net.h"

/* TODO: insert other definitions and declarations here. */
#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void mg_random(void *buf, size_t len) {  // Use on-board RNG
  TRNG_GetRandomData(TRNG, buf, len);  // Init by BOARD_InitBootPeripherals()
}

static void timer_fn(void *arg) {
  USER_LED_TOGGLE();
  struct mg_tcpip_if *ifp = arg;                         // And show
  const char *names[] = {"down", "up", "req", "ready"};  // network stats
  MG_INFO(("Ethernet: %s, IP: %M, rx:%u, tx:%u, dr:%u, er:%u",
           names[ifp->state], mg_print_ip4, &ifp->ip, ifp->nrecv, ifp->nsent,
           ifp->ndrop, ifp->nerr));
}

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
    ethernet_init();        // Initialise ethernet pins and clocks
    USER_LED_INIT(1);
    test_init();  // for internal testing purposes only
    MG_INFO(("Starting, CPU freq %g MHz", (double) SystemCoreClock / 1000000));

    struct mg_mgr mgr;        // Initialise
    mg_mgr_init(&mgr);        // Mongoose event manager
    mg_log_set(MG_LL_DEBUG);  // Set log level

    // Initialise Mongoose network stack
    struct mg_tcpip_driver_imxrt_data driver_data = {.mdc_cr = 24};
    struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(),
                              // Uncomment below for static configuration:
                              // .ip = mg_htonl(MG_U32(192, 168, 0, 223)),
                              // .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                              // .gw = mg_htonl(MG_U32(192, 168, 0, 1)),
                              .driver = &mg_tcpip_driver_imxrt,
                              .driver_data = &driver_data};
    mg_tcpip_init(&mgr, &mif);
    mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, &mif);

    MG_INFO(("MAC: %M. Waiting for IP...", mg_print_mac, mif.mac));
    while (mif.state != MG_TCPIP_STATE_READY) {
      mg_mgr_poll(&mgr, 0);
    }

    MG_INFO(("Initialising application..."));
    web_init(&mgr);

    MG_INFO(("Starting event loop"));
    for (;;) {
      mg_mgr_poll(&mgr, 0);
    }

    return 0;
}
