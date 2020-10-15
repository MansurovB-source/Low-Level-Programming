#include "./linked_list.h"
#include <stdbool.h>
#ifndef H_PRINT
#define H_PRINT
 
bool save(list* link, const char* filename);
bool load(list** link, const char* filename);
bool serialize(list* link, const char* filename);
bool deserialize(list** link, const char* filename);
void print_space(int);
void print_new(int);
 
#endif
