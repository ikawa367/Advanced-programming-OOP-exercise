#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int transId = 1;

int convertString(string input)
{
    int result;
    stringstream argument(input);
    argument >> result;
    return result;
}

class Customer
{
    static list<Customer *> customers;
    string name;
    int id;
    int balance;
    int discountCode;
public:
    Customer(string name, int id)
    {
        this->name = name;
        this->id = id;
        this->balance = 0;
        this->discountCode = -1;
        customers.push_back(this);
    }

    static Customer *getCustomerByID(int id)
    {
        for (auto i = customers.begin(); i != customers.end(); i++)
        {
            if ((*i)->id == id)
            {
                return *i;
            }
        }
        return NULL;
    }

    void increaseCustomerBalance(int balance)
    { this->balance += balance; }

    void setBalance(int balance)
    { this->balance = balance; }

    int getBalance()
    { return this->balance; }

    int getID()
    { return this->id; }

    void setDiscountCode(int discountCode)
    { this->discountCode = discountCode; }

    int getDiscountCode()
    { return this->discountCode; }

    void decreaseBalance(int balance)
    { this->balance -= balance; }
};

list<Customer *> Customer::customers;

class Warehouse
{
    int amount;
    string materialName;
    static list<Warehouse *> warehouses;
public:
    Warehouse(string materialName, int amount)
    {
        this->materialName = materialName;
        this->amount = amount;
        Warehouse::warehouses.push_back(this);
    }

    void increaseMaterial(int amount)
    { this->amount += amount; }

    void setAmount(int amount)
    { this->amount = amount; }

    int getAmount()
    { return this->amount; }

    void decreaseMaterial(int amount)
    { this->amount -= amount; }

    static Warehouse *getWarehouseByName(string name)
    {
        for (auto i = warehouses.begin(); i != warehouses.end(); i++)
        {
            if ((*i)->materialName == name)
            {
                return *i;
            }
        }
        return NULL;

    };
};

list<Warehouse *>Warehouse::warehouses;

class Confectionary
{
    int balance;
    static map<int, int> discount;  //fist element is code and second element is price.
public:
    Confectionary()
    {
        this->balance = 0;
    }

    void setBalance(int balance)
    { this->balance = balance; }

    int getBalance()
    { return this->balance; }

    void increaseBalance(int balance)
    { this->balance += balance; }

    static bool isDiscountExists(int code)
    {
        return discount.find(code) != discount.end();
    }

    static void addDiscount(int code, int price)
    {
        if (isDiscountExists(code))
        {
            cout << "discount with this code already exists";
        } else
        {
            discount.insert(pair<int, int>(code, price));
        }
    }

    static int getDiscountPriceByCode(int code)
    {
        if (code == -1)
        {
            return 0;
        }
        return discount[code];
    }
};

map<int, int>Confectionary::discount;
Confectionary *ptr = new Confectionary;

class Transaction
{
    static int idCounter;
    int id;
    int customerId;
    int amount;
    int discountCode;
    int discountPrice;
    int finalPayment;
    bool isAccepted;
    static list<Transaction *> transactions;
public:
    Transaction(int customerId, int amount, int discountCode)
    {
        this->customerId = customerId;
        this->amount = amount;
        this->isAccepted = false;
        this->discountCode = discountCode;
        idCounter = transId;
        this->id = idCounter;
        transactions.push_back(this);
    }

    int getId()
    { return this->id; }

    void setAccepted(bool accepted)
    { this->isAccepted = accepted; }

    void exchangeMoney()
    {
        this->discountPrice = ptr->getDiscountPriceByCode(this->discountCode);
        if (this->discountCode != -1)
        {
            if (this->discountPrice >= this->amount)
            {
                this->finalPayment = 0;
            } else if (this->discountPrice < this->amount)
            {
                this->finalPayment = this->amount - this->discountPrice;
            }
        } else
        {
            this->finalPayment = this->amount;
        }
    };

    bool isTransactionAccepted()
    {
        if (this->isAccepted == 1)
        {
            return true;
        } else
        {
            return false;
        }
    }

    int getDiscountCode()
    { return this->discountCode; }

    int getAmount()
    { return this->amount; }

    int getFinalPayment()
    {
        exchangeMoney();
        return this->finalPayment;
    }

    int getCustomerId()
    { return this->customerId; }

    static Transaction *getTransactionByID(int id)
    {
        for (auto i = transactions.begin(); i != transactions.end(); i++)
        {
            if ((*i)->id == id)
            {
                return *i;
            }
        }
        return NULL;
    }

