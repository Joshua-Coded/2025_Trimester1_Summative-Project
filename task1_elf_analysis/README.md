# Task 1: ELF Binary Analysis

This directory contains tools and resources for analyzing an ELF binary file as required by Question 1 of the assignment.

## Current Status

The analysis is currently facing an issue: the downloaded file (`binary_file`) is not an ELF binary but an HTML document. This likely occurred because the GitHub link provided redirects to the GitHub web interface rather than providing direct access to the raw binary file.

## Getting the Correct Binary

To get the actual binary file, try one of these approaches:

1. **Use the raw GitHub URL**:
   ```bash
   wget https://raw.githubusercontent.com/t4gatera/Linux_Summative_J25/main/magic -O binary_file
   ```

2. **Clone the entire repository**:
   ```bash
   git clone https://github.com/t4gatera/Linux_Summative_J25.git
   cp Linux_Summative_J25/magic ./binary_file
   ```

3. **Check your course materials**: The binary might be available on your learning management system (Canvas, Moodle, etc.)

After getting the correct binary file, make sure it has executable permissions:
```bash
chmod +x binary_file
```

## Tools and Files Included

### Analysis Tools
- `analyze_elf.sh`: Main analysis script that performs:
  - Disassembly of the binary
  - Identification of function calls
  - Identification of memory accesses
  - Analysis of control flow
  - Extraction of strings and symbols

### Documentation
- `binary_analysis.md`: Template for documenting your analysis findings
- `strace_guide.md`: Guide for using the strace tool for dynamic analysis
- `gdb_guide.md`: Guide for using GDB for debugging and dynamic analysis

### Output Directory
- The `output/` directory contains all the analysis results

## Using the Analysis Tools

1. **Run the static analysis script**:
   ```bash
   ./analyze_elf.sh binary_file
   ```

2. **Perform dynamic analysis with strace**:
   ```bash
   strace -o output/strace_output.txt ./binary_file
   ```

3. **Debug with GDB**:
   ```bash
   gdb binary_file
   ```

4. **Document your findings** in the `binary_analysis.md` file:
   - Fill in the disassembly section with key functions
   - Document all identified function calls
   - Document memory access patterns
   - Explain the control flow based on jumps and branches
   - Reconstruct the overall functionality of the program

## Requirements Checklist

Your completed analysis should address:

- [ ] Disassembly of the binary
- [ ] Identification of all function calls
- [ ] Identification of all memory accesses
- [ ] Clarification of the control flow in the program (assembly)

## Troubleshooting

If you continue to have issues with the binary file:

1. **Verify it's an ELF file**:
   ```bash
   file binary_file
   ```
   It should show something like "ELF 64-bit LSB executable" (or similar)

2. **Check for permissions**:
   ```bash
   chmod +x binary_file
   ```

3. **Try running it directly**:
   ```bash
   ./binary_file
   ```

4. **If on macOS**: Note that ELF binaries are Linux executables and won't run natively on macOS. You may need to:
   - Use a Linux virtual machine
   - Use a container (Docker)
   - Set up a Linux environment (WSL on Windows)
