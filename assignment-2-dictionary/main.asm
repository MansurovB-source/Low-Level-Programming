section .text
%include "colon.inc"
extern read_word
extern find_word
extern string_length
extern print_string
extern print_error
extern exit
 
global _start
section .rodata

error: db "No such word in dictionary", 0
%include "words.inc"
 
section .text
_start:
    push rbp ; сохраняем rbx
    mov rbp, rsp ; запоминаем вершину стека
    sub rsp, 256 ; создаем буфер
    mov rdi, rsp ; вершину стека в read_word
    push rsi ; сохраняем rsi
    mov rsi, 256 ;
    call read_word ; читаем члово, с помощью extern
    ; подготовка аргументов для поиска слова
    mov rdi, rax ; сохраняем слово в rdi
    mov rsi, last_word ; передаем адрес последнего слова
    call find_word ; выполняем поиск по словарю
    test rax, rax ; проверяем на 0 
    jz .end ; если слово не найдено, заканчивам
    add rax, 8 ; пропускае ссылку на след слова
    push rax ; кладем rax на стек
    mov rax, [rsp] ; сохраняем определение из словаря 
    mov rdi, rax ; передаем его как аргумен в string_length
    call string_length ; находим длину
    pop rdi ; востанавливаем ссылку на слово в rdi
    add rdi, rax ; прибавляем длину 
    inc rdi ; для нуль- терминатора 
    call print_string ; печатаем строку 
    mov rsp, rbp ; восстанавливаем стек
    pop rsi; восстанавливаем rsi
    pop rbp ; восстанавливаем rbp
    call exit ; выход из программы
.end:
    mov rdi, error ; сообщение
    call print_error ; выводим сообщение0
    mov rsp, rbp ; возвращаем стек в начальное состояние
    pop rsi; восстанавливаем rsi
    pop rbp ; rbp в начальное состояние
    call exit ; выход из программы 
