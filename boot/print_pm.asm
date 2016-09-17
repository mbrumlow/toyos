[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
  pusha
  call print_clear_line
  mov edx, VIDEO_MEMORY

print_string_pm_loop:
  mov al, [ebx]
  mov ah, WHITE_ON_BLACK

  cmp al, 0
  je print_string_pm_done

  mov [edx], ax

  add ebx, 1
  add edx, 2

  jmp print_string_pm_loop

print_string_pm_done:
  popa
  ret

print_clear_line:

  mov edx, VIDEO_MEMORY
  mov ecx, 0x0
  .clear:
  mov al, 0x0
  mov ah, WHITE_ON_BLACK
  mov [edx], ax
  add edx, 2
  inc ecx
  cmp ecx, 80
  je .clear_end
  jmp .clear
  .clear_end:
  ret


