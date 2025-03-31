# Main Makefile for Linux Programming Summative Project

# Define the subdirectories for each task
SUBDIRS = task2_assembly task4_producer_consumer task3_c_extension task5_chat_system

# Default target - build all tasks
all: $(SUBDIRS)

# Rule to make each subdirectory
$(SUBDIRS):
	$(MAKE) -C $@

# Clean all subdirectories
clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

# phony targets
.PHONY: all clean $(SUBDIRS)