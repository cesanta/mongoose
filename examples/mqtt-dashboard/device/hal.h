// Mocked device pins
static bool s_pins[100];

static inline void gpio_write(uint16_t pin, bool status) {
  if (pin <= (int) (sizeof(s_pins) / sizeof(s_pins[0]))) {
    s_pins[pin] = status;
  }
}

static inline int gpio_read(uint16_t pin) {
  return (pin <= (int) (sizeof(s_pins) / sizeof(s_pins[0])));
}