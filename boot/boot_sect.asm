[org 0x7c00]

KERNEL_OFFSET equ 0x2000               ; Offset to load kernel from.

  mov [BOOT_DRIVE], dl                 ; Sve boot drive from BIOS.

  mov bp, 0x1000                       ; Setup the stack.
  mov sp, bp

  call init_screen                     ; Clear the screen an indicate we we

  mov bx, MSG_REAL_MODE                ; are in real mode.
  call print_boot_status

  mov bx, MSG_PROT_MODE                ; are in real mode.
  call print_boot_status

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

load_kernel:

  mov bx, MSG_LOAD_KERNEL
  call print_boot_status

  mov bx, KERNEL_OFFSET
  mov dh, KERNEL_SECTORS
  mov dl, [BOOT_DRIVE]
  call disk_load

  ret

[bits 32]
BEGIN_PM:

  mov ebx, MSG_PROT_MODE
  call print_string_pm

  mov edx, KERNEL_OFFSET
  call KERNEL_OFFSET

  jmp $

; Global variables
BOOT_DRIVE          db 0
MSG_REAL_MODE       db "Started in 16-bit Real Mode.", 0
MSG_PROT_MODE       db "Successfully landed in 32 - bit Protected Mode.", 0
MSG_LOAD_KERNEL     db "Loading kernel into memory.", 0

; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55

