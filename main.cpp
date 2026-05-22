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

    // FIXED: Stray backslashes removed here
    system.addProduct(Product("Cola", 2.5, 10, drinks, "D1"));
    system.addProduct(Product("Water", 1.2, 20, drinks, "D2"));
    system.addProduct(Product("Chips", 3.4, 5, snacks, "S1"));

    int choice;

    do
    {
        cout << "\n====================================================================\n";
        cout << "1.Show      | 2.Search Prod| 3.Search Cat| 4.Sale       | 5.LowStock\n";
        cout << "6.History   | 7.Report     | 8.Restock   | 9.Add Prod   | 10.Rem Prod\n";
        cout << "11.Add Cat  | 12.Rem Cat   | 13.Edit Prod| 14.Edit Cat  | 0.Exit\n";
        cout << "====================================================================\n";
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid selection entered.\n";
            cin.clear(); 
            cin.ignore(10000, '\n');
            choice = -1; 
            continue;
        }

        switch(choice)
        {
        case 1: 
            system.showProducts(); 
            break;
        case 2:
        {
            int opt;
            cout << "\nSearch Options:\n1. Search by Name\n2. Search by ID\nEnter choice (1-2): ";
            if (!(cin >> opt)) { 
                cin.clear(); 
                cin.ignore(10000, '\n'); 
                cout << "Invalid numerical input formatting entry.\n";
                break; 
            }
            
            if (opt == 1) {
                string term; cout << "Enter search term: "; cin >> term;
                system.searchByName(term);
            } 
            else if (opt == 2) {
                string id; cout << "Enter search ID: "; cin >> id;
                Product* ptr = system.findProductById(id);
                if (ptr) {
                    system.searchByName(id);
                } else {
                    cout << "No matching ID found.\n";
                }
            } 
            else {
                cout << "Invalid menu selection! Please choose strictly 1 or 2.\n";
            }
            break;
        }
        case 3:
        {
            string catName; cout << "Enter category name: "; cin >> catName;
            system.searchByCategory(catName);
            break;
        }
        case 4: 
            system.makeSale(); 
            break;
        case 5: 
        {
            int lim; cout << "Enter low stock checking threshold limit: ";
            if (!(cin >> lim)) { cin.clear(); cin.ignore(10000, '\n'); lim = 5; }
            system.lowStockWarning(lim); 
            break;
        }
        case 6: 
        {
            int subChoice;
            cout << "1.Sales | 2.Restocks | 3.Combined: ";
            if (!(cin >> subChoice)) { cin.clear(); cin.ignore(10000, '\n'); subChoice = 3; }
            system.showTransactions(subChoice); 
            break;
        }
        case 7: 
            system.report(); 
            break;
        case 8: 
            system.restock(); 
            break;
        case 9:
        {
            string name, id, cName; double bPrice; int initialQty;
            cout << "Enter ID: "; cin >> id;
            if (system.findProductById(id)) { cout << "Error: Duplicate product ID assigned.\n"; break; }
            cout << "Enter name: "; cin >> name;
            cout << "Enter tracking category name: "; cin >> cName;
            
            Category* catPtr = system.findCategoryByName(cName);
            
            if (!catPtr) { 
                char createChoice;
                cout << "Category '" << cName << "' does not exist. Create it now? (y/n): ";
                cin >> createChoice;
                
                if (createChoice == 'y' || createChoice == 'Y') {
                    system.addCategory(Category(cName));
                    catPtr = system.findCategoryByName(cName); 
                    cout << "Category '" << cName << "' successfully registered into memory.\n";
                } else {
                    cout << "Operation aborted. Product creation cancelled due to missing category mapping.\n";
                    break; 
                }
            }
            
            cout << "Enter initialization baseline price: "; cin >> bPrice;
            cout << "Enter opening stock quantities: "; cin >> initialQty;
            system.addProduct(Product(name, bPrice, initialQty, *catPtr, id));
            cout << "Product successfully created.\n";
            break;
        }
        case 10:
        {
            string id; cout << "Enter asset ID to clean from database: "; cin >> id;
            Product* p = system.findProductById(id);
            if(p) system.removeProduct(*p);
            else cout << "Asset reference empty.\n";
            break;
        }
        case 11:
        {
            string catName; cout << "Enter new structural identity context name: "; cin >> catName;
            system.addCategory(Category(catName)); break;
        }
        case 12:
        {
            string catName; cout << "Enter category name to wipe: "; cin >> catName;
            system.removeCategory(Category(catName)); break;
        }
        case 13: 
        {
            string oldId, newId, newName; double newPrice;
            cout << "Enter current product ID to edit: "; cin >> oldId;
            if (!system.findProductById(oldId)) {
                cout << "Product not found.\n"; break;
            }
            cout << "Enter new universal ID (or repeat current): "; cin >> newId;
            cout << "Enter new name: "; cin >> newName;
            cout << "Enter new price: ";
            
            if (!(cin >> newPrice)) {
                cout << "Error: Invalid selection entered.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            
            if (system.updateProductInformation(oldId, newId, newName, newPrice)) {
                cout << "Product modifications applied successfully.\n";
            }
            break;
        }
        case 14: 
        {
            string oldName, newName;
            cout << "Enter existing category name to modify: "; cin >> oldName;
            cout << "Enter new replacement title: "; cin >> newName;
            
            if (system.updateCategoryInformation(oldName, newName)) {
                cout << "Category updated universally.\n";
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