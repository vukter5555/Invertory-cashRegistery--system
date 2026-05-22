#include "TransactionItem.h"
#include <iostream>

TransactionItem::TransactionItem() : productName(""), customSoldPrice(0.0), quantity(0) {}

TransactionItem::TransactionItem(const std::string& name, double soldPrice, int quantity)
    : productName(name), customSoldPrice(soldPrice), quantity(quantity) {}

std::string TransactionItem::getProductName() const { return productName; }
int TransactionItem::getQuantity() const { return quantity; }
double TransactionItem::getTotalPrice() const { return customSoldPrice * quantity; }

void TransactionItem::print() const 
{
    std::cout << productName << " x" << quantity 
              << " | Total: " << getTotalPrice() << " $" << std::endl;
}