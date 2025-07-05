#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "accel_data_type.h"

void check_signal(const accel_data_t *actual, const accel_data_t *expected);
void check_signal_close(const accel_data_t *actual, const accel_data_t *expected,
                        float32_t tolerance);

#endif
