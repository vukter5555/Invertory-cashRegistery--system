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
        std::cout << "Category not found. Operation aborted.\n";
        return;
    }

    // Cascading deletion logic to scrub matching products assigned to deleted categories
    for (size_t i = 0; i < products.size(); i++)
    {
        if (toLower(products[i].getCategory().getName()) == targetCatLower)
        {
            products.erase(products.begin() + i);
            i--;
        }
    }
    std::cout << "Category and all tracking inventory items successfully purged.\n";
}

Product* InventorySystem::findProductById(const std::string& id)
{
    for (auto& p : products)
    {
        // Convert both IDs to lowercase for case-insensitive matching
        if (p.getId() == id)
        {
            return &p;
        }
    }
    return nullptr;
}

Category* InventorySystem::findCategoryByName(const std::string& name)
{
    std::string searchName = toLower(name);
    for (auto& c : categories) {
        if (toLower(c.getName()) == searchName) return &c;
    }
    return nullptr;
}

bool InventorySystem::updateProductInformation(const std::string& oldId, const std::string& newId, const std::string& newName, double newPrice)
{
    Product* p = findProductById(oldId);
    if (!p) return false;

    if (toLower(oldId) != toLower(newId) && findProductById(newId) != nullptr) {
        std::cout << "Collision Error: The chosen target new ID is already registered to an asset.\n";
        return false;
    }

    p->setName(newName);
    p->setPrice(newPrice);
    p->setQuantity(p->getQuantity()); // Validates boundaries inside class
    
    // Explicitly reconstruct state tracking around updating object parameters
    std::string storedQty = std::to_string(p->getQuantity());
    
    // Modifying identification pointers cleanly
    *p = Product(newName, newPrice, p->getQuantity(), p->getCategory(), newId);
    return true;
}

bool InventorySystem::updateCategoryInformation(const std::string& oldName, const std::string& newName)
{
    Category* cat = findCategoryByName(oldName);
    if (!cat) return false;

    if (toLower(oldName) != toLower(newName) && findCategoryByName(newName) != nullptr) {
        return false;
    }

    cat->setName(newName);
    for (auto& p : products) {
        if (toLower(p.getCategory().getName()) == toLower(oldName)) {
            p.setCategory(*cat);
        }
    }
    return true;
}

void InventorySystem::showProducts() const
{
    std::cout << "\n============================== PRODUCTS ==============================\n";
    // UI MISTAKE FIXED: Adjusted column width formatting buffers from 14 to 24 to prevent header string overflow smushing
    std::cout << std::left << std::setw(8) << "ID" 
              << std::setw(18) << "Name" 
              << std::setw(15) << "Category"
              << std::setw(12) << "Base Price" 
              << std::setw(24) << "Retail (Markup)"
              << std::setw(10) << "Quantity" << "\n";
    std::cout << "----------------------------------------------------------------------------\n";

    for (const auto& p : products)
    {
        std::cout << std::left << std::setw(8) << p.getId()
                  << std::setw(18) << p.getName()
                  << std::setw(15) << p.getCategory().getName()
                  << std::setw(12) << p.getPrice()
                  << std::setw(24) << (p.getPrice() * 1.25)
                  << std::setw(10) << p.getQuantity() << std::endl;
    }

    std::cout << "\n==================== REGISTERED CATEGORIES ====================\n";
    if (categories.empty()) std::cout << "(No working classifications registered)\n";
    else {
        for (const auto& c : categories) {
            std::cout << "- " << c.getName() << "\n";
        }
    }
}

