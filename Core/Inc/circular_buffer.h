// circular_buffer.h
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CIRCULAR_BUFFER_TYPE
#error "CIRCULAR_BUFFER_TYPE must be defined before including this header"
#endif

#ifndef CIRCULAR_BUFFER_PREFIX
#define CIRCULAR_BUFFER_PREFIX cb
#endif

#define CB_NAME_JOIN(a, b) a##b
#define CB_NAME_EXPAND(a, b) CB_NAME_JOIN(a, b)
#define CB_NAME(name) CB_NAME_EXPAND(CIRCULAR_BUFFER_PREFIX, name)

#ifndef CIRCULAR_BUFFER_COMMON
#define CIRCULAR_BUFFER_COMMON

typedef enum {
    cb_status_ok = 0,
    cb_status_error_delay_too_large,
    cb_status_error_null
} cb_status_t;

#endif // CIRCULAR_BUFFER_COMMON

typedef struct {
    CIRCULAR_BUFFER_TYPE *buffer;
    uint16_t capacity;
    uint16_t index;
    uint16_t count;
} CB_NAME(_t);

static inline cb_status_t CB_NAME(_init)(CB_NAME(_t) *cb, CIRCULAR_BUFFER_TYPE
                                         *buffer, uint16_t capacity)
{
    if (!cb || !buffer) return cb_status_error_null;

    cb->buffer = buffer;
    cb->capacity = capacity;
    cb->index = 0;
    cb->count = 0;

    for (uint16_t i = 0; i < capacity; ++i) {
        cb->buffer[i] = (CIRCULAR_BUFFER_TYPE){0};
    }

    return cb_status_ok;
}

static inline cb_status_t CB_NAME(_push)(CB_NAME(_t) *cb,
                                         CIRCULAR_BUFFER_TYPE item)
{
    if (!cb || !cb->buffer) return cb_status_error_null;

    cb->buffer[cb->index] = item;
    cb->index = (cb->index + 1) % cb->capacity;

    if (cb->count < cb->capacity) {
        cb->count++;
    }

    return cb_status_ok;
}

/// Returns the number of valid elements in the buffer
static inline uint32_t CB_NAME(_get_count)(CB_NAME(_t) *cb)
{
    if (!cb) return 0;
    return cb->count;
}

/// Gets an element from the buffer with a delay (0 = most recent)
static inline cb_status_t CB_NAME(_get_delayed)(const CB_NAME(_t) *cb, uint16_t
                                                delay, CIRCULAR_BUFFER_TYPE
                                                *out)
{
    if (!out || !cb || !cb->buffer) return cb_status_error_null;
    if (delay >= cb->count) return cb_status_error_delay_too_large;

    uint16_t read_index = (cb->index + cb->capacity - delay - 1) % cb->capacity;
    *out = cb->buffer[read_index];

    return cb_status_ok;
}

#ifdef __cplusplus
}
#endif

