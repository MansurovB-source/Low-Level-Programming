all: main
 
main: main.c linked_list.c higher_order_func.c print.c linked_list.h higher_order_func.h print.h 
	gcc -c linked_list.c higher_order_func.c print.c main.c
	gcc -o main -ansi -pedantic-errors -Wall -Werror linked_list.o higher_order_func.o print.o main.o -lm
clean:
	rm -f main *.o
