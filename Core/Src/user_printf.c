/*
 * user_printf.c
 *
 *  Created on: Apr 15, 2025
 *      Author: richardmatthews
 */
#include "stm32u5xx_hal.h"
#include "main.h"

int _write(int file, char *ptr, int len)
{
  HAL_StatusTypeDef status;

  // Transmit the data over UART
  status = HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);

  // If successful, return the length, otherwise 0 or an error code
  return (status == HAL_OK) ? len : 0;
}
