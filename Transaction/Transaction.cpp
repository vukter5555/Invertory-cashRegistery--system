#include "Transaction.h"
#include <iostream>

Transaction::Transaction() : total(0.0) {}

void Transaction::addItem(const TransactionItem& item)
{
    items.push_back(item);
    total += item.getTotalPrice();
}

double Transaction::getTotal() const
{
    return total;
}

const std::vector<TransactionItem>& Transaction::getItems() const
{
    return items;
}

void Transaction::printReceipt() const
{
    std::cout << "------------------------------------\n";
    std::cout << "Receipt Details:\n";
    for (const auto& item : items)
    {
        item.print();
    }
    std::cout << "Total Due: " << total << " $\n";
    std::cout << "------------------------------------\n";
}