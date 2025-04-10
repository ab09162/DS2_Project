#ifndef GRID_OF_TRIES_H
#define GRID_OF_TRIES_H

#include "Trie.h"
#include <unordered_map>
#include <string>

class GridOfTries {
public:
    // Map category name to its trie.
    std::unordered_map<std::string, Trie*> categoryTries;

    GridOfTries();
    ~GridOfTries();

    // Insert a product into the given category.
    void insertProduct(const std::string& category, const std::string& product, int initPopularity, float price);

    // Search for a product in a specific category.
    bool searchProduct(const std::string& category, const std::string& product);

    // Update a product's popularity in a category.
    bool updateProductPopularity(const std::string& category, const std::string& product, int newPopularity);

    // Update a product's price in a category.
    bool updateProductPrice(const std::string& category, const std::string& product, float newPrice);

    // Delete a product from a category.
    bool deleteProduct(const std::string& category, const std::string& product);

    // Display all products in a given category.
    void displayCategory(const std::string& category);
};

#endif  // GRID_OF_TRIES_H
