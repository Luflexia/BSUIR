#include <iostream>
#include <Windows.h>
#include <powrprof.h>
#include <conio.h>

#include <winbase.h>
#include <winioctl.h>
#include <batclass.h>

#include <Poclass.h>
#include <Setupapi.h>
#include <devguid.h>

#pragma comment(lib, "powrprof.lib")
#pragma comment(lib, "setupapi.lib")

using namespace std;

class AccumulatorInfo {
public:
	// тип энергопитания
	static void printPowerType() {
		SYSTEM_POWER_STATUS powerStatus;
		if (!GetSystemPowerStatus(&powerStatus)) {
			cout << "Не удалось получить информаию о состоянии энергопитания" << endl;
			return;
		}

		switch (powerStatus.ACLineStatus) {
		case 0:
			cout << "Питание от батареи" << endl;
			break;
		case 1:
			cout << "Питание от сети" << endl;
			break;
		default:
			cout << "Неизвестный тип питания" << endl;
			break;
		}
	}

	// тип батареи
	static void printBatteryType() {
		// получить дескриптор для набора сведений об устройстве
		HDEVINFO DeviceInfoSet;			
		DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

		// интерфейс устройства в наборе сведений об устройстве
		SP_DEVICE_INTERFACE_DATA DeviceInterfaceData = { 0 };
		ZeroMemory(&DeviceInterfaceData, sizeof(SP_DEVINFO_DATA));
		DeviceInterfaceData.cbSize = sizeof(SP_DEVINFO_DATA);			// размер структуры с байтах		

		// элемент информации о устройстве в наборе информации об устройстве(набор инфы об устройстве, индекс элемента, идентификатор батареи, интерфейс устройства)
		SetupDiEnumDeviceInterfaces(DeviceInfoSet, NULL, &GUID_DEVCLASS_BATTERY, 0, &DeviceInterfaceData);
		DWORD cbRequired = 0;											// в эту переменную будет получен нужный размер буфера

		// получить требуемый размер буфера
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, NULL, NULL, &cbRequired, NULL);
		
		// выделение памяи для переменной пути к интерфейсу устройства
		PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, cbRequired);
		pdidd->cbSize = sizeof(*pdidd);									// размер структуры в байтах

		// получить требуемый размер буфера
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, pdidd, cbRequired, &cbRequired, NULL);
		
		// дескриптор файла устройства
		HANDLE batteryFile = CreateFile(pdidd->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// структура с информацией о запросе батареи
		BATTERY_QUERY_INFORMATION BatteryQueryInformation = { 0 };
		DWORD bytesWait = 0;											// входной буфер
		DWORD bytesReturned = 0;										// выходной буфер

		// команда файлу устройства извлечь текущую метку батареи
		DeviceIoControl(batteryFile, IOCTL_BATTERY_QUERY_TAG, &bytesWait, sizeof(bytesWait), &BatteryQueryInformation.BatteryTag,
			sizeof(BatteryQueryInformation.BatteryTag), &bytesReturned, NULL) && BatteryQueryInformation.BatteryTag;

		// информация о батарее
		BATTERY_INFORMATION BatteryInfo = { 0 };
		BatteryQueryInformation.InformationLevel = BatteryInformation;

		// команда файлу устройства извлечь информацию об аккумуляторе
		DeviceIoControl(batteryFile, IOCTL_BATTERY_QUERY_INFORMATION, &BatteryQueryInformation, sizeof(BatteryQueryInformation),
			&BatteryInfo, sizeof(BatteryInfo), &bytesReturned, NULL);

		// вывод состава батареи
		cout << "Тип батареи:";
		for (int i = 0; i < 4; i++){
			cout << BatteryInfo.Chemistry[i];
		};
		cout << endl;

		// освобождение памяти
		LocalFree(pdidd);
		SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	}

	// уровень заряда
	static void printBateryLevel() {
		SYSTEM_POWER_STATUS powerStatus;
		if (!GetSystemPowerStatus(&powerStatus)) {
			cout << "Не удалось получить информаию о состоянии энергопитания" << endl;
			return;
		}

		cout << "Заряд батареи: " << (int)powerStatus.BatteryLifePercent << "%" << endl;
	}

	// текущий режим энергосбережения
	static void printPowerSavingMode() {
		SYSTEM_POWER_STATUS powerStatus;
		if (!GetSystemPowerStatus(&powerStatus)) {
			cout << "Не удалось получить информаию о состоянии энергопитания" << endl;
			return;
		}

		switch (powerStatus.SystemStatusFlag) {
		case 0:
			cout << "Режим экономии заряда батареи отключен" << endl;
			break;
		case 1:
			cout << "Включен режим энергосбережения" << endl;
			break;
		case 2: 
			cout << "Только экран в режиме энергосбережения" << endl;
			break;
		case 4:
			cout << "Режим энергосбережения включен" << endl;
			break;
		default:
			cout << "Неподдерживаемый режим" << endl;
			break;
		}
	}

	// время работы аккумулятора без зарядки
	static void printFullTime() {
		SYSTEM_POWER_STATUS powerStatus;
		if (!GetSystemPowerStatus(&powerStatus)) {
			cout << "Не удалось получить информаию о состоянии энергопитания" << endl;
			return;
		}

		if (powerStatus.BatteryLifePercent != 255) {
			int seconds = powerStatus.BatteryFullLifeTime;
			if (seconds == -1) {
				if (powerStatus.ACLineStatus == 1) {
					cout << "Аккумулятор включен в сеть" << endl;
				}
				else cout << "Время работы аккумулятора без зарядки неизввестно" << endl;
				return;
			}
			int hours = seconds / 3600;
			int minutes = (seconds % 3600) / 60;
			seconds = (seconds % 3600) % 60;
			cout << "Время работы аккумулятора без зарядки: " << hours << " часов " << minutes << " минут " << seconds << " секунд" << endl;
		}
		else cout << "Статус заряда батареи неизвестен" << endl;
	}

	// оставшееся время работы 
	static void printTime() {
		SYSTEM_POWER_STATUS powerStatus;
		if (!GetSystemPowerStatus(&powerStatus)) {
			cout << "Не удалось получить информаию о состоянии энергопитания" << endl;
			return;
		}

		// проверка на включенность аккумулятора в сеть
		if (powerStatus.ACLineStatus == 1) {
			cout << "Аккумулятор включен в сеть" << endl;
			return;
		}

		// известен ли статус заряда батареи
		if (powerStatus.BatteryLifePercent != 255) {
			int seconds = powerStatus.BatteryLifeTime;

			// проверка на подключенность к сети
			if (seconds == -1) {
				cout << "Извлеките устройство из сети и подождите" << endl;
				return;
			}
			int hours = seconds / 3600;
			int minutes = (seconds % 3600) / 60;
			seconds = (seconds % 3600) % 60;
			cout << "Время работы аккумулятора без зарядки: " << hours << " часов " << minutes << " минут " << seconds << " секунд" << endl;
		}
		else cout << "Статус заряда батареи неизвестен" << endl;
	}
};

const char* menu[] = {
	"1 - Тип энергопитания",
	"2 - Тип батареи",
	"3 - Уровень заряда",
	"4 - Текущий режим энергосбережения",
	"5 - Оставшееся время работы аккумулятора без зарядки",
	"6 - Время работа аккумулятора без зарядки",
	"7 - Спящий режим",
	"8 - Гибернация", 
	"q - Выход",
	NULL
};

void printMenu() {
	for (int i = 0; menu[i] != NULL; i++) {
		cout << menu[i] << endl;
	}
	cout << endl << endl;
}

int main(){
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	printMenu();

	while (1) {
		char choise = _getch();
		system("cls");
		printMenu();
		switch (choise) {
			case '1':	
				AccumulatorInfo::printPowerType();
				break;
			case '2':
				AccumulatorInfo::printBatteryType();
				break;
			case '3':
				AccumulatorInfo::printBateryLevel();
				break;
			case '4':
				AccumulatorInfo::printPowerSavingMode();
				break;
			case '5':
				AccumulatorInfo::printTime();
				break;
			case '6':
				AccumulatorInfo::printFullTime();
				break;
			case '7':
				if (SetSuspendState(FALSE, FALSE, FALSE) == 0) {			// переход в спящий режим
					cout << "Ошибка при переходе в спящий режим" << endl;
				}
				break;
			case '8':
				if (SetSuspendState(TRUE, FALSE, FALSE) == 0) {				// переход в режим гибернации
					cout << "Ошибка при гибернации" << endl;
				}
				break;
			case 'q':
				return 0;
			default:
				break;
		}
	}
}

