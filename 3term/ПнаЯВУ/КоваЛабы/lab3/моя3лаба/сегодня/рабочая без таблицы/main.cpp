#include "Watch.h"
#include "MechanicalWatch.h"
#include "ElectronicWatch.h"
#include "SmartWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "WristWatch.h"

vector<Watch*> watches;

void displayHeader() {
    cout << left << setw(S) << "ID";
    cout << setw(S) << "Brand";
    cout << setw(S) << "Price";
    cout << setw(S) << "Additional Info";
    cout << endl;
}

void displayWatches() {
    displayHeader();
    for (const auto& watch : watches) {
        watch->display();
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
        delete* it;
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

    cout << "Enter your choice: ";
    cin >> typeChoice;

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
        cout << "Invalid choice. Watch not added." << endl;
        return;
    }

    newWatch->update();
    watches.push_back(newWatch);
    cout << "Watch added successfully." << endl;
}

int main() {
    int choice;

    do {
        cout << "Menu:" << endl;
        cout << "1. Add Watch" << endl;
        cout << "2. Display Watches" << endl;
        cout << "3. Modify Watch" << endl;
        cout << "4. Delete Watch" << endl;
        cout << "0. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addWatch();
            break;
        case 2:
            // Вывод созданных объектов
            cout << "Watches:" << endl;
            displayWatches();
            break;
        case 3: {
            // Пример изменения всех полей объекта по id
            int idToModify;
            cout << "Enter the ID of the watch to modify: ";
            cin >> idToModify;
            modifyWatchById(idToModify);
            break;
        }
        case 4: {
            // Пример удаления объекта по id
            int idToDelete;
            cout << "Enter the ID of the watch to delete: ";
            cin >> idToDelete;
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
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 0);

    return 0;
}
