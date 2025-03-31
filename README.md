# Linux Programming Summative Project

## Overview

This repository contains implementations for the Linux Programming Summative Project, which consists of five distinct tasks that demonstrate proficiency in various Linux programming concepts including binary analysis, assembly programming, Python C extensions, multi-threading, and inter-process communication.

## Project Structure

```
linux_programming_project/
├── Makefile                # Main Makefile to build all tasks
├── README.md               # Main project documentation
├── task1_elf_analysis/     # ELF Binary Analysis
│   ├── Makefile            # Task-specific Makefile
│   ├── README.md           # Instructions and analysis report
│   └── binary_file         # The ELF binary to analyze
├── task2_assembly/         # Assembly Name Counter
│   ├── Makefile            # Task-specific Makefile
│   ├── README.md           # Instructions for the assembly program
│   ├── names.txt           # Sample file with random names
│   └── count_names.asm     # Assembly program source code
├── task3_c_extension/      # C Extension for Python
│   ├── Makefile            # Task-specific Makefile
│   ├── README.md           # Instructions for the C extension
│   ├── setup.py            # Python setup script
│   ├── statmodule.c        # C extension source code
│   └── test_extension.py   # Python test script
├── task4_producer_consumer/ # Producer-Consumer Implementation
│   ├── Makefile            # Task-specific Makefile  
│   ├── README.md           # Instructions for the producer-consumer implementation
│   └── producer_consumer.c  # Source code for producer-consumer
└── task5_chat_system/      # Chat System
    ├── Makefile            # Task-specific Makefile
    ├── README.md           # Instructions for the chat system
    ├── server.c            # Server implementation
    ├── client.c            # Client implementation
    └── common.h            # Common header file for both server and client
```

## Building and Running

Each task has its own Makefile for easy compilation. To build all tasks:

```bash
make
```

To build a specific task (e.g., Task 2):

```bash
cd task2_assembly
make
```

To clean compiled files:

```bash
make clean
```

## Detailed Task Descriptions

### Task 1: ELF Binary Analysis 

This task involves analyzing an ELF binary file using tools such as `objdump`, `strace`, and `gdb` to reconstruct its functionality. The analysis includes:

- Disassembling the binary
- Identifying all function calls
- Identifying all memory accesses
- Clarifying the control flow in the program at the assembly level

### Task 2: Assembly Name Counter 

Implementation of an assembly program that:
- Loads a file containing random names (between 30-100 names)
- Counts the number of names in the file
- Outputs the count to the console

### Task 3: C Extension for Python 

Development of a C extension for Python that processes arrays of floating-point numbers, providing the following functionality:

- Sum calculation of all values
- Average calculation
- Standard deviation calculation
- Mode value determination
- Count of total values

### Task 4: Producer-Consumer Model 

Implementation of a multi-threaded producer-consumer model simulating a manufacturing assembly line with the following specifications:

- Producer places items on the assembly line every 2 seconds
- Consumer removes items every 3 seconds
- Consumer only works when items are available
- Queue capacity limited to 10 items
- Producer stops when queue is full and resumes when space is available

### Task 5: Chat System (6 points)

Development of a client-server chat system with the following features:

- Central server that manages 4 clients
- Client authentication via username
- Display of online clients
- Direct messaging between specific clients
- Real-time bidirectional communication
- Message reception only for authenticated clients

## System Requirements

- Linux-based operating system
- GCC compiler
- NASM assembler (for Task 2)
- Python development headers (for Task 3)
- POSIX threads library (for Task 4 and 5)
