#include "ring.h"

#include <inttypes.h>

bool is_empty(const ring_buffer* __queue) {
    if (__queue == NULL) {
        return true;
    }
    return false;
}

ring_node* constructor_node() {
    ring_node* buffer = (ring_node*)calloc(1, sizeof(ring_node));
    buffer->next = NULL;
    buffer->prev = NULL;
    buffer->flag_is_busy = false;
    return buffer;
}

ring_buffer* constructor_buffer() {
    ring_buffer* buffer = (ring_buffer*)malloc(sizeof(ring_buffer));
    buffer->begin = NULL;
    buffer->tail = NULL;
    buffer->size_queue = 0;
    return buffer;
}

void append(ring_buffer** __head, bool flag_after) {
    if (__head == NULL)
        exit(-100);
    if (*__head == NULL) {
        *__head = constructor_buffer();
        (*__head)->begin = (*__head)->tail = constructor_node();
        (*__head)->begin->next = (*__head)->begin->prev = (*__head)->begin;
        (*__head)->size_queue++;
        return;
    }
    (*__head)->size_queue++;
    ring_node* __buffer = constructor_node();
    if ((*__head)->begin->next == (*__head)->begin) {
        (*__head)->begin->next = (*__head)->begin->prev = __buffer;
        __buffer->next = __buffer->prev = (*__head)->begin;
    }else {
        __buffer->next = (*__head)->begin;
        __buffer->prev = (*__head)->begin->prev;
        __buffer->prev->next = __buffer;
        (*__head)->begin->prev = __buffer;
    }
    if (flag_after) {
        if (__buffer->next == (*__head)->tail) {
            if ((*__head)->tail == (*__head)->begin && (*__head)->begin->flag_is_busy == false) {
                (*__head)->tail = (*__head)->begin = __buffer;
            }else (*__head)->tail = __buffer;
        }
    }
}

bool erase(ring_buffer** __head) {
    if (__head == NULL) {
        exit(-100);
    }
    if (*__head == NULL) {
        printf("The queue is empty.");
        exit(-100);
    }
    if((*__head)->begin == NULL) {
        printf("The queue is empty.");
        exit(-100);
    }
    (*__head)->size_queue--;
    bool result;
    if ((*__head)->tail == (*__head)->begin) {
        if ((*__head)->begin == (*__head)->begin->next) {
            result = (*__head)->begin->flag_is_busy;
            free((*__head)->begin);
            free(*__head);
            *__head = NULL;
        }else {
            ring_node* buffer = (*__head)->begin;
            (*__head)->begin->next->prev = (*__head)->begin->prev;
            (*__head)->begin->prev->next = (*__head)->begin->next;
            (*__head)->begin = (*__head)->tail = (*__head)->begin->next;
            result = buffer->flag_is_busy;
            free(buffer);
        }
        return result;
    }
    if ((*__head)->begin->next == (*__head)->begin->prev) {
        (*__head)->tail = (*__head)->tail->prev;
        result = (*__head)->tail->next->flag_is_busy;
        free((*__head)->tail->next);
        (*__head)->tail->next = (*__head)->tail->prev = (*__head)->tail;
        return result;
    }
    ring_node* buffer = (*__head)->tail;
    (*__head)->tail->next->prev = (*__head)->tail->prev;
    (*__head)->tail->prev->next = (*__head)->tail->next;
    (*__head)->tail = (*__head)->tail->next;
    result = buffer->flag_is_busy;
    free(buffer);
    return result;
}

void clear_ring(ring_buffer** __head) {
    if (__head == NULL)
        return;
    if ((*__head)->size_queue == 0)
        return;
    if ((*__head)->size_queue == 1) {
        free(*__head);
        *__head = NULL;
        return;
    }
    size_t size_ring = (*__head)->size_queue;
    ring_node* __cursor = (*__head)->begin;
    while(size_ring - 1) {
        free(__cursor->prev);
        __cursor = __cursor->next;
        size_ring--;
    }
    free(__cursor);
    *__head = NULL;
}

void add_message(ring_buffer* __head, const u_int8_t* __message) {
    if (__head == NULL) {
        printf("The ring is empty.\n");
        return;
    }
    if (__head->begin == NULL) {
        printf("There are 0 places in the ring.\n");
        return;
    }
    ring_node* __curr = __head->tail;
    if (__curr->flag_is_busy == true) {
        printf("No free places.\n");
        return;
    }
    for (size_t i = 0; i < LEN_MESSAGE; ++i) {
        __curr->message[i] = __message[i];
    }
    __curr->flag_is_busy = true;
    __head->tail = __head->tail->next;
    __head->produced++;
}

u_int8_t * extract_message(ring_buffer* __head) {
    if (__head == NULL) {
        printf("The ring is empty.\n");
        return NULL;
    }
    if (__head->begin == NULL) {
        printf("There are 0 places in the ring.\n");
        return NULL;
    }
    ring_node* __curr = __head->begin;
    if (__curr->flag_is_busy == false) {
        printf("No free places.\n");
        return NULL;
    }

    u_int8_t* __message = (u_int8_t*)calloc(LEN_MESSAGE, sizeof(u_int8_t));

    for (size_t i = 0; i < LEN_MESSAGE; ++i) {
        __message[i] = __curr->message[i];
    }
    __curr->flag_is_busy = false;
    __head->begin = __head->begin->next;
    __head->consumed++;
    return __message;
}

