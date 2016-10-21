/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#include "app.h"
#include "system/debug/sys_debug.h"
#include "simplelink.h"
#include "user_params.h"

P_EVENT_HANDLER g_cc3100_IRQ_handler = NULL;

void AssertCS() {
  PLIB_PORTS_PinClear(PORTS_ID_0, CS_PORT, CS_PIN);
}

void DeassertCS() {
  PLIB_PORTS_PinSet(PORTS_ID_0, CS_PORT, CS_PIN);
}

void CC3100_enable() {
  PLIB_PORTS_PinSet(PORTS_ID_0, HIB_PORT, HIB_PIN);
}

void CC3100_disable() {
  PLIB_PORTS_PinClear(PORTS_ID_0, HIB_PORT, HIB_PIN);
}

Fd_t spi_Open(char *ifName, unsigned long flags) {
  CC3100_disable();

  return 0;
}

int spi_Write(Fd_t fd, unsigned char *pBuff, int len) {
  if (len > 512) {
    SYS_PRINT("BUFFER TOO SMALL");
    return 0;
  }

  AssertCS();
  uint8_t dummy[512];
  DRV_SPI0_BufferAddWriteRead(pBuff, dummy, len);
  DRV_SPI_Tasks(sysObj.spiObjectIdx0);
  DeassertCS();

  return len;
}

int spi_Read(Fd_t fd, unsigned char *pBuff, int len) {
  if (len > 512) {
    SYS_PRINT("BUFFER TOO SMALL");
    return 0;
  }
  uint8_t dummy[512];
  memset(dummy, 0xFF, len);
  AssertCS();
  DRV_SPI0_BufferAddWriteRead(dummy, pBuff, len);
  DRV_SPI_Tasks(sysObj.spiObjectIdx0);
  DeassertCS();
  return len;
}

int registerInterruptHandler(P_EVENT_HANDLER int_handler, void *pValue) {
  (void) pValue;
  g_cc3100_IRQ_handler = int_handler;
}
