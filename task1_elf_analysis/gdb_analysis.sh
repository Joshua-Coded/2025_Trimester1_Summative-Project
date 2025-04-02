#!/bin/bash
# Script to perform GDB analysis on the binary

# Check if binary filename is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <binary_file>"
    exit 1
fi

BINARY="$1"
OUTPUT_DIR="output"
GDB_SCRIPT="$OUTPUT_DIR/gdb_commands.gdb"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Make binary executable
chmod +x "$BINARY"

echo "=== Creating GDB script for analysis ==="

# Create GDB script
cat > "$GDB_SCRIPT" << EOL
# GDB script for analyzing binary
set pagination off
set disassembly-flavor intel

# Get basic binary information
echo "\n=== Binary Information ===\n"
info files

# List functions
echo "\n=== Functions in Binary ===\n"
info functions

# Disassemble main function
echo "\n=== Main Function Disassembly ===\n"
disassemble main

# Set breakpoints on key functions
echo "\n=== Setting Breakpoints ===\n"
break main
break count_names
info breakpoints

# Run the program
echo "\n=== Running Program ===\n"
run

# Examine stack frame and variables
echo "\n=== Main Function Variables ===\n"
info locals
info args

# Continue to count_names function
echo "\n=== Continuing to count_names ===\n"
continue

# Examine count_names function
echo "\n=== count_names Function Variables ===\n"
info locals
info args

# Examine stack frame
echo "\n=== Stack Frame ===\n"
info frame

# Disassemble the count_names function at runtime
echo "\n=== count_names Disassembly ===\n"
disassemble

# Exit GDB
quit
EOL

echo "=== Running GDB analysis on $BINARY ==="
gdb -x "$GDB_SCRIPT" "$BINARY" > "$OUTPUT_DIR/gdb_analysis.txt"

echo "=== GDB analysis complete ==="
echo "Results saved to $OUTPUT_DIR/gdb_analysis.txt"
