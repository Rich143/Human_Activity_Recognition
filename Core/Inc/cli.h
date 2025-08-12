/*
 * cli.h
 *
 *  Created on: Aug 4, 2025
 *      Author: richardmatthews
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    CLI_STATUS_OK = 0,
    CLI_STATUS_ERROR_NULL,
    CLI_STATUS_ERROR_OTHER,
} cli_status_t;

cli_status_t cli_init();
cli_status_t cli_start();
cli_status_t cli_run();

/// Returns true if the CLI should be enabled
bool cli_check_for_input_to_enable();

#endif /* INC_CLI_H_ */
