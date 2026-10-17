#include "RW612.h"
#include "fsl_flexspi.h" // peripheral structures

__attribute__((section(".iram"))) static void deinit_flash(FLEXSPI_Type *base) {
  CLKCTL0->PSCCTL0_SET =
      CLKCTL0_PSCCTL0_SET_FLEXSPI0_MASK; // Enable FLEXSPI clock again
  base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK; // Enable FLEXSPI module
  // Wait until FLEXSPI is not busy
  while (!((base->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) &&
           (base->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    (void)0;
  base->MCR0 |=
      FLEXSPI_MCR0_MDIS_MASK; // Disable module during the reset procedure
}

__attribute__((section(".iram"))) static void init_flash(FLEXSPI_Type *base) {
  uint32_t status;
  uint32_t lastStatus;
  uint32_t retry;

  // Loopback from DQS pad can maximize RD board flash speed.
  if ((base->MCR0 & FLEXSPI_MCR0_RXCLKSRC_MASK) != FLEXSPI_MCR0_RXCLKSRC(1)) {
    base->MCR0 =
        (base->MCR0 & ~FLEXSPI_MCR0_RXCLKSRC_MASK) | FLEXSPI_MCR0_RXCLKSRC(1);
  }
  base->DLLCR[0] = 0x1U;
  base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK; // Enable FLEXSPI module
  base->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
  while (base->MCR0 & FLEXSPI_MCR0_SWRESET_MASK)
    (void)0;

  // Need to wait DLL locked if DLL enabled
  if (0U != (base->DLLCR[0] & FLEXSPI_DLLCR_DLLEN_MASK)) {
    lastStatus = base->STS2;
    retry = 10;
    // Wait slave delay line locked and slave reference delay line locked.
    do {
      status = base->STS2;
      if ((status &
           (FLEXSPI_STS2_AREFLOCK_MASK | FLEXSPI_STS2_ASLVLOCK_MASK)) ==
          (FLEXSPI_STS2_AREFLOCK_MASK | FLEXSPI_STS2_ASLVLOCK_MASK)) {
        // Locked
        retry = 100;
        break;
      } else if (status == lastStatus) {
        retry--;
      } else {
        retry = 10;
        lastStatus = status;
      }
    } while (retry > 0);
    // According to ERR011377, need to delay at least 100 NOPs to ensure the DLL
    // is locked
    for (; retry > 0U; retry--)
      (void)0;
  }
}

__attribute__((section(".iram"))) void
set_flexspi_clock(FLEXSPI_Type *base, uint32_t src, uint32_t divider) {
  if ((CLKCTL0->FLEXSPIFCLKSEL != CLKCTL0_FLEXSPIFCLKSEL_SEL(src)) ||
      ((CLKCTL0->FLEXSPIFCLKDIV & CLKCTL0_FLEXSPIFCLKDIV_DIV_MASK) !=
       (divider - 1))) {
    // Always deinit FLEXSPI and init FLEXSPI for the flash to make sure the
    // flash works correctly after the FLEXSPI root clock changed as the default
    //  FLEXSPI configuration may does not work for the new root clock frequency.
    deinit_flash(base);

    // Disable clock before changing clock source
    CLKCTL0->PSCCTL0_CLR = CLKCTL0_PSCCTL0_CLR_FLEXSPI0_MASK;
    // Update flexspi clock.
    CLKCTL0->FLEXSPIFCLKSEL = CLKCTL0_FLEXSPIFCLKSEL_SEL(src);
    CLKCTL0->FLEXSPIFCLKDIV |=
        CLKCTL0_FLEXSPIFCLKDIV_RESET_MASK; // Reset the divider counter
    CLKCTL0->FLEXSPIFCLKDIV = CLKCTL0_FLEXSPIFCLKDIV_DIV(divider - 1);
    while ((CLKCTL0->FLEXSPIFCLKDIV) & CLKCTL0_FLEXSPIFCLKDIV_REQFLAG_MASK)
      (void)0;
    // Enable FLEXSPI clock again
    CLKCTL0->PSCCTL0_SET = CLKCTL0_PSCCTL0_SET_FLEXSPI0_MASK;
    init_flash(base);
  }
}
