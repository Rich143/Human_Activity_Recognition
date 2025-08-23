/*
 * config.c
 *
 *  Created on: Aug 23, 2025
 *      Author: richardmatthews
 */

#include "config.h"
#include <stdbool.h>

bool logging_enabled = false;
bool print_csv_enabled = false;
bool cli_enabled = false;

void config_init() {
  logging_enabled = false;
  print_csv_enabled = false;
  cli_enabled = false;
}

void config_set_logging_enabled(bool enabled) {
  logging_enabled = enabled;
}

void config_set_print_csv_data(bool enabled) {
  print_csv_enabled = enabled;
}

bool config_get_logging_enabled() {
  return logging_enabled;
}

bool config_get_print_csv_enabled() {
  return print_csv_enabled;
}

bool config_get_cli_enabled() {
  return cli_enabled;
}

void config_set_cli_enabled(bool enabled) {
  cli_enabled = enabled;
}
