#include "InventorySystem.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

InventorySystem::InventorySystem() : budget(1000.0) {}

std::string InventorySystem::toLower(std::string str) const
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return str;
}

void InventorySystem::addProduct(const Product& p) { products.push_back(p); }

void InventorySystem::removeProduct(const Product& p)
{
    bool found = false;
    for (size_t i = 0; i < products.size(); i++)
    {
        if (toLower(products[i].getId()) == toLower(p.getId()))
        {
            products.erase(products.begin() + i);
            found = true;
            i--; 
        }
    }
    if (found) std::cout << "Product removed successfully.\n";
    else std::cout << "Product not found.\n";
}

void InventorySystem::addCategory(const Category& c) { categories.push_back(c); }

void InventorySystem::removeCategory(const Category& category)
{
    bool categoryFound = false;
    std::string targetCatLower = toLower(category.getName());

    for (size_t i = 0; i < categories.size(); i++)
    {
        if (toLower(categories[i].getName()) == targetCatLower)
        {
            categories.erase(categories.begin() + i);
            categoryFound = true;
            i--;
        }
    }

    if (!categoryFound) {
        std::cout << "Category not found.\n";
        return;
    }

    for (size_t i = 0; i < products.size(); i++)
    {
        if (toLower(products[i].getCategory().getName()) == targetCatLower)
        {
            products.erase(products.begin() + i);
            i--;
        }
    }
    std::cout << "Category and all its associated products wiped successfully.\n";
}

Product* InventorySystem::findProductById(const std::string& id)
{
    std::string targetIdLower = toLower(id);
    for (auto& p : products)
    {
        if (toLower(p.getId()) == targetIdLower) return &p;
    }
    return nullptr;
}

Category* InventorySystem::findCategoryByName(const std::string& name)
{
    std::string targetNameLower = toLower(name);
    for (auto& c : categories)
    {
        if (toLower(c.getName()) == targetNameLower) return &c;
    }
    return nullptr;
}

bool InventorySystem::updateProductInformation(const std::string& oldId, const std::string& newId, const std::string& newName, double newPrice)
{
    Product* p = findProductById(oldId);
    if (!p) return false;

    if (toLower(oldId) != toLower(newId)) {
        if (findProductById(newId) != nullptr) {
            std::cout << "Error: Product ID collision triggered.\n";
            return false;
        }
    }

    p->setId(newId);
    p->setName(newName);
    p->setPrice(newPrice);
    return true;
}

bool InventorySystem::updateCategoryInformation(const std::string& oldName, const std::string& newName)
{
    Category* cat = findCategoryByName(oldName);
    if (!cat) return false;

    if (toLower(oldName) != toLower(newName)) {
        if (findCategoryByName(newName) != nullptr) return false;
    }

    std::string oldTitleUnchanged = cat->getName();
    cat->setName(newName);

    for (auto& p : products)
    {
        if (toLower(p.getCategory().getName()) == toLower(oldTitleUnchanged))
        {
            p.setCategory(*cat); // Коригирано на p. вместо p->
        }
    }
    return true;
}

void InventorySystem::showProducts() const
{
    std::cout << "\n==================== WAREHOUSE INVENTORY STOCK LEDGER ====================\n";
    if (products.empty()) {
        std::cout << "   [No registered products found in active memory]\n";
    } else {
        for (const auto& p : products)
        {
            std::cout << "ID: " << p.getId() << " | Name: " << p.getName() 
                      << " | Category: " << p.getCategory().getName()
                      << " | Retail Price: " << p.getPrice() << " $"
                      << " | Stock Volume: " << p.getQuantity() << " units\n";
        }
    }
    std::cout << "=========================================================================\n";
}

void InventorySystem::searchByName(const std::string& name) const
{
    std::cout << "\n-------------------- SEARCH RESULTS (NAME) --------------------\n";
    bool found = false;
    std::string searchLower = toLower(name);
    for (const auto& p : products)
    {
        if (toLower(p.getName()).find(searchLower) != std::string::npos)
        {
            std::cout << "ID: " << p.getId() << " | Name: " << p.getName() 
                      << " | Category: " << p.getCategory().getName()
                      << " | Price: " << p.getPrice() << " $ | Stock: " << p.getQuantity() << " units\n";
            found = true;
        }
    }
    if (!found) std::cout << "No items matches the query parameters.\n";
    std::cout << "---------------------------------------------------------------\n";
}

