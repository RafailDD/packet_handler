import timeit
import random

# Generate large sample data for benchmarking
# Each sample has a random number of 32-bit words, from 1 to 8, simulating randomized packet sizes
data_samples = [[random.getrandbits(32) for _ in range(random.randint(1, 8))] for _ in range(100000)]

def method_original(data):
    """The original approach using reversed and enumerate."""
    expected_data = 0
    for i, word in enumerate(reversed(data)):
        expected_data |= (word << (i * 32))
    return expected_data

def method_optimized(data):
    """The optimized structural accumulation approach."""
    expected_data = 0
    for word in data:
        expected_data = (expected_data << 32) | word
    return expected_data

def run_original():
    for d in data_samples:
        method_original(d)

def run_optimized():
    for d in data_samples:
        method_optimized(d)

if __name__ == "__main__":
    print("Benchmarking `word` combination bitwise methods...")

    # Run the original method
    time_original = timeit.timeit(run_original, number=10)
    print(f"Original Method:  {time_original:.4f} seconds")

    # Run the optimized method
    time_optimized = timeit.timeit(run_optimized, number=10)
    print(f"Optimized Method: {time_optimized:.4f} seconds")

    improvement = ((time_original - time_optimized) / time_original) * 100
    print(f"Improvement:      {improvement:.2f}% faster")
