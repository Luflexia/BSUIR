#include <iostream>
#include <string>
#include <exception>
#define MAX_SIZE 100
#define TYPE int

using namespace std;

class MatrixException
{
private:
    string message = "Matrix exception";

public:
    MatrixException(string&& message) : message(message) {}
    const char* what() const noexcept
    {
        return message.c_str();  // т.к. функция работает с указателями, возвращаем указать на строку
    }
};


template <typename T>
class Matrix
{
public:
    Matrix(T **array, int matrix_rows, int matrix_columns)
    {
        this->matrix_rows = matrix_rows;
        this->matrix_columns = matrix_columns;
        for (int i = 0; i < matrix_rows; i++)
        {

            for (int j = 0; j < matrix_columns; j++) {
                this->matrix_array[i][j] = array[i][j];
            }
        }
    }
    void output()
    {
        for (int i = 0; i < matrix_rows; i++)
        {

            for (int j = 0; j < matrix_columns; j++) {
                cout << this->matrix_array[i][j] << " ";
            }
            cout << endl;
        }
    }
    bool square()
    {
        if (matrix_rows == matrix_columns)
        {
            return 1;
        }
        return 0;
    }
    void transposition()
    {
        for (int i = 0; i < matrix_columns; i++)
        {

            for (int j = 0; j < matrix_rows; j++) {
                cout << this->matrix_array[j][i] << " ";
            }
            cout << endl;
        }
    }
    bool operator==(const Matrix& other)
    {
        if ((this->matrix_rows != other.matrix_rows) || (this->matrix_columns != other.matrix_columns))
            throw MatrixException("Matrices have different sizes");

        cout << "1111111" << endl;


        for (int i = 0; i < matrix_rows; i++)
        {
            for (int j = 0; j < matrix_columns; j++) {
                if (this->matrix_array[i][j] != other.matrix_array[i][j])
                    return 0;
            }
        }

        return 1;
    }
private:
    T matrix_array[MAX_SIZE][MAX_SIZE];
    int matrix_rows;
    int matrix_columns;
};

int main()
{
    srand(time(0));
    int matrix_rows, matrix_columns;
    cout << "enter value of rows " << endl;
    cin >> matrix_rows;
    cout << "enter value of columns " << endl;
    cin >> matrix_columns;

    TYPE** array = new TYPE * [matrix_rows];
    for (int i = 0; i < matrix_rows; i++)
    {
        array[i] = new TYPE[matrix_columns];
    }
    cout << "enter array" <<endl;
    for (int i = 0; i < matrix_rows; i++)
    {

        for (int j = 0; j < matrix_columns; j++) {
            cin >> array[i][j];
        }
    }


    TYPE** array2 = new TYPE * [matrix_rows+1];
    for (int i = 0; i < matrix_rows+1; i++)
    {
        array2[i] = new TYPE[matrix_columns];
    }
    cout << "enter array" << endl;
    for (int i = 0; i < matrix_rows+1; i++)
    {

        for (int j = 0; j < matrix_columns; j++) {
            array2[i][j] = 1 + rand() % 10;
        }
    }


    try
    {
        auto my_matrix = Matrix<TYPE>(array, matrix_rows, matrix_columns);
        cout << "\n\nMatrix" << endl;
        my_matrix.output();
        cout << "\nMatrix is square?" << endl ;
        cout << my_matrix.square() << endl << endl;
        cout << "Transposition" << endl;
        my_matrix.transposition();
        cout << "\n\nMatrix == Matrix?" << endl;
        cout << (my_matrix == my_matrix)<<endl;

        auto my_matrix2 = Matrix<TYPE>(array2, matrix_rows+1, matrix_columns);
        cout << "\n\nMatrix2" << endl;
        my_matrix2.output();
        cout << "\n\nMatrix == Matrix2?" << endl; // !
        cout << (my_matrix == my_matrix2);
    }
    catch (const MatrixException& e)
    {
        cout << e.what() << endl; // what - метод класса exception, который возвращающая указатель на строку, так и получаем строку с причиной исключительной ситуации
        int unexpected();
    }

    for (int i = 0; i < matrix_rows; i++)
    {
        delete (array[i]);
    }
    delete[] array;

    for (int i = 0; i < matrix_rows+1; i++)
    {
        delete (array2[i]);
    }
    delete[] array2;
}