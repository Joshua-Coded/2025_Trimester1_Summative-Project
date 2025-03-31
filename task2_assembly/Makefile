# Makefile for Assembly Name Counter (Task 2)

# Program name
TARGET = count_names

# Assembler and Linker
AS = nasm
LD = ld

# Flags
ASFLAGS = -f elf64
LDFLAGS = 

# Object files
OBJS = $(TARGET).o

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Compile assembly to object file
$(TARGET).o: $(TARGET).asm
	$(AS) $(ASFLAGS) -o $@ $

# Generate a test names file if it doesn't exist
names.txt:
	@echo "Generating sample names.txt file..."
	@for i in {1..50}; do \
		echo "Name$$i" >> $@; \
	done

# Run the program
run: $(TARGET) names.txt
	./$(TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET) $(OBJS)

# Clean all generated files
distclean: clean
	rm -f names.txt

# phony targets
.PHONY: all run clean distclean