// list.h
#pragma once
#include <iostream>

template <typename T>
struct Node
{
    T data;
    Node<T>* next;
    Node<T>* prev;
    Node(const T& data)
    {
        this->data = data;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

template <typename T>
class List
{
private:
    Node<T>* head;
    int count;

public:
    List();
    List(const List<T>& obj);
    ~List();

    int getCount() const;
    bool empty() const;
    void push(const T& value, int index = 0);
    void pushHead(const T& value);
    void removeAt(int index);
    void clear();
    T& getData(int index = 0);
    void print();
};

#include "list.cpp"