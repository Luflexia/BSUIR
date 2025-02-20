#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <Winsock2.h>
#include <Ws2bth.h>
#include <BluetoothAPIs.h>
#include <filesystem>
#include <algorithm>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Bthprops.lib")

using namespace std;
namespace fs = std::filesystem;

vector<BLUETOOTH_DEVICE_INFO> devices;
vector<pair<string, string>> file_names;
SOCKET clientSocket;

enum FileType { UNKNOWN, MP3, PNG, MP4, TXT };

void findBluetoothDevices() {
    devices.clear();
    BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams = { sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS) };
    searchParams.fReturnAuthenticated = TRUE;
    searchParams.fReturnConnected = TRUE;
    searchParams.fReturnRemembered = TRUE;
    searchParams.fReturnUnknown = TRUE;
    searchParams.cTimeoutMultiplier = 8;

    BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO), 0 };
    HBLUETOOTH_DEVICE_FIND hFind = BluetoothFindFirstDevice(&searchParams, &deviceInfo);

    if (hFind == NULL) {
        cout << "���������� �� �������. ���������, ��� Bluetooth �������." << endl;
        return;
    }

    do {
        devices.push_back(deviceInfo);
    } while (BluetoothFindNextDevice(hFind, &deviceInfo));

    BluetoothFindDeviceClose(hFind);
}

void printDevices() {
    findBluetoothDevices();
    for (size_t i = 0; i < devices.size(); i++) {
        wcout << i << ". " << devices[i].szName << endl;
    }
}

void printFiles() {
    file_names.clear();
    wstring path = L".\\files";
    for (const auto& entry : fs::directory_iterator(path)) {
        file_names.emplace_back(entry.path().filename().string(), entry.path().string());
    }
    for (size_t i = 0; i < file_names.size(); i++) {
        cout << i << ". " << file_names[i].first << endl;
    }
}

int sendFile() {
    while (true) {
        printFiles();
        cout << "�������� ���� ��� �������� (��� -1 ��� ������): ";
        int pickFile;
        cin >> pickFile;
        if (pickFile == -1) break;

        string pathToFile = file_names[pickFile].second;
        string fileName = file_names[pickFile].first;

        ifstream fileToSend(pathToFile, ios::binary);
        if (!fileToSend) {
            cerr << "�� ������� ������� ����: " << pathToFile << endl;
            continue;
        }

        // �������� ����� �����
        int fileNameLength = fileName.length();
        send(clientSocket, reinterpret_cast<char*>(&fileNameLength), sizeof(fileNameLength), 0);
        send(clientSocket, fileName.c_str(), fileNameLength, 0);

        // �������� ������� �����
        fileToSend.seekg(0, ios::end);
        int fileSize = fileToSend.tellg();
        fileToSend.seekg(0);
        send(clientSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

        // �������� ����������� �����
        const int bufferSize = 8192; // ����������� ������ ������
        char buffer[bufferSize];
        int totalSent = 0;
        while (!fileToSend.eof()) {
            fileToSend.read(buffer, bufferSize);
            int bytesRead = fileToSend.gcount();
            int bytesSent = send(clientSocket, buffer, bytesRead, 0);
            if (bytesSent == SOCKET_ERROR) {
                cerr << "������ ��� �������� �����." << endl;
                break;
            }
            totalSent += bytesSent;
            cout << "\r���������� " << totalSent << " �� " << fileSize << " ���� ("
                << (totalSent * 100 / fileSize) << "%)" << flush;
        }
        cout << endl;
        fileToSend.close();
        cout << "���� ������� ���������." << endl;
    }
    return 0;
}

// ������� ��� ������ � ��������
void initSocketToSend() {
    cout << "�������� Bluetooth ���������� ��� �����������: ";
    int choice;
    cin >> choice;
    BLUETOOTH_DEVICE_INFO deviceInfo = devices[choice];

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKADDR_BTH sockaddrBthServer = { 0 };
    sockaddrBthServer.addressFamily = AF_BTH;
    sockaddrBthServer.serviceClassId = RFCOMM_PROTOCOL_UUID;
    sockaddrBthServer.port = 6;
    sockaddrBthServer.btAddr = deviceInfo.Address.ullLong;

    clientSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    connect(clientSocket, reinterpret_cast<sockaddr*>(&sockaddrBthServer), sizeof(sockaddrBthServer));

    cout << "���������� � ����������." << endl;

    sendFile();

    closesocket(clientSocket);
    WSACleanup();
}

// ������� ������� � �������
int client() {
    while (true) {
        cout << "1. ����� ���������" << endl;
        cout << "2. ������������ � ��������� ����" << endl;
        cout << "3. �����" << endl;
        cout << "�������� ��������: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            printDevices();
            break;
        case 2:
            printDevices();
            initSocketToSend();
            break;
        case 3:
            return 0;
        default:
            cout << "�������� �����. ���������� �����." << endl;
        }
    }
}

