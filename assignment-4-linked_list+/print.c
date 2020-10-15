#include "./linked_list.h"
#include "./print.h"
#include <stdbool.h>
#include <stdio.h>
 
#include "./higher_order_func.h"
 
bool save(list* link, const char* filename) {
    FILE* file;
    file = fopen(filename, "w");
    if(file == NULL) {
        return false;
    }
    while(link != NULL) {
        fprintf(file, "%d ", link -> value);
        link = link -> next;
    };
    fclose(file);
    return true;
}
 
bool load(list** link, const char* filename) {
	FILE* file;
    int val;
    file = fopen(filename, "r");
    if(file == NULL) {
        return false;
    }
    if(fscanf(file, "%d ", &val) != EOF) {
        *link = list_create(val);
    }
    while(fscanf(file, "%d ", &val) != EOF) {
       *link = list_add_front(val, *link);
    }
 	printf("%p", &*link);
    fclose(file);
    return true;
}
 
bool serialize(list* link, const char* filename) {
    FILE* bfile;
    if((bfile = fopen(filename, "wb")) == NULL) {
        return false;
    }
    while (link != NULL) {
        fwrite(&(link -> value), sizeof(int), 1, bfile);
        link = link -> next;
    }
    fclose(bfile);
    return true;
}
 
 
 
bool deserialize(list** link, const char* filename) {
    FILE* bfile;
    int val;
    
    if((bfile = fopen(filename, "rb")) == NULL) {
        return false;
    }
    if(fread(&val, sizeof(int),1, bfile)) {
        *link = list_create(val);
    }
    while (fread(&val, sizeof(int),1,bfile) != 0) {
        *link = list_add_front(val, *link);
    }
    fclose(bfile);
    return true;
}
 
void print_space(int i){
    printf("%d ",i);
}
 
void print_new(int i){
    printf("%d\n",i);
}
