/*
 * utils.c
 *
 *  Created on: Nov 4, 2025
 *      Author: richardmatthews
 */

#include "utils.h"

uint32_t ceil_div(uint32_t a, uint32_t b) {
    return (a + b - 1) / b;
}