    static list<Transaction *> getTransactions()
    {
        return transactions;
    }
};

list<Transaction *> Transaction::transactions;
int Transaction::idCounter;

class Sweet
{
    string name;
    int price;
    int amount;
    map<string, int> materials;
    static list<Sweet *> sweets;
public:
    Sweet(string name, int price, map<string, int> materials)
    {
        this->name = name;
        this->price = price;
        this->materials = materials;
        this->amount = 0;
        sweets.push_back(this);
    }

    map<string, int> getMaterials()
    {
        return materials;
    }

    string getName()
    { return this->name; }

    void increaseSweet(int amount)
    { this->amount += amount; }

    int getAmount()
    { return this->amount; }

    void setAmount(int amount)
    { this->amount = amount; }

    int getPrice()
    { return this->price; }

    void decreaseMaterialOfSweetFromWarehouse(int amount)
    {
        for (map<string, int>::iterator show = materials.begin(); show != materials.end(); show++)
        {
            Warehouse::getWarehouseByName(show->first)->decreaseMaterial(show->second);
        }
    }

    static Sweet *getSweetByName(string name)
    {
        for (auto i = sweets.begin(); i != sweets.end(); i++)
        {
            if ((*i)->name == name)
            {
                return *i;
            }
        }
        return NULL;
    }
};

void seprator(const smatch &Matcher, map<string, int> &checker, vector<string> &materialSorted);

list<Sweet *>Sweet::sweets;

bool doesThisWarehouseExist(string input)
{
    if (Warehouse::getWarehouseByName(input) != NULL)
    {
        return 1;
    }
    return 0;
}

void addCustomer(smatch Matcher)
{
    string name = Matcher.str(2);
    int id = convertString(Matcher.str(1));
    if (Customer::getCustomerByID(id) != NULL)
    {
        cout << "customer with this id already exists" << endl;
    } else
    {
        new Customer(name, id);
    }
}

void chargeCustomerBalance(smatch Matcher)
{
    int id_argument = convertString(Matcher.str(1));
    int amount_argument = convertString(Matcher.str(2));
    if (Customer::getCustomerByID(id_argument) == NULL)
    {
        cout << "customer not found" << endl;
    } else
    {
        Customer::getCustomerByID(id_argument)->increaseCustomerBalance(amount_argument);
    }
}

void addWarehouse(smatch Matcher)
{
    string string_argument = Matcher.str(1); //name of material
    int int_argument = convertString(Matcher.str(2));  //amount of material
    if (Warehouse::getWarehouseByName(string_argument) != NULL)
    {
        cout << "warehouse having this material already exists" << endl;
    } else
    {
        new Warehouse(string_argument, int_argument); //add warehouse
    }
}

void increaseWarehouseMaterial(smatch Matcher)
{
    string string_argument = Matcher.str(1);
    int int_argument = convertString(Matcher.str(2));
    if (Warehouse::getWarehouseByName(string_argument) != NULL)
    {
        Warehouse::getWarehouseByName(string_argument)->increaseMaterial(int_argument);
    } else
    {
        cout << "warehouse not found" << endl;
    }
}

void addSweet(smatch Matcher)
{
    string name = Matcher.str(1);
    int price = convertString(Matcher.str(3));
    map<string, int> checker;
    vector<string> materialSorted;
    seprator(Matcher, checker, materialSorted);
    bool flag = false;
    string error = "not found warehouse(s):";
    for (auto counter = materialSorted.begin(); counter != materialSorted.end(); counter++)
    {
        if (doesThisWarehouseExist(*counter) == 0)
        {
            flag = true;
            error += " ";
            error += (*counter);
        }
    }
    if (flag)
    {
        cout << error << "\n";
    } else
    {
        new Sweet(name, price, checker);
    }
}

void seprator(const smatch &Matcher, map<string, int> &checker, vector<string> &materialSorted)
{
    smatch match;
    string in = Matcher.str(0);
    regex pre("[a-zA-Z0-9 ]*:"), reg("\\s([a-zA-Z0-9 ]+) (\\d+)");
    auto begin = in.cbegin();
    regex_search(begin, in.cend(), match, pre);
    begin = match.suffix().first;
    while (regex_search(begin, in.cend(), match, reg))
    {
        string Material = match.str(1);
        materialSorted.push_back(Material);
        int int_value = convertString(match.str(2));
        checker.insert(pair<string, int>(Material, int_value));
        begin = match.suffix().first;
    }
}

