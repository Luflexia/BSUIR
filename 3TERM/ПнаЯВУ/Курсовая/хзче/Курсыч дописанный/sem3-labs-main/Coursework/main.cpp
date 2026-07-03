#include <iostream>
#include <windows.h>

#include "interfaces/Interfaces.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(1251);

    Interfaces myInterface;
    try {
        myInterface.run();
    } catch (Exception exception) {
        exception.what();
    } catch (...) {
        unexpected();
    }
    return 0;
}