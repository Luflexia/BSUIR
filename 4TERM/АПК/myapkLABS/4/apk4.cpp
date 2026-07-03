#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

unsigned char data[7]; // данные часов
unsigned int delayTime = 0;
unsigned int delayInterval = 1; // интервал генерации задержки в миллисекундах
unsigned int registerArray[] = { 0x00, 0x02, 0x04, 0x06, 0x07, 0x08, 0x09 };
int alarmOn = 0;
char* months[] =
{
    "JANUARY",
    "FEBRUARY",
    "MARCH",
    "APRIL",
    "MAY",
    "JUNE",
    "JULY",
    "AUGUST",
    "SEPTEMBER",
    "OCTOBER",
    "NOVEMBER",
    "DECEMBER"
};

void interrupt newTime(...);  // новый обработчик прерываний часов
void interrupt newAlarm(...); // новый обработчик прерываний будильника
void interrupt(*lastTime)(...); // старое прерывание часов
void interrupt(*lastAlarm) (...); // старое прерывание будильника

void Menu();
void ShowTime();
int ConvertToDecimal(int BCD);
int convertToBCD(int decimal);
void setTime();
void MyDelay(unsigned int delayMs);
void enterTime();
void setAlarm();
void resetAlarm();
void setDelayInterval(unsigned int interval);

int main()
{
    Menu();
    return 0;
}

void Menu() {
    while (1) {
        system("cls");
        ShowTime();
        printf("\n1 - Set time  2 - Set delay  3 - Set delay interval  4 - Set alarm  0 - Exit");
        if(alarmOn == 1) printf("\n\nALARM ON");
        if(alarmOn == 2) {
            printf("\n\nALARM! ALARM! ALARM!");
            delay(5000);
            alarmOn = 0;
        }

        printf("\n\nEnter choice: ");
        delay(1000);
        if (kbhit()) {
            switch(getch()) {
            case '0':
                return;

            default:
                break;

            case '1':
                system("cls");
                setTime();
                break;

            case '2':
                system("cls");
                unsigned int delayMs = 0;
                printf("Input delay (ms): ");
                scanf("%d", &delayMs);
                MyDelay(delayMs);
                break;

            case '3':
                system("cls");
                unsigned int interval = 0;
                printf("Input delay interval (ms): ");
                scanf("%d", &interval);
                setDelayInterval(interval);
                break;

            case '4':
                system("cls");
                setAlarm();
                break;
            }
        }
    }
}

void ShowTime() {

    int i = 0;
    for (i = 0; i < 7; i++) {
        outp(0x70, registerArray[i]); // выбор адреса в памяти CMOS
        data[i] = inp(0x71); // считывание значения по адресу в массив
    }

    int decimalData[7]; // перевод значений в десятичный вид
    for (i = 0; i < 7; i++) {
        decimalData[i] = ConvertToDecimal(data[i]);
    }

    // вывод на экран
    printf("%02d:%02d:%02d %02d.%02d.%02d\n", decimalData[2], decimalData[1], decimalData[0], decimalData[3], decimalData[4], decimalData[5]);
}

int ConvertToDecimal(int BCD) {
    return ((BCD / 16 * 10) + (BCD % 16));
}

int ConvertToBCD(int decimal)
{
    return ((decimal / 10 * 16) + (decimal % 10));
}

void setTime()
{
    enterTime(); // ввод нового времени
    disable(); // запрет на прерывание

    // проверка на доступность значений для чтения/записи
    unsigned int check;
    do {
        outp(0x70, 0xA); // выбор регистра А
        check = inp(0x71) & 0x80; // 0x80 - 1000 0000
        // 7-й бит в 1 для обновления времени
    } while (check);

    // отключение обновления часов реального времени
    outp(0x70, 0xB); // выбор регистра B
    outp(0x71, inp(0x71) | 0x80); // 0x80 - 1000 0000
    // 7-й бит в 1 для запрета обновления часов

    for (int i = 0; i < 7; i++) {
        outp(0x70, registerArray[i]); // выбор нужного значения данных
        outp(0x71, data[i]); // подача в регистр нужного значения
    }

    // включение обновления часов реального времени
    outp(0x70, 0xB); // выбор регистра В
    outp(0x71, inp(0x71) & 0x7F); // 0x7F - 0111 1111
    // 7-й бит в 0 для разрешения обновления часов

    enable(); // разрешение на прерывание
    system("cls");
}

void enterTime() {
    int enter;

    do {
        rewind(stdin);
        printf("Enter century: ");
        scanf("%i", &enter);
    } while ((enter > 99 || enter < 0));
    data[6] = ConvertToBCD(enter);

    do {
        rewind(stdin);
        printf("Enter year: ");
        scanf("%i", &enter);
    } while ((enter > 99 || enter < 0));
    data[5] = ConvertToBCD(enter);

    do {
        rewind(stdin);
        printf("Enter month: ");
        scanf("%i", &enter);
    } while ((enter > 12 || enter < 1));
    data[4] = ConvertToDecimal(enter);

    do {
        rewind(stdin);
        printf("Enter day: ");
        scanf("%i", &enter);
    } while ((enter > 31 || enter < 1));
    data[3] = ConvertToBCD(enter);

    do {
        rewind(stdin);
        printf("Enter hours: ");
        scanf("%i", &enter);
    } while ((enter > 23 || enter < 0));
    data[2] = ConvertToBCD(enter);

    do {
        rewind(stdin);
        printf("Enter minutes: ");
        scanf("%i", &enter);
    } while (enter > 59 || enter < 0);
    data[1] = ConvertToBCD(enter);

    do {
        rewind(stdin);
        printf("Enter seconds: ");
        scanf("%i", &enter);
    } while (enter > 59 || enter < 0);
    data[0] = ConvertToBCD(enter);
}

void MyDelay(unsigned int delayMs)
{
    disable(); // запрет на прерывание

    // установка нового обработчика прерываний
    lastTime = getvect(0x70);
    setvect(0x70, newTime);

    enable(); // разрешение на прерывание

    // размаскировка линии сигнала запроса от ЧРВ
    // 0xA1 - новое значение счетчика для системного таймера
    outp(0xA1, inp(0xA1) & 0xFE); // 0xFE = 1111 1110
    // 0-й бит в 0 для разрешения прерывания от ЧРВ

    outp(0x70, 0xB); // выбор регистра В
    outp(0x71, inp(0x71) | 0x40); // 0x40 = 0100 0000
    // 6-й бит регистра B установлен в 1 для периодического прерывания

    delayTime = 0;
    while (delayTime <= delayMs);
    setvect(0x70, lastTime);
    return;
}

void setAlarm()
{
    enterTime(); // ввод нового времени
    disable(); // запрет на прерывание

    // проверка на доступность значений для чтения/записи
    unsigned int check;
    do {
        outp(0x70, 0xA); // выбор регистра A
        check = inp(0x71) & 0x80; // 0x80 - 1000 0000
        // 7-й бит в 1 для обновления времени
    } while (check);

    // установка часов в регистр будильника
    outp(0x70, 0x05);
    outp(0x71, data[2]);

    // установка минут в регистр будильника
    outp(0x70, 0x03);
    outp(0x71, data[1]);

    // установка секунд в регистр будильника
    outp(0x70, 0x01);
    outp(0x71, data[0]);

    // установка бита будильника в регистре B
    outp(0x70, 0xB); // выбор регистра B
    outp(0x71, (inp(0x71) | 0x20)); // 0x20 - 0010 0000
    // 5-й бит регистра B установлен в 1 для разрешения прерывания будильника

    // переопределение прерывания будильника
    lastAlarm = getvect(0x70); // 0x70 - IRQ8 (CMOS Real-Time Clock)
    setvect(0x70, newAlarm);

    enable(); // разрешение на прерывание
    alarmOn = 1;
}

void resetAlarm()
{
    // проверка на наличие установленного будильника
    if (lastAlarm == NULL)
        return;

    disable(); // запрет на прерывание

    // возврат старого прерывания
    setvect(0x70, lastAlarm);  // 0x70 - IRQ8 (CMOS Real-Time Clock)

    // проверка на доступность значений для чтения/записи
    unsigned int check;
    do {
        outp(0x70, 0xA); // выбор регистра A
        check = inp(0x71) & 0x80;  // 0x80 - 1000 0000
        // 7-й бит в 1 для обновления времени
    } while (check);

    // запись нулевых значений в регистр будильника
    outp(0x70, 0x05); // 0x05 - часы
    outp(0x71, 0x00);

    outp(0x70, 0x03); // 0x03 - минуты
    outp(0x71, 0x00);

    outp(0x70, 0x01); // 0x01 - секунды
    outp(0x71, 0x00);

    // сброс бита будильника в регистре B
    outp(0x70, 0xB); // выбор регистра B
    outp(0x71, (inp(0x71) & 0xDF)); // 0xDF - 1101 1111
    // 5-й бит в 0 для запрета прерывания будильника

    enable(); // разрешение на прерывание
}

void interrupt newTime(...) // новый обработчик прерываний часов
{
    delayTime++;
    outp(0x70, 0x0C); // выбор адреса в памяти CMOS (запись)
    inp(0x71); // данные по этому адресу (запись/чтение)
    // посыл сигнала контроллерам прерываний об окончании прерывания
    outp(0x20, 0x20);
    outp(0xA0, 0x20);
}

void interrupt newAlarm(...) // новый обработчик прерываний будильника (IRQ8)
{
    system("cls");
    alarmOn = 2;
    lastAlarm();
    resetAlarm();
}

void setDelayInterval(unsigned int interval)
{
    delayInterval = interval;
}
