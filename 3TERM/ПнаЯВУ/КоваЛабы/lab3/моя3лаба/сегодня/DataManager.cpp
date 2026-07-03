#include "DataManager.h"
#include <fstream>

void DataManager::saveWatches(const MyList<Watch*>& watches, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (auto it = watches.begin(); it != watches.end(); ++it) {
            // Получаем тип часов и вызываем соответствующий метод для сохранения данных
            string watchType = getWatchType(*it);
            file << (*it)->getWatchType() << (*it)->getId() << " " << (*it)->getBrand() << " " << (*it)->getPrice() << " " << watchType << " ";
            (*it)->save(file);
            file << "\n";
        }
        file.close();
    }
}

MyList<Watch*> DataManager::loadWatches(const std::string& filename) {
    MyList<Watch*> watches;
    std::ifstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < 10; ++i) {
            string type;
            int id;
            std::string brand;
            double price;
            std::string watchType;

            file >> id >> brand >> price >> watchType;

            Watch* newWatch = nullptr;

            // Определение типа часов и создание соответствующего объекта
            if (watchType == "ElectronicWatch") {
                newWatch = new ElectronicWatch(type, id, brand, price, "");
                newWatch->load(file); 
            }
            else if (watchType == "MechanicalWatch") {
                newWatch = new MechanicalWatch(type, id, brand, price, "");
                newWatch->load(file); 
            }
            else if (watchType == "SmartWatch") {
                newWatch = new SmartWatch(type, id, brand, price, "", "");
                newWatch->load(file); 
            }
            else if (watchType == "TableClock") {
                newWatch = new TableClock(type, id, brand, price, "", "");
                newWatch->load(file);
            }
            else if (watchType == "WristSmartWatch") {
                newWatch = new WristSmartWatch(type, id, brand, price, "", "", "");
                newWatch->load(file);
            }
            else if (watchType == "WristWatch") {
                newWatch = new WristWatch(type, id, brand, price, "", "");
                newWatch->load(file);
            }

            if (newWatch != nullptr) {
                watches.push_back(newWatch);
            }
        }
        file.close();
    }
    return watches;
}

std::string DataManager::getWatchType(const Watch* watch) {
    if (dynamic_cast<ElectronicWatch*>(const_cast<Watch*>(watch)) != nullptr) {
        return "ElectronicWatch";
    }
    else if (dynamic_cast<MechanicalWatch*>(const_cast<Watch*>(watch)) != nullptr) {
        return "MechanicalWatch";
    }
    else if (dynamic_cast<SmartWatch*>(const_cast<Watch*>(watch)) != nullptr) {
        return "SmartWatch";
    }
    else if (dynamic_cast<TableClock*>(const_cast<Watch*>(watch)) != nullptr) {
        return "TableClock";
    }
    else if (dynamic_cast<WristSmartWatch*>(const_cast<Watch*>(watch)) != nullptr) {
        return "WristSmartWatch";
    }
    else if (dynamic_cast<WristWatch*>(const_cast<Watch*>(watch)) != nullptr) {
        return "WristWatch";
    }
    return "Unknown";
}


//MyList<Watch*> DataManager::loadWatches(const std::string& filename) {
//    MyList<Watch*> watches;
//    std::ifstream file(filename);
//    if (file.is_open()) {
//        while (!file.eof()) {
//            string type;
//            int id;
//            std::string brand;
//            double price;
//            std::string watchType;
//
//            file >> id >> brand >> price >> watchType;
//
//            Watch* newWatch = nullptr;
//
//            // Определение типа часов и создание соответствующего объекта
//            if (watchType == "ElectronicWatch") {
//                newWatch = new ElectronicWatch(type, id, brand, price, "");
//                newWatch->load(file);
//            }
//            else if (watchType == "MechanicalWatch") {
//                newWatch = new MechanicalWatch(type, id, brand, price, "");
//                newWatch->load(file);
//            }
//            else if (watchType == "SmartWatch") {
//                newWatch = new SmartWatch(type, id, brand, price, "", "");
//                newWatch->load(file);
//            }
//            else if (watchType == "TableClock") {
//                newWatch = new TableClock(type, id, brand, price, "", "");
//                newWatch->load(file);
//            }
//            else if (watchType == "WristSmartWatch") {
//                newWatch = new WristSmartWatch(type, id, brand, price, "", "", "");
//                newWatch->load(file);
//            }
//            else if (watchType == "WristWatch") {
//                newWatch = new WristWatch(type, id, brand, price, "", "");
//                newWatch->load(file);
//            }
//
//            if (newWatch != nullptr) {
//                watches.push_back(newWatch);
//            }
//        }
//        file.close();
//    }
//    return watches;
//}
