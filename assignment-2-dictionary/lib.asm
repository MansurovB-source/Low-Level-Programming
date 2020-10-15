section .text
 
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global read_char
global read_word
global parce_uint
global parse_int
global string_equals
global string_copy
global print_error
global exit
 
;Выход из программы
exit:
    mov rax, 60
    xor rdi, rdi
    syscall
; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax    
    
.loop:
    cmp byte[rdi + rax], 0 ; вычесляется адрес, сравнивается с 0 (конец строки) и устанавливаются флаги
    je .end ; смотрим в флаг ZF если там 1 (значит у нас символ конца строки) тогда заканчиваем цикл
    inc rax ; учитываем символ и продолжаем цикл
    jmp .loop
.end:
    ret 
 
; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    xor rax, rax
    call string_length ; считываем дasmлину строки и помещаем его в rax
    mov rsi, rdi ; адрес строки
    mov rdx, rax ; количество байтов для вывода
    mov rdi, 1 ; дескриптор stdout (приемник (файл))
    mov rax, 1 ; номер системного вызова write
    syscall ; системный вызов (вызывается write)
    ret
    
; Принимает указатель на нуль-терминированную строку, выводит её в stderr
print_error:
    xor rax, rax
    call string_length
    mov rsi, rdi
    mov rdx, rax
    mov rdi, 2
    mov rax, 1
    syscall
    ret
 
; Принимает код символа и выводит его в stdout
print_char:
    ;xor rax, rax 
    ;push rdi ; положим символ в вершину стека
    ;mov rdi, rsp ; вынимаем адрес с которого надо читать (из стека) 
    ;call print_string ; выводем символ
    ;pop rdi ; вынемаем символ из стека
    ;ret
    
    ; Алтернативное решение
     xor rax, rax 
     push rdi ; положим символ в вершину стека
     mov rsi, rsp ; вынимаем адрес с которого надо читать (из стека) 
     mov rax, 1
     mov rdx, 1
     mov rdi, 1
     syscall
     pop rdi; вынемаем символ из стека
     ret
    
; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    xor rax, rax
    mov rdi, 10
    call print_char
    ret
 
; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    xor rax, rax
    xor r9, r9
    mov rax, rdi ; заносим значение в rax
    mov r9, rsp ; сохроним вершину стека чтобы потом востановить 
    push 0 ; это будет концом строки для вывода стека 
    mov rdi, rsp ; сохраняем вершину стека для переменных
    sub rsp, 20; максимум у нас будет 19 чисел и поэтому -20;
    mov r10, 10 ; делитель
    
.loop:
    xor rdx, rdx ; остаток от деления будет хранится в rdx, мы очищаем rdx для записи
    div r10 ; делим на 10
    add dl, 0x30 ; остаток + 0x30 = остаток в 10 сс
    dec rdi ; для адреса следующего числа
    mov [rdi], dl ; заносим в память
    test rax, rax ; смотрим закончились ли числа
    jnz .loop
    
    call print_string ; выводим 
    mov rsp, r9 ; возвращаем стек в первоначальное состояние
    ret
    
; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    xor rax, rax
    test rdi, rdi ; тест на то что является ли rdi отрицательным
    jns print_uint ; если нет вызовем предыдущую функцию
    push rdi ; сохраним число в стек
    mov rdi, '-' ; знак - 
    call print_char ; выводим знак -
    pop rdi ; достаем число из стека
    neg rdi ; сделаем число беззановым
    jmp print_uint ; вызываем предыдущую функцию
    ret
 
; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rax, rax
    mov al, [rdi] ; в al первый байт строки
    cmp al, [rsi] ; сравниваем первый байт первой и второй строки
    jne .unequal ; если они не равни возвращаем 1
    inc rdi ; переходим на следующий байт
    inc rsi ; переходим на следующий байт
    test al, al ; сравниваем если 0 возвращаем 1
    jne string_equals ; если нет то начинаем все с начала со следующего байта
    mov rax, 1 
    ret
    