void increaseSweet(smatch Matcher)
{
    string string_argument = Matcher.str(1);
    int int_argument = convertString(Matcher.str(3));
    string error = "insufficient material(s):";
    bool flag = false;
    if (Sweet::getSweetByName(string_argument) != NULL)
    {
        map<string, int> temp = Sweet::getSweetByName(string_argument)->getMaterials();
        for (map<string, int>::iterator it = temp.begin(); it != temp.end(); it++)
        {
            if (Warehouse::getWarehouseByName(it->first)->getAmount() < (it->second * int_argument))
            {
                flag = true;
                error += " ";
                error += (it->first);
            }
        }
        if (flag)
        {
            cout << error << endl;
        } else
        {
            Sweet::getSweetByName(string_argument)->increaseSweet(int_argument);
            Sweet::getSweetByName(string_argument)->decreaseMaterialOfSweetFromWarehouse(int_argument);
        }
    } else
    {
        cout << "sweet not found\n";
    }
}

void addDiscount(smatch Matcher)
{
    int code_argument = convertString(Matcher.str(1));
    int price_argument = convertString(Matcher.str(2));
    if (Confectionary::isDiscountExists(code_argument))
    {
        cout << "discount with this code already exists" << endl;
    } else
    {
        Confectionary::addDiscount(code_argument, price_argument);
    }
}

void addDiscountToCustomer(smatch Matcher)
{
    int code_argument = convertString(Matcher.str(1));
    int id_argument = convertString(Matcher.str(2));
    if (Confectionary::isDiscountExists(code_argument) && Customer::getCustomerByID(id_argument) != NULL)
    {
        Customer::getCustomerByID(id_argument)->setDiscountCode(code_argument);
    } else if (!Confectionary::isDiscountExists(code_argument))
    {
        cout << "discount code not found" << endl;
    } else if (Customer::getCustomerByID(id_argument) == NULL)
    {
        cout << "customer not found" << endl;
    }
}

void sellSweet(smatch Matcher)
{
    string sweetName = Matcher.str(1);
    int amount = convertString(Matcher.str(3));
    int customer_id = convertString(Matcher.str(4));
    if (Sweet::getSweetByName(sweetName) != NULL)
    {
        int amountOfConfectionary = Sweet::getSweetByName(sweetName)->getAmount();
        int all_money = amount * Sweet::getSweetByName(sweetName)->getPrice();
        if (amountOfConfectionary < amount)
            cout << "insufficient sweet\n";
        else
        {
            if (Customer::getCustomerByID(customer_id) != NULL)
            {
                int customer_balance = Customer::getCustomerByID(customer_id)->getBalance();
                int discount_code = Customer::getCustomerByID(customer_id)->getDiscountCode();
                int discount_price = Confectionary::getDiscountPriceByCode(discount_code);
                if ((customer_balance + discount_price) < all_money)
                {
                    cout << "customer has not enough money\n";
                } else
                {
                    new Transaction(customer_id, all_money, Customer::getCustomerByID(customer_id)->getDiscountCode());
                    Sweet::getSweetByName(sweetName)->setAmount(amountOfConfectionary - amount);
                    Customer::getCustomerByID(customer_id)->setDiscountCode(-1);
                    Customer::getCustomerByID(customer_id)->decreaseBalance(all_money - discount_price);
                    cout << "transaction " << Transaction::getTransactionByID(transId)->getId()
                         << " successfully created\n";
                    transId++;
                }
            } else
                cout << "customer not found\n";
        }
    } else
        cout << "sweet not found\n";
}

void acceptTransaction(smatch Matcher)
{
    int transactionID = convertString(Matcher.str(1));
    if (Transaction::getTransactionByID(transactionID) != NULL)
    {
        if (Transaction::getTransactionByID(transactionID)->isTransactionAccepted())
        {
            cout << "no waiting transaction with this id was found\n";
        } else
        {
            Transaction::getTransactionByID(transactionID)->setAccepted(true);
            int finalPayment = Transaction::getTransactionByID(transactionID)->getFinalPayment();
            ptr->increaseBalance(finalPayment);
        }
    } else
    {
        cout << "no waiting transaction with this id was found\n";
    }
}

void printTransActions()
{
    for (int counter = 1; counter < transId; counter++)
    {
        if (Transaction::getTransactionByID(counter)->isTransactionAccepted())
        {
            int transaction_id = (Transaction::getTransactionByID(counter))->getId();
            int customer_id = (Transaction::getTransactionByID(counter))->getCustomerId();
            int paymentAmount = ((Transaction::getTransactionByID(counter))->getAmount());
            int discountCode = (Transaction::getTransactionByID(counter))->getDiscountCode();
            int finalPayment = (Transaction::getTransactionByID(counter))->getFinalPayment();
            cout << "transaction " << transaction_id << ": " << customer_id << " " << paymentAmount << " "
                 << discountCode << " " << finalPayment << "\n";
        }
    }
}

