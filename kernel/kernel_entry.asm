[bits 32]

[global start]
[extern end]
[extern main]

start:

push eax
call main

jmp $

