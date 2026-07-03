#pragma comment(lib, "setupapi.lib")
#include <Windows.h>
#include <Setupapi.h>
#include <iostream>
#include <Cfgmgr32.h>
#include <vector>
#include <string>
#include <iomanip>
#include <dbt.h>
#include <initguid.h>
#include <Usbiodef.h>
#include <conio.h>
#include <initguid.h>
#include <Usbiodef.h>
using namespace std;
using std::vector;

HANDLE thread;

class Device {
private:
	std::wstring HARDWARE_ID;
	std::wstring name;
	std::wstring device_path;
	std::wstring pid;
	bool ejectable;
	DEVINST devInst;

	HDEVNOTIFY notification_handle;
	
public:
	Device(PDEV_BROADCAST_DEVICEINTERFACE info, HWND hWnd = nullptr) {
		HDEVINFO deviceList = SetupDiCreateDeviceInfoList(nullptr, nullptr);			// пустой набор инфы об устройстве
		SetupDiOpenDeviceInterfaceW(deviceList, (LPCWSTR)info->dbcc_name, NULL, NULL);
		SP_DEVINFO_DATA deviceInfo;														// экземпляр устройства в наборе информации
		ZeroMemory(&deviceInfo, sizeof(SP_DEVINFO_DATA));
		deviceInfo.cbSize = sizeof(SP_DEVINFO_DATA);
		SetupDiEnumDeviceInfo(deviceList, 0, &deviceInfo);								// получение инфы об устройстве
		*this = Device(deviceList, deviceInfo, hWnd);
	}

	Device(HDEVINFO deviceList, SP_DEVINFO_DATA deviceInfo, HWND hWnd) {
		this->devInst = deviceInfo.DevInst;								// дескриптор устройства
		TCHAR buffer[1024];
		ZeroMemory(buffer, sizeof(buffer));
		SetupDiGetDeviceRegistryProperty(deviceList, &deviceInfo, SPDRP_DEVICEDESC, NULL, (BYTE*)buffer, 1024, NULL);
		this->name = std::wstring(buffer);
		ZeroMemory(buffer, sizeof(buffer));
		SetupDiGetDeviceRegistryProperty(deviceList, &deviceInfo, SPDRP_HARDWAREID, nullptr, (BYTE*)buffer, 1024, nullptr);
		this->HARDWARE_ID = std::wstring(buffer);
		if (!this->HARDWARE_ID.empty() && (HARDWARE_ID.find(L"PID_") != -1))
			this->pid = HARDWARE_ID.substr(HARDWARE_ID.find(L"PID_") + 4, 4);

		// можно ли удалить устройство
		DWORD properties;
		SetupDiGetDeviceRegistryPropertyA(deviceList, &deviceInfo, SPDRP_CAPABILITIES, NULL, (PBYTE)&properties, sizeof(DWORD), NULL);
		this->ejectable = properties & CM_DEVCAP_REMOVABLE;

		if (hWnd != nullptr && this->ejectable) {
			SP_DEVICE_INTERFACE_DATA devInterfaceData;						// сведение об интерфейсах устройства
			SP_DEVICE_INTERFACE_DETAIL_DATA* devInterfaceDetailData;		// детальные сведения об интерфейсах устройства
			devInterfaceData.cbSize = sizeof(devInterfaceData);
			// получение информации об устройстве в буфер devInterfaceData
			SetupDiEnumDeviceInterfaces(deviceList, &deviceInfo, &GUID_DEVINTERFACE_USB_DEVICE, 0, &devInterfaceData);

			// получить детальные сведение об интерфейсе устройства, а из них путь к устройству
			DWORD requiredLength;
			SetupDiGetDeviceInterfaceDetail(deviceList, &devInterfaceData, NULL, 0, &requiredLength, NULL);
			devInterfaceDetailData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(requiredLength);
			devInterfaceDetailData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
			SetupDiGetDeviceInterfaceDetail(deviceList, &devInterfaceData, devInterfaceDetailData, requiredLength, NULL, &deviceInfo);

			device_path = std::wstring(devInterfaceDetailData->DevicePath);

			this->register_handle(hWnd);
		}
	}

