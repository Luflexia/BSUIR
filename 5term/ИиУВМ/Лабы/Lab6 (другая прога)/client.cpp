#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <filesystem>
#include <winsock2.h>
#include <conio.h>
#include <ws2bth.h>
#include <vector>
#include <thread>
#include <mmsystem.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

#define ALIAS "random_str"
#define FILDIR "D:\\files\\"

static const GUID MY_SERVICE = { 0x6afcbbcb, 0x995a, 0x425d, { 0xa0, 0x95, 0x9f, 0x6e, 0xac, 0x4f, 0x8e, 0x74 } };

using namespace std;
namespace fs = std::filesystem;
using namespace cv;

struct BluetoothDevice {
    wstring name;
    BTH_ADDR addr{};
    SOCKADDR sock_addr{};
};

vector<BluetoothDevice> bluetooth_devs = vector<BluetoothDevice>();
SOCKADDR_BTH sockAddr;
SOCKET bluetooth_socket{};
bool socket_connected = false;
int connect_ind{};
bool end_flag = false;
DWORD err = 0;
bool end_of_connection = false;
bool recv_thread_is_over = true;
bool thread_is = false;
thread recieve_thread;

INT get_service_port(LPSOCKADDR addr);
BOOL get_devices_list();
void print_menu();
void print_bluetooth_devices();
void connect_to_device(int dev_num);
void disconnect_from_serv();
void send_file(string f_name);
string choose_file_from_dir();
void recieve_procc(SOCKET client);
void play_audio(string file_name);
void record_file(string fname);
INT make_photo(string photo_n);
INT show_png(string photo_n);


int main()
{
    setlocale(LC_ALL, "rus");
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

    WSADATA wsaData;
    int error;

    error = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (error != 0) {
        printf("WSAStartup failed with error: %d\n", error);
        return 1;
    }

    char choice = 'c';
    print_menu();
    while (choice != '8') {
        choice = _getche();
        switch (choice)
        {
        case '1':
        {
            system("cls");
            cout << "Finding devices, please wait...";
            get_devices_list();
            print_bluetooth_devices();
            system("pause");
            system("cls");
            print_menu();
            break;
        }
        case '2':
        {
            if (recv_thread_is_over == 0) {
                system("cls");
                cout << "Disconnecting from server...\n";
                disconnect_from_serv();
                cout << "Disconnected\n\n";

            }
            else {
                system("cls");
                print_bluetooth_devices();
                cout << "Choose device number to connect: ";
                int devNum;
                cin >> devNum;
                if (recv_thread_is_over == 0) {
                    recieve_thread.join();
                    recv_thread_is_over = 1;
                }
                connect_to_device(devNum);
                if (socket_connected == false)
                    cout << "\n...Connection failed\n";
                else {
                    recieve_thread = thread(recieve_procc, bluetooth_socket);
                }
            }
            system("pause");
            system("cls");
            print_menu();
            break;
        }
        case '3':
        {
            system("cls");
            cout << "Trying to send file...\n";
            string fname;
            fname = choose_file_from_dir();
            if (fname == "") {
                cout << "\nFile is not chosen\n";
            }
            else {
                send_file(fname);
                cout << "\nYour file has been sent\n\n";
            }
            system("pause");
            system("cls");
            print_menu();
            break;
        }
        case '4':
        {
            system("cls");
            cout << "Recording Aydio...\n";
            string fname;
            cout << "Enter file name: ";
            cin >> fname;
            fname = FILDIR + fname + ".wav";
            cout << "Wait a few seconds...\n";
            record_file(fname);
            system("pause");
            system("cls");
            print_menu();
            break;
        }
        case '5':
        {
            system("cls");
            cout << "Playing audio file...\n";
            string fname;
            fname = choose_file_from_dir();
            if (fname == "") {
                cout << "\nFile is not chosen\n";
            }
            else {
                cout << endl << fname << " ";
                play_audio(fname);
            }
            system("pause");
            system("cls");
            print_menu();
            break;
        }
        case '6': {
            system("cls");
            cout << "Taking photo...\n";
            string fname;
            cout << "Enter file name:";
            cin >> fname;
            fname = FILDIR + fname + ".png";
            cout << "\nPhoto will be taking now!";
            make_photo(fname);
            system("pause");
            system("cls");
            print_menu();
            break;
        }
        case '7': {
            system("cls");
            cout << "Opening photo...\n";
            string fname;
            fname = choose_file_from_dir();

            if (fname == "") {
                cout << "\nFile is not chosen\n";
            }
            system("pause");
            system("cls");
            print_menu();
            break;
        }
        }
    }
    if (recv_thread_is_over == 0) {
        recieve_thread.join();
        recv_thread_is_over = 1;
    }
    WSACleanup();
    return 0;
}


