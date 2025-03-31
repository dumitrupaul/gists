#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fmt/chrono.h>
#include <fstream>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <print>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class BST {
    struct Node {
        int data{-1};
        unique_ptr<Node> left;
        unique_ptr<Node> right;
    };

    unique_ptr<Node> root;

    static unique_ptr<Node> insertRecursive(unique_ptr<Node> node, int val) {
        if (!node)
            return make_unique<Node>(val);
        if (val < node->data)
            node->left = insertRecursive(std::move(node->left), val);
        else
            node->right = insertRecursive(std::move(node->right), val);
        return node;
    }

    static bool searchRecursive(unique_ptr<Node> const& node, int searchData) {
        if (!node)
            return false;
        println("checked {}", node->data);
        if (node->data == searchData) {
            return true;
        } else if (node->data > searchData) {
            return searchRecursive(node->left, searchData);
        } else {
            return searchRecursive(node->right, searchData);
        }
    }

    static int getHeightRecursive(unique_ptr<Node> const& node) {
        if (!node)
            return 0;

        int left = 1 + getHeightRecursive(node->left);
        int right = 1 + getHeightRecursive(node->right);

        return left >= right ? left : right;
    }

    static void bfsPrint(unique_ptr<Node> const& node) {
        if (!node)
            return;

        if (node->left) {
            println("{}", node->left->data);
        }
        if (node->right) {
            println("{}", node->right->data);
        }
        bfsPrint(node->left);
        bfsPrint(node->right);
    }

    static void preOrderDFSPrint(unique_ptr<Node>& node) {
        if (!node)
            return;

        println("{}", node->data);
        preOrderDFSPrint(node->left);
        preOrderDFSPrint(node->right);
    }

    static unique_ptr<Node>& findMin(unique_ptr<Node>& node) {
        if (!node->left)
            return node;
        return findMin(node->left);
    }

    static unique_ptr<Node> deleteRecursive(unique_ptr<Node> node, int value) {
        if (!node)
            return node;
        if (value < node->data)
            node->left = deleteRecursive(std::move(node->left), value);
        else if (value > node->data)
            node->right = deleteRecursive(std::move(node->right), value);
        else {
            if (!node->right)
                return std::move(node->left);
            if (!node->left)
                return std::move(node->right);
            auto& foundMin{findMin(node->right)};
            node->data = foundMin->data;
            node->right = deleteRecursive(std::move(foundMin), node->data);
        }
        return node;
    }

public:
    int getHeight() {
        return getHeightRecursive(root);
    }

    void insert(int data) {
        root = insertRecursive(std::move(root), data);
    }

    bool search(int data) {
        return searchRecursive(root, data);
    }

    void bfsPrint() {
        bfsPrint(root);
    }

    void preOrderDFSPrint() {
        preOrderDFSPrint(root);
    }

    int findMin() {
        return findMin(root)->data;
    }

    void iterativeBFS() {
        // BONUS: with level printing
        queue<pair<Node*, int>> visited;
        if (!root)
            return;
        visited.push({root.get(), 0});
        int PreviousLevel{-1};
        while (!visited.empty()) {
            auto elem{visited.front()};
            if (PreviousLevel != elem.second) {
                print("\nLevel {}:", elem.second);
                PreviousLevel = elem.second;
            }
            print("{}, ", elem.first->data);
            if (elem.first->left)
                visited.push({elem.first->left.get(), PreviousLevel + 1});
            if (elem.first->right)
                visited.push({elem.first->right.get(), PreviousLevel + 1});
            visited.pop();
        }
        println();
    }

    void deleteValue(int value) {
        root = deleteRecursive(std::move(root), value);
    }
};

int main() {
    static string const INPUT_FILE{"cpp-gist/input/bst.in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    string inputString;
    while (std::getline(inputFile, inputString)) {
    }

    BST tree;

    tree.insert(15);
    tree.insert(9);
    tree.insert(17);
    tree.insert(16);
    tree.insert(20);
    tree.insert(3);
    tree.insert(10);
    tree.insert(29);

    tree.iterativeBFS();

    println("Searching for 10: {}", tree.search(10));
    println("Height: {}", tree.getHeight());
    println("Min: {}", tree.findMin());

    tree.deleteValue(17);
    tree.iterativeBFS();

    println("Height: {}", tree.getHeight());

    return EXIT_SUCCESS;
}