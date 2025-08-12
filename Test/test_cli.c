#include "FreeRTOS_Types.h"
#include "unity.h"

#include "mock_uart.h"
#include "mock_cli_commands.h"
#include "mock_cmsis_gcc.h"

#include "cli.h"
#include "uart.h"
#include "FreeRTOS_CLI.h"
#include "double_buffer.h"

// Helper macros for test status checking
#define TEST_CLI_OK(expr) \
    do { \
        cli_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(CLI_STATUS_OK, status, "Expected CLI_STATUS_OK"); \
    } while (0)

uart_rx_cli_cb_t uart_rx_cli_cb;

void uart_rx_cli_register_callback_mock_cb(uart_rx_cli_cb_t cb, int cmock_num_calls)
{
    uart_rx_cli_cb = cb;
}

void mock_cmsis_gcc() {
    __get_PRIMASK_IgnoreAndReturn(0);
    __disable_irq_Ignore();
    __DMB_Ignore();
    __set_PRIMASK_Ignore();
}

static bool cli_initialised = false;

void setUp(void)
{
    mock_cmsis_gcc();

    uart_cli_rx_start_Ignore();

    uart_rx_cli_register_callback_Stub(&uart_rx_cli_register_callback_mock_cb);

    if (!cli_initialised) {
        // CLI Init can only be called once (due to command registration)
        TEST_CLI_OK(cli_init());
        cli_initialised = true;
    }

    TEST_CLI_OK(cli_start());
}

void tearDown(void)
{
}

void expectPing() {
    pingCommand_ExpectAndReturn(NULL, 0, NULL, pdFALSE);
    pingCommand_IgnoreArg_pcWriteBuffer();
    pingCommand_IgnoreArg_xWriteBufferLen();
    pingCommand_IgnoreArg_pcCommandString();
}

void sendPing() {
    uart_rx_cli_cb('p');
    uart_rx_cli_cb('i');
    uart_rx_cli_cb('n');
    uart_rx_cli_cb('g');
    uart_rx_cli_cb('\n');
}

void testPing(void) {
    expectPing();
    sendPing();

    TEST_CLI_OK(cli_run());
}

void test2Pings(void) {
    expectPing();
    expectPing();

    sendPing();
    TEST_CLI_OK(cli_run());

    sendPing();
    TEST_CLI_OK(cli_run());
}


void expectLogClear() {
    logClearCommand_ExpectAndReturn(NULL, 0, NULL, pdFALSE);
    logClearCommand_IgnoreArg_pcWriteBuffer();
    logClearCommand_IgnoreArg_xWriteBufferLen();
    logClearCommand_IgnoreArg_pcCommandString();
}

void sendLogClear() {
    uart_rx_cli_cb('l');
    uart_rx_cli_cb('o');
    uart_rx_cli_cb('g');
    uart_rx_cli_cb('C');
    uart_rx_cli_cb('l');
    uart_rx_cli_cb('e');
    uart_rx_cli_cb('a');
    uart_rx_cli_cb('r');
    uart_rx_cli_cb('\n');
}

void testLogClear(void) {
    expectLogClear();
    sendLogClear();
    TEST_CLI_OK(cli_run());
}

void expectLogDump() {
    logDumpCommand_ExpectAndReturn(NULL, 0, NULL, pdFALSE);
    logDumpCommand_IgnoreArg_pcWriteBuffer();
    logDumpCommand_IgnoreArg_xWriteBufferLen();
    logDumpCommand_IgnoreArg_pcCommandString();
}

void sendLogDump() {
    uart_rx_cli_cb('l');
    uart_rx_cli_cb('o');
    uart_rx_cli_cb('g');
    uart_rx_cli_cb('D');
    uart_rx_cli_cb('u');
    uart_rx_cli_cb('m');
    uart_rx_cli_cb('p');
    uart_rx_cli_cb('\n');
}

void testLogDump(void) {
    expectLogDump();
    sendLogDump();
    TEST_CLI_OK(cli_run());
}

void testMultipleCommands(void) {
    expectPing();
    expectLogClear();
    expectLogDump();

    sendPing();
    TEST_CLI_OK(cli_run());

    sendLogClear();
    TEST_CLI_OK(cli_run());

    sendLogDump();
    TEST_CLI_OK(cli_run());
}
