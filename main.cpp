#include <iostream>
#include <string>
#include "InventorySystem.h"

using namespace std;

int main()
{
    InventorySystem system;

    // Sample data
    system.addCategory(Category("Drinks"));
    system.addCategory(Category("Snacks"));

    system.addProduct(Product("Cola", 2.5, 10, Category("Drinks"), "D1"));
    system.addProduct(Product("Water", 1.2, 20, Category("Drinks"), "D2"));
    system.addProduct(Product("Chips", 3.4, 5, Category("Snacks"), "S1"));

    int choice;

    while (true)
    {
        std::cout << "\n============================\n";
        std::cout << "   INVENTORY SYSTEM MENU\n";
        std::cout << "============================\n";
        std::cout << "1. Show all products\n";
        std::cout << "2. Search product by name\n";
        std::cout << "3. Show low stock items\n";
        std::cout << "4. Add new product\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose: ";
        std::cin >> choice;

        if (choice == 0)
        {
            std::cout << "Exiting...\n";
            break;
        }

        switch (choice)
        {
            case 1:
                system.showProducts();
                break;

            case 2:
            {
                std::string name;
                std::cout << "Enter product name: ";
                std::cin >> name;
                system.searchByName(name);
                break;
            }

            case 3:
                system.lowStockWarning(5);
                break;

            case 4:
            {
                std::string name, id, categoryName;
                double price;
                int qty;

                std::cout << "Enter product name: ";
                std::cin >> name;

                std::cout << "Enter product id: ";
                std::cin >> id;

                std::cout << "Enter category: ";
                std::cin >> categoryName;

                std::cout << "Enter price: ";
                std::cin >> price;

                std::cout << "Enter quantity: ";
                std::cin >> qty;

                system.addProduct(Product(name, price, qty, Category(categoryName), id));

                std::cout << "Product added successfully!\n";
                break;
            }

            default:
                std::cout << "Invalid option!\n";
        }
    }

    return 0;
}