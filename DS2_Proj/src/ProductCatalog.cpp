// #include "ProductCatalog.h"
// #include <algorithm> // For transform, sort
// #include <cctype>    // For tolower
// #include <vector>    // For return types and intermediate storage
// #include <limits>    // Potentially for numeric_limits if needed

// // --- Private Helper: normalizeCategory ---
// // Basic normalization: convert to lowercase. Could be expanded.
// std::string ProductCatalog::normalizeCategory(const std::string& category) {
//     std::string lowerCategory = category;
//     std::transform(lowerCategory.begin(), lowerCategory.end(), lowerCategory.begin(),
//                    [](unsigned char c){ return std::tolower(c); });
//     // Trim whitespace or handle other normalization if needed
//     return lowerCategory;
// }

// // --- Public Method: ensureCategoryExists ---
// void ProductCatalog::ensureCategoryExists(const std::string& category) {
//     std::string normCategory = normalizeCategory(category);
//     // try_emplace will create a default Trie only if the key doesn't exist.
//     // If it already exists, it does nothing.
//     categoryTries.try_emplace(normCategory);
// }

// // --- Public Method: addProduct ---
// void ProductCatalog::addProduct(const std::string& category, const std::string& productName, int productID, double initialScore) {
//     std::string normCategory = normalizeCategory(category);
//     // Accessing with [] will create the Trie if it doesn't exist.
//     // Then, call the insert method on that Trie.
//     categoryTries[normCategory].insert(productName, productID, initialScore);
// }

// // --- Public Method: searchProducts ---
// std::vector<ProductSuggestion> ProductCatalog::searchProducts(const std::string& prefix, const std::string& category, int limit) {
//     std::vector<ProductSuggestion> results;

//     if (!category.empty()) {
//         // --- Case 1: Search within a specific category ---
//         std::string normCategory = normalizeCategory(category);
//         auto it = categoryTries.find(normCategory);
//         if (it != categoryTries.end()) {
//             // Category found, search within its Trie
//             // The Trie's searchPrefix method already handles normalization of the prefix and sorting.
//             results = it->second.searchPrefix(prefix, limit);
//         }
//         // If category not found, results remain empty, which is correct.
//     } else {
//         // --- Case 2: Search across ALL categories ---
//         // Aggregate results from all Tries first.
//         std::vector<ProductSuggestion> allResults;
//         for (auto const& [catName, trie] : categoryTries) {
//              // Call searchPrefix on each Trie. We get potentially sorted lists per category.
//              // Fetch slightly more than needed per category initially to improve chances
//              // of finding top global items, but avoid fetching everything if Tries are huge.
//              // A simple approach is to just fetch 'limit' from each, but could miss top items.
//              // Fetching all and sorting globally is the most accurate, but potentially slower.
//              // Let's go with accuracy: fetch all matching prefix, then sort globally.
//              // (Pass 0 or a very large number as limit to the individual trie search)
//              std::vector<ProductSuggestion> categoryResults = trie.searchPrefix(prefix, 0); // 0 = no limit internally
//              allResults.insert(allResults.end(), categoryResults.begin(), categoryResults.end());
//         }

//         // Sort the combined results globally by popularity
//         std::sort(allResults.begin(), allResults.end(), std::greater<ProductSuggestion>());

//         // Apply the final limit
//         if (limit > 0 && allResults.size() > static_cast<size_t>(limit)) {
//             allResults.resize(limit);
//         }
//         results = allResults;
//     }

//     return results;
// }


// // --- Public Method: updateProductPopularity ---
// bool ProductCatalog::updateProductPopularity(const std::string& category, const std::string& productName, double scoreIncrement) {
//     std::string normCategory = normalizeCategory(category);
//     auto it = categoryTries.find(normCategory);

//     if (it != categoryTries.end()) {
//         // Category found, attempt to update popularity in its Trie
//         return it->second.updatePopularity(productName, scoreIncrement);
//     }

//     return false; // Category not found
// }

// // --- Public Method: applyGlobalDecay ---
// void ProductCatalog::applyGlobalDecay(double decayFactor) {
//     // Iterate through the map and call applyDecay on each Trie object
//     for (auto& [catName, trie] : categoryTries) { // Note: use reference `auto&` to modify the trie
//         trie.applyDecay(decayFactor);
//     }
// }

// // --- Public Method: getCategories ---
// std::vector<std::string> ProductCatalog::getCategories() {
//     std::vector<std::string> categoryNames;
//     categoryNames.reserve(categoryTries.size()); // Reserve space for efficiency
//     for (auto const& [catName, trie] : categoryTries) {
//         categoryNames.push_back(catName); // Push the original category name key
//     }
//     // You might want to sort the category names alphabetically here if needed.
//     // std::sort(categoryNames.begin(), categoryNames.end());
//     return categoryNames;
// }

#include "ProductCatalog.h"
#include <algorithm> // For transform (to lowercase)
#include <cctype>    // For tolower
#include <unordered_map>
#include <vector>
#include <string>
#include <map>
std::chrono::steady_clock::time_point ProductCatalog::lastDecay = std::chrono::steady_clock::now();
void ProductCatalog::decayPopularity() {
    auto now = std::chrono::steady_clock::now();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(now - lastDecay).count();
    if (secs > 0) {
        float factor = std::pow(decayRatePerSec, (float)secs);
        for (auto &kv : popularityMap)
            kv.second *= factor;
        lastDecay = now;
    }
}
void ProductCatalog::increasePopularity(int productID) {
    decayPopularity();
    popularityMap[productID] += 1.0f;
}

