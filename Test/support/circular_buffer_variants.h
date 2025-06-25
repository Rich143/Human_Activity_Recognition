#ifndef CB_VARIANTS_H
#define CB_VARIANTS_H

#include <stdint.h>

#define CIRCULAR_BUFFER_TYPE uint8_t
#define CIRCULAR_BUFFER_PREFIX u8_cb
#include "circular_buffer.h"
#undef CIRCULAR_BUFFER_TYPE
#undef CIRCULAR_BUFFER_PREFIX

#define CIRCULAR_BUFFER_TYPE float
#define CIRCULAR_BUFFER_PREFIX f32_cb
#include "circular_buffer.h"
#undef CIRCULAR_BUFFER_TYPE
#undef CIRCULAR_BUFFER_PREFIX

#endif // CB_VARIANTS_H
