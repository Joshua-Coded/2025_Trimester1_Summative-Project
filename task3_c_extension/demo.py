#!/usr/bin/env python3
"""
Demo script showcasing the use of stat_module C extension for data analysis.
"""

import stat_module
import random
import time

def generate_random_data(size=10000):
    """Generate a large list of random floating point numbers."""
    return [random.uniform(0, 100) for _ in range(size)]

def demo_c_extension():
    """Demonstrate the use of the C extension with timing comparisons."""
    # Generate sample data
    print("Generating random data...")
    data = generate_random_data()
    print(f"Generated {len(data)} random values between 0 and 100\n")
    
    print("Statistical Analysis using C Extension:")
    print("-" * 40)
    
    # Measure time taken by C extension functions
    start_time = time.time()
    sum_c = stat_module.sum_array(data)
    sum_time = time.time() - start_time
    
    start_time = time.time()
    avg_c = stat_module.avg_array(data)
    avg_time = time.time() - start_time
    
    start_time = time.time()
    std_dev_c = stat_module.std_dev_array(data)
    std_dev_time = time.time() - start_time
    
    start_time = time.time()
    mode_c = stat_module.mode_array(data[:1000])  # Using subset for mode (faster)
    mode_time = time.time() - start_time
    
    start_time = time.time()
    count_c = stat_module.count_array(data)
    count_time = time.time() - start_time
    
    # Print results from C extension
    print(f"Sum of values: {sum_c:.2f} (Time: {sum_time:.6f}s)")
    print(f"Average value: {avg_c:.2f} (Time: {avg_time:.6f}s)")
    print(f"Standard deviation: {std_dev_c:.2f} (Time: {std_dev_time:.6f}s)")
    print(f"Mode value (from first 1000): {mode_c} (Time: {mode_time:.6f}s)")
    print(f"Total number of values: {count_c} (Time: {count_time:.6f}s)")
    
    # Comparison with Python's built-in functions
    print("\nComparison with Python's built-in functions:")
    print("-" * 40)
    
    # Measure time taken by Python built-in functions
    start_time = time.time()
    sum_py = sum(data)
    sum_py_time = time.time() - start_time
    
    start_time = time.time()
    avg_py = sum_py / len(data)
    avg_py_time = time.time() - start_time
    
    start_time = time.time()
    import statistics
    std_dev_py = statistics.stdev(data)
    std_dev_py_time = time.time() - start_time
    
    start_time = time.time()
    from collections import Counter
    data_counts = Counter(data[:1000])  # Using subset for mode (faster)
    py_mode = data_counts.most_common(1)[0][0]
    mode_py_time = time.time() - start_time
    
    start_time = time.time()
    count_py = len(data)
    count_py_time = time.time() - start_time
    
    # Print results from Python built-in functions
    print(f"Sum (Python): {sum_py:.2f} (Time: {sum_py_time:.6f}s)")
    print(f"Average (Python): {avg_py:.2f} (Time: {avg_py_time:.6f}s)")
    print(f"Standard deviation (Python): {std_dev_py:.2f} (Time: {std_dev_py_time:.6f}s)")
    print(f"Mode (Python, from first 1000): {py_mode} (Time: {mode_py_time:.6f}s)")
    print(f"Count (Python): {count_py} (Time: {count_py_time:.6f}s)")
    
    # Performance comparison
    print("\nPerformance Comparison (C Extension vs. Python):")
    print("-" * 40)
    print(f"Sum: C is {sum_py_time / sum_time:.2f}x faster")
    print(f"Average: C is {avg_py_time / avg_time:.2f}x faster")
    print(f"Std Dev: C is {std_dev_py_time / std_dev_time:.2f}x faster")
    print(f"Mode: C is {mode_py_time / mode_time:.2f}x faster")
    print(f"Count: C is {count_py_time / count_time:.2f}x faster")

if __name__ == "__main__":
    demo_c_extension()
