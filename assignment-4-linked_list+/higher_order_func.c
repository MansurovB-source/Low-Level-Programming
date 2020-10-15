#include "./linked_list.h"
#include "./higher_order_func.h"
#include <stddef.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
 
void foreach(list* link, void (*func)(int)) {
    while(link != NULL) {
        func(link -> value);
        link = link -> next;    
    }
}
 
list* map(list* link, int (*func)(int)) {
    if(link -> next != NULL) {
        return list_add_front(func(link -> value), map(link -> next, func));
    } else {
        return list_create(func(link -> value));
    }
}
 
void map_mut(list* link, int (*func)(int)) {
    while(link -> next != NULL) {
        link -> value = func(link -> value);
        link = link -> next;
    }
}
 
list* iterate(int s, int length, int (*func)(int)) {
    int i;
    list* new_node = list_create(s);
    for(i = 1; i < length; i++) {
        new_node = list_add_front(func(new_node -> value), new_node);
    }
    return new_node;
}
 
int foldl(int acum, int (*func)(int, int), list* link) {
    while(link != NULL) {
        acum = func(link -> value, acum);
        link = link -> next;
    }
    return acum;
}
 
int square(int x) {
    if(x < sqrt(INT_MAX)) {
        return x * x;
    } else {
        return 0;
    }
}
 
int cub(int x) {
    x = abs(x);	
    if(log(x) < (log(INT_MAX) / 3)) {
        return x * x * x;
    } else {
        return 0;
    }
}
 
int sum(int a, int b) {
    if(a < INT_MAX - b) {
        return a + b;
    } else {
        return 0;
    }
}
 
int mul_2(int n) {
    if (n < INT_MAX / 2) {
        return n * 2;
    } else {
        return 0;
    }
}
