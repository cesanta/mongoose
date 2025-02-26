#include "hal.h"

#define MAX_PIN_NO 144

// Mocked device pins
static bool s_pins[MAX_PIN_NO];

bool gpio_write(uint16_t pin, bool status) {
  bool ok = false;
  if (pin >= 0 && pin < MAX_PIN_NO) {
    s_pins[pin] = status;
    ok = true;
  }
  return ok;
}

bool gpio_read(uint16_t pin) {
  return (pin >= 0 && pin < MAX_PIN_NO) ? s_pins[pin] : false;
}
