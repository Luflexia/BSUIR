#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <windows.h>
#include "myvector.h"

using namespace std;

// Функция для проверки ввода
template<typename T>
T getInput(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(100, '\n');  // Игнорируем до 100 символов или до конца строки
            return value;
        }
        cin.clear();
        cin.ignore(100, '\n');  // Очищаем буфер ввода
        cout << "Invalid input. Please try again.\n";
    }
}

// Базовый класс
class ConfectioneryItem {
protected:
    string name;
    double price;
    string ingredients;
    int calories;

public:
    ConfectioneryItem(const string& name, double price, int calories)
            : name(name), price(price), calories(calories) {}

    virtual ~ConfectioneryItem() = default;

    virtual void displayInfo() const = 0;

    string getName() const { return name; }
    double getPrice() const { return price; }
    int getCalories() const { return calories; }

    void setIngredients(const string& newIngredients) {
        ingredients = newIngredients;
    }

    const string& getIngredients() const {
        return ingredients;
    }

    virtual void setName(const string& newName) { name = newName; }
    virtual void setPrice(double newPrice) { price = newPrice; }
    virtual void setCalories(int newCalories) { calories = newCalories; }
};

// Производный класс первого уровня - Выпечка
class BakedGood : public ConfectioneryItem {
protected:
    double weight;

public:
    BakedGood(const string& name, double price, double weight, int calories)
            : ConfectioneryItem(name, price, calories), weight(weight) {}

    double getWeight() const { return weight; }
    void setWeight(double newWeight) { weight = newWeight; }
};

// Производный класс второго уровня - Торт
class Cake : public BakedGood {
public:
    Cake(const string& name, double price, double weight, int calories)
            : BakedGood(name, price, weight, calories) {}

    void displayInfo() const override {
        cout << "\n" << string(50, '=') << "\n";
        cout << "Type: Cake\n"
             << "Name: " << name << "\n"
             << "Price: " << fixed << setprecision(2) << price << " USD\n"
             << "Weight: " << weight << " kg\n"
             << "Calories: " << calories << "\n"
             << "Ingredients: " << ingredients << "\n";
    }

};

// Производный класс третьего уровня - Свадебный торт
class WeddingCake : public Cake {
private:
    int tiers;

public:
    WeddingCake(const string& name, double price, double weight, int tiers, int calories)
            : Cake(name, price, weight, calories), tiers(tiers) {}

    int getTiers() const { return tiers; }
    void setTiers(int newTiers) { tiers = newTiers; }

    void displayInfo() const override {
        cout << "\n" << string(50, '=') << "\n";
        cout << "Type: Wedding Cake\n"
             << "Name: " << name << "\n"
             << "Price: " << fixed << setprecision(2) << price << " USD\n"
             << "Weight: " << weight << " kg\n"
             << "Tiers: " << tiers << "\n"
             << "Calories: " << calories << "\n"
             << "Ingredients: " << ingredients << "\n";
    }
};

// Производный класс - Печенье
class Cookie : public ConfectioneryItem {
private:
    string shape;

public:
    Cookie(const string& name, double price, const string& shape, int calories)
            : ConfectioneryItem(name, price, calories), shape(shape) {}

    string getShape() const { return shape; }
    void setShape(const string& newShape) { shape = newShape; }

    void displayInfo() const override {
        cout << "\n" << string(50, '=') << "\n";
        cout << "Type: Cookie\n"
             << "Name: " << name << "\n"
             << "Price: " << fixed << setprecision(2) << price << " USD\n"
             << "Shape: " << shape << "\n"
             << "Calories: " << calories << "\n"
             << "Ingredients: " << ingredients << "\n";
    }
};

// Класс для управления ассортиментом
class ConfectioneryManager {
private:
    vector<ConfectioneryItem*> items;

public:
    ~ConfectioneryManager() {
        for (auto item : items) {
            delete item;
        }
    }

    void addItem(ConfectioneryItem* item) {
        while (true) {
            // Проверяем, существует ли уже элемент с таким именем
            auto it = find_if(items.begin(), items.end(),
                              [&item](const ConfectioneryItem* existingItem) {
                                  return existingItem->getName() == item->getName();
                              });

            if (it != items.end()) {
                // Если элемент с таким именем уже существует, просим ввести новое имя
                cout << "Error: An item with the name '" << item->getName() << "' already exists.\n";
                string newName = getInput<string>("Please enter a different name: ");
                item->setName(newName);
            } else {
                // Если элемент с таким именем не найден, добавляем новый элемент
                items.push_back(item);
                cout << "Item '" << item->getName() << "' has been added successfully.\n";
                break;
            }
        }
    }

