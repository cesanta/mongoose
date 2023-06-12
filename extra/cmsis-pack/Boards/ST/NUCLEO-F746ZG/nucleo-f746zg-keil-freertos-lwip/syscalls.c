#include <stdlib.h>
#include <string.h>

#include "main.h"

int stdout_putchar (int ch) {
  extern UART_HandleTypeDef huart3;
  HAL_UART_Transmit(&huart3, (const uint8_t *)&ch, 1, 100);
  return ch;
}
