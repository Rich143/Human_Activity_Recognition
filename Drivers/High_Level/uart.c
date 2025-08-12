/*
 * uart.c
 *
 *  Created on: Aug 4, 2025
 *      Author: richardmatthews
 */

#include "uart.h"

#include "stm32u5xx_hal.h"
#include "main.h"

#include <stdio.h>

uint8_t rx_buff[1];

uart_rx_cli_cb_t uart_rx_cli_cb = NULL;

__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (uart_rx_cli_cb != NULL) {
        uart_rx_cli_cb(rx_buff[0]);
    }

    HAL_UART_Receive_IT(&huart1, rx_buff, 1);
}

void uart_cli_rx_start() {
    HAL_UART_Receive_IT(&huart1, rx_buff, 1);
}

void uart_rx_cli_register_callback(uart_rx_cli_cb_t cb) {
    uart_rx_cli_cb = cb;
}

bool uart_send_data(uint8_t *data, uint32_t len) {
    return HAL_UART_Transmit(&huart1, data, len, HAL_MAX_DELAY) == HAL_OK;
}
