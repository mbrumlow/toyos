[bits 32]

[global start]
[extern main]

start:

push ebp
mov ebp, esp
push eax
call main
add esp, 4
pop ebp
ret

