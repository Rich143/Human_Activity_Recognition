/*
 * uart.c
 *
 *  Created on: Aug 4, 2025
 *      Author: richardmatthews
 */

#include "uart.h"

#include "stm32u5xx_hal.h"
#include "main.h"

#include <stdint.h>
#include <stdio.h>

#define UART_CLI_HANDLE huart1
#define UART_BLE_HANDLE huart4

#define UART_CLI_INSTANCE USART1
#define UART_BLE_INSTANCE UART4

uint8_t cli_rx_buff[1];
uint8_t ble_rx_buff[1];

uart_rx_cb_t uart_cli_rx_cb = NULL;

uart_rx_cb_t uart_ble_rx_cb = NULL;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if((uint32_t)huart->Instance == (uint32_t)UART_BLE_INSTANCE) {
        if (uart_ble_rx_cb != NULL) {
            uart_ble_rx_cb(ble_rx_buff[0]);
        }

        HAL_UART_Receive_IT(huart, ble_rx_buff, 1);
    } else if ((uint32_t)huart->Instance == (uint32_t)UART_CLI_INSTANCE) {
        if (uart_cli_rx_cb != NULL) {
            uart_cli_rx_cb(cli_rx_buff[0]);
        }

        HAL_UART_Receive_IT(huart, cli_rx_buff, 1);
    }
}

void uart_cli_rx_start() {
    HAL_UART_Receive_IT(&UART_CLI_HANDLE, cli_rx_buff, 1);
}

void uart_cli_rx_register_callback(uart_rx_cb_t cb) {
    uart_cli_rx_cb = cb;
}

void uart_ble_rx_start() {
    HAL_UART_Receive_IT(&UART_BLE_HANDLE, ble_rx_buff, 1);
}

void uart_ble_rx_register_callback(uart_rx_cb_t cb) {
    uart_ble_rx_cb = cb;
}

bool uart_cli_send_data(uint8_t *data, uint32_t len) {
    return HAL_UART_Transmit(&UART_CLI_HANDLE, data, len,
                             HAL_MAX_DELAY) == HAL_OK;
}

bool uart_ble_send_data(uint8_t *data, uint32_t len) {
    return HAL_UART_Transmit(&UART_BLE_HANDLE, data, len,
                             HAL_MAX_DELAY) == HAL_OK;
}
