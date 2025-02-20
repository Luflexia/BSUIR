#include <windows.h>
#include <iostream.h>
using namespace std;

// Глобальные переменные для COM портов 
HANDLE COM_Port_1;
LPCTSTR Port_Name_1 = L"COM1";
HANDLE COM_Port_2;
LPCTSTR Port_Name_2 = L"COM2";

// Функция для чтения из порта COM2
void Read_from_COM()
{
    DWORD Size;
    char Received_Char;
    
    // Чтение символа из порта COM2
    ReadFile(COM_Port_2, &Received_Char, 1, &Size, 0); 
    if (Size > 0) 
    {
        cout << "Received: " << Received_Char << endl; // Вывод принятого символа
    }
}

int main()
{
    // Открытие COM1 и COM2 для записи и чтения
    COM_Port_1 = ::CreateFile(Port_Name_1, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    COM_Port_2 = ::CreateFile(Port_Name_2, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    // Проверка открытия портов
    if (COM_Port_1 == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            cout << "COM-port 1 does not exist!\n";
        }
        cout << "Some other error opening COM-port 1.\n";
    }

    if (COM_Port_2 == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            cout << "COM-port 2 does not exist!\n";
        }
        cout << "Some other error opening COM-port 2.\n";
    }

    // Настройка параметров порта COM1 для передачи данных
    DCB Serial_Params = { 0 };
    Serial_Params.DCBlength = sizeof(Serial_Params);
    if (!GetCommState(COM_Port_1, &Serial_Params))
    {
        cout << "Getting state error for COM-port 1.\n";
    }
    Serial_Params.BaudRate = CBR_9600;
    Serial_Params.ByteSize = 8;
    Serial_Params.StopBits = ONESTOPBIT;
    Serial_Params.Parity = NOPARITY;
    if (!SetCommState(COM_Port_1, &Serial_Params))
    {
        cout << "Error setting serial port state for COM-port 1.\n";
    }

    char data = 'A'; 
    DWORD Size = sizeof(data);  
    DWORD Bytes_Written;    

    // Отправка данных из порта COM1
    BOOL Ret = WriteFile(COM_Port_1, &data, Size, &Bytes_Written, NULL);

    // Вывод информации о количестве отправленных байт
    cout << Size << " Bytes in string. " << Bytes_Written << " Bytes sent from COM-port 1. " << endl;

    // Чтение данных из порта COM2
    Read_from_COM();

    return 0;
}