void InventorySystem::searchByCategory(const std::string& category) const
{
    std::cout << "\n-------------------- SEARCH RESULTS (CATEGORY) --------------------\n";
    bool found = false;
    std::string searchLower = toLower(category);
    for (const auto& p : products)
    {
        if (toLower(p.getCategory().getName()) == searchLower)
        {
            std::cout << "ID: " << p.getId() << " | Name: " << p.getName() 
                      << " | Price: " << p.getPrice() << " $ | Stock: " << p.getQuantity() << " units\n";
            found = true;
        }
    }
    if (!found) std::cout << "No items mapped under the queried organizational node.\n";
    std::cout << "-------------------------------------------------------------------\n";
}

void InventorySystem::makeSale()
{
    std::cout << "\n==================== TRANSACTION MANAGEMENT TERMINAL ====================\n";
    
    // Подобрение: Показва всички продукти в реално време преди започване на продажбата
    std::cout << "Available Active Products List:\n";
    if (products.empty()) {
        std::cout << "   [No items available in storage blueprint]\n";
    } else {
        for (const auto& p : products) {
            std::cout << "   ID: " << p.getId() << " | Name: " << p.getName() 
                      << " | Price: " << p.getPrice() << " $ | In Stock: " << p.getQuantity() << " units\n";
        }
    }
    std::cout << "-----------------------------------------------------------------------\n";
    std::cout << "(Type 'exit' at any time inside the ID prompt to complete and print invoice)\n\n";

    Transaction t(TransactionType::SALE);
    std::string inputId;
    int amount;

    while (true) {
        std::cout << "Enter Product ID to purchase (or 'exit'): ";
        std::cin >> inputId;

        if (toLower(inputId) == "exit") {
            break; // Изход от режима на маркиране
        }

        Product* p = findProductById(inputId);
        if (!p) {
            std::cout << "[ERROR]: Reference key non-existent. Please try again or type 'exit'.\n\n";
            continue; // Продължава цикъла без да прекъсва сметката
        }

        std::cout << "-> Selected: '" << p->getName() << "' | Units Available: " << p->getQuantity() << "\n";
        std::cout << "Enter quantity to sell: ";
        
        if (!(std::cin >> amount) || amount <= 0) {
            std::cout << "[ERROR]: Invalid quantity format specification.\n\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue; // Продължава напред
        }

        try {
            p->reduceQuantity(amount);
            t.addItem(TransactionItem(p->getName(), p->getPrice(), amount));
            std::cout << "-> Linked: Successfully added to active transaction list.\n\n";
        }
        catch (const std::exception& errorEntity) {
            std::cout << "[REJECTED]: " << errorEntity.what() << "\n\n";
        }
    }

    if (t.getTotal() > 0) {
        transactions.push_back(t);
        std::cout << "\nCheckout completed. Printing final transaction receipt:\n";
        t.printReceipt();
    } else {
        std::cout << "Notice: Empty basket transaction discarded.\n";
    }
}

