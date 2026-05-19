#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

using namespace std;

class Category
{
private:
    string name;

public:
    Category() : name("") {}
    Category(string name) : name(name) {}

    string getname() const { return name; }

    void changename(string name) { this->name = name; }
};

class Product
{
private:
    string name;
    double price;
    int quantity;
    string id;
    Category category;

public:
    Product()
        : name(""), price(0), quantity(0), category(""), id("") {}

    Product(string name, double price, int quantity, Category category, string id)
        : name(name), price(price), quantity(quantity), category(category), id(id)
    {
        if(price < 0) throw out_of_range("Price must be positive.");
        if(quantity < 0) throw out_of_range("Quantity must be positive.");
    }

    string getname() const { return name; }
    double getprice() const { return price; }
    int getquantity() const { return quantity; }
    string getId() const { return id; }
    const Category& getCategory() const { return category; }

    void changename(string name) { this->name = name; }

    void changeprice(double price)
    {
        if(price < 0) throw out_of_range("Price must be positive.");
        this->price = price;
    }

    void changequantity(int quantity)
    {
        if(quantity < 0) throw out_of_range("Quantity must be positive.");
        this->quantity = quantity;
    }

    void changeCategory(Category category)
    {
        this->category = category;
    }

    void reduceQuantity(int q)
    {
        if(q > quantity) throw out_of_range("Not enough quantity.");
        quantity -= q;
    }
};

class TransactionItem
{
private:
    Product* product;
    int quantity;

public:
    TransactionItem() : product(nullptr), quantity(0) {}

    TransactionItem(Product* product, int quantity)
        : product(product), quantity(quantity)
    {
        if(quantity <= 0)
            throw out_of_range("Quantity must be greater than 0.");
    }

    double getTotalPrice() const
    {
        return product->getprice() * quantity;
    }

    int getQuantity() const { return quantity; }

    Product* getProduct() const { return product; }

    void print() const
    {
        cout << product->getname()
             << " x" << quantity
             << " = " << getTotalPrice()
             << " lv\n";
    }
};

class Transaction
{
private:
    vector<TransactionItem> items;
    double total;

public:
    Transaction() : total(0) {}

    void addItem(const TransactionItem& item)
    {
        items.push_back(item);
        total += item.getTotalPrice();
    }

    double getTotal() const { return total; }

    const vector<TransactionItem>& getItems() const { return items; }

    void printReceipt() const
    {
        cout << "\n===== RECEIPT =====\n";

        for(const auto& item : items)
            item.print();

        cout << "-------------------\n";
        cout << "TOTAL: " << total << " lv\n";
    }
};

class InventorySystem
{
private:
    vector<Product> products;
    vector<Category> categories;
    vector<Transaction> transactions;
    double budget;

public:
    InventorySystem() : budget(0) {}

    void addCategory(const Category& category)
    {
        categories.push_back(category);
    }

    void addProduct(const Product& product)
    {
        products.push_back(product);
    }

    Product* findProductById(const string& id)
    {
        for(auto& p : products)
            if(p.getId() == id)
                return &p;

        return nullptr;
    }

    void removeProduct(const string& id)
    {
        for(auto it = products.begin(); it != products.end(); ++it)
        {
            if(it->getId() == id)
            {
                products.erase(it);
                return;
            }
        }
    }

    void showProducts() const
    {
        cout << "\n===== PRODUCTS =====\n";

        for(const auto& p : products)
        {
            cout << setw(10) << p.getId()
                 << setw(15) << p.getname()
                 << setw(10) << p.getprice()
                 << setw(10) << p.getquantity()
                 << setw(15) << p.getCategory().getname()
                 << endl;
        }
    }

    void searchByName(const string& name) const
    {
        cout << "\n===== SEARCH RESULTS =====\n";

        for(const auto& p : products)
            if(p.getname() == name)
                cout << p.getname() << " | " << p.getprice() << " | " << p.getquantity() << endl;
    }

    void searchByCategory(const string& categoryName) const
    {
        cout << "\n===== SEARCH RESULTS =====\n";

        for(const auto& p : products)
            if(p.getCategory().getname() == categoryName)
                cout << p.getname() << " | " << p.getprice() << " | " << p.getquantity() << endl;
    }

