#include "Product.h"
#include <stdexcept>

Product::Product()
    : name(""),
      price(0),
      quantity(0),
      id(""),
      category("")
{
}

Product::Product(
    const std::string& name,
    double price,
    int quantity,
    const Category& category,
    const std::string& id
)
    : name(name),
      price(price),
      quantity(quantity),
      id(id),
      category(category)
{
    if(price < 0)
        throw std::out_of_range("Invalid price.");

    if(quantity < 0)
        throw std::out_of_range("Invalid quantity.");
}

std::string Product::getName() const
{
    return name;
}

double Product::getPrice() const
{
    return price;
}

int Product::getQuantity() const
{
    return quantity;
}

std::string Product::getId() const
{
    return id;
}

const Category& Product::getCategory() const
{
    return category;
}

void Product::setName(const std::string& name)
{
    this->name = name;
}

void Product::setPrice(double price)
{
    if(price < 0)
        throw std::out_of_range("Invalid price.");

    this->price = price;
}

void Product::setQuantity(int quantity)
{
    if(quantity < 0)
        throw std::out_of_range("Invalid quantity.");

    this->quantity = quantity;
}

void Product::setCategory(const Category& category)
{
    this->category = category;
}

void Product::reduceQuantity(int amount)
{
    if(amount < 0 || quantity < amount)
        throw std::out_of_range("Not enough stock or invalid amount.");
    quantity -= amount;
}

void Product::increaseQuantity(int amount)
{
    if(amount < 0)
        throw std::out_of_range("Invalid amount.");
    quantity += amount;
}