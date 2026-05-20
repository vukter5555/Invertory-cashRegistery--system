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
        cout << "\n=======================================================\n";
        cout << "1.Show | 2.SearchName | 3.SearchCat | 4.Sale | 5.LowStock\n";
        cout << "6.History | 7.Report  | 8.Restock   | 0.Exit\n";
        cout << "=======================================================\n";
        cout << "Enter choice: ";
        if (!(cin >> choice)) break;

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
            system.showTransactions(); 
            break;
        case 7: 
            system.report(); 
            break;
        case 8: 
            system.restock(); 
            break;
        case 0:
            cout << "Exiting application...\n";
            break;
        default:
            cout << "Invalid selection! Try again.\n";
        }
    } while(choice != 0);

    return 0;
}