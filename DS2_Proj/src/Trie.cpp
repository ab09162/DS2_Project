// #include "Trie.h"
// #include <iostream> // Useful for debugging output (optional)
// #include <algorithm> // For std::sort
// #include <vector>    // Required for vector usage
// #include <cctype>   // For tolower

// // Helper function to normalize strings (e.g., convert to lowercase)
// // This helps make searches case-insensitive.
// std::string normalizeString(const std::string& input) {
//     std::string output = input;
//     std::transform(output.begin(), output.end(), output.begin(),
//                    [](unsigned char c){ return std::tolower(c); });
//     // Could add other normalization here, like removing punctuation or collapsing spaces
//     return output;
// }


// // --- Constructor ---
// Trie::Trie() {
//     root = new TrieNode(); // Allocate the root node
// }

// // --- Destructor ---
// Trie::~Trie() {
//     delete root; // Deleting root triggers recursive deletion in TrieNode's destructor
// }

// // --- Private Helper: findNode ---
// // Finds the node corresponding to the end of the given key string.
// // Returns nullptr if the key is not found as a prefix.
// TrieNode* Trie::findNode(const std::string& key) const {
//     TrieNode* current = root;
//     std::string normalizedKey = normalizeString(key); // Normalize the key for lookup

//     for (char ch : normalizedKey) {
//         // Check if the character exists in the children map
//         auto it = current->children.find(ch);
//         if (it == current->children.end()) {
//             // Character not found, prefix doesn't exist
//             return nullptr;
//         }
//         // Move to the child node
//         current = it->second;
//     }
//     return current; // Return the node corresponding to the last character
// }

// // --- Public Method: insert ---
// void Trie::insert(const std::string& productName, int productID, double initialScore) {
//     TrieNode* current = root;
//     std::string normalizedName = normalizeString(productName); // Normalize before inserting

//     for (char ch : normalizedName) {
//         // If character not present, create a new node
//         if (current->children.find(ch) == current->children.end()) {
//             current->children[ch] = new TrieNode();
//         }
//         // Move to the next node
//         current = current->children[ch];
//     }

//     // Mark the end of the word and store product info
//     current->isEndOfWord = true;
//     current->productID = productID;
//     // Only set the initial score if it wasn't already set (or handle updates differently if needed)
//     if (current->popularityScore == 0.0) { // Avoid overwriting existing score if re-inserting
//          current->popularityScore = initialScore > 0 ? initialScore : 1.0; // Ensure positive score
//     }
//     // Note: If a product is inserted multiple times, this currently only sets the score the first time.
//     // You might want different behavior (e.g., update ID/score, throw error, etc.)
// }


// // --- Private Helper: getAllProductsFromNode ---
// // Performs DFS to collect all products (leaf nodes) from a starting node.
// void Trie::getAllProductsFromNode(TrieNode* node, std::string currentPrefix, std::vector<ProductSuggestion>& results) const {
//     if (node == nullptr) {
//         return;
//     }

//     // If this node marks the end of a word, add it to results
//     if (node->isEndOfWord) {
//         results.push_back({currentPrefix, node->popularityScore, node->productID});
//     }

//     // Recursively traverse all children
//     for (auto const& [key, val] : node->children) {
//         getAllProductsFromNode(val, currentPrefix + key, results);
//     }
// }

// // --- Public Method: searchPrefix ---
// std::vector<ProductSuggestion> Trie::searchPrefix(const std::string& prefix, int limit) const {
//     std::vector<ProductSuggestion> results;
//     std::string normalizedPrefix = normalizeString(prefix); // Normalize search prefix

//     // 1. Find the node corresponding to the prefix
//     TrieNode* prefixNode = findNode(normalizedPrefix);

//     // 2. If the prefix node exists, perform DFS from there
//     if (prefixNode) {
//         // We need the original prefix case *if* we wanted to display it,
//         // but the product names stored during DFS might differ in case.
//         // Let's reconstruct names found in the trie based on the traversal path.
//         getAllProductsFromNode(prefixNode, normalizedPrefix, results);
//         // NOTE: The 'name' in ProductSuggestion will currently be lowercase
//         // because we normalize on insert and search. If you want original casing,
//         // you'd need to store it alongside the normalized version or retrieve
//         // the original name from the database using the productID later.
//         // For now, we'll stick with normalized names in suggestions.
//     }

