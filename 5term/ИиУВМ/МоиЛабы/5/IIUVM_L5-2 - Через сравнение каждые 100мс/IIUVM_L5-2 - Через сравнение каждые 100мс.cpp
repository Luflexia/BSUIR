#include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <usbiodef.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <conio.h>
#include <chrono>
#include <thread>
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

    bool running = true;

    while (running) {
        updateDeviceList();
        printDevices();
        checkForUnsafeRemoval();

        // Ожидание ввода пользователя в течение 1 секунды
        for (int i = 0; i < 10; ++i) {
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 'q' || ch == 'Q') {
                    running = false;
                    break;
                } else if (ch >= '1' && ch <= '9') {
                    int index = ch - '1';
                    if (index < devices.size()) {
                        if (devices[index].eject()) {
                            std::wcout << L"Устройство успешно извлечено\n";
                            lastSafelyRemovedDeviceId = devices[index].hardwareId;
                        } else {
                            std::wcout << L"Не удалось извлечь устройство\n";
                        }
                        std::wcout << L"Нажмите Enter для продолжения...";
                        while (_getch() != '\r'); // Ожидание нажатия Enter
                        break; // Выход из цикла ожидания для немедленного обновления списка
                    }
                }
            }
            Sleep(100); // Короткая задержка между проверками ввода
        }
    }

    return 0;
}