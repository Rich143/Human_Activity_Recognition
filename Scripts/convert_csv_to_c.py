#!/usr/bin/env python3

import argparse
import pandas as pd
import numpy as np

###
# Argument Parsing
###

parser = argparse.ArgumentParser(
    description="Convert a dataset CSV file to a C array (header and source file)"
)

# Named arguments with short and long flags
parser.add_argument(
    "-i", "--input",
    required=True,
    help="Path to the input CSV file"
)

parser.add_argument(
    "-n", "--name",
    required=True,
    help="Name of the data array (also used for output file names)"
)

parser.add_argument(
    "-o", "--output",
    required=True,
    help="Output directory"
)

args = parser.parse_args()

###
# Main Script
###

def format_array(values, per_line=4):
    lines = []
    for i in range(0, len(values), per_line):
        chunk = values[i:i+per_line]
        line = ", ".join(f"{v:.7f}f" for v in chunk)
        lines.append(f"    {line}")
    return ",\n".join(lines)

input_csv = args.input
output_name = args.name.upper()
output_dir = args.output

# Define the correct column headers
columns = [
    "unproc_x", "unproc_y", "unproc_z",
    "lowpass_filtered_x", "lowpass_filtered_y", "lowpass_filtered_z",
    "proc_x", "proc_y", "proc_z",
    "model_output_0", "model_output_1", "model_output_2", "model_output_3",
    "output_class", "contains_output"
]

# Load the CSV using the correct headers
df = pd.read_csv(input_csv, skiprows=1, names=columns)
df.reset_index(drop=True, inplace=True)
df["time_s"] = df.index * 0.01  # Assuming 100 Hz

# # Filter valid output
# valid_mask = df["contains_output"] == 1
# df_valid = df[valid_mask].reset_index(drop=True)

# # Extract float arrays
# output_x = df_valid["proc_x"].astype(np.float32).to_numpy()
# output_y = df_valid["proc_y"].astype(np.float32).to_numpy()
# output_z = df_valid["proc_z"].astype(np.float32).to_numpy()

input_x = df["unproc_x"].astype(np.float32).to_numpy()
input_y = df["unproc_y"].astype(np.float32).to_numpy()
input_z = df["unproc_z"].astype(np.float32).to_numpy()

# Metadata
sample_rate = 100
input_len = len(df)
# output_len = len(df_valid)
# valid_start_idx = df["contains_output"].idxmax()

# Generate header and source strings for input
input_h = f"""#pragma once
#include <math.h>
#include "arm_math.h"

#define {output_name}_INPUT_DURATION_SEC {input_len // sample_rate}
#define {output_name}_INPUT_SAMPLE_RATE_HZ {sample_rate}
#define {output_name}_INPUT_LEN {input_len}

extern const uint32_t  {output_name}_input_len;
extern const float32_t {output_name}_input_x[];
extern const float32_t {output_name}_input_y[];
extern const float32_t {output_name}_input_z[];
"""

input_c = f"""#include "{output_name}_input.h"
const uint32_t {output_name}_input_len = {input_len};

const float32_t {output_name}_input_x[] = {{
{format_array(input_x)}
}};

const float32_t {output_name}_input_y[] = {{
{format_array(input_y)}
}};

const float32_t {output_name}_input_z[] = {{
{format_array(input_z)}
}};
"""

# # Generate header and source strings for output
# output_h = f"""#pragma once
# #include <math.h>
# #include "arm_math.h"

# #define WALKING_CAPTURE_OUTPUT_DURATION_SEC {output_len // sample_rate}
# #define WALKING_CAPTURE_OUTPUT_SAMPLE_RATE_HZ {sample_rate}
# #define WALKING_CAPTURE_OUTPUT_LEN {output_len}
# #define WALKING_CAPTURE_OUTPUT_VALID_START_IDX {valid_start_idx}

# extern const uint32_t walking_capture_output_len;
# extern const float32_t walking_capture_output_x[];
# extern const float32_t walking_capture_output_y[];
# extern const float32_t walking_capture_output_z[];
# """

# output_c = f"""#include "walking_capture_output.h"
# const uint32_t walking_capture_output_len = {output_len};

# const float32_t walking_capture_output_x[] = {{
    # {', '.join(f"{v:.6f}f" for v in output_x)}
# }};

# const float32_t walking_capture_output_y[] = {{
    # {', '.join(f"{v:.6f}f" for v in output_y)}
# }};

# const float32_t walking_capture_output_z[] = {{
    # {', '.join(f"{v:.6f}f" for v in output_z)}
# }};
# """

# Save to .c and .h files
with open(f"{output_dir}/{output_name}_input.h", "w") as f:
    f.write(input_h)
with open(f"{output_dir}/{output_name}_input.c", "w") as f:
    f.write(input_c)
# with open("/mnt/data/walking_capture_output.h", "w") as f:
    # f.write(output_h)
# with open("/mnt/data/walking_capture_output.c", "w") as f:
    # f.write(output_c)
