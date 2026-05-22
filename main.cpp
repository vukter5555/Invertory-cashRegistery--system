#include <iostream>
#include <string>
#include "InventorySystem.h"

using namespace std;

int main()
{
    InventorySystem system;

    // Seeding baseline categories
    Category drinks("Drinks");
    Category snacks("Snacks");

    system.addCategory(drinks);
    system.addCategory(snacks);

    // Seeding sample items
    system.addProduct(Product("Cola", 2.5, 10, drinks, "D1"));
    system.addProduct(Product("Water", 1.2, 20, drinks, "D2"));
    system.addProduct(Product("Chips", 3.4, 5, snacks, "S1"));

    int choice;

    do
    {
        cout << "\n====================================================================\n";
        cout << "1.Show      | 2.SearchName | 3.SearchCat | 4.Sale       | 5.LowStock\n";
        cout << "6.History   | 7.Report     | 8.Restock   | 9.Add Prod   | 10.Rem Prod\n";
        cout << "11.Add Cat  | 12.Rem Cat   | 0.Exit\n";
        cout << "====================================================================\n";
        cout << "Enter choice: ";
        
        // Safety mechanism to catch bad string data or overflows right away
        if (!(cin >> choice)) {
            cout << "Invalid input format! Please enter a valid number.\n";
            cin.clear();                 // Clear the fail state flag
            cin.ignore(10000, '\n');     // Wipe out the invalid string from the buffer
            choice = -1;                 // Set to a safe dummy value so it continues looping
            continue;
        }

        switch(choice)
        {
        case 1: 
            system.showProducts(); 
            break;
        case 2:
        {
            cout << "Enter product name: ";
            string n; cin >> n;
            system.searchByName(n);
            break;
        }
        case 3:
        {
            cout << "Enter category name: ";
            string c; cin >> c;
            system.searchByCategory(c);
            break;
        }
        case 4: 
            system.makeSale(); 
            break;
        case 5: 
            system.lowStockWarning(5); 
            break;
        case 6: 
        {
            // Prompt sub-menu filtering choice
            int subChoice;
            cout << "\nSelect View History Mode:\n";
            cout << "1. Only Customer Sales\n";
            cout << "2. Only Vendor Restocks\n";
            cout << "3. Combined Full History\n";
            cout << "Enter selection (1-3): ";
            
            if (!(cin >> subChoice) || subChoice < 1 || subChoice > 3) {
                cout << "Invalid selection entry. Defaulting to show full history logs.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                subChoice = 3; // Default fallthrough choice safety state
            }
            
            system.showTransactions(subChoice); 
            break;
        }
        case 7: 
            system.report(); 
            break;
        case 8: 
            system.restock(); 
            break;
            
        case 9: // Add Product
        {
            string name, id, catName;
            double price;
            int qty;
            
            cout << "Enter new product ID: ";
            cin >> id;
            
            // Check if ID already exists to prevent duplicate items
            if (system.findProductById(id) != nullptr) {
                cout << "Error: A product with ID '" << id << "' already exists!\n";
                break;
            }
            
            cout << "Enter product name: ";
            cin >> name;
            cout << "Enter price: ";
            if (!(cin >> price)) {
                cout << "Invalid price format!\n";
                cin.clear(); cin.ignore(10000, '\n');
                break;
            }
            cout << "Enter initial quantity: ";
            if (!(cin >> qty)) {
                cout << "Invalid quantity format!\n";
                cin.clear(); cin.ignore(10000, '\n');
                break;
            }
            cout << "Enter category name for this product: ";
            cin >> catName;
            
            // Instantiate a temporary Category object to assign to the product layout
            Category targetCat(catName);
            system.addProduct(Product(name, price, qty, targetCat, id));
            cout << "Product '" << name << "' added successfully.\n";
            break;
        }
        
        case 10: // Remove Product
        {
            string id;
            cout << "Enter product ID to remove: ";
            cin >> id;
            
            Product* p = system.findProductById(id);
            if (p == nullptr) {
                cout << "Product not found. Removal failed.\n";
            } else {
                // Pass a temporary matching reference copy for identification matching inside our loop
                system.removeProduct(*p);
            }
            break;
        }
        
        case 11: // Add Category
        {
            string catName;
            cout << "Enter new category name: ";
            cin >> catName;
            
            system.addCategory(Category(catName));
            cout << "Category '" << catName << "' added successfully.\n";
            break;
        }
        
        case 12: // Remove Category
        {
            string catName;
            cout << "Enter category name to remove: ";
            cin >> catName;
            
            // Trigger cascading vector erasure loops using our input criteria object wrapper
            system.removeCategory(Category(catName));
            break;
        }

        case 0:
            cout << "Exiting application...\n";
            break;
        default:
            cout << "Invalid selection! Try again.\n";
        }
    } while(choice != 0);

    return 0;
}