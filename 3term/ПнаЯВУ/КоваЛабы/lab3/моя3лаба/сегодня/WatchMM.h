#include <iostream>
#include <map>

class WatchMultiMap {
public:
    // ������� ��� ���������� �������� ����� � ����������
    void addWatch(const std::string& date, const std::string& brand, const std::string& model, double price) {
        // ��������� ���� � ����������
        watches.insert(std::make_pair(date, Watch{ brand, model, price }));
        std::cout << "Watch added to MultiMap successfully." << std::endl;
    }

    // ������� ��� ��������� ����������� ����������
    void viewMultiMap() const {
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "| Date       | Brand           | Model    | Price   |" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;

        for (const auto& pair : watches) {
            const auto& watch = pair.second;
            std::cout << "| " << std::setw(10) << pair.first << " | " << std::setw(15) << watch.brand
                << " | " << std::setw(8) << watch.model << " | " << std::setw(7) << watch.price << " |" << std::endl;
        }

        std::cout << "------------------------------------------------------" << std::endl;
    }

    void add() {
        addWatch("2024-01-11", "Cartier", "Model1", 5000.0);
        addWatch("2024-01-12", "Chopard", "Model2", 7000.0);
        addWatch("2024-01-13", "Blancpain", "Model3", 10000.0);
        addWatch("2024-01-12", "Baume", "Model4", 3000.0);
        addWatch("2024-01-15", "Breitling", "Model5", 8000.0);
        addWatch("2024-01-18", "Glashutte", "Model6", 6000.0);
        addWatch("2024-01-17", "Rolex", "Model7", 12000.0);
        addWatch("2024-01-18", "Casio", "Model8", 200.0);
    }

    void modifyByName(const std::string& name) {
        std::cout << "Enter new information for the watch with brand '" << name << "':" << std::endl;

        // ���������� ��� �������� ����� ������� ����� � ��������� �������
        bool found = false;

        // ��������� �� ���� ����� � ����������
        auto it = watches.begin();
        while (it != watches.end()) {
            auto& watch = it->second;

            // ���������� ������ ��� ����� ��������
            if (watch.brand == name) {
                found = true;

                std::cout << "Enter new brand: ";
                std::cin >> watch.brand;

                std::cout << "Enter new model: ";
                std::cin >> watch.model;

                std::cout << "Enter new price: ";
                std::cin >> watch.price;

                // ��������� � ���������� �������� ����� ���������
                ++it;
            }
            else {
                // ���� ����� �� ���������, ������ ��������� � ���������� ��������
                ++it;
            }
        }

        // ������� ��������� � ������, ���� �� ������� ����� � ��������� �������
        if (!found) {
            std::cout << "No watches found with brand '" << name << "'." << std::endl;
        }
        else {
            std::cout << "Information modified successfully." << std::endl;
        }
    }


    bool isEmpty() const {
        return watches.empty();
    }

    void deleteByName(const std::string& name) {
        auto it = watches.begin();
        while (it != watches.end()) {
            const auto& watch = it->second;
            if (watch.brand == name) {
                it = watches.erase(it);
            }
            else {
                ++it;
            }
        }
        std::cout << "Watches with brand '" << name << "' deleted from MultiMap." << std::endl;
    }

private:
    // ���������� ����� Watch ��� �������� ���������� � �����
    struct Watch {
        std::string brand;
        std::string model;
        double price;
    };

    // ���������� ��� �������� �������� �����
    std::multimap<std::string, Watch> watches;
};