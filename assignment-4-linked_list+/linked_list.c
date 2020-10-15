#include "./linked_list.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

list* list_create(int value) {
    return list_add_front(value, NULL);
}
 
list* list_add_front(int value, list* link) {
    list* new_node;
    new_node = malloc(sizeof(list));
    new_node -> value = value;
    new_node -> next = link;
    return new_node;
}
 
void list_add_back(int value, list* link) {
    while(link -> next != NULL) {
        link = link -> next;
    }
    list* new_node;
    new_node = malloc(sizeof(list));
    new_node -> value = value;
    new_node -> next = link;
}
 
int list_get(int index, list* link) {
    list* node;
    if((node = list_node_at(link, index)) != NULL) {
        return node -> value;
    } else {
        return 0;
    }
}
 
void list_free(list* link) {
	//list* f_free;
    //while(link != NULL) {
    //    f_free = link;
    //    link = link -> next;
    //  free(f_free);
    //}

    if(link == NULL) return;
    list_free(link -> next);
	free(link);
	
}
 
int list_length(list* link) {
    int length = 0;
    while(link != NULL) {
        link = link -> next;
        length++;
    }
    return length;
}
 
list* list_node_at(list* link, int index) {
    int i = 1;
    while(i < index && link != NULL) {
        link = link -> next;
        i++;
    }
    if(i == index) {
        return link;
    } else {
        return NULL;
    }
}
 
long list_sum(list* link) {
    long sum = 0;
    while(link != NULL) {
        sum += link -> value;
        link = link -> next;
    }
    return sum;
}
