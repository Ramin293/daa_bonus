#pragma once

#include <cstddef>
#include <functional>
#include <list>
#include <vector>

class HashTable {
public:
    explicit HashTable(std::size_t initial_capacity = 8)
        : buckets_(initial_capacity == 0 ? 1 : initial_capacity) {}

    bool insert(int value) {
        auto& bucket = buckets_[index(value)];
        for (int item : bucket) {
            if (item == value) {
                return false;
            }
        }

        bucket.push_back(value);
        ++size_;
        if (load_factor() > max_load_factor_) {
            rehash(buckets_.size() * 2);
        }
        return true;
    }

    bool contains(int value) const {
        const auto& bucket = buckets_[index(value)];
        for (int item : bucket) {
            if (item == value) {
                return true;
            }
        }
        return false;
    }

    bool remove(int value) {
        auto& bucket = buckets_[index(value)];
        for (auto item = bucket.begin(); item != bucket.end(); ++item) {
            if (*item == value) {
                bucket.erase(item);
                --size_;
                return true;
            }
        }
        return false;
    }

    std::size_t size() const noexcept {
        return size_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    double load_factor() const noexcept {
        return static_cast<double>(size_) / buckets_.size();
    }

private:
    static constexpr double max_load_factor_ = 0.75;

    std::size_t index(int value) const {
        return std::hash<int>{}(value) % buckets_.size();
    }

    void rehash(std::size_t new_capacity) {
        std::vector<std::list<int>> new_buckets(new_capacity);
        for (const auto& bucket : buckets_) {
            for (int value : bucket) {
                new_buckets[std::hash<int>{}(value) % new_capacity].push_back(value);
            }
        }
        buckets_.swap(new_buckets);
    }

    std::vector<std::list<int>> buckets_;
    std::size_t size_ = 0;
};
