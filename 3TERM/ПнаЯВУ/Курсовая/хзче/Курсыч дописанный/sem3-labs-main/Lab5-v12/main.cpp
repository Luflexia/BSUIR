#include <iostream>

#define LENGTH 5

using namespace std;

template<typename T>  /// Set (mathematics)
class Set {
public:
    Set() {   // Constructor
        for (int i = 0; i < sizeof(array) / sizeof(*array); ++i) {
            array[i] = NULL;
        }
    }

    ~Set() = default;

    void add(T x) {
        if (pointer + 1 < sizeof(array) / sizeof(*array)) {
            array[pointer++] = x;
        } else {
            cout << "Error: Not enough free space" << endl;
            cout << "\tHit: Delete an element to free up space." << endl;
        }
    }

    void remove(int index) {
        if (index < 0 || index > sizeof(array) / sizeof(*array)) {
            cout << "Error: Invalid index";
            return;
        }

        if (index != sizeof(array) / sizeof(*array) - 1) {
            for (int i = index; i < sizeof(array) / sizeof(*array) - 1; ++i) {
                array[i] = array[i + 1];
            }
        }

        array[sizeof(array) / sizeof(*array) - 1] = NULL;
        pointer--;
    }

    void find(T x) {
        for (int i = 0; i < sizeof(array) / sizeof(*array); ++i)
            if (array[i] == x) {
                cout << i << " index of element " << x << endl;
                return;
            }
        cout << '"' << x << '"' << " - not found" << endl;
    }

    void view() {
        for (int i = 0; i < sizeof(array) / sizeof(*array); ++i) {
            if (array[i] == NULL)
                return;
            cout << i + 1 << ") Set[" << i << "] = " << array[i] << endl;
        }
    }

    void sort() {
        cout << "Before: ";
        for (int i = 0; i < sizeof(array) / sizeof(*array); ++i) {
            if (array[i] != NULL)
                cout << array[i] << " ";
        }
        cout << endl;

        for (int i = 0; i < sizeof(array) / sizeof(*array); i++) {
            for (int j = 0; j < sizeof(array) / sizeof(*array) - 1; j++) {
                if (array[j] > array[j + 1]) {
                    int b = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = b;
                }
            }
        }

        cout << "After: ";
        for (int i = 0; i < sizeof(array) / sizeof(*array); ++i) {
            if (array[i] != NULL)
                cout << array[i] << " ";
        }
        cout << endl;
    }

private:
    int pointer = 0;
    T array[LENGTH];
};

template<>
class Set<char *> {
public:
    Set() {
        cout << "An explicit template specialization has occurred." << endl;
    }
};

template<typename T>
void comparison(T a, T b) {
    if (a > b)
        cout << a << " > " << b << endl;
    else if (a < b)
        cout << a << " < " << b << endl;
    else if (a == b)
        cout << a << " = " << b << endl;
    else cout << "unknown" << endl;
}

int main() {
    cout << endl << "Set for 'int':" << endl;
    Set<int> A;
    A.add(4);
    A.add(1);
    A.add(16);
    A.add(66);

    cout << "\tView:" << endl;
    A.view();
    cout << endl;

    cout << "\tFind:" << endl;
    A.find(16);
    cout << endl;

    cout << "\tRemove:" << endl;
    A.remove(2);
    A.view();

    cout << "\tSorting:" << endl;
    A.sort();
    cout << endl;

    cout << endl << "Set for 'char *':" << endl;
    Set<char *> C;
    cout << endl;

    cout << "Comparison:" << endl;
    comparison(5, 7);
    comparison(23.4, 12.8);

    return 0;
}