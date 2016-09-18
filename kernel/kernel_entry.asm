[bits 32]

[global start]
[extern end]
[extern main]

start:

;mov esp, stack_top                     ; Setup stack.

push end
push start
call main

jmp $

