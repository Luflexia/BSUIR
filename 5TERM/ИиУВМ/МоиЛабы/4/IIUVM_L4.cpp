#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>
#include <SetupAPI.h>
#include <devguid.h>
#include <string>
#include <sstream>

#pragma comment(lib, "setupapi.lib")

using namespace cv;
using namespace std;

int photoCounter = 0;   // Счётчик фотографий

// Функция для захвата и сохранения фото
void takeHiddenPhoto() {
    VideoCapture camera(0);  // Открытие камеры
    if (!camera.isOpened()) {
        cerr << "Ошибка: не удалось открыть камеру." << endl;
        return;
    }

    Mat frame;
    camera >> frame;  // Снимок с камеры

    if (!frame.empty()) {
        // Сохраняем снимок в файл
        stringstream filename;
        filename << "photo_" << ++photoCounter << ".jpg";
        imwrite(filename.str(), frame);  // Сохраняем снимок в файл
        cout << "Фото сохранено: " << filename.str() << endl;
    }
    else {
        cerr << "Ошибка: не удалось получить кадр." << endl;
    }

    camera.release();  // Закрытие камеры
}

// Обработчик для клавиатурных событий
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        auto* keyCode = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);  // Преобразуем lParam в структуру
        if (keyCode->vkCode == VK_SPACE) {  // Если нажата клавиша "Пробел"
            takeHiddenPhoto();  // Делаем фото
        }
        if (keyCode->vkCode == VK_ESCAPE) {  // Если нажата клавиша "Esc"
            PostQuitMessage(0);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void showCameraInfo() {
    SP_DEVINFO_DATA deviceInfoData{ sizeof(SP_DEVINFO_DATA) };
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_CAMERA, NULL, NULL, DIGCF_PRESENT);

    if (deviceInfoSet == INVALID_HANDLE_VALUE || !SetupDiEnumDeviceInfo(deviceInfoSet, 0, &deviceInfoData)) {
        MessageBoxW(NULL, L"Не удалось получить информацию о камере.", L"Ошибка", MB_OK | MB_ICONERROR);
        return;
    }

    wchar_t buffer[1024];
    wstringstream info;

    if (SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)buffer, sizeof(buffer), NULL))
        info << L"Имя: " << buffer << L"\n";
    if (SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_MFG, NULL, (PBYTE)buffer, sizeof(buffer), NULL))
        info << L"Производитель: " << buffer << L"\n";
    if (SetupDiGetDeviceInstanceIdW(deviceInfoSet, &deviceInfoData, buffer, sizeof(buffer), NULL)) {
        wstring deviceID(buffer);
        auto vidPos = deviceID.find(L"VID_"), pidPos = deviceID.find(L"PID_");
        if (vidPos != wstring::npos) info << L"Vendor ID: " << deviceID.substr(vidPos + 4, 4) << L"\n";
        if (pidPos != wstring::npos) info << L"Product ID: " << deviceID.substr(pidPos + 4, 4) << L"\n";
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    MessageBoxW(NULL, info.str().c_str(), L"Информация о камере", MB_OK | MB_ICONINFORMATION);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    setlocale(LC_ALL, "Russian");

    showCameraInfo();

    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}