INT get_service_port(LPSOCKADDR addr)
{
    WSAQUERYSET wsaq; // структура дл€ определени€ параметров запроса
    ZeroMemory(&wsaq, sizeof(WSAQUERYSET));
    wsaq.dwSize = sizeof(WSAQUERYSET);
    wsaq.dwNameSpace = NS_BTH;

    // выделение пам€ти дл€ строки bluetooth-адреса
    DWORD addressLength = 128;
    wchar_t* addressString = new wchar_t[addressLength];
    // преобразование bluetooth-адреса в строку
    int result = WSAAddressToStringW(addr,
        sizeof(SOCKADDR_BTH),
        NULL,
        addressString,
        &addressLength);
    // guid дл€ идентификации bluetooth-сервиса
    GUID guid = MY_SERVICE;
    wsaq.lpServiceClassId = &guid;
    wsaq.lpszContext = addressString;

    HANDLE hLookup;
    // функци€ начала процесса поиска
    int iRet = WSALookupServiceBegin(&wsaq, LUP_FLUSHCACHE, &hLookup);
    if (WSAGetLastError() != 0)
    {
        cout << "Bluetooth problems" << endl;
        system("pause");
        return false;
    }
    // цикл поиска
    while (1)
    {
        DWORD dw_size = 0;
        CHAR* buf = new CHAR[4096];
        LPWSAQUERYSET pwsa_result = (LPWSAQUERYSET)buf;
        ZeroMemory(pwsa_result, sizeof(WSAQUERYSET));
        // параметры поиска
        pwsa_result->dwSize = sizeof(WSAQUERYSET);
        pwsa_result->dwNameSpace = NS_BTH;
        pwsa_result->lpBlob = NULL;

        dw_size = 4096;
        // выполнение поиска следующего сервиса
        iRet = WSALookupServiceNext(hLookup, LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_FLUSHCACHE, &dw_size, pwsa_result);
        if (WSAGetLastError() != 0)
        {
            delete[] buf;
            break;
        }

        // извлечение порта из информации
        INT res = ((SOCKADDR_BTH*)pwsa_result->lpcsaBuffer->RemoteAddr.lpSockaddr)->port;

        delete[] buf;
        return res;
    }
    // завершение поиска сервисов
    WSALookupServiceEnd(hLookup);
    return -1;
}

BOOL get_devices_list()
{
    bluetooth_devs.clear();
    // инициализаци€ структуры поиска
    WSAQUERYSET wsaq;
    ZeroMemory(&wsaq, sizeof(WSAQUERYSET));
    wsaq.dwSize = sizeof(WSAQUERYSET);
    wsaq.dwNameSpace = NS_BTH;  // пространство имен дл€ bluetooth

    HANDLE hLookup;
    // функци€ начала процесса поиска
    int iRet = WSALookupServiceBegin(&wsaq, LUP_CONTAINERS | LUP_RETURN_ADDR | LUP_RETURN_NAME | LUP_FLUSHCACHE, &hLookup);
    if (WSAGetLastError() != 0)
    {
        cout << "\nBluetooth problems\n";
        return false;
    }

    while (1)
    {
        DWORD dw_size = 0;
        CHAR* buf = new CHAR[4096];
        // структура дл€ результатов поиска
        LPWSAQUERYSET pwsa_result = (LPWSAQUERYSET)buf;
        ZeroMemory(pwsa_result, sizeof(WSAQUERYSET));
        pwsa_result->dwSize = sizeof(WSAQUERYSET);
        pwsa_result->dwNameSpace = NS_BTH;
        pwsa_result->lpBlob = NULL;

        dw_size = 4096;
        // выполнение поиска следующего устройства
        iRet = WSALookupServiceNext(hLookup, LUP_CONTAINERS | LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_FLUSHCACHE, &dw_size, pwsa_result);
        if (WSAGetLastError() != 0)
        {
            delete[] buf;
            break;
        }
        // создание экземпл€ра и заполнение информации
        BluetoothDevice instance;
        instance.addr = 0;
        instance.name = pwsa_result->lpszServiceInstanceName;
        CHAR* p = (CHAR*)pwsa_result->lpcsaBuffer->RemoteAddr.lpSockaddr;
        instance.sock_addr = *(pwsa_result->lpcsaBuffer->RemoteAddr.lpSockaddr);
        // извлечение адреса из данных
        memcpy(&instance.addr, &p[2], 8);
        // добавление устройства
        bluetooth_devs.push_back(instance);
        delete[] buf;
    }
    // окончание поиска устройств
    WSALookupServiceEnd(hLookup);
    return true;
}


