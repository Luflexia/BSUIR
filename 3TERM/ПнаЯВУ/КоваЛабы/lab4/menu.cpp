// menu.cpp
#pragma once
#include "list.h"

template <typename T>
void displayMenu(List<T>* list) {
    int choice;
    T data;
    int position;

    do {
        std::string typeName = typeid(T).name();
        std::cout << "List " << typeName << "\n";
        std::cout << "1. Push to list\n";
        std::cout << "2. Push to head\n";
        std::cout << "3. Remove at position\n";
        std::cout << "4. Clear list\n";
        std::cout << "5. Get data at position\n";
        std::cout << "6. Print list\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;


        system("cls");
        switch (choice) {
        case 1:
            std::cout << "Enter data to push: ";
            std::cin >> data;
            std::cout << "Enter position (default is 0): ";
            std::cin >> position;
            list->push(data, position);
            break;

        case 2:
            std::cout << "Enter data to push to head: ";
            std::cin >> data;
            list->pushHead(data);
            break;

        case 3:
            std::cout << "Enter position to remove (default is 0): ";
            std::cin >> position;
            list->removeAt(position);
            break;

        case 4:
            list->clear();
            std::cout << "List cleared.\n";
            break;

        case 5:
            std::cout << "Enter position to get data (default is 0): ";
            std::cin >> position;
            T retrievedData = list->getData(position);
            std::cout << "Data at position " << position << ": " << retrievedData << "\n";
            break;

        case 6:
            list->print();
            break;

        case 0:
            std::cout << "Exiting.\n";
            break;

        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }

    } while (choice != 0);
}
