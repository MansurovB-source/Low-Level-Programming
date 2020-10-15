#include "./linked_list.h"
#ifndef H_HIGHER_ORDER_FUNC
#define H_HIGHER_ORDER_FUNC
 
void foreach(list* link, void (*func)(int));
list* map(list* link, int (*func)(int));
void map_mut(list* link, int (*func)(int));
list* iterate(int s, int length, int (*func)(int));
int foldl(int acum, int (*func)(int, int), list* link);
int square(int x);
int cub(int x);
int sum(int a, int b);
int mul_2(int n);
 
#endif