    void removeItem(const string& name) {
        auto it = find_if(items.begin(), items.end(),
                               [&name](const ConfectioneryItem* item) { return item->getName() == name; });
        if (it != items.end()) {
            delete *it;
            items.erase(it);
        }
    }

    void displayAllItems() const {
        cout << "\n" << string(120, '=') << "\n";
        cout << left
             << setw(30) << "Name"
             << setw(15) << "Price (USD)"
             << setw(15) << "Weight (kg)"
             << setw(15) << "Type"
             << setw(15) << "Calories"
             << setw(15) << "Additional"
             << "Ingredients\n";
        cout << string(120, '-') << "\n";
        for (const auto item : items) {
            cout << left
                 << setw(30) << item->getName()
                 << setw(15) << fixed << setprecision(2) << item->getPrice()
                 << setw(15);

            if (auto cake = dynamic_cast<Cake*>(item)) {
                cout << fixed << setprecision(3) << cake->getWeight();
            } else {
                cout << "-";
            }

            string itemType;
            string additional;

            if (auto weddingCake = dynamic_cast<WeddingCake*>(item)) {
                itemType = "Wedding Cake";
                additional = to_string(weddingCake->getTiers()) + " tiers";
            } else if (dynamic_cast<Cake*>(item)) {
                itemType = "Cake";
            } else if (auto cookie = dynamic_cast<Cookie*>(item)) {
                itemType = "Cookie";
                additional = cookie->getShape();
            }

            cout << left << setw(15) << itemType
                 << setw(15) << item->getCalories()
                 << setw(15) << additional;

            // Выводим ингредиенты
            cout << item->getIngredients() << "\n";
        }
        cout << string(120, '=') << "\n";
    }

    ConfectioneryItem* findItem(const string& name) {
        auto it = find_if(items.begin(), items.end(),
                               [&name](const ConfectioneryItem* item) { return item->getName() == name; });
        return (it != items.end()) ? *it : nullptr;
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            throw runtime_error("Failed to open file for writing");
        }

        for (const auto item : items) {
            file << item->getName() << "|" << item->getPrice() << "|" << item->getCalories();

            if (auto cake = dynamic_cast<Cake*>(item)) {
                file << "|Cake|" << cake->getWeight();
                if (auto weddingCake = dynamic_cast<WeddingCake*>(cake)) {
                    file << "|" << weddingCake->getTiers();
                } else {
                    file << "|0";
                }
            } else if (auto cookie = dynamic_cast<Cookie*>(item)) {
                file << "|Cookie|" << cookie->getShape();
            }

            // Сохраняем ингредиенты
            file << "|" << item->getIngredients() << "\n";
        }

        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "File " << filename << " not found. Creating a new file.\n";
            return;
        }

        // Очищаем текущий список изделий
        for (auto item : items) {
            delete item;
        }
        items.clear();

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string name, type, shape, ingredientsStr;
            double price, weight;
            int calories, tiers;

            getline(iss, name, '|');
            iss >> price;
            iss.ignore();
            iss >> calories;
            iss.ignore();
            getline(iss, type, '|');

            ConfectioneryItem* item = nullptr;

            if (type == "Cake") {
                iss >> weight;
                iss.ignore();
                iss >> tiers;
                if (tiers > 0) {
                    item = new WeddingCake(name, price, weight, tiers, calories);
                } else {
                    item = new Cake(name, price, weight, calories);
                }
            } else if (type == "Cookie") {
                getline(iss, shape, '|');
                item = new Cookie(name, price, shape, calories);
            }

            if (item) {
                // Читаем ингредиенты как одну строку
                getline(iss, ingredientsStr);
                // Удаляем начальный разделитель, если он есть
                if (!ingredientsStr.empty() && ingredientsStr[0] == '|') {
                    ingredientsStr.erase(0, 1);
                }
                item->setIngredients(ingredientsStr);
                addItem(item);
            }
        }

        file.close();
        cout << "Data successfully loaded from file " << filename << "\n";
    }
};