	Device(const Device& that) {
		this->HARDWARE_ID = that.HARDWARE_ID;
		this->name = that.name;
		this->pid = that.pid;
		this->ejectable = that.ejectable;
		this->devInst = that.devInst;
		this->device_path = that.device_path;
		this->notification_handle = that.notification_handle;
	}

	void register_handle(HWND hWnd) {
		DEV_BROADCAST_HANDLE filter = { 0 };			// информация о дескрипторе файловой системы
		// создать файл устройства
		HANDLE deviceHandle = CreateFile(device_path.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		filter.dbch_size = sizeof(filter);
		filter.dbch_devicetype = DBT_DEVTYP_HANDLE;
		filter.dbch_handle = deviceHandle;
		// зарегистрировать устройство, о котором будет получена информация
		this->notification_handle = RegisterDeviceNotification(hWnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);
		CloseHandle(deviceHandle);						// закрыть файл устройства
	}
	void unregister_handle() const {
		UnregisterDeviceNotification(notification_handle);
	}

	bool operator ==(const Device& other) const {
		return this->pid == other.pid;
	}

	bool is_ejectable() const {
		return this->ejectable;
	}

	bool eject() const {
		if (this->ejectable) {
			CM_Request_Device_EjectW(this->devInst, nullptr, nullptr, NULL, NULL);
			return this->ejectable;
		}
		//else return false;
	}

	void print() const {
		std::wcout << setw(40) << left << HARDWARE_ID << " | " << std::endl;
	}

	std::wstring get_name() const {
		return name;
	}
};

class DeviceList {
public:
	vector<Device> devices;

	void add(Device device) {
		devices.push_back(device);
	}

	void remove(const Device& device) {
		for (size_t i = 0; i < devices.size(); i++) {
			if (device == devices[i]) {
				devices[i].unregister_handle();
				devices.erase(devices.begin() + i);
				break;
			}
		}
	}

	void print_devices() {
		if (devices.size() == 0) {
			std::cout << "\t\t\t\t" << "No devices" << std::endl;
			return;
		}
		std::cout << "n   | " << setw(40) << left << "Hardware id" << " | "
			<< std::endl;
		for (int i = 0; i < devices.size(); i++) {
			std::cout << setw(3) << left << i + 1 << " | ";
			devices[i].print();
		}
	}

	Device find_by_index(int index) {
		return devices[index];
	}

	int count() {
		return devices.size();
	}

