#include "flash_error_log.h"
#include "nor_flash.h"
#include "flash_error_log_internal.h"

#include <assert.h>

error_log_status_t error_log_init()
{
  // TODO: implement
  return ERROR_LOG_OK;
}

error_log_status_t check_data_type(error_code_t error_code,
                                   error_data_types_t data_type) {
  // TODO: implement
  return ERROR_LOG_OK;
}

error_log_status_t error_log(error_code_t error_code,
                             error_data_types_t data_type,
                             error_code_data_t data)
{
  error_log_row_t row;

  error_log_status_t status = check_data_type(error_code, data_type);
  if (status != ERROR_LOG_OK) {
    return status;
  }

  row.row_start_marker = ERROR_LOG_ROW_START_MARKER;
  row.timestamp_ms = 0;
  row.error_code = error_code;
  row.data = data;

  // TODO: Use the correct address
  nor_flash_write(0, (uint8_t *)&row, sizeof(row));

  return ERROR_LOG_OK;
}
