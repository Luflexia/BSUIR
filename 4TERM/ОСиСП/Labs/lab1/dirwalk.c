#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h> 
#include <dirent.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdbool.h> 
#include <locale.h> 

void print_entry(const char* name, int type) {
    printf("%s\n", name); // Выводим имя элемента
}

int compare(const void* a, const void* b) {
    return strcoll((*(struct dirent**)a)->d_name, (*(struct dirent**)b)->d_name); // Сравниваем имена элементов для сортировки (strcoll - сравнивание строк с учетом локали)
}

void dirwalk(const char* dir, bool symbolic_links, bool directories, bool files, bool sort) {
    DIR* dp; // Указатель на DIR. Структура содержит информацию о состоянии и местоположении открытой директории
    struct dirent** entry_list; // Указатель на dirent представляет собой структуру данных, которая содержит информацию о каждом элементе в директории
    int num_entries; // Количество элементов в директории для определения размера массива entry_list

    dp = opendir(dir); // Открываем директорию
    if (dp == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dir); // Выводим сообщение об ошибке, если не удалось открыть директорию
        return;
    }

    num_entries = scandir(dir, &entry_list, NULL, sort ? compare : alphasort); // Используем scandir для сортировки, если sort = true, если сортировка не задана то по алфавиту
    if (num_entries == -1) {
        fprintf(stderr, "Error scanning directory: %s\n", dir); // Выводим сообщение об ошибке, если не удалось сканировать директорию
        closedir(dp); // Закрываем директорию
        return;
    }

    for (int i = 0; i < num_entries; i++) {
        struct dirent* entry = entry_list[i]; // Получаем текущий элемент директории

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // Пропускаем элементы "." и ".."

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name); // Создаем полный путь к элементу
        // snprintf форматирует строку по формату и возвращает ее в path

        if ((!symbolic_links && entry->d_type == DT_LNK) ||
            (!directories && entry->d_type == DT_DIR) ||
            (!files && entry->d_type == DT_REG))
            continue; // Пропускаем элементы в зависимости от указанных флагов
            // Прим. если флаг symbolic_links установлен в false и тип элемента директории entry->d_type указывает на символическую ссылку (DT_LNK), то элемент будет пропущен.

        print_entry(path, entry->d_type); // Выводим информацию об элементе

        if (entry->d_type == DT_DIR) {
            dirwalk(path, symbolic_links, directories, files, sort); // Рекурсивно вызываем функцию для поддиректорий
        }
    }

    for (int i = 0; i < num_entries; i++) {
        free(entry_list[i]); // Освобождаем память для каждого элемента директории
    }
    free(entry_list); // Освобождаем память для массива элементов директории

    closedir(dp); // Закрываем директорию
}

int main(int argc, char* argv[]) {
    char* dir = NULL; // Переменная для хранения имени директории
    bool symbolic_links = true; // Флаг для отображения символических ссылок
    bool directories = true; // Флаг для отображения директорий
    bool files = true; // Флаг для отображения файлов
    bool sort =true; // Флаг для сортировки элементов

    int opt;
    while ((opt = getopt(argc, argv, "ldfs")) != -1) {
        switch (opt) {
            case 'l':
                symbolic_links = true;
                directories = false;
                files = false;
                break; // Устанавливаем флаги в соответствии с выбранными опциями
            case 'd':
                symbolic_links = false;
                directories = true;
                files = false;
                break; // Устанавливаем флаги в соответствии с выбранными опциями
            case 'f':
                symbolic_links = false;
                directories = false;
                files = true;
                break; // Устанавливаем флаги в соответствии с выбранными опциями
            case 's':
                sort = true;
                break; // Устанавливаем флаг сортировки в true
            default:
                fprintf(stderr, "Usage: %s [dir] [options]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        dir = argv[optind]; // Если указана директория в аргументах командной строки, используем ее
    } else {
        dir = "."; // Устанавливается текущая директория, если не задано
    }

    setlocale(LC_COLLATE, ""); // Лкаль для сортировки
    
//ru_RU.UTF-8           en_US.UTF-8


    dirwalk(dir, symbolic_links, directories, files, sort); // Вызываем функцию для обхода директории

    return 0;
}
