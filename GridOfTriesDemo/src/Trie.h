#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>
#include <vector>

// Define the structure for a node in the Trie.
struct TrieNode {
    bool isEndOfProduct;
    int popularity;
    float price;  

    std::unordered_map<char, TrieNode*> children;

    TrieNode();
};

class Trie {
public:
    TrieNode* root;
    
    Trie();
    ~Trie();

    // Insert a product into the trie.
    void insert(const std::string& product, int initPopularity = 0, float price = 0);

    // Search for a product by exact name.
    TrieNode* search(const std::string& product);

    // Update a product's popularity.
    bool updatePopularity(const std::string& product, int newPopularity);

    bool updatePrice(const std::string& product, float newPrice);

    // Delete a product from the trie.
    bool remove(const std::string& product);

    // Display all products in the trie.
    void display();
    
    // Auto-complete function: returns a vector of suggestions given a prefix.
    std::vector<std::string> autoComplete(const std::string& prefix);

private:
    // Helper function for deletion.
    bool removeHelper(TrieNode* node, const std::string& product, int depth);
    
    // Recursively free memory.
    void freeNode(TrieNode* node);

    // Helper function for display.
    void displayHelper(TrieNode* node, std::string currentWord);

    // Helper function for auto-complete.
    void autoCompleteHelper(TrieNode* node, std::string currentWord, std::vector<std::string>& suggestions);
};

#endif  // TRIE_H
