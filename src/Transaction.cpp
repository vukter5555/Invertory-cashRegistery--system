#include "Transaction.h"
#include <iostream>

Transaction::Transaction(TransactionType type) : total(0.0), type(type) {}

void Transaction::addItem(const TransactionItem& item)
{
    items.push_back(item);
    total += item.getTotalPrice();
}

double Transaction::getTotal() const
{
    return total;
}

TransactionType Transaction::getType() const
{
    return type;
}

const std::vector<TransactionItem>& Transaction::getItems() const
{
    return items;
}

void Transaction::printReceipt() const
{
    std::cout << "------------------------------------\n";
    if (type == TransactionType::RESTOCK) {
        std::cout << "Receipt Details [VENDOR SUPPLY RESTOCK]:\n";
    } else {
        std::cout << "Receipt Details [CUSTOMER CHECKOUT SALE]:\n";
    }
    
    for (const auto& item : items)
    {
        item.print();
    }
    std::cout << "Total Value: " << total << " $\n";
    std::cout << "------------------------------------\n";
}