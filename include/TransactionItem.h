#pragma once
#include <string>

class TransactionItem
{
private:
    std::string productName;
    double customSoldPrice;
    int quantity;

public:
    TransactionItem();
    // Architectural Fix: Snapshots name and actual transactional pricing dynamically 
    TransactionItem(const std::string& name, double soldPrice, int quantity);

    std::string getProductName() const;
    int getQuantity() const;
    double getTotalPrice() const;

    void print() const;
};