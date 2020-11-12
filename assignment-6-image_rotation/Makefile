SRC = bmp_struct.h bmp.c main.c
CC = gcc
OUTPUT = main

all: 
	$(CC) -o $(OUTPUT) $(SRC) -lm

clean:
	rm -f main *.o

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