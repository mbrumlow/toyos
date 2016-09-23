[org 0x7c00]

STAGE1_OFFSET equ 0x1000               ; Offset to load kernel from.
KERNEL_OFFSET equ 0x8000               ; Offset to load kernel from.

  mov [BOOT_DRIVE], dl                 ; Sve boot drive from BIOS.

  mov bp, 0x1000                       ; Setup the stack.
  mov sp, bp

  call load_stage1                     ; Load kernel into memory.
  call load_kernel                     ; Load kernel into memory.

  call switch_to_pm                    ; Switch to protected mode, no return.

  jmp $

%include "boot/screen.asm"
%include "boot/print.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/print_pm.asm"
%include "boot/pm.asm"

[bits 16]

load_stage1:

  mov bx, STAGE1_OFFSET
  mov dh, STAGE1_SECTORS
  mov dl, [BOOT_DRIVE]
  mov cl, 0x02
  call disk_load

  ret

load_kernel:

  mov bx, KERNEL_OFFSET
  mov dh, KERNEL_SECTORS
  mov dl, [BOOT_DRIVE]
  mov cl, STAGE1_SECTORS
  add cl, 0x02
  call disk_load

  ret

[bits 32]
BEGIN_PM:

  mov ebx, MSG_PROT_MODE
  call print_string_pm

  mov eax, KERNEL_SECTORS

  call STAGE1_OFFSET
  call 0x4000000
  jmp $

; Global variables
BOOT_DRIVE          db 0
MSG_REAL_MODE       db "Started in 16-bit Real Mode.", 0
MSG_PROT_MODE       db "Successfully landed in 32 - bit Protected Mode.", 0
MSG_LOAD_KERNEL     db "Loading kernel into memory.", 0

; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55

