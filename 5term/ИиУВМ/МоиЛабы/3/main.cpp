#include "stdafx.h"
#include "hexioctrl.h"
#include "(PCI_DEVS)pci_codes.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <string.h>

unsigned short driveData[256]; 

void WaitWhileBusy(int channel);
bool RetrieveDriveInfo(int device, int channel);
void DisplayDriveDetails();
bool WaitUntilReady(int channel); 

int main()
{
    ALLOW_IO_OPERATIONS;
    setlocale(LC_ALL, "Russian");

    // Цикл по каждому каналу (0(P), 1(S))
    for (int channel = 0; channel < 2; channel++) 
    {
        // Цикл по каждому устройству (0(M), 1(S))
        for (int device = 0; device < 2; device++) 
        {
            if (RetrieveDriveInfo(device, channel))
            {
                printf("\n Канал %d | Устройство %d\n", channel, device);
                printf("======================================\n");
                DisplayDriveDetails();
            }
        }
    }

    system("pause");
    return 0;
}

// Проверка готовности устройства к записи команды
bool WaitUntilReady(int channel)
{
    // Определение порта для чтения статуса
    unsigned short statusPort = (channel == 0) ? 0x3F6 : 0x376; // 0x3F6/0x376 - для первичного/вторичного канала

    for (int i = 0; i < 100; i++)
    {
        unsigned char status = _inp(statusPort);
        if (status & (1 << 6)) return true; // Установлен ли бит DRDY (готовность)
    }
    return false;
}

// Ожидание сброса бита BSY
void WaitWhileBusy(int channel)
{
    // Определение порта для чтения статуса
    unsigned short statusPort = (channel == 0) ? 0x3F6 : 0x376; 

    unsigned char status;
    do
    {
        status = _inp(statusPort);
    } while (status & (1 << 7)); // 7 бит BSY в 0 (не занят
}

// Получение информации об устройстве
bool RetrieveDriveInfo(int device, int channel)
{
    // Адрес регистра Device/Head для выбора устройства
    unsigned short deviceHeadPort = (channel == 0) ? 0x1F6 : 0x176; // 0x1F6/0x176 - первичный/вторичный каналы, выбор устройства

    // Выбор команды Identify
    unsigned short commandPort = (channel == 0) ? 0x1F7 : 0x177; // 0x1F7/0x177 - первичный/вторичный каналы, отправка команды

    for (int attempt = 0; attempt < 2; attempt++) // 1try 0xA1 для ATAPI и 2try 0xEC для ATA
    {
        WaitWhileBusy(channel); 

        unsigned char devHead = (device << 4) + 0xE0; // Формирование значения Device/Head 0x00(M)+ или 0x10(S)+
        _outp(deviceHeadPort, devHead);

        if (!WaitUntilReady(channel)) return false;

        // Отправка команды Identify (0xA1 для ATAPI, 0xEC для ATA)
        _outp(commandPort, (attempt == 0) ? 0xA1 : 0xEC);
        WaitWhileBusy(channel);

        unsigned char status = _inp(commandPort); // Чтение статуса
        if (!(status & (1 << 3))) // Проверка бита DRQ (устройство готово)
        {
            if (attempt == 1) return false; // Если после второй попытки данных нет, выйти
            continue;
        }
        else break;
    }

    // Считывание данных устройства
    unsigned short dataPort = (channel == 0) ? 0x1F0 : 0x170; // Регистр DR
    for (int i = 0; i < 256; i++)
    {
        driveData[i] = _inpw(dataPort);
    }

    return true;
}

// Отображение данных об устройстве
void DisplayDriveDetails()
{
    printf("   Модель устройства: ");
    for (int i = 27; i <= 46; i++) // Слова 27-46: модель устройства
        printf("%c%c", driveData[i] >> 8, driveData[i] & 0xFF); // Старший и младший байты слова

    printf("\n   Серийный номер: ");
    for (int i = 10; i <= 19; i++) // Слова 10-19: серийный номер
        printf("%c%c", driveData[i] >> 8, driveData[i] & 0xFF);

    printf("\n   Версия прошивки: ");
    for (int i = 23; i <= 26; i++) // Слова 23-26: версия прошивки
        printf("%c%c", driveData[i] >> 8, driveData[i] & 0xFF);

    printf("\n   Тип интерфейса: ");
    if (driveData[0] & (1 << 15)) // Проверка бита 15: 1=ATAPI или 0=ATA
        printf("ATAPI\n");
    else
        printf("ATA\n");

    if (!(driveData[0] & (1 << 15))) // ATAPI / ATA
    {
        // Расчет объема (слова 60-61)
        printf("   Объем: %.2lf ГБ\n", (long double)(((unsigned long *)driveData)[30]) * 512 / (1024 * 1024 * 1024));
    }															 // Кол-во секторов      1 сектор байт     в ГБ

    printf("\n   Поддерживаемые режимы:\n");
    printf("   PIO: ");
    if (driveData[64] & 1) printf("PIO 3, ");
    if (driveData[64] & 2) printf("PIO 4\n");

    printf("   Multiword DMA: ");
    if (driveData[63] & 1) printf("MWDMA 0, ");
    if (driveData[63] & 2) printf("MWDMA 1, ");
    if (driveData[63] & 4) printf("MWDMA 2\n");

    printf("   Ultra DMA: ");
    if (driveData[88] & 1) printf("UDMA 0, ");
    if (driveData[88] & (1 << 1)) printf("UDMA 1, ");
    if (driveData[88] & (1 << 2)) printf("UDMA 2, ");
    if (driveData[88] & (1 << 3)) printf("UDMA 3, ");
    if (driveData[88] & (1 << 4)) printf("UDMA 4, ");
    if (driveData[88] & (1 << 5)) printf("UDMA 5\n");

    printf("   Версии ATA:" );
    if(driveData[80] & (1 << 1)) printf(" ATA 1," );
    if(driveData[80] & (1 << 2)) printf(" ATA 2," );
    if(driveData[80] & (1 << 3)) printf(" ATA 3," );
    if(driveData[80] & (1 << 4)) printf(" ATA 4," );
    if(driveData[80] & (1 << 5)) printf(" ATA 5," );
    if(driveData[80] & (1 << 6)) printf(" ATA 6," );
    if(driveData[80] & (1 << 7)) printf(" ATA 7\n" );

    printf("\n-----------------------------------------------------------------\n");
}