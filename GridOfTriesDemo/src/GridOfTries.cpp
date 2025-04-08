#include "GridOfTries.h"
#include <iostream>

GridOfTries::GridOfTries() {
    // Initially, the grid is empty.
}

GridOfTries::~GridOfTries() {
    // Free all allocated tries.
    for (auto& pair : categoryTries) {
        delete pair.second;
    }
}

void GridOfTries::insertProduct(const std::string& category, const std::string& product, int initPopularity) {
    // If the category doesn't exist, create a new Trie.
    if (categoryTries.find(category) == categoryTries.end()) {
        categoryTries[category] = new Trie();
    }
    categoryTries[category]->insert(product, initPopularity);
}

bool GridOfTries::searchProduct(const std::string& category, const std::string& product) {
    if (categoryTries.find(category) == categoryTries.end())
        return false;
    return categoryTries[category]->search(product);
}

bool GridOfTries::updateProductPopularity(const std::string& category, const std::string& product, int newPopularity) {
    if (categoryTries.find(category) == categoryTries.end())
        return false;
    return categoryTries[category]->updatePopularity(product, newPopularity);
}

bool GridOfTries::deleteProduct(const std::string& category, const std::string& product) {
    if (categoryTries.find(category) == categoryTries.end())
        return false;
    return categoryTries[category]->remove(product);
}

void GridOfTries::displayCategory(const std::string& category) {
    if (categoryTries.find(category) == categoryTries.end()) {
        std::cout << "Category '" << category << "' not found." << std::endl;
        return;
    }
    std::cout << "Displaying products in category: " << category << std::endl;
    categoryTries[category]->display();
}
