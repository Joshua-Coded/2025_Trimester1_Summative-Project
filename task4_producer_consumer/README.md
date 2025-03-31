# Task 4: Producer-Consumer Implementation

## Overview

This program implements a multi-threaded producer-consumer model that simulates a manufacturing assembly line. It demonstrates thread synchronization, mutex locks, condition variables, and queue management in a concurrent programming environment.

## Requirements

- Linux system
- GCC compiler
- POSIX threads library (pthread)

## Features

- Producer adds items to the assembly line with a 2-second delay
- Consumer removes items from the assembly line with a 3-second delay
- Queue has a maximum capacity of 10 items
- Producer stops when the queue is full and resumes when space becomes available
- Consumer only works when there are items in the queue
- Visual representation of the queue state on the console

## File Structure

- `producer_consumer.c`: Source code implementing the producer-consumer model
- `Makefile`: For easy compilation and execution

## Implementation Details

### Data Structures

- `queue_t`: A structure containing:
  - Array of items
  - Current count of items
  - Mutex for thread synchronization
  - Condition variables for queue not full/empty states

### Threads

- **Producer Thread**: Adds items to the queue with a 2-second delay
- **Consumer Thread**: Removes items with a 3-second delay

### Synchronization

- **Mutex**: Ensures exclusive access to the shared queue
- **Condition Variables**:
  - `not_full`: Signaled when an item is removed, allowing producer to add
  - `not_empty`: Signaled when an item is added, allowing consumer to remove

## Compilation and Execution

### Using Makefile

```bash
# Build the program
make

# Run the program
make run
# or
./producer_consumer

# Clean the compiled files
make clean
```

### Manual Compilation

```bash
# Compile with pthread linking
gcc -o producer_consumer producer_consumer.c -lpthread

# Run the program
./producer_consumer
```

## Expected Output

The program provides real-time feedback on the console:

```
Starting Producer-Consumer Simulation
Producer adds items every 2 seconds
Consumer removes items every 3 seconds
Maximum queue size: 10 items

Producer: Item 1 added to the assembly line
[█| | | | | | | | | ] (1/10 items)
...
Consumer: Item 1 removed from the assembly line
[ | | | | | | | | | ] (0/10 items)
...
```

### Termination

The program runs indefinitely. To terminate, press `Ctrl+C` in the terminal.

## Troubleshooting

- If compilation fails with "undefined reference to pthread", ensure you're linking against the pthread library (`-lpthread`)
- If the program seems stuck, check for potential deadlocks in the mutex handling
- For visual display issues, ensure your terminal supports UTF-8 characters