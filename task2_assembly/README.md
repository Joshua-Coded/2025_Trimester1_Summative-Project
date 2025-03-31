# Task 2: Assembly Name Counter

## Overview

This program implements a name counter in x86-64 assembly language that reads a file containing names, counts the total number of names present, and displays the count to the console.

## Requirements

- Linux x86-64 system
- NASM assembler (for compilation)
- LD linker

## File Structure

- `count_names.asm`: The assembly source code
- `names.txt`: A sample file containing random names (one name per line)
- `Makefile`: For easy compilation and clean-up

## Implementation Details

The program uses Linux system calls directly to perform the following operations:

1. **File Operations**:
   - `open`: Opens the names.txt file in read-only mode
   - `read`: Reads the contents into a buffer
   - `close`: Closes the file after reading

2. **Text Processing**:
   - Counts the number of newline characters to determine the number of names
   - Handles the edge case when the file doesn't end with a newline

3. **Output**:
   - Converts the count to a string representation
   - Outputs the count to the console using the `write` system call

## System Calls Used

| System Call | Number (RAX) | Purpose |
|-------------|--------------|---------|
| open        | 2            | Open file |
| read        | 0            | Read file content |
| close       | 3            | Close file |
| write       | 1            | Output to console |
| exit        | 60           | Terminate program |

## Compilation and Execution

### Using Makefile

The provided Makefile simplifies the build process:

```bash
# Build the program
make

# Run the program
./count_names

# Clean the compiled files
make clean
```

### Manual Compilation

If you prefer to compile manually:

```bash
# Assemble the source code
nasm -f elf64 count_names.asm -o count_names.o

# Link the object file
ld count_names.o -o count_names

# Run the program
./count_names
```

## Expected Output

After running the program, you should see output similar to:

```
Number of names: 50
```

The actual number will depend on the content of your names.txt file.

## Troubleshooting

- If you get a "Permission denied" error, make sure the executable has the right permissions: `chmod +x count_names`
- If the file isn't found, ensure `names.txt` is in the same directory as the executable
- For buffer overflow errors, increase the buffer size in the assembly code (currently 4KB)