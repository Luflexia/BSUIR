#include <iostream>
using namespace std;

template <typename T>
T max_element(const T* array, const int &num)
{
    T max = array[0];
    for (int i = 1; i < num; i++)
    {
        if (max < array[i])
            max = T(array[i]);
    }
    return max;
}

int main()
{
    int num;
    cout << "enter integer value of elements" << endl;
    cin >> num;
    int* array = new int[num];
    cout << "enter integer elements of array" << endl;

    for (int i = 0; i < num; i++)
    {
        int buf;
        cin >> buf;
        array[i] = buf;
    }
    cout<< "Max element of array is " << max_element(array, num) << endl;

    delete[] array;

    /////////////////////////////

    cout << "enter integer value of elements" << endl;
    cin >> num;
    double* array2 = new double[num];
    cout << "enter double elements of array" << endl;
    for (int i = 0; i < num; i++)
    {
        double buf;
        cin >> buf;
        array2[i] = buf;
    }
    cout << "Max element of array is " <<  max_element(array2, num);

    delete[] array2;
}