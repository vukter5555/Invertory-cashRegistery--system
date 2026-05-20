#include "Category.h"

Category::Category() : name("") {}

Category::Category(const std::string& name)
    : name(name) {}

std::string Category::getName() const
{
    return name;
}

void Category::setName(const std::string& name)
{
    this->name = name;
}