void print_menu() {
    cout << "\nActions List:\n\n" << "1. Get devices list\n";
    if (recv_thread_is_over == 1)
        cout << "2. Connect to device\n";
    else
        cout << "2. Disconnect\n";
    cout << "3. Send file\n" << "4. Make audio file\n" << "5. Play audio file\n"
        << "6. Make a photo\n" << "7. Take a look on photo\n" << "8. Exit from program\n\n";
}

void print_bluetooth_devices() {   // вывод устройств
    system("cls");
    if (bluetooth_devs.size() == 0)
        return;
    cout << "\n\nBluetooth devices list:\n"
        << "\n";
    for (int i = 0; i < bluetooth_devs.size(); i++)
    {
        if (bluetooth_devs[i].name.size() == 0) continue;
        cout << "\n" << i + 1 << " ";
        wcout << left << setw(40) << setfill(L' ') << bluetooth_devs[i].name << L" ";
        DWORD address_length = 128;
        wchar_t* address_string = new wchar_t[address_length];
        WSAAddressToStringW(&bluetooth_devs[i].sock_addr, sizeof(SOCKADDR_BTH), NULL, address_string, &address_length);
        wcout << endl << " " << left << setw(38) << setfill(L' ') << address_string << L"\n";
        cout << " ";
    }
    cout << "\n\n\n";
}

