#include "updateProcess.h"
#include "readProcess.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Функция для обновления данных о процессах
void update_process_data(Process *processes, int num_processes) {
    // Используем функцию read_processes() для чтения информации о процессах
    int updated_processes = read_processes(processes);
    if (updated_processes < 0) {
        perror("Ошибка при чтении информации о процессах");
        return;
    }

    // Если количество прочитанных процессов больше, чем необходимо, обрабатываем только num_processes процессов
    if (updated_processes > num_processes) {
        updated_processes = num_processes;
    }
}