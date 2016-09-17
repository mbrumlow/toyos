
print_boot_status:

  push bx
  call clear_screen
  call reset_cursor
  pop bx

  asm_print_top:
  mov ah, 0x0e
  mov al, [bx]
  cmp al, 0
  je asm_print_end
  int 0x10
  add bx, 0x01
  jmp asm_print_top
  asm_print_end:
  call reset_cursor
  ret

