# BST vs. hash table

So I basically wanted to see how a plain binary search tree and a simple hash table compare when they do the exact same thing: store a set of numbers so you can add, find, or delete them. Both store integers, and if you try to add the same number twice, it just ignores it.

The BST is simple and doesn't rebalance itself or do anything fancy. The hash table uses linked lists for collisions and grows when it gets full. That difference matters once you put more stuff into them.

## How to run it

You need C++17 and CMake installed. From the project folder, run:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

On macOS or Linux, run `build/daa_bonus`. On Windows, run `build/Release/daa_bonus.exe`.

To run the benchmark and save a CSV file:

```bash
build/benchmark > benchmark_results.csv
```

Or, in PowerShell:

```powershell
.\build\Release\benchmark.exe > benchmark_results.csv
```

## What I measured

I tested both structures with shuffled sets of 100, 1,000, and 10,000 integers. I fixed the seed so the same random order is used every time. For each size, the code measures how long it takes to add everything, find everything, and delete everything. Search and delete are timed after building the whole tree or table, so the timer only covers those operations.

The benchmark repeats each measurement 15 times and takes the median time per operation in nanoseconds. I ran the whole benchmark three times and used the median of those runs below. You can check [`results/benchmark_results.csv`](results/benchmark_results.csv) for the data.

| Number of values | Operation | BST (ns per op) | Hash table (ns per op) |
| ---: | --- | ---: | ---: |
| 100 | Add | 31.56 | 187.95 |
| 100 | Find | 19.93 | 2.80 |
| 100 | Remove | 19.53 | 14.73 |
| 1,000 | Add | 49.97 | 131.93 |
| 1,000 | Find | 43.81 | 2.94 |
| 1,000 | Remove | 51.07 | 17.33 |
| 10,000 | Add | 190.82 | 214.81 |
| 10,000 | Find | 165.65 | 4.29 |
| 10,000 | Remove | 151.33 | 32.65 |

The hash table destroyed the BST on lookups and was faster at deleting too. Insertion was the weird one, because the BST was faster every time. I think that's because my hash table uses linked lists, which means dynamic memory allocations, and it occasionally has to resize and move stuff around. That slows insertion down.

I ran this on Wandbox with GCC 13.2.0 and the `-O2` flag. Wandbox doesn't say what CPU it uses, so your numbers might be different on your computer. Also, my input was shuffled. If I inserted sorted numbers into this BST, it could turn into a long line and get really slow.

## Big-O stuff

`n` is the number of elements. A BST is quick when it's balanced, but mine doesn't rebalance itself. A hash table is quick when values are spread out, but collisions can slow it down.

| Operation | BST best | BST average | BST worst | Hash table best | Hash table expected | Hash table worst |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Add | O(1) | O(log n) | O(n) | O(1) | O(1) amortized | O(n) |
| Find | O(1) | O(log n) | O(n) | O(1) | O(1) | O(n) |
| Remove | O(1) | O(log n) | O(n) | O(1) | O(1) | O(n) |

The BST averages O(log n) when its shape stays reasonably balanced, which is likely with shuffled input. In the worst case it gets skewed and operations take O(n). Hash table insertion is O(1) amortized because resizing takes O(n), but only happens once in a while. If everything lands in one bucket, search and delete can take O(n) too.

## Files in this project

- `include/bst.hpp` — the BST code.
- `include/hash_table.hpp` — the hash table with linked-list buckets.
- `src/main.cpp` — a small example showing how to use both.
- `src/benchmark.cpp` — the code that measures speed.
