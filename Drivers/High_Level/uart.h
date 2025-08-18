/*
 * uart.h
 *
 *  Created on: Aug 4, 2025
 *      Author: richardmatthews
 */

#ifndef HIGH_LEVEL_UART_H_
#define HIGH_LEVEL_UART_H_

#include <stdint.h>
#include <stdbool.h>

typedef void (*uart_rx_cb_t)(uint8_t data);

void uart_cli_rx_start();

void uart_cli_rx_register_callback(uart_rx_cb_t cb);

void uart_ble_rx_start();

void uart_ble_rx_register_callback(uart_rx_cb_t cb);

bool uart_cli_send_data(uint8_t *data, uint32_t len);

bool uart_ble_send_data(uint8_t *data, uint32_t len);

#endif /* HIGH_LEVEL_UART_H_ */
