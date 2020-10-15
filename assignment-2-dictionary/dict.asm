global find_word ; глобальная функция (объявление)
extern string_equals ; функция из другого модуля
 
section .rodata
error: db "No such word in dictionary", 0
 
section .text
 
find_word:
    xor rax, rax ; обнуление
.loop:
    test rsi, rsi ; проверям словарь на пустоту
    jz .end ; если в словаре нету слов, завершаем
    push rdi ; положим в стек искомое слово
    push rsi ; положим в стек последнее слова в словаре
    add rsi, 8; пропускаем указатель на след. слово 
    call string_equals ; сравниваем искомое слово 
    ; востонавливаем значения ригистров
    pop rsi
    pop rdi
    test rax, rax ; проверяем что вернула функция
    jne .found ; если не 0, тогда слова совподают
    mov rsi, [rsi] ; берем следующее слова из словаря
    jmp .loop ; повторяем цикл
.found:
    mov rax, rsi; возвращаем ссылку на найденное слова
.end:
    ret ; выход
