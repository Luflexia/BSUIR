#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void get_thread_list(pthread_t **threads, size_t *count) {
    pthread_t *thread_array = NULL;
    size_t num_threads = 0;
    size_t array_size = 10;

    thread_array = malloc(array_size * sizeof(pthread_t));
    if (thread_array == NULL) {
        perror("Ошибка при выделении памяти");
        exit(EXIT_FAILURE);
    }

    pthread_t self = pthread_self();

    // Получаем идентификаторы потоков
    pthread_t *all_threads;
    size_t max_threads = 1024;
    all_threads = malloc(max_threads * sizeof(pthread_t));
    if (all_threads == NULL) {
        perror("Ошибка при выделении памяти");
        exit(EXIT_FAILURE);
    }

    if (pthread_getaffinity_np(self, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("Ошибка при получении привязки потоков");
        exit(EXIT_FAILURE);
    }

    // Перебираем все потоки, проверяем, если они привязаны к текущему процессу
    for (size_t i = 0; i < max_threads; i++) {
        if (pthread_equal(all_threads[i], self)) {
            continue;  // Пропускаем текущий поток
        }

        int result = pthread_setaffinity_np(all_threads[i], sizeof(cpu_set_t), &cpuset);
        if (result == 0) {
            // Проверяем, достаточно ли места в массиве thread_array
            if (num_threads >= array_size) {
                array_size *= 2;
                thread_array = realloc(thread_array, array_size * sizeof(pthread_t));
                if (thread_array == NULL) {
                    perror("Ошибка при выделении памяти");
                    exit(EXIT_FAILURE);
                }
            }

            // Добавляем поток в массив thread_array
            thread_array[num_threads] = all_threads[i];
            num_threads++;
        }
    }

    free(all_threads);

    *threads = thread_array;
    *count = num_threads;
}