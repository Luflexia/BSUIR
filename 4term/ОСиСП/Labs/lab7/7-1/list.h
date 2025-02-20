#ifndef LIST_H
#define LIST_H

#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct node_list {
    char index_worker;
    pthread_t pthread_id;
    struct node_list* next;
}node_list;

node_list* constructor_list(pthread_t, char);
void push_list(node_list**, pthread_t, char);
void display_list(const node_list*);
pthread_t pop_list(node_list**);
void clear_list(node_list**);
bool erase_list(node_list**, size_t, pthread_t*);

#endif //LIST_H