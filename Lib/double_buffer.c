#include "double_buffer.h"

// CMSIS intrinsics for Cortex-M
#include "FreeRTOS_Types.h"

void dbuf_init(double_buffer_state_t *state)
{
    state->isr_index = 0;
    state->ready_index = NO_BUFFER_READY;
}

uint8_t dbuf_get_isr_index(double_buffer_state_t *state)
{
    return state->isr_index;
}

// Called from ISR when line is complete
bool dbuf_mark_ready(double_buffer_state_t *state)
{
    if (state->ready_index != NO_BUFFER_READY) {
        // Main hasn't processed the previous buffer yet
        return false;
    }

    state->ready_index = state->isr_index;
    state->isr_index = 1 - state->isr_index;  // Swap to other buffer
    return true;
}

// Called from main to check if a buffer is ready
int8_t dbuf_get_ready(double_buffer_state_t *state)
{
    int8_t index;

    critical_section_t cs = enter_critical_section();
    index = state->ready_index;
    exit_critical_section(cs);

    return index;
}

// Called from main after processing
void dbuf_mark_processed(double_buffer_state_t *state)
{
    critical_section_t cs = enter_critical_section();
    state->ready_index = NO_BUFFER_READY;
    exit_critical_section(cs);
}
