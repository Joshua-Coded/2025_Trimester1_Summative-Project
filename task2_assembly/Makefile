# Simple Makefile for Assembly Name Counter

count_names: count_names.o
	ld -o count_names count_names.o

count_names.o: count_names.asm
	nasm -f elf64 count_names.asm -o count_names.o

clean:
	rm -f count_names count_names.o

.PHONY: clean
