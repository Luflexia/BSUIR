#include "list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

node_list* constructor_list(pthread_t _pthread_id, char index) {
    node_list* val = (node_list*)malloc(1 * sizeof(node_list));
    val->next = NULL;
    val->pthread_id = _pthread_id;
    val->index_worker = index;
    return val;
}

void push_list(node_list** head, pthread_t _pthread_id, char index) {
    if (head == NULL)
        exit(-1);
    if (*head ==  NULL) {
        *head = constructor_list(_pthread_id, index);
        return;
    }
    node_list* cursor = *head;
    while(cursor->next)
        cursor = cursor->next;
    cursor->next = constructor_list(_pthread_id, index);
}

void display_list(const node_list* head) {
    size_t i = 1;
    if (head == NULL) {
        printf("There are no elements in the list!\n");
        return;
    }
    while(head) {
        printf("Pthread_%lu_%c id = %lu\n", i++, head->index_worker, head->pthread_id);
        head = head->next;
    }
}

pthread_t pop_list(node_list** head) {
    if (head == NULL)
        exit(-1);
    if (*head == NULL)
        return -1;
    node_list* cursor = *head;
    node_list* prev = NULL;
    while(cursor -> next) {
        prev = cursor;
        cursor = cursor->next;
    }
    pid_t pid = cursor->pthread_id;
    free(cursor);
    if (prev == NULL)
        *head = NULL;
    else prev -> next = NULL;
    return pid;
}

void clear_list(node_list** head) {
    if (head == NULL) {
        return;
    }
    if (*head == NULL) {
        printf("List is empty!");
        return;
    }
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }
    node_list* prev = *head;
    while(*head) {
        prev = *head;
        *head = (*head)->next;
        free(prev);
    }
    *head = NULL;
}

bool erase_list(node_list** head, size_t num, pthread_t* pthread_id) {
    if (head == NULL) {
        exit(-2);
    }
    if (*head == NULL) {
        printf("There are no elements in the list!\n");
        return false;
    }
    node_list* prev = NULL;
    node_list* curr = *head;
    size_t i = 0;
    while(curr->next && i != num) {
        prev = curr;
        curr = curr->next;
        ++i;
    }
    if (i != num) {
        printf("There is no element with this number.");
        return false;
    }
    *pthread_id = curr->pthread_id;
    if (prev == NULL) {
        *head = (*head)->next;
        free(curr);
        return true;
    }
    prev -> next = curr->next;
    free(curr);
    return true;
}