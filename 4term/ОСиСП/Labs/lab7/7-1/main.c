#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include "ring.h"
#include <pthread.h>
#include "list.h"

enum structure_of_message {
    TYPE = 0,
    HIGH_BYTE_HASH = 1,
    LOW_BYTE_HASH = 2,
    SIZE = 3,
    DATA_BEGIN = 4
};

#define INITIAL_SIZE_BUFFER 3

pthread_cond_t cond;
pthread_mutex_t mutex;

ring_buffer* queue = NULL;
_Thread_local bool FLAG_CONTINUE = true;

bool flag_break_process = false;

size_t producer_passes = 0;

size_t CNT_FREE_PLACES = INITIAL_SIZE_BUFFER;
size_t CNT_FILL_PLACES = 0;

u_int16_t control_sum(const u_int8_t*, size_t);
void producer();
void consumer();
u_int8_t* generate_message();
void handler_stop_proc();
void display_message(const u_int8_t*);
void break_all_pthreads(const node_list*);

int main() {

    node_list* list_of_ptreads = NULL;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (size_t i = 0; i < CNT_FREE_PLACES; ++i)
        append(&queue, false);

    bool flag = true;
    do {
        char ch = getchar();
        switch(ch) {
            case 'p' : {
                pthread_t pthread_producer;
                pthread_create(&pthread_producer, NULL, producer, NULL);
                push_list(&list_of_ptreads, pthread_producer, 'P');
                break;
            }
            case 'c' : {
                pthread_t pthread_consumer;
                pthread_create(&pthread_consumer, NULL, consumer, NULL);
                push_list(&list_of_ptreads, pthread_consumer, 'C');
                break;
            }
            case 'l' : {
                display_list(list_of_ptreads);
                if (!is_empty(queue)) {
                    printf("Current size of queue = %lu\n", queue->size_queue);
                }
                break;
            }
            case 'k' : {
                size_t num;
                scanf("%lu", &num);
                pthread_t pthread_id;
                if (erase_list(&list_of_ptreads, num, &pthread_id)) pthread_kill(pthread_id, SIGUSR1);
                break;
            }
            case '+' : {
                pthread_mutex_lock(&mutex);
                printf("insert\n");
                append(&queue, true);
                printf("Insert, count of places : %lu\n", queue->size_queue);
                CNT_FREE_PLACES++;
                pthread_mutex_unlock(&mutex);
                break;
            }
            case '-' : {
                pthread_mutex_lock(&mutex);
                sleep(2);
                bool flag_execute;
                if (!is_empty(queue)) {
                     flag_execute = erase(&queue);
                }
                else {
                    printf("Queue is empty!\n");
                    continue;
                }
                if (flag_execute == false) {
                    producer_passes++;
                    CNT_FREE_PLACES--;
                }
                if (flag_execute == true) {
                    CNT_FILL_PLACES--;
                }
                if (!is_empty(queue)) {
                    printf("Delete place, current size of queue = %lu\n", queue->size_queue);
                }else {
                    printf("Delete last place, queue is empty!\n");
                }
                pthread_mutex_unlock(&mutex);
                break;
            }
            default : {
                flag = false;
                flag_break_process = true;
                pthread_cond_broadcast(&cond);
                break;
            }
        }
        getchar();
    }while(flag);

    break_all_pthreads(list_of_ptreads);
    free(list_of_ptreads);
    if (!is_empty(queue)) {
        free(queue);
    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return 0;
}


void break_all_pthreads(const node_list* head) {
    if (head == NULL) {
        return;
    }
    while(head) {
        pthread_cancel(head->pthread_id);
        head = head->next;
    }
}


u_int16_t control_sum(const u_int8_t* data, size_t length) {
    u_int16_t hash = 0;
    for (size_t i = 0; i < length; ++i) {
        hash += data[i];
    }
    return hash;
}

u_int8_t* generate_message() {
    srand(time(NULL));
    u_int8_t* result_message = (u_int8_t*)calloc(LEN_MESSAGE , sizeof(u_int8_t));
    size_t size = 0;
    size_t data_size = 0;
    while(size == 0) size = rand() % 257;
    if (size == 256) {
        size = 0;
        data_size = 256;
    }else {
        data_size = ((size + 3) / 4) * 4;
    }
    for (size_t i = DATA_BEGIN; i < data_size; ++i) {
        result_message[i] = rand() % 256;
    }
    u_int16_t hash = control_sum(result_message, size);
    result_message[TYPE] = 1;
    result_message[HIGH_BYTE_HASH] = (hash >> 8) & 0xFF;
    result_message[LOW_BYTE_HASH] = hash & 0xFF;
    result_message[SIZE] = size;
    return result_message;
}

void display_message(const u_int8_t* message) {
    size_t message_size = 0;
    if (message[SIZE] == 0) {
        message_size = LEN_MESSAGE;
    }else {
        message_size = message[SIZE] + OFFSET;
    }
    for (size_t i = 0; i < message_size; ++i)
        printf("%02X", message[i]);
    printf("\n");
}
void handler_stop_proc() {
    FLAG_CONTINUE = false;
}

void consumer() {
    signal(SIGUSR1, handler_stop_proc);
    do {
        pthread_mutex_lock(&mutex);
        while (CNT_FILL_PLACES == 0) {
            pthread_cond_wait(&cond, &mutex);
            if (flag_break_process == true)
                break;
        }
        sleep(2);
        u_int8_t* message = extract_message(queue);
        CNT_FILL_PLACES--;
        CNT_FREE_PLACES++;
        display_message(message);
        free(message);
        printf("Consumed from pthread with id = %lu\n", pthread_self());
        printf("Total messages retrieved = %lu\n", queue->consumed);
        pthread_mutex_unlock(&mutex);
        usleep(50000);
    }while(FLAG_CONTINUE);
    pthread_join(pthread_self(), NULL);
}

void producer() {
    signal(SIGUSR1, handler_stop_proc);
    do {
        pthread_mutex_lock(&mutex);
        if (CNT_FREE_PLACES == 0) {
            pthread_mutex_unlock(&mutex);
            sleep(5);
            continue;
        }
        sleep(2);
        u_int8_t* new_message = generate_message();
        add_message(queue, new_message);
        CNT_FILL_PLACES++;
        CNT_FREE_PLACES--;
        if (CNT_FREE_PLACES == 0) {
            pthread_cond_broadcast(&cond);
        }
        free(new_message);
        printf("Produced from pthread with id = %lu\n", pthread_self());
        printf("Total ojbects created = %lu\n", queue->produced);
        pthread_mutex_unlock(&mutex);
        usleep(50000);
    }while(FLAG_CONTINUE);
    pthread_join(pthread_self(), NULL);
}


