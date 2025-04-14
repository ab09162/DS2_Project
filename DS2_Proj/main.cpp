#include "ProductCatalog.h" // Include the main catalog header
#include <iostream>         // For console output (cout)
#include <vector>           // For working with search results
#include <iomanip>          // For formatting output (setw, fixed, setprecision)

// Helper function to print search suggestions neatly
void printSuggestions(const std::string& prefix, const std::string& category, const std::vector<ProductSuggestion>& suggestions) {
    std::cout << "--- Search Results for prefix '" << prefix << "'";
    if (!category.empty()) {
        std::cout << " in category '" << category << "'";
    } else {
        std::cout << " across all categories";
    }
    std::cout << " ---" << std::endl;

    if (suggestions.empty()) {
        std::cout << "No suggestions found." << std::endl;
    } else {
        std::cout << std::fixed << std::setprecision(2); // Format popularity scores
        for (const auto& sugg : suggestions) {
            std::cout << "  - Product: " << std::left << std::setw(20) << sugg.name
                      << " (ID: " << std::setw(3) << sugg.productID << ")"
                      << " Popularity: " << sugg.popularity << std::endl;
        }
    }
    std::cout << "----------------------------------------------" << std::endl << std::endl;
}


int main() {
    // 1. Create the Product Catalog
    ProductCatalog catalog;
    std::cout << "Product Catalog created." << std::endl << std::endl;

    // 2. Add some sample products
    // Arguments: category, productName, productID, initialScore (optional, defaults to 1.0)
    catalog.addProduct("Electronics", "Smartphone", 101, 15.0);
    catalog.addProduct("Electronics", "Smartwatch", 102, 12.0);
    catalog.addProduct("Electronics", "Smart TV", 103, 10.0);
    catalog.addProduct("Electronics", "Laptop", 104, 20.0);
    catalog.addProduct("Electronics", "Wireless Earbuds", 105, 8.0);
    catalog.addProduct("Electronics", "Drone", 106); // Default score 1.0

    catalog.addProduct("Books", "Data Structures and Algorithms", 201, 5.0);
    catalog.addProduct("Books", "The C++ Programming Language", 202, 4.0);
    catalog.addProduct("Books", "Database Design Basics", 203, 3.0);
    catalog.addProduct("Books", "Clean Code", 204); // Default score 1.0

    catalog.addProduct("Clothing", "Smart T-Shirt", 301, 2.0); // Example: a product name starting with 'smart' in another category
    catalog.addProduct("Clothing", "Running Shoes", 302, 6.0);
    catalog.addProduct("Clothing", "Jeans", 303, 7.0);

    std::cout << "Sample products added." << std::endl;
    std::cout << "Available categories: ";
    auto categories = catalog.getCategories();
    for(const auto& cat : categories) { std::cout << cat << " "; }
    std::cout << std::endl << std::endl;

    // 3. Perform initial searches
    std::cout << "=== Initial Searches ===" << std::endl;
    printSuggestions("smart", "Electronics", catalog.searchProducts("smart", "Electronics", 5));
    printSuggestions("smart", "", catalog.searchProducts("smart", "", 5)); // Search all categories
    printSuggestions("data", "Books", catalog.searchProducts("data", "Books", 5));
    printSuggestions("lap", "", catalog.searchProducts("lap", "", 5)); // Search 'lap' (should find laptop)

    // 4. Simulate user interactions - increasing popularity
    std::cout << "=== Simulating User Interactions (Increasing Popularity) ===" << std::endl;
    std::cout << "User views/searches for 'Smartwatch' and 'Smartphone' frequently..." << std::endl;
    catalog.updateProductPopularity("Electronics", "Smartwatch", 5.0); // Increase score
    catalog.updateProductPopularity("Electronics", "Smartwatch", 5.0);
    catalog.updateProductPopularity("Electronics", "Smartphone", 3.0);
    catalog.updateProductPopularity("Clothing", "Smart T-Shirt", 10.0); // This one gets a boost!
    catalog.updateProductPopularity("Electronics", "NonExistentProduct", 5.0); // Test update fail

    std::cout << "Popularity scores updated." << std::endl << std::endl;

    // 5. Perform searches again to see ranking changes
    std::cout << "=== Searches After Popularity Updates ===" << std::endl;
    printSuggestions("smart", "Electronics", catalog.searchProducts("smart", "Electronics", 5));
    printSuggestions("smart", "", catalog.searchProducts("smart", "", 5)); // Search all categories - expect Smartwatch > Smartphone, also Smart T-Shirt might appear high

    // 6. Apply Global Popularity Decay
    double decayFactor = 0.90; // Reduce all scores by 10%
    std::cout << "=== Applying Global Decay (Factor: " << decayFactor << ") ===" << std::endl;
    catalog.applyGlobalDecay(decayFactor);
    std::cout << "Decay applied to all products." << std::endl << std::endl;

    // 7. Perform searches after decay
    std::cout << "=== Searches After Decay ===" << std::endl;
    printSuggestions("smart", "Electronics", catalog.searchProducts("smart", "Electronics", 5));
    printSuggestions("smart", "", catalog.searchProducts("smart", "", 5)); // Scores should be lower, relative order might change slightly over time
    printSuggestions("lap", "", catalog.searchProducts("lap", "", 5)); // Check 'Laptop' score


    std::cout << "=== Demo Finished ===" << std::endl;

    return 0;
}