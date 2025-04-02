#!/usr/bin/env python3
"""
Test script for stat_module C extension.
This demonstrates how to use the statistical functions provided by the extension.
"""

import stat_module

def main():
    # Sample data: array of floating point numbers
    data = [42.5, 30.7, 15.2, 42.5, 30.7, 18.9, 42.5, 55.3, 30.7]
    
    print("Test data:", data)
    print("\nStatistical Analysis using C Extension:")
    print("-" * 40)
    
    # Use the sum_array function
    total = stat_module.sum_array(data)
    print(f"Sum of values: {total:.2f}")
    
    # Use the avg_array function
    average = stat_module.avg_array(data)
    print(f"Average value: {average:.2f}")
    
    # Use the std_dev_array function
    std_dev = stat_module.std_dev_array(data)
    print(f"Standard deviation: {std_dev:.2f}")
    
    # Use the mode_array function
    mode = stat_module.mode_array(data)
    print(f"Mode value (most frequent): {mode}")
    
    # Use the count_array function
    count = stat_module.count_array(data)
    print(f"Total number of values: {count}")
    
    # Verification with Python's built-in functions
    print("\nVerification with Python's built-in functions:")
    print("-" * 40)
    
    import statistics
    
    print(f"Sum (Python): {sum(data):.2f}")
    print(f"Average (Python): {sum(data)/len(data):.2f}")
    print(f"Standard deviation (Python): {statistics.stdev(data):.2f}")
    
    # Calculate mode manually for verification
    from collections import Counter
    data_counts = Counter(data)
    py_mode = data_counts.most_common(1)[0][0]
    print(f"Mode (Python): {py_mode}")
    
    print(f"Count (Python): {len(data)}")


if __name__ == "__main__":
    main()
