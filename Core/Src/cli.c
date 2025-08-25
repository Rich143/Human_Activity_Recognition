/*
 * cli.c
 *
 *  Created on: Aug 4, 2025
 *      Author: richardmatthews
 */

#include "cli.h"
#include "FreeRTOS_CLI_Config.h"
#include "FreeRTOS_Types.h"
#include "High_Level/uart.h"
#include "FreeRTOS_CLI.h"
#include "config.h"
#include "terminal_print.h"
#include "cli_commands.h"
#include "double_buffer.h"
#include "stm32u5xx_hal.h"

#include <stdio.h>
#include <string.h>

static double_buffer_state_t buffer_state;

// Only ISR writes / reads these, no protection needed
uint32_t rx_buffer_0_ptr;
uint32_t rx_buffer_1_ptr;

// ISR and main never access the same buffer, no protection needed
uint8_t rx_buffer_0[configCOMMAND_INT_MAX_INPUT_SIZE];
uint8_t rx_buffer_1[configCOMMAND_INT_MAX_INPUT_SIZE];

const char *prompt = "CLI > ";
const char enable_char = 'a';

static volatile uint32_t received_enable_char_count = 0;

void uart_rx_cb_check_for_enable_input(uint8_t data) {
    if (data == enable_char) {
        received_enable_char_count++;
    } else if (received_enable_char_count >= CONFIG_CLI_ENABLE_KEY_PRESSES
               && (data == '\n' || data == '\r')) {
        // Ignore carriage returns and newlines if we've received enough enable chars
        return;
    } else {
        received_enable_char_count = 0;
    }
}

uint8_t *get_uart_rx_buffer_isr() {
    if (dbuf_get_isr_index(&buffer_state) == 0) {
        return rx_buffer_0;
    } else {
        return rx_buffer_1;
    }
}

uint32_t *get_uart_rx_buffer_isr_ptr() {
    if (dbuf_get_isr_index(&buffer_state) == 0) {
        return &rx_buffer_0_ptr;
    } else {
        return &rx_buffer_1_ptr;
    }
}

void handle_command_received() {
    if (dbuf_get_ready(&buffer_state) != NO_BUFFER_READY) {
        // Error: main is still processing last command
        printf("ERROR: CLI ISR has no free buffers\n");
        while (dbuf_get_ready(&buffer_state) != NO_BUFFER_READY);
    }

    // Reset buffer ptr, since main doesn't use this
    // Main is responsible for clearing the buffer when done
    uint32_t *rx_buffer_ptr = get_uart_rx_buffer_isr_ptr();
    *rx_buffer_ptr = 0;

    dbuf_mark_ready(&buffer_state);
}

void handle_erase_char() {
    uint32_t *rx_buffer_ptr = get_uart_rx_buffer_isr_ptr();

    if ((*rx_buffer_ptr) > 0) {
        (*rx_buffer_ptr)--;
    }
}

void handle_new_char(uint8_t data) {
    uint8_t *rx_buffer = get_uart_rx_buffer_isr();
    uint32_t *rx_buffer_ptr = get_uart_rx_buffer_isr_ptr();

    rx_buffer[*rx_buffer_ptr] = data;

    (*rx_buffer_ptr)++;
}

void uart_rx_cb(uint8_t data) {
    switch (data) {
        case '\n':
            handle_command_received();
            break;
        case '\r':
            // Ignore carriage returns
            break;
        case '\b':
            handle_erase_char();
            break;
        default:
            handle_new_char(data);
            break;
    }
}

CLI_Definition_List_Item_t pingCommandListItem;
CLI_Definition_List_Item_t logClearCommandListItem;
CLI_Definition_List_Item_t logDumpCommandListItem;
CLI_Definition_List_Item_t logSizeCommandListItem;
CLI_Definition_List_Item_t printCSVDataCommandListItem;
CLI_Definition_List_Item_t logEnableCommandListItem;
CLI_Definition_List_Item_t exitCLICommandListItem;
CLI_Definition_List_Item_t flashDumpCommandListItem;
CLI_Definition_List_Item_t flashClearCommandListItem;
CLI_Definition_List_Item_t flashClearChipCommandListItem;
CLI_Definition_List_Item_t flashClearedCommandListItem;


