# Makefile for Producer-Consumer Implementation (Task 4)

# Program name
TARGET = producer_consumer

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lpthread

# Source files
SRC = $(TARGET).c

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET)

# phony targets
.PHONY: all run clean