CC = gcc

all: main

rotate_logic.o: image_struct.h rotate_logic.c 
	$(CC) -c -o rotate_logic.o rotate_logic.c

bmp.o: bmp_struct.h bmp.c
	$(CC) -c -o bmp.o bmp.c

sepia_func.o: sepia_func.h sepia_func.c
	$(CC) -c -o sepia_func.o sepia_func.c

sepia_sse.o: sepia_sse.asm
	nasm -felf64 sepia_sse.asm -o sepia_sse.o

read_file.o: read_file.c
	$(CC) -c -o read_file.o read_file.c

main.o: main.c
	$(CC) -c -o main.o main.c

main: rotate_logic.o bmp.o sepia_func.o sepia_sse.o read_file.o main.o
	$(CC) -o main rotate_logic.o bmp.o sepia_func.o sepia_sse.o read_file.o main.o -lm

clean:
	rm -f main *.o

start:
	./main -m bmp -t s -a 290

# CC=gcc
# CFLAGS=-Wall -Wextra -Wpedantic
# LDFLAGS=

# SOURCES=$(wildcard *.c )
# OBJECTS=$(SOURCES:.c=.o)
# EXECUTABLE=exec

# all: $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)

# $(OBJECTS): $(SOURCES)
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm $(EXECUTABLE) $(OBJECTS)