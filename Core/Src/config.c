/*
 * config.c
 *
 *  Created on: Aug 23, 2025
 *      Author: richardmatthews
 */

#include "config.h"
#include <stdbool.h>

bool logging_enabled;
bool print_csv_enabled;
bool cli_enabled;
bool predictions_enabled;

void config_init() {
  logging_enabled = CONFIG_LOGGING_ENABLED_DEFAULT;
  print_csv_enabled = false;
  cli_enabled = false;
  predictions_enabled = CONFIG_PREDICTIONS_ENBLED_DEFAULT;
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

void config_set_predictions_enabled(bool enabled) {
  predictions_enabled = enabled;
}

bool config_get_predictions_enabled() {
  return predictions_enabled;
}
