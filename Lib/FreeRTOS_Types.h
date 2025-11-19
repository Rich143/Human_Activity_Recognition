#ifndef FREERTOS_TYPES_H
#define FREERTOS_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "flash_error_log.h"

#include "cmsis_gcc.h"

typedef int32_t BaseType_t;
typedef uint32_t UBaseType_t;

#define pdTRUE true
#define pdFALSE false

#define pdPASS 0


#define configASSERT(expr) \
    do { \
        if (!(expr)) { \
            printf("Assertion failed: %s, file %s, line %d\n", \
                    #expr, __FILE__, __LINE__); \
            LOG_ERROR(ERROR_CLI_FREERTOS_ASSERT_ERROR, ERROR_DATA_LINE_NUMBER, \
                      __LINE__, ERROR_LOG_HANG_ON_LOG_FAILURE); \
            while (1); \
        } \
    } while (0)

typedef struct {
    uint32_t saved_primask;
} critical_section_t;

static inline critical_section_t enter_critical_section(void) {
    critical_section_t cs;
    cs.saved_primask = __get_PRIMASK();
    __disable_irq();
    __DMB();
    return cs;
}

static inline void exit_critical_section(critical_section_t cs) {
    __DMB();
    __set_PRIMASK(cs.saved_primask);
}

#endif
