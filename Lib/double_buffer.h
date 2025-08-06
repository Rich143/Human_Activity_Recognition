#ifndef DOUBLE_BUFFER_H
#define DOUBLE_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define NO_BUFFER_READY (-1)

typedef struct {
    volatile uint8_t isr_index;         // Which buffer ISR is writing into
    volatile int8_t ready_index;        // Which buffer is ready for main (or NO_BUFFER_READY)
} double_buffer_state_t;

void dbuf_init(double_buffer_state_t *state);
uint8_t dbuf_get_isr_index(double_buffer_state_t *state);
bool dbuf_mark_ready(double_buffer_state_t *state);
int8_t dbuf_get_ready(double_buffer_state_t *state);
void dbuf_mark_processed(double_buffer_state_t *state);

#endif // DOUBLE_BUFFER_H
