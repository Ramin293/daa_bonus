#pragma once

#include <cstddef>
#include <memory>
#include <utility>

class BinarySearchTree {
public:
    BinarySearchTree() = default;
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;
    BinarySearchTree(BinarySearchTree&&) noexcept = default;
    BinarySearchTree& operator=(BinarySearchTree&&) noexcept = default;

    bool insert(int value) {
        if (!insert(root_, value)) {
            return false;
        }
        ++size_;
        return true;
    }

    bool contains(int value) const {
        const Node* current = root_.get();
        while (current != nullptr) {
            if (value == current->value) {
                return true;
            }
            current = value < current->value ? current->left.get()
                                             : current->right.get();
        }
        return false;
    }

    bool remove(int value) {
        return remove(root_, value);
    }

    std::size_t size() const noexcept {
        return size_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

private:
    struct Node {
        explicit Node(int item) : value(item) {}

        int value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    static bool insert(std::unique_ptr<Node>& node, int value) {
        if (node == nullptr) {
            node = std::make_unique<Node>(value);
            return true;
        }
        if (value == node->value) {
            return false;
        }
        return insert(value < node->value ? node->left : node->right, value);
    }

    bool remove(std::unique_ptr<Node>& node, int value) {
        if (node == nullptr) {
            return false;
        }
        if (value < node->value) {
            return remove(node->left, value);
        }
        if (value > node->value) {
            return remove(node->right, value);
        }

        if (node->left == nullptr) {
            node = std::move(node->right);
        } else if (node->right == nullptr) {
            node = std::move(node->left);
        } else {
            std::unique_ptr<Node>* successor_link = &node->right;
            while ((*successor_link)->left != nullptr) {
                successor_link = &(*successor_link)->left;
            }
            node->value = (*successor_link)->value;
            *successor_link = std::move((*successor_link)->right);
        }
        --size_;
        return true;
    }

    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;
};
