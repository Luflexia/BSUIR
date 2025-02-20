#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class MyVector {
private:
    T* elements;
    size_t size;
    size_t capacity;

public:
    MyVector();
    ~MyVector();
    MyVector(const MyVector& other);
    MyVector& operator=(const MyVector& other);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    size_t getSize() const;
    void pushBack(const T& value);
    void reserve(size_t newCapacity);
    void clear();
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;
};