// Helper function to normalize category names (convert to lowercase)
std::string ProductCatalog::normalizeCategory(const std::string& category) {
    std::string lowerCategory = category;
    std::transform(lowerCategory.begin(), lowerCategory.end(), lowerCategory.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerCategory;
}


// Adds a product to the specified category and subcategory's Trie
void ProductCatalog::addProduct(const std::string& category, const std::string& subcategory, const std::string& productName, int productID, double initialScore) {
    std::string normCategory = normalizeCategory(category);
    // Insert the product into the corresponding category -> subcategory -> Trie
    categorySubcategoryTries[normCategory][subcategory].insert(productName, productID, initialScore);
}

// Fetches a list of subcategories for a given category
std::vector<std::string> ProductCatalog::getSubcategoriesForCategory(const std::string& category) {
    std::vector<std::string> subcategories;
    std::string normCategory = normalizeCategory(category);

    // Check if category exists in the map
    auto catIt = categorySubcategoryTries.find(normCategory);
    if (catIt != categorySubcategoryTries.end()) {
        // Retrieve subcategories for the category
        for (const auto& subcategory : catIt->second) {
            subcategories.push_back(subcategory.first);
        }
    }

    return subcategories;
}

// Searches for products matching the search query in the specified category and subcategory
// std::vector<ProductSuggestion> ProductCatalog::searchProducts(const std::string& prefix, const std::string& category, const std::string& subcategory, int limit) {
//     std::vector<ProductSuggestion> results;

//     if (!category.empty()) {
//         // Search within a specific category and subcategory
//         std::string normCategory = normalizeCategory(category);
//         auto catIt = categorySubcategoryTries.find(normCategory);
//         if (catIt != categorySubcategoryTries.end()) {
//             // If category exists, search the subcategory Trie
//             auto subIt = catIt->second.find(subcategory);
//             if (subIt != catIt->second.end()) {
//                 // Search the specific subcategory's Trie
//                 results = subIt->second.searchPrefix(prefix, limit);
//             }
//         }
//     } else {
//         // Search across all categories and subcategories
//         for (const auto& [catName, subcats] : categorySubcategoryTries) {
//             for (const auto& [subName, trie] : subcats) {
//                 // Search the Trie for the current subcategory
//                 std::vector<ProductSuggestion> subResults = trie.searchPrefix(prefix, limit);
//                 results.insert(results.end(), subResults.begin(), subResults.end());
//             }
//         }
//     }

//     // Sort results by popularity
//     std::sort(results.begin(), results.end(), std::greater<ProductSuggestion>());

//     // Apply the final limit to the results
//     if (limit > 0 && results.size() > static_cast<size_t>(limit)) {
//         results.resize(limit);
//     }

//     return results;
// }
std::vector<ProductSuggestion> ProductCatalog::searchProducts(const std::string& query,
                                                              const std::string& category,
                                                              const std::string& subcategory,
                                                              int maxResults) {
    std::vector<ProductSuggestion> results;
    decayPopularity();
    if (category.empty()) {
        // Search all categories and subcategories
        for (const auto& catPair : categorySubcategoryTries) {
            for (const auto& subPair : catPair.second) {
                // Access Trie object directly with '.'
                std::vector<ProductSuggestion> subResults = subPair.second.search(query, maxResults);
                results.insert(results.end(), subResults.begin(), subResults.end());
            }
        }
    } else {
        auto catIt = categorySubcategoryTries.find(category);
        if (catIt != categorySubcategoryTries.end()) {
            if (subcategory.empty() || subcategory == "All Subcategories") {
                // Corrected: Use catIt->second instead of undefined subcategoryMap
                for (const auto& subPair : catIt->second) {
                    std::vector<ProductSuggestion> subResults = subPair.second.search(query, maxResults);
                    results.insert(results.end(), subResults.begin(), subResults.end());
                }
            } else {
                auto subIt = catIt->second.find(subcategory);
                if (subIt != catIt->second.end()) {
                    // Corrected: Use subIt->second for the specific subcategory
                    std::vector<ProductSuggestion> subResults = subIt->second.search(query, maxResults);
                    results.insert(results.end(), subResults.begin(), subResults.end());
                }
            }
        }
    }
    for (auto &s : results) {
        auto it = popularityMap.find(s.productID);
        s.popularity = (it != popularityMap.end() ? it->second : 0.0f);
    }
    // Sorting by popularity
    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.popularity > b.popularity;
    });

    // Limit the results if necessary
    if ((int)results.size() > maxResults) {
        results.resize(maxResults);
    }

    return results;
}



// Fetches all categories in the catalog
std::vector<std::string> ProductCatalog::getCategories() {
    std::vector<std::string> categories;
    categories.reserve(categorySubcategoryTries.size());
    for (const auto& [category, _] : categorySubcategoryTries) {
        categories.push_back(category);
    }
    return categories;
}