#include <stdlib.h>
#include <string.h>

#include "main.h"

#ifdef UART_DEBUG // For internal testing purposes
#include "hal.h"
int stdout_putchar (int ch) {
  uart_write_byte(USART1, (uint8_t) ch);
  return ch;
}
#else
int stdout_putchar (int ch) {
  extern UART_HandleTypeDef huart3;
  HAL_UART_Transmit(&huart3, (const uint8_t *)&ch, 1, 100);
  return ch;
}
#endif
