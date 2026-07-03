//
// Created by Egor on 12/4/2022.
//

#ifndef COURSEWORK_STORAGE_H
#define COURSEWORK_STORAGE_H


#include <string>

class Storage {
    std::string filepath;
protected:
    template<typename T>
    T load(){
        T date;
        //  Get data/class from json
        return date;
    }

    template<typename T>
    void upload(T date){
        //  Set data/class to json
    }
};


#endif //COURSEWORK_STORAGE_H
