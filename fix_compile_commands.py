import json
import re

# Load the compile_commands.json file
with open("compile_commands.json", "r") as f:
    data = json.load(f)

# Function to fix include paths and remove specific flags
def fix_arguments(arguments):
    arguments = [re.sub(r'(?<=-I)\.\./', '', arg) for arg in arguments]
    arguments = [re.sub(r'(?<=-I)\.\./', '', arg) for arg in arguments]
    arguments = [arg for arg in arguments if arg != "-fcyclomatic-complexity"]
    return arguments

# Apply the fix to each entry
for entry in data:
    if "arguments" in entry:
        entry["arguments"] = fix_arguments(entry["arguments"])

# Save the modified file
with open("compile_commands_fixed.json", "w") as f:
    json.dump(data, f, indent=2)

print("Fixed compile_commands.json saved as compile_commands_fixed.json")
