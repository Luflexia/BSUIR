#ifndef CONTROL_H
#define CONTROL_H

typedef enum {
    SORT_BY_PID,
    SORT_BY_RESIDENT_MEMORY,
    SORT_BY_VIRTUAL_MEMORY
} SortCriteria;

typedef enum {
    SORT_ORDER_ASCENDING,
    SORT_ORDER_DESCENDING
} SortOrder;

extern SortCriteria current_sort;

void handle_user_input(int ch);
void kill_process_or_thread();
int compare_by_pid(const void *a, const void *b);
int compare_by_resident_memory(const void *a, const void *b);
int compare_by_virtual_memory(const void *a, const void *b);
#endif //CONTROL_H