	void clear() {
		devices.clear();
	}
};

DeviceList device_list;


void exit_handler() {
	CloseHandle(thread);												
	device_list.clear();												
	exit(-1);
}


// обработчик событий устройств
LRESULT FAR PASCAL WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_DEVICECHANGE) {					// произошло измененние аппаратной конфигурации компа
		switch (wParam) {
		case DBT_DEVICEARRIVAL:								// добавлено устройство
		{	// получить класс информации об устройстве
			PDEV_BROADCAST_DEVICEINTERFACE checkGUID = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
			if (checkGUID->dbcc_classguid != GUID_DEVINTERFACE_USB_DEVICE)	// ус-во не подключено  USB-концентрантору
				break;

			// создание устройства и добавление его в список устройств
			Device device((PDEV_BROADCAST_DEVICEINTERFACE)lParam, hWnd);
			if (device.get_name().empty()) break;
			device_list.add(device);

			// вывод информации о добавленном устройстве
			system("CLS");
			device_list.print_devices();
			break;
		}
		case DBT_DEVICEREMOVECOMPLETE:						// устройство удалено
		{
			// создать устройство и удалить его из списка устройств
			Device device((PDEV_BROADCAST_DEVICEINTERFACE)lParam);
			if (device.get_name().empty())
				break;
			device_list.remove(device);

			system("CLS");
			device.print();
			std::cout << " disconnected" << std::endl;

			device_list.print_devices();
			break;
		}
		case DBT_DEVICEQUERYREMOVE:							// запрос на извлечение
		{
			system("CLS");
			std::cout << "Tring to remove safely..." << std::endl;;
			device_list.print_devices();
			break;
		}
		case DBT_DEVICEQUERYREMOVEFAILED:					// запрос на извлечение отклонен
		{
			system("CLS");
			std::cout << "Failed to remove safely" << std::endl;
			device_list.print_devices();
			break;
		}
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


DWORD WINAPI thread_start(void*) {
	WNDCLASSEXW wx;									// атрибуты класса окна
	ZeroMemory(&wx, sizeof(wx));					
	wx.cbSize = sizeof(WNDCLASSEX);					
	wx.lpfnWndProc = (WNDPROC)WndProc;				
	wx.lpszClassName = L"NONE";						// имя класса окна


	HWND hWnd = NULL;								// окно
	GUID guid = GUID_DEVINTERFACE_USB_DEVICE;		// класс интерфейса устройства
	if (RegisterClassExW(&wx))						// зарегистрировать окно для дальнейших действий
		// создать изначально свернутое окно с размерами по умолчанию (с окном связан дескриптор модуля) 
		hWnd = CreateWindowA("NONE", "DevNotifWnd", WS_ICONIC, 0, 0, CW_USEDEFAULT, 0, 0, NULL, GetModuleHandle(nullptr), NULL);


	DEV_BROADCAST_DEVICEINTERFACE_A filter;			// структура, содержащая инфу о классе устройств
	filter.dbcc_size = sizeof(filter);				 
	filter.dbcc_classguid = guid;
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;	// так надо
	// окно будет получать уведомления от структуры,содержащей инфу оклассе устройств
	RegisterDeviceNotificationA(hWnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);


	SP_DEVINFO_DATA devInfoData;					// экземпляр устройства (член набора)
	// получить дескриптор набора сведений об устройстве												 (устройства, которые сейчас в системе)
	const HDEVINFO deviceInfoSet = SetupDiGetClassDevsA(&GUID_DEVINTERFACE_USB_DEVICE, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (deviceInfoSet == INVALID_HANDLE_VALUE) {	// не получилось получить инфу об устройстве - выход
		std::cout << "Cannot retrieve device information set" << std::endl;
		exit_handler();
	}


	// получить все доступные устройства
	for (int i = 0; ; i++) {
		devInfoData.cbSize = sizeof(devInfoData);
		// получить инфу об устройстве
		if (SetupDiEnumDeviceInfo(deviceInfoSet, i, &devInfoData) == FALSE)
			break;

		// создать объект экземпляра устройства, добавить в вектор 
		Device device(deviceInfoSet, devInfoData, hWnd);
		if (device.is_ejectable()) {
			device_list.add(device);
			std::cout << std::endl;
		}
	}
	SetupDiDestroyDeviceInfoList(deviceInfoSet);			// удалить структуру с набором инфы об устройстве


	MSG msg;
	while (true) {											// обработать сообщения 
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {			// получить сообщение для окна из очереди
			TranslateMessage(&msg);									// преобразовать в символьное сообщение
			DispatchMessage(&msg);									// отправить сообщение оконной процедуре
		}
	}
	return 0;
}



int main() {
	thread = CreateThread(nullptr, 0, thread_start, nullptr, 0, nullptr);
	if (thread == nullptr) {
		std::cout << "Cannot create thread." << std::endl;
		return GetLastError();
	}

	while (true) {
		system("cls");
		device_list.print_devices();
		rewind(stdin);
		char ch = _getch();

		if (ch >= '1' && ch <= '9') {
			if (ch - '0' <= device_list.count()) {
				Device device = device_list.find_by_index(ch - '0' - 1);

				if (!device.eject()) std::cout << "\n\n\n\n\nDevice " << ch << " isn't removable." << std::endl;
				Sleep(100);
			}
		}
		else if (ch == 'e') exit_handler();
	}
	return 0;
}