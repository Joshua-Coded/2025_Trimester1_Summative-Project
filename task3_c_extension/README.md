# Task 3: C Extension for Python (Statistical Functions)

This project implements a C extension for Python that provides statistical functions for arrays of floating-point numbers.

## Features

The extension (`stat_module`) provides the following functions:

1. `sum_array()` - Calculates the sum of all values in the array
2. `avg_array()` - Calculates the average (mean) value of the array
3. `std_dev_array()` - Calculates the standard deviation of the values in the array
4. `mode_array()` - Finds the mode (most frequent value) in the array
5. `count_array()` - Counts the total number of values in the array

## Building the Extension

To build the C extension, follow these steps:

1. Make sure you have Python development headers installed:
   ```
   sudo apt-get install python3-dev  # For Debian/Ubuntu
   ```

2. Build the extension using the Makefile:
   ```
   make
   ```
   
   Or manually:
   ```
   python3 setup.py build_ext --inplace
   ```

This will compile the C code and create a shared object file (e.g., `stat_module.cpython-3x-x86_64-linux-gnu.so`) that can be imported in Python.

## Usage

After building the extension, you can use it in your Python code:

```python
import stat_module

# Create an array of floating-point numbers
data = [42.5, 30.7, 15.2, 42.5, 30.7, 18.9, 42.5, 55.3, 30.7]

# Use the functions provided by the extension
total = stat_module.sum_array(data)
average = stat_module.avg_array(data)
std_dev = stat_module.std_dev_array(data)
mode = stat_module.mode_array(data)
count = stat_module.count_array(data)

# Print the results
print(f"Sum: {total}")
print(f"Average: {average}")
print(f"Standard deviation: {std_dev}")
print(f"Mode: {mode}")
print(f"Count: {count}")
```

## Testing and Demo

Two scripts are provided to demonstrate the usage of the extension:

1. `test_stat_module.py` - Basic test that verifies the correctness of the C extension functions by comparing with Python's built-in functions.
2. `demo.py` - More comprehensive demonstration that includes performance comparisons between the C extension and Python's built-in functions.

Run the test script:
```
make test
```

Run the demo script:
```
make demo
```

## Implementation Details

- The extension is written in C and uses the Python C API.
- It properly handles Python objects and reference counting.
- Error checking is implemented to ensure the input is a list of numeric values.
- The extension is compatible with Python 3.x.

## File Structure

- `stat_module.c` - C source code for the extension
- `setup.py` - Build script for the extension
- `test_stat_module.py` - Test script to verify correctness
- `demo.py` - Demo script with performance comparisons
- `Makefile` - Automation for building and testing
- `README.md` - This documentation file
