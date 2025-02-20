#include <windows.h>
#include <locale>
#include <iostream>
#include "Interfaces.h"


using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");

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
