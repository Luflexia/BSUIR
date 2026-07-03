#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <stdexcept>

template<typename T>
class MyVector {
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    MyVector() : data(nullptr), size(0), capacity(0) {}

    ~MyVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (size == capacity) {
            size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < size; ++i) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
        data[size++] = value;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t get_size() const {
        return size;
    }

    bool empty() const {
        return size == 0;
    }
};

#endif // MYVECTOR_H