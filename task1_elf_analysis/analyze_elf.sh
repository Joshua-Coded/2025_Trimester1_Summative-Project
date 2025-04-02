#!/bin/bash
# ELF Binary Analysis Script - Focused version

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
RESET='\033[0m'

# Check if a filename was provided
if [ "$#" -ne 1 ]; then
    echo -e "${RED}Usage: $0 <binary_file>${RESET}"
    exit 1
fi

BINARY="$1"
OUTPUT_DIR="output"

# Check if the binary exists
if [ ! -f "$BINARY" ]; then
    echo -e "${RED}Error: File '$BINARY' not found.${RESET}"
    exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

echo -e "${GREEN}===== Analyzing ELF Binary: $BINARY =====${RESET}"

# Step 1: Basic file information and verification
echo -e "${BLUE}[1/6] Gathering basic file information...${RESET}"
file "$BINARY" > "$OUTPUT_DIR/file_info.txt"
cat "$OUTPUT_DIR/file_info.txt"

# Check if it's an ELF file
if ! file "$BINARY" | grep -q "ELF"; then
    echo -e "${RED}Warning: File '$BINARY' may not be an ELF binary.${RESET}"
    echo -e "${YELLOW}Continuing analysis anyway...${RESET}"
fi

# Step 2: Disassembly - Key requirement
echo -e "\n${BLUE}[2/6] Disassembling binary...${RESET}"
objdump -d "$BINARY" > "$OUTPUT_DIR/disassembly.txt"
echo -e "${GREEN}✓ Disassembly saved to $OUTPUT_DIR/disassembly.txt${RESET}"

# Look for main function
echo -e "${YELLOW}Locating main function...${RESET}"
grep -A 20 "<main>:" "$OUTPUT_DIR/disassembly.txt" > "$OUTPUT_DIR/main_function.txt"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Found main function. First 20 lines saved to $OUTPUT_DIR/main_function.txt${RESET}"
else
    echo -e "${YELLOW}Main function not found with standard name. Will need manual inspection.${RESET}"
fi

# Step 3: Function call analysis - Key requirement
echo -e "\n${BLUE}[3/6] Identifying function calls...${RESET}"
grep -n "call" "$OUTPUT_DIR/disassembly.txt" > "$OUTPUT_DIR/function_calls.txt"
CALL_COUNT=$(grep -c "call" "$OUTPUT_DIR/function_calls.txt")
echo -e "${GREEN}✓ Found $CALL_COUNT function calls. Details saved to $OUTPUT_DIR/function_calls.txt${RESET}"

# Step 4: Memory access analysis - Key requirement
echo -e "\n${BLUE}[4/6] Identifying memory accesses...${RESET}"
grep -n -E '\[|ptr|mov |lea ' "$OUTPUT_DIR/disassembly.txt" > "$OUTPUT_DIR/memory_accesses.txt"
MEM_ACCESS_COUNT=$(wc -l < "$OUTPUT_DIR/memory_accesses.txt")
echo -e "${GREEN}✓ Found $MEM_ACCESS_COUNT memory access instructions. Details saved to $OUTPUT_DIR/memory_accesses.txt${RESET}"

# Step 5: Control flow analysis - Key requirement
echo -e "\n${BLUE}[5/6] Analyzing control flow...${RESET}"
grep -n -E 'j[a-z]{1,2} ' "$OUTPUT_DIR/disassembly.txt" > "$OUTPUT_DIR/control_flow_jumps.txt"
JUMPS_COUNT=$(wc -l < "$OUTPUT_DIR/control_flow_jumps.txt")
echo -e "${GREEN}✓ Found $JUMPS_COUNT jump instructions (conditionals/branches). Details saved to $OUTPUT_DIR/control_flow_jumps.txt${RESET}"

# Additional information for understanding the binary
echo -e "\n${BLUE}[6/6] Gathering additional helpful information...${RESET}"

# Symbol table
objdump -t "$BINARY" > "$OUTPUT_DIR/symbol_table.txt"
echo -e "${GREEN}✓ Symbol table saved to $OUTPUT_DIR/symbol_table.txt${RESET}"

# Strings that might give clues about functionality
strings "$BINARY" > "$OUTPUT_DIR/strings.txt"
echo -e "${GREEN}✓ Strings extracted to $OUTPUT_DIR/strings.txt${RESET}"

# Dynamic analysis hints
echo -e "\n${YELLOW}===== Static Analysis Complete =====${RESET}"
echo -e "${YELLOW}Next Steps for Dynamic Analysis:${RESET}"

echo -e "${GREEN}To trace system calls:${RESET}"
echo -e "strace -o $OUTPUT_DIR/strace_output.txt ./$BINARY [arguments]"

echo -e "\n${GREEN}To debug with GDB:${RESET}"
echo -e "gdb $BINARY"
echo -e "  (gdb) set disassembly-flavor intel"
echo -e "  (gdb) disassemble main"
echo -e "  (gdb) break main"
echo -e "  (gdb) run [arguments]"

echo -e "\n${YELLOW}==== All analysis files saved to the '$OUTPUT_DIR' directory ====${RESET}"