FileType identifyFileType(const char* buffer) {
    if (buffer[0] == (char)0xFF && buffer[1] == (char)0xFB) return MP3;
    if (buffer[0] == (char)0x89 && buffer[1] == (char)0x50 && buffer[2] == (char)0x4E && buffer[3] == (char)0x47) return PNG;
    if (buffer[0] == (char)0x00 && buffer[1] == (char)0x00 && buffer[2] == (char)0x00 && buffer[3] == (char)0x18) return MP4;
    return TXT;
}

int getFile(SOCKET clientSocket) {
    // ��������� ����� �����
    int fileNameLength;
    recv(clientSocket, reinterpret_cast<char*>(&fileNameLength), sizeof(fileNameLength), 0);
    vector<char> fileNameBuffer(fileNameLength + 1);
    recv(clientSocket, fileNameBuffer.data(), fileNameLength, 0);
    string fileName(fileNameBuffer.begin(), fileNameBuffer.end() - 1);

    // ��������� ������� �����
    int fileSize;
    recv(clientSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);
    cout << "��������� �����: " << fileName << " (" << fileSize << " ����)" << endl;

    // ����������� ���� ��� ���������� �����
    string downloadPath = getenv("USERPROFILE");
    downloadPath += "\\Downloads\\";
    string filePath = downloadPath + fileName;

    // ��������� ����������� �����
    const int bufferSize = 8192; // ����������� ������ ������
    vector<char> buffer(bufferSize);
    FileType fileType = UNKNOWN;
    ofstream outputFile(filePath, ios::binary);
    int remainingBytes = fileSize;
    int totalReceived = 0;
    while (remainingBytes > 0) {
        int bytesToRead = min(bufferSize, remainingBytes);
        int bytesRead = recv(clientSocket, buffer.data(), bytesToRead, 0);
        if (bytesRead <= 0) break;
        if (fileType == UNKNOWN) {
            fileType = identifyFileType(buffer.data());
        }
        outputFile.write(buffer.data(), bytesRead);
        remainingBytes -= bytesRead;
        totalReceived += bytesRead;
        cout << "\r�������� " << totalReceived << " �� " << fileSize << " ���� ("
            << (totalReceived * 100 / fileSize) << "%)" << flush;
    }
    cout << endl;
    outputFile.close();

    // �������� �����
    string command = "start \"\" \"" + filePath + "\"";
    system(command.c_str());

    return 0;
}

int server() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKADDR_BTH sockaddrBthServer = { 0 };
    sockaddrBthServer.addressFamily = AF_BTH;
    sockaddrBthServer.serviceClassId = RFCOMM_PROTOCOL_UUID;
    sockaddrBthServer.port = 6;

    SOCKET serverSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    bind(serverSocket, reinterpret_cast<sockaddr*>(&sockaddrBthServer), sizeof(sockaddrBthServer));
    listen(serverSocket, SOMAXCONN);

    cout << "������ ����� ��������� ����������." << endl;

    while (true) {
        cout << "�������� �����������..." << endl;
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        cout << "������ ���������." << endl;

        while (getFile(clientSocket) != -1) {}

        cout << "�������� ����� ���������." << endl;
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "1. ���������" << endl;
    cout << "2. ��������" << endl;
    int choice;
    cin >> choice;

    system("cls");

    switch (choice) {
    case 1:
        client();
        break;
    case 2:
        server();
        break;
    default:
        cout << "�������� �����." << endl;
    }

    return 0;
}