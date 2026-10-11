#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "bst.hpp"
#include "hash_table.hpp"

namespace {

using Clock = std::chrono::steady_clock;
constexpr int trials = 15;
volatile std::size_t benchmark_sink = 0;

double median(std::vector<double> values) {
    std::sort(values.begin(), values.end());
    return values[values.size() / 2];
}

template <typename Structure>
void fill(Structure& structure, const std::vector<int>& values) {
    for (int value : values) {
        structure.insert(value);
    }
}

template <typename Structure>
void measure(const std::string& name, const std::vector<int>& values) {
    const std::size_t count = values.size();
    std::vector<double> insertion_times;
    std::vector<double> search_times;
    std::vector<double> deletion_times;
    insertion_times.reserve(trials);
    search_times.reserve(trials);
    deletion_times.reserve(trials);

    for (int trial = 0; trial < trials; ++trial) {
        Structure structure;
        std::size_t inserted = 0;
        const auto insert_start = Clock::now();
        for (int value : values) {
            inserted += structure.insert(value);
        }
        const auto insert_end = Clock::now();
        benchmark_sink += inserted;
        insertion_times.push_back(
            std::chrono::duration<double, std::nano>(insert_end - insert_start).count() /
            count);
    }

    const std::size_t search_repetitions = std::max<std::size_t>(1, 100000 / count);
    for (int trial = 0; trial < trials; ++trial) {
        Structure structure;
        fill(structure, values);
        std::size_t found = 0;
        const auto search_start = Clock::now();
        for (std::size_t repetition = 0; repetition < search_repetitions; ++repetition) {
            for (int value : values) {
                found += structure.contains(value);
            }
        }
        const auto search_end = Clock::now();
        benchmark_sink += found;
        const auto operations = count * search_repetitions;
        search_times.push_back(
            std::chrono::duration<double, std::nano>(search_end - search_start).count() /
            operations);
    }

    for (int trial = 0; trial < trials; ++trial) {
        Structure structure;
        fill(structure, values);
        std::size_t removed = 0;
        const auto remove_start = Clock::now();
        for (int value : values) {
            removed += structure.remove(value);
        }
        const auto remove_end = Clock::now();
        benchmark_sink += removed;
        deletion_times.push_back(
            std::chrono::duration<double, std::nano>(remove_end - remove_start).count() /
            count);
    }

    std::cout << count << ',' << name << ",insert," << median(insertion_times) << '\n'
              << count << ',' << name << ",search," << median(search_times) << '\n'
              << count << ',' << name << ",delete," << median(deletion_times) << '\n';
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(2)
              << "size,structure,operation,ns_per_operation\n";
    for (std::size_t count : {100U, 1000U, 10000U}) {
        std::vector<int> values(count);
        std::iota(values.begin(), values.end(), 0);
        std::mt19937 generator(20261011);
        std::shuffle(values.begin(), values.end(), generator);

        measure<BinarySearchTree>("BST", values);
        measure<HashTable>("HashTable", values);
    }

    return benchmark_sink == 0 ? 1 : 0;
}
