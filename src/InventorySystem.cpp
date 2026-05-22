#include "InventorySystem.h"
#include <iostream>
#include <iomanip>

InventorySystem::InventorySystem() : budget(1000.0) {}

void InventorySystem::addProduct(const Product& p)
{
    products.push_back(p);
}

void InventorySystem::removeProduct(const Product& p)
{
    bool found = false;
    for (size_t i = 0; i < products.size(); i++)
    {
        if (products[i].getId() == p.getId())
        {
            products.erase(products.begin() + i);
            found = true;
            i--; 
        }
    }
    if (found) std::cout << "Product removed successfully.\n";
    else std::cout << "Product not found.\n";
}

void InventorySystem::addCategory(const Category& c)
{
    categories.push_back(c);
}

void InventorySystem::removeCategory(const Category& category)
{
    bool categoryFound = false;
    for (size_t i = 0; i < categories.size(); i++)
    {
        if (categories[i].getName() == category.getName())
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
        if (products[i].getCategory().getName() == category.getName())
        {
            products.erase(products.begin() + i);
            i--; 
        }
    }
    std::cout << "Category and its cascading products removed cleanly.\n";
}

bool InventorySystem::updateProductInformation(const std::string& oldId, const std::string& newId, const std::string& newName, double newPrice)
{
    Product* target = findProductById(oldId);
    if (!target) return false;

    // Enforce ID uniqueness across all items
    if (oldId != newId && findProductById(newId) != nullptr) {
        std::cout << "Error: The target new ID choice is already mapped to another item!\n";
        return false;
    }

    target->setName(newName);
    target->setPrice(newPrice);
    target->setQuantity(target->getQuantity()); // Reset bounds integrity checks
    
    // Perform cascading updates if ID changes
    if(oldId != newId) {
        // Safe pointer reassignment loop
        Product replacement(newName, newPrice, target->getQuantity(), target->getCategory(), newId);
        removeProduct(*target);
        addProduct(replacement);
    }
    return true;
}

bool InventorySystem::updateCategoryInformation(const std::string& oldName, const std::string& newName)
{
    Category* cat = findCategoryByName(oldName);
    if (!cat) return false;

    if (oldName != newName && findCategoryByName(newName) != nullptr) {
        std::cout << "Error: Category '" << newName << "' already exists!\n";
        return false;
    }

    cat->setName(newName);

    // Propagate changes to items assigned to this category
    for (auto& p : products) {
        if (p.getCategory().getName() == oldName) {
            p.setCategory(*cat);
        }
    }
    return true;
}

void InventorySystem::showProducts() const
{
    std::cout << "\n============================== PRODUCTS ==============================\n";
    std::cout << std::left << std::setw(8) << "ID" 
              << std::setw(18) << "Name" 
              << std::setw(15) << "Category"
              << std::setw(12) << "Price" 
              << std::setw(10) << "Quantity" << "\n";
    std::cout << "----------------------------------------------------------------------\n";

    for (const auto& p : products)
    {
        std::cout << std::left << std::setw(8) << p.getId()
                  << std::setw(18) << p.getName()
                  << std::setw(15) << p.getCategory().getName()
                  << std::setw(12) << p.getPrice()
                  << std::setw(10) << p.getQuantity() << std::endl;
    }

    std::cout << "\n==================== REGISTERED CATEGORIES ====================\n";
    if (categories.empty()) std::cout << "(No categories registered yet)\n";
    else {
        for (const auto& c : categories) {
            std::cout << "- " << c.getName() << "\n";
        }
    }
}

Product* InventorySystem::findProductById(const std::string& id)
{
    for (auto& p : products) { if (p.getId() == id) return &p; }
    return nullptr;
}

Category* InventorySystem::findCategoryByName(const std::string& name)
{
    for (auto& c : categories) { if (c.getName() == name) return &c; }
    return nullptr;
}

void InventorySystem::searchByName(const std::string& name) const
{
    bool found = false;
    for (const auto& p : products) {
        if (p.getName() == name) {
            std::cout << "ID: " << p.getId() << " | Category: " << p.getCategory().getName() << " | Price: " << p.getPrice() << " | Stock: " << p.getQuantity() << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No items found.\n";
}

void InventorySystem::searchByCategory(const std::string& categoryName) const
{
    if (!const_cast<InventorySystem*>(this)->findCategoryByName(categoryName)) {
        std::cout << "Category does not exist.\n";
        return;
    }
    for (const auto& p : products) {
        if (p.getCategory().getName() == categoryName) {
            std::cout << "ID: " << p.getId() << " | Name: " << p.getName() << " | Price: " << p.getPrice() << "\n";
        }
    }
}

void InventorySystem::makeSale()
{
    Transaction currentTransaction(TransactionType::SALE); 
    bool itemsAdded = false;

    while (true)
    {
        std::string id;
        std::cout << "\nEnter product ID to sell (type exit to stop): ";
        std::cin >> id;
        if (id == "exit") break;

        Product* p = findProductById(id);
        if (!p) {
            std::cout << "Product not found.\n";
            continue; 
        }

        int qty;
        std::cout << "Enter quantity: ";
        if (!(std::cin >> qty)) {
            std::cin.clear(); std::cin.ignore(10000, '\n');
            continue;
        }

        try {
            p->reduceQuantity(qty);
            TransactionItem item(p, qty);
            currentTransaction.addItem(item);
            itemsAdded = true;
            std::cout << "Added to cart.\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    if (itemsAdded) {
        budget += currentTransaction.getTotal();
        transactions.push_back(currentTransaction);
        currentTransaction.printReceipt();
    }
}

void InventorySystem::restock()
{
    std::string id; int qty;
    std::cout << "\nEnter product ID to restock: "; std::cin >> id;
    std::cout << "Enter quantity: "; 
    if (!(std::cin >> qty)) { std::cin.clear(); std::cin.ignore(10000, '\n'); return; }

    Product* p = findProductById(id);
    if (!p) return;

    double spend = qty * p->getPrice();
    if (spend > budget) return;

    try {
        p->increaseQuantity(qty);
        budget -= spend; 
        Transaction restockTx(TransactionType::RESTOCK);
        TransactionItem item(p, qty);
        restockTx.addItem(item);
        transactions.push_back(restockTx); 
        std::cout << "Restocked successfully.\n";
    } catch (...) {}
}

void InventorySystem::report() const
{
    std::cout << "\n==================== ANALYTICS REPORT ====================\n";
    std::cout << "Current Budget / Cash Reserve: " << budget << " $\n";
    std::cout << "Transactions Settled: " << transactions.size() << "\n";

    const Product* bestSelling = nullptr; // highest revenue
    const Product* mostSailed = nullptr;   // highest total units sold

    for (const auto& p : products) {
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
                  << " (" << bestSelling->getTotalRevenueGenerated() << " $ generated)\n";
    } else {
        std::cout << "Bestselling Product: N/A (No sales transactions recorded)\n";
    }

    if (mostSailed) {
        std::cout << "Most Sold Product (Highest Volume): " << mostSailed->getName() 
                  << " (" << mostSailed->getTotalUnitsSold() << " units shipped)\n";
    } else {
        std::cout << "Most Sold Product: N/A\n";
    }
    std::cout << "==========================================================\n";
}

void InventorySystem::showTransactions(int filterChoice) const
{
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
    for (const auto& p : products) {
        if (p.getQuantity() <= limit) {
            std::cout << "Low Stock -> ID: " << p.getId() << " | Name: " << p.getName() << " | Qty: " << p.getQuantity() << "\n";
        }
    }
}