#pragma once

#include <vector>
#include "TransactionItem.h"

// Enum to separate customer checkouts from vendor replenishments
enum class TransactionType {
    SALE,
    RESTOCK
};

class Transaction
{
private:
    std::vector<TransactionItem> items;
    double total;
    TransactionType type; // Store the type of this transaction

public:
    // Constructor defaults to SALE if no type is explicitly given
    Transaction(TransactionType type = TransactionType::SALE);

    void addItem(const TransactionItem& item);

    double getTotal() const;
    TransactionType getType() const; // Getter to check filter state

    const std::vector<TransactionItem>& getItems() const;

    void printReceipt() const;
};