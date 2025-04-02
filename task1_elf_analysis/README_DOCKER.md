# Docker Setup for ELF Binary Analysis

This guide explains how to use Docker to analyze the ELF binary on macOS (or any non-Linux system).

## Setup Instructions

1. **Install Docker** (if not already installed):
   - Download and install from [Docker's website](https://www.docker.com/products/docker-desktop)
   - Verify installation with `docker --version`

2. **Build and run the Docker container**:
   ```bash
   chmod +x build_and_run.sh
   ./build_and_run.sh
   ```

   This script:
   - Builds a Docker image with the necessary analysis tools
   - Runs a container with the current directory mounted
   - Gives you a shell inside the container

3. **Inside the Docker container**, you can run:
   ```bash
   # Make analysis scripts executable
   chmod +x analyze_elf.sh strace_analysis.sh gdb_analysis.sh

   # Run static analysis
   ./analyze_elf.sh binary_file

   # Run strace analysis
   ./strace_analysis.sh binary_file

   # Run GDB analysis
   ./gdb_analysis.sh binary_file
   ```

## Files in this Setup

- **Dockerfile**: Defines the Ubuntu-based container with analysis tools
- **build_and_run.sh**: Script to build and run the Docker container
- **analyze_elf.sh**: Main analysis script for static analysis
- **strace_analysis.sh**: Script for dynamic analysis using strace
- **gdb_analysis.sh**: Script for dynamic analysis using GDB

## Analysis Output

All analysis results are saved in the `output/` directory, which is accessible both inside the container and on your local machine since the directory is mounted.

## Example Workflow

```bash
# On your macOS terminal
./build_and_run.sh

# Inside the Docker container
./analyze_elf.sh binary_file
./strace_analysis.sh binary_file
./gdb_analysis.sh binary_file

# Exit container when done
exit

# Back on macOS, the output directory contains all results
ls output/
```

## Troubleshooting

- **Permission issues**: If you encounter permission problems, run `chmod -R 755 .` to ensure all files have proper permissions
- **Binary not running**: Verify the binary is executable with `chmod +x binary_file`
- **Docker errors**: Make sure Docker Desktop is running on your macOS
