#pragma once
#include <vector>
#include <string>
#include "Product.h"
#include "Transaction.h"

class InventorySystem
{
private:
    std::vector<Product> products;
    std::vector<Category> categories;
    std::vector<Transaction> transactions;
    double budget;

    std::string toLower(std::string str) const;

public:
    InventorySystem();

    void addCategory(const Category& category);
    void addProduct(const Product& product);
    void removeProduct(const Product& product);
    void removeCategory(const Category& category);

    Product* findProductById(const std::string& id);
    Category* findCategoryByName(const std::string& name);

    bool updateProductInformation(const std::string& oldId, const std::string& newId, const std::string& newName, double newPrice);
    bool updateCategoryInformation(const std::string& oldName, const std::string& newName);

    void showProducts() const;
    void searchByName(const std::string& name) const;
    void searchByCategory(const std::string& category) const;

    void makeSale();
    void restock();

    void report() const;
    void showTransactions(int filterChoice) const;
    void lowStockWarning(int limit) const;
};