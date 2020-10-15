section .text
extern exit
extern print_newline
extern print_uint
 
%define O_RDONLY 0 
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2
 
section .data
fname: db 'sum.txt', 0
 
section .text
global _start
 
_start:
.open:
    mov rax, 2
    mov rdi, fname
    mov rsi, O_RDONLY
    mov rdx, 0
    
    syscall
    
.mmap:
    mov r8, rax
    
    mov rax, 9
    mov rdi, 0
    mov rsi, 4096
    mov rdx, PROT_READ
    mov r10, MAP_PRIVATE
    mov r9, 0
    
    syscall
    
    mov rdi, rax
 
.summ:
    xor rax, rax
    mov dl, [rdi]
    inc rdi
    sub rdx, 0x30
    xor r9, r9

.loop:
    dec rdx
    js .end
    mov r9b, byte[rdi] ; читаем первый символ
    cmp r9b, 0x30 ; сравниваем с 0
    jb .end
    cmp r9b, 0x39 ; сравниваем с 9
    ja .end
    sub r9b, 0x30 ; вычитаем 0 чтобы получить 0 - 9
    add rax, r9 ; добавляем в аккумулятор
    inc rdi ; счетчик
    jmp .loop 

.end:
	mov rdi, rax
	call print_uint
	call print_newline	
	call exit
