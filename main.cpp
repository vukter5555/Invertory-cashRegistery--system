#include <iostream>
#include <string>
#include "InventorySystem.h"

using namespace std;

int main()
{
    InventorySystem system;

    Category drinks("Drinks");
    Category snacks("Snacks");
    system.addCategory(drinks);
    system.addCategory(snacks);

    system.addProduct(Product("Cola", 2.5, 10, drinks, "D1"));
    system.addProduct(Product("Water", 1.2, 20, drinks, "D2"));
    system.addProduct(Product("Chips", 3.4, 5, snacks, "S1"));

    int choice;

    do
    {
        cout << "\n====================================================================\n";
        cout << "1.Show      | 2.SearchName | 3.SearchCat | 4.Sale       | 5.LowStock\n";
        cout << "6.History   | 7.Report     | 8.Restock   | 9.Add Prod   | 10.Rem Prod\n";
        cout << "11.Add Cat  | 12.Rem Cat   | 13.Edit Prod| 14.Edit Cat  | 0.Exit\n";
        cout << "====================================================================\n";
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid selection entered.\n";
            cin.clear(); cin.ignore(10000, '\n');
            choice = -1; continue;
        }

        switch(choice)
        {
        case 1: system.showProducts(); break;
        case 2: {
            cout << "Enter product name: "; string n; cin >> n;
            system.searchByName(n); break;
        }
        case 3: {
            cout << "Enter category name: "; string c; cin >> c;
            system.searchByCategory(c); break;
        }
        case 4: system.makeSale(); break;
        case 5: system.lowStockWarning(5); break;
        case 6: {
            int subChoice;
            cout << "1.Sales | 2.Restocks | 3.Combined: "; cin >> subChoice;
            system.showTransactions(subChoice); break;
        }
        case 7: system.report(); break;
        case 8: system.restock(); break;
        case 9: {
            string name, id, catName; double price; int qty;
            cout << "Enter unique product ID: "; cin >> id;
            if (system.findProductById(id) != nullptr) {
                cout << "Error: ID already registered universally!\n"; break;
            }
            cout << "Enter product name: "; cin >> name;
            cout << "Enter price: "; cin >> price;
            cout << "Enter initial quantity: "; cin >> qty;
            cout << "Enter category name: "; cin >> catName;
            
            Category* existingCat = system.findCategoryByName(catName);
            if (existingCat == nullptr) {
                char createChoice;
                cout << "Category does not exist. Create it? (y/n): "; cin >> createChoice;
                if (createChoice == 'y' || createChoice == 'Y') {
                    system.addCategory(Category(catName));
                } else break;
            }
            system.addProduct(Product(name, price, qty, Category(catName), id));
            break;
        }
        case 10: {
            string id; cout << "Enter ID to remove: "; cin >> id;
            Product* p = system.findProductById(id);
            if (p) system.removeProduct(*p);
            break;
        }
        case 11: {
            string catName; cout << "Enter new category name: "; cin >> catName;
            if (system.findCategoryByName(catName)) cout << "Error: Name exists!\n";
            else system.addCategory(Category(catName));
            break;
        }
        case 12: {
            string catName; cout << "Enter category name to wipe: "; cin >> catName;
            system.removeCategory(Category(catName)); break;
        }
        case 13: { // Edit Product Information
            string oldId, newId, newName; double newPrice;
            cout << "Enter current product ID to edit: "; cin >> oldId;
            if (!system.findProductById(oldId)) {
                cout << "Product not found.\n"; break;
            }
            cout << "Enter new universal ID (or repeat current): "; cin >> newId;
            cout << "Enter new name text: "; cin >> newName;
            cout << "Enter new price: "; cin >> newPrice;
            
            if (system.updateProductInformation(oldId, newId, newName, newPrice)) {
                cout << "Product modifications applied successfully.\n";
            }
            break;
        }
        case 14: { // Edit Category Information
            string oldName, newName;
            cout << "Enter existing category name to modify: "; cin >> oldName;
            cout << "Enter new replacement title: "; cin >> newName;
            
            if (system.updateCategoryInformation(oldName, newName)) {
                cout << "Category updated universally across all child nodes.\n";
            } else {
                cout << "Category not found or name collision triggered.\n";
            }
            break;
        }
        case 0: cout << "Exiting...\n"; break;
        default: cout << "Invalid option chosen.\n";
        }
    } while(choice != 0);

    return 0;
}