#include <iostream>
#include <winsock2.h>
#include <ws2bth.h>
#include <vector>
#include <thread>
#include <filesystem>
#include <mmsystem.h>

#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/videoio.hpp"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

#define ALIAS "random_str"
#define FILESDIR "D:\\files\\"

using namespace cv;
using std::endl, std::cout, std::thread, std::string, std::cerr, std::cin, std::vector;
namespace fs = std::filesystem;

static const GUID MY_SERVICE = { 0x6afcbbcb, 0x995a, 0x425d, { 0xa0, 0x95, 0x9f, 0x6e, 0xac, 0x4f, 0x8e, 0x74 } };

void play_audio(std::string file_name) {
    // использование media control interface
    if (mciSendStringA(("open " + file_name + " type mpegvideo alias mp3").c_str(), NULL, 0, NULL) != 0) {
        cerr << "\nError in open file\n";
        return;
    }
    if (mciSendString(L"play mp3", NULL, 0, NULL) != 0) {
        cerr << "Play error\n";
        mciSendString(L"close mp3", NULL, 0, NULL);
        return;
    }
    printf("\nPress key to stop playing\n");
    system("pause");
    if (mciSendString(L"close mp3", NULL, 0, NULL) != 0) {
        cerr << "Close error\n";
        return;
    }
}

