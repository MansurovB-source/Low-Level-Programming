lib.o: lib.asm 
	nasm -f elf64 -o lib.o lib.asm
    
dict.o: dict.asm 
	nasm -f elf64 -o dict.o dict.asm

main.o: main.asm colon.inc words.inc
	nasm -f elf64 -o main.o main.asm
 
main: main.o dict.o lib.o
	ld -o main main.o dict.o lib.o

mapping.o: mapping.asm
	nasm -f elf64 -o mapping.o mapping.asm

mapping: mapping.o lib.o
	ld -o mapping mapping.o lib.o

clean: 
	rm -f *.o 
 
all: main
