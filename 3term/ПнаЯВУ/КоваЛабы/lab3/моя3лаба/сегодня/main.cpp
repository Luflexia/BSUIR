//main.cpp
#include "Watch.h"
#include "MechanicalWatch.h"
#include "ElectronicWatch.h"
#include "SmartWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "WristWatch.h"
#include "Exceptions.h"
#include "DataManager.h"
#include "WatchMM.h"
#include <map>

MyList<Watch*> watches;
std::multimap<std::string, std::string> watchesMM;
WatchMultiMap watchMM;
void displayHeader() {
    cout << left << setw(S) << "ID";
    cout << setw(S) << "Brand";
    cout << setw(S) << "Price";
    cout << setw(S) << "Additional Info";
    cout << endl;
}

void displayWatches() {
    displayHeader();
    for (auto it = watches.begin(); it != watches.end(); ++it) {
        (*it)->display();
        cout << "------------------------------------------------------------------------------------------" << endl;
    }
}

void modifyWatchById(int id) {
    auto it = find_if(watches.begin(), watches.end(), [id](const Watch* watch) {
        return watch->getId() == id;
        });

    if (it != watches.end()) {
        (*it)->update();
        cout << "Watch with ID " << id << " modified successfully." << endl;
    }
    else {
        cout << "Watch with ID " << id << " not found." << endl;
    }
}

void deleteWatchById(int id) {
    auto it = find_if(watches.begin(), watches.end(), [id](const Watch* watch) {
        return watch->getId() == id;
        });

    if (it != watches.end()) {
        delete it.getPtr(); // Используем новый метод для доступа к ptr
        watches.erase(it);
        cout << "Watch with ID " << id << " deleted successfully." << endl;
    }
    else {
        cout << "Watch with ID " << id << " not found." << endl;
    }
}
void addWatch() {
    int typeChoice;
    cout << "Select the type of watch to add:" << endl;
    cout << "1. Mechanical Watch" << endl;
    cout << "2. Electronic Watch" << endl;
    cout << "3. Smart Watch" << endl;
    cout << "4. Wrist Smart Watch" << endl;
    cout << "5. Table Clock" << endl;
    cout << "6. Wrist Watch" << endl;

    cout << "Enter your choice: " << endl;;
    cin >> typeChoice;

    Watch* newWatch = nullptr;

    switch (typeChoice) {
    case 1:
        newWatch = new MechanicalWatch("", 0, "", 0.0, ""); 
        break;
    case 2:
        newWatch = new ElectronicWatch("", 0, "", 0.0, "");   
        break;
    case 3:
        newWatch = new SmartWatch("", 0, "", 0.0, "", "");    
        break;
    case 4:
        newWatch = new WristSmartWatch("", 0, "", 0.0, "", "", ""); 
        break;
    case 5:
        newWatch = new TableClock("", 0, "", 0.0, "", false);  
        break;
    case 6:
        newWatch = new WristWatch("", 0, "", 0.0, "", "");    
        break;
    default:
        cout << "Invalid choice. Watch not added." << endl;
        return;
    }

    newWatch->update();
    watches.push_back(newWatch);
    DataManager::saveWatches(watches, "watches.txt");
    cout << "Watch added successfully." << endl;
}


int main() {
    int choice;
    watches = DataManager::loadWatches("watches.txt");
    do {
        cout << "\nMenu:" << endl;
        cout << "1. Add Watch" << endl;
        cout << "2. Display Watches" << endl;
        cout << "3. Modify Watch" << endl;
        cout << "4. Delete Watch" << endl;
        cout << "5. Add to MultiMap" << endl;
        cout << "6. View in MultiMap" << endl;
        cout << "7. Delete from MultiMap by name" << endl;
        cout << "8. Modify in MultiMap by name" << endl;
        cout << "9. Sort MultiMap by cost" << endl;
        cout << "0. Exit" << endl;

        cout << "Enter your choice: ";
        while (!(std::cin >> choice)) {
            std::cin.clear();  // Очистка флагов ошибок
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Очистка буфера ввода
            std::cout << "\nInvalid input. Please enter a number: ";
        }


        switch (choice) {//MYLIST
        case 1:
            addWatch();
            DataManager::saveWatches(watches, "watches.txt");
            break;
        case 2:
            // Вывод созданных объектов
            cout << "Watches:" << endl;
            displayWatches();
            break;
        case 3: {
            // Изменения всех полей объекта по id
            int idToModify;
            cout << "Enter the ID of the watch to modify: ";
            cin >> idToModify;
            modifyWatchById(idToModify);
            DataManager::saveWatches(watches, "watches.txt");
            break;
        }
        case 4: {
            // Удаление объекта по id
            int idToDelete;
            cout << "Enter the ID of the watch to delete: ";
            cin >> idToDelete;
            deleteWatchById(idToDelete);
            DataManager::saveWatches(watches, "watches.txt");
            break;
        }//MULTIMAP
        case 5: {
            // Добавление часов в мультимапу
            watchMM.add();
            cout << "Watch added to MultiMap successfully." << endl;
            break;
        }
        case 6:
            // Проверка наличия элементов в мультимапе перед выводом
            if (!watchMM.isEmpty()) {
                cout << "Watches in MultiMap:" << endl;
                watchMM.viewMultiMap();
            }
            else {
                cout << "MultiMap is empty." << endl;
            }
            break;
        case 7: {
            // Удаление по имени, запрашиваем имя у пользователя
            std::string nameToDelete;
            std::cout << "Enter the name to delete from MultiMap: ";
            std::cin >> nameToDelete;
            watchMM.deleteByName(nameToDelete);
            break;
        }
            case 8: {
            std::string nameToModify;
            std::cout << "Enter the name to delete from MultiMap: ";
            std::cin >> nameToModify;
            watchMM.modifyByName(nameToModify);
            break;
            }
            case 9:
                //watchMM.sortByPrice();
                //cout << "MultiMap sorted by price." << endl;
                //break;
        case 0: {
            DataManager::saveWatches(watches, "watches.txt");
            // Освобождение памяти перед выходом
            for (const auto& watch : watches) {
                delete watch;
            }
            break;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 0);

    return 0;
}