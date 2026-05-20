#include "InventorySystem.h"
#include <iostream>
#include <iomanip>

// Explicitly define the constructor that was missing
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
    std::cout << "\n===== PRODUCTS =====\n";

    for (const auto& p : products)
    {
        // Fixed capitalization here to match Product.h exactly
        std::cout << std::setw(10) << p.getId()
                  << " | " << p.getName()
                  << " | " << p.getPrice()
                  << " | " << p.getQuantity()
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

void InventorySystem::makeSale()
{
    std::string id;
    int qty;
    std::cout << "\nEnter product ID to sell: ";
    std::cin >> id;
    std::cout << "Enter quantity: ";
    std::cin >> qty;

    Product* p = findProductById(id);
    if (!p)
    {
        std::cout << "Product not found.\n";
        return;
    }

    try
    {
        p->reduceQuantity(qty);
        
        // Record transaction
        Transaction currentTransaction;
        TransactionItem item(p, qty);
        currentTransaction.addItem(item);
        
        transactions.push_back(currentTransaction);
        budget += currentTransaction.getTotal();

        std::cout << "\n--- Sale Successful ---\n";
        currentTransaction.printReceipt();
    }
    catch (const std::exception& e)
    {
        std::cout << "Sale failed: " << e.what() << "\n";
    }
}

void InventorySystem::restock()
{
    std::string id;
    int qty;
    std::cout << "\nEnter product ID to restock: ";
    std::cin >> id;
    std::cout << "Enter quantity: ";
    std::cin >> qty;

    Product* p = findProductById(id);
    if (!p)
    {
        std::cout << "Product not found.\n";
        return;
    }

    try
    {
        p->increaseQuantity(qty);
        std::cout << "Restocked " << qty << " units of " << p->getName() << " successfully.\n";
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