.unequal:
    xor rax, rax
    ret
 
; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    xor rax, rax ; дескриптор системного вызова
    xor rdi, rdi ; дескриптор stdin
    mov rdx, 1 ; сколько байтов будем читать
    push 0 ; подготавливаем стек
    mov rsi, rsp ; где будем записовать
    syscall
    pop rax ; two birds with one stone
    ret 
 
; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор
 
read_word:
    xor rax, rax
    push r12
    push r13
    xor r12, r12
    xor r13, r13
    mov r13, rsi
    
.A:
    push rdi ; сохраняем состояние rdi, так как вызов read_char использует rdi
    call read_char ; читаем символ из stdin
    pop rdi ; возвращаем rdi
    cmp al, 0x9
    je .A       ; Проверяем на пробельные символы
    cmp al, 0xA
    je .A
    cmp al, 0x13
    je .A
    test al, al ; если конец строки переходим на С
    jz .C
    
.B:
    mov byte[rdi + r12], al ; Если прошел проверку читаем символ в память
    inc r12 ; счетчик адреса
    push rdi ; read_char использует rdi
    call read_char
    pop rdi
    cmp al, 0x9
    je .C       ; проверяем на пробельные символи если находим заканчиваем чтение символов
    cmp al, 0xA
    je .C
    cmp al, 0x13
    je .C
    test al, al
    jz .C
    cmp r13, r12 ; если символов много чем надо переходим в D
    je .D
    
    jmp .B
    
.C:
    mov byte[rdi + r12], 0 ; добавляем конец строки
    mov rax, rdi ; Возвращаем начало строки
    
    mov rdx, r12  ; Длина строки
    pop r13
    pop r12 ; Возвращаем r12, r13 в начальное состояние
    ret
    
.D:
    xor rax, rax
    pop r13
    pop r12
    ret
    
; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    xor rax, rax
    mov r8, 10 ; Значение для умножения
    xor rcx, rcx
    xor r9, r9
.loop:
    mov r9b, byte[rdi + rcx] ; читаем первый символ
    cmp r9b, 0x30 ; сравниваем с 0
    jb .end
    cmp r9b, 0x39 ; сравниваем с 9
    ja .end
    mul r8 ; умнажаем аккумулятор на 10
    sub r9b, 0x30 ; вычитаем 0 чтобы получить 0 - 9
    add rax, r9 ; добавляем в аккумулятор
    inc rcx ; счетчик
    jmp .loop
    ret 
.end:
    mov rdx, rcx ; возвращаем длину
    ret
 
; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    xor rax, rax
    mov al, byte[rdi] 
    cmp al, '-' ; смотрим является ли число знаковым или беззнаковым
    je .signed ; если знакковый => signed
    jmp parse_uint ; если беззнаковый
 
.signed: 
    inc rdi ; следующи символ за знаковым
    call parse_uint ; вызываем parce_uint
    neg rax  ; сделаем число отрицательным
    test rdx, rdx ; смотрим получилось ли прочитать число
    jz .error ; переходим в error 
    inc rdx ; включаем знак
    ret
 
.error:
    xor rax, rax
    ret
 
; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
   push rdi 
   push rsi ; сохраняем в стек, string_length использует эти регистры
   push rdx
   call string_length
   pop rdx
   pop rsi ; первоночальное состояние
   pop rdi
   
   cmp rax, rdx ; сравниваем с длинной буфера если если длиннее 
   jae .long
   push rsi
 
.loop: 
    mov dl, byte[rdi] ; читаем в dl - rdx
    mov byte[rsi], dl ; записываем в буфер
    inc rdi ; адрес следующего байта
    inc rsi ; адрес следующего байта
    test dl, dl ; конец строки
    jnz .loop
 
  pop rax
  ret
    
.long:
    xor rax, rax ; Возвращаем 0
    ret