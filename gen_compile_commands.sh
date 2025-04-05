#!/bin/zsh

# Step 1: Run fix_build_log.py
echo "Running fix_build_log.py..."
python3 fix_build_log.py || { echo "Failed to run fix_build_log.py"; exit 1 }

# Step 2: Run compiledb (make sure compiledb is installed)
echo "Running compiledb using fixed build log..."
compiledb --overwrite --parse build_log_fixed.txt || { echo "Failed to run compiledb"; exit 1 }

# Step 3: Run fix_compile_commands.py
echo "Running fix_compile_commands.py..."
python3 fix_compile_commands.py || { echo "Failed to run fix_compile_commands.py"; exit 1 }

# Step 4: rename compile_commands_fixed.json and delete extra files
echo "Renaming compile_commands_fixed.json and deleting extra files..."
mv compile_commands_fixed.json compile_commands.json
rm build_log_fixed.txt

echo "All steps completed successfully."
