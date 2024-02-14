#pragma once

#include "mongoose.h"

bool gpio_write(uint16_t pin, bool status);
bool gpio_read(uint16_t pin);
