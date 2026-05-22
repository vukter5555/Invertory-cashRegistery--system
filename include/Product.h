#pragma once
#include <string>
#include "Category.h"

class Product
{
private:
    std::string name;
    double price;
    int quantity;
    std::string id;
    Category category;

    // Financial performance tracking counters
    int totalUnitsSold;
    double totalRevenueGenerated;

public:
    Product();
    Product(
        const std::string& name,
        double price,
        int quantity,
        const Category& category,
        const std::string& id
    );

    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;
    std::string getId() const;
    const Category& getCategory() const;

    int getTotalUnitsSold() const;
    double getTotalRevenueGenerated() const;

    void setName(const std::string& name);
    void setPrice(double price);
    void setQuantity(int quantity);
    void setCategory(const Category& category);

    void reduceQuantity(int amount);
    void increaseQuantity(int amount);
};