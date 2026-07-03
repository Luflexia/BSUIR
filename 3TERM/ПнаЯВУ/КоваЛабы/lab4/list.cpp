// list.cpp
#pragma once
#include "list.h"

template <typename T>
List<T>::List() : head(nullptr), count(0) {}

template <typename T>
List<T>::List(const List<T>& obj)
{
    this->head = obj.head;
    this->count = obj.count;
}

template <typename T>
List<T>::~List()
{
    clear();
}

template <typename T>
int List<T>::getCount() const
{
    return count;
}

template <typename T>
bool List<T>::empty() const
{
    return head == nullptr;
}

template <typename T>
void List<T>::push(const T& value, int index)
{
    Node<T>* newNode = new Node<T>(value);
    Node<T>* tmp = head;

    if (empty())
    {
        head = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    }
    else
    {
        if (index <= 0)
        {
            // Вставка в начало списка
            newNode->next = head;
            newNode->prev = head->prev;
            newNode->prev->next = newNode;
            head->prev = newNode;
            head = newNode;
        }
        else
        {
            // Вставка на указанную позицию
            for (int i = 0; i < index && tmp->next != head; i++)
            {
                tmp = tmp->next;
            }

            newNode->next = tmp->next;
            newNode->prev = tmp;
            tmp->next->prev = newNode;
            tmp->next = newNode;
        }
    }
    count++;
}


template <typename T>
void List<T>::pushHead(const T& value)
{
    push(value, 0);
}

template <typename T>
void List<T>::removeAt(int index)
{
    if (empty())
        return;
    if (count == 1)
    {
        delete head;
        head = nullptr;
        count--;
        return;
    }
    Node<T>* tmp = head;
    for (int i = 0; index > 0 ? i < index : i < count - 1; i++)
    {
        tmp = tmp->next;
    }
    if (tmp == head)
    {
        head = head->next;
    }
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    delete tmp;

    count--;
}

template <typename T>
void List<T>::clear()
{
    while (!empty())
        removeAt(0);
}

template <typename T>
T& List<T>::getData(int index)
{
    Node<T>* tmp = head;
    if (index >= 0)
    {
        for (int i = 0; i < index; i++)
        {
            tmp = tmp->next;
        }
    }
    return tmp->data;
}

template <typename T>
void List<T>::print()
{
    Node<T>* tmp = head;
    for (int i = 0; i < count; i++)
    {
        std::cout << "Index: " << i << ", Value: " << tmp->data << std::endl;
        tmp = tmp->next;
    }
}