# ELF Binary Analysis Report

## 1. Introduction

This report documents the analysis of the provided ELF binary file "binary_file" (originally named "magic"). The analysis aims to understand its functionality by examining its machine code, function calls, memory accesses, and control flow.

## 2. Binary Information

- **Filename**: magic (renamed to binary_file)
- **File type**: ELF 64-bit LSB pie executable, x86-64
- **Architecture**: x86-64
- **Linkage**: Dynamically linked
- **Interpreter**: /lib64/ld-linux-x86-64.so.2
- **Compilation**: GCC (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
- **Source**: Compiled from "magic.c"
- **Symbols**: Not stripped (symbol information available)

## 3. Disassembly Analysis

### 3.1 Main Function

```assembly
000000000000141d <main>:
    141d: f3 0f 1e fa                  endbr64
    1421: 55                           pushq %rbp
    1422: 48 89 e5                     movq %rsp, %rbp
    1425: 48 83 ec 20                  subq $32, %rsp
    1429: 48 8d 05 22 0c 00 00         leaq 3106(%rip), %rax        # 0x2052 <_IO_stdin_used+0x52>
    1430: 48 89 45 f0                  movq %rax, -16(%rbp)
    1434: 48 8d 05 28 0c 00 00         leaq 3112(%rip), %rax        # 0x2063 <_IO_stdin_used+0x63>
    143b: 48 89 45 f8                  movq %rax, -8(%rbp)
    143f: 48 8b 45 f0                  movq -16(%rbp), %rax
    1443: 48 89 c7                     movq %rax, %rdi
    1446: e8 de fd ff ff               callq 0x1229 <count_names>
    144b: 89 45 ec                     movl %eax, -20(%rbp)
    144e: 83 7d ec ff                  cmpl $-1, -20(%rbp)
    1452: 74 47                        je 0x149b <main+0x7e>
    1454: 8b 45 ec                     movl -20(%rbp), %eax
    1457: 89 c6                        movl %eax, %esi
    1459: 48 8d 05 20 0c 00 00         leaq 3104(%rip), %rax        # 0x2080 <_IO_stdin_used+0x80>
    1460: 48 89 c7                     movq %rax, %rdi
    1463: b8 00 00 00 00               movl $0, %eax
    1468: e8 83 fc ff ff               callq 0x10f0 <.plt.sec+0x30>
```

**Analysis of the Main Function**:
- The function sets up a stack frame and reserves 32 bytes of stack space
- It loads two string pointers from the code segment (likely file paths)
- It calls the `count_names` function with the first string as an argument
- It checks if the return value is -1 (error check)
- If successful, it prints the result using printf

### 3.2 Important Functions

#### Function 1: count_names
**Purpose**: Counts the number of names in a file
- Takes a filename as input
- Returns a count of names or -1 on error

#### Function 2: convert_names_to_uppercase
**Purpose**: Reads names from an input file and writes uppercase versions to an output file
- Called after the count_names operation if successful

#### Function 3: to_uppercase
**Purpose**: Helper function that converts a string to uppercase
- Uses the toupper() function from the C library

## 4. Function Call Analysis

### 4.1 External Function Calls

| Function Name | Purpose |
|---------------|---------|
| fopen | Opens files for reading/writing |
| fgets | Reads lines from file |
| fclose | Closes file handles |
| toupper | Converts characters to uppercase |
| perror | Prints error messages |
| printf | Formats and prints output |

### 4.2 Internal Function Calls

| Function Name | Called From | Purpose |
|---------------|-------------|---------|
| count_names | main | Counts names in the input file |
| convert_names_to_uppercase | main | Converts names to uppercase |
| to_uppercase | convert_names_to_uppercase | Helper function to convert a single string |

## 5. Memory Access Analysis

Based on the disassembly of the main function:

### 5.1 Stack Operations
- Stack frame setup: `pushq %rbp; movq %rsp, %rbp`
- Stack space allocation: `subq $32, %rsp`
- Local variables storage:
  - File paths stored at -16(%rbp) and -8(%rbp)
  - Return value from count_names stored at -20(%rbp)

### 5.2 Data Segment Access
- String constants accessed:
  - Input file path: "data/entries.txt"
  - Output file path: "data/names_uppercase.txt"
  - Format strings for printf

## 6. Control Flow Analysis

### 6.1 Key Decision Points

| Address | Condition | Purpose |
|---------|-----------|---------|
| 0x144e | cmpl $-1, -20(%rbp) | Checks if count_names returned an error |
| 0x1452 | je 0x149b | If error occurred, skip further processing |

### 6.2 Program Logic Flow

1. Main function loads file paths
2. Calls count_names to count names in the input file
3. If count_names succeeds (returns >= 0):
   - Prints the number of names
   - Calls convert_names_to_uppercase
   - Prints confirmation message
4. If count_names fails (returns -1):
   - Skips conversion and exits with error

## 7. Strings Analysis

Key strings reveal the program's functionality:

| String | Purpose |
|--------|---------|
| "data/entries.txt" | Input file path |
| "data/names_uppercase.txt" | Output file path |
| "Number of names in the file: %d" | Format string for printing count |
| "Converted names have been written to %s" | Success message |
| "Error opening file" | Generic error message |
| "Error opening input file" | Specific error message |
| "Error opening output file" | Specific error message |

## 8. Reconstructed Functionality

### 8.1 Program Purpose

The program reads a list of names from a file ("data/entries.txt"), counts them, converts them to uppercase, and writes the uppercase versions to another file ("data/names_uppercase.txt").

### 8.2 Pseudocode

```c
int main() {
    char* input_file = "data/entries.txt";
    char* output_file = "data/names_uppercase.txt";
    
    // Count the names in the input file
    int count = count_names(input_file);
    
    // Check for errors
    if (count == -1) {
        // Error handling (already done in count_names)
        return 1;
    }
    
    // Print the count
    printf("Number of names in the file: %d", count);
    
    // Convert the names to uppercase
    convert_names_to_uppercase(input_file, output_file);
    
    // Print confirmation
    printf("Converted names have been written to %s", output_file);
    
    return 0;
}

int count_names(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    
    int count = 0;
    char line[BUFFER_SIZE];
    
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        count++;
    }
    
    fclose(file);
    return count;
}

void convert_names_to_uppercase(char* input_file, char* output_file) {
    FILE* in_file = fopen(input_file, "r");
    if (in_file == NULL) {
        perror("Error opening input file");
        return;
    }
    
    FILE* out_file = fopen(output_file, "w");
    if (out_file == NULL) {
        perror("Error opening output file");
        fclose(in_file);
        return;
    }
    
    char line[BUFFER_SIZE];
    
    while (fgets(line, BUFFER_SIZE, in_file) != NULL) {
        to_uppercase(line);
        fputs(line, out_file);
    }
    
    fclose(in_file);
    fclose(out_file);
}

void to_uppercase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}
```

### 8.3 Expected Input/Output

- **Input**: A text file ("data/entries.txt") containing names, one per line
- **Output**: 
  1. Console output indicating the number of names counted
  2. A new file ("data/names_uppercase.txt") containing the same names, but in uppercase

## 9. Conclusion

The binary is a simple text processing utility that:
1. Counts the number of names (lines) in a file
2. Converts those names to uppercase
3. Writes the uppercase versions to a new file

The program uses standard C library functions for file I/O and string manipulation. Its control flow is straightforward, with basic error handling for file operations.

## 10. Analysis Commands Used

```bash
# Basic file information
file binary_file

# Disassembly
objdump -d binary_file > output/disassembly.txt

# Extract strings
strings binary_file > output/strings.txt

# Identify function calls
grep -n "call" output/disassembly.txt > output/function_calls.txt

# Extract main function
grep -A 20 "<main>:" output/disassembly.txt > output/main_function.txt
```
