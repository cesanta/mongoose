/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    frdm-mcxn947-xpresso-freertos-builtin.c
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
#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

static void server(void *args) {
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  MG_INFO(("Initialising application..."));
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) mg_mgr_poll(&mgr, 1);  // Infinite event loop
  (void) args;
}

static void blinker(void *args) {
  gpio_output(LED1);
  for (;;) {
    gpio_toggle(LED1);
    vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
  }
  (void) args;
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

    hal_init();  // Cross-platform hardware init

    // Start tasks. NOTE: stack sizes are in 32-bit words
    xTaskCreate(blinker, "blinker", 128, ":)", configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(server, "server", 2048, 0, configMAX_PRIORITIES - 2, NULL);

    vTaskStartScheduler();  // This blocks
    return 0;
}
