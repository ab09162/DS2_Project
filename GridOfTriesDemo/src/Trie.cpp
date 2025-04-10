#include "Trie.h"
#include <iostream>

// Constructor for TrieNode.
TrieNode::TrieNode() : isEndOfProduct(false), popularity(0), price(0.0f) {}

// Constructor for Trie.
Trie::Trie() {
    root = new TrieNode();
}

// Destructor for Trie.
Trie::~Trie() {
    freeNode(root);
}

// Recursively free memory of a node and its children.
void Trie::freeNode(TrieNode* node) {
    if (!node)
        return;
    for (auto& child : node->children) {
        freeNode(child.second);
    }
    delete node;
}

void Trie::insert(const std::string& product, int initPopularity, float price) {
    TrieNode* node = root;
    for (char c : product) {
        if (node->children.find(c) == node->children.end()) {
            node->children[c] = new TrieNode();
        }
        node = node->children[c];
    }
    node->isEndOfProduct = true;
    node->popularity = initPopularity;
    node->price = price;
}


bool Trie::search(const std::string& product) {
    TrieNode* node = root;
    for (char c : product) {
        if (node->children.find(c) == node->children.end())
            return false;
        node = node->children[c];
    }
    return node && node->isEndOfProduct;
}

bool Trie::updatePopularity(const std::string& product, int newPopularity) {
    TrieNode* node = root;
    for (char c : product) {
        if (node->children.find(c) == node->children.end())
            return false;
        node = node->children[c];
    }
    if (node && node->isEndOfProduct) {
        node->popularity = newPopularity;
        return true;
    }
    return false;
}

bool Trie::updatePrice(const std::string& product, float newPrice) {
    TrieNode* node = root;
        for (char c : product) {
        if (node->children.find(c) == node->children.end())
            return false;
        node = node->children[c];
    }
    if (node && node->isEndOfProduct) {
        node->price = newPrice;
        return true;
    }
    return false;
}

bool Trie::remove(const std::string& product) {
    return removeHelper(root, product, 0);
}

// Helper function for recursive deletion.
bool Trie::removeHelper(TrieNode* node, const std::string& product, int depth) {
    if (!node)
        return false;
    if (depth == product.size()) {
        if (!node->isEndOfProduct)
            return false;
        node->isEndOfProduct = false;
        // If no children exist, this node can be deleted.
        return node->children.empty();
    }
    char c = product[depth];
    if (node->children.find(c) == node->children.end())
        return false;
    bool shouldDeleteChild = removeHelper(node->children[c], product, depth + 1);
    if (shouldDeleteChild) {
        delete node->children[c];
        node->children.erase(c);
        // Delete this node if it is no longer the end of another product and has no other children.
        return (!node->isEndOfProduct && node->children.empty());
    }
    return false;
}

// Display all products in the Trie.
void Trie::display() {
    displayHelper(root, "");
}

void Trie::displayHelper(TrieNode* node, std::string currentWord) {
    if (node->isEndOfProduct) {
        std::cout << "Product: " << currentWord 
                  << " | Popularity: " << node->popularity << "%"
                  << " | Price: $" << node->price << std::endl;
    }
    for (auto& child : node->children) {
        displayHelper(child.second, currentWord + child.first);
    }
}

