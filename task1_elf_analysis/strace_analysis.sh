#!/bin/bash
# Script to perform strace analysis on the binary

# Check if binary filename is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <binary_file>"
    exit 1
fi

BINARY="$1"
OUTPUT_DIR="output"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Make binary executable
chmod +x "$BINARY"

echo "=== Running strace analysis on $BINARY ==="

# Basic strace (all system calls)
echo "Performing basic strace analysis..."
strace -o "$OUTPUT_DIR/strace_basic.txt" ./"$BINARY"

# Detailed strace with timestamps
echo "Performing detailed strace analysis with timestamps..."
strace -tt -o "$OUTPUT_DIR/strace_detailed.txt" ./"$BINARY"

# Strace with file operations only
echo "Analyzing file operations..."
strace -e trace=file -o "$OUTPUT_DIR/strace_file_ops.txt" ./"$BINARY"

# Strace with system call statistics
echo "Generating system call statistics..."
strace -c -o "$OUTPUT_DIR/strace_stats.txt" ./"$BINARY"

echo "=== Strace analysis complete ==="
echo "Results saved in the $OUTPUT_DIR directory"
