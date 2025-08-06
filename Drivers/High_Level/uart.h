/*
 * uart.h
 *
 *  Created on: Aug 4, 2025
 *      Author: richardmatthews
 */

#ifndef HIGH_LEVEL_UART_H_
#define HIGH_LEVEL_UART_H_

#include <stdint.h>

typedef void (*uart_rx_cli_cb_t)(uint8_t data);

void uart_cli_rx_start();

void uart_rx_cli_register_callback(uart_rx_cli_cb_t cb);

#endif /* HIGH_LEVEL_UART_H_ */
