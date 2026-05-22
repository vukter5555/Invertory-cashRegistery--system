#include "Product.h"
#include <stdexcept>

Product::Product()
    : name(""), price(0.0), quantity(0), id(""), category(""), totalUnitsSold(0), totalRevenueGenerated(0.0)
{}

Product::Product(
    const std::string& name,
    double price,
    int quantity,
    const Category& category,
    const std::string& id
)
    : name(name), price(price), quantity(quantity), id(id), category(category), totalUnitsSold(0), totalRevenueGenerated(0.0)
{
    if (price < 0)
        throw std::out_of_range("Invalid price initialization parameter.");
    if (quantity < 0)
        throw std::out_of_range("Invalid inventory quantity configuration parameters.");
}

std::string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
int Product::getQuantity() const { return quantity; }
std::string Product::getId() const { return id; }
const Category& Product::getCategory() const { return category; }
int Product::getTotalUnitsSold() const { return totalUnitsSold; }
double Product::getTotalRevenueGenerated() const { return totalRevenueGenerated; }

void Product::setName(const std::string& name) { this->name = name; }

void Product::setId(const std::string& id) 
{ 
    this->id = id; 
}

void Product::setPrice(double price)
{
    if (price < 0) throw std::out_of_range("Price cannot be a negative value.");
    this->price = price;
}

void Product::setQuantity(int quantity)
{
    if (quantity < 0) throw std::out_of_range("Quantity cannot fall below zero units.");
    this->quantity = quantity;
}

void Product::setCategory(const Category& category) { this->category = category; }

void Product::reduceQuantity(int amount)
{
    if (amount <= 0)
        throw std::invalid_argument("Amount to deduct must be positive.");
    if (amount > quantity)
        throw std::out_of_range("Insufficient stock level available for transaction execution.");

    quantity -= amount;
    totalUnitsSold += amount;
    
    totalRevenueGenerated += (price * amount);
}

void Product::increaseQuantity(int amount)
{
    if (amount <= 0)
        throw std::invalid_argument("Restock replenishment volume must be positive.");
    quantity += amount;
}