#include "Watch.h"
#include "MechanicalWatch.h"
#include "ElectronicWatch.h"
#include "SmartWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "WristWatch.h"

std::vector<Watch*> watches;

void displayHeader() {
    //std::cout <<left<< std::setw(S+8) << "Type";
    std::cout << left << std::setw(S) << "ID";
    std::cout << std::setw(S) << "Brand";
    std::cout << std::setw(S) << "Price";
    std::cout << std::setw(S) << "Additional Info";
    std::cout << std::endl;
}

void displayWatches() {
    displayHeader();
    for (const auto& watch : watches) {
        watch->display();
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
    }
}

void modifyWatchById(int id) {
    auto it = std::find_if(watches.begin(), watches.end(), [id](const Watch* watch) {
        return watch->getId() == id;
        });

    if (it != watches.end()) {
        (*it)->update();
        std::cout << "Watch with ID " << id << " modified successfully." << std::endl;
    }
    else {
        std::cout << "Watch with ID " << id << " not found." << std::endl;
    }
}

void deleteWatchById(int id) {
    auto it = std::find_if(watches.begin(), watches.end(), [id](const Watch* watch) {
        return watch->getId() == id;
        });

    if (it != watches.end()) {
        delete* it;
        watches.erase(it);
        std::cout << "Watch with ID " << id << " deleted successfully." << std::endl;
    }
    else {
        std::cout << "Watch with ID " << id << " not found." << std::endl;
    }
}

void addWatch() {
    int typeChoice;
    std::cout << "Select the type of watch to add:" << std::endl;
    std::cout << "1. Mechanical Watch" << std::endl;
    std::cout << "2. Electronic Watch" << std::endl;
    std::cout << "3. Smart Watch" << std::endl;
    std::cout << "4. Wrist Smart Watch" << std::endl;
    std::cout << "5. Table Clock" << std::endl;
    std::cout << "6. Wrist Watch" << std::endl;

    std::cout << "Enter your choice: ";
    std::cin >> typeChoice;

    Watch* newWatch = nullptr;

    switch (typeChoice) {
    case 1:
        newWatch = new MechanicalWatch(0, "", 0.0, "");  // Default values, will be updated later
        break;
    case 2:
        newWatch = new ElectronicWatch(0, "", 0.0, "");   // Default values, will be updated later
        break;
    case 3:
        newWatch = new SmartWatch(0, "", 0.0, "", "");      // Default values, will be updated later
        break;
    case 4:
        newWatch = new WristSmartWatch(0, "", 0.0, "", "", "");  // Default values, will be updated later
        break;
    case 5:
        newWatch = new TableClock(0, "", 0.0, "", false);   // Default values, will be updated later
        break;
    case 6:
        newWatch = new WristWatch(0, "", 0.0, "", "");      // Default values, will be updated later
        break;
    default:
        std::cout << "Invalid choice. Watch not added." << std::endl;
        return;
    }

    newWatch->update();
    watches.push_back(newWatch);
    std::cout << "Watch added successfully." << std::endl;
}

int main() {
    int choice;

    do {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Add Watch" << std::endl;
        std::cout << "2. Display Watches" << std::endl;
        std::cout << "3. Modify Watch" << std::endl;
        std::cout << "4. Delete Watch" << std::endl;
        std::cout << "0. Exit" << std::endl;

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            addWatch();
            break;
        case 2:
            // Вывод созданных объектов
            std::cout << "Watches:" << std::endl;
            displayWatches();
            break;
        case 3: {
            // Пример изменения всех полей объекта по id
            int idToModify;
            std::cout << "Enter the ID of the watch to modify: ";
            std::cin >> idToModify;
            modifyWatchById(idToModify);
            break;
        }
        case 4: {
            // Пример удаления объекта по id
            int idToDelete;
            std::cout << "Enter the ID of the watch to delete: ";
            std::cin >> idToDelete;
            deleteWatchById(idToDelete);
            break;
        }
        case 0: {
            // Освобождение памяти перед выходом
            for (const auto& watch : watches) {
                delete watch;
            }
            break;
        }
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

    } while (choice != 0);

    return 0;
}
