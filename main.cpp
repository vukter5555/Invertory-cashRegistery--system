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
        cout << "1.Show      | 2.Search Prod| 3.Search Cat| 4.Sale       | 5.LowStock\n";
        cout << "6.History   | 7.Report     | 8.Restock   | 9.Add Prod   | 10.Rem Prod\n";
        cout << "11.Add Cat  | 12.Rem Cat   | 13.Edit Prod| 14.Edit Cat  | 0.Exit\n";
        cout << "====================================================================\n";
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid selection entered.\n";
            cin.clear(); 
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice)
        {
        case 1: system.showProducts(); break;
        case 2: 
        {
            int searchMode;
            cout << "Choose search parameters: (1) Search by custom Product ID | (2) Search by text Name pattern: ";
            if (!(cin >> searchMode)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Invalid mode.\n"; break;
            }
            if (searchMode == 1) {
                string id; cout << "Enter precise product unique ID to find: "; cin >> id;
                Product* p = system.findProductById(id);
                if (p) {
                    cout << "Match found: ID: " << p->getId() << " | Name: " << p->getName() 
                          << " | Category: " << p->getCategory().getName() << " | Shelf Price: " << p->getPrice() 
                          << " $ | Stock Volume: " << p->getQuantity() << " units\n";
                } else {
                    cout << "No entity corresponds to that identification parameters.\n";
                }
            } else {
                string name; cout << "Enter name snippet query string: "; cin >> name;
                system.searchByName(name);
            }
            break;
        }
        case 3: 
        {
            string catName; cout << "Enter category organizational folder title: "; cin >> catName;
            system.searchByCategory(catName); break;
        }
        case 4: system.makeSale(); break;
        case 5: 
        {
            int limit; cout << "Enter safety threshold margin volume (integer units limit): ";
            if (!(cin >> limit)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Invalid input.\n"; break;
            }
            system.lowStockWarning(limit); break;
        }
        case 6: 
        {
            int filter; 
            cout << "Set structural parsing filters: (1) Output SALES logs only | (2) Output RESTOCKS logs only | (3) Fetch complete array logs: ";
            if (!(cin >> filter) || filter < 1 || filter > 3) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Invalid criteria.\n"; break;
            }
            system.showTransactions(filter); break;
        }
        case 7: system.report(); break;
        case 8: system.restock(); break;
        case 9: 
        {
            string name, id, catName; double price; int qty;
            cout << "Enter alphanumeric unique item identification (Product ID): "; cin >> id;
            cout << "Enter nominal name: "; cin >> name;
            cout << "Enter target group node name (Category): "; cin >> catName;
            cout << "Enter product retail shelf price: "; 
            if (!(cin >> price) || price < 0) {
                cout << "Invalid price signature registration rejected.\n";
                cin.clear(); cin.ignore(10000, '\n'); break;
            }
            cout << "Enter initial stock quantity value: ";
            if (!(cin >> qty) || qty < 0) {
                cout << "Invalid volume initialization metrics rejected.\n";
                cin.clear(); cin.ignore(10000, '\n'); break;
            }
            
            Category* catRef = system.findCategoryByName(catName);
            if (!catRef) {
                char creationPrompt;
                cout << "Warning: Parent category '" << catName << "' does not exist. Auto-generate node category? (y/n): ";
                cin >> creationPrompt;
                if (creationPrompt == 'y' || creationPrompt == 'Y') {
                    system.addCategory(Category(catName));
                    catRef = system.findCategoryByName(catName);
                    cout << "Category infrastructure dynamically updated.\n";
                } else {
                    cout << "Operation aborted: Product cannot stand orphaned from a category node.\n";
                    break;
                }
            }
            
            system.addProduct(Product(name, price, qty, *catRef, id));
            cout << "Product record initialized into stock layers.\n";
            break;
        }
        case 10: 
        {
            string id; cout << "Enter product ID to purge from catalog: "; cin >> id;
            Product* p = system.findProductById(id);
            if (p) system.removeProduct(*p);
            else cout << "Target asset missing from registry lookup operations.\n";
            break;
        }
        case 11: 
        {
            string catName; cout << "Enter structural classification group title (Category Name): "; cin >> catName;
            if (system.findCategoryByName(catName)) {
                cout << "Collision error: Node title exists in data trees.\n";
            } else {
                system.addCategory(Category(catName));
                cout << "Category profile registered.\n";
            }
            break;
        }
        case 12: 
        {
            string catName; cout << "Enter category name to wipe: "; cin >> catName;
            system.removeCategory(Category(catName)); break;
        }
        case 13: 
        {
            string oldId, newId, newName; double newPrice; int newQty;
            cout << "Enter current product ID to edit: "; cin >> oldId;
            Product* p = system.findProductById(oldId);
            if (!p) {
                cout << "Product not found.\n"; break;
            }
            
            int subChoice;
            cout << "Edit Fields Menu:\n";
            cout << " (1) Selectively alter specific items fields\n";
            cout << " (2) Rewrite all data parameters holistically\n";
            cout << "Enter menu subset option choice: ";
            if (!(cin >> subChoice)) {
                cin.clear(); cin.ignore(10000, '\n'); break;
            }
            
            if (subChoice == 1) {
                int fieldChoice;
                cout << "   [1] Modify Product ID Code\n"
                     << "   [2] Modify Product Text Name\n"
                     << "   [3] Update Base Retail Pricing\n"
                     << "   [4] Modify Inventory Quantities (Shrinkage/Wastage adjustments)\n"
                     << "   Enter selection index: ";
                if (!(cin >> fieldChoice)) {
                    cin.clear(); cin.ignore(10000, '\n'); break;
                }
                
                if (fieldChoice == 1) {
                    cout << "Enter alternative unique Product ID: "; cin >> newId;
                    if (system.updateProductInformation(oldId, newId, p->getName(), p->getPrice())) {
                        cout << "Unique key assignment verified.\n";
                    }
                } else if (fieldChoice == 2) {
                    cout << "Enter new name token: "; cin >> newName;
                    p->setName(newName);
                    cout << "Name properties modified.\n";
                } else if (fieldChoice == 3) {
                    cout << "Enter new updated price value: ";
                    if (cin >> newPrice && newPrice >= 0) {
                        p->setPrice(newPrice);
                        cout << "Pricing parameters adjusted.\n";
                    } else {
                        cout << "Validation error: Negative value rejected.\n";
                        cin.clear(); cin.ignore(10000, '\n');
                    }
                } else if (fieldChoice == 4) {
                    cout << "Current stock: " << p->getQuantity() << " units. Enter exact new quantity: ";
                    if (cin >> newQty && newQty >= 0) {
                        p->setQuantity(newQty);
                        cout << "Inventory volume ledger levels calibrated.\n";
                    } else {
                        cout << "Validation error: Stock cannot fall below zero metrics.\n";
                        cin.clear(); cin.ignore(10000, '\n');
                    }
                }
            } else {
                cout << "Enter new universal ID (or repeat current): "; cin >> newId;
                cout << "Enter new name: "; cin >> newName;
                cout << "Enter new price: ";
                if (!(cin >> newPrice) || newPrice < 0) {
                    cout << "Invalid price entry. Modification aborted.\n";
                    cin.clear(); cin.ignore(10000, '\n');
                    break;
                }
                
                cout << "Enter updated stock quantity: ";
                if (!(cin >> newQty) || newQty < 0) {
                    cout << "Invalid quantity entry. Modification aborted.\n";
                    cin.clear(); cin.ignore(10000, '\n');
                    break;
                }
                
                if (system.updateProductInformation(oldId, newId, newName, newPrice)) {
                    Product* updatedProd = system.findProductById(newId);
                    if (updatedProd) updatedProd->setQuantity(newQty);
                    cout << "All product information updated successfully.\n";
                }
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