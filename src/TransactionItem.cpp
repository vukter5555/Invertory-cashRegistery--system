#include "TransactionItem.h"
#include <iostream>

TransactionItem::TransactionItem() : product(nullptr), quantity(0) {}

TransactionItem::TransactionItem(Product* product, int quantity)
    : product(product), quantity(quantity) {}

Product* TransactionItem::getProduct() const 
{
    return product;
}

int TransactionItem::getQuantity() const 
{
    return quantity;
}

double TransactionItem::getTotalPrice() const 
{
    return product ? (product->getPrice() * quantity) : 0.0;
}

void TransactionItem::print() const 
{
    if (product) 
    {
        std::cout << product->getName() << " x" << quantity 
                  << " | Total: " << getTotalPrice() << std::endl;
    }
}