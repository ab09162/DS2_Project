#include "GridOfTries.h"
#include <iostream>
#include <string>
#include <vector>

void printMenu() {
    std::cout << "\n----- Demo Menu -----\n";
    std::cout << "1. Insert a product\n";
    std::cout << "2. Search for a product\n";
    std::cout << "3. Update product popularity\n";
    std::cout << "4. Delete a product\n";
    std::cout << "5. Display all products in a category\n";
    std::cout << "6. Auto-complete suggestions\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    GridOfTries grid;
    int choice;
    std::string category, product;
    int popularity;
    std::string prefix;

    // Sample pre-inserted data for demo purposes.
    grid.insertProduct("Electronics", "Smartphone", 50);
    grid.insertProduct("Electronics", "Smartwatch", 30);
    grid.insertProduct("Books", "DataStructures", 20);
    grid.insertProduct("Books", "Algorithms", 40);
    grid.insertProduct("Electronics", "SmartTV", 35);

    while (true) {
        printMenu();
        std::cin >> choice;
        std::cin.ignore(); // Clear newline from input buffer
        
        switch (choice) {
            case 1:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                std::cout << "Enter product name: ";
                std::getline(std::cin, product);
                std::cout << "Enter initial popularity: ";
                std::cin >> popularity;
                std::cin.ignore();
                grid.insertProduct(category, product, popularity);
                std::cout << "Product inserted successfully.\n";
                break;

            case 2:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                std::cout << "Enter product name: ";
                std::getline(std::cin, product);
                if (grid.searchProduct(category, product))
                    std::cout << "Product found in category '" << category << "'.\n";
                else
                    std::cout << "Product not found.\n";
                break;

            case 3:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                std::cout << "Enter product name: ";
                std::getline(std::cin, product);
                std::cout << "Enter new popularity: ";
                std::cin >> popularity;
                std::cin.ignore();
                if (grid.updateProductPopularity(category, product, popularity))
                    std::cout << "Product popularity updated.\n";
                else
                    std::cout << "Update failed: Product not found.\n";
                break;

            case 4:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                std::cout << "Enter product name: ";
                std::getline(std::cin, product);
                if (grid.deleteProduct(category, product))
                    std::cout << "Product deleted successfully.\n";
                else
                    std::cout << "Deletion failed: Product not found.\n";
                break;

            case 5:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                grid.displayCategory(category);
                break;

            case 6:
                std::cout << "Enter category for auto-complete: ";
                std::getline(std::cin, category);
                std::cout << "Enter prefix: ";
                std::getline(std::cin, prefix);
                {
                    // Check if the category exists using the map.
                    if (grid.categoryTries.find(category) == grid.categoryTries.end()) {
                        std::cout << "Category not found.\n";
                        break;
                    }
                    
                    // Retrieve auto-complete suggestions from the trie of the given category.
                    std::vector<std::string> suggestions = grid.categoryTries[category]->autoComplete(prefix);
                    if (suggestions.empty()) {
                        std::cout << "No suggestions found for prefix '" << prefix << "'.\n";
                    } else {
                        std::cout << "Suggestions:\n";
                        for (const auto& suggestion : suggestions) {
                            std::cout << "- " << suggestion << "\n";
                        }
                    }
                }
                break;
            
            case 7:
                std::cout << "Exiting demo.\n";
                return 0;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}
