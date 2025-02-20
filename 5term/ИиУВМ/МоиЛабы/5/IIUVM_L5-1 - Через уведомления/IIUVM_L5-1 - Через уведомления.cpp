#include <windows.h>
#include <setupapi.h>
#include <dbt.h>
#include <cfgmgr32.h>
#include <usbiodef.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <conio.h>
#include <functional>

std::wstring lastSafelyRemovedDeviceId;

struct Device {
    std::wstring name;
    std::wstring hardwareId;
    bool ejectable;
    DEVINST devInst;

    Device() : ejectable(false), devInst(0) {}

    void print() const {
        std::wcout << std::setw(40) << std::left << hardwareId << L" | " << name << std::endl;
    }

    [[nodiscard]] bool eject() const {
        if (ejectable) {
            return CM_Request_Device_EjectW(devInst, nullptr, nullptr, 0, 0) == CR_SUCCESS;
        }
        return false;
    }
};

std::vector<Device> devices;

void updateDeviceList() {
    devices.clear();
    HDEVINFO deviceInfo = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_USB_DEVICE, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (deviceInfo == INVALID_HANDLE_VALUE) return;

    SP_DEVINFO_DATA devInfoData;
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfo, i, &devInfoData); i++) {
        Device device;
        device.devInst = devInfoData.DevInst;
        WCHAR buffer[256];
        if (SetupDiGetDeviceRegistryPropertyW(deviceInfo, &devInfoData, SPDRP_DEVICEDESC, nullptr, reinterpret_cast<PBYTE>(buffer), sizeof(buffer), nullptr)) {
            device.name = buffer;
        }
        if (SetupDiGetDeviceRegistryPropertyW(deviceInfo, &devInfoData, SPDRP_HARDWAREID, nullptr, reinterpret_cast<PBYTE>(buffer), sizeof(buffer), nullptr)) {
            device.hardwareId = buffer;
        }
        DWORD properties;
        if (SetupDiGetDeviceRegistryPropertyW(deviceInfo, &devInfoData, SPDRP_CAPABILITIES, nullptr, reinterpret_cast<PBYTE>(&properties), sizeof(DWORD), nullptr)) {
            device.ejectable = (properties & CM_DEVCAP_REMOVABLE) != 0;
        }

        devices.push_back(device);
    }
    SetupDiDestroyDeviceInfoList(deviceInfo);
}

void printDevices() {
    system("cls");
    std::wcout << L"Список USB-устройств:\n";
    for (size_t i = 0; i < devices.size(); i++) {
        std::wcout << i + 1 << L". ";
        devices[i].print();
    }
    std::wcout << L"\nВыберите номер устройства для извлечения или 'q' для выхода.\n";
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DEVICECHANGE:
            switch (wParam) {
                case DBT_DEVICEARRIVAL: {
                    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
                    if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                        std::wcout << L"Устройство подключено\n";
                        updateDeviceList();
                        printDevices();
                    }
                    break;
                }
                case DBT_DEVICEREMOVECOMPLETE: {
                    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
                    if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                        std::wcout << L"Устройство отключено\n";
                        updateDeviceList();
                        printDevices();
                    }
                    break;
                }
                case DBT_DEVICEQUERYREMOVE:
                    std::wcout << L"Попытка безопасного извлечения...\n";
                    break;
                case DBT_DEVICEQUERYREMOVEFAILED:
                    std::wcout << L"Ошибка безопасного извлечения\n";
                    break;
                case DBT_DEVICEREMOVEPENDING:
                    std::wcout << L"Устройство готово к безопасному извлечению\n";
                    break;
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void checkForUnsafeRemoval() {
    static std::vector<Device> previousDevices;
    static std::vector<std::wstring> reportedDevices;
    std::vector<Device> currentDevices = devices;
    // Проверяем устройства, которые были удалены
    for (const auto& prevDevice : previousDevices) {
        auto it = std::find_if(currentDevices.begin(), currentDevices.end(),
                               [&prevDevice](const Device& dev) { return dev.hardwareId == prevDevice.hardwareId; });

        if (it == currentDevices.end() &&
            std::find(reportedDevices.begin(), reportedDevices.end(), prevDevice.hardwareId) == reportedDevices.end() &&
            prevDevice.hardwareId != lastSafelyRemovedDeviceId) {
            std::wcout << L"Внимание: Устройство было извлечено небезопасно: " << prevDevice.name << L"\n";
            reportedDevices.push_back(prevDevice.hardwareId);
        }
    }
    // Очистка списка сообщенных устройств, которые снова появились
    reportedDevices.erase(
            std::remove_if(reportedDevices.begin(), reportedDevices.end(),
                           [&currentDevices](const std::wstring& reportedId) {
                               return std::any_of(currentDevices.begin(), currentDevices.end(),
                                                  [&reportedId](const Device& dev) { return dev.hardwareId == reportedId; });
                           }),
            reportedDevices.end()
    );
    // Обновляем список предыдущих устройств
    previousDevices = currentDevices;
    // Сбрасываем ID последнего безопасно извлеченного устройства
    lastSafelyRemovedDeviceId.clear();
}

int main() {
    setlocale(LC_ALL, "");

    WNDCLASSEXW wx = {};
    wx.cbSize = sizeof(WNDCLASSEXW);
    wx.lpfnWndProc = WndProc;
    wx.lpszClassName = L"USBMonitorClass";
    RegisterClassExW(&wx);

    HWND hWnd = CreateWindowExW(0, L"USBMonitorClass", L"USB Monitor", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

    DEV_BROADCAST_DEVICEINTERFACE notificationFilter = {};
    notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    notificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;

    HDEVNOTIFY hDevNotify = RegisterDeviceNotification(hWnd, &notificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);

    updateDeviceList();
    printDevices();

    bool running = true;
    while (running) {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q') {
                running = false;
            } else if (ch >= '1' && ch <= '9') {
                int index = ch - '1';
                if (index < devices.size()) {
                    if (devices[index].eject()) {
                        std::wcout << L"Устройство успешно извлечено\n";
                        lastSafelyRemovedDeviceId = devices[index].hardwareId;
                    } else {
                        std::wcout << L"Не удалось извлечь устройство\n";
                    }
                    Sleep(2000);  // Пауза в 2 секунды, чтобы пользователь мог прочитать сообщение
                    updateDeviceList();  // Обновляем список устройств после попытки извлечения
                    printDevices();
                }
            }
        }

        checkForUnsafeRemoval();
        Sleep(100);  // Небольшая задержка, чтобы не нагружать процессор
    }

    UnregisterDeviceNotification(hDevNotify);
    return 0;
}