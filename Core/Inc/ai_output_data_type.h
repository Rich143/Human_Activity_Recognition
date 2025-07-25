/*
 * ai_input_output_type.h
 *
 *  Created on: July 25, 2025
 *      Author: richardmatthews
 */

#ifndef AI_OUTPUT_DATA_TYPE_H
#define AI_OUTPUT_DATA_TYPE_H

/// Output is 4 floats (16 bytes)
#define AI_OUTPUT_CHANNEL 4

/// Class_names for PAMAP2 (check user_config.yaml):
///     [stationary, walking, running, cycling]
typedef enum {
  AI_OUTPUT_DATA_TYPE_STATIONARY = 0,
  AI_OUTPUT_DATA_TYPE_WALKING = 1,
  AI_OUTPUT_DATA_TYPE_RUNNING = 2,
  AI_OUTPUT_DATA_TYPE_CYCLING = 3
} ai_output_data_type_t;

#endif /* AI_OUTPUT_DATA_TYPE_H */