//     // 3. Sort the results by popularity (descending)
//     // Using the overloaded > operator defined in the struct
//     std::sort(results.begin(), results.end(), std::greater<ProductSuggestion>());

//     // 4. Limit the results if necessary
//     if (limit > 0 && results.size() > static_cast<size_t>(limit)) {
//         results.resize(limit);
//     }

//     return results;
// }


// // --- Public Method: updatePopularity ---
// bool Trie::updatePopularity(const std::string& productName, double scoreIncrement) {
//     std::string normalizedName = normalizeString(productName); // Normalize for lookup
//     TrieNode* node = findNode(normalizedName);

//     // Check if the node exists and it's actually the end of a word
//     if (node != nullptr && node->isEndOfWord) {
//         node->popularityScore += scoreIncrement;
//         // Optional: Add a check to ensure score doesn't go below zero
//         if (node->popularityScore < 0) {
//             node->popularityScore = 0;
//         }
//         return true;
//     }
//     return false; // Product not found
// }


// // --- Private Helper: applyDecayRecursive ---
// void Trie::applyDecayRecursive(TrieNode* node, double decayFactor) {
//     if (node == nullptr) {
//         return;
//     }

//     // Apply decay if this node represents a product
//     if (node->isEndOfWord) {
//         node->popularityScore *= decayFactor;
//         // Optional: Could set a floor value (e.g., score doesn't decay below 0.1)
//          if (node->popularityScore < 0.01) { // Example floor
//              // Consider maybe removing very unpopular items or setting to a minimum non-zero?
//              // For now, just letting them decay towards zero.
//          }
//     }

//     // Recursively call on children
//     for (auto const& [key, val] : node->children) {
//         applyDecayRecursive(val, decayFactor);
//     }
// }


// // --- Public Method: applyDecay ---
// void Trie::applyDecay(double decayFactor) {
//     if (decayFactor >= 1.0 || decayFactor < 0.0) {
//         std::cerr << "Warning: Invalid decay factor (" << decayFactor << "). Must be between 0.0 and 1.0." << std::endl;
//         return; // Or throw an exception
//     }
//     applyDecayRecursive(root, decayFactor);
// }
#include "Trie.h"
#include <algorithm>
#include <cctype>


void Trie::insert(const std::string& productName, int productID, double initialScore) {
    TrieNode* current = root;
    std::string normalizedName = productName;
    std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);

    for (char ch : normalizedName) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode();
        }
        current = current->children[ch];
    }

    current->isEndOfWord = true;
    current->popularityScore = initialScore;
    current->productID = productID;
}
std::vector<ProductSuggestion> Trie::search(const std::string& query, int limit) const {
    return searchPrefix(query, limit);
}

void Trie::getAllProductsFromNode(TrieNode* node, std::string currentPrefix, std::vector<ProductSuggestion>& results) const {
    if (node == nullptr) return;

    if (node->isEndOfWord) {
        results.push_back({currentPrefix, node->popularityScore, node->productID});
    }

    for (auto& [ch, child] : node->children) {
        getAllProductsFromNode(child, currentPrefix + ch, results);
    }
}

std::vector<ProductSuggestion> Trie::searchPrefix(const std::string& prefix, int limit) const {
    std::vector<ProductSuggestion> results;
    TrieNode* current = root;
    std::string normalizedPrefix = prefix;
    std::transform(normalizedPrefix.begin(), normalizedPrefix.end(), normalizedPrefix.begin(), ::tolower);

    for (char ch : normalizedPrefix) {
        if (current->children.find(ch) == current->children.end()) {
            return results; // Prefix not found
        }
        current = current->children[ch];
    }

    getAllProductsFromNode(current, normalizedPrefix, results);

    std::sort(results.begin(), results.end(), std::greater<ProductSuggestion>());

    if (limit > 0 && results.size() > static_cast<size_t>(limit)) {
        results.resize(limit);
    }

    return results;

    
        
}

bool Trie::isEmpty() const {
    return root->children.empty();  // If there are no children, the Trie is empty
}