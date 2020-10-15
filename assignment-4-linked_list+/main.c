#include "./linked_list.h"
#include "./higher_order_func.h"
#include "./print.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int n;
    list* link = NULL;
    printf("Enter the elements of linked_list:\n");
    while(scanf("%d", &n) != EOF) {
        if(link == NULL) {
            link = list_create(n);
        } else {
            link = list_add_front(n, link);
        }
    }
    printf("\n");
    
	printf("The length of linked_list: %d\n", list_length(link));

    printf("Sum of the linked_list elements: %ld\n", list_sum(link));

    printf("Enter the index of element: ");
    scanf("%d", &n);
    n = list_get(n, link);
    if(n) {
        printf("The value is: %d\n", n);
    } else {
        printf("Not enough length");
    }
	
	printf("%s\n", "foreach with spaces");
	foreach(link, print_space);
	printf("\n");
	
	printf("%s\n", "foreach with new line");
	foreach(link, print_new);

	list *squares = map(link, square);
	list *cubes = map(link, cub);
	
	printf("map square \n");
	foreach(squares, print_space);
	printf("\n");
	
	printf("map cubes \n");
	foreach(cubes, print_space);
	printf("\n");

	map_mut(link, abs);
	printf("map_mut abs \n");
	foreach(link, print_space);
	printf("\n");

	int s = foldl(0, sum, link);
	printf("foldl sum -> %d\n", s);

	list *iter = iterate(1, 10, mul_2);
	printf("iteration\n");
	foreach(iter, print_space);
	printf("\n");

	printf("save in file\n");
	if (!save(iter, "list.txt")) {
		printf("error\n");
		return -1;
	}
	printf("saved\n");
	
	list_free(iter);
	iter = NULL;
	//printf("%p", &*iter);
	printf("load from file\n");
	if (!load(&iter, "list.txt")) {
		printf("error\n");
	}
	//printf("%p", &*iter);
	printf("load complite, list is -> ");
	foreach(iter, print_space);
	printf("\n");
		
	printf("serialize\n");
	if (!serialize(iter, "list.bin")) {
		printf("error\n");
		return -3;
	}
	printf("serialize complit\n");
	
	list_free(iter);
	iter = NULL;

	printf("deserialize\n");
	if (!deserialize(&iter, "list.bin")) {
		printf("error\n");
		return -4;
	}
	printf("deserialize complite, list is -> ");
	foreach(iter, print_space);
	printf("\n");
	
	list_free(link);
	list_free(squares);
	list_free(cubes);
	list_free(iter);
    return 0;
}
