#include "mainFunctions.h"

int compare(const void* a, const void* b) {           // Сравнение двух объектов для qsort.
    if (((index_record*)a)->time_mark < ((index_record*)b)->time_mark)
        return -1;
    else if(((index_record*)a)->time_mark > ((index_record*)b)->time_mark)
        return 1;
    else
        return 0;
}

void* sorting(void* threadA) {
    threadArgs* args = (threadArgs*)threadA;          // Получение параметров файла.
    pthread_barrier_wait(&barrier);

    printf("Qsort in %d thread.\n", args->threadNum); // Сортировка блоков qsort.
    while(cur < args->buf + size) {
        pthread_mutex_lock(&mutex);
        if(cur < args->buf + size) {
            index_record *temp = cur;
            cur += args->blockSize;
            pthread_mutex_unlock(&mutex);
            qsort(temp, args->blockSize, sizeof(index_record), compare);
        } else {
            pthread_mutex_unlock(&mutex);
            pthread_barrier_wait(&barrier);
            break;
        }
    }
    printf("Merging in %d thread.\n", args->threadNum);
    int mergeStep = 2;
    // merge
    while(mergeStep<=blocks) {                       // Слияние отсортированных блоков.
        pthread_barrier_wait(&barrier);
        cur = args->buf;
        while (cur < args->buf + size) {
            pthread_mutex_lock(&mutex);
            if(cur < args->buf + size) {
                index_record *temp = cur;
                cur += mergeStep*args->blockSize;
                pthread_mutex_unlock(&mutex);
                int bufSize = (mergeStep / 2) * args->blockSize;
                index_record *left = (index_record *) malloc(bufSize * sizeof(index_record));
                memcpy(left, temp, (mergeStep / 2) * args->blockSize*sizeof(index_record));
                index_record *right = (index_record *) malloc(bufSize * sizeof(index_record));
                memcpy(right, temp + (mergeStep / 2) * args->blockSize,(mergeStep / 2) * args->blockSize*sizeof(index_record));

                int i = 0, j = 0;
                while (i < bufSize && j < bufSize) {
                    if (left[i].time_mark < right[j].time_mark) {
                        temp[i+j].time_mark = left[i].time_mark;
                        temp[i+j].recno = left[i].recno;
                        i++;
                    } else {
                        temp[i+j].time_mark = right[j].time_mark;
                        temp[i+j].recno = right[j].recno;
                        j++;
                    }
                }
                if (i == bufSize) {
                    while (j > bufSize) {
                        temp[i+j].time_mark = right[j].time_mark;
                        temp[i+j].recno = right[j].recno;
                        j++;
                    }
                }
                if (j == bufSize) {
                    while (i < bufSize) {
                        temp[i+j].time_mark = left[i].time_mark;
                        temp[i+j].recno = left[i].recno;
                        i++;
                    }
                }
            } else {
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
        mergeStep*=2;
    }
    pthread_mutex_unlock(&mutex);
    pthread_barrier_wait(&barrier);
}

void *openMem(void* data) {
    printf("Opening data start.\n");
    creatingData* cData = (creatingData*)data;      

    FILE* f = fopen(cData->fname, "rb+");       // Открыть файл.
    if(f==NULL){
        printf("Error while open file.\n");
        return 0;
    }

    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    int fd = fileno(f);
    void * buf;                                 // Отобразить файл в память.
    if((buf = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
        printf("Error while creating mapping, %d\n", errno);
        exit(errno);
    }

    buf+=sizeof(uint64_t);                      // Переход к первой структуре.

    cur = (index_record *)buf;
    printf("Creating threads.\n");
    pthread_t threadsId[cData->threads-1];      // Запуск потоков.
    for(int i = 1; i<cData->threads; i++) {
        threadArgs* args = (threadArgs*)malloc(sizeof(threadArgs));
        args->blockSize = cData->blockSize;
        args->threadNum = i;
        args->buf = (index_record*)buf;

        if(pthread_create(&threadsId[i-1], NULL, sorting, args) != 0) {
            printf("Error while creating %d thread.\n", i);
            exit(0);
        }
    }
    threadArgs* args = (threadArgs*)malloc(sizeof(threadArgs));
    args->blockSize = cData->blockSize;
    args->threadNum = 0;
    args->buf = (index_record*)buf;
    sorting(args);

    for(int i = 1; i<cData->threads; i++)
        pthread_join(threadsId[i-1], NULL);     // Ожидание завершения.
    printf("End of threads.\n");

    munmap(buf, fsize);
    fclose(f);
    printf("Memory closing.\n");
}