std::string choose_file_from_dir() {
    string path = FILESDIR;
    vector<fs::directory_entry> files = vector<fs::directory_entry>();
    for (const auto& entry : fs::directory_iterator(path)) // заполнение записей
        files.push_back(entry);
    for (int i = 0; i < files.size(); i++)
    {
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

string get_current_time() { // получение текущего времени
    time_t now = time(0);
    tm* ltm = new tm;
    localtime_s(ltm, &now);
    std::stringstream sstream;
    sstream << ltm->tm_hour << '_' << ltm->tm_min << '_' << ltm->tm_sec << '_'
        << ltm->tm_year + 1900 << '_' << ltm->tm_mon + 1 << '_' << ltm->tm_mday;
    return sstream.str();
}

string make_photo() { // создание фото
    VideoCapture myVid(0);
    string filename = FILESDIR + string("\\") + get_current_time() + ".png";
    Mat frame;
    myVid.read(frame);
    imwrite(filename, frame);
    myVid.release();
    return filename;
}

int end_of_prog = 0;
int end_of_connection = 0;
SOCKET btSocket;

void init_sock_addr(SOCKADDR_BTH* sockaddr) // инициализация структуры для представления сетевого адреса
{
    ZeroMemory(sockaddr, sizeof(SOCKADDR_BTH));
    sockaddr->addressFamily = AF_BTH;
    sockaddr->serviceClassId = RFCOMM_PROTOCOL_UUID;
    sockaddr->port = 27;
    sockaddr->btAddr = 0;
}

void init_query_set(WSAQUERYSET* set, WCHAR* name_of_server, GUID* guid) // функция инициализации структуры для запроса
{
    ZeroMemory(set, sizeof(WSAQUERYSET));
    set->dwSize = sizeof(WSAQUERYSET);
    set->lpszServiceInstanceName = name_of_server;
    set->lpServiceClassId = guid;
    set->dwNameSpace = NS_BTH;
    set->dwNumberOfCsAddrs = 1;
}

void init_csaddr(CSADDR_INFO* csAddrInfo) // инициализация структуры для представления информации об адресе
{
    memset(csAddrInfo, 0, sizeof(CSADDR_INFO));
    csAddrInfo->LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
    csAddrInfo->iSocketType = SOCK_STREAM;
    csAddrInfo->iProtocol = BTHPROTO_RFCOMM;
}

void recv_cycle(SOCKET client) // функция циклического чтения данных от клиента
{
    end_of_connection = 0; // отследивание окончания соединения
    std::cout << "Client accepted" << endl;
    int bytes_read = 0;
    char end_flag = 0;
    while (!end_of_connection)
    {
        int command = 0;
        // чтение команды от клиента
        bytes_read = recv(client, (char*)&command, 4, 0);
        if (command == 1)
        {
            char name[512];
            int len{};
            recv(client, (char*)&len, 4, 0);
            bytes_read = recv(client, name, len, 0);
            name[len] = '\0';
            FILE* file;
            fopen_s(&file, name, "wb");
            if (!file) continue;
            while (1)
            {
                char buf[512];
                // чтение данных
                bytes_read = recv(client, buf, 512, 0);
                // запись в файл
                fwrite(buf, 1, bytes_read, file);
                bytes_read = recv(client, &end_flag, 1, 0);

                if (end_flag)
                {
                    cout << "End of file\n";
                    break;
                }
            }
            fclose(file);
        }
        if (command == 2)
        {
            end_of_connection = 1;
            closesocket(client);
            break;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "rus");

    WORD wVersionRequested;
    WSADATA wsaData;
    int error;
    wVersionRequested = MAKEWORD(2, 2);

    error = WSAStartup(wVersionRequested, &wsaData); // инициализация winsock
    if (error != 0) {
        printf("Error WSA init\n");
        return 1;
    }

    std::thread my_thread{};
    SOCKADDR_BTH sockAddr;
    init_sock_addr(&sockAddr); // инициализация структуры sockaddr_btn для представления локального адреса
    
    WCHAR name_of_server[] = L"Server";
    GUID guid = MY_SERVICE;
    WSAQUERYSETW query_set;
    init_query_set(&query_set, name_of_server, &guid); // инициализация переменных, связанных с параметрами запроса службы
    
    CSADDR_INFO csAddrInfo;
    init_csaddr(&csAddrInfo); // инициализация структуры адреса
    // создание сокета
    btSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if (btSocket == SOCKET_ERROR) {
        cout << "SOCKET ERROR" << endl;
        return 1;
    }

    sockaddr* addr = (sockaddr*)&sockAddr;
    error = bind(btSocket, addr, sizeof(sockAddr)); // привязка сокета к адресу
    if (error != 0) {
        cout << "Bind error" << endl;
        return 1;
    }

    int len = sizeof(sockaddr);
    getsockname(btSocket, addr, &len); // получение локального адреса сокета, чтобы узнать порт
    csAddrInfo.LocalAddr.lpSockaddr = addr;
    query_set.lpcsaBuffer = &csAddrInfo;
    error = WSASetService(&query_set, RNRSERVICE_REGISTER, 0);
    if (error != 0) {
        cout << "WSASetService error" << endl;
        return 1;
    }

    error = listen(btSocket, 2);
    if (error != 0) {
        cout << "Listen error" << endl;
        return 1;
    }

    int bytes_read = 0;
    int err = 0;
    while (!end_of_prog) {
        system("CLS");
        SOCKET client{};
        cout << "Wait for client" << endl;
        client = accept(btSocket, NULL, NULL);
        if (client == INVALID_SOCKET) break;
        my_thread = thread(recv_cycle, client);
        while (1)
        {
            system("CLS");
            cout << "\nConnection\n";
            cout << "Actions:\n\n";
            cout << "1. Send file\n";
            cout << "2. Play sound\n";
            cout << "3. Make photo\n";
            cout << "4. Disconnect\n\n";

            int ch = 0;
            cin >> ch;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000000, '\n');
            }
            // проверка, было ли завершено соединение
            if (end_of_connection == 1) {
                cout << "Connection terminated\n";
                my_thread.join();
                end_of_connection = 0;
                system("pause");
                break;
            }
            // обработка выбора пользователя
            if (ch == 1) {
                system("CLS");
                cout << "Choose file number: \n";
                string path = FILESDIR;
                vector<fs::directory_entry> files = vector<fs::directory_entry>();
                for (const auto& entry : fs::directory_iterator(path))
                    files.push_back(entry);
                for (int i = 0; i < files.size(); i++)
                {
                    cout << i + 1 << ". " << files[i].path().u8string() << '\n';
                }
                cout << files.size() + 1 << ". Return to menu" << "\n";
                string fname;
                int f_ch{};
                cin >> f_ch;
                if (f_ch == files.size() + 1) continue;
                fname = files[f_ch - 1].path().u8string();
                int com = 1;
                send(client, (char*)&com, 4, 0);
                int len = fname.length();
                send(client, (char*)&len, 4, 0);
                send(client, fname.c_str(), fname.length(), 0);
                FILE* file;
                fopen_s(&file, fname.c_str(), "rb");
                if (!file) continue;
                char end_flag = 0;
                while (1)
                {
                    char buf[512];
                    int bytes_read = fread(buf, 1, 512, file);
                    err = send(client, buf, bytes_read, 0);
                    if (feof(file)) end_flag = 1;
                    err = send(client, &end_flag, 1, 0);
                    if (end_flag == 1) break;
                }
                fclose(file);
            }
            if (ch == 2) {
                cout << "\nPlaying of audio file\n";
                string fname;
                fname = choose_file_from_dir();

                if (fname == "") {
                    cout << "\nFile not choosen\n";
                }
                else {
                    cout << "\nThis file of chosen: " << fname << endl;
                    play_audio(fname);
                }
            }
            if (ch == 3) {
                string fname = make_photo();
                cout << "Photo saved to path: " << fname << '\n';
                system("pause");
            }
            if (ch == 4) {
                int com = 2;
                send(client, (char*)&com, 4, 0);
                end_of_connection = 1;
                closesocket(client);
                my_thread.join();
                break;
            }
        }
    }
    WSACleanup();
    return 0;
}