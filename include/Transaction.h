#pragma once
#include <vector>
#include "TransactionItem.h"

enum class TransactionType {
    SALE,
    RESTOCK
};

class Transaction
{
private:
    std::vector<TransactionItem> items;
    double total;
    TransactionType type;

public:
    Transaction(TransactionType type = TransactionType::SALE);

    void addItem(const TransactionItem& item);
    double getTotal() const;
    TransactionType getType() const;

    const std::vector<TransactionItem>& getItems() const;
    void printReceipt() const;
};