#ifndef TRIE_NODE_H
#define TRIE_NODE_H

#include <map>    // For children mapping (char -> TrieNode*)
#include <string> // Needed for potentially storing product ID as string later, though int for now

// Represents a single node in the Trie
class TrieNode {
public:
    // Using a map for children allows flexibility with characters (a-z, 0-9, -, etc.)
    std::map<char, TrieNode*> children;

    // Flag to indicate if this node marks the end of a complete product name
    bool isEndOfWord;

    // Popularity score for the product ending at this node
    // Only meaningful if isEndOfWord is true
    double popularityScore;

    // Unique identifier for the product (linking back to a database/full product info)
    // Only meaningful if isEndOfWord is true. -1 indicates no product ends here.
    int productID;

    // Constructor
    TrieNode() : isEndOfWord(false), popularityScore(0.0), productID(-1) {}

    // Destructor: Crucial for cleaning up memory.
    // It recursively deletes all child nodes.
    ~TrieNode() {
        for (auto const& [key, val] : children) {
            delete val; // Delete child node
        }
        // No need to clear the map itself, it goes out of scope.
    }

    // --- Deleted Copy/Move Semantics (Rule of 5) ---
    // Prevent accidental copying/moving which would mess up pointer ownership
    // and lead to double deletions or dangling pointers. If copying is needed,
    // a deep copy implementation would be required. For this project,
    // preventing copying is safer and simpler.

    TrieNode(const TrieNode&) = delete;            // No copy constructor
    TrieNode& operator=(const TrieNode&) = delete; // No copy assignment
    TrieNode(TrieNode&&) = delete;                 // No move constructor
    TrieNode& operator=(TrieNode&&) = delete;      // No move assignment

};

#endif // TRIE_NODE_H