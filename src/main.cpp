#include <iostream>

#include "bst.hpp"
#include "hash_table.hpp"

int main() {
    BinarySearchTree bst;
    HashTable hash_table;

    for (int value : {8, 3, 10, 1, 6, 14, 4, 7, 13}) {
        bst.insert(value);
        hash_table.insert(value);
    }

    std::cout << "BST contains 6: " << std::boolalpha << bst.contains(6) << '\n';
    std::cout << "Hash table contains 6: " << hash_table.contains(6) << '\n';

    bst.remove(3);
    hash_table.remove(3);
    std::cout << "After removing 3, BST contains it: " << bst.contains(3) << '\n';
    std::cout << "After removing 3, hash table contains it: "
              << hash_table.contains(3) << '\n';
}