void InventorySystem::restock()
{
    std::cout << "\n==================== RESTOCK LOGISTICS PROCUREMENT ====================\n";
    
    std::cout << "Available Products in Warehouse Inventory:\n";
    if (products.empty()) {
        std::cout << "   [No products registered in the database yet]\n";
        return;
    } else {
        for (const auto& p : products) {
            std::cout << "   ID: " << p.getId() << " | Name: " << p.getName() 
                      << " | Current Stock: " << p.getQuantity() << " units | Shelf Price: " << p.getPrice() << " $\n";
        }
    }
    std::cout << "-----------------------------------------------------------------------\n";

    std::string id;
    std::cout << "Enter product ID to replenish: ";
    std::cin >> id;

    Product* p = findProductById(id);
    if (!p) {
        std::cout << "Error: Target asset reference missing from infrastructure system.\n";
        return;
    }

    int amount;
    std::cout << "Enter replenishment quantity volume: ";
    if (!(std::cin >> amount) || amount <= 0) {
        std::cout << "Invalid quantity entry. Procurement aborted.\n";
        std::cin.clear(); std::cin.ignore(10000, '\n');
        return;
    }

    double customSupplyPrice;
    std::cout << "Enter actual unit supply purchase price (Cost from supplier): ";
    if (!(std::cin >> customSupplyPrice) || customSupplyPrice < 0) {
        std::cout << "Invalid supply price entry. Procurement aborted.\n";
        std::cin.clear(); std::cin.ignore(10000, '\n');
        return;
    }

    double totalCost = customSupplyPrice * amount;
    if (totalCost > budget) {
        std::cout << "PROCUREMENT REJECTED: Total cost (" << totalCost 
                  << " $) exceeds available corporate budget (" << budget << " $).\n";
        return;
    }

    // Промяна: Винаги иска изрично потвърждение преди задействане на покупката
    char confirmChoice;
    std::cout << "\nSummary: You are about to transfer " << totalCost << " $ from the corporate balance sheet.\n";
    std::cout << "Confirm corporate authorization payment? (y/n): ";
    std::cin >> confirmChoice;
    if (confirmChoice != 'y' && confirmChoice != 'Y') {
        std::cout << "Procurement explicit cancelled by administrative bypass control.\n";
        return;
    }

    budget -= totalCost;
    p->increaseQuantity(amount);

    Transaction t(TransactionType::RESTOCK);
    t.addItem(TransactionItem(p->getName(), customSupplyPrice, amount));
    transactions.push_back(t);

    std::cout << "SUCCESS: Procured " << amount << " units of '" << p->getName() 
              << "' at " << customSupplyPrice << " $ each.\n";
    std::cout << "Remaining budget: " << budget << " $\n";
}

void InventorySystem::report() const
{
    std::cout << "\n==================== FINANCIAL PERFORMANCE REPORT ====================\n";
    std::cout << "Current Corporate Liquidity Budget: " << budget << " $\n";
    std::cout << "Total Active Stock Ledger Items   : " << products.size() << " products\n";
    std::cout << "----------------------------------------------------------\n";

    const Product* bestSelling = nullptr;
    const Product* mostSailed = nullptr;

    for (const auto& p : products)
    {
        if (p.getTotalUnitsSold() > 0) {
            if (!bestSelling || p.getTotalRevenueGenerated() > bestSelling->getTotalRevenueGenerated()) {
                bestSelling = &p;
            }
            if (!mostSailed || p.getTotalUnitsSold() > mostSailed->getTotalUnitsSold()) {
                mostSailed = &p;
            }
        }
    }

    if (bestSelling) {
        std::cout << "Bestselling Product (Highest Revenue): " << bestSelling->getName() 
                  << " (\"" << bestSelling->getTotalRevenueGenerated() << " $\" generated)\n";
    } else {
        std::cout << "Bestselling Product: N/A\n";
    }

    if (mostSailed) {
        std::cout << "Most Sold Product (Highest volume shipped): " << mostSailed->getName() 
                  << " (" << mostSailed->getTotalUnitsSold() << " units)\n";
    } else {
        std::cout << "Most Sold Product: N/A\n";
    }
    std::cout << "==========================================================\n";
}

void InventorySystem::showTransactions(int filterChoice) const
{
    std::cout << "\n==================== STRATIFIED LOG HISTORIES ====================\n";
    int counter = 1;
    for (const auto& t : transactions) {
        if ((filterChoice == 1 && t.getType() == TransactionType::SALE) ||
            (filterChoice == 2 && t.getType() == TransactionType::RESTOCK) ||
            (filterChoice == 3)) {
            std::cout << "Transaction #" << counter << "\n";
            t.printReceipt();
        }
        counter++;
    }
}

void InventorySystem::lowStockWarning(int limit) const
{
    std::cout << "\n==================== TRIGGERED ALERT RADAR ====================\n";
    bool triggered = false;
    for (const auto& p : products) {
        if (p.getQuantity() <= limit) {
            std::cout << "[WARN] ID: " << p.getId() << " | Name: " << p.getName() 
                      << " | Stock remaining: " << p.getQuantity() << " units!\n";
            triggered = true;
        }
    }
    if (!triggered) {
        std::cout << "All inventory stocks are safely above the minimum designated baseline.\n";
    }
    std::cout << "===============================================================\n";
}