// Класс для управления заказами
class Order {
private:
    vector<ConfectioneryItem*> items;
    MyVector<int> quantities;

public:
    void addItem(ConfectioneryItem* item, int quantity) {
        items.push_back(item);
        quantities.push_back(quantity);
    }

    double calculateTotal() const {
        double total = 0;
        for (size_t i = 0; i < items.size(); ++i) {
            total += items[i]->getPrice() * quantities[i];
        }
        return total;
    }

    void displayOrder() const {
        cout << "\n" << string(100, '=') << "\n";
        cout << left
             << setw(20) << "Item Name"
             << setw(10) << "Quantity"
             << setw(10) << "Price"
             << setw(10) << "Total"
             << setw(10) << "Calories"
             << "Ingredients\n";
        cout << string(100, '-') << "\n";

        for (size_t i = 0; i < items.size(); ++i) {
            cout << left
                 << setw(20) << items[i]->getName()
                 << setw(10) << quantities[i]
                 << setw(10) << fixed << setprecision(2) << items[i]->getPrice()
                 << setw(10) << fixed << setprecision(2) << items[i]->getPrice() * quantities[i]
                 << setw(10) << items[i]->getCalories()
                 << items[i]->getIngredients() << "\n";
        }

        cout << string(100, '-') << "\n";
        cout << left << setw(50) << "Total:"
             << fixed << setprecision(2) << calculateTotal() << " USD\n";
        cout << string(100, '=') << "\n";
    }


    void saveToFile(const string& filename) const {
        string fullFilename = filename + ".txt";
        ofstream file(fullFilename);
        if (!file) {
            throw runtime_error("Failed to open file for writing: " + fullFilename);
        }

        file << "Order:\n";
        file << string(120, '-') << "\n";
        file << left << setw(30) << "Item Name"
             << right << setw(10) << "Quantity"
             << setw(12) << "Price"
             << setw(12) << "Total"
             << setw(12) << "Calories"
             << "  Ingredients\n";
        file << string(120, '-') << "\n";

        int totalCalories = 0;
        for (size_t i = 0; i < items.size(); ++i) {
            ConfectioneryItem* item = items[i];
            int quantity = quantities[i];
            double itemPrice = item->getPrice();
            double itemTotal = itemPrice * quantity;
            int itemCalories = item->getCalories() * quantity;
            totalCalories += itemCalories;

            file << left << setw(30) << item->getName()
                 << right << setw(10) << quantity
                 << setw(12) << fixed << setprecision(2) << itemPrice
                 << setw(12) << fixed << setprecision(2) << itemTotal
                 << setw(12) << itemCalories
                 << "  " << left << item->getIngredients() << "\n";
        }

        file << string(120, '-') << "\n";
        file << left << setw(30) << "Total:"
             << right << setw(34) << fixed << setprecision(2) << calculateTotal() << " USD\n";
        file << left << setw(30) << "Total Calories:"
             << right << setw(34) << totalCalories << "\n";

        file.close();
        cout << "Order successfully saved to file: " << fullFilename << "\n";
    }
};

