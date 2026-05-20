#pragma once
#include <string>

class Category
{
private:
    std::string name;

public:
    Category();
    Category(const std::string& name);

    std::string getName() const;
    void setName(const std::string& name);
};