#include "updateThreads.h"
#include "readThreads.h"

// Функция для обновления данных о потоках
void update_thread_data(Thread *threads, int num_threads) {
    // Вызываем функцию для чтения информации о потоках
    int thread_count = read_threads(threads);
    if (thread_count < 0) {
        // Обработка ошибки
        return;
    }
    
    if (thread_count > num_threads) {
        // Если количество прочитанных потоков больше, чем необходимо, обрабатываем только num_threads потоков
        thread_count = num_threads;
    }
}