int main() {
    // Устанавливаем кодировку консоли в UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    char buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);
    cout << "Current working directory: " << buffer << endl;

    ConfectioneryManager manager;
    Order currentOrder;

    try {
        manager.loadFromFile("confectionery.txt");
    } catch (const exception& e) {
        cout << "Error loading data: " << e.what() << "\n";
    }

    cout << "Current list of items:\n";
    manager.displayAllItems();

    while (true) {
        //cout << "\n" << string(50, '=') << "\n";
        cout << "Menu:\n" << string(50, '-') << "\n"
                  << "1. Show all items\n"
                  << "2. Add item\n"
                  << "3. Remove item\n"
                  << "4. Show item information\n"
                  << "5. Edit item\n"  // New menu item
                  << "6. Add to order\n"
                  << "7. Show current order\n"
                  << "8. Save order\n"
                  << "9. Exit\n"
                  << string(50, '-') << "\n"
                  << "Choose an action: ";

        int choice = getInput<int>("");

        //cout << string(50, '=') << "\n";

        switch (choice) {
            case 1:
                manager.displayAllItems();
                break;
            case 2: {
                string name = getInput<string>("Enter name: ");
                double price = getInput<double>("Enter price: ");
                int calories = getInput<int>("Enter calories per 100g: ");

                cout << "Enter ingredients: ";
                string ingredients;
                getline(cin, ingredients);

                cout << "Choose item type:\n1. Cake\n2. Wedding Cake\n3. Cookie\nEnter choice: ";
                int typeChoice = getInput<int>("");

                ConfectioneryItem* newItem = nullptr;

                switch (typeChoice) {
                    case 1: {
                        double weight = getInput<double>("Enter weight (kg): ");
                        newItem = new Cake(name, price, weight, calories);
                        break;
                    }
                    case 2: {
                        double weight = getInput<double>("Enter weight (kg): ");
                        int tiers = getInput<int>("Enter number of tiers: ");
                        newItem = new WeddingCake(name, price, weight, tiers, calories);
                        break;
                    }
                    case 3: {
                        string shape = getInput<string>("Enter shape: ");
                        newItem = new Cookie(name, price, shape, calories);
                        break;
                    }
                    default:
                        cout << "Invalid item type choice.\n";
                        break;
                }
                if (newItem) {
                    newItem->setIngredients(ingredients);
                    manager.addItem(newItem);
                    std::cout << "Item successfully added.\n";
                    manager.saveToFile("confectionery.txt");
                    std::cout << "Current list of items:\n";
                    manager.displayAllItems();
                }
                break;
            }
            case 3: {
                string name = getInput<string>("Enter the name of the item to remove: ");
                manager.removeItem(name);
                break;
            }
            case 4: {
                string name = getInput<string>("Enter the name of the item to display: ");
                ConfectioneryItem *item = manager.findItem(name);
                if (item) {
                    item->displayInfo();
                } else {
                    cout << "Item not found\n";
                }
                break;
            }
            case 5: {
                string name = getInput<string>("Enter the name of the item to edit: ");
                ConfectioneryItem *item = manager.findItem(name);
                if (item) {
                    cout << "Current item details:\n";
                    item->displayInfo();

                    string newName = getInput<string>("Enter new name (or 0 to keep current): ");
                    if (newName != "0") item->setName(newName);

                    double newPrice = getInput<double>("Enter new price (or 0 to keep current): ");
                    if (newPrice != 0) item->setPrice(newPrice);

                    int newCalories = getInput<int>("Enter new calories per 100g (or 0 to keep current): ");
                    if (newCalories != 0) item->setCalories(newCalories);

                    std::cout << "Current ingredients: " << item->getIngredients() << "\n";
                    std::cout << "Enter new ingredients, or '0' to keep current): ";
                    std::string newIngredientsInput;
                    std::getline(std::cin, newIngredientsInput);
                    if (newIngredientsInput != "0") {
                        item->setIngredients(newIngredientsInput);
                    }

                    if (auto cake = dynamic_cast<Cake *>(item)) {
                        double newWeight = getInput<double>("Enter new weight (or 0 to keep current): ");
                        if (newWeight != 0) cake->setWeight(newWeight);

                        if (auto weddingCake = dynamic_cast<WeddingCake *>(cake)) {
                            int newTiers = getInput<int>("Enter new number of tiers (or 0 to keep current): ");
                            if (newTiers != 0) weddingCake->setTiers(newTiers);
                        }
                    } else if (auto cookie = dynamic_cast<Cookie *>(item)) {
                        string newShape = getInput<string>("Enter new shape (or '0' to keep current): ");
                        if (newShape != "0") cookie->setShape(newShape);
                    }

                    cout << "Item successfully edited.\n";
                    manager.saveToFile("confectionery.txt");
                } else {
                    cout << "Item not found\n";
                }
                break;
            }
            case 6: {
                string name = getInput<string>("Enter the name of the item to add to the order: ");
                ConfectioneryItem *item = manager.findItem(name);
                if (item) {
                    int quantity = getInput<int>("Enter quantity: ");
                    currentOrder.addItem(item, quantity);
                } else {
                    cout << "Item not found\n";
                }
                break;
            }
            case 7:
                currentOrder.displayOrder();
                break;
            case 8: {
                string filename = getInput<string>("Enter the filename to save the order: ");
                try {
                    currentOrder.saveToFile(filename);
                    cout << "Order saved to file " << filename << "\n";
                } catch (const exception &e) {
                    cout << "Error saving order: " << e.what() << "\n";
                }
                break;
            }
            case 9:
                try {
                    manager.saveToFile("confectionery.txt");
                    cout << "Data saved.\n";
                } catch (const exception &e) {
                    cout << "Error saving data: " << e.what() << "\n";
                }
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
    return 0;
}