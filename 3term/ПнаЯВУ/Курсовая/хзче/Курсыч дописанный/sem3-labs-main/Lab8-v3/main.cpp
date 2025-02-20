#include <iostream>

using namespace std;

class Exception {
    string errorMsg;
public:
    Exception(const string &msg) { errorMsg = "Error: " + msg; }

    ~Exception() = default;

    string what() { return errorMsg; }
};

template<typename T>
class Node {
public:
    T value;
    Node<T> *next = nullptr;

    Node(T value) { this->value = value; }
};

template<typename T>
class List {
    Node<T> *firstNode;
public:
    List() { firstNode = nullptr; }

    ~List() {
        if (!isEmpty()) {
            Node<T> *ptr = firstNode;
            Node<T> *ptrNext;
            while (ptr) {
                ptrNext = ptr->next;
                delete ptr;
                ptr = ptrNext;
            }
        }
    }

    void push(T value) {
        bool flag = false;
        try {
            if (find(value))
                flag = true;
        } catch (...) {}
        if (flag)
            throw Exception("This value is already in the list.");

        auto *ptr = new Node<T>(value);
        if (isEmpty()) {
            firstNode = ptr;
            return;
        }
        Node<T> *lastNode = firstNode; // last(current)
        while (lastNode->next != nullptr)
            lastNode = lastNode->next;
        lastNode->next = ptr;
        lastNode = ptr;
    }

    T pop(int index = 0) {
        if (isEmpty())
            throw Exception("List is empty.");
        if (index < 0)
            throw Exception("index - uses an invalid value.");

        Node<T> *ptr = firstNode;
        Node<T> *lastPtr = firstNode;
        int i = 0;
        while (ptr) {
            if (i == index)
                break;
            if (i++ > 0)
                lastPtr = lastPtr->next;
            ptr = ptr->next;
        }

        if (index != i || ptr == nullptr)
            throw Exception("index - uses an invalid value.");

        T tmp = ptr->value;
        if (index == 0)
            firstNode = ptr->next;
        else
            lastPtr->next = ptr->next;
        delete ptr;
        return tmp;
    }

    int find(T value) {
        if (isEmpty())
            throw Exception("List is empty.");

        Node<T> *ptr = firstNode;
        int index = 0;
        while (ptr) {
            if (ptr->value == value)
                return index;
            index++;
            ptr = ptr->next;
        }
        throw Exception("Value not found.");
    }

    void print() {
        if (isEmpty()) {
            cout << "List is empty." << endl;
            return;
        }
        Node<T> *ptr = firstNode;
        while (ptr) {
            cout << ptr->value << " ";
            ptr = ptr->next;
        }
        cout << endl;
    }

    bool isEmpty() { return firstNode == nullptr; }

    size_t size() {
        if (isEmpty())
            return sizeof(nullptr);

        int counter = 0;
        Node<T> *ptr = firstNode;
        while (ptr) {
            counter++;
            ptr = ptr->next;
        }
        return (sizeof(Node<T>) * counter);
    }

    Node<T> *operator[](const int index) {
        if (isEmpty())
            throw Exception("List is empty.");
        if (index < 0)
            throw Exception("index - uses an invalid value.");

        Node<T> *ptr = firstNode;
        int i = 0;
        while (ptr) {
            if (i == index)
                break;
            i++;
            ptr = ptr->next;
        }

        if (index != i || ptr == nullptr)
            throw Exception("index - uses an invalid value.");

        return ptr;
    }
};

template<typename T>
void printExample(List<T> list, T array[5]) {
    try {
        for (int i = 0; i < 4; ++i)
            list.push(array[i]);

        cout << "Add elements of the array to the list (from 1st to 4th):\n  ";
        list.print();

        cout << "We find \"" << array[2] << "\" in the list and delete it:\n  ";
        list.pop(list.find(array[2]));
        list.print();

        cout << "Adding the 5th element of the array to the list:\n  ";
        list.push(array[4]);
        list.print();
    } catch (Exception exception) {
        cout << exception.what() << endl;
    } catch (...) {
        unexpected();
    }
}

int main() {
    List<int> list1;
    List<double> list2;
    List<char> list3;

    cout << "\n\tExample using <Integer> type:\n";
    int array1[] = {19, 20, -33, 41, 50};
    printExample(list1, array1);
    cout << "\n\tExample using <Double> type:\n";
    double array2[] = {1.9, 0.2, -0.03, 1.45, 6.7};
    printExample(list2, array2);
    cout << "\n\tExample using <Char> type:\n";
    char array3[] = {'3', 'd', 'H', '*', 'q'};
    printExample(list3, array3);

    return 0;
}