    void lowStockWarning(int limit) const
    {
        cout << "\n===== LOW STOCK =====\n";

        for(const auto& p : products)
            if(p.getquantity() <= limit)
                cout << p.getname() << " only has " << p.getquantity() << " left.\n";
    }

    void makeSale()
    {
        Transaction transaction;

        while(true)
        {
            string id;
            cout << "\nEnter product ID (or done): ";
            cin >> id;

            if(id == "done") break;

            Product* product = findProductById(id);

            if(!product)
            {
                cout << "Product not found.\n";
                continue;
            }

            int quantity;
            cout << "Enter quantity: ";
            cin >> quantity;

            try
            {
                product->reduceQuantity(quantity);
                TransactionItem item(product, quantity);
                transaction.addItem(item);
                budget += item.getTotalPrice();
                cout << "Added.\n";
            }
            catch(exception& e)
            {
                cout << e.what() << endl;
            }
        }

        transactions.push_back(transaction);
        transaction.printReceipt();
    }

    void restock()
    {
        while(true)
        {
            string id;
            cout << "\nEnter product ID (or done): ";
            cin >> id;

            if(id == "done") break;

            Product* product = findProductById(id);

            if(!product)
            {
                cout << "Product not found.\n";
                continue;
            }

            int quantity;
            cout << "Enter quantity to add: ";
            cin >> quantity;

            try
            {
                if(quantity <= 0)
                    throw out_of_range("Quantity must be positive.");

                product->changequantity(product->getquantity() + quantity);
                cout << "Stock updated.\n";
            }
            catch(exception& e)
            {
                cout << e.what() << endl;
            }
        }
    }

    void showTransactions() const
    {
        cout << "\n===== TRANSACTION HISTORY =====\n";

        for(const auto& t : transactions)
            cout << "Transaction total: " << t.getTotal() << " lv\n";
    }

    void report() const
    {
        double revenue = 0;
        string bestProduct = "";
        int bestQty = 0;

        vector<string> names;
        vector<int> qty;

        for(const auto& t : transactions)
        {
            revenue += t.getTotal();

            for(const auto& item : t.getItems())
            {
                string n = item.getProduct()->getname();
                int q = item.getQuantity();

                bool found = false;

                for(size_t i = 0; i < names.size(); i++)
                {
                    if(names[i] == n)
                    {
                        qty[i] += q;
                        found = true;
                        break;
                    }
                }

                if(!found)
                {
                    names.push_back(n);
                    qty.push_back(q);
                }
            }
        }

        for(size_t i = 0; i < names.size(); i++)
        {
            if(qty[i] > bestQty)
            {
                bestQty = qty[i];
                bestProduct = names[i];
            }
        }

        cout << "\n===== REPORT =====\n";
        cout << "Revenue: " << revenue << " lv\n";
        cout << "Budget: " << budget << " lv\n";

        if(!bestProduct.empty())
            cout << "Best selling: " << bestProduct << " (" << bestQty << ")\n";
    }
};

int main()
{
    InventorySystem system;

    Category drinks("Drinks");
    Category snacks("Snacks");

    system.addCategory(drinks);
    system.addCategory(snacks);

    system.addProduct(Product("Cola", 2.5, 10, drinks, "D1"));
    system.addProduct(Product("Water", 1.2, 20, drinks, "D2"));
    system.addProduct(Product("Chips", 3.4, 5, snacks, "S1"));

    int choice;

    do
    {
        cout << "\n1.Show 2.SearchName 3.SearchCat 4.Sale 5.LowStock 6.History 7.Report 8.Restock 0.Exit\n";
        cin >> choice;

        switch(choice)
        {
        case 1: system.showProducts(); break;
        case 2:
        {
            string n; cin >> n;
            system.searchByName(n);
            break;
        }
        case 3:
        {
            string c; cin >> c;
            system.searchByCategory(c);
            break;
        }
        case 4: system.makeSale(); break;
        case 5: system.lowStockWarning(5); break;
        case 6: system.showTransactions(); break;
        case 7: system.report(); break;
        case 8: system.restock(); break;
        }
    } while(choice != 0);

    return 0;
}