// MyList.h
#pragma once
#include <iostream>
#include <algorithm>

template <class T>
class MyList {
public:
    class Iterator {
    public:
        Iterator(T* ptr) : ptr(ptr) {}
        T& operator*() { return *ptr; }
        Iterator& operator++() { ++ptr; return *this; }
        Iterator& operator--() { --ptr; return *this; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        T* getPtr() const { return ptr; }

    private:
        T* ptr;
    };

    MyList();
    ~MyList();

    void clear() {
        delete[] array;
        array = nullptr;
        capacity = 0;
        currentSize = 0;
    }

    void push_back(const T& value);
    void erase(Iterator it);
    void display() const;
    int size() const;

    Iterator begin() const {
        return Iterator(array);
    }

    Iterator end() const {
        return Iterator(array + currentSize);
    }

    T& operator[](int index);

private:
    T* array;
    int capacity;
    int currentSize;
};

template <class T>
MyList<T>::MyList() : array(nullptr), capacity(0), currentSize(0) {}

template <class T>
MyList<T>::~MyList() {
    delete[] array;
}

template <class T>
void MyList<T>::push_back(const T& value) {
    if (currentSize == capacity) {
        int newCapacity = (capacity == 0) ? 1 : capacity * 2;
        T* newArray = new T[newCapacity];
        std::copy(array, array + currentSize, newArray);
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    array[currentSize++] = value;
}

template <class T>
void MyList<T>::erase(Iterator it) {
    if (it.getPtr() < array || it.getPtr() >= array + currentSize) {
        std::cerr << "Error: Iterator out of bounds" << std::endl;
        return;
    }

    std::copy(it.getPtr() + 1, array + currentSize, it.getPtr());
    --currentSize;
}

template <class T>
void MyList<T>::display() const {
    for (int i = 0; i < currentSize; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

template <class T>
int MyList<T>::size() const {
    return currentSize;
}

template <class T>
T& MyList<T>::operator[](int index) {
    return array[index];
}
