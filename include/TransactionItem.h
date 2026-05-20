#pragma once

#include "Product.h"

class TransactionItem
{
private:
    Product* product;
    int quantity;

public:
    TransactionItem();
    TransactionItem(Product* product, int quantity);

    Product* getProduct() const;
    int getQuantity() const;

    double getTotalPrice() const;

    void print() const;
};