cli_status_t cli_register_commands() {
    BaseType_t status = FreeRTOS_CLIRegisterCommandStatic(&pingCommandDefinition,
                                      &pingCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&logClearCommandDefinition,
                                      &logClearCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&logDumpCommandDefinition,
                                      &logDumpCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&logSizeCommandDefinition,
                                      &logSizeCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&printCSVDataCommandDefinition,
                                      &printCSVDataCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&logEnableCommandDefinition,
                                      &logEnableCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&exitCLICommandDefinition,
                                      &exitCLICommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&flashDumpCommandDefinition,
                                      &flashDumpCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&flashClearCommandDefinition,
                                      &flashClearCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&flashClearChipCommandDefinition,
                                      &flashClearChipCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    status = FreeRTOS_CLIRegisterCommandStatic(&flashClearedCommandDefinition,
                                      &flashClearedCommandListItem);
    if (status != pdPASS) {
        return CLI_STATUS_ERROR_OTHER;
    }

    return CLI_STATUS_OK;
}

void cli_setup_waiting_for_input() {
    received_enable_char_count = 0;

    uart_cli_rx_register_callback(uart_rx_cb_check_for_enable_input);

    uart_cli_rx_start();

    cli_printf("Press '%c' %d times to enable CLI...\n", enable_char,
               CONFIG_CLI_ENABLE_KEY_PRESSES);
}

cli_status_t cli_init() {
    // We only register commands here so that unit tests can do this once and
    // only once. FreeRTOS_CLI is not able to be initialized more than once due
    // to command registration
    return cli_register_commands();
}

cli_status_t cli_start() {
    rx_buffer_0_ptr = 0;
    memset( rx_buffer_0, 0x00, configCOMMAND_INT_MAX_INPUT_SIZE );

    rx_buffer_1_ptr = 0;
    memset( rx_buffer_1, 0x00, configCOMMAND_INT_MAX_INPUT_SIZE );

    memset( FreeRTOS_CLIGetOutputBuffer(), 0x00, configCOMMAND_INT_MAX_OUTPUT_SIZE );

    dbuf_init(&buffer_state);

    uart_cli_rx_register_callback(uart_rx_cb);

    received_enable_char_count = 0;

    cli_printf("\n%s", prompt);

    return CLI_STATUS_OK;
}

void handle_command(uint8_t *rx_buffer) {
    BaseType_t moreDataToFollow;
    char *output_buffer = FreeRTOS_CLIGetOutputBuffer();

    cli_printf("\r\n");

    do
    {
        /* Send the command string to the command interpreter. Any
               output generated by the command interpreter will be placed in the
               output buffer. */
        moreDataToFollow = FreeRTOS_CLIProcessCommand(rx_buffer,
                                                      output_buffer,
                                                      configCOMMAND_INT_MAX_OUTPUT_SIZE);

        /* Write the output generated by the command interpreter to the
           console. */
        if (strlen(output_buffer) > 0) {
            cli_printf("%s", output_buffer);
        }

    } while( moreDataToFollow != pdFALSE );

    if (config_get_cli_enabled()) {
        // Need to check enabled, so we don't print prompt after exit
        cli_printf("%s", prompt);
    }

    /* All the strings generated by the input command have been sent.
       Processing of the command is complete.
       Clear the input buffer ready for the next command. */
    memset( rx_buffer, 0x00, configCOMMAND_INT_MAX_INPUT_SIZE );
    memset( output_buffer, 0x00, configCOMMAND_INT_MAX_OUTPUT_SIZE );
}

uint8_t *get_uart_rx_buffer_main(int8_t readyBuffer) {
    if (readyBuffer == NO_BUFFER_READY) {
        return NULL;
    } else if (readyBuffer == 0) {
        return rx_buffer_0;
    } else if (readyBuffer == 1) {
        return rx_buffer_1;
    } else {
        return NULL;
    }
}

cli_status_t cli_run() {
    int8_t readyBuffer = dbuf_get_ready(&buffer_state);

    if (readyBuffer != NO_BUFFER_READY) {
        uint8_t *rx_buffer = get_uart_rx_buffer_main(readyBuffer);
        if (rx_buffer == NULL) {
            return CLI_STATUS_ERROR_OTHER;
        }

        handle_command(rx_buffer);
        dbuf_mark_processed(&buffer_state);
    }

    return CLI_STATUS_OK;
}

bool cli_check_for_input_to_enable() {
    if (received_enable_char_count >= CONFIG_CLI_ENABLE_KEY_PRESSES) {
        return true;
    } else {
        return false;
    }
}
