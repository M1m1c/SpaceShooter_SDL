#pragma once
#include <cstdint>
struct Node {
    uint32_t value;
    Node* left;
    Node* right;

    Node(uint32_t val) : value(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    BinaryTree() : root_(nullptr) {}

    ~BinaryTree() {
        // TODO: Implement the destructor to free memory
    }

    // Add a new value to the binary tree
    void add(uint32_t val) {
        if (!root_) {
            root_ = new Node(val);
            return;
        }

        Node* current = root_;
        while (true) {
            if (val < current->value) {
                // Left
                if (current->left) {
                    current = current->left;
                }
                else {
                    current->left = new Node(val);
                    break;
                }
            }
            else {
                // Right
                if (current->right) {
                    current = current->right;
                }
                else {
                    current->right = new Node(val);
                    break;
                }
            }
        }
    }

    // Check if a value is in the binary tree
    bool contains(uint32_t val) const {
        Node* current = root_;
        while (current) {
            if (val == current->value) {
                return true;
            }
            else if (val < current->value) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return false;
    }

private:
    Node* root_;
};