void printIncome()
{
    cout << ptr->getBalance() << endl;
}

void run()
{
    regex create_confectionary("^\\s*create confectionary\\s*$");
    regex add_customer("^\\s*add customer id (\\d+) name ([a-zA-z ]+)\\s*$");
    regex increase_balance_customer("^\\s*increase balance customer (\\d+) amount (\\d+)\\s*$");
    regex add_warehouse_material("^\\s*add warehouse material ([a-zA-Z ]+) amount (\\d+)\\s*$");
    regex increase_warehouse_material("^\\s*increase warehouse material ([a-zA-Z ]+) amount (\\d+)\\s*$");
    regex add_sweet("^\\s*add sweet name (\\w+(\\s\\w+)*) price "
                    "(\\d+) materials: (\\w+(\\s\\w+)*)\\s\\d+(, (\\w+(\\s\\w+)*)\\s\\d+)*\\s*$");
    regex increase_sweet("^\\s*increase sweet (\\w+(\\s\\w+)*) amount (\\d+)\\s*$");
    regex add_discount_code("^\\s*add discount code (\\d+) price (\\d+)\\s*$");
    regex add_discount_code_customer("^\\s*add discount code code (\\d+) to customer id (\\d+)\\s*$");
    regex sell_sweet("^\\s*sell sweet (\\w+(\\s\\w+)*) amount (\\d+) to customer (\\d+)\\s*$");
    regex print_income("^\\s*print income\\s*$");
    regex print_transactions("^\\s*print transactions list\\s*$");
    regex accept_transaction("^\\s*accept transaction (\\d+)\\s*$");
    bool confectionary_is_made = false;
    string input_command;
    while (input_command != "end")
    {
        input_command.clear();
        cin.clear();
        cin.sync();
        getline(cin, input_command);
        smatch saver;
        if (input_command == "end")
        {
            break;
        } else if (regex_match(input_command, create_confectionary) && confectionary_is_made == 0)
        {
            confectionary_is_made = true;
        } else if (regex_match(input_command, add_customer) && confectionary_is_made)
        {
            regex_match(input_command, saver, add_customer);
            addCustomer(saver);
        } else if (regex_match(input_command, accept_transaction) && confectionary_is_made)
        {
            regex_match(input_command, saver, accept_transaction);
            acceptTransaction(saver);
        } else if (regex_match(input_command, increase_balance_customer) && confectionary_is_made)
        {
            regex_match(input_command, saver, increase_balance_customer);
            chargeCustomerBalance(saver);
        } else if (regex_match(input_command, add_warehouse_material) && confectionary_is_made)
        {
            regex_match(input_command, saver, add_warehouse_material);
            addWarehouse(saver);
        } else if (regex_match(input_command, increase_warehouse_material) && confectionary_is_made)
        {
            regex_match(input_command, saver, increase_warehouse_material);
            increaseWarehouseMaterial(saver);
        } else if (regex_match(input_command, add_sweet) && confectionary_is_made)
        {
            regex_match(input_command, saver, add_sweet);
            addSweet(saver);
        } else if (regex_match(input_command, increase_sweet) && confectionary_is_made)
        {
            regex_match(input_command, saver, increase_sweet);
            increaseSweet(saver);
        } else if (regex_match(input_command, add_discount_code) && confectionary_is_made)
        {
            regex_match(input_command, saver, add_discount_code);
            addDiscount(saver);
        } else if (regex_match(input_command, add_discount_code_customer) && confectionary_is_made)
        {
            regex_match(input_command, saver, add_discount_code_customer);
            addDiscountToCustomer(saver);
        } else if (regex_match(input_command, sell_sweet) && confectionary_is_made)
        {
            regex_match(input_command, saver, sell_sweet);
            sellSweet(saver);
        } else if (regex_match(input_command, print_transactions) && confectionary_is_made)
        {
            printTransActions();
        } else if (regex_match(input_command, print_income) && confectionary_is_made)
        {
            printIncome();
        } else if (input_command != "end")
        {
            cout << "invalid command" << endl;
        }
        if (input_command == "end")
        {
            break;
        }
    }

}

int main()
{
    run();
    return 0;
}
