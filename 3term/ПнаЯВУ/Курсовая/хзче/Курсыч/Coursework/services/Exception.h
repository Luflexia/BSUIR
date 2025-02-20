//
// Created by Egor on 12/4/2022.
//

#ifndef COURSEWORK_EXCEPTION_H
#define COURSEWORK_EXCEPTION_H

#include <string>
#include <iostream>

using namespace std;

class Exception {
    string msg;
    int exceptionTypeCode;  //  -1 - Silent exception | 0 - Error | 1 - Fatal error | 2 - Warning
public:
    Exception(const string &msg, int type = 0) {
        exceptionTypeCode = type;
        this->msg = msg;
    }

    ~Exception() = default;

    void what() {
        switch (exceptionTypeCode) {
            case -1:
                cout << msg << endl;
                break;
            case 0:
                cout << "[Error] " << msg << endl;
                break;
            case 1:
                cout << "[ERROR] " << msg << endl;
                exit(EXIT_FAILURE);
            case 2:
                cout << "[Warning] " << msg << endl;
                break;
            default:
                cout << "[System] Unknown error code. \nError: " << msg << endl;
        }
    }
};

#endif //COURSEWORK_EXCEPTION_H