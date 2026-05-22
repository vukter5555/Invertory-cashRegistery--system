#pragma once

#include <vector>
#include "Product.h"
#include "Transaction.h"

class InventorySystem
{
private:
    std::vector<Product> products;
    std::vector<Category> categories;
    std::vector<Transaction> transactions;

    double budget;

public:
    InventorySystem();

    void addCategory(const Category& category);
    void addProduct(const Product& product);
    void removeProduct(const Product& product);
    void removeCategory(const Category& category);


    Product* findProductById(const std::string& id);

    void showProducts() const;

    void searchByName(const std::string& name) const;
    void searchByCategory(const std::string& category) const;

    void makeSale();
    void restock();

    void report() const;
    
    // UPDATED: Accepts filter integer selection from the menu
    void showTransactions(int filterChoice) const;

    void lowStockWarning(int limit) const;
};