void connect_to_device(int dev_num) {
    // проверка, что номер устройства находитс€ в пределах диапазона
    if (dev_num >= 1 && dev_num <= bluetooth_devs.size())
    {
        // получение порта выбранного устройства
        INT port = get_service_port(&bluetooth_devs[dev_num - 1].sock_addr);
        // создание сокета
        bluetooth_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
        ZeroMemory(&sockAddr, sizeof(sockAddr));
        // настройка структуры дл€ соединени€
        sockAddr.addressFamily = AF_BTH;
        sockAddr.serviceClassId = MY_SERVICE; 
        sockAddr.port = port;
        sockAddr.btAddr = bluetooth_devs[dev_num - 1].addr;
        int error;
        for (int i = 0; i < 2; i++) {
            // поптыка установить соединение
            error = connect(bluetooth_socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
            if (error == 0) {
                wcout << L"Connected with " << bluetooth_devs[dev_num - 1].name << L"\n\n";
                err = 0;
                socket_connected = true;
                connect_ind = dev_num - 1;
                break;
            }
            else {
                err = WSAGetLastError();
            }
        }
    }
}

void disconnect_from_serv() {
    int comand = 2;
    send(bluetooth_socket, (char*)&comand, 4, 0); // отправка команды 2 на сервер
    end_of_connection = true;
    closesocket(bluetooth_socket);
    socket_connected = false;
}

void send_file(string f_name) {
    int com = 1;
    send(bluetooth_socket, (char*)&com, 4, 0); // отправка команды 1 на сервер
    int file_len = f_name.length(); 
    send(bluetooth_socket, (char*)&file_len, 4, 0); // отправка длины имени файла
    send(bluetooth_socket, f_name.c_str(), f_name.length(), 0); // отправка имени файла
    FILE* file;
    fopen_s(&file, f_name.c_str(), "rb"); 
    if (!file) return;
    char end_flag = 0; // флаг конца файла
    while (1)
    {
        char buf[512];
        int bytes_read = fread(buf, 1, 512, file); // чтение блока данных
        err = send(bluetooth_socket, buf, bytes_read, 0); // отправка блока
        err = WSAGetLastError();
        if (feof(file)) end_flag = 1;
        err = send(bluetooth_socket, &end_flag, 1, 0);
        err = WSAGetLastError();
        if (end_flag == 1) break;
    }
    fclose(file);
}

string choose_file_from_dir() {
    string path = FILDIR; 
    vector<fs::directory_entry> files = vector<fs::directory_entry>();
    for (const auto& entry : fs::directory_iterator(path)) // заполнение запис€ми
        files.push_back(entry);
    for (int i = 0; i < files.size(); i++) {
        cout << i + 1 << ". " << files[i].path().u8string() << '\n';
    }
    cout << "Choose file: ";
    string fname = "";
    int f_ch{};
    cin >> f_ch;
    if (f_ch > files.size())
        return "";
    fname = files[f_ch - 1].path().u8string();
    return fname;
}


void recieve_procc(SOCKET client) {
    recv_thread_is_over = 0;
    end_of_connection = false;
    thread_is = true;
    cout << "\nMessage from server\n";
    int bytes_read = 0;
    char end_flag = 0;
    while (!end_of_connection) {
        int command = 0;
        bytes_read = recv(client, (char*)&command, 4, 0);
        if (command == 1) { 
            char name[512];
            int len{};
            recv(client, (char*)&len, 4, 0);
            bytes_read = recv(client, name, len, 0);
            name[len] = '\0';
            FILE* file;
            fopen_s(&file, name, "wb");
            if (!file) continue;
            while (1) {

                char buf[512];
                bytes_read = recv(client, buf, 512, 0);

                fwrite(buf, 1, bytes_read, file);
                bytes_read = recv(client, &end_flag, 1, 0);

                if (end_flag)
                {
                    cout << "File accepted\n" << "Check " << FILDIR << endl;
                    break;
                }
            }
            fclose(file);
        }
        if (command == 2)
        {
            cout << "Server has kicked you\n";
            closesocket(client);
            end_of_connection = true;
            socket_connected = false;
            break;
        }
    }
    cout << "\nThread end\n";
}


void play_audio(string file_name)
{
    wstring filePath(file_name.begin(), file_name.end());
    // открытие аудио с использованием media control interface
    if (mciSendString((L"open \"" + filePath + L"\" type mpegvideo alias mp3").c_str(), NULL, 0, NULL) != 0) {
        cerr << "\nfile open error\n";
        return;
    }
    // воспроизведение
    if (mciSendString(L"play mp3", NULL, 0, NULL) != 0) {
        cerr << "play error\n";
        mciSendString(L"close mp3", NULL, 0, NULL);
        return;
    }
    printf("Now playing...\nTo stop - ");
    system("pause");
    if (mciSendString(L"close mp3", NULL, 0, NULL) != 0) {
        cerr << "close error\n";
        return;
    }
}

void record_file(string fname) {
    int mci_error;
    mci_error = mciSendStringA("open new type waveaudio alias wavee", NULL, 0, NULL);
    mci_error = mciSendStringA("set wavee time format ms", NULL, 0, NULL);
    mci_error = mciSendStringA("record wavee notify", NULL, 0, NULL);
    cout << "\nRecording has been started\nTo stop record - ";
    system("pause");
    mci_error = mciSendStringA("stop wavee", NULL, 0, NULL);
    mci_error = mciSendStringA(("save wavee " + fname).c_str(), NULL, 0, NULL);
    mci_error = mciSendStringA("close wavee", NULL, 0, NULL);
    cout << "Recording has been ended\nFile is saved as " << fname << endl << endl;
}

INT make_photo(string photo_n) {
    VideoCapture cap(0, CAP_DSHOW);
    if (!cap.isOpened()) {
        cerr << "open error" << endl;
        return -1;
    }
    Mat frame;
    cap >> frame; 
    if (frame.empty()) {
        cerr << "empty frame" << endl;
        return -1;
    }
    imwrite(photo_n, frame);
    cap.release();
    return 0;
}


INT show_png(string photo_n) {
    Mat image = imread(photo_n);
    if (image.empty()) {
        cout << "Can't load image" << endl;
        return -1;
    }
    namedWindow("Image", WINDOW_NORMAL);
    imshow("Image", image);
    waitKey(0);
    destroyAllWindows();
    return 0;
}