/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    MCXN947_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MCXN947_cm33_core0.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "hal.h"
#include "mongoose.h"
#include "net.h"

/* TODO: insert other definitions and declarations here. */
#define BLINK_PERIOD_MS 1000

static void timer_fn(void *arg) {
  gpio_toggle(LED1);  // Blink LED
  (void) arg;         // Unused
}

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    struct mg_mgr mgr;  // Mongoose event manager

    hal_init();  // Cross-platform hardware init

    mg_mgr_init(&mgr);        // Initialise it
    mg_log_set(MG_LL_DEBUG);  // Set log level to debug
    mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, &mgr);

    MG_INFO(("Initialising application..."));
    web_init(&mgr);

    for (;;) {
      mg_mgr_poll(&mgr, 0);
    }

    return 0;
}
