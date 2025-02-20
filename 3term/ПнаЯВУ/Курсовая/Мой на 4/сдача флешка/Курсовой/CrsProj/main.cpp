#include <windows.h>
#include <locale>
#include <iostream>
#include "Interfaces.h"


using namespace std;

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    //setlocale(LC_ALL, "rus");

    Interfaces myInterface;
    try {
        myInterface.run();
    }
    catch (Exception exception) {
        exception.what();
    }
    catch (...) {
        unexpected();
    }
    return 0;
}
