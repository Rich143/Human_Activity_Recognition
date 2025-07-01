/*
 * filter.h
 *
 *  Created on: June 30, 2025
 *      Author: richardmatthews
 */

#ifndef FILTER_SIGNALS_H
#define FILTER_SIGNALS_H

#include "preprocess.h"

typedef enum {
    FILTER_STATUS_OK = 0,
    DELAY_STATUS_ERROR_NULL,
    DELAY_STATUS_ERROR_OTHER,
} filter_status_t;

filter_status_t filter_signal(AccelData *data, AccelData *output);

#endif
