SRC = image_struct.h bmp_struct.h rotate_logic.c bmp.c read_file.c main.c
CC = gcc
OUTPUT = main

all: 
	$(CC) -o $(OUTPUT) $(SRC) -lm

clean:
	rm -f main *.o

start:
	./main -m bmp -t b -a 290

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