#pragma once

#include <vector>
#include "TransactionItem.h"

class Transaction
{
private:
    std::vector<TransactionItem> items;
    double total;

public:
    Transaction();

    void addItem(const TransactionItem& item);

    double getTotal() const;

    const std::vector<TransactionItem>& getItems() const;

    void printReceipt() const;
};