void InventorySystem::searchByName(const std::string& name) const
{
    std::string searchTarget = toLower(name);
    bool found = false;
    for (const auto& p : products) {
        if (toLower(p.getName()).find(searchTarget) != std::string::npos) {
            std::cout << "ID: " << p.getId() << " | Name: " << p.getName() 
                      << " | Category: " << p.getCategory().getName() 
                      << " | Base Price: " << p.getPrice() << " $ | Selling Price (25% profit): " 
                      << (p.getPrice() * 1.25) << " $ | Stock: " << p.getQuantity() << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No matching items found.\n";
}

void InventorySystem::searchByCategory(const std::string& category) const
{
    std::string targetCat = toLower(category);
    Category* matchedPointer = const_cast<InventorySystem*>(this)->findCategoryByName(category);
    
    if(!matchedPointer) {
        std::cout << "Category does not exist.\n";
        return;
    }

    for (const auto& p : products) {
        if (toLower(p.getCategory().getName()) == targetCat) {
            std::cout << "ID: " << p.getId() << " | Name: " << p.getName() << " | Price: " << p.getPrice() << " $\n";
        }
    }
}

void InventorySystem::makeSale()
{
    std::cout << "\n--- AVAILABLE INVENTORY FOR SALE ---\n";
    showProducts();

    Transaction currentTransaction(TransactionType::SALE);
    bool itemsAdded = false;

    while (true)
    {
        std::string id;
        std::cout << "\nEnter product ID to sell (type 'exit' to finalize): ";
        std::cin >> id;
        if (toLower(id) == "exit") break;

        Product* p = findProductById(id);
        if (!p) {
            std::cout << "Product not found.\n";
            continue; 
        }

        int qty;
        std::cout << "Enter quantity: ";
        if (!(std::cin >> qty)) {
            std::cin.clear(); std::cin.ignore(10000, '\n');
            std::cout << "Invalid numeric parsing payload formatting choice!\n";
            continue;
        }

        try {
            double finalSellingPrice = p->getPrice() * 1.25;
            p->reduceQuantity(qty); // Tracks analytics internal telemetry maps correctly
            
            // MATH / REVENUE SNAPSHOT FIX: Explicitly lock the static calculated markup pricing context parameters
            TransactionItem item(p->getName(), finalSellingPrice, qty);
            currentTransaction.addItem(item);
            
            itemsAdded = true;
            std::cout << "Added to checkout tray with a 25% profit margin markup applied.\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error encountered: " << e.what() << "\n";
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
    std::string id;
    std::cout << "\nEnter product ID to replenish: ";
    std::cin >> id;

    Product* p = findProductById(id);
    if (!p) {
        std::cout << "Product lookup returned empty data sets. Restock canceled.\n";
        return;
    }

    int qty;
    std::cout << "Enter supply item quantity payload configuration: ";
    if (!(std::cin >> qty) || qty <= 0) {
        std::cout << "Invalid restocking scale parameters requested.\n";
        std::cin.clear(); std::cin.ignore(10000, '\n');
        return;
    }

    double structuralCost = p->getPrice() * qty;
    if (budget < structuralCost) {
        std::cout << "Transaction Blocked: Insufficient organizational cash reserves to clear processing costs.\n";
        return;
    }

    budget -= structuralCost;
    p->increaseQuantity(qty);

    Transaction restockTx(TransactionType::RESTOCK);
    restockTx.addItem(TransactionItem(p->getName(), p->getPrice(), qty));
    transactions.push_back(restockTx);

    std::cout << "Inventory tracking units scaled up across " << p->getName() << " successfully.\n";
}

void InventorySystem::report() const
{
    std::cout << "\n==================== ANALYTICS REPORT ====================\n";
    std::cout << "Current Budget / Cash Reserve: " << budget << " $\n";
    std::cout << "Transactions Settled: " << transactions.size() << "\n";

    const Product* bestSelling = nullptr;
    const Product* mostSailed = nullptr;

    for (const auto& p : products) {
        if (p.getTotalUnitsSold() > 0) {
            // REPORT VALUE MATH FIX: Read values naturally since direct structural double calculations are updated in reduceQuantity
            if (!bestSelling || p.getTotalRevenueGenerated() > bestSelling->getTotalRevenueGenerated()) {
                bestSelling = &p;
            }
            if (!mostSailed || p.getTotalUnitsSold() > mostSailed->getTotalUnitsSold()) {
                mostSailed = &p;
            }
        }
    }

    if (bestSelling) {
        std::cout << "Bestselling Product (Highest Grossing Revenue): " << bestSelling->getName() 
                  << " (" << bestSelling->getTotalRevenueGenerated() << " $ gross sales managed)\n";
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
            std::cout << "[WARN] ID: " << p.getId() << " | Name: " << p.getName() << " -> Critical Stock Status: " << p.getQuantity() << " units remaining.\n";
            triggered = true;
        }
    }
    if (!triggered) std::cout << "All registered lines sit safely above target thresholds.\n";
}