input_path = "build_log.txt"
output_path = "build_log_fixed.txt"

prefix_to_match = 'arm-none-eabi-gcc "../'
replacement_prefix = 'arm-none-eabi-gcc "/Users/richardmatthews/Documents/Programming/ST-AI/HAR-Project/HAR/'

with open(input_path, "r") as infile, open(output_path, "w") as outfile:
    for line in infile:
        if line.startswith(prefix_to_match):
            line = line.replace(prefix_to_match, replacement_prefix, 1)
        outfile.write(line)

print(f"Updated lines written to {output_path}")
