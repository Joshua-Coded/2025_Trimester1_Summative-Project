# Makefile for stat_module C extension

.PHONY: all clean test demo

all: build

build:
	python3 setup.py build_ext --inplace

test: build
	python3 test_stat_module.py

demo: build
	python3 demo.py

clean:
	rm -rf build/
	rm -f *.so
	rm -f *.o
	rm -f *.c~
