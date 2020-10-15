#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct list {
    int value;
    struct list *next;
} list;
 
list* list_create(int value);
list* list_add_front(int value, list* link);
void list_add_back(int value, list* link);
int list_get(int index, list* link);
void list_free(list* link);
int list_length(list* link);
list* list_node_at(list* link, int index);
long list_sum(list* link);

#endif
