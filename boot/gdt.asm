; GGT
gdt_start:

gdt_null: ; null descriptor
  dd 0x0
  dd 0x0

gdt_code: ; code segment
  ; base=0x0, limit=0xfffff,
  ; 1st flags: ( present )1 ( privilege )00 ( descriptor type )1 -> 1001 b
  ; type flags : ( code )1 ( conforming )0 ( readable )1 ( accessed )0 -> 1010 b
  ; 2nd flags : ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100 b
  dw 0xffff     ; Limit ( bits 0 -15)
  dw 0x0        ; Base ( bits 0 -15)
  db 0x0        ; Base ( bits 16 -23)
  db 10011010b  ; 1st flags , type flags
  db 11001111b  ; 2nd flags , Limit ( bits 16 -19)
  db 0x0        ; Base ( bits 24 -31)

gdt_data: ; data segment
  ; Same as code segment except for the type flags :
  ; type flags : ( code )0 ( expand down )0 ( writable )1 ( accessed )0 -> 0010 b
  dw 0xffff     ; Limit ( bits 0 -15)
  dw 0x0        ; Base ( bits 0 -15)
  db 0x0        ; Base ( bits 16 -23)
  db 10010010b  ; 1st flags , type flags
  db 11001111b  ; 2nd flags , Limit ( bits 16 -19)
  db 0x0        ; Base ( bits 24 -31)

gdt_end:

gdt_descriptor:
; GDT descroptor
  dw gdt_end - gdt_start -1 ; GDT is always less one of the start address
  dd gdt_start

; Global constants
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

