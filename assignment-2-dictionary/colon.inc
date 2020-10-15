%define last_word 0
 
%macro colon 2
%%last_word: dq last_word
db %1, 0
v_ %+ %2:
 
%define last_word %%last_word
%endmacro

