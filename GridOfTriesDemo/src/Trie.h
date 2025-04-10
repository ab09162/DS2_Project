#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>

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
    bool search(const std::string& product);

    // Update a product's popularity.
    bool updatePopularity(const std::string& product, int newPopularity);

    bool updatePrice(const std::string& product, float newPrice);

    // Delete a product from the trie.
    bool remove(const std::string& product);

    // Optional: Display all products with their popularity (for demo)
    void display();
    
private:
    // Helper function for deletion.
    bool removeHelper(TrieNode* node, const std::string& product, int depth);
    
    // Helper function to recursively free memory.
    void freeNode(TrieNode* node);

    // Helper function for display.
    void displayHelper(TrieNode* node, std::string currentWord);
};

#endif  // TRIE_H
