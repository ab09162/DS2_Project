#include "GridOfTries.h"
#include <iostream>
#include <string>
#include <vector>

void printMenu() {
    std::cout << "\n----- Demo Menu -----\n";
    std::cout << "1. Insert a product\n";
    std::cout << "2. Search for a product using Category name\n"; 
    std::cout << "3. Search for a product\n"; 
    std::cout << "4. Update product popularity\n";
    std::cout << "5. Update product price\n";
    std::cout << "6. Delete a product\n";
    std::cout << "7. Display all products in a category\n";
    std::cout << "8. Auto-complete suggestions\n";
    std::cout << "9. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    GridOfTries grid;
    int choice;
    std::string category, product;
    int popularity;
    float price;

    // Sample pre-inserted data for demo purposes.
    grid.insertProduct("Electronics", "Laptop", 80, 1500);
    grid.insertProduct("Electronics", "Tablet", 70, 600);
    grid.insertProduct("Electronics", "Headphones", 90, 200);
    grid.insertProduct("Electronics", "Smart TV", 60, 1200);
    grid.insertProduct("Books", "C++ Programming", 45, 55);
    grid.insertProduct("Books", "Python for Data Science", 50, 60);
    grid.insertProduct("Books", "Introduction to Algorithms", 85, 75);
    grid.insertProduct("Books", "Clean Code", 95, 40);
    grid.insertProduct("Clothing", "T-Shirt", 110, 15);
    grid.insertProduct("Clothing", "Jeans", 120, 40);
    grid.insertProduct("Clothing", "Jacket", 80, 85);
    grid.insertProduct("Clothing", "Sneakers", 75, 90);
    grid.insertProduct("Sports", "Soccer Ball", 40, 25);
    grid.insertProduct("Sports", "Tennis Racket", 65, 120);
    grid.insertProduct("Sports", "Basketball", 50, 30);
    grid.insertProduct("Sports", "Running Shoes", 90, 150);
    grid.insertProduct("Furniture", "Sofa", 60, 500);
    grid.insertProduct("Furniture", "Dining Table", 55, 300);
    grid.insertProduct("Furniture", "Bookshelf", 45, 120);
    grid.insertProduct("Furniture", "Chair", 110, 70);
    grid.insertProduct("Toys", "Lego Set", 95, 60);
    grid.insertProduct("Toys", "Action Figure", 80, 25);
    grid.insertProduct("Toys", "Doll", 70, 20);
    grid.insertProduct("Toys", "Remote Control Car", 85, 50);
    grid.insertProduct("Beauty", "Shampoo", 130, 10);
    grid.insertProduct("Beauty", "Face Cream", 140, 30);
    grid.insertProduct("Beauty", "Perfume", 110, 50);
    grid.insertProduct("Beauty", "Lipstick", 95, 20);
    grid.insertProduct("Food", "Organic Apples", 120, 3);
    grid.insertProduct("Food", "Whole Wheat Bread", 130, 2);
    grid.insertProduct("Food", "Almonds", 90, 12);
    grid.insertProduct("Food", "Greek Yogurt", 85, 4);

    std::string prefix;


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
                std::cout << "Enter price: ";
                std::cin >> price;
                std::cin.ignore();
                grid.insertProduct(category, product, popularity, price);
                std::cout << "Product inserted successfully.\n";
                break;


            case 2:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                std::cout << "Enter product name: ";
                std::getline(std::cin, product);
                if (grid.searchProduct1(category, product))
                    std::cout << "Product found in category '" << category << "'.\n";
                else
                    std::cout << "Product not found.\n";
                break;

            case 3:
                std::cout << "Enter product name: ";
                std::getline(std::cin, product);
                grid.searchProduct2(product);  // uses new function
                break;


            case 4:
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

            case 5:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                std::cout << "Enter product name: ";
                std::getline(std::cin, product);
                std::cout << "Enter new price: ";
                std::cin >> price;
                std::cin.ignore();
                if (grid.updateProductPrice(category, product, price))
                    std::cout << "Product price updated.\n";
                else
                    std::cout << "Update failed: Product not found.\n";
                break;

            case 6:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                std::cout << "Enter product name: ";
                std::getline(std::cin, product);
                if (grid.deleteProduct(category, product))
                    std::cout << "Product deleted successfully.\n";
                else
                    std::cout << "Deletion failed: Product not found.\n";
                break;

            case 7:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                grid.displayCategory(category);
                break;

            case 8:
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
            
            case 9:
                std::cout << "Exiting demo.\n";
                return 0;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}
