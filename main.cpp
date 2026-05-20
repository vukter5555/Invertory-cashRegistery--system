#include "InventorySystem.h"

int main()
{
    InventorySystem system;

    Category drinks("Drinks");
    Category snacks("Snacks");

    system.addCategory(drinks);
    system.addCategory(snacks);

    system.addProduct(Product(
        "Cola",
        2.5,
        10,
        drinks,
        "D1"
    ));

    system.addProduct(Product(
        "Water",
        1.2,
        20,
        drinks,
        "D2"
    ));

    system.addProduct(Product(
        "Chips",
        3.4,
        5,
        snacks,
        "S1"
    ));

    system.showProducts();

    return 0;
}