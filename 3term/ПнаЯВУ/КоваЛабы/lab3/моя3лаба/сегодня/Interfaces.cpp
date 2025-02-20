#include "Interfaces.h"
#include "MyList.h"
#include "Watch.h"
#include "MechanicalWatch.h"
#include "ElectronicWatch.h"
#include "SmartWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "WristWatch.h"

MyList<Watch*> watches;

void Interfaces::displayHeader() {
    cout << left << setw(S) << "ID";
    cout << setw(S) << "Brand";
    cout << setw(S) << "Price";
    cout << setw(S) << "Additional Info";
    cout << endl;
}

void Interfaces::displayWatches() {
    displayHeader();
    for (auto it = watches.begin(); it != watches.end(); ++it) {
        (*it)->display();
        cout << "------------------------------------------------------------------------------------------" << endl;
    }
}

void Interfaces::modifyWatchById(int id) {
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

void Interfaces::deleteWatchById(int id) {
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

void Interfaces::addWatch() {
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