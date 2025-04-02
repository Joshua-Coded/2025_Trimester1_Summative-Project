#!/bin/bash
# Script to build and run the Docker container for ELF binary analysis

# Build the Docker image
echo "Building Docker image for binary analysis..."
docker build -t elf-analysis .

# Run the container with the current directory mounted
echo "Running Docker container..."
docker run -it --rm \
    -v "$(pwd):/analysis" \
    elf-analysis

# Inform the user about how to run the analysis
echo "Inside the container, you can run:"
echo "./analyze_elf.sh binary_file"
echo "strace ./binary_file"
echo "gdb ./binary_file"
