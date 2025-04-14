#ifndef PRODUCT_CATALOG_H
#define PRODUCT_CATALOG_H

#include "Trie.h"        // We need the Trie class definition
#include <string>        // For category names, product names, prefixes
#include <vector>        // For returning search results
#include <unordered_map> // To map category names to Tries (efficient lookup)
#include <map>           // Needed if we want sorted iteration, unordered_map is generally faster for lookups.
#include <memory>        // For potentially using smart pointers if desired later

// ProductCatalog manages multiple Tries, one for each product category.
class ProductCatalog {
private:
    // Using an unordered_map for potentially faster average-case lookups by category name.
    // It maps: Category Name (string) -> Trie object for that category.
    // Storing Trie objects directly (not pointers) means the map manages their lifetime.
    // Trie's destructor will handle cleaning up its nodes.
    std::unordered_map<std::string, Trie> categoryTries;

    // Helper to normalize category names (optional, but good practice)
    std::string normalizeCategory(const std::string& category);

public:
    // Constructor
    ProductCatalog() = default; // Default constructor is sufficient

    // --- Rule of 5 Compliance ---
    // We delete copy/move semantics because copying a potentially large catalog
    // of Tries could be very expensive and likely unintentional.
    ProductCatalog(const ProductCatalog&) = delete;
    ProductCatalog& operator=(const ProductCatalog&) = delete;
    ProductCatalog(ProductCatalog&&) = delete;
    ProductCatalog& operator=(ProductCatalog&&) = delete;

    // Adds a product to the specified category's Trie.
    // If the category doesn't exist, it will be created automatically.
    void addProduct(const std::string& category, const std::string& productName, int productID, double initialScore = 1.0);

    // Searches for product suggestions based on a prefix.
    // If 'category' is provided and not empty, searches only within that category.
    // If 'category' is empty, searches across ALL categories, aggregates results,
    // sorts them by popularity, and applies the limit.
    // Returns suggestions sorted by popularity descending.
    std::vector<ProductSuggestion> searchProducts(const std::string& prefix, const std::string& category = "", int limit = 10);

    // Updates the popularity of a specific product within a given category.
    // Returns true if the product was found and updated in that category, false otherwise.
    bool updateProductPopularity(const std::string& category, const std::string& productName, double scoreIncrement);

    // Applies the popularity decay factor to all products in all categories.
    void applyGlobalDecay(double decayFactor);

    // Explicitly ensures a category Trie exists. Not strictly necessary if
    // addProduct creates categories, but can be useful for initialization.
    void ensureCategoryExists(const std::string& category);

    // Lists all available category names.
    std::vector<std::string> getCategories();
};

#endif // PRODUCT_CATALOG_H