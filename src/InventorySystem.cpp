#include "InventorySystem.h"
#include <iostream>
#include <iomanip>

// Define the constructor and initialize starting budget
InventorySystem::InventorySystem() : budget(1000.0) {}

void InventorySystem::addProduct(const Product& p)
{
    products.push_back(p);
}

void InventorySystem::addCategory(const Category& c)
{
    categories.push_back(c);
}

void InventorySystem::showProducts() const
{
    std::cout << "\n==================== PRODUCTS ====================\n";
    std::cout << std::left << std::setw(6) << "ID" 
              << std::setw(15) << "Name" 
              << std::setw(12) << "Price" 
              << std::setw(10) << "Quantity" << "\n";
    std::cout << "--------------------------------------------------\n";

    for (const auto& p : products)
    {
        std::cout << std::left << std::setw(6) << p.getId()
                  << std::setw(15) << p.getName()
                  << std::setw(12) << p.getPrice()
                  << std::setw(10) << p.getQuantity()
                  << std::endl;
    }
}

Product* InventorySystem::findProductById(const std::string& id)
{
    for (auto& p : products)
    {
        if (p.getId() == id)
        {
            return &p;
        }
    }
    return nullptr;
}

void InventorySystem::searchByName(const std::string& name) const
{
    std::cout << "\n--- Search Results for Name: " << name << " ---\n";
    bool found = false;
    for (const auto& p : products)
    {
        if (p.getName() == name)
        {
            std::cout << "ID: " << p.getId() << " | Price: " << p.getPrice() << " | Stock: " << p.getQuantity() << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No product found with that name.\n";
}

void InventorySystem::searchByCategory(const std::string& categoryName) const
{
    bool foundc = false;
    for (const auto& c : categories)
    {
        if (c.getName() == categoryName)
        {
            foundc = true;
            break;
        }
    }

    if (foundc == false)
    {
        std::cout<< "This category doesn't exist yet.\n";
    }
    else
    {
        std::cout << "\n--- Products in Category: " << categoryName << " ---\n";
        bool found = false;
        for (const auto& p : products)
        {
            if (p.getCategory().getName() == categoryName)
            {
                std::cout << "ID: " << p.getId() << " | Name: " << p.getName() << " | Price: " << p.getPrice() << "\n";
                found = true;
            }
        }
        if (!found) std::cout << "No products found in this category.\n";
    }
    
}


void InventorySystem::makeSale()
{
    Transaction currentTransaction;
    bool itemsAdded = false;

    // Clear any leftover newlines from the main menu selection stream
    std::cin.ignore(10000, '\n'); 

    while (true)
    {
        std::string id;
        std::cout << "\nEnter product ID to sell (type exit to stop): ";
        
        // Safety check: if stream fails to read a string, exit safely
        if (!(std::cin >> id))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            break;
        }

        if (id == "exit")
            break;

        Product* p = findProductById(id);
        if (!p)
        {
            std::cout << "Product not found. Please try again.\n";
            
            // FIX: Clear stream and wipe out any stray characters/newlines 
            // left behind so the next cycle is completely fresh
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue; 
        }

        int qty;
        std::cout << "Enter quantity: ";
        if (!(std::cin >> qty))
        {
            std::cout << "Invalid quantity format. Skipping item.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        try
        {
            p->reduceQuantity(qty);
        
            TransactionItem item(p, qty);
            currentTransaction.addItem(item);
            itemsAdded = true;
            
            std::cout << "\n--- Product added successfully ---\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "Failed to add product: " << e.what() << "\n";
        }
    }

    if (itemsAdded)
    {
        budget += currentTransaction.getTotal();
        transactions.push_back(currentTransaction);
        std::cout << "\n--- Sale successful ---\n";
        currentTransaction.printReceipt();
    }
    else
    {
        std::cout << "\nSale cancelled or cart was empty.\n";
    }
}


void InventorySystem::restock()
{
    std::string id;
    int qty;
    std::cout << "\nEnter product ID to restock: ";
    std::cin >> id;
    
    std::cout << "Enter quantity: ";
    if (!(std::cin >> qty))
    {
        std::cout << "Error: Invalid quantity amount or numeric overflow!\n";
        std::cin.clear();              
        std::cin.ignore(10000, '\n');  
        return;
    }

    Product* p = findProductById(id);
    if (!p)
    {
        std::cout << "Product not found.\n";
        return;
    }

    double spend = qty * p->getPrice();
    if (spend > this->budget)
    {
        std::cout << "You don't have that much money. Restock failed.\n";
        return;
    }

    // FIX: Warning check for spending more than half the budget
    if (spend >= this->budget / 2)
    {
        char confirm;
        std::cout << "Warning: You will spend " << spend << " $, which is more than half of your current budget (" << this->budget << " $).\n";
        std::cout << "Are you sure you want to continue? (y/n): ";
        std::cin >> confirm;
        
        // Clean up the input stream buffer so the trailing '\n' doesn't break the main menu
        std::cin.ignore(10000, '\n');

        if (confirm != 'y' && confirm != 'Y')
        {
            std::cout << "Restock cancelled by user.\n";
            return; // Exit out ONLY if they decide not to proceed
        }
    }

    try
    {
        p->increaseQuantity(qty);
        this->budget -= spend; // FIX: Deduct the spent funds from your store budget
        std::cout << "Restocked " << qty << " units of " << p->getName() << " successfully.\n";
        std::cout << "Remaining budget: " << this->budget << " $\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "Restock failed: " << e.what() << "\n";
    }
}


void InventorySystem::report() const
{
    std::cout << "\n==================== REPORT ====================\n";
    std::cout << "Total Register Balance / Budget: " << budget << " $\n";
    std::cout << "Total Transactions Processed: " << transactions.size() << "\n";
}

void InventorySystem::showTransactions() const
{
    std::cout << "\n==================== TRANSACTION HISTORY ====================\n";
    if (transactions.empty())
    {
        std::cout << "No transactions recorded yet.\n";
        return;
    }

    int counter = 1;
    for (const auto& t : transactions)
    {
        std::cout << "Transaction #" << counter++ << "\n";
        t.printReceipt();
    }
}

void InventorySystem::lowStockWarning(int limit) const
{
    std::cout << "\n==================== LOW STOCK WARNINGS ====================\n";
    bool alert = false;
    for (const auto& p : products)
    {
        if (p.getQuantity() <= limit)
        {
            std::cout << "ALERT: Product " << p.getName() << " (ID: " << p.getId() 
                      << ") is low on stock! Current: " << p.getQuantity() << "\n";
            alert = true;
        }
    }
    if (!alert) std::cout << "All items have sufficient stock levels.\n";
}