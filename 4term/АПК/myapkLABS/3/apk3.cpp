#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>
int frequencies[] = { 784, 392, 440, 440, 784, 440, 493, 493 };

int duration[] = 200;

void sound(int frequency, int time) {

// b6 = 10110110 - channel 2(3), операция 11 – чтение/запись младшего, а затем  старшего байта, генерация прямоугольного импульса, 0 – двй код, 43h – регистр команд
    outp(0x43, 0xB6); 

    long signal = 1193180; // внутренняя частота таймера
    long cnt = signal / frequency; //Расчет значения счетчика для заданной частоты

    outp(0x42, cnt % 256); // запись младшего байта
    cnt /= 256; // запись  старший байта
    outp(0x42, cnt); // запись значения в порт динамика

    //включение динамика, 3 = 00000011
    outp(0x61, inp(0x61) | 3);

    delay(time);

    //выключение динамика, fc = 11111100
    outp(0x61, inp(0x61) & 0xFC);
}

void state_channel() {
    unsigned char temp;
    char* string = new char[9];

    //чтение состояния канала 0
    //11 – режим считывания, 10 – получение состояния канала, 001 – номер канала
    outp(0x43, 0xE2); //E2 = 11100010

    temp = inp(0x40); // получение слова состояния канала
    itoa(temp, string, 2); // integer to ascii переводим значение в строку
    cout << "Channel 0 state word: " << string << endl;

    // канал 1
    outp(0x43, 0xE4); //E4 = 11100100

    temp = inp(0x41);
    itoa(temp, string, 2);
    cout << "Channel 1 state word: " << string << endl;

    // канал 2
    outp(0x43, 0xE8); //E8 = 11101000

    temp = inp(0x42);
    itoa(temp, string, 2);
    cout << "Channel 2 state word: " << string << endl;

    free(string);
}

void print_counter(int channel) {

    //управляющее слово устанавливает режим чтения счетчика указанного канала
    unsigned char controlWord = 0xE2 | (channel << 1);
    outp(0x43, controlWord);

    unsigned short counter;
    outp(0x43, 0x00); // выбор младшего байта в регистре
    counter = inp(0x40); // Младший байт
    counter |= (inp(0x40) << 8); // Старший байт, сдвиг на 8 разрядов, логическое ИЛИ, объединение младшего и старшего байта в одно 16 битное значение

    cout << "Channel " << channel << " counter: " << hex << counter << endl;
}

int main() {
    for (int i = 0; i < 8; i++) {
        sound(frequencies[i], duration[i]);
    }
    state_channel();

    printf("\n");

    for (int j = 0; j < 3; j++) {
        print_counter(j);
    }
    return 0;
}
