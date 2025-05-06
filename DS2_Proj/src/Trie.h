// #ifndef TRIE_H
// #define TRIE_H

// #include "TrieNode.h" // Include the TrieNode definition
// #include <string>     // For product names and prefixes
// #include <vector>     // To return lists of suggestions
// #include <tuple>      // To hold (productName, popularity, productID) triples
// #include <algorithm>  // For sorting suggestions
// #include <functional> // For std::function if needed (not strictly necessary here yet)
// #include <memory>     // Potentially for smart pointers if we change node management (sticking with raw for now per initial structure)

// // Structure to hold product suggestions found during search
// struct ProductSuggestion {
//     std::string name;
//     double popularity;
//     int productID;

//     // Custom comparator for sorting suggestions in descending order of popularity
//     bool operator>(const ProductSuggestion& other) const {
//         return popularity > other.popularity;
//     }
// };


// class Trie {
// private:
//     TrieNode* root; // The root node of the Trie

//     // Helper function for recursive traversal during search/auto-complete
//     void getAllProductsFromNode(TrieNode* node, std::string currentPrefix, std::vector<ProductSuggestion>& results) const;

//     // Helper function for recursively applying decay
//     void applyDecayRecursive(TrieNode* node, double decayFactor);

//     // Helper function to find the node corresponding to a specific prefix or word
//     TrieNode* findNode(const std::string& key) const;

// public:
//     // Constructor: Initializes the Trie with a root node
//     Trie();

//     // Destructor: Cleans up the memory allocated for the Trie nodes
//     ~Trie();

//     // --- Rule of 5 Compliance ---
//     // Prevent copying/moving to avoid ownership issues with raw pointers.
//     // If these were needed, deep copies or move semantics for `root` would be required.
//     Trie(const Trie&) = delete;
//     Trie& operator=(const Trie&) = delete;
//     Trie(Trie&&) = delete;
//     Trie& operator=(Trie&&) = delete;

//     // Inserts a product name into the Trie.
//     // Associates it with a product ID and an initial popularity score.
//     void insert(const std::string& productName, int productID, double initialScore = 1.0);

//     // Searches for products matching the given prefix.
//     // Returns a list of suggestions, sorted by popularity in descending order.
//     // Limits the number of results returned.
//     std::vector<ProductSuggestion> searchPrefix(const std::string& prefix, int limit = 10) const;

//     // Updates the popularity score of a specific product.
//     // Returns true if the product was found and updated, false otherwise.
//     bool updatePopularity(const std::string& productName, double scoreIncrement);

//     // Applies a decay factor to the popularity score of all products in the Trie.
//     void applyDecay(double decayFactor);

// };

// #endif // TRIE_H
#ifndef TRIE_H
#define TRIE_H

#include <map>
#include <string>
#include <vector>

struct ProductSuggestion {
    std::string name;
    double popularity;
    int productID;

    bool operator>(const ProductSuggestion& other) const {
        return popularity > other.popularity;
    }
};

class TrieNode {
public:
    std::map<char, TrieNode*> children;
    bool isEndOfWord = false;
    double popularityScore = 0.0;
    int productID = -1;

    ~TrieNode() {
        for (auto& [key, child] : children) {
            delete child;
        }
    }
};

class Trie {
private:
    TrieNode* root;

    void getAllProductsFromNode(TrieNode* node, std::string currentPrefix, std::vector<ProductSuggestion>& results) const;

public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { delete root; }
    // std::vector<ProductSuggestion> Trie::search(const std::string& query, int limit) const;
    std::vector<ProductSuggestion> search(const std::string& query, int limit) const;

    void insert(const std::string& productName, int productID, double initialScore = 1.0);
    std::vector<ProductSuggestion> searchPrefix(const std::string& prefix, int limit = 10) const;
    bool isEmpty() const;
};

#endif // TRIE_H