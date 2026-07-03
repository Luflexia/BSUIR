// sysinfo.h
#ifndef SYSINFO_H
#define SYSINFO_H

typedef struct {
	int total_processes; // общее количество процессов
	int running_processes; // количество активных процессов
	int total_threads; // общее количество потоков
	int num_users; // количество пользователей
	float load_avg[3]; // средняя нагрузка на систему за последние 1, 5 и 15 минут
	float cpu_usage_user; // использование ЦП пользователем
	float cpu_usage_system; // использование ЦП системой
} SysInfo;

#endif // SYSINFO_H