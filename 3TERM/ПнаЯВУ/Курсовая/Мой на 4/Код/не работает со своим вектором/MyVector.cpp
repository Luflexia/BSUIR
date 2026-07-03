#include "MyVector.h"
template <typename T>
MyVector<T>::MyVector() : elements(nullptr), size(0), capacity(0) {}

template <typename T>
MyVector<T>::~MyVector() {
    clear();
    delete[] elements;
}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) : size(other.size), capacity(other.size * 2) {
    elements = new T[capacity];
    for (size_t i = 0; i < size; ++i) {
        elements[i] = other.elements[i];
    }
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this != &other) {
        clear();
        delete[] elements;

        size = other.size;
        capacity = other.size * 2;

        elements = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            elements[i] = other.elements[i];
        }
    }
    return *this;
}

template <typename T>
T& MyVector<T>::operator[](size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return elements[index];
}

template <typename T>
const T& MyVector<T>::operator[](size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return elements[index];
}

template <typename T>
size_t MyVector<T>::getSize() const {
    return size;
}

template <typename T>
void MyVector<T>::pushBack(const T& value) {
    if (size == capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    elements[size++] = value;
}

template <typename T>
void MyVector<T>::reserve(size_t newCapacity) {
    if (newCapacity > capacity) {
        T* newElements = new T[newCapacity];
        for (size_t i = 0; i < size; ++i) {
            newElements[i] = elements[i];
        }
        delete[] elements;
        elements = newElements;
        capacity = newCapacity;
    }
}

template <typename T>
void MyVector<T>::clear() {
    for (size_t i = 0; i < size; ++i) {
        elements[i].~T();
    }
    size = 0;
}

template <typename T>
T* MyVector<T>::begin() {
    return elements;
}

template <typename T>
T* MyVector<T>::end() {
    return elements + size;
}

template <typename T>
const T* MyVector<T>::begin() const {
    return elements;
}

template <typename T>
const T* MyVector<T>::end() const {
    return elements + size;
}
