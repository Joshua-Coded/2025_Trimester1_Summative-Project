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
