/*
 * cli.h
 *
 *  Created on: Aug 4, 2025
 *      Author: richardmatthews
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_

typedef enum {
    CLI_STATUS_OK = 0,
    CLI_STATUS_ERROR_NULL,
    CLI_STATUS_ERROR_OTHER,
} cli_status_t;

cli_status_t cli_init();
cli_status_t cli_start();
cli_status_t cli_run();

#endif /* INC_CLI_H_ */
