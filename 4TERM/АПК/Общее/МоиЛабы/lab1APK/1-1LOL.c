#include <windows.h>
#include <stdio.h>

int setComPort(HANDLE com);

int main()
{
    HANDLE com1 = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    HANDLE com2 = CreateFile(L"COM4", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    char data = '1', result;
    DWORD bytesWritten, bytesRead;

    if (com1 == INVALID_HANDLE_VALUE || com2 == INVALID_HANDLE_VALUE)
    {
        printf("Error while open com port.\n");
        CloseHandle(com1);
        CloseHandle(com2);
        return 0;
    }

    if (!setComPort(com1) || !setComPort(com2))
    {
        CloseHandle(com1);
        CloseHandle(com2);
        return 0;
    }

    if (WriteFile(com1, &data, sizeof(data), &bytesWritten, NULL))
        printf("Data written succesfully.\n");
    else
    {
        printf("Error while writing data.\n");
        return 0;
    }

    if (ReadFile(com2, &result, sizeof(data), &bytesRead, NULL))
        printf("Data read successfully.\n");
    else
    {
        printf("Error while reading data.\n");
        return 0;
    }
    printf("Read data: %c\n", result);

    CloseHandle(com1);
    CloseHandle(com2);
    return 0;
}

int setComPort(HANDLE com)
{
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(com, &dcbSerialParams)) {       // Получение параметров ком-порта.
        printf("Can\'t get state of com port.\n");
        return 0;
    }
    dcbSerialParams.BaudRate = CBR_9600;     // Скорость передачи.
    dcbSerialParams.ByteSize = 8;            // Размер байта.
    dcbSerialParams.StopBits = ONESTOPBIT;   // Длина стопового бита.
    dcbSerialParams.Parity = NOPARITY;       // Паритет (для отслеживания ошибок).
    if (!SetCommState(com, &dcbSerialParams)) {       // Сохранение параметров ком-порта.
        printf("Can\'t set state of com port.\n");
        return 0;
    }
    return 1;
}