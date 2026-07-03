#include <format>
#include <mutex>
#include <thread>
#include <iostream>
#include <Windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <opencv2\opencv.hpp>
#include <chrono>
#include <ShObjIdl.h>
using namespace cv;
using namespace std;

//#pragma comment (lib, std::setupapi.lib)
#define EXT_KEY VK_MENU
#define VIDEO 0x44
#define EXIT 0x51

HHOOK keyboardHook;
bool exitFlag = false;

// код сообщения, тип события, указатель на структуру о событии клавиши
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam; // структура о событии клавиши

        // если клавиша нажата и это Esc
        if (wParam == WM_KEYDOWN && kb->vkCode == VK_ESCAPE)
        {
            cout << "завершение видеозаписи" << endl;
            exitFlag = true; 
            return 1;  
        }
    }

    // передача управления следующему хуку в цепочке
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void camera_info()
{
    SP_DEVINFO_DATA DeviceInfoData;
    ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    HDEVINFO DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_CAMERA, L"USB", NULL, DIGCF_PRESENT);

    if (DeviceInfoSet == INVALID_HANDLE_VALUE)
        return;

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    if (!SetupDiEnumDeviceInfo(DeviceInfoSet, 0, &DeviceInfoData))
    {
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
        return;
    }

    const int PropertyBufferSize = 1024;
    wchar_t deviceID[PropertyBufferSize], deviceName[PropertyBufferSize], companyName[PropertyBufferSize];
    ZeroMemory(&deviceID, sizeof(deviceID));
    ZeroMemory(&deviceName, sizeof(deviceName));
    ZeroMemory(&companyName, sizeof(companyName));

    if (!SetupDiGetDeviceInstanceId(DeviceInfoSet, &DeviceInfoData, deviceID, sizeof(deviceID), NULL))
    {
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
        return;
    }

    if (!SetupDiGetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)deviceName, sizeof(deviceName), NULL))
    {
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
        return;
    }

    if (!SetupDiGetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_MFG, NULL, (PBYTE)companyName, sizeof(companyName), NULL))
    {
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
        return;
    }

    wstring venAndDevId(deviceID);

    wstring info = std::format(L"Name: {}\nManufacture: {}\nDevice ID: {}\nVendor ID: {}", deviceName, companyName, venAndDevId.substr(8, 4), venAndDevId.substr(17, 4));

    SetupDiDestroyDeviceInfoList(DeviceInfoSet);

    MessageBox(NULL, info.c_str(), L"WEBCAM", MB_OK);
}

int main()
{
    setlocale(LC_ALL, "RU");

    // установка хука - тип, функция, дескриптор приложения, идентификатор потока
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);

    // вывод информации об установленной веб-камере
    camera_info();
    VideoCapture cap(0);/*
    cout << "веб-камера " << 1 << ": ";
    cout << "ширина = " << cap.get(CAP_PROP_FRAME_WIDTH) << ", ";
    cout << "высота = " << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;*/

    Sleep(3000);
    // скрытие окна с изображением
    HWND hwnd = GetConsoleWindow();
    if (hwnd != NULL)
    {
        ShowWindow(hwnd, SW_HIDE);
    }

    if (!cap.isOpened())
    {
        cerr << "ошибка при открытии камеры." << endl;
        return -1;
    }

    // фото
    Mat frame;
    cap >> frame;
    if (frame.empty())
    {
        cerr << "пустой кадр." << endl;
    }
    imwrite("my_image.jpg", frame);

    // видео
    // сжатие - Motion JPEG, 10 кадров в секунду
    VideoWriter video("my_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)));
    do
    {
        Mat frame;
        cap >> frame;
        if (frame.empty())
        {
            cerr << "пустой кадр." << endl;
            break;
        }
        video.write(frame);

        // скрытие приложения из панели задач
        HWND hwndConsole = GetConsoleWindow();
        if (hwndConsole != NULL)
        {
            // дескриптор окна, индекс изменяемого атрибута (стиль), 
            // получение текущего атрибута и добавление нужного атрибута
            SetWindowLong(hwndConsole, GWL_EXSTYLE,
                GetWindowLong(hwndConsole, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
        }

        // Проверка флага завершения программы
        if (exitFlag)
        {
            break;
        }

    } while (1);

    ShowWindow(hwnd, SW_SHOWNORMAL);
    destroyAllWindows();
    video.release();

    UnhookWindowsHookEx(keyboardHook);